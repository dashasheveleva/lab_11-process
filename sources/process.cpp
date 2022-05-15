// Copyright 2022 Shevelyova Darya photodoshfy@gmail.com

#include <process.hpp>

Process::Process(std::string config_, bool install_, bool pack_, int time_) {
  config = std::move(config_);
  install = install_;
  pack = pack_;
  time = time_;
}

bool Process::get_pack() const {
  return pack;
}

bool Process::get_install() const {
  return install;
}

int Process::get_time() const {
  return time;
}

std::string Process::get_command(const std::string& target){
  if (target == "config") {
    return (" -H. -B" + BUILD_TARGET + " -DCMAKE_INSTALL_PREFIX=" +
            INSTALL_TARGET + " -DCMAKE_BUILD_TYPE=" + config);
  } else {
    return ("--build " + BUILD_TARGET +
            (target != "build" ? (" --target " + target) : ""));
  }
}
