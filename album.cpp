#include "album.h"
#include <iostream>
using namespace std;

Album::Album (const Album& otherAlbum)
{
  strcpy (UPC, otherAlbum.UPC);
  strcpy (Artist, otherAlbum.Artist);
  strcpy (Title, otherAlbum.Title);
}
Album::Album (String upc, String artist, String title)
{
  strcpy (UPC, upc);
  strcpy (Artist, artist);
  strcpy (Title, title);
}
Album::Album ()
{
  strcpy (UPC, "\0");
  strcpy (Artist, "\0");
  strcpy (Title, "\0");
}
Album & Album::operator = (const Album& otherAlbum)
{
  strcpy (UPC, otherAlbum.UPC);
  strcpy (Artist, otherAlbum.Artist);
  strcpy (Title, otherAlbum.Title);
  return *this;
}
bool operator < (const Album& a, const Album& b)
{
  string tempA=a.UPC;
  string tempB=b.UPC;
  if(tempA.compare(tempB) < 0 )
  {
    return true;
  }
  else
  {
    return false;
  }
}
ifstream & operator >> (ifstream & stream, Album & C)
{
  string temp;
  getline(stream,temp);
  strcpy (C.UPC, temp.c_str());
  getline(stream,temp);
  strcpy (C.Artist, temp.c_str());
  getline(stream,temp);
  strcpy (C.Title, temp.c_str());
  return stream;
}
ostream & operator << (ostream & stream, Album & C)
{
  cout << C.UPC << "|" << C.Artist << "|"<< C.Title <<"|"<<endl;
  //cout << C.Artist << endl;
  //cout << C.Title << endl;
  return stream;
}
string Album::getUPC ()
{
  return UPC;
}
int Album::recordSize()
{
  return (strlen(UPC)+strlen(Artist)+strlen(Title));
}
