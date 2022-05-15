// Copyright 2022 Shevelyova Darya photodoshfy@gmail.com

#include <builder.hpp>

void Builder::create_program_options(
    boost::program_options::options_description& desc,
    boost::program_options::variables_map& vmap,
    const int& argc, const char** argv) {
  desc.add_options()
      ("help,h", "Help screen\n")
      ("log_lvl,l", boost::program_options::value<std::string>()->
           default_value("Debug"),
           "Logger severity\n")
      ("config,c", boost::program_options::value<std::string>()->
          default_value("Debug"),
          "Config build\n")
      ("install,i", "Install step\n")
      ("pack,p", "Pack step\n")
      ("timeout,t", boost::program_options::value<int>()->
          default_value(0),
          "Set waiting time\n");
  store(parse_command_line(argc, argv, desc), vmap);
  notify(vmap);
}

void time_handler(Process_info&);

void Builder::start(const boost::program_options::variables_map& vm) {
  init(choose_sev_lvl(vm["log_lvl"].as<std::string>()));

  settings_process(vm);

  Process_info process_info{false, boost::process::child()};

  std::unique_ptr<Timer> _timer;
  if (p_process->get_time() != 0) {
    _timer = std::make_unique<Timer>(Timer(
        std::chrono::seconds(p_process->get_time()),
        time_handler,
        process_info));
  }
  try{
    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::shared_future<bool> pack = std::async([this, &process_info]() ->
                                               bool {return this->run_process(
                                                     "config",
                                                     process_info);
                                               });

    pack.wait();
    if (pack.get()) {
      pack = std::async([this, &process_info]() -> bool {
        return this->run_process("build", process_info);
      });
      pack.wait();
    }
    if (p_process->get_install()) {
      if (pack.get()) {
        pack = std::async([this, &process_info]() -> bool {
          return this->run_process("install", process_info);
        });
        pack.wait();
      }
    }
    if (p_process->get_pack()) {
      if (pack.get()) {
        pack = std::async([this, &process_info]() -> bool {
          return this->run_process("package", process_info);
        });
        pack.wait();
      }
    }
  } catch (const std::exception& e){
    BOOST_LOG_TRIVIAL(error) << "Error in processing: " << e.what();
  }
}
bool Builder::run_process(const std::string& target, Process_info& process_info)
{
  if (process_info.terminated) {
    return false;
  }
  boost::process::ipstream stream;
  auto cmake_path = boost::process::search_path("cmake");
  BOOST_LOG_TRIVIAL(info) << "Found cmake: " << cmake_path.string();
  boost::process::child child(
      std::string(cmake_path.string() + " " + p_process->get_command(target)),
      boost::process::std_out > stream);
  process_info.set_bool(false);
  process_info.set_child(std::move(child));
  for (std::string line;
       process_info.current_child.running() && std::getline(stream,
                                                            line);) {
    BOOST_LOG_TRIVIAL(info) << line;
  }
  process_info.current_child.wait();

  auto exit_code = process_info.current_child.exit_code();

  if (exit_code != 0) {
    BOOST_LOG_TRIVIAL(error) << "Non zero exit code. Exiting...";
    process_info.set_bool(true);
    return false;
  } else {
    return true;
  }
}
void Builder::init(const logging::severity_level& sev_lvl) {
  boost::log::add_common_attributes();

  boost::log::core::get()->set_filter(boost::log::trivial::severity >=
                                      sev_lvl);

  boost::log::add_console_log(std::clog,
                              boost::log::keywords::format =
                                  "[%Severity%] %TimeStamp%: %Message%");
}
logging::severity_level Builder::choose_sev_lvl(
    const std::string& sev_lvl_str) {
  if (sev_lvl_str == "trace")
    return logging::severity_level::trace;
  else if (sev_lvl_str == "debug")
    return logging::severity_level::debug;
  else if (sev_lvl_str == "info")
    return logging::severity_level::info;
  else if (sev_lvl_str == "warning")
    return logging::severity_level::warning;
  else
    return logging::severity_level::error;
}
void Builder::settings_process(const boost::program_options::variables_map&vmap)
{
  bool install = false, pack = false;
  std::string config = "Debug";
  int time = 0;
  if (vmap.count("config")) {
    config = vmap["config"].as<std::string>();
  }
  if (vmap.count("install")) {
    install = true;
  }
  if (vmap.count("packege")) {
    pack = true;
  }
  if (vmap.count("timeout")) {
    BOOST_LOG_TRIVIAL(debug)
        << "Timeout args got: "
        << vmap["timeout"].as<int>()
        << ". Setting timer";
    time = vmap["timeout"].as<int>();
  }
  p_process = new Process(config, install, pack, time);
}
Builder::~Builder() {
  delete p_process;
}
void time_handler(Process_info& process_info) {
  BOOST_LOG_TRIVIAL(debug) << "Timer timeout. Stopping all child processes...";
  try {
    if (process_info.current_child.running()) {
      process_info.current_child.terminate();
    }
    process_info.set_bool(true);
    BOOST_LOG_TRIVIAL(debug) << "_pdata set: " << process_info.terminated;
  } catch (const std::exception& e) {
    BOOST_LOG_TRIVIAL(fatal) << "Terminating error: " << e.what()
                             << " Process: " << process_info.current_child.id();
  }
}
