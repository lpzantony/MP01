/*
 * Benchmark.h
 *
 *  Created on: 2 mars 2017
 *      Author: rubisoft
 */

#ifndef CORE_BENCHMARK_H_
#define CORE_BENCHMARK_H_

#include <mutex>
//Pour le shell
#include "core/Shell.h"
#include "network/TCPClient.h"
#include "EventListener.h"
#include "messages/Msg.h"
using namespace std;
using MsgListener = EventListener<message_header_t> ;
#define SERVER_IP  "192.168.1.78"
#define DEFAULT_PORT 12345

class Benchmark : MsgListener {
public:


	Benchmark(std::mutex * locked_mutex);
	virtual ~Benchmark();
	void ConsoleRoutine();
	void shutdown(){run->unlock(); run_bool = false;}

	void connectClient		(ConstStrVect& args, OutputStream& output);
	void disconnectClient	(ConstStrVect& args, OutputStream& output);
	void exit_from_shell	(ConstStrVect& args, OutputStream& output);
	void sendNick			(ConstStrVect& args, OutputStream& output);
	void pingServer			(ConstStrVect& args, OutputStream& output);
	void msg				(ConstStrVect& args, OutputStream& output);
	void join				(ConstStrVect& args, OutputStream& output);

	void notify(std::vector<std::string> body, message_header_t head);
private:
	std::string nick = "noname";
	std::string pending_chan = "";
	std::string chan = "";
	TCPClient client;
	Shell shell;
	std::mutex * run;
	bool printing = false;
	bool run_bool = false;
};

#endif /* CORE_BENCHMARK_H_ */
