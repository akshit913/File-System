#include"wad.h"
#include<fstream>
using namespace std;
Wad::Wad(vector<unsigned char> data){
  this->data=data;
}
Wad* Wad::loadWad(const string &path){
  ifstream infile;
  infile.open(path.c_str(),ios::binary);
  vector<unsigned char>buffer(istreambuf_iterator<char>(infile),{});
  Wad *object=new Wad(buffer);
  uint32_t *head=new uint32_t[2];
  head=getOff(path.c_str());
  object->numDes=head[0];
  object->offset=head[1];
  uint32_t a=(uint32_t)buffer.at(object->offset+7)<<24;
  a|=(uint32_t)buffer.at(object->offset+6)<<16;
  a|=(uint32_t)buffer.at(object->offset+5)<<8;
  a|=(uint32_t)buffer.at(object->offset+4);
  object->ioffset=a;
  //object->froot = getdetails(object->offset,path);
  return object;
}
char* Wad::getMagic(){
  char* magic=new char[4];
  for(int i=0;i<4;i++){
  magic[i]=this->data[i];
  }
  return magic;
}
uint32_t* getOff(const string &path){
  ifstream file;
  file.open(path.c_str(),ifstream::binary);
  vector<unsigned char>buffer(istreambuf_iterator<char>(file),{});
  uint32_t *arr=new uint32_t[2];
  uint32_t x=0;
  x=(uint32_t)buffer.at(7) << 24;
  x|=(uint32_t)buffer.at(6) << 16;
  x|=(uint32_t)buffer.at(5) << 8;
  x|=(uint32_t)buffer.at(4);
  uint32_t y=0;
  y=(uint32_t)buffer.at(11) << 24;
  y|=(uint32_t)buffer.at(10) << 16;
  y|=(uint32_t)buffer.at(9) << 8;
  y|=(uint32_t)buffer.at(8);
  arr[0]=x;
  arr[1]=y;
  return arr;
}
bool Wad::isContent(const string &path){
  Wad* obj;
  if(obj->froot->fpath == path){
    if(obj->froot->length == 0){return true;}
  }else{
    for(int i=0;i< obj->froot->child.size();i++){
      if(obj->froot->child.at(i)->fpath==path){
        if(obj->froot->child.at(i)->length==0){
          return true;
        }else{
          for(int j=0;j< obj->froot->child.size();j++){
            if(obj->froot->child.at(i)->fpath==path){
            return true;
            }else{return false;}
          }
        }
      }
    }
  }
}
lump::lump(uint32_t eoffset,uint32_t length,const string &name){
  this->eoffset=eoffset;
  this->length=length;
  this->name=name;
  this->em=false;
  this->flag=0;
}
  lump* getdetails(uint32_t off,const string &path){
  uint32_t x,y;
  string fname;
  vector<unsigned char> fcontent;
  ifstream infile;
  infile.open(path.c_str(),ios::binary);
  vector<unsigned char>buffer(istreambuf_iterator<char>(infile),{});
  vector <lump*> track;
  lump* root=new lump(0,0,"ROOT");
  for(int i=off;i<buffer.size();i=i+y){
    x = buffer.at(i+3) << 24;
    x|= buffer.at(i+2) << 16;
    x|= buffer.at(i+1) << 8;
    x|= buffer.at(i);
    y = buffer.at(i+6) << 24;
    y|= buffer.at(i+5) << 16;
    y|= buffer.at(i+4) << 8;
    y|= buffer.at(i+3);
    for(int j=0;j<8;j++){
     fname[i]=buffer.at(j+7);
    }
    lump* currFile=new lump(x,y,fname);
    root->fpath="/";
    if((currFile->name.substr(currFile->name.length()-6,currFile->name.length()-1)=="_START")&&(currFile->length==0)){
      if(track.at(track.size()-1)->flag >= 10){
        for(int n=0;n<track.size();n++){
          if(track.at(i)->name==currFile->name){
            delete track[i];
          }
        }
      }
      track.push_back(currFile);
      if(track.size()==1){
        currFile->parent=root;
      }else{
        currFile->parent=track.at(track.size()-2);
        if(currFile->parent->em){
          currFile->parent->flag++;
        }
      }i++;
      currFile->child.push_back(currFile);
      currFile->fpath=currFile->parent->fpath + currFile->name.substr(0,currFile->name.length()-7);
    }else if((currFile->name[0]=='E' && isdigit(currFile->name[1]) && currFile->name[2]=='M' && isdigit(currFile->name[3])) && currFile->length==0){
      if(track.at(track.size()-1)->flag >= 10){
        for(int n=0;n<track.size();n++){
          if(track.at(i)->name==currFile->name){
            delete track[i];
          }
        }
      }
      track.push_back(currFile);
      if(track.size()==1){
        currFile->parent=root;
      }else{
    	  currFile->parent=track.at(track.size()-2);
          if(currFile->parent->em){
            currFile->parent->flag++;
          }
      }
      currFile->em=true;
      currFile->fpath=currFile->parent->fpath + currFile->name;
      currFile->child.push_back(currFile);
      i++;
      }else if((currFile->name.substr(currFile->name.length()-4,currFile->name.length()-1) == "_END") && currFile->length == 0){
        if(track.at(track.size()-1)->flag >= 10){
        for(int n=0;n<track.size();n++){
          if(track.at(i)->name==currFile->name){
            delete track[i];
          }
        }
        }
        for(int j=0;j<track.size();j++){
          if(track.at(j)->name.substr(0,track.at(j)->name.length()-7) == currFile->name.substr(0,currFile->name.length()-7)){
            delete track[j];
          }
        }i++;
      }else{
        if(track.at(track.size()-1)->flag >= 10){
        for(int n=0;n<track.size();n++){
          if(track.at(i)->name==currFile->name){
            delete track[i];
          }
        }
        }
        if(track.size()==0){
          currFile->parent=root;
        }else{
          currFile->parent=track.at(track.size()-1);
          if(currFile->parent->em){
            currFile->parent->flag++;
          }
        }
        for(int m = currFile->eoffset + off;m < (currFile->eoffset + currFile->length + off);m++){
          currFile->content.push_back(buffer.at(m));
        }
        currFile->fpath = currFile->parent->fpath + currFile->name;
        currFile->parent->child.push_back(currFile);
      }
  }
return root;
}

