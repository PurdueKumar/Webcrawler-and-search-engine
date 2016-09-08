
// Implementation of a dictionary using an array and binary search
// It will inherit from the ArrayDictionary

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bsearch-dictionary.h"

using namespace std;

// Constructor
BinarySearchDictionary::BinarySearchDictionary()
{
	sorted = false;
}

bool
BinarySearchDictionary::addRecord( KeyType key, DataType record) {
	sorted = false;
	return ArrayDictionary::addRecord(key, record);
}

bool
BinarySearchDictionary::removeElement(KeyType key) {

	if(!sorted) {

		sort();
		sorted = true;

	}

	int low = 0;
	int high = currentNumber -1;
	int mid = (high + low)/2;

	while(low < high) {

		if(!strcmp(array[mid].key , key)) {

			free(array[mid].key);
			array[mid].key = strdup(array[currentNumber-1].key);
			array[mid].data = array[currentNumber-1].data;
			currentNumber--;
			sorted = false;
			return true;

		}

		else if(strcmp(key , array[mid].key) < 0) {


			high = mid - 1;

			if(high >= currentNumber || high < 0) {

				break;

			}			

			if(!strcmp(key , array[high].key)) {

				free(array[high].key);
				array[high].key = strdup(array[currentNumber-1].key);
				array[high].data = array[currentNumber-1].data;
				currentNumber--;
				sorted = false;
				return true;

			}			


		}
		
		else {

			low = mid+1;
			if(low >= currentNumber || low < 0) {

				break;

			} 
			if(!strcmp(key , array[low].key)) {

				free(array[low].key);
				array[low].key = strdup(array[currentNumber-1].key);
				array[low].data = array[currentNumber-1].data;
				currentNumber--;
				sorted = false;
				return true;

			}
		} 

		mid = (low+high)/2;
	}

	return false;


}

// Find a key in the dictionary and return corresponding record or NULL
DataType
BinarySearchDictionary::findRecord( KeyType key)
{
	if (!sorted) {
		printf("\nSORTING!!!!!!!!!!!!\n");
		sort();
		sorted = true;
	}
	
	for(int i = 0 ; i < currentNumber ; i++) {

		printf("%s\n" , array[i].key);

	}
	printf("%d words\n" , currentNumber);

	// Use binary search

	int low = 0;
	int high = currentNumber-1;
	int mid = (high + low)/2;
	DataType ret;
	
	while(low < high) {
		
		if(!strcmp(key,array[mid].key)) {
			
			ret = array[mid].data;
			return ret;
			
		}
		
		else if((strcmp(key,array[mid].key)) < 0) {
			
			high = mid -1;
			if(high >= currentNumber || high < 0) {

				break;
	
			}

			if(!strcmp(key,array[high].key)) {
				
				ret = array[high].data;
				return ret;
				
			}
			
		}
		
		else if((strcmp(key,array[mid].key)) > 0) {
			
			low = mid + 1;

			if(low >= currentNumber || low < 0) {

				break;

			}
			
			if(!strcmp(key,array[low].key)) {
				
				ret = array[low].data;
				return ret;
				
			}
			
		}
		
		mid = (high+low)/2;
		
	}

	return NULL;
}


// Sort array using heap sort
void
BinarySearchDictionary::sort()
{
	ArrayDictionaryNode * heap = new ArrayDictionaryNode[currentNumber];
	int heapElems = 0;
	
	for (int i = 0; i < currentNumber; i++) {
		addHeap(heap , array[i] , heapElems);
	}

	int i = 0;
	while(heapElems > 0) {
		
		ArrayDictionaryNode node = rmHeap(heap , heapElems);
		
		free(array[i].key);
		array[i].key = strdup(node.key);
		array[i].data = node.data;
		i++;
	}

	delete[] heap;
	

}

void BinarySearchDictionary::addHeap(ArrayDictionaryNode* heap , ArrayDictionaryNode add , int &n) {
	
	heap[n].key = strdup(add.key);
	heap[n].data = add.data;
	
	int parent = (n-1)/2;
	int child = n;

	while(child > 0) {
		
		if(strcmp(heap[child].key , heap[parent].key) < 0) {
			
			ArrayDictionaryNode temp;
			temp.key = strdup(heap[child].key);
			temp.data = heap[child].data;
			
			free(heap[child].key);
			heap[child].key = strdup(heap[parent].key);
			heap[child].data = heap[parent].data;
			
			free(heap[parent].key);
			heap[parent].key = strdup(temp.key);
			heap[parent].data = temp.data;
			
		}
		
		else {
			
			break;
			
		}
		
		child = parent;
		parent = (parent-1)/2;
		
	}
	n++;

}

ArrayDictionaryNode BinarySearchDictionary::rmHeap(ArrayDictionaryNode* heap , int &n) {
	
	ArrayDictionaryNode ret;
	ret.key = strdup(heap[0].key);
	ret.data = heap[0].data;
	
	free(heap[0].key);
	heap[0].key = strdup(heap[n-1].key);
	heap[0].data = heap[n-1].data;
	
	n--;
	
	int parent = 0;
	int ileft = parent * 2 + 1;
	int iright = parent * 2 + 2;
	int imin = ileft;
	
	while(ileft < n) {
		
		if(iright < n && (strcmp(heap[iright].key,heap[ileft].key) < 0)) {
			
			imin = iright;
			
		}
		
		if(strcmp(heap[imin].key , heap[parent].key) < 0) {
			
			ArrayDictionaryNode temp;
			temp.key = strdup(heap[imin].key);
			temp.data = heap[imin].data;
			
			free(heap[imin].key);
			heap[imin].key = strdup(heap[parent].key);
			heap[imin].data = heap[parent].data;
			
			free(heap[parent].key);
			heap[parent].key = strdup(temp.key);
			heap[parent].data = temp.data;
			
		}
		
		else {
			
			break;
			
		}
		
		parent = imin;
		ileft = parent * 2 + 1;
		iright = parent * 2 + 2;
		imin = ileft;
		
		
	}
	
	return ret;
	
}

bool BinarySearchDictionary::addUnique(KeyType key, DataType record) {
	
	
	sorted = false;
	return ArrayDictionary::addUnique(key, record);
	
}
