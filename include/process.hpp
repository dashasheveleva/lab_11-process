// Copyright 2022 Shevelyova Darya photodoshfy@gmail.com

#include <builder.hpp>
#include <string>
#include <utility>

#ifndef INCLUDE_PROCESS_HPP_
#define INCLUDE_PROCESS_HPP_

struct Process_info{
  Process_info(bool term, boost::process::child&& child) {
    terminated.store(term);
    current_child = std::move(child);
  }

  void set_bool(bool term) { terminated.store(term); }
  void set_child(boost::process::child&& child) {
    current_child = std::move(child);
  }

  std::atomic_bool terminated = false;
  boost::process::child current_child;
};

class Process{
 private:
  std::string config;
  bool install, pack;
  int time = 0;

 public:
  const std::string BUILD_TARGET = "_builds";
  const std::string INSTALL_TARGET = "_install";

  Process(std::string config_, bool install_, bool pack_, int time_);

  bool get_pack() const;

  bool get_install() const ;

  int get_time() const ;

  std::string get_command(const std::string& target);

};

#endif  // INCLUDE_PROCESS_HPP_
