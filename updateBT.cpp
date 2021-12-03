#include <iostream>
#include "btree.h"
#include <string>
#include <fstream>
using namespace std;

typedef char String[50];

bool testFile(string name);
void transaction(string name, BTree& b);
int main(int argc, char const *argv[]) {
  //name of tree file and name of transaction file
  //print number of reads and writes
  if(argc!=3)
  {
    cerr<<"Improper Command Line Input"<<endl;
    return 1;
  }
  string treeFileName=argv[1];
  string transactFile=argv[2];
  if( !testFile(treeFileName) || !testFile(transactFile) )
  {
    cerr<<"File does not exist"<<endl;
    return 1;
  }
  BTree b(treeFileName,'U');
  b.printTree();
  transaction(transactFile,b);
  b.printTree();
  b.totalio();
  return 0;
}
/////////////////////////////////////////////////////////////
bool testFile(string name) //checks if file exists
{
  fstream file(name.c_str(), ios::in);
  if(file)
  {
    return true;
  }
  return false;
}
//////////////////////////////////////////////////////////////
void transaction(string name, BTree& b)
{
  ifstream file(name.c_str(), ios::in);
  char instruction;
  Album temp;
  while(file >> instruction)
  {
    cout<<"instruction: "<<instruction<<endl;
    //file.get();
    if(instruction=='I')
    {
      //file.read((char*) &temp, sizeof(temp));
      file.get();
      //file >> temp;
      string tempUPC, tempart, tempalb;
      getline(file,tempUPC);
      cout<<tempUPC<<endl;
      getline(file,tempart);
      getline(file, tempalb);
      Album temp((char*)tempUPC.c_str(),(char*)tempart.c_str(),(char*)tempalb.c_str());
      //cout<<"Insertion Attempt for: "<<temp<<": ";
      b.insert(temp);
    }
    else if(instruction == 'S')
    {
      string tempUPC;
      file>>tempUPC;
      cout<<tempUPC<<endl;
      cout<<b.search(tempUPC)<<endl;
    }
  }
}
