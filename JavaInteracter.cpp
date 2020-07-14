#include <cstdio>
#include <cstdlib>
#include <ext/stdio_filebuf.h>
#include <functional>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include "ChildProcess.hpp"



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
