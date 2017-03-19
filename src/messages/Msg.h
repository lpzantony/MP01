/*
 * Msg.h
 *
 *  Created on: 16 mars 2017
 *      Author: edwardvergara
 */

#ifndef NETWORK_MSG_H_
#define NETWORK_MSG_H_
#include "protocol.hxx"
#include <vector>
#include "messages/Msg.h"

class Msg {
public:
	Msg(const struct message_header_t& header, const std::vector<std::string>& body );
	virtual ~Msg(){}
	virtual int process(std::ostream& output){return true;};
	virtual message_header_t* formatHeader();
	const 	std::vector<std::string>& getBody(){return body;};
	int		getType(){return header.type;};
	std::string msgInfo();

protected:
	Msg();
	int  getBodySize();
	bool to_bool(std::string const& s)
		{ return (s == "True" || s == "true")? true : false;}
	struct message_header_t 	header;
	std::vector<std::string> 	body;
};

#endif /* NETWORK_MSG_H_ */
