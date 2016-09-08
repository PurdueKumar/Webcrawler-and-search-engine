
// Implementation of a dictionary using an array and sequential search
// The array will be dynamically resized if necessary

#include <stdlib.h>
#include <string.h>

#include "array-dictionary.h"

// Constructor
ArrayDictionary::ArrayDictionary()
{
	maxNumber = 100;
	currentNumber = 0;
	this->array = (ArrayDictionaryNode *) malloc (sizeof(ArrayDictionaryNode) * maxNumber);
}

// Add a record to the dictionary. Returns false if key already exists
bool
ArrayDictionary::addRecord( KeyType key, DataType record)
{
	
	for(int i = 0 ; i < currentNumber ; i++) {
		
		if(!strcmp(array[i].key , key)) {
			
			array[i].data = record;
			return false;
			
		}
		
	}
	
    if(currentNumber >= maxNumber) {
		 
		maxNumber *= 2;
		array = (ArrayDictionaryNode *) realloc(array , sizeof(ArrayDictionaryNode) *  maxNumber);
		
	}
	
	array[currentNumber].key = strdup(key);
	array[currentNumber].data = record;
	currentNumber++;
	
	return true;
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
ArrayDictionary::findRecord( KeyType key)
{
    for(int i = 0 ; i < currentNumber ; i++) {
		
		if(!strcmp(array[i].key , key)) {
			
			return array[i].data;
			
		}
		
	}

	return NULL;
}

// Removes one element from the table
bool
ArrayDictionary::removeElement(KeyType key)
{
	
	if(currentNumber == 0) {
		
		return false;
		
	}
        // Add your code here
	for(int i = 0 ; i < currentNumber ; i++) {
		
		if(!strcmp(array[i].key , key)) {
			
			free(array[i].key);
			array[i].key = array[currentNumber-1].key;
			array[i].data = array[currentNumber-1].data;
			currentNumber--;
			
			return true;
		}
		
	}

	return false;
	
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
ArrayDictionary::keys(int * n)
{
        // Add yoru code here
	*n = currentNumber;
		
	if(currentNumber == 0) {	
	
	
		return NULL;
	}
	
	KeyType * ret = new KeyType[*n];
	
	for (int i = 0 ; i < *n ; i++) {
		
		ret[i] = strdup(array[i].key);
		
	}
	
	return ret;
}

bool
ArrayDictionary::addUnique(KeyType key, DataType record) {
	
	if(currentNumber >= maxNumber) {
		 
		maxNumber *= 2;
		array = (ArrayDictionaryNode *) realloc(array , sizeof(ArrayDictionaryNode) *  maxNumber);
		
	}
	
	array[currentNumber].key = strdup(key);
	array[currentNumber].data = record;
	currentNumber++;
	
	return true;
	
}

//destructor
ArrayDictionary::~ArrayDictionary() {
	
	for(int i = 0 ; i < currentNumber ; i++) {
		
		free(array[i].key);
	}
	
	
}