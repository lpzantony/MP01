/*
 * Benchmark.cpp
 *
 *  Created on: 2 mars 2017
 *      Author: rubisoft
 */

#include <core/Benchmark.h>

//=====> includes et defines pour TeemoRoutine
//pour le lock de socket
#include <fcntl.h>
//pour strcmp, strcpy...
#include <cstring>
#include <errno.h>
#include<iostream>
#include <string>

//std::bind
#include <functional>
#include "messages/NickMsg.h"
#include "messages/AckNickMsg.h"
#include "messages/TextMsg.h"
#include "messages/PingMsg.h"
#include "messages/AckJoinMsg.h"
#include "messages/JoinMsg.h"


Benchmark::Benchmark(std::mutex * locked_mutex)
{
	if(locked_mutex ==nullptr) throw runtime_error("Cant build benchmark without allocated mutex!");
	if(locked_mutex->try_lock()){
		locked_mutex->unlock();
		throw runtime_error("Can't build benchmark. The given mutex must be locked (std::mutex.lock() )before calling Benchmark constructor.");
	}
	 this->run = locked_mutex;
	 client.addObserver(this);
}

Benchmark::~Benchmark() {
	shutdown();
}

void echofct(ConstStrVect& args, OutputStream& output){
	for(auto i : args){
		output << i << ", ";
	}
	output << endl;
}

void Benchmark::exit_from_shell(ConstStrVect& args, OutputStream& output) {
	client.disconnect();
	shutdown();
}

void Benchmark::connectClient(ConstStrVect& args, OutputStream& output){
	if(args.size() < 4){
		std::cout << "Bad command usage : \"connect ipadress port nickname\"" << std::endl;
		return;
	}
	if(!client.connect(args[1], std::stoi(args[2])))
		std::cout << "Connected to "<< client.getClientAddr() << ", port " << client.getClientPort()<< std::endl;
	else{
		std::cout << "Could not connect"<< std::endl; return;
	}
	NickMsg nick(args[3]);
	client.sendMsg(nick);
}

void Benchmark::sendNick(ConstStrVect& args, OutputStream& output){
	if(args.size()<2){
		std::cout<< "No nick given!"<< std::endl;
		return;
	}
	NickMsg nick(args[1]);

	client.sendMsg(nick);
}

void Benchmark::notify(std::vector<std::string> body, message_header_t head){

	AckNickMsg* acknick;
	TextMsg* text;
	PingMsg* ping;
	AckJoinMsg* ackjoin;

	switch(head.type){
	case MSG_TYPE_ACKNOWLEDGE_CHANGE_NICK:
		acknick = new AckNickMsg(head, body);
		if(acknick->process(cerr))
			nick  = body[0];
		delete acknick;
		break;
	case MSG_TYPE_MESSAGE:
		body.push_back(chan);
		text = new TextMsg(head, body);
		text->process(cerr);
		delete text;
		break;
	case MSG_TYPE_PING:
		ping = new PingMsg(head, body);
		ping->process(cerr);
		delete ping;
		break;
	case MSG_TYPE_ACKNOWLEDGE_JOIN:
		chan = pending_chan;
		pending_chan = "";
		ackjoin = new AckJoinMsg(head, body);
		if(ackjoin->process(cerr))
			chan = pending_chan;
		pending_chan = "";
		delete ackjoin;
		break;
	default:
		cout << "Can't recognize message type : " << head.type << endl;
	}
	if(printing)
		cerr << "> ";
}

void Benchmark::disconnectClient(ConstStrVect& args, OutputStream& output){
	client.disconnect();
}
void Benchmark::pingServer(ConstStrVect& args, OutputStream& output){
	PingMsg ping;
	client.sendMsg(ping);
}

void Benchmark::join(ConstStrVect& args, OutputStream& output){
	if(args.size() < 2){
		cout<< "No channel name given!" << endl; return;
	}
	JoinMsg jo(args[1]);
	pending_chan = args[1];
	client.sendMsg(jo);
}

void Benchmark::msg(ConstStrVect& args, OutputStream& output){
	if(args.size() < 3){
		std::cout << "Bad command usage : \"msg target message to send...\"" << std::endl;
		return;
	}
	std::vector<std::string> ve;
	for(int i = 2; i< args.size(); i++)
		ve.push_back(args[i]);
	TextMsg txt(nick, args[1], ve);
	client.sendMsg(txt);
}

void Benchmark::ConsoleRoutine(){
	shell.setCmd("bob", 		[](ConstStrVect&, OutputStream& output){output << "bob of bob";});
	shell.setCmd("connect", 	member(&Benchmark::connectClient, this));
	shell.setCmd("disconnect", 	member(&Benchmark::disconnectClient, this));
	shell.setCmd("exit", 		member(&Benchmark::exit_from_shell, this));
	shell.setCmd("nick", 		member(&Benchmark::sendNick, this));
	shell.setCmd("ping", 		member(&Benchmark::pingServer, this));
	shell.setCmd("msg", 		member(&Benchmark::msg, this));
	shell.setCmd("join", 		member(&Benchmark::join, this));
	shell.setCmd("echo", 		echofct);

	std::string line;
	run_bool = true;
	while(run_bool) {
		try {
			printing = true;
			std::cout << std::endl << "> " << std::flush;
			std::getline(std::cin, line);
			if (line.size() == 0) continue;
			shell.parse(line, std::cout);
			printing = false;
		} catch(std::exception& e) {
			std::cout << "Erreur: " << e.what() << std::endl;
		}
	}
}

