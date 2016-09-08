#include"webcrawler.h"

using namespace std;

int main(int argc , char** argv) {
	
	char usage[] = "usage: webcrawl [-u <maxurls>] url-list";
	
	if(argc == 2) {
		
		char ** init = new char*[1000];
		
		ifstream f;
		
		f.open(argv[1]);
		
		if(f.fail()) {
			
			printf("ERROR: can't open file %s\n" , argv[1]);
			exit(1);
			
		}
		
		char * line = new char[100]; 
		int i = 0;
			
		while(1) {
			
			if(f.eof()) {
				
				break;
				
			}
			
			if(i >= 1000) {
				
				break;
				
			}
			
			f.getline(line,100);
			init[i] = strdup(line);
			i++;
			
		}
		
		
	   delete line;
	   f.close();
	   WebCrawler * w = new WebCrawler(1000 , i , (const char**)init);
	   w->crawl();
	   w->writeURLFile("url.txt");
	   w->writeWordFile("word.txt");
	   
	   delete [] init;
	   
	}
	
	if (argc == 4) {
			
			if(strcmp(argv[1] , "-u")) {
				
				printf("%s\n" , usage);
				exit(1);
				
			}
			
			
		
		int n = atoi(argv[2]);
		
		if (n <= 0) {
			
			printf("max number of urls has to be an integer greater than 0\n");
			exit(1);
			
		}
		
		char ** init = new char*[n];
		
		ifstream f;
		
		f.open(argv[3]);
		
		if(f.fail()) {
			
			printf("ERROR: can't open file %s\n" , argv[1]);
			exit(1);
			
		}
		
		char * line = new char[100]; 
		int i = 0;
			
		while(1) {
			
			if(f.eof()) {
				
				break;
				
			}
			
			if(i >= n) {
				
				break;
				
			}
			
			f.getline(line,100);
			init[i] = strdup(line);
			i++;
			
		}
		
		
	   delete line;
	   f.close();
	   WebCrawler * w = new WebCrawler(n , i , (const char**)init);
	   w->crawl();
	   w->writeURLFile("url.txt");
	   w->writeWordFile("word.txt");
	   delete[] init;
			
		}
	
}

