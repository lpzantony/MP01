/*
 * AckNickMsg.h
 *
 *  Created on: 17 mars 2017
 *      Author: edwardvergara
 */

#ifndef MESSAGES_ACKNICKMSG_H_
#define MESSAGES_ACKNICKMSG_H_
#include "Msg.h"

class AckNickMsg : public Msg{
public:
	AckNickMsg(const struct message_header_t& header,
				const std::vector<std::string>& body ): Msg(header, body){}
	int process(std::ostream& output){
		if(body.size()<2) return false;
		if(to_bool(body[1])){
			output << "Nickname "<< body[0]<<" accepted" << std::endl;
			return true;
		}
		else{
			output << "Nickname "<< body[0]<<" already in use" << std::endl;
			return false;
		}
	}
};

#endif /* MESSAGES_ACKNICKMSG_H_ */
