/*
 * Msg.cpp
 *
 *  Created on: 16 mars 2017
 *      Author: edwardvergara
 */

#include <messages/Msg.h>


Msg::Msg(){}

Msg::Msg(const struct message_header_t& header, const std::vector<std::string>& body ){
	this->header.length  = header.length;
	this->header.type  = header.type;
	this->header.body_checksum  = header.body_checksum;
	this->header.head_checksum  = header.head_checksum;
	this->body = body;
}
message_header_t* Msg::formatHeader(){
	header.length = sizeof(message_header_t) + (uint32_t)getBodySize();
	header.body_checksum = fletcher16(body);
	header.head_checksum = 0;
	header.head_checksum = fletcher16(reinterpret_cast<const uint8_t*>(&header), sizeof(message_header_t));
	return &header;
}

int Msg::getBodySize(){
	int size = 0;
	for(std::string const& str: body){
		size +=str.length() +1;
	}
	return size;
}

std::string Msg::msgInfo(){
	std::string str = "header:\n";
	str+= "\t length = " + std::to_string(header.length) + "\n";
	str+= "\t type   = " + std::to_string(header.type) + "\n";
	str+= "\t bodyCS = " + std::to_string(header.body_checksum) + "\n";
	str+= "\t headCS = " + std::to_string(header.head_checksum) ;
	return str;
}
