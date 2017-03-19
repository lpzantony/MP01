#include <stdio.h>
#include <signal.h>
#include <iostream>
#include <mutex>
#include "Benchmark.h"


Benchmark * bench;
std::mutex mut;

void handle_sig(int sig)
{
	//printf("Received signal : %i\n",sig);
	std::cerr << std::endl <<"Received signal : " << std::to_string(sig)<<std::endl;
	bench->shutdown();
	exit(-1);
}



int main(int argc, char **argv)
{
	mut.lock();
	bench = new Benchmark(&mut);
	signal(SIGINT, handle_sig);
	signal(SIGABRT, handle_sig);
	signal(SIGSEGV, handle_sig);
	printf("ENTER: Starting Teemo\n");
	bench->ConsoleRoutine();


	std::cout << "EXIT: Ending IRCLIENT\n";
	return 0;
}

