
// Implementation of a dictionary using an array and binary search
// The class inherits from ArrayDictionary

#ifndef BSEARCH_H
#define BSEARCH_H

#include "array-dictionary.h"

class BinarySearchDictionary : public ArrayDictionary {
  // Add any member variables you need
  bool sorted;
public:
  // Constructor
  BinarySearchDictionary();
  
  bool addRecord( KeyType key, DataType record);
  
  // Find a key in the dictionary and return corresponding record or NULL
  // Use binary search
  DataType findRecord( KeyType key);

  // Sort array using heap sort.
  void sort();

    // Removes one element from the table
  bool removeElement(KeyType key);

  // Returns all the elements in the table as an array of strings.
  // *n is the size of the table and it is returned by reference
  //KeyType * keys(int * n);

  // Add other methods you may need
  
  //function to add nodes to the heap for heap sort  
  void addHeap(ArrayDictionaryNode * , ArrayDictionaryNode , int&);
  
  //function to remove nodes from the heap for heap sort
  ArrayDictionaryNode rmHeap(ArrayDictionaryNode * heap ,int&);
  
  //adds key that we know is unique so we don't have to check for collisions
  bool addUnique(KeyType key, DataType record);
  
  
  
};

#endif
