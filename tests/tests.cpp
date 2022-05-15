// Copyright 2022 Shevelyova Darya photodoshfy@gmail.com

#include <gtest/gtest.h>
#include <stdexcept>
#include <thread>

TEST(DISABLED_Snapshot, Speen) {
  for (;;) std::this_thread::yield();
}
