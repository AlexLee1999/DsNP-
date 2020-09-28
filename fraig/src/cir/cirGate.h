/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "cirDef.h"
#include "sat.h"

using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
class CirGate
{
public:
   CirGate(GateType t=UNDEF_GATE):_type(t),_line(0),_id(0),_name(""),_ismarked(0),
   _isindfs(0),_output(0){}
    ~CirGate() {}
   // Basic access methods
   string getTypeStr() const { 
    if(_type==UNDEF_GATE) return "UNDEF";
    else if(_type==PI_GATE) return "PI";
    else if(_type==PO_GATE) return "PO";
    else if(_type==AIG_GATE) return "AIG";
    else if(_type==CONST_GATE) return "CONST";
    else return "UNDEF_GATE";
    }
   unsigned getLineNo() const { return _line; }
   GateType getType()const{
     return _type;
   }
   bool isAig() const{
     if (_type==AIG_GATE){
       return true;
     }
     else{
       return false;
     }
   }
   // Printing functions
   //virtual void printGate() const = 0;
   void reportGate() const;
   void reportFanin(int level) const;
   void reportFanout(int level) const;
   void printfanin(int,int,int,bool)const;
   void printfanout(int,int,int,bool)const;

   void settype(GateType x){
     _type=x;
   }
   void setline(unsigned x){
     _line=x;
   }
   void setid(unsigned x){
     _id=x;
   }
   void setfanin(unsigned x){
     _fanin.push_back(x);
   }
   void setfaninlist(CirGate* x){
     _faninlist.push_back(x);
   }
   GateList faninlist()const{
     return _faninlist;
   }
   GateList& faninlistref(){
     return _faninlist;
   }
   GateList fanoutlist()const{
     return _fanoutlist;
   }
   GateList& fanoutlistref(){
     return _fanoutlist;
   }
   IdList& fanin(){
     return _fanin;
   }
   IdList& fanout(){
     return _fanout;
   }
   void setfanout(unsigned x){
     _fanout.push_back(x);
   }
   void setfanoutlist(CirGate* x){
     _fanoutlist.push_back(x);
   }
   void setname(string x){
     _name=x;
   }
   bool checkfloat()const{
     for(int i=0;i<_faninlist.size();i++){
       if(_faninlist[i]->_type==UNDEF_GATE){
         return true;
       }
     }
     return false;
   }
   unsigned id()const{
     return _id;
   }
   bool checknotused(){return _fanout.empty()?true:false;}
   void setmarked(int x)const{
     _ismarked=x;
   }
   int marked()const{
     return _ismarked;
   }
   string name()const{
     return _name;
   }
   void setisfanin(int x)const{
     _isfanin=x;
   }
   void setisfanout(int x)const{
     _isfanout=x;
   }
   bool isinvert(IdList& a,unsigned x)const{
     return a[x]%2;
   }
   int isfanin()const{
     return _isfanin;
   }
   int isfanout()const{
     return _isfanout;
   }
   void dfs(vector<CirGate*>&,int);
   void setindfs(){
     _isindfs=true;
   }
   void resetindfs(){
     _isindfs=false;
   }
   bool& isindfs(){
     return _isindfs;
   }
   void removefanout(int j){
    remove(_fanout.begin(),_fanout.end(),_fanout[j]);
    remove(_fanoutlist.begin(),_fanoutlist.end(),_fanoutlist[j]);
    _fanout.pop_back();
    _fanoutlist.pop_back();
   }
   
   void setagainfanout(int j,IdList l,GateList g,bool inv){
     remove(_fanout.begin(),_fanout.end(),_fanout[j]);
    remove(_fanoutlist.begin(),_fanoutlist.end(),_fanoutlist[j]);
    _fanout.pop_back();
    _fanoutlist.pop_back();
    for(int i=0;i<l.size();i++){
      if(inv){
        if(l[i]%2==1){
          _fanout.push_back(l[i]-1);
        }
        else{
          _fanout.push_back(l[i]+1);
        }
      }
      else{
      _fanout.push_back(l[i]);
      }
    }
    for(int m=0;m<g.size();m++){
      _fanoutlist.push_back(g[m]);
    }
   }
   void setfanoutagain(IdList l,GateList g,bool inv){
for(int i=0;i<l.size();i++){
      if(inv){
        if(l[i]%2==1){
          _fanout.push_back(l[i]-1);
        }
        else{
          _fanout.push_back(l[i]+1);
        }
      }
      else{
      _fanout.push_back(l[i]);
      }
    }
    for(int m=0;m<g.size();m++){
      _fanoutlist.push_back(g[m]);
    }
   }
   void setagainfanin(int j,int l,CirGate* g,bool inv){
    remove(_fanin.begin(),_fanin.end(),_fanin[j]);
    remove(_faninlist.begin(),_faninlist.end(),_faninlist[j]);
    _fanin.pop_back();
    _faninlist.pop_back();
    if(inv==1){
      if(l%2==1){
    _fanin.push_back(l-1);
      }
      else{
    _fanin.push_back(l+1);
      }
    }
    else{
    _fanin.push_back(l);
    }
    _faninlist.push_back(g);
   }
   void deleteallfanoutlist(CirGate* g){
     remove(_fanoutlist.begin(),_fanoutlist.end(),g);
     remove(_fanoutlist.begin(),_fanoutlist.end(),g);
     _fanoutlist.pop_back();
     _fanoutlist.pop_back();
   }
  void deleteallfanout(int l){
     remove(_fanout.begin(),_fanout.end(),l);
     remove(_fanout.begin(),_fanout.end(),l);
     _fanout.pop_back();
     _fanout.pop_back();
   }
   void deletefanoutlist(CirGate* g){
     remove(_fanoutlist.begin(),_fanoutlist.end(),g);
     _fanoutlist.pop_back();
   }
  void deletefanout(int l){
     remove(_fanout.begin(),_fanout.end(),l);
     _fanout.pop_back();
   }
   void deletefaninlist(CirGate* g){
     remove(_faninlist.begin(),_faninlist.end(),g);
     _faninlist.pop_back();
   }
  void deletefanin(int l){
     remove(_fanin.begin(),_fanin.end(),l);
     _fanin.pop_back();
   }
   void setfec(int &x){
     _fec=x;
   }
   void setinversefec(bool x){
     inversefec=x;
   }
   int& getfec(){return _fec;}
   bool& getinverse(){return inversefec;}
   void sop(size_t &x){
     _output+=x;
   }
   void setop(unsigned long long &x){
     _output=x;
   }
   size_t& gop(){
     return _output;
   }
   void resetoutput(){
     _output=0;
   }
  void setvar(int x){
    _var=x;
  }
  int var(){
    return _var;
  }
  void setbit(size_t& x){
    _output<<=1;
    _output|=x;
  }
private:
GateType _type;
unsigned _line;
unsigned int _id;
IdList _fanin;
GateList _faninlist;
IdList _fanout;
GateList _fanoutlist;
string _name;
mutable int _ismarked;
mutable int _isfanin;
mutable int _isfanout;
bool _isindfs;
int _fec;
bool inversefec;
size_t _output;
int _var;
protected:

};


#endif // CIR_GATE_H
