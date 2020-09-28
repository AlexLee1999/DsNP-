/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr
{
public:
   CirMgr():M(0),I(0),L(0),O(0),A(0),_gismarked(0),_gisfanin(0),_gisfanout(0){}
   ~CirMgr() {}

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const ;

   // Member functions about circuit construction
   bool readCircuit(const string&);
   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void writeAag(ostream&) const;
   void printdfs(vector<CirGate*> ,bool)const;
   void countdfs(CirGate*,int,int&)const;
   void adfs(CirGate*,ostream&,int)const;
   int sfanin(){
     _gisfanin++;
    return _gisfanin;
   }
   int sfanout(){
     _gisfanout++;
    return _gisfanout;
   }
   void sgismark()const{
     _gismarked++;
   }
   
private:
  int M;
  int I;
  int L;
  int O;
  int A;
  GateList _Gates;
  IdList _input;
  IdList _output;
  IdList _outputwrite;
  IdList _aig;
  GateList _inputlist;
  GateList _outputlist;
  GateList _aiglist;
  mutable int _gismarked;
  mutable int _gisfanin;
  mutable int _gisfanout;
  
};

#endif // CIR_MGR_H
