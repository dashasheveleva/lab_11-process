// Copyright 2022 Shevelyova Darya photodoshfy@gmail.com

#include <timer.hpp>

Timer::Timer(Timer&& t)   {
  _timer_thread = std::move(t._timer_thread);
}

Timer::~Timer(){
  if (_timer_thread.joinable()) _timer_thread.detach();
}
