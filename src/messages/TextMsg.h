/*
 * TextMsg.h
 *
 *  Created on: 17 mars 2017
 *      Author: edwardvergara
 */

#ifndef MESSAGES_TEXTMSG_H_
#define MESSAGES_TEXTMSG_H_

#include <messages/Msg.h>

class TextMsg: public Msg {
public:
	TextMsg(std::string from, std::string to, std::vector<std::string> txt):Msg(){
		header.type = MSG_TYPE_MESSAGE;
		body.push_back(from);
		body.push_back(to);
		std::string str = "";
		for( auto st : txt){
			str +=st + " ";
		}
		body.push_back(str);
	}
	TextMsg(const struct message_header_t& header,
			const std::vector<std::string>& body ): Msg(header, body){}
	int process(std::ostream& output){
		if(body.size()<4) return false;
		output << "#"<< body[3]<< " | "<< body[0] << " -> " << body[1] <<" : " << body[2] << std::endl;
		return true;
	}
};

#endif /* MESSAGES_TEXTMSG_H_ */
