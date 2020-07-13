#include "test.hpp"
#include <thread>

using namespace std::chrono;

struct empty_test {
  template <typename... T> empty_test(const T &...) {}
};

using test = testing::test<empty_test>;

using client = testing::client<empty_test>;

namespace testing {

template <>
std::unique_ptr<run_result> &testing::client<empty_test>::client_action(
    std::unique_ptr<run_result> &result) {
	std::this_thread::sleep_for(3ms);
  return result;
}

} // namespace testing

int main(int argc, char **argv) {
  testing::configuration_parameters params;
  std::cin >> params;
  std::cout << "testing params: " << params << std::endl;
  test t1{params};
  t1.run_test();
  exit(0);
}
