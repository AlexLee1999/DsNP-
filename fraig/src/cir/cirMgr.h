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
#include <algorithm>
#include "./../util/myHashSet.h"
#include <functional>
#include "cirGate.h"
using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

#include "cirDef.h"

extern CirMgr *cirMgr;
class FECkey;
class CirMgr
{
public:
   CirMgr():M(0),I(0),L(0),O(0),A(0),_gismarked(0),_gisfanin(0),_gisfanout(0),issim(0){}
   ~CirMgr() {}

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const ;

   // Member functions about circuit construction
   bool readCircuit(const string&);

   // Member functions about circuit optimization
   void sweep();
   void optimize();

   // Member functions about simulation
   void randomSim();
   void fileSim(ifstream&);
   void setSimLog(ofstream *logFile) { _simLog = logFile; }

   // Member functions about fraig
   void strash();
   void printFEC() const;
   void fraig();

   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void printFECPairs() const;
   void writeAag(ostream&) const;
   void writeGate(ostream&, CirGate*) const;
   void printdfs(vector<CirGate*> ,bool)const;
   void countdfs(CirGate*,int)const;
   void adfs(CirGate*,ostream&,int)const;
   void idfs(CirGate*,int)const;
   GateList& getgates(){
     return _Gates;
   }
   int& sfanin(){
     _gisfanin++;
    return _gisfanin;
   }
   int& sfanout(){
     _gisfanout++;
    return _gisfanout;
   }
   void sgismark()const{
     _gismarked++;
   }
   void createdfs();
   bool bothinvert(int x,int y){
     if((x/2)==(y/2)&&x!=y){
       return true;
     }
     else return false;
   }
   void aigsim();
   void posim();
  void initfec();
  void collectfec();
  void setfec(int &i,CirGate*&g){
    _fecg[i].push_back(g);
  }
  void removefec(int i ,CirGate*& g){
    remove(_fecg[i].begin(),_fecg[i].end(),g);
    _fecg[i].pop_back();
  }
  vector<GateList>& getfecg(){
    return _fecg;
  }
  void setrandom();
  static bool cmp(CirGate*&,CirGate*&);
  static bool cmp1(GateList&,GateList&);
  void sortfec();
  bool iss(){
    return issim;
  }
  void sortfanout();
  void resetallop(){
    for(int k=0;k<_dfslist.size();k++){
      _dfslist[k]->resetoutput();
    }
  }
  void runfraig(bool&);
  void createvar();
  bool finishedfraig(){
    for(int i=0;i<_fecg.size();i++){
      if(_fecg[i].size()>1){
        return false;
      }
    }
    return true;
  }
  void mergegates(CirGate*&,CirGate*&);
  void removefecg(GateList&x){
    remove(_fecg.begin(),_fecg.end(),x);
    _fecg.pop_back();
  }
private:
  ofstream           *_simLog;
  int M;
  int I;
  int L;
  int O;
  int A;
  GateList _Gates;     //Gate*
  IdList _input;       //input id
  IdList _output;      //output id
  IdList _outputwrite; //_output literal
  GateList _inputlist; 
  GateList _outputlist;
  mutable int _gismarked;
  mutable int _gisfanin;
  mutable int _gisfanout;
  GateList _dfslist;
  IdList _optlist;
  IdList _sweepgatelist;
  IdList _strlist;
  vector<vector<CirGate*> > _fecg;
  HashSet<FECkey>* myhash;
  bool issim;
  SatSolver _sat;
  IdList _deletelist;
};
class HashKey
{
 public:
    HashKey(int id,int fanin1,int fanin2):id(id),fanin1(fanin1),fanin2(fanin2) {}
    HashKey():id(0),fanin1(0),fanin2(0){}
    size_t operator() () const { return (size_t)(fanin1*fanin2*(fanin2+fanin1)+(fanin2*fanin1)); }
 
    bool operator == (const HashKey& k) const { 
       if(fanin1==k.fanin2&&fanin2==k.fanin1){
          return true;
       }
       if(fanin1==k.fanin1&&fanin2==k.fanin2){
          return true;
       }
       return false;
     }
     int getfanin1(){
       return fanin1;
     }
     int getfanin2(){
       return fanin2;
     }
     CirGate* getfaninlist1(){
       return cirMgr->getgates()[fanin1/2];
     }
     CirGate* getfaninlist2(){
       return cirMgr->getgates()[fanin2/2];
     }
    CirGate* getcirgate(){
      return cirMgr->getgates()[id];
    }
    int getid(){
      return id;
    }
    
 private:
 int id;
 int fanin1;
 int fanin2;
 };
 class FECkey
 {
   public:
   FECkey(unsigned int id,size_t pattern,bool inv):id(id),h(pattern),in(inv){
   }
    size_t operator() () const { 
      size_t t=h;
      t^= t >> 10;
      t *= 0xff51afd7ed558ccdL;
      t ^= t >> 33;
      t *= 0xc4ceb9fe1a85ec53L;
      t ^= t >> 20;
      return (size_t)t;
     }
    bool operator ==(const FECkey& k)const{
      return k.h==h;
    }
    unsigned int getid(){
      return id;
    }
    CirGate*& getcirgate(){
      return cirMgr->getgates()[id];
    }
    bool inv(){
      return in;
    }
   private:
   unsigned int id;
   size_t h;
   bool in;
 };

#endif // CIR_MGR_H
