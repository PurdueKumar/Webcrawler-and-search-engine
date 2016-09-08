
// Implementation of a dictionary using an AVL tree
// AVL Trees are balanced binary trees 

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "avl-dictionary.h"

bool debug = false;

// Constructor
AVLDictionary::AVLDictionary()
{
	root = NULL;
	nElements = 0;
}

// Add a record to the dictionary. Returns false if key already exists
bool
AVLDictionary::addRecord( KeyType key, DataType record)
{

	
	if ( debug) {
		printf("------------------------------------\n");
		printf("addRecord(\"%s\",%ld)\n",  key, (long) record);
		printf("---------- Before -----------------\n");
		printNode("", root, 0);
	}
	
	// Add your implementation here
	
	if(nElements == 0) {
		
		root = new AVLNode;
		root->key = strdup(key);
		root->data = record;
		root->parent = NULL;
		root->left = NULL;
		root->right = NULL;
		root->height = 1;
		nElements++;
		
		if (debug) {
		checkRecursive(root);
		
		printf("---------- root added -----------------\n");
		printNode("", root, 0);
		
		
		}
		return true;
	}
		
	AVLNode * n = root;
	AVLNode * p = root;
	while(n != NULL) {
		
		if(!strcmp(n->key , key)) {

			n->data = record;
			return false;

		}
		
		else if(strcmp(key , n->key) < 0) {
			
			p = n;
			n = n->left;
			
		}
		
		else {
			
			p = n;
			n = n->right;
			
		}
		
	}
	
	if(strcmp(key , p->key) < 0) {
		
		p->left = new AVLNode;
		p->left->key = strdup(key);
		p->left->data = record;
		p->left->parent = p;
		p->left->left = NULL;
		p->left->right = NULL;
		p->left->height = 1;
		nElements++;
		
	}
	
	else {
		
		p->right = new AVLNode;
		p->right->key = strdup(key);
		p->right->data = record;
		p->right->parent = p;
		p->right->left = NULL;
		p->right->right = NULL;
		p->right->height = 1;
		nElements++;
		
	}
		
	
	//Find node to insert into 
	//Node does not exist. Create it.
	//Height might not be valid anymore.
	//We need to restructure .
	
	

	if ( debug) {
		printf("---------- Before Restructure -----------------\n");
		printNode("", root, 0);
	}
	
	// Call restructure
	//restructure updates the heights of the nodes in the tree when a new node is added
	restructure(p);
	
	
	if (debug) {
		
		printf("---------- After Restructure -----------------\n");
		printNode("", root, 0);
		
		checkRecursive(root);
	}
		
	return true;
}

