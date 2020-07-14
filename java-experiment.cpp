#include "ChildProcess.hpp"
#include "mutils-tasks/blockingconcurrentqueue.h"
#include "test.hpp"
#include <thread>

using namespace std::chrono;

std::string java_name;

moodycamel::BlockingConcurrentQueue<std::unique_ptr<ChildProcess>> javap;

struct child_process_test {
  std::unique_ptr<ChildProcess> java;
  template <typename... T> child_process_test(const T &...) {
    javap.wait_dequeue(java);
  }
  ~child_process_test() {
      java->out.put(2);
      java->out.put(2);
  }

  std::unique_ptr<testing::run_result> &
  action(std::unique_ptr<testing::run_result> &result, double read_percent) {
      if (mutils::better_rand() < read_percent)
	  java->out.put(1);
      else java->out.put(0);
    java->out.flush();
    char recv;
    java->in >> recv;
    assert(recv == 0);
    return result;
  }
};

using test = testing::test<child_process_test>;

using client = testing::client<child_process_test>;

namespace testing {

template <>
std::unique_ptr<run_result> &testing::client<child_process_test>::client_action(
    std::unique_ptr<run_result> &result) {
    return i.action(result, read_percent);
}

} // namespace testing

int main(int argc, char **argv) {
  assert(argc > 1);
  java_name = argv[1];
  testing::configuration_parameters params;
  std::cin >> params;
  std::cout << "testing params: " << params << std::endl;
  for (auto i = 0u; i < params.max_clients(); ++i)
	  javap.enqueue(std::make_unique<ChildProcess>(environment_overrides{}, "java", java_name));
  test t1{params};
  std::unique_ptr<testing::run_result> nullp;
  std::cout << "running initial action" << std::endl;
  // child_process_test{}.action(nullp);
  t1.run_test();
  exit(0);
}
