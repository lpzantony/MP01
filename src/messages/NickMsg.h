/*
 * NickMsg.h
 *
 *  Created on: 16 mars 2017
 *      Author: edwardvergara
 */

#ifndef MESSAGES_NICKMSG_H_
#define MESSAGES_NICKMSG_H_
#include "Msg.h"
#include <iostream>

class NickMsg : public Msg{
public:
	NickMsg(const std::string& newNick):Msg(){
		body.push_back(newNick);
		header.type = MSG_TYPE_CHANGE_NICK;
	}

	NickMsg(const struct message_header_t& header,
			const std::vector<std::string>& body ): Msg(header, body){}

};

#endif /* MESSAGES_NICKMSG_H_ */
