/*
 * proto.h
 *
 * copyright (2012) Benoit Gschwind
 *
 */

#ifndef PROTO_H_
#define PROTO_H_

// needed to use RTTI
#include <typeinfo>


#include <cassert>
#include <stdexcept>
#include <sys/fcntl.h>
#include <stdint.h>
#include <vector>
#include <algorithm>
#include <iostream>

using byte = unsigned char;

enum message_type_e : uint32_t {
	MSG_TYPE_NOP                        = 0, // not used
	MSG_TYPE_PUBLIC                     = 1, // not used
	MSG_TYPE_CHANGE_NICK                = 2, // change nickname (string)
	MSG_TYPE_PRIVATE                    = 3, // not used
	MSG_TYPE_ACKNOWLEDGE_CHANGE_NICK    = 4, // acknowledge change nick (string,string)
	MSG_TYPE_MESSAGE                    = 5, // message (string,string,string)
	MSG_TYPE_JOIN                       = 6, // join channel (string)
	MSG_TYPE_PING                       = 7, // used to check if client is alive
	MSG_TYPE_PONG                       = 8, // not used
	MSG_TYPE_ACKNOWLEDGE_JOIN           = 9  // acknowledge join channel
};

struct message_header_t {
	uint32_t length;                // message length
	uint32_t type;                  // message_type_e
	uint16_t body_checksum;         //
	uint16_t head_checksum;         //
} __attribute__((packed)); /* do not use padding for alignment */

/**
 * Simple checksum function.
 *
 * source : http://en.wikipedia.org/wiki/Fletcher%27s_checksum
 **/
inline  uint16_t fletcher16(uint8_t const * data, int count) {
	uint16_t sum1 = 0;
	uint16_t sum2 = 0;
	int index;
	for (index = 0; index < count; ++index) {
		sum1 = (sum1 + data[index]) % 255;
		sum2 = (sum2 + sum1) % 255;
	}
	return (sum2 << 8) | sum1;
}

inline  uint16_t fletcher16(const std::vector<std::string>& data) {
	uint16_t sum1 = 0;
	uint16_t sum2 = 0;
	for (std::string const& str: data) { 						// for each string
		for(int i = 0; i< str.size(); ++i){
			sum1 = (sum1 + str.at(i)) % 255;
			sum2 = (sum2 + sum1) % 255;
		}
		sum1 = (sum1 + '\0') % 255;
		sum2 = (sum2 + sum1) % 255;
	}
	return (sum2 << 8) | sum1;
}



#endif /* PROTO_H_ */
