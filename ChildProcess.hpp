#pragma once

#include <cstdio>
#include <ext/stdio_filebuf.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <map>
#include <string>
#include <vector>

struct environment_overrides {
  std::map<std::string, std::string> overrides;

public:
  environment_overrides() = default;
  environment_overrides(const std::string &filename);
  environment_overrides(std::istream &read);
};

std::vector<environment_overrides> envs_from_file(const std::string &filename);

class ChildProcess {
  struct initializer {
    int use_for_out;
    int use_for_in;
    pid_t pid;
    initializer(const environment_overrides &e, const std::string &pname,
                const std::vector<std::string> &pargs);
  };

  __gnu_cxx::stdio_filebuf<char> outbuf;
  __gnu_cxx::stdio_filebuf<char> inbuf;
  const pid_t child_pid;
  ChildProcess(initializer i);

public:
  ~ChildProcess();
  std::ostream out{&outbuf};
  std::istream in{&inbuf};

  template <typename T>
  ChildProcess(const environment_overrides &e, const std::string &process_name,
               const std::initializer_list<T> &args)
      : ChildProcess(initializer(e, process_name, args)) {}

private:
  template <typename T>
  static std::vector<std::string> construct_map(const std::vector<T> &out_of) {
    std::vector<std::string> into;
    for (const auto &t : out_of)
      into.emplace_back(t);
    return into;
  }

public:
  using cstr_t = const char *;

  inline ChildProcess(const environment_overrides &e,
                      const std::string &process_name, cstr_t const *const argv,
                      std::size_t argc)
      : ChildProcess(initializer(
            e, process_name,
            construct_map(std::vector<const char *>(argv, argv + argc)))) {}

  std::string read_to_string();

  bool child_alive();

  const std::function<void()> block_until_dead;
};
