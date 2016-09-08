
// Implementation of a dictionary using a hash table

#include <stdlib.h>
#include <string.h>
#include "hash-dictionary.h"

// Constructor
HashDictionary::HashDictionary()
{
	int nElements = 0;
	
	for (int i = 0 ; i < MaxBuckets ; i++) {
		
		buckets[i] = NULL;
		
	}
}
	
int
HashDictionary::hash(KeyType key) {
  // Add your code here
  int sum = 0;
  int len = strlen(key);
  for (int i = 0; i < len; i++) {
		sum += i*key[i];
  }
  return sum%MaxBuckets;
}

// Add a record to the dictionary. Returns false if key already exists
bool
HashDictionary::addRecord( KeyType key, DataType record)
{
  
  int h = hash(key);
  
  if(buckets[h] == NULL) {
	  
	  buckets[h] = new HashNode;
	  
	  buckets[h]->key = strdup(key);
	  buckets[h]->data = record;
	  buckets[h]->next = NULL;
	  nElements++;
	
	  return true;
	  
  }
  
  HashNode * tmp = buckets[h];
  
  if(!(strcmp(tmp->key , key))) {
	  
	  tmp->data = record;
	  return false;
	  
  }
  
  while(tmp->next != NULL) {
	  
	  if(!strcmp(tmp->next->key , key)) {
		  
		  tmp->next->data = record;
		  return false;
		  
	  }
	  
	  tmp = tmp->next;
	  
  }
  
  tmp->next = new HashNode;
  tmp->next->next = NULL;
  tmp->next->key = strdup(key);
  tmp->next->data = record;
  nElements++;
  
  return true;
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
HashDictionary::findRecord( KeyType key)
{
	
	int h = hash(key);
	
	HashNode * t = buckets[h];
	
	while(t != NULL) {
		
		if(!strcmp(t->key , key)) {
			
			return t->data;
			
		}
		
		t = t->next;
	}
	
	return NULL;
}

// Removes one element from the table
bool
HashDictionary::removeElement(KeyType key)
{
	int h = hash(key);
	
	HashNode * t = buckets[h];
	HashNode * p = buckets[h];
	HashNode * del;
	
	while(t != NULL) {
		
		if(!strcmp(t->key , key)) {
			
			p->next = t->next;
			free((char *)t->key);
			free(t);
			if(t == buckets[h]) {
				
				buckets[h] == NULL;
				
			}
			nElements--;
			return true;
		}
		
		p = t;
		t = t->next;
		
	}
  
	return false;
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
HashDictionary::keys(int * n)
{
	KeyType * a = (KeyType *) malloc(nElements * sizeof(KeyType));

	*n = nElements;
	int i = 0;
	for (int h = 0; h < MaxBuckets; h++) {
		HashNode * n = buckets[h];
		while (n!=NULL) {
			a[i] = n->key;
			i++;
			n = n->next;
		}
	}
	*n = nElements;
	return a;
}

bool
HashDictionary::addUnique(KeyType key, DataType record) {

	return false;

}

