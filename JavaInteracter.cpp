#include <cstdio>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <ext/stdio_filebuf.h>

class ChildProcess{
    struct initializer{
	int use_for_out;
	int use_for_in;
	initializer(std::string process_name){
	    using namespace std;
	    int in_fd[2];
	    int out_fd[2];
	    pid_t pid;
	    pipe(in_fd);
	    pipe(out_fd);
	    pid = fork();
	    if (pid == 0){
		close(out_fd[0]);
		dup2(out_fd[1], STDOUT_FILENO);
		close(out_fd[1]);
		
		close(in_fd[1]);
		dup2(in_fd[0], STDIN_FILENO);
		close(in_fd[0]);
		execl("/usr/bin/java", "Stdin", (char *) NULL);
		throw "I thought this would end";
	    }
	    else if (pid == -1){
		throw pid;
	    }
	    else {
		use_for_out = in_fd[1];
		use_for_in = out_fd[0];
	    }
	}
    };

    __gnu_cxx::stdio_filebuf<char> outbuf;
    __gnu_cxx::stdio_filebuf<char> inbuf;
    
    ChildProcess(initializer i):
	outbuf(i.use_for_out,std::ios::out),
	inbuf(i.use_for_in, std::ios::in) {}
    
public:
    std::ostream java_out{&outbuf};
    std::istream java_in{&inbuf};
    ChildProcess(std::string process_name):
	ChildProcess(initializer(process_name)){}
};

int main(){
    using namespace std;
    std::cout << "running java" << std::endl;
    FILE* fhandle = popen("java Stdin", "w");
    std::cout << "java started" << std::endl;
    __gnu_cxx::stdio_filebuf<char> gnubuf(fhandle, std::ios::out);
    std::basic_filebuf<char> *bbuf = &gnubuf;
    std::ostream java_out(bbuf);
    std::cout << "bufs constructed" << std::endl;
    int num_java_pokes = 1232;
    for (int i = 0; i < num_java_pokes; ++i){
	java_out.put(0);
	java_out.put(0);
	java_out.flush();
    }
    java_out.put(1);
    java_out.flush();
    std::cout << "closing stream" << std::endl;
    pclose(fhandle);
}



