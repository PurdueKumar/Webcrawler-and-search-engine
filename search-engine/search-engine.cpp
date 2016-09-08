
#include <string.h>
#include "search-engine.h"
#include "webcrawl.h"
#define BILLION 1000000000

using namespace std;

SearchEngine::SearchEngine( int port, DictionaryType dictionaryType):
  MiniHTTPD(port)
{

	numsearch = 0;
	totaltime = 0;
	
	switch(dictionaryType) {

		case(ArrayDictionaryType):
			
			_wordToURLList = new ArrayDictionary();
			break;

		case( HashDictionaryType):
		
			_wordToURLList = new HashDictionary();
			break;
	
		
		case(BinarySearchDictionaryType):
		
			_wordToURLList = new BinarySearchDictionary();
			break;

		case(AVLDictionaryType):
		
			_wordToURLList = new AVLDictionary;
			break;

	}

	ifstream urls;
	ifstream words;
	urls.open("url.txt");
	if(urls.fail()) {

		printf("ERROR: Can't open URLs file\n");
		return;

	}

	words.open("word.txt");
	if(urls.fail()) {

		printf("ERROR: Can't open URLs file\n");
		return;

	}

	URLRecord * urlList = (URLRecord *) malloc(sizeof(URLRecord) * 100);
	int listSize = 100;
	int i = 0;
	char * line = (char *)malloc(sizeof(char) * 9000);

	while(1) {
		
		while(urls.peek() != 'h') {
			
			if(urls.eof()) {
				
				break;
				
			}
			urls.ignore(1);
			
		}
		
		if(urls.eof()) {

			break;

		}

		if(i >= listSize) {

			listSize *= 2;
			urlList = (URLRecord *) realloc(urlList , sizeof(URLRecord) * listSize);

		}

		urls.getline(line, 300);
		urlList[i]._url = strdup(line);
		
		urls.getline(line , 300);	
		urlList[i]._description = strdup(line);
	
		i++;
		
	}
	
	URLRecordList ** recordList = (URLRecordList **) malloc(sizeof(URLRecordList *) * 100);
	int recordSize = 100;
	i = 0;
	int pos;
	
	char * w = (char *) malloc(sizeof(char) * 100);
	
	while(1) {
		
		if(words.eof()) {
			
			break;
			
		}
		
		if( i >= recordSize) {
			
			recordSize *= 2;
			
			recordList = (URLRecordList **) realloc(recordList, sizeof(URLRecordList *) * recordSize);
		
			
		}
		
		recordList[i] = new URLRecordList;
		recordList[i]->_next = NULL;
		
		URLRecordList * curr = recordList[i];
		URLRecordList * p = NULL;
		
		words.getline(w , 100 , ' ');
		if(!strcmp(w,"")) {

			continue;

		}
		
		if(w[0] == '\0') {
			
			break;
			
		}
		
		words.getline(line , 9000);
		char * token = strtok(line , " ");
		
		while(token != NULL) {
		
			pos = atoi(token);
			
			if(p != NULL) {	
				
					p->_next = new URLRecordList;
					curr = p->_next;
			}
			
			curr->_urlRecord = new URLRecord;
			curr->_urlRecord->_url = urlList[pos]._url;
			curr->_urlRecord->_description = urlList[pos]._description;
			curr->_next = NULL;
			p = curr;
		
			token = strtok(NULL , " ");
		
		}
		
		if(dictionaryType == ArrayDictionaryType || dictionaryType == BinarySearchDictionaryType) {
			
			_wordToURLList->addUnique(w , recordList[i]);
			
		}
		
		else {
			
			_wordToURLList->addRecord(w, recordList[i]);
			
		}
		
		i++;
		
	}

	/*int size;


	KeyType * array = _wordToURLList->keys(&size);
	
	for(int j  = 0 ; j < size ; j++) {
		
		printf("%d , %s\n" , j , array[j]);
		
	}*/

	// Create dictionary of the indicated type

	// Populate dictionary and sort it if necessary
	
	urls.close();
	words.close();
	
}

