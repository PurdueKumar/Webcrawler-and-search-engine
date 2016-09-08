
#ifndef SEARCH_H
#define SEARCH_H

#include <cstdlib>
#include "minihttpd.h"
#include "dictionary.h"
#include "array-dictionary.h"
#include "bsearch-dictionary.h"
#include "hash-dictionary.h"
#include "avl-dictionary.h"
#include <fstream>
#include <iostream>

// Types of dictionaries used
enum DictionaryType {
  ArrayDictionaryType,
  HashDictionaryType,
  AVLDictionaryType,
  BinarySearchDictionaryType
};

// Inherits from MiniHTTPD
class SearchEngine : public MiniHTTPD {

  Dictionary * _wordToURLList;
  int numsearch;
  double totaltime;
  
  // Add any other member variables you need
 public:
  // Constructor for SearchEngine
  SearchEngine( int port, DictionaryType dictionaryType);

  // Called when a request arrives
  void dispatch( FILE * out, const char * requestLine );

};


#endif
