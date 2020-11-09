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
    if (mutils::better_rand() < read_percent) {
      if (result)
        result->is_write = false;
      java->out.put(1);
    } else {
      if (result)
        result->is_write = true;
      java->out.put(0);
    }
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
  // args: arg1: environment file.  args 2-∞: passed to child process.
  assert(argc > 2);

  java_name = argv[2];
  testing::configuration_parameters params;
  std::cin >> params;
  std::cout << "testing params: " << params << std::endl;
  auto hardcoded_envs = envs_from_file(argv[1]);
  std::cout << "loaded " << hardcoded_envs.size() << " envs" << std::endl;
  if (hardcoded_envs.size() == 0)
    hardcoded_envs.emplace_back();
  for (auto i = 0u; i < params.max_clients(); ++i) {
    auto this_env = hardcoded_envs[i % hardcoded_envs.size()];
    this_env.overrides.emplace("TASK_ID", std::to_string(i));
    javap.enqueue(
        std::make_unique<ChildProcess>(this_env, argv[2], argv + 3, argc - 3));
  }
  test t1{params};
  std::unique_ptr<testing::run_result> nullp;
  std::cout << "running initial action" << std::endl;
  // child_process_test{}.action(nullp);
  t1.run_test();
}
