
#ifndef SIMPLEHTMLPARSER_H
#define SIMPLEHTMLPARSER_H

#include "openhttp.h"
#include <string.h>

class SimpleHTMLParser {
	// URL read in anchor

protected:
	int urlAnchorLength;
	char urlAnchor[MaxURLLength];
	int desLength;
	char pdes[2000];
	bool dfound;
	int tLength;
	char title[200];
	char nWord[100];
	int wl;
	
	bool match(char **b, const char *m);
 public:
	SimpleHTMLParser();
	
	bool parse(char * buffer, int n);
	void printDes();

	// Calbacks
	virtual void onContentFound(char c);
	virtual void onAnchorFound(char * url);

};

#endif

