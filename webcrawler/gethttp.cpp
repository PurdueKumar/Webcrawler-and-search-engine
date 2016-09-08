/*
  Copyright (c) 2004
  Gustavo Rodriguez-Rivera
  All rights reserved. 

This work was developed by the author(s) at Purdue University
during 2004.
 
Redistribution and use in source and binary forms are permitted provided that
this entire copyright notice is duplicated in all such copies.  No charge,
other than an "at-cost" distribution fee, may be charged for copies,
derivations, or distributions of this material without the express written
consent of the copyright holders. Neither the name of the University, nor the
name of the author may be used to endorse or promote products derived from
this material without specific prior written permission.

THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR ANY PARTICULAR PURPOSE.
*/

//
// gethttp:
//   Example program that shows how to use openttp.
//   It gets the URL passed as argument
//
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "openhttp.h"
#include "SimpleHTMLParser.h"

void
printUsage()
{
  fprintf( stderr, "  Usage: gethttp [-h|-t|-a] url\n");
  fprintf( stderr, "  Example: gethttp http://www.cs.purdue.edu\n");
}

////////////////// HTML Parser to print the content //////////////////

class HTMLParserPrintContent : public SimpleHTMLParser {
public:
	void onContentFound(char c);
};

void
HTMLParserPrintContent::onContentFound(char c) {
	if(c == ' ') {
		
		if(wl == 0) {
			
			return;
			
		}
		
		else if(!((nWord[0] >= 'a' && nWord[0] <= 'z') ||  (nWord[0] >= 'A' && nWord[0] <= 'Z'))) {
			
			wl = 0;
			return;
			
		}
		
		nWord[wl] = '\0';
		wl = 0;
		printf("%s " , nWord);
		/*URlRecordList * d;
		if(w->_wordToURLRecordList->find(nWord, d)) {
			
			while (d->next != NULL) {
				
				d = d->next;
				
			}
			
			d->next = new URLRecordList;
			d->next->_urlRecordIndex = w->_headURL;
			d->next->next = NULL;
			return;
		}
		
		d = new URLRecordList;
		d->URLRecordIndex = _headURL;
		d->next = NULL;
		
		w->_wordToURLRecordList->insertItem(nWord , d);*/
		
		return;
		
	}
	
	if(!((c >= 'a' && c <= 'z') ||  (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))){
		
		onContentFound(' ');
		return;
	}
	
	if(wl == 199) {
		
		printf("\nerror: word larger than 200 characters!\n");
		onContentFound(' ');
		return;
		
	}
	
	else {
		
		nWord[wl] = c;
		wl++;
		return;
	}
}

////////////////// HTML Parser to print the href in anchors //////////////////

class HTMLParserPrintHrefs : public SimpleHTMLParser {
public:
	void onAnchorFound(char * url);
};

void
HTMLParserPrintHrefs::onAnchorFound(char * url) {
	
	//remove https links
	if(!strncmp(url , "https" , strlen("https"))) {
		
		return;
		
	}
		
	//remove e-mail addresses
	else if(!strncmp(url , "mailto" , strlen("mailto"))) {
		
		return;
		
	}
	
	//remove jquery elements
	else if(url[0] == '#') {
		
		return;
		
	}
	
	//sanitize urls starting with //
	else if(!strncmp(url , "//" , strlen("//"))){
		
		char * b = strdup(url);
		
		sprintf(url , "http:%s" , b);
		
		free(b);
	}
	
	//else if not normal http link
	else if(strncmp(url , "http:" , strlen("http:"))) {
	
		char host[] = "http://www.this.com/newsroom/";
		char * b = host;
		while (*b != '\0') {
		
			b++;
		
		}
		b--;
		b--;
	
		int n = 0;
	
		while(*b != '/') {
		
			b--;
			n++;
		}
	
		b++;
		char * u = url;

		if(url[0] == '/') {
		
			u++;
		
		}
	
		if(!(strncmp(u,b,n))) {
		
			*b = '\0';
			char * c = strdup(u);
			sprintf(url , "%s%s" , host , c);
		
			free(c);
		}
	
		else {
		
			char * c = strdup(u);
			sprintf(url , "%s%s" , host ,c);
			free(c);
		
		}
	
	}
	
	char * n = url;
	
	while(*n != '\0') {
		
		if(!strncmp(n , "index." , strlen("index."))) {
			
			*n = '\0';
			break;
			
		}
		
		n++;
	}
	
	printf("%s\n",url);
}

int
main( int argc, char ** argv )
{
  const char * option;
  char * url;
  
  if ( argc == 1) {
	  printUsage();
	  exit(1);	  
  }
  else if (argc == 2) {
	  option = "";
	  url = argv[1];
  }
  else { 
	  // There is an option
	  option = argv[1];
	  url = argv[2];
  }
  
  // Process the arguments
  if ( !strcmp(option,"-h") ) {
	  printUsage();
	  exit(1);
  }
  else if ( !strcmp(option,"") ) {
	  // show document
	  int n;
	   if(!(testURL(url))) {
		  
		  printf("*** Cannot open URL\n");
		  exit(1);
		  
	  }
	  char * buffer = fetchHTML( url, &n);
	  if (buffer != NULL) {
		  fwrite(buffer, n, 1, stdout);
	  }
  } else if ( !strcmp(option,"-t") ) {
	  // Eliminate tags
	  int n;
	   if(!(testURL(url))) {
		  
		  printf("*** Cannot open URL\n");
		  exit(1);
		  
	  }
	  char * buffer = fetchHTML( url, &n);
	  if (buffer==NULL) {
		  fprintf(stderr, "*** Cannot open URL\n");
		  exit(1);
	  }
	  HTMLParserPrintContent htmlContent;
	  htmlContent.parse(buffer, n);
	  htmlContent.printDes();
  } else if ( !strcmp(option,"-a") ) {
	  // Eliminate tags
	  int n;
	  if(!(testURL(url))) {
		  
		  printf("*** Cannot open URL\n");
		  exit(1);
		  
	  }
	  char * buffer = fetchHTML( url, &n);
	  if (buffer==NULL) {
		  fprintf(stderr, "*** Cannot open URL\n");
		  exit(1);
	  }
	  HTMLParserPrintHrefs htmlHrefs;
	  htmlHrefs.parse(buffer, n);
  }

  if ( *argv == NULL ) {
    // Print usage also if no URL after the arguments
    printUsage();
    exit(1);
  }
}

