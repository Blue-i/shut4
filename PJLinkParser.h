/*
 * PJLinkParser.h
 *
 *  Created on: 23 Oct 2016
 *      Author: rag
 */

#ifndef PJLINKPARSER_H_
#define PJLINKPARSER_H_

#include "Arduino.h"



class PJLinkParser {
	static const char* commandString[];

	PJLinkParser();
	PJLinkParser(const PJLinkParser &);
	PJLinkParser& operator=(const PJLinkParser &);
public:

	enum PJLinkCommand {
		AVMT = 0,
		POWR = 1
	};

	enum PJLinkResponse {
		UNKNOWN,
		OK,
		ERROR,
		AVMT_ON,
		AVMT_OFF
	};

	PJLinkResponse parseMessage(char * buffer);
	static PJLinkParser* instance();
};

#endif /* PJLINKPARSER_H_ */
