#include <iostream>
#include "btree.h"
#include <string>
#include <fstream>
using namespace std;

bool testFile(string name);
void fillTree(string name, BTree& b);
int main(int argc, char const *argv[]) {
  //name seq file, name btree file to create
  //print preorder traversal heights should be included
  if(argc!=3)
  {
    cerr<<"Improper Command Line Input"<<endl;
    return 1;
  }
  string seqFileName=argv[1];
  string treeFileName=argv[2];
  if( !testFile(seqFileName) )
  {
    cerr<<"File does not exist"<<endl;
    return 1;
  }
  BTree b(treeFileName,'C');
  fillTree(seqFileName,b);
  b.printTree();
  b.close();
  return 0;
}
bool testFile(string name) //checks if file exists
{
  fstream file(name.c_str(), ios::in);
  if(file)
  {
    return true;
  }
  return false;
}
void fillTree(string name, BTree& b)
{
  ifstream file(name.c_str());
  int count=0;
  Album temp;
  while(file >>temp)
  {
    cout<<"Insertion Attempt for: "<<temp.getUPC()<<": ";
    b.insert(temp);
    count++;
    //if(count>10)
    //  b.printTree();
    //pause=true;
  }

}
