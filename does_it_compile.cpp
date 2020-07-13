#include "test.hpp"



struct empty_test {
  template <typename... T> empty_test(const T &...) {}
};

using test = testing::test<empty_test>;

using client = testing::client<empty_test>;

namespace testing {

    template <>
    std::unique_ptr<run_result> &
    testing::client<empty_test>::client_action(std::unique_ptr<run_result> &result) {
	result.reset(new run_result());
	return result;
    }

}

int main(int argc, char **argv) {
  testing::configuration_parameters params;
  std::cout << "testing params: " << params << std::endl;
  test t1{params};
  t1.run_test();
  exit(0);
}
