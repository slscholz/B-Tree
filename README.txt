README Project 3
CSC 310
Samantha Scholz

Purpose:
	-create a B-tree structure (order 5) for given albums
	-update the tree for given transactions

album class:
	-3 constructors
		-set upc, artist, and title
		-copy information from another album
		-null album
	-operator overloading for 
		-comparisons of upcs
		-reading in and sending out album information
		-setting the values of one album equal to another
	-getter for UPC
	-getter for record size
	
mini client class : client.cpp
	-displays all functions of album.cpp

btree class : 
	-keeps track of root node address
	-inserts elements into B-tree
	-prints the tree
	-resets the tree
	-searches for specified keys
	-getter for height 
	-counts number of leaves
  -is leaf function
	
Printing the Tree:
	-while the address is not null the function traverses through the tree
	using a for loop and recursion 
	-recursive call is used for all the children of the node
	-calls a printNode function on each address

Insertions:
	-if empty create root and record root address
	-else traverse tree and insert as leaf if not full
	-if leaf is full split node

Splitting a Node:
	-add 2 more nodes
	-store the beginning half(2) of the keys of original node in new node A
	-store the later half(2) of the keys of orignal node in a new node B
  -call boolean is Leaf function
	-if the original node is an internal node link the last children(3) to the 
	node B and the beginning half of children(3) to node A
	-link A in place of the original node and B as the rightmost child of the original node's 
	predecessor
	-Add the middle key of the original node to the keys of the predecessor in order

	-change dummy node with root address if necessary
	-use a set to keep contents of root sorted

CountLeaves:
	-use a dummy node to get the node of the current address
	-loop through the children of the node
	-if a child is not null, flag it as an internal node and recursively call
	the function on its children
	-outside of the loop
		-return 1 if the node was not flagged as an internal node
		-otherwise return a count of the recursive calls indicating their leaves
   
is Leaf:
  -checks if children of node are all null

