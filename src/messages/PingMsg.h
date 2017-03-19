/*
 * PingMsg.h
 *
 *  Created on: 17 mars 2017
 *      Author: edwardvergara
 */

#ifndef MESSAGES_PINGMSG_H_
#define MESSAGES_PINGMSG_H_

#include <messages/Msg.h>

class PingMsg: public Msg {
public:
	PingMsg():Msg(){ header.type = MSG_TYPE_PING; }

	PingMsg(const struct message_header_t& header,
			const std::vector<std::string>& body ): Msg(header, body){}
	int process(std::ostream& output){return true;}
};

#endif /* MESSAGES_PINGMSG_H_ */
