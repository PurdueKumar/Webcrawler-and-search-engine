
//
// CS251 Data Structures
// Hash Table
//

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Each hash entry stores a key, object pair
template <typename Data>
struct HashTableTemplateEntry {
  const char * _key;
  Data _data;
  HashTableTemplateEntry * _next;
};

// This is a Hash table that maps string keys to objects of type Data
template <typename Data>
class HashTableTemplate {
 public:
  // Number of buckets
  enum { TableSize = 2039};
  
  // Array of the hash buckets.
  HashTableTemplateEntry<Data> **_buckets;
  
  // Obtain the hash code of a key
  int hash(const char * key);
  
 public:
  HashTableTemplate();
  
  // Add a record to the hash table. Returns true if key already exists.
  // Substitute content if key already exists.
  bool insertItem( const char * key, Data data);

  // Find a key in the dictionary and place in "data" the corresponding record
  // Returns false if key is does not exist
  bool find( const char * key, Data * data);

  // Removes an element in the hash table. Return false if key does not exist.
  bool removeElement(const char * key);

  // Returns the data that corresponds to this index.
  // Data is initialized to 0s if the entry does not exist
  Data operator[] (const char * &key);
};

template <typename Data>
int HashTableTemplate<Data>::hash(const char * key)
{
  int h = 0;
  const char * p = key;
  while (*p) {
    h += *p;
    p++;
  }
  return h % TableSize;
}

template <typename Data>
HashTableTemplate<Data>::HashTableTemplate()
{
  _buckets = new HashTableTemplateEntry<Data>* [TableSize];
  
  for(int i = 0; i < TableSize ; i++) {
	  
	  _buckets[i] = NULL;
	  
  }
  
}

template <typename Data>
bool HashTableTemplate<Data>::insertItem( const char * key, Data data)
{
  
	int h = hash(key);
	
	HashTableTemplateEntry<Data> * t = _buckets[h];
	
	while(t != NULL) {
		
		if(!strcmp(t->_key,key)) {
			
			t->_data = data;
			return true;
			
		}
		
		t = t->_next;
	}
	
	
	HashTableTemplateEntry<Data> * n = new HashTableTemplateEntry<Data>;

	n->_data = data;
	n->_key = strdup(key);
	n->_next = NULL;

	HashTableTemplateEntry<Data> * p = _buckets[h];
	
	_buckets[h] = n;
		
	
	if(p != NULL) {
		
		_buckets[h]->_next = p;
		
	}

  return false;
}

template <typename Data>
bool HashTableTemplate<Data>::find( const char * key, Data * data)
{
  HashTableTemplateEntry<Data> * n;
	
	int h;
	
	h = hash(key);
	
	n = _buckets[h];
	
	if(n == NULL) {
		
		return false;
		
	}
	
	do{
		
		if(!strcmp(n->_key,key)) {
			
			*data = n->_data; 
			return true;
			
		}
		
		n = n->_next;
		
	}while(n != NULL);

	return false;
}

template <typename Data>
Data HashTableTemplate<Data>::operator[] (const char * &key) {
  Data d;
  
  if(find(key , &d)) {
		
	return d;
  }
  
  else{
	  
	  return 0;
	  
  }
}

template <typename Data>
bool HashTableTemplate<Data>::removeElement(const char * key)
{
  HashTableTemplateEntry<Data> * n;
  HashTableTemplateEntry<Data> * p;
	
	int h = hash(key);
	
	n = _buckets[h];
	p = _buckets[h];
	
	if(n == NULL) {
		
		return false;
		
	}
	
	do{
		
		if(!strcmp(n->_key,key)) {
			
			p->_next = n->_next;
			
			delete n->_key;
			//delete n->_data;
			delete n->_next;
			delete n;
			
			return true;
		}
		
		p = n;
		n = n->_next;
		
	}while(n != NULL);

  return false;
}

template <typename Data>
class HashTableTemplateIterator {
  int _currentBucket;
  HashTableTemplateEntry<Data> *_currentEntry;
  HashTableTemplate<Data> * _hashTable;
 public:
  HashTableTemplateIterator(HashTableTemplate<Data> * hashTable);
  bool next(const char * & key, Data & data);
};

template <typename Data>
HashTableTemplateIterator<Data>::HashTableTemplateIterator(HashTableTemplate<Data> * hashTable)
{
  this->_hashTable = hashTable;
  this->_currentBucket = 0;
  this->_currentEntry = _hashTable->_buckets[0];
}

template <typename Data>
bool HashTableTemplateIterator<Data>::next(const char * & key, Data & data)
{
  if(_currentEntry != NULL) {
		
		if(_currentEntry->_next != NULL) {
			
			_currentEntry = _currentEntry->_next;
			key = _currentEntry->_key;
			data = _currentEntry->_data;
			return true;
			
		}
		
		_currentBucket++;
		if(_currentBucket == this->_hashTable->TableSize) {
		  
		  return false;
	    }
		
		_currentEntry = _hashTable->_buckets[_currentBucket];
		
	}
	
  while(_currentEntry == NULL) {
	  
	    _currentBucket++;
		if(_currentBucket == this->_hashTable->TableSize) {
		  
		  return false;
	    }
		
		_currentEntry = _hashTable->_buckets[_currentBucket];
	  
  }
  
  key = _currentEntry->_key;
  data = _currentEntry->_data;
  
  return true;
}
