
#include "SimpleHTMLParser.h"
#include "openhttp.h"

SimpleHTMLParser::SimpleHTMLParser()
{
	dfound = false;
	wl = 0;
}

void SimpleHTMLParser::printDes() {
	
	if(dfound) {
		
		printf("\n description: %s\n" , pdes);
		
	}
	
	else {
		
		printf("\ndescription: %s\n" , title);
		
	}
	
}

bool
SimpleHTMLParser::match(char **b, const char *m) {
	int len = strlen(m);
	if ( !strncasecmp(*b, m, len)) {
		(*b)+=len;
		return true;
	}
	return false;
}

bool
SimpleHTMLParser::parse(char * buffer, int n)
{
	enum { START, TAG, SCRIPT, ANCHOR, HREF,
	       COMMENT, FRAME, SRC, STYLE, META, TITLE , DES } state;
	
	state = TAG;
	
	char * bufferEnd = buffer + n;
	char * b = buffer;
	char * s;
	bool lastCharSpace = false;
	bool p = false; //temp value

	while(!(match(&b , "<"))) {
		
		if(b >= bufferEnd) {
		
		printf("\nnothing in this document\n");
		strcpy(title , "connection refused");
		return false;
		
	}
		
		b++;
	}
	
	while (b < bufferEnd) {
		//printf("<%c,%d,%d>", *b, *b,state);
		if(!p) { //temp code
			printf("\ninside buffer\n");
			p  = true; //temp code
		}//temp code
		switch (state) {
		case START: {
			if (match(&b,"<SCRIPT")) {
				state = SCRIPT;
			}		
			else if(!dfound && match(&b , "<META")) {
				state = META;
			}
			else if(match(&b , "<TITLE>")) {
				state = TITLE;
			}
			else if(match(&b , "<STYLE")) {

				state = STYLE;
			}
			else if (match(&b,"<!--")) {
				state = COMMENT;
			}
			else if (match(&b,"<A ")) {
				state = ANCHOR;
			}
			else if (match(&b,"<FRAME ")) {
				state = FRAME;
			}

			else if	(match(&b,"<")) {
				state = TAG;
			}
			
			else {
				char c = *b;
				//Substitute one or more blank chars with a single space
				if (c=='\n'||c=='\r'||c=='\t'||c==' ' || c=='.' || c==',' || c== ';' || c=='?' || c == '-' || c=='/' || c == '|') {
					if (!lastCharSpace) {
						onContentFound(' ');
					}
					lastCharSpace = true;
				}
				else {
					onContentFound(c);
					lastCharSpace = false;
				}
				
				b++;
			}
			break;
		}
		case ANCHOR: {
			if (match(&b,"href=\"")) {
				state = HREF;
				urlAnchorLength=0;
				//printf("href=");
			}
			else if (match(&b,">")) {
				// End script
				state = START;
			}
			else {
				b++;
			}
			break;
				
		}
		case HREF: {
			if (match(&b,"\"")) {
				// Found ending "
				state = ANCHOR;
				if(urlAnchor[urlAnchorLength-1] != '/') {
					urlAnchor[urlAnchorLength] = '/';
					urlAnchorLength++;
				}
				urlAnchor[urlAnchorLength] = '\0';
				onAnchorFound(urlAnchor);
				//printf("\n");
			}
			else {
				if ( urlAnchorLength < MaxURLLength-2) {
					urlAnchor[urlAnchorLength] = *b;
					urlAnchorLength++;
				}
				//printf("%c", *b, *b);
				b++;
			}
			break;
		}
		case META: {
			
			s = b;
			while(true) {
				
				if(match(&b , "description")) {
					
					state = DES;
					desLength = 0;
					dfound = true;
					break;
					
				}
				
				else if (match(&b,">")) {
					
					state = START;
					break;
				}
				
				b++;
			
			
			}
			
			
			break;
				
		}
		case DES: {
			while (!(match(&s,"content=\""))) {
				// Found start"
				s++;
			}
			while (!(match(&s,"\"")) && desLength < (2000-1)) {
			
				pdes[desLength] = *s;
				desLength++;
				s++;
				
				if(pdes[0] == '$') {
					
					dfound = false;
					break;
					
				}
			}
			
			pdes[desLength] = '\0';
			state = META;
			break;
		}
		case FRAME: {
			if (match(&b,"src=\"")) {
				state = SRC;
				urlAnchorLength=0;
				//printf("href=");
			}
			else if (match(&b,">")) {
				// End script
				state = START;
			}
			else {
				b++;
			}
			break;
				
		}
		case SRC: {
			if (match(&b,"\"")) {
				// Found ending "
				state = FRAME;
				if(urlAnchor[urlAnchorLength-1] != '/') {
					urlAnchor[urlAnchorLength] = '/';
					urlAnchorLength++;
				}
				urlAnchor[urlAnchorLength] = '\0';
				onAnchorFound(urlAnchor);
				//printf("\n");
			}
			else {
				if ( urlAnchorLength < MaxURLLength-2) {
					urlAnchor[urlAnchorLength] = *b;
					urlAnchorLength++;
				}
				//printf("%c", *b, *b);
				b++;
			}
			break;
		}
		case SCRIPT: {
			if (match(&b,"/SCRIPT>")) {
				// End script
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		case STYLE: {
			if (match(&b , "/STYLE>")) {
				//End style
				state = START;
			}
			else {
				b++;
	
			}
			break;
		}	
		case COMMENT: {
			if (match(&b,"-->")) {
				// End comments
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		case TITLE: {
			tLength = 0;
			while(!match(&b,"</TITLE>")) {
				if(tLength == 199) {
					
					break;
					
				}
				title[tLength] = *b;
				onContentFound(*b);
				b++;
				tLength++;
				
			}
			
			title[tLength] = '\0';
			state = START;
			onContentFound(' ');
			break;
		}
		case TAG: {
			if (match(&b, ">")) {
				onContentFound(' ');
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		default:;
		}
		
	}
	
	return true;
}

void
SimpleHTMLParser::onContentFound(char c) {
}

void
SimpleHTMLParser::onAnchorFound(char * url) {
}
