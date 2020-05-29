/****************************************************************************
  FileName     [ dbJson.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Json member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <cmath>
#include <string>
#include <algorithm>
#include <vector>
#include "dbJson.h"
#include "util.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream&
operator << (ostream& os, const DBJsonElem& j)
{
   os << "\"" << j._key << "\" : " << j._value;
   return os;
}

istream& operator >> (istream& is, DBJson& j)
{
   // TODO: to read in data from Json file and store them in a DB 
   // - You can assume the input file is with correct JSON file format
   // - NO NEED to handle error file format
   assert(j._obj.empty());
   int k=0;
   
   string a;
   is>>a;
   vector<string> buff;
   while(true){
      string t;
      is>>t;
      int index=0;
      if (!t.empty())
         while((index=t.find(' ',index))!= string::npos){
               t.erase(index,1);
         }
      if(t == "}"){
         break;
      }
      else if(t==":"||t==","||t=="\n"){
         continue;
      }
      else{
         t.erase(remove(t.begin(),t.end(),','),t.end());
         t.erase(remove(t.begin(),t.end(),'\"'),t.end());
         buff.push_back(t);
      }
      k=k+1;
   }
   for (int i=0;i<buff.size()/2;i++){
      DBJsonElem temp(buff[i*2],stoi(buff[i*2+1]));
      j._obj.push_back(temp);
   }
  
   return is;
}

ostream& operator << (ostream& os, const DBJson& j)
{
   // TODO
   if(j._obj.empty()){
      os<<"{"<<endl;
      
      os<<"}";
   return os;
   }

   os<<"{"<<endl;
   for(int i=0;i<j.size()-1;i++){
   os<<"  "<<j[i]<<","<<endl;
   }
   os<<"  "<<j[j.size()-1]<<endl;
   os<<"}";
   return os;
}

/**********************************************/
/*   Member Functions for class DBJsonElem    */
/**********************************************/
/*****************************************/
/*   Member Functions for class DBJson   */
/*****************************************/
void
DBJson::reset()
{
   // TODO
  vector<DBJsonElem>().swap(_obj);
}

// return false if key is repeated
bool
DBJson::add(const DBJsonElem& elm)
{
   // TODO
   for (int i=0;i<size();i++){
      if(elm.key()==_obj[i].key()){
         return false;
      }
   }
   _obj.push_back(elm);
   return true;
   
}

// return NAN if DBJson is empty
float
DBJson::ave() const
{
   // TODO
   if(size()==0){
      return NAN;
   }
   return (float)sum()/(float)size();
}

// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::max(size_t& idx) const
{
   // TODO
   int maxN = INT_MIN;
   if(_obj.empty()){
      idx=size();
      return maxN;
   }
   else{
   for(int i=0;i<size();i++){
      if (_obj[i].value()>maxN){
         maxN=_obj[i].value();
         idx=i;
      }
   }
   return  maxN;
   }
}

// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::min(size_t& idx) const
{
   // TODO
   int minN = INT_MAX;
   if(_obj.empty()){
      idx=size();
      return minN;
   }
   else{
   for(int i=0;i<size();i++){
      if (_obj[i].value()<minN){
         minN=_obj[i].value();
         idx=i;
      }
   }
   return  minN;
   }
}

void
DBJson::sort(const DBSortKey& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

void
DBJson::sort(const DBSortValue& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

// return 0 if empty
int
DBJson::sum() const
{
   // TODO
   int s = 0;
   if(!empty()){
      for(int i=0;i<size();i++){
         s+=_obj[i].value();
      }
   }
   return s;
}
bool 
DBJson::exist(string&str){
   for(unsigned int i=0;i<size();++i){
   if(_obj[i].key()==str)
      return true;
}
return false;
}
DBJsonElem
DBJson::find(string& str) const{
for(unsigned int i=0;i<size();++i){
   if(_obj[i].key()==str)
      return _obj[i];
}
}
