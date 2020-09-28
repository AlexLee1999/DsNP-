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
#include "cirDef.h"

using namespace std;

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes
class CirGate
{
public:
   CirGate(GateType t=UNDEF_GATE):_type(t),_line(0),_id(0),_name(""),_ismarked(0) {}
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
   GateList fanoutlist()const{
     return _fanoutlist;
   }
   IdList fanin(){
     return _fanin;
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
   bool isinvert(IdList a,unsigned x)const{
     return a[x]%2;
   }
   int isfanin()const{
     return _isfanin;
   }
   int isfanout()const{
     return _isfanout;
   }
   void dfs(vector<CirGate*>&,int);
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
protected:

};


#endif // CIR_GATE_H
