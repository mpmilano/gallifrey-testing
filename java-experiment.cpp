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
    java->out.put(1);
    java->out.put(1);
  }

  std::unique_ptr<testing::run_result> &
  action(std::unique_ptr<testing::run_result> &result) {
    java->out.put(0);
    java->out.put(0);
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
  return i.action(result);
}

} // namespace testing

int main(int argc, char **argv) {
  assert(argc > 1);
  java_name = argv[1];
  testing::configuration_parameters params;
  std::cin >> params;
  std::cout << "testing params: " << params << std::endl;
  environment_overrides hardcoded_envs[4];
  hardcoded_envs[0].overrides["ANTIDOTE_HOST"] = "pinky11.mpi-sws.org";
  hardcoded_envs[0].overrides["ANTIDOTE_BACKEND"] = "rmi://pinky11.mpi-sws.org/JavaBackend";
  hardcoded_envs[1].overrides["ANTIDOTE_HOST"] = "pinky13.mpi-sws.org";
  hardcoded_envs[1].overrides["ANTIDOTE_BACKEND"] = "rmi://pinky13.mpi-sws.org/JavaBackend";
  hardcoded_envs[2].overrides["ANTIDOTE_HOST"] = "pinky14.mpi-sws.org";
  hardcoded_envs[2].overrides["ANTIDOTE_BACKEND"] = "rmi://pinky14.mpi-sws.org/JavaBackend";
  hardcoded_envs[3].overrides["ANTIDOTE_HOST"] = "pinky19.mpi-sws.org";
  hardcoded_envs[3].overrides["ANTIDOTE_BACKEND"] = "rmi://pinky19.mpi-sws.org/JavaBackend";
  for (auto i = 0u; i < params.max_clients(); ++i){
	  javap.enqueue(std::make_unique<ChildProcess>(hardcoded_envs[i % 4], "java", "-cp", "/home/isheff/Documents/gallifrey/gallifreyc/tests/out:/home/isheff/Documents/gallifrey/gallifrey-antidote/full-runtime.jar", java_name, "/home/isheff/Documents/gallifrey/gallifrey-testing/shared-counter", "/local/isheff/logs/experiment-2020-07-14-18:30:01/client-" + std::to_string(i) + ".log"));
  }
  test t1{params};
  std::unique_ptr<testing::run_result> nullp;
  std::cout << "running initial action" << std::endl;
  // child_process_test{}.action(nullp);
  t1.run_test();
  exit(0);
}
