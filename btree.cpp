#include <vector>
#include <set>
#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include "btree.h"
using namespace std;

/////////////////////////////////////////////////////////////////////////////
BTree::BTree(string filename, char status)
{
  treeFile.open(filename.c_str(), ios::in | ios::out | ios::binary);
  rootAddr=-1;
  read=0;
  write=0;
  if(status=='C')//create
  {
    writeHeader();
    numNodes=0;
  }
  else if(status=='U')//update
  {
    BTNode dummy=getNode(0);
    rootAddr=dummy.child[0];
    numNodes=dummy.child[1];
  }

}
/////////////////////////////////////////////////////////////////////////////
void BTree::writeHeader ()
{
  BTNode dummy;
  dummy.child[0]=rootAddr;
  dummy.child[1]=numNodes;
  treeFile.seekp(0);
  treeFile.write((char*) &dummy, sizeof(BTNode) );
  write++;
}
/////////////////////////////////////////////////////////////////////////////
void BTree::insert (keyType key)
{
  insert(key,rootAddr);
}
/////////////////////////////////////////////////////////////////////////////
void BTree::insert (keyType key, int recAddr)//, int oneAddr, int twoAddr)
{
  //if empty
  if(rootAddr==-1)//or write==0
  {
    rootAddr=1;
    root.contents[0]=key;
    root.currSize=1;
    //write out root
    treeFile.seekg(1*sizeof(BTNode));
    treeFile.write((char*) &root, sizeof(BTNode));
    write++;
    numNodes++;
  }
  else
  {
    BTNode dummy=getNode(recAddr);
    if(isLeaf(dummy)) //if only root eexists
    {
      if(dummy.currSize==ORDER-1) // if root full
      {
        cout<<"Now Splitting Root"<<endl;
        splitNode(key, recAddr, -1,-1);
        //remember to adjust root and increase writes
      }
      else //just root and root not full
      {
        cout<<"Adding to Root"<<endl;
        //shift keys if needed
        int i=dummy.currSize-1; //start at last index
        while(i>=0 && key<dummy.contents[i])
        {
          dummy.contents[i+1]=dummy.contents[i];
          i--;
        }
        dummy.contents[i+1]=key;
        dummy.currSize++;
        treeFile.seekg(recAddr*sizeof(BTNode)); //update read?
        treeFile.write((char*) &dummy, sizeof(BTNode));
        write++;
      }
    }
    else //height>0 find where it goes by traversing tree
    {
      cout<<"Finding my place"<<endl;
      int addressLeaf=recAddr;
      int addressParent=recAddr;
      int i=dummy.currSize-1;
      while(!isLeaf(dummy))
      {
        if(i==0 && key<dummy.contents[i])
        {
          addressParent=addressLeaf;
          addressLeaf=dummy.child[i];
          //cout<<"child: "<<addressLeaf<<endl;
          dummy=getNode(dummy.child[i]);
          i=dummy.currSize-1;
        }
        else if(dummy.contents[i]<key)
        {
          addressParent=addressLeaf;
          addressLeaf=dummy.child[i+1];
          //cout<<"child: "<<addressLeaf<<endl;
          dummy=getNode(dummy.child[i+1]);
          i=dummy.currSize-1;
        }
        else
        {
          i--;
        }
      }
      //now at desired leaf
      if(dummy.currSize==ORDER-1) // if leaf full
      {
        cout<<"Splitting a Leaf"<<endl;
        //cout<<"addressParent: "<<addressParent<<endl;
        //cout<<"addressleaf: "<<addressLeaf<<endl;
        splitNode(key, addressLeaf, addressParent,-1);
        //remember to increase writes
      }
      else //not full leaf
      {
        cout<<"Joining an open Leaf"<<endl;
        //shift keys if needed
        int i=dummy.currSize-1; //start at second to last index
        while(i>=0 && key<dummy.contents[i])
        {
          dummy.contents[i+1]=dummy.contents[i];
          i--;
        }
        dummy.contents[i+1]=key;
        dummy.currSize++;
        treeFile.seekg(addressLeaf*sizeof(BTNode));
        treeFile.write((char*) &dummy, sizeof(BTNode));
        write++;
      }
    }
  }

}
/////////////////////////////////////////////////////////////////////////////
void BTree::splitNode (keyType& key,int recAddr,int oneAddr,int twoAddr)
{
  BTNode dummy=getNode(recAddr);
  BTNode newDummy;
  if(twoAddr!=0)
  {
    int i=dummy.currSize-1; //should be 4-1
    while(i>=0 && key<dummy.contents[i])
    {
      dummy.contents[i+1]=dummy.contents[i];
      i--;
    }
    dummy.contents[i+1]=key;
  }
  for(int j=0;j<2;j++) //copy last 2 contents to new node
  {
    newDummy.contents[j]=dummy.contents[j+3];
  }
  //change size of original node
  dummy.currSize=2;
  newDummy.currSize=2;

  if(!isLeaf(dummy)) //copy over right hand children
  {
    for(int j=0;j<3;j++)
    {
      newDummy.child[j]=dummy.child[j+3];
    }
  }

  if(recAddr==rootAddr) //update root by creating new parent
  {
    root.contents[0]=dummy.contents[2];
    root.child[0]=recAddr;
    numNodes++;
    root.child[1]=numNodes;
    treeFile.seekg(numNodes*sizeof(BTNode));
    //change root Size
    root.currSize=1;
    //write out all nodes
    treeFile.write((char*) &newDummy, sizeof(BTNode)); //right child
    numNodes++;
    rootAddr=numNodes;         //update root location
    treeFile.write((char*) &root, sizeof(BTNode));  //new root
    treeFile.seekg(recAddr*sizeof(BTNode)); //old root location is now left child
    treeFile.write((char*) &dummy, sizeof(BTNode));
    write+=3;
  }
  else //a parent exists to link to
  {
    BTNode parent=getNode(oneAddr);
    int k=parent.currSize-1; //should be 4-1
    while(k>=0 && dummy.contents[2]<parent.contents[k])
    {
      parent.contents[k+1]=parent.contents[k];
      k--;
    }

    int z=5;
    while(z>k+1)//key<dummy.contents[i])
    {
      parent.child[z]=parent.child[z-1];
      z--;
    }
    parent.child[k+1]=recAddr;
    numNodes++;
    parent.child[k+2]=numNodes;
    //cout<<"Child of Parent: "<<parent.child[0]<<parent.child[1]<<parent.child[2]<<parent.child[3]<<parent.child[4]<<endl;
    treeFile.seekg(numNodes*sizeof(BTNode));

    treeFile.write((char*) &newDummy, sizeof(BTNode)); //right child
    treeFile.seekg(recAddr*sizeof(BTNode)); //old root location is now left child
    treeFile.write((char*) &dummy, sizeof(BTNode));
    write+=2;

    parent.contents[k+1]=dummy.contents[2];
    parent.currSize++;
      treeFile.seekg(oneAddr*sizeof(BTNode));
      treeFile.write((char*) &parent, sizeof(BTNode));
      write++;

    if(parent.currSize==5)
    {

      splitNode( dummy.contents[2],oneAddr,findpAddr(dummy.contents[2]),0 );
    }

  }
}
/////////////////////////////////////////////////////////////////////////////
void BTree::reset (char * filename)
{

}
/////////////////////////////////////////////////////////////////////////////
void BTree::printTree() //public print
{
    cout << "-------- B-tree of height " << getHeight(rootAddr) << " --------" << endl;
    printTree(rootAddr);
}
/////////////////////////////////////////////////////////////////////////////
void BTree::printTree (int recAddr) //private print
{
  if (recAddr != -1)
  {
    BTNode dummy = getNode(recAddr);
    printNode(recAddr);
    //cout<<endl;
    for (int i = 0;  i <= dummy.currSize;  i++)
    {
      printTree(dummy.child[i]);
    }
  }
}
/////////////////////////////////////////////////////////////////////////////
void BTree::printNode(int recAddr)
{
  BTNode dummy=getNode(recAddr);
  cout<<"    *** node of size "<<dummy.currSize<<" ***"<<endl;
  for (int i = 0;  i < dummy.currSize;  i++)
  {
    cout<<dummy.contents[i];
  }
}
/////////////////////////////////////////////////////////////////////////////
int BTree::getHeight(int recAddr)
{
  int h=0;
  BTNode dummy=getNode(recAddr);
  while( !isLeaf(dummy) )
  {
    dummy=getNode(dummy.child[0]);
    h++;
  }
  return h;
}
/////////////////////////////////////////////////////////////////////////////
bool BTree::search (string key)
{
  BTNode dummy=getNode(rootAddr);
  int i=dummy.currSize-1;
  while(i>=0)
  {
    if( dummy.contents[i].getUPC()==key )
    {
      return true;
    }
    else if(i==0 && !isLeaf(dummy.child[i]) && key<dummy.contents[i].getUPC())
    {
      dummy=getNode(dummy.child[i]);
      i=dummy.currSize-1;
    }
    else if(dummy.contents[i].getUPC()<key && !isLeaf(dummy.child[i]))
    {
      dummy=getNode(dummy.child[i+1]);
      i=dummy.currSize-1;
    }
    else
    {
      i--;
    }
  }
  return false;
}
/////////////////////////////////////////////////////////////////////////////
void BTree::close ()
{
  writeHeader();
  treeFile.close();
}
/////////////////////////////////////////////////////////////////////////////
void BTree::totalio() const
{
  cout<<"reads: "<<read<<endl;
  cout<<"writes: "<<write<<endl;
}
/////////////////////////////////////////////////////////////////////////////
int BTree::countLeaves() //public count leaves
{
  if(rootAddr!=-1)
    return countLeaves(rootAddr);
  else
    return -1;
}
/////////////////////////////////////////////////////////////////////////////
int BTree::countLeaves(int recAddr) //private count leaves
{
  int count=0;
  BTNode dummy=getNode(recAddr);
  bool leaf=true;
  for(int i=0; i<=dummy.currSize;i++)
  {
    if(dummy.child[i]!=-1)
    {
      leaf=false;
      count+=( countLeaves(dummy.child[i]) );
    }
  }
  if(leaf)
  {
    return 1;
  }
  else
  {
    return count;
  }
}
/////////////////////////////////////////////////////////////////////////////
int BTree::findAddr (keyType key, BTNode t, int tAddr)
{
  return -1;
}
/////////////////////////////////////////////////////////////////////////////
int BTree::findpAddr(keyType key)//, BTNode t, int tAddr)
{
  bool found=false;
  int addressLeaf=rootAddr;
  int addressParent=rootAddr;
  BTNode dummy=getNode(rootAddr);
  int i=dummy.currSize-1;
  while(!found)
  {
    if( !(dummy.contents[i]<key) && !(key<dummy.contents[i]) )//==
    {
      found=true;
    }
    else if(i==0 && key<dummy.contents[i])
    {
      addressParent=addressLeaf;
      addressLeaf=dummy.child[i];
      //cout<<"child: "<<addressLeaf<<endl;
      dummy=getNode(dummy.child[i]);
      i=dummy.currSize-1;
    }
    else if(dummy.contents[i]<key)
    {
      addressParent=addressLeaf;
      addressLeaf=dummy.child[i+1];
      //cout<<"child: "<<addressLeaf<<endl;
      dummy=getNode(dummy.child[i+1]);
      i=dummy.currSize-1;
    }
    else
    {
      i--;
    }
  }
  return addressParent;
}
/////////////////////////////////////////////////////////////////////////////
BTNode BTree::getNode (int recAddr)
{
  //read from offset
  BTNode dummy;
  treeFile.seekg(recAddr*sizeof(BTNode));
  treeFile.read( (char*) &dummy, sizeof(dummy) );
  read++;
  //cout<<dummy.contents[0]<<endl;
  return dummy;
}
/////////////////////////////////////////////////////////////////////////////
bool BTree::isLeaf (int recAddr)
{
  BTNode dummy=getNode(recAddr);
  return isLeaf(dummy);
}
/////////////////////////////////////////////////////////////////////////////
bool BTree::isLeaf(BTNode t)
{
  for(int i=0; i<=t.currSize;i++)
  {
    if(t.child[i]!=-1)
    {
      return false;
    }
  }
  return true;
}
