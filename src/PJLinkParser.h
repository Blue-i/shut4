/*
 * PJLinkParser.h
 *
 *  Created on: 23 Oct 2016
 *      Author: rag
 */
#pragma once

#ifndef PJLINKPARSER_H_
#define PJLINKPARSER_H_

class PJLinkParser {
	PJLinkParser();
	PJLinkParser(const PJLinkParser &);
	PJLinkParser& operator=(const PJLinkParser &);
public:

	enum PJLinkResponse {
		UNKNOWN,
		OK,
		ERROR,
		AVMT_ON,
		AVMT_OFF
	};

	PJLinkResponse parseMessage(char const * buffer);
	static PJLinkParser* instance();
};

#endif /* PJLINKPARSER_H_ */
