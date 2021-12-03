
#include <vector>
#include <set>
#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include "album.h"
using namespace std;

#ifndef BTREE_H
#define BTREE_H

class Album;
const int ORDER = 5;
typedef Album keyType;
typedef int IndexArray[ORDER+1];
typedef keyType ValueArray[ORDER];

struct BTNode
{
        int currSize;
        ValueArray contents; //4 albums
        IndexArray child={-1,-1,-1,-1,-1};  //5 children
};

struct Pair
{
        Album element;
        int loffset;
        int roffset;
        friend bool operator < (const Pair& p1, const Pair& p2)
           { return (p1.element < p2.element);  }
};

class BTree
{
 public:
	BTree(string filename,char status);
	void writeHeader ();
	void insert (keyType key);
	void reset (char * filename);
	void printTree();
	int getHeight(int recAddr);
	bool search (string key);
  void close();
	//keyType retrieve (string key);
	void totalio() const;
	int countLeaves();

 private:
	BTNode root;
  int rootAddr;
	fstream treeFile;
  int numNodes;
	int read;
	int write;

  void printTree (int recAddr);
	int findAddr (keyType key, BTNode t, int tAddr);
	int findpAddr(keyType key);//, BTNode t, int tAddr);
	void insert (keyType key, int recAddr);//, int oneAddr, int twoAddr);
  BTNode getNode (int recAddr);
	void printNode(int recAddr);
	bool isLeaf (int recAddr);
	bool isLeaf(BTNode t);
	int countLeaves(int recAddr);
  void splitNode (keyType& key,int recAddr,int oneAddr,int twoAddr);
	//bool search (string key, BTNode t, int tAddr);

};

#endif
