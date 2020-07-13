#pragma once
#include "mutils/Hertz.hpp"
#include "mutils/mutils.hpp"
#include <cmath>

namespace mutils {

// I'm guessing miliseconds.  Here's hoping!
std::chrono::microseconds getArrivalInterval(Frequency arrival_rate);

unsigned int get_zipfian_value(unsigned int max, double alpha);

template <typename A, typename B>
auto micros(std::chrono::duration<A, B> time) {
  using namespace std::chrono;
  return duration_cast<microseconds>(time).count();
}

using time_point =
    std::decay_t<decltype(std::chrono::high_resolution_clock::now())>;

constexpr int name_max = 478446;

int get_name_read(double alpha);

int get_name_write();

} // namespace mutils
