/*
 * JoinMsg.h
 *
 *  Created on: 19 mars 2017
 *      Author: edwardvergara
 */

#ifndef MESSAGES_JOINMSG_H_
#define MESSAGES_JOINMSG_H_

#include <messages/Msg.h>

class JoinMsg: public Msg {
public:
	JoinMsg(const std::string& channel):Msg(){
			body.push_back(channel);
			header.type = MSG_TYPE_JOIN;
		}
	JoinMsg(const struct message_header_t& header,
				const std::vector<std::string>& body ): Msg(header, body){}
};

#endif /* MESSAGES_JOINMSG_H_ */
