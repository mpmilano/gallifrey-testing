#include <cstdio>
#include <ext/stdio_filebuf.h>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

class ChildProcess {
  struct initializer {
    int use_for_out;
    int use_for_in;
    pid_t pid;
      initializer(std::string pname, std::vector<std::string> pargs);
  };

  __gnu_cxx::stdio_filebuf<char> outbuf;
  __gnu_cxx::stdio_filebuf<char> inbuf;
  const pid_t child_pid;
    ChildProcess(initializer i);


public:
    ~ChildProcess();
  std::ostream out{&outbuf};
  std::istream in{&inbuf};
  template <typename... Args>
  ChildProcess(std::string process_name, Args &&... args)
      : ChildProcess(initializer(process_name, {std::forward<Args>(args)...})) {}

    std::string read_to_string();

    bool child_alive();

  const std::function<void()> block_until_dead;
};
