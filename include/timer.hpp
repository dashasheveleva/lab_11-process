// Copyright 2022 Shevelyova Darya photodoshfy@gmail.com

#include <process.hpp>
#include <string>
#include <utility>

#ifndef INCLUDE_TIMER_HPP_
#define INCLUDE_TIMER_HPP_

class Timer{
 private:
  std::thread _timer_thread;

 public:
  Timer(Timer&& t) ;
  Timer(std::chrono::seconds delay,
        std::function<void(Process_info&)> callback_obj,
        Process_info& pdata)
      : _timer_thread([delay, callback_obj, &pdata]() {
          std::this_thread::sleep_for(std::chrono::seconds(delay));
          callback_obj(pdata);
        }) {}
  ~Timer() ;
};

#endif  // INCLUDE_TIMER_HPP_
