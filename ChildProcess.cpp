#include "ChildProcess.hpp"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ext/stdio_filebuf.h>
#include <functional>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

using std::string;

ChildProcess::initializer::initializer(const environment_overrides &e,
                                       const std::string &pname,
                                       std::vector<string> pargs) {
  assert(pargs.size() < 7);
  using namespace std;

  struct env_manip {
    const environment_overrides &e;
	  std::map<string, string> oe;
    env_manip(const environment_overrides &e) : e(e) {
      for (const auto &p : e.overrides) {
        oe[p.first] = getenv(p.second.c_str());
        setenv(p.first.c_str(),p.second.c_str(),1);
      }
    }
    ~env_manip() {
      for (const auto &p : oe) {
	      setenv(p.first.c_str(),p.second.c_str(),1);
      }
    }
  };

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
    switch (pargs.size()) {
    case 0:
      execlp(pname.c_str(), pname.c_str(), (char *)NULL);
      break;
    case 1:
      execlp(pname.c_str(), pname.c_str(), pargs[0].c_str(), (char *)NULL);
      break;
    case 2:
      execlp(pname.c_str(), pname.c_str(), pargs[0].c_str(), pargs[1].c_str(),
             (char *)NULL);
      break;
    case 3:
      execlp(pname.c_str(), pname.c_str(), pargs[0].c_str(), pargs[1].c_str(),
             pargs[2].c_str(), (char *)NULL);
      break;
    case 4:
      execlp(pname.c_str(), pname.c_str(), pargs[0].c_str(), pargs[1].c_str(),
             pargs[2].c_str(), pargs[3].c_str(), (char *)NULL);
      break;
    case 5:
      execlp(pname.c_str(), pname.c_str(), pargs[0].c_str(), pargs[1].c_str(),
             pargs[2].c_str(), pargs[3].c_str(), pargs[4].c_str(),
             (char *)NULL);
      break;
    case 6:
      execlp(pname.c_str(), pname.c_str(), pargs[0].c_str(), pargs[1].c_str(),
             pargs[2].c_str(), pargs[3].c_str(), pargs[4].c_str(),pargs[5].c_str(),
             (char *)NULL);
      break;
    default:
      break;
    }

    throw "I thought this would end";
  } else if (pid == -1) {
    throw pid;
  } else {
    use_for_out = in_fd[1];
    use_for_in = out_fd[0];
  }
}

ChildProcess::ChildProcess(initializer i)
    : outbuf(i.use_for_out, std::ios::out), inbuf(i.use_for_in, std::ios::in),
      child_pid(i.pid),
      block_until_dead([pid = i.pid, in = i.use_for_out, out = i.use_for_in] {
        int status;
        waitpid(pid, &status, 0);
        close(in);
        close(out);
      }) {}

ChildProcess::~ChildProcess() {
  outbuf.close();
  inbuf.close();
}

std::string ChildProcess::read_to_string() {
  using namespace std;
  return string{(istreambuf_iterator<char>(in)), istreambuf_iterator<char>()};
}

bool ChildProcess::child_alive() {
  int status;
  return waitpid(child_pid, &status, WNOHANG) == 0;
}
