#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <stack>
using namespace std;
class lump{
public:
  uint32_t eoffset;
  uint32_t length;
  string name;
  vector<unsigned char> content;
  lump *parent;
  string fpath;
  vector<lump*> child;
  bool em;
  int flag;
  lump(uint32_t eoffset,uint32_t length,const string &name);
};
lump* getdetails(uint32_t off,const string &path);
class Wad{
private:
  uint32_t numDes;
  uint32_t offset;
  uint32_t ioffset;
  char *header=new char[12];
  vector<unsigned char> data;
public:
  lump* froot;
  Wad(vector<unsigned char> data);
  static Wad* loadWad(const std::string &path);
  char* getMagic();
  bool isContent(const std::string &path);
  int getSize(const std::string &path);
  int getContents(const std::string &path, char *buffer, int length, int offset=0);
  int getDirectory(const std::string &path, std::vector<std::string> *directory);
};
  uint32_t* getOff(const string &path);
  uint32_t btol(char *arr);
  uint32_t* getol(uint32_t offset,vector<unsigned char> data);