void
SearchEngine::dispatch( FILE * fout, const char * documentRequested)
{

	struct timespec start, end;
	double accum = 0.0;
	if(clock_gettime(CLOCK_REALTIME , &start) == -1) {


		perror("clock gettime");
		//exit(1);

	}


	printf("documentRequested: %s\n" , documentRequested);
	
  if (strcmp(documentRequested, "/")==0) {
    // Send initial form
    fprintf(fout, "<TITLE>CS251 Search</TITLE>\r\n");
    fprintf(fout, "<CENTER><H1><em>Boiler Search</em></H1>\n");
    fprintf(fout, "<H2>\n");
    fprintf(fout, "<FORM ACTION=\"search\">\n");
    fprintf(fout, "Search:\n");
    fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
    fprintf(fout, "</H2>\n");
    fprintf(fout, "</FORM></CENTER>\n");
	fprintf(fout, "<CENTER><H3> <u>Welcome to boiler search! Purdue's finest search engine</u></H3></CENTER>\n");

    return;
  }

  // TODO: The words to search in "documentRequested" are in the form
  // /search?word=a+b+c
  //
  // You need to separate the words before search
  // Search the words in the dictionary and find the URLs that
  // are common for al the words. Then print the URLs and descriptions
  // in HTML. Make the output look nicer.

  // Here the URLs printed are hardwired
  
  if(strcmp(documentRequested, "/search?word=")==0) {

	fprintf(fout, "<TITLE>No words in search</TITLE>\r\n");
    fprintf(fout, "<CENTER><H1><em>Boiler Search</em></H1>\n");
    fprintf(fout, "<H2>\n");
    fprintf(fout, "<FORM ACTION=\"search\">\n");
    fprintf(fout, "Search:\n");
    fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
    fprintf(fout, "</H2>\n");
    fprintf(fout, "</FORM>\n");
	fprintf(fout, "<H3> Empty search </H3>\n");
	fprintf( fout, "<blockquote>You did not enter any search terms<p></blockquote></CENTER>\n");

	return;
	
  }

  int * purl;
  int p = 0;
  bool invalid = false;
  int passes = 0;

  int i = 0;
  int j;

  char * words = (char *) malloc (sizeof(char) * 80);

  URLRecord * urls = (URLRecord *) malloc (sizeof(URLRecord) * 1000);
  purl = (int *) malloc (sizeof(int) * 1000);

  int urlSize = 1000;

  char * b;

  b = strdup(documentRequested);
  char * l = strdup(documentRequested);

  while(*b != '=') {

	b++;
	l++;

  }  
  b++;
  l++;

  char * c = b;

  while (*c != '\0') {

	if(*c == '+') {

		*c = ' ';

	}
	c++;
  }

  words = strtok(l , "+");

  while(words != NULL) {

	if(i >= urlSize) {
		
		urlSize *= 2;
		urls = (URLRecord *) realloc(urls, sizeof(URLRecord) * urlSize);
		purl = (int *) realloc(urls, sizeof(int) * urlSize);
	}

	printf("search term: %s\n" , words);

	URLRecordList * x = (URLRecordList *) _wordToURLList->findRecord(words);

	if(x == NULL) {

		invalid = true;
		break;

	}

	while(x != NULL) {

		if(passes == 0) {

			urls[i]._url = x->_urlRecord->_url;
			urls[i]._description = x->_urlRecord->_description;
			i++;

		}

		else {
	
			for(j = 0; j < i ; j++) {

				if(!(strcmp(x->_urlRecord->_url , urls[j]._url))) {

					purl[p] = j;
					p++;

				}

			}
		}

		x = x->_next;

	}

	printf("p is: %d\n" , p);
	printf("passes is: %d\n" , passes);

	if( p == 0 && passes != 0) {
		
		i = p;
		break;

	}
	if(passes != 0) {	


		URLRecord * templist = new URLRecord[p];
		for(int n = 0 ; n < p ; n++) {

			templist[n]._url = urls[purl[n]]._url;
			templist[n]._description = urls[purl[n]]._description;

		}

		for(int n = 0 ; n < p ; n++) {

			urls[n]._url = templist[n]._url;
			urls[n]._description = templist[n]._description;
			printf("new url %d: %s\n" , n , urls[n]._url);

		}
	
	
		i = p;
		p = 0;

		delete[] templist;
	}

	words = strtok(NULL , "+");
	passes++;


  }

	if(clock_gettime(CLOCK_REALTIME , &end) == -1) {


		perror("clock gettime");
		//exit(1);

	}
	accum = (double)((end.tv_sec - start.tv_sec) + (double)((end.tv_nsec - start.tv_nsec))/BILLION);
	totaltime += accum;
	numsearch++;

	printf("start sec: %ld\n" , start.tv_sec);
	printf("end sec: %ld\n" , end.tv_sec);
	printf("start nsec: %ld\n" , start.tv_nsec);
	printf("end nsec: %ld\n" , end.tv_sec);

  fprintf( stderr, "Search for words: \"%s\"\n", words);

  fprintf( fout, "<TITLE>Search Results</TITLE>\r\n");
  fprintf( fout, "<H1> <Center><em>Boiler Search</em></H1>\n");
  fprintf( fout, "search time: %lfs<BR>" , accum);
  fprintf( fout, "average time: %lfs<BR><HR>" , (totaltime/numsearch));


if(!invalid && i != 0) {

  fprintf( fout, "<H2> Search Results for \"%s\"</center></H2>\n", b );


  for ( j = 0; j < i; j++ ) {

    fprintf( fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", j+1, urls[j]._url , urls[j]._url );
    fprintf( fout, "<blockquote>%s<p></blockquote>\n",urls[j]._description );
  }

}

else if(!invalid && i == 0) {

	
	fprintf(fout, "<H3> No urls with overlapping search terms </H3>\n");
	fprintf( fout, "<blockquote>There are no urls which contain all of the terms \"%s\" that you serched for<p></blockquote></CENTER>\n" , b);


}

else {

	fprintf(fout, "<H3> Invalid search term </H3>\n");
	fprintf( fout, "<blockquote>The word: \"%s\" that you serched for does not appear in any of the urls<p></blockquote></CENTER>\n" , words);


}

  // Add search form at the end
  fprintf(fout, "<HR><H2>\n");
  fprintf(fout, "<FORM ACTION=\"search\">\n");
  fprintf(fout, "Search:\n");
  fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
  fprintf(fout, "</H2>\n");
  fprintf(fout, "</FORM>\n");
}

void
printUsage()
{
  const char * usage =
    "Usage: search-engine port (array | hash | avl | bsearch)\n"
    "  It starts a search engine at this port using the\n"
    "  data structure indicated. Port has to be larger than 1024.\n";

  fprintf(stderr, "%s", usage);
}

int main(int argc, char ** argv)
{
  if (argc < 3) {
    printUsage();
    return 1;
  }

  // Get port
  int port;
  sscanf( argv[1], "%d", &port);

  // Get DictionaryType
  const char * dictType = argv[2];
  DictionaryType dictionaryType;
  if (!strcmp(dictType, "array")) {
    dictionaryType = ArrayDictionaryType;
  }
  else if (!strcmp(dictType, "hash")) {
    dictionaryType = HashDictionaryType;
  }
  else if (!strcmp(dictType, "avl")) {
    dictionaryType = AVLDictionaryType;
  }
  else if (!strcmp(dictType, "bsearch")) {
    dictionaryType = BinarySearchDictionaryType;
  }
  else {
    printUsage();
    return 0;
  }
  
  SearchEngine httpd(port, dictionaryType);
  
  httpd.run();

  return 0;
}
