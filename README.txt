webcrawler-and-search-engine
-------------------------------------
author: Nirmal Kumar Asokan

External Functions and Libraries used:
--------------------------------------
Linux socket library

Developed using:
--------------------------------------
gcc
gdb (for debugging)

Program description:
--------------------------------------
disclaimer: This program will only run in linux. To allow it to run on your operating system
you'll have to modify gethttp and openhttp with your system's socket implementation.

This was a final project for my data structures and algorithms class. It involved searching
http websites for words that appear on them, storing those words in different data structures
and then when a search is made retrieving the right website in which they are found. This was
the first time I had to parse html files and understand how information is presented on the 
internet.