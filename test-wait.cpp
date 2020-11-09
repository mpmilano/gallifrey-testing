#include <future>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>



int main(){
    using namespace std;
    using namespace chrono;
    pid_t pid = fork();
    if (pid == 0){
	this_thread::sleep_for(20s);
	/*child */
    }
    else {
	std::cout << "starting wait" << std::endl;
	auto before_wait = system_clock::now();
	wait_for_child(pid,5s);
	auto after_wait = system_clock::now();
	std::cout << "ending wait after " << duration_cast<seconds>(after_wait - before_wait).count() << " seconds" << std::endl;
	kill(pid,SIGTERM);
    }
}
