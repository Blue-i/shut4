/*
 * PJLinkParser.cpp
 *
 *  Created on: 23 Oct 2016
 *      Author: rag
 */

#include "PJLinkParser.h"
#include "Logging.h"


PJLinkParser::PJLinkResponse PJLinkParser::parseMessage(char * msg) {

	size_t length = strlen(msg);
	Log.Debug("PLRP %s%s",msg,CR);
	if(length < 9) return UNKNOWN;										//too short to be valid
	if(msg[0] != '%' && msg[1] != '1' && msg[6] != '=') return UNKNOWN;	//does not conform to a PJLink Response
	if(msg[7] == 'O' && msg[8] == 'K') return OK;						//OK from any command
	if(msg[7] == 'E') return ERROR;										//ERROR from any command

	//need to parse command specific return values
	char command[5];
	for(size_t i = 2; i < 6; ++i){
		command[i-2] = msg[i];
	}
	command[4] = '\0';
	Log.Debug("PJLP CMD %s%s",command, CR);

	//match to command
	if(strcmp("AVMT", command) == 0){
		if(msg[8] == '0') return AVMT_OFF;
		if(msg[8] == '1') return AVMT_ON;
	}

	return UNKNOWN;
}

PJLinkParser::PJLinkParser() {
}

PJLinkParser* PJLinkParser::instance() {
	static PJLinkParser instance;
	return &instance;
}
