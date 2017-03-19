/*
 * AckJoinMsg.h
 *
 *  Created on: 17 mars 2017
 *      Author: edwardvergara
 */

#ifndef MESSAGES_ACKJOINMSG_H_
#define MESSAGES_ACKJOINMSG_H_

#include <messages/Msg.h>

class AckJoinMsg: public Msg {
public:
	AckJoinMsg(const struct message_header_t& header,
			const std::vector<std::string>& body ): Msg(header, body){}
	int process(std::ostream& output){
		if(body.size()<2) return false;
		if(to_bool(body[1])){
			output << ">> Joined channel #" << body[0] << std::endl;
			return true;
		}
		else{
			output << "Can't join channel #"<< body[0] << std::endl;
			return false;
		}
	}
};

#endif /* MESSAGES_ACKJOINMSG_H_ */
