/*
 * TCPClient.h
 *
 *  Created on: 15 mars 2017
 *      Author: edwardvergara
 */

#ifndef NETWORK_TCPCLIENT_H_
#define NETWORK_TCPCLIENT_H_
#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>
#include <mutex>
#include "core/EventSpeaker.h"
#include "messages/Msg.h"

#define ERR (char)-1

using namespace std;
using MsgSpeaker = EventSpeaker<message_header_t>;

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;

class TCPClient : public MsgSpeaker{
public:
	static const uint32_t 		BUF_SIZE = 500;
	static const unsigned short UDP = 2;
	static const unsigned short TCP = 1;

	TCPClient();
	virtual ~TCPClient();
	int 	connect(const string& ip, int port);
	void 	disconnect();
	void  	serverRoutine();
	bool 	sendMsg(Msg& msg);
	bool 	isConnected() const {return connected;}

	unsigned short int getClientPort()	const
		{ return ntohs(client_addr.sin_port);}
	std::string getClientAddr() const
		{ return  inet_ntoa(client_addr.sin_addr);}
private:
	SOCKET 		client_skt = 0;
	SOCKADDR_IN	client_addr;
	std::thread the_thread;
	std::mutex 	the_mutex;
	std::mutex 	sktmut;
	bool 		running = false;
	bool 		connected = false;

	int 	subconnect(const string& ip, int port);
	bool 	getRunning();
	void 	setRunning(const bool run);
	void 	lockMutex(){ the_mutex.lock();}
	void 	unlockMutex(){ the_mutex.unlock();}
	int  	setFdBlocking(int fd, bool blocking);
	int     setRcvTimeout(long timeout_s, long timeout_us);
	int 	setAddress(struct sockaddr_in& addr, const string& host, int port);
};

#endif /* NETWORK_TCPCLIENT_H_ */
