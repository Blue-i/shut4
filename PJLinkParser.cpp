/*
 * PJLinkParser.cpp
 *
 *  Created on: 23 Oct 2016
 *      Author: rag
 */

#include "PJLinkParser.h"

const char * PJLinkParser::commandString[2] = {
		"AVMT",
		"POWR"
	};

PJLinkParser::PJLinkResponse PJLinkParser::parseMessage(char * msg) {

	size_t length = strlen(msg);
	if(length < 10) return UNKNOWN;										//too short to be valid
	if(msg[0] != '%' && msg[1] != '1' && msg[6] != '=') return UNKNOWN;	//does not conform to a PJLink Response
	if(msg[8] == 'O' && msg[9] == 'K') return OK;						//OK from any command
	if(msg[8] == 'E') return ERROR;										//ERROR from any command

	//need to parse command specific return values
	char command[5];
	for(size_t i = 3; i < 6; ++i){
		command[i-3] = msg[i];
	}
	command[5] = '\0';

	//match to command
	for(size_t i = 0; i < sizeof(commandString); ++i){
		if(strcmp(commandString[i], command) == 0){
			switch(i){
			case AVMT:
				//parse AVMT response
				if(msg[8] == '0') return AVMT_OFF;
				if(msg[8] == '1') return AVMT_ON;
				break;
			}
			break;
		}
	}
	return UNKNOWN;
}

PJLinkParser::PJLinkParser() {
}

PJLinkParser* PJLinkParser::instance() {
	static PJLinkParser instance;
	return &instance;
}