void
AVLDictionary::restructure(AVLNode * n) {
	//Balance the tree starting at n 
	
	AVLNode * z = n;
	AVLNode * y;
	AVLNode * x;
	AVLNode * p = z->parent;
	AVLNode * a;
	AVLNode * b;
	AVLNode * c;
	AVLNode * t0;
	AVLNode * t1;
	AVLNode * t2;
	AVLNode * t3;

	//variables to store height of right and left subtrees
	int rh;
	int lh;
	
	//variable to store tree rotation case
	int type;
	
	//Go upwards until root is found
	
	while (z != NULL) {
		
		//update height of z and store heights of right and left node
		z->right == NULL ? rh = 0 : rh = z->right->height;
		z->left == NULL ? lh = 0 : lh = z->left->height;
		rh > lh ? z->height = rh+1 : z->height = lh+1;
		
		//check if subtree at z is balanced
		if(abs(rh-lh) > 1) {
		
			//find y and x, assign t0,t1,t2 and t3 and find the subtree rotation case
			if(rh > lh) {
				
				y = z->right;
				t0 = z->left;
				
				y->right == NULL ? rh = 0 : rh = y->right->height;
				y->left == NULL ? lh = 0 : lh = y->left->height;
				
				if(rh > lh) {
					
					x = y->right;
					t1 = y ->left;
					t2 = x->left;
					t3 = x->right;
					type = 1;
					
				}
				
				else {
					
					x = y->left;
					t1 = x->left;
					t2 = x->right;
					t3 = y->right;
					type = 3;
					
				}
				
			}
			
			else {
				
				y = z->left;
				t3 = z->right;
				
				y->right == NULL ? rh = 0 : rh = y->right->height;
				y->left == NULL ? lh = 0 : lh = y->left->height;
				
				if(rh > lh) {
					
					x = y->right;
					t0 = y->left;
					t1 = x->left;
					t2 = x->right;
					type = 4;
					
				}
				
				else {
					
					x = y->left;
					t0 = x->left;
					t1 = x->right;
					t2 = y->right;
					type = 2;
					
				}
				
				
			}
			
			//assign a,b,c according to the subtree rotation case
			switch(type) {
				
				case 1:
					
					a = z;
					b = y;
					c = x;
					
					break;
				
				case 2:
				
					a = x;
					b = y;
					c = z;
					
					break;
			
				case 3:
				
					a = z;
					b = x;
					c = y;
					break;
					
				case 4:
				
					a = y;
					b = x;
					c = z;
					break;
			}
			
			//rotate nodes and place the subtree branches t0,t1,t2 and t3 to the new subtree
			a->left = t0;
			if(t0 != NULL) {
				
				t0->parent = a;
				lh = t0->height;
				
			}
			else {
				
				lh = 0;
				
			}
			
			a->right = t1;
			if(t1 != NULL) {
				
				t1->parent = a;
				rh = t1->height;
				
			}
			
			else {
				
				rh = 0;
				
			}
			//update height of a
			rh > lh ? a->height = rh+1 : a->height = lh+1;
			
			c->left = t2;
			if(t2 != NULL) {
				
				t2->parent = c;
				lh = t2->height;
				
			}
			else {
				
				lh = 0;
				
			}
			
			c->right = t3;
			if(t3 != NULL) {
				
				t3->parent = c;
				rh = t3->height;
				
			}
			
			else {
				
				rh = 0;
				
			}
			//update height of c
			rh > lh ? c->height = rh+1 : c->height = lh+1;
			
			
			b->left = a;
			a->parent = b;
			lh = a->height;
			
			b->right = c;
			c->parent = b;
			rh = c->height;
			
			//if the rotated subtree still has imbalance there is a problem with the restructuring code
			if(abs(rh -lh) > 1) {
				
				printf("BUG IN RESTRUCTURE CODE!\n");
				
			}
			
			//update height of b
			rh > lh ? b->height = rh+1 : b->height = lh+1;
			
			//check if originally z was the root of the tree. i.e. z->parent was NULL (only the root will have a NULL parent)
			if(p == NULL) {
				
				root = b;
				//update b->parent
				b->parent = NULL;
				
			}
			
			//check which side of the parent node b will be on
			else if(strcmp(b->key , p->key) < 0) {
				
				p->left = b;
				//update b->parent
				b->parent = p;
				
				
			}
			
			else {
				
				p->right = b;
				//update b->parent
				b->parent = p;
				
			}
		}	
		
		//go up the tree to check if the rest of the tree is balanced
		z = p;
		if(z == NULL) {
			
			break;
			
		}
		p = z->parent;
	}
	

	
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
AVLDictionary::findRecord( KeyType key)
{
    AVLNode * x = root;
	
	while (x != NULL) {
	
		if(!strcmp(key , x->key)) {
			
			return x->data;
		 
		}
	
		if(strcmp(key , x->key) < 0) {
		
			x = x->left;
		
		}
		
		else {
			
			x = x->right;
			
		}
	}

	return NULL;
}

// Removes one element from the dictionary
bool
AVLDictionary::removeElement(KeyType key)
{
	if (debug) {
		printf("------------------------------------\n");
		printf("removeElement(\"%s\")\n",  key);
		printf("---------- Before -----------------\n");
		printNode("", root, 0);
	}


	// Add your implementation here
	
	if(nElements == 0) {
		
		return false;
		
	}
	AVLNode * r = root;
	AVLNode * rep;
	AVLNode * del;
	AVLNode * res;
	bool found = false;
	while( r != NULL) {
		
		if(!strcmp(key , r->key)) {
			
			found = true;
			break;
		 
		}
	
		if(strcmp(key , r->key) < 0) {
		
			r = r->left;
		
		}
		
		else {
			
			r = r->right;
			
		}
	}
		
	if(!found) {
			
		return false;
			
	}
	
	del = r;
	r = r->parent;
	
	if(nElements == 1) {
		
		del = root;
		root = NULL;
		
		free((char *)del->key);
		delete del;
		nElements--;
	}
		
	else if(del->right == NULL) {
		
		if(del->left == NULL) {
			
			if(del == r->right) {
				
				r->right = NULL;
			}
			else {
				
				r->left = NULL;
				
			}
			
			free((char *)del->key);
			delete del;
			nElements--;
			
			restructure(r);
		}
		
		else {
			
			rep = del->left;
			
			while(rep->right != NULL) {
				
				rep = rep->right;
			}
			
			restructure(replaceNode(rep , del));
			
		}
		
	}
	
	else { 
	
		rep = del->right;
		while(rep->left != NULL) {
			
			rep = rep->left;
			
		}
	
		restructure(replaceNode(rep , del));
	}
	
	if (debug) {
		printf("---------- After -----------------\n");
		printNode("", root, 0);

		checkRecursive(root);
	}
	
	return true;
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
AVLDictionary::keys(int * n)
{
	KeyType * a = (KeyType *) malloc(nElements * sizeof(KeyType));
	*n = 0;
	addKeysRecursive(root, a, n);

	return a;
}

// Add keys recursively
void
AVLDictionary::addKeysRecursive(AVLNode * node, KeyType * a, int * n) {
	if (node==NULL) {
		return;
	}
	
	a[*n] = node->key;
	(*n)++;
	addKeysRecursive(node->left, a, n);
	addKeysRecursive(node->right, a, n);
}

//branches of Node r are inherited by r->parent and r is replaced with d
//parent of r is returned as it is the node on which restructure has to be called
AVLNode *
AVLDictionary::replaceNode(AVLNode * r, AVLNode * d) {
	
	AVLNode * p = r->parent;
	
	//variable to store branch of r
	AVLNode * b;
	
	//the node that is replacing d has to have one child that is null
	//the node that is not null is assigned to b;
	if(r->left == NULL) {
		
		b = r->right;
		
	}
	
	else {
		
		b = r->left;
		
	}
	
	//checking to see which side of the parent r is on
	//parent inherit's r's branches
	if(p->right == r) {
		
		p->right = b;
		
	}
	
	else {
		
		p->left = b;
		
	}
	
	//data and key of r is copied to d and r is deleted
	free((char *)d->key);
	d->key = strdup(r->key);
	d->data = r->data;
	
	free(r);
	nElements--;
	
	return p;
}

bool
AVLDictionary::addUnique(KeyType key, DataType record) {

	return false;

}

