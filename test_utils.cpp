#include "test_utils.hpp"

namespace mutils {

int get_name_read(double alpha) {
  constexpr int max = name_max;
  auto ret = get_zipfian_value(max, alpha);
  if (ret > (max + 14)) {
    std::cerr << "Name out of range! Trying again" << std::endl;
    return get_name_read(alpha);
  } else
    return ret + 14;
}

int get_name_write() { return 14 + int_rand() % name_max; }

// I'm guessing miliseconds.  Here's hoping!
std::chrono::microseconds getArrivalInterval(Frequency arrival_rate) {
  using namespace std;
  using namespace chrono;
  // exponential
  constexpr double thousands = -1000000.0;
  double U = better_rand();
  double T = thousands * log(U) / arrival_rate.hertz;
  unsigned long l = round(T);
  return microseconds(l);
}

unsigned int get_zipfian_value(unsigned int max, double alpha) {
  double y = better_rand();
  assert(y < 1.1);
  assert(y > -0.1);
  double max_pow = pow(max, 1 - alpha);
  double x = pow(max_pow * y, 1 / (1 - alpha));
  return round(x);
}

/*inline int get_strong_ip() {
        static int ip_addr{[](){
        std::string static_addr {STRONG_REMOTE_IP};
                        if (static_addr.length() == 0) static_addr =
   "127.0.0.1"; return mutils::decode_ip(static_addr);
                }()};
        return ip_addr;
        }*/

} // namespace mutils
