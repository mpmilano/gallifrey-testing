#include <cstdio>
#include <cstdlib>
#include <ext/stdio_filebuf.h>
#include <functional>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

class ChildProcess {
  struct initializer {
    int use_for_out;
    int use_for_in;
    pid_t pid;
    template <typename... Args>
    initializer(std::string pname, Args &&... pargs) {
      using namespace std;
      int in_fd[2];
      int out_fd[2];
      pipe(in_fd);
      pipe(out_fd);
      pid = fork();
      if (pid == 0) {
        close(out_fd[0]);
        dup2(out_fd[1], STDOUT_FILENO);
        close(out_fd[1]);

        close(in_fd[1]);
        dup2(in_fd[0], STDIN_FILENO);
        close(in_fd[0]);
        execlp(pname.c_str(), pname.c_str(), std::string{pargs}.c_str()...,
               (char *)NULL);
        throw "I thought this would end";
      } else if (pid == -1) {
        throw pid;
      } else {
        use_for_out = in_fd[1];
        use_for_in = out_fd[0];
      }
    }
  };

  __gnu_cxx::stdio_filebuf<char> outbuf;
  __gnu_cxx::stdio_filebuf<char> inbuf;
  const pid_t child_pid;
  ChildProcess(initializer i)
      : outbuf(i.use_for_out, std::ios::out), inbuf(i.use_for_in, std::ios::in),
        child_pid(i.pid),
        block_until_dead([pid = i.pid, in = i.use_for_out, out = i.use_for_in] {
          int status;
          waitpid(pid, &status, 0);
          close(in);
          close(out);
        }) {}

public:
  ~ChildProcess() {
    outbuf.close();
    inbuf.close();
  }
  std::ostream out{&outbuf};
  std::istream in{&inbuf};
  template <typename... Args>
  ChildProcess(std::string process_name, Args &&... args)
      : ChildProcess(initializer(process_name, std::forward<Args>(args)...)) {}

  std::string read_to_string() {
    using namespace std;
    return string{(istreambuf_iterator<char>(in)), istreambuf_iterator<char>()};
  }

  bool child_alive() {
    int status;
    return waitpid(child_pid, &status, WNOHANG) == 0;
  }

  const std::function<void()> block_until_dead;
};

int main() {
  using namespace std;
  std::cout << "running java" << std::endl;
  ChildProcess java{"/usr/bin/java", "Stdin"};
  std::cout << "java started" << std::endl;
  std::cout << "bufs constructed" << std::endl;
  int num_java_pokes = 1232;
  for (int i = 0; i < num_java_pokes; ++i) {
    java.out.put(0);
    java.out.put(0);
    java.out.flush();
  }
  java.out.put(1);
  java.out.flush();
  std::cout << "java says: " << std::endl;
  std::string line;
  while (java.child_alive() && line != "%" && std::getline(java.in, line)) {
    std::cout << line << std::endl;
  }
  java.block_until_dead();
}
