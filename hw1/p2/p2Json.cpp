/****************************************************************************
  FileName     [ p2Json.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define member functions of class Json and JsonElem ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2018-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <string>
#include "p2Json.h"
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

// Implement member functions of class Row and Table here
bool
Json::read(const string& jsonFile)
{
   fstream file;
   int j=0;
   string a;
   file.open(jsonFile.c_str(),ios::in);
   file>>a;
   while(true){
      string t;
      file>>t;
      
      int index=0;
      if (!t.empty())
         while((index=t.find(' ',index))!= string::npos){
               t.erase(index,1);
         }
      //cout<<t<<endl;
      if(t == "}"){
         break;
      }
      else if(t==":"||t==","||t=="\n"){
         continue;
      }
      else{
         t.erase(remove(t.begin(),t.end(),','),t.end());
         buff.push_back(t);
         //cout<<buff[j]<<endl;
      }
      j=j+1;
   }
   for (int i=0;i<buff.size()/2;i++){
      JsonElem temp(buff[i*2],stoi(buff[i*2+1]));
      _obj.push_back(temp);
      //cout<<temp.getkey()<<temp.getvalue()<<endl;
   }
   file.close();
   return true;  //TO DO
}
bool Json::print(){
      cout<<'{'<<endl;
      if (!_obj.empty()){
      for(int i=0;i <_obj.size()-1;i++) {
         cout<<"  "<<_obj[i].getkey()<<':'<<_obj[i].getvalue()<<','<<endl;
         }
      cout<<"  "<<_obj[_obj.size()-1].getkey()<<':'<<_obj[_obj.size()-1].getvalue()<<endl<<'}'<<endl;
      //cout<<'}'<<endl;
      }
      else {
         cout<<"}"<<endl;
      }
      return true;
   }
bool Json::add(const string& key,int value){
   string newkey="\"" + key + "\"";
   JsonElem ele(newkey,value);
    _obj.push_back(ele);
    return true;
}
int Json::getsum(){
      
      int total=0;
      for(int i=0;i<_obj.size();i++){
         total+=_obj[i].getvalue();
      }
      return total;
      }
int Json::maxnumber(){
      int tem=_obj[0].getvalue();
      for(int i=0;i<_obj.size();i++){
         if (_obj[i].getvalue()>tem){
            tem=_obj[i].getvalue();
         }
      }
      return tem;
   }
int Json::maxindex(){
      int tem=_obj[0].getvalue();      
      int max=0;
      for(int i=0;i<_obj.size();i++){
         if (_obj[i].getvalue()>tem){
            tem=_obj[i].getvalue();
            max=i;
         }
      }
      return max;
   }
int Json::minnumber(){
      int tem=_obj[0].getvalue();
      for(int i=0;i<_obj.size();i++){
         if (_obj[i].getvalue()<tem){
            tem=_obj[i].getvalue();
         }
      }
      return tem;
   }
int Json::minindex(){
      int tem=_obj[0].getvalue();
      int min=0;
      for(int i=0;i<_obj.size();i++){
         if (_obj[i].getvalue()<tem){
            tem=_obj[i].getvalue();
            min=i;
         }
      }
      return min;
   }
float Json::getave(){
      return (float)getsum()/(float)_obj.size();
   }
bool Json::min(){
   if (_obj.empty()){
         cout<<"Error: No element found!!"<<endl;
         return true;
   }
   else{
   cout<<"The minimum element is: { "<<_obj[minindex()].getkey()<<" : "<<minnumber()<<" }."<<endl;
   return true;
   }
}
bool Json::max(){
   if (_obj.empty()){
         cout<<"Error: No element found!!"<<endl;
         return true;
   }
   else{
   cout<<"The maximum element is: { "<<_obj[maxindex()].getkey()<<" : "<<maxnumber()<<" }."<<endl;
   return true;
}
}
bool Json::sum(){
   if (_obj.empty()){
         cout<<"Error: No element found!!"<<endl;
         return true;
   }
   else{
   cout<<"The summation of the values is: "<<getsum()<<"."<<endl;
   return true;
   }
}
bool Json::ave(){
   if (_obj.empty()){
         cout<<"Error: No element found!!"<<endl;
         return true;
   }
   else{
   cout<<"The average of the values is: "<<fixed<<setprecision(1)<<getave()<<"."<<endl;
   return true;
}
}
ostream&
operator << (ostream& os, const JsonElem& j)
{
   return (os << "\"" << j._key << "\" : " << j._value);
}

