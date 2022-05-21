// Copyright 2022 Shevelyova Darya photodoshfy@gmail.com

#ifndef INCLUDE_BUILDER_HPP_
#define INCLUDE_BUILDER_HPP_

#include <async++.h>
#include <pthread.h>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/process.hpp>
#include <boost/program_options.hpp>
#include <future>
#include <iostream>
#include <string>

#include <process.hpp>
#include <timer.hpp>

namespace logging = boost::log::trivial;

const char error[] = "Error"
    "Write to get Help: --help or -h";

class Builder {
 private:
  Process* p_process;

 public:
  // выводим все возможные команды
  static void create_program_options(
      boost::program_options::options_description& desc,
      boost::program_options::variables_map&vmap,
      const int& argc,
      const char *argv[]);

  void start(const boost::program_options::variables_map& vmap);

  void init(const logging::severity_level& sev_lvl);

  logging::severity_level choose_sev_lvl(const std::string&sev_lvl_str);

  void settings_process(const boost::program_options::variables_map& vmap);

  bool run_process(const std::string& target, Process_info& process_info);

  ~Builder();
};

#endif // INCLUDE_BUILDER_HPP_
