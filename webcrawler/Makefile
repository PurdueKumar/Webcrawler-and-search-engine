
all: gethttp webcrawl git-commit

gethttp: gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp
	g++ -o gethttp -g gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp

webcrawl: main.cpp webcrawler.cpp openhttp.cpp SimpleHTMLParser.cpp
	g++ -o webcrawl -g main.cpp webcrawler.cpp openhttp.cpp SimpleHTMLParser.cpp

git-commit:
	git add Makefile *.h *.cpp  >> .local.git.out
	git commit -a -m "Commit web-crawler" >> .local.git.out

clean:
	rm -f *.o gethttp core
	rm -f *.o webcrawl core
