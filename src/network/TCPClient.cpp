/*
 * TCPClient.cpp
 *
 *  Created on: 15 mars 2017
 *      Author: edwardvergara
 */
#include <iostream> //print
#include <fcntl.h> //set blocking
#include <unistd.h> //close
#include <network/TCPClient.h>
#include <netdb.h>//gethostbyname
#include <list>
#include <string>
#include "messages/protocol.hxx"

TCPClient::TCPClient(){}
TCPClient::~TCPClient() {}


int TCPClient::connect(const string& ip, int port){
	if(subconnect(ip, port)== ERR)
			return ERR;
	setRunning(true);

	the_thread= std::thread(&TCPClient::serverRoutine, this);
	return 0;
}

void TCPClient::disconnect(){
	setRunning(false);
	connected = false;
	close(client_skt);
	if (the_thread.joinable()) the_thread.join();
}

int TCPClient::subconnect(const string& ip, int port)
{
	client_skt = socket(AF_INET, SOCK_STREAM, 0);
	if(client_skt < 0){
		printf("Failed creating socket\n");
		return ERR;
	}

	if (setAddress(client_addr, ip, port) < 0) return -1;
	setRcvTimeout(0,200000);
	//setFdBlocking(client_skt, false);
	int ret = ::connect(client_skt, (struct sockaddr*)&client_addr, sizeof(client_addr));
	if (ret == -1){
		printf("retval = %i\n", ret);
		perror("Could not connect!");
		return ERR;
	}
	connected = true;
	return 0;

}

void TCPClient::serverRoutine()
{
	int retval=0;
	int received=0;
	uint32_t expected = 1;
	struct message_header_t header;
	uint8_t  buf[BUF_SIZE] = "";
	std::list<uint8_t> fifo;

	while(getRunning() && connected){
		expected = 1; received = 0; 	// Reset loop variables
		while(received < expected){
			sktmut.lock();
			retval= recv(client_skt, &buf, sizeof(uint8_t) * BUF_SIZE, 0);
			sktmut.unlock();
			if(retval == -1 && errno != EAGAIN){
				perror("Socket receiving error. Disconnecting... ");
				connected = false;
				break;
			}
			//push all bytes into a fifo
			for(int i=0; i< retval; i++) fifo.push_back(buf[i]);
			received += retval;

			//calculating expected
			if(expected == 1 && retval > 0 ){ // can't == 1 because sizeof(message) = 12
				expected = 	(*(std::next(fifo.begin(), 3)) << 24) +
							(*(std::next(fifo.begin(), 2)) << 16) +
							(*(std::next(fifo.begin(), 1)) << 8 ) +
							(*fifo.begin());
			}
		}
		if(retval <=0) continue;

		do{
			// We have at least one full message in the fifo
			std::list<uint8_t>::iterator it = fifo.begin();
			header.length = expected;
			header.type =	(*(std::next(it, 7)) << 24) +
							(*(std::next(it, 6)) << 16) +
							(*(std::next(it, 5)) << 8 ) +
							(*(std::next(it, 4)) << 0 ) ;

			header.body_checksum =	(*(std::next(it, 9 )) << 8) +
									(*(std::next(it, 8 )) << 0) ;

			header.head_checksum =	(*(std::next(it, 11)) << 8) +
									(*(std::next(it, 10)) << 0) ;

			// Erasing head data from fifo
			for(int i=0; i< sizeof(message_header_t); i++) fifo.pop_front();

			std::vector<std::string> body;
			string str;
			for(int i=0; i< header.length - sizeof(message_header_t); i++){
				if(*fifo.begin() == '\0'){ 	// if end of string, append current string to
					body.push_back(str);	// vector and clear it
					str.clear();
				}
				else						// else it is a standard char, append it to str
					str += *fifo.begin();
				fifo.pop_front();			// Remove the character from fifo anyway
			}

			//Sending to main
			notifyObservers( body, header);

			//Check if another message is available in fifo
			if(fifo.size() >= sizeof(message_header_t)){
				expected = 	(*(std::next(fifo.begin(), 3)) << 24) +
							(*(std::next(fifo.begin(), 2)) << 16) +
							(*(std::next(fifo.begin(), 1)) << 8 ) +
							(*fifo.begin());
			}
		}while(fifo.size() >= expected);
	}
	printf("Disconnected.\n");
}


int TCPClient::setFdBlocking(int fd, bool blocking)
{
    bool ret = true;
    const int flags = fcntl(fd, F_GETFL, 0);
	if ((flags & O_NONBLOCK) && !blocking) { printf("set_blocking_mode(): socket was already in non-blocking mode\n"); return ret; }
	if (!(flags & O_NONBLOCK) && blocking) { printf("set_blocking_mode(): socket was already in blocking mode\n"); return ret; }
	ret = 0 == fcntl(fd, F_SETFL, blocking ? flags ^ O_NONBLOCK : flags | O_NONBLOCK);
	return ret;
}

bool TCPClient::getRunning()
{
	bool tmp;
	lockMutex();
	tmp = running;
	unlockMutex();
	return tmp;
}

void TCPClient::setRunning(bool run)
{
	lockMutex();
	running = run;
	unlockMutex();
}

// for INET4 sockets
int TCPClient::setAddress(struct sockaddr_in& addr, const string& host, int port) {
	struct hostent* hostinfo = NULL;
	// gethostbyname() is obsolete!
	if (host.empty() || !(hostinfo = ::gethostbyname(host.c_str())))
		return -1; // host not found
	::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	// creer l'adresse du remote host a partir de son nom
	::memcpy((void *)&addr.sin_addr, hostinfo->h_addr_list[0], hostinfo->h_length);
	return 0;
}

int TCPClient::setRcvTimeout(long timeout_s, long timeout_us){
    struct timeval timeout_t;
    timeout_t.tv_sec = timeout_s;
    timeout_t.tv_usec = timeout_us;

    if (setsockopt (client_skt, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout_t, sizeof(timeout_t)) < 0){
        perror("setsockopt failed\n");
        return ERR;
    }
    return 0;
}


bool TCPClient::sendMsg(Msg& msg)
{
	sktmut.lock();
	int retval = send(client_skt, msg.formatHeader(), sizeof(message_header_t), 0);
	sktmut.unlock();
	switch(retval){
	case -1:
		perror( "ERROR : data not sent!" );
		return false;
	default:
		const std::vector<std::string>& body = msg.getBody();
		for(std::string const& str: body){
			sktmut.lock();
			retval = send(client_skt, str.c_str(), str.length() + 1, 0);
			sktmut.unlock();
			if(retval == -1){
				perror( "ERROR : data not sent!" );
				return false;
			}
		}
		return true;
	}
}

