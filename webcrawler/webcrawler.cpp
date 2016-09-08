
#include "webcrawler.h"


using namespace std;

// Add your implementation here
Parser::Parser(WebCrawler * wc) {
	
	w = wc;
	
}



void Parser::onContentFound(char c) {
	
	if(c == ' ') {
		
		if(wl == 0) {
			
			return;
			
		}
		
		else if(!((nWord[0] >= 'a' && nWord[0] <= 'z') ||  (nWord[0] >= 'A' && nWord[0] <= 'Z'))) {
			
			wl = 0;
			return;
			
		}
		
		nWord[wl] = '\0';
		
		for(int i = 0 ; i < wl ; i++) {
			
			if(nWord[i] >= 'A' && nWord[i] <= 'Z') {
				
				nWord[i] += 32;
				
			}
			
		}
		
		wl = 0;
		
		URLRecordList * d;
		if(w->_wordToURLRecordList->find(nWord, &d)) {
			
			if(d->_urlRecordIndex == w->_headURL) {
				
				return;
				
			}
			
			while (d->_next != NULL) {
				
				if(d->_next->_urlRecordIndex == w->_headURL) {
					
					return;
					
				}
				
				d = d->_next;
				
				
			}
			
			d->_next = new URLRecordList;
			d->_next->_urlRecordIndex = w->_headURL;
			d->_next->_next = NULL;
			return;
		}
		
		d = new URLRecordList;
		d->_urlRecordIndex = w->_headURL;
		d->_next = NULL;
		
		w->_wordToURLRecordList->insertItem(nWord , d);
		
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

void Parser::onAnchorFound(char* url) {
	
	if(w->_tailURL >= w->_maxUrls){
		
		return;
		
	}
	
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
	
	//check if url is not a normal http link
	else if(strncmp(url , "http:" , strlen("http:"))){
	
		char * host = strdup(w->_urlArray[w->_headURL]._url);
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
			printf("host is: %s\n" , host);
			sprintf(url , "%s%s" , host , c);
		
			free(c);
		}
	
		else {
		
			char * c = strdup(u);
			sprintf(url , "%s%s" , host ,c);
			free(c);
		
		}
	
		free(host);
	
	}
	
	char * n = url;
	
	while(*n != '\0') {
		
		if(!strncmp(n , "index." , strlen("index."))) {
			
			*n = '\0';
			break;
			
		}
		
		n++;
	}
		
	int num ;
	if(!testURL(url)){
		
		return;
		
	}
		
	int x;
	
	if(w->_urlToUrlRecord->find(url , &x)) {
		
		
		return;
		
	}
	
	else {
		
		w->_urlArray[w->_tailURL]._url = strdup(url);
		w->_urlToUrlRecord->insertItem(url , w->_tailURL);
		w->_tailURL++;
		
		
	}
	
}
	


char * Parser::getDes() {
	
	if(dfound) {
		
		dfound = false;
		return pdes;
		
	}
	
	else{
		
		return title;
		
	}
	
}


WebCrawler::WebCrawler(int maxUrls, int nurlRoots, const char ** urlRoots) {

	_maxUrls = maxUrls;
	_urlArray = new URLRecord[maxUrls];
	_headURL = 0;
	_tailURL = 0;
	
	_urlToUrlRecord = new HashTableTemplate<int>;
	_wordToURLRecordList = new HashTableTemplate<URLRecordList *>;
	
	for (int i = 0 ; i < nurlRoots ; i++) {
		
		if(i == maxUrls) {
			
			break;
			
		}
		
		int n;
		
		if(!testURL(urlRoots[i])) {
			
			continue;
			
		}
		
		_urlArray[i]._url = strdup(urlRoots[i]);
		_tailURL++;
		
		_urlToUrlRecord->insertItem(urlRoots[i] , _tailURL);
		
	}
	
	
	
	_urlToUrlRecord = new HashTableTemplate<int>;
	_wordToURLRecordList = new HashTableTemplate<URLRecordList *>;
	
}

void WebCrawler::crawl() {
	
	int n;
	char * buffer;
	Parser p(this);
	
	while(_headURL != _tailURL) {
	
		buffer = fetchHTML(_urlArray[_headURL]._url , &n);
		
		p.parse(buffer , n);
		_urlArray[_headURL]._description = strdup(p.getDes());
		
	printf("%s parsed\n" , _urlArray[_headURL]._url);
	
		
		_headURL++;
		
	printf("now parsing %s\n" , _urlArray[_headURL]._url);
		
	}
	
	
}

void WebCrawler::writeURLFile(const char * urlFileName) {
		
		ofstream f;
		f.open(urlFileName);
		if(f.fail()) {
			
			printf("ERROR: can't create file %s\n" , urlFileName); 
			return;
		}
		
		for (int i = 0 ; i < _tailURL ; i++) {
			
			f << i << " " <<_urlArray[i]._url << endl <<  _urlArray[i]._description << endl << endl;
			
		}
		
		f.close();
}
	
void WebCrawler::writeWordFile(const char *wordFileName) {
		
	ofstream f;
		f.open(wordFileName);
		if(f.fail()) {
			
			printf("ERROR: can't create file %s\n" , wordFileName); 
			return;
		}
		
		for (int i = 0 ; i < _wordToURLRecordList->TableSize ; i++) {
			
			HashTableTemplateEntry<URLRecordList *>* d = _wordToURLRecordList->_buckets[i];
			
			while(d !=  NULL) {
				
				URLRecordList * data = d->_data;
				f << d->_key <<" ";
				while(data != NULL) {
					
					f << data->_urlRecordIndex <<" ";
					data = data->_next;
				}
				f << endl;
 				
				d=d->_next;
				
			}
			
		}
		
		f.close();
}