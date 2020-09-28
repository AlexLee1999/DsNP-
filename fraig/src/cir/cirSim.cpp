/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir simulation functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include <string>
#include <cstring>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <bitset>
#include <random>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"
#include "limits.h"

using namespace std;

// TODO: Keep "CirMgr::randimSim()" and "CirMgr::fileSim()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/
static size_t zero=0;
/**************************************/
/*   Static varaibles and functions   */
/**************************************/
void randnum(unsigned long long &x){
  std::random_device rd;
  std::default_random_engine gen = std::default_random_engine(rd());
  std::uniform_int_distribution<long long> dis(0,ULLONG_MAX);
  x=dis(gen);
}
/************************************************/
/*   Public member functions about Simulation   */
/************************************************/
void
CirMgr::randomSim()
{
    initfec();
    int size=3*(I*I+A-((I*I+A)%64))>320? 3*(I*I+A-((I*I-A)%64)):320;
    
    _Gates[0]->sop(zero);
    resetallop();
    for(int i=0;i<size/64;i++){
    setrandom();
     if(_simLog!=0){
    for(int j=0;j<64;j++){
    for(int i=0;i<I;i++){
      (*_simLog)<<((_inputlist[i]->gop()>>j)&1);
      }
      *_simLog<<' ';
      for(int i=0;i<O;i++){
      (*_simLog)<<((_outputlist[i]->gop()>>j)&1);
      }
      *_simLog<<endl;
    }
  }
    }
    cout<<size<<" patterns simulated."<<endl;
    sortfec();
}
void
CirMgr::fileSim(ifstream& patternFile)
{
  string temp;
  bool isf=1;
  int count=0;
  int size=0;
  _Gates[0]->sop(zero);
  initfec();
while(patternFile>>temp){
  if(temp.size()!=_inputlist.size()){
    cerr<<"Error: Pattern("<<temp<<") length("<<temp.size()<<") does not match the number of inputs("<<_input.size()<<") in a circuit!!"<<endl;
    cout<<"0 patterns simulated."<<endl;
    isf=0;
    break;
  }
  size_t a=temp.find_first_not_of("01");
  if(a!=string::npos){
    cerr<<"Error: Pattern("<<temp<<") contains a non-0/1 character('"<<temp[a]<<"')."<<endl;
    cout<<"0 patterns simulated."<<endl;
    isf=0;
    break;
  }
  
  if(((count%64)==0)&&(count!=0)&&isf){
  aigsim();
  posim();
  collectfec();
  if(_simLog!=0){
    for(int j=0;j<64;j++){
    for(int i=0;i<I;i++){
      (*_simLog)<<((_inputlist[i]->gop()>>j)&1);
      }
      *_simLog<<' ';
      for(int i=0;i<_outputlist.size();i++){
      (*_simLog)<<((_outputlist[i]->gop()>>j)&1);
      }
      *_simLog<<endl;
    }
  }
  resetallop();
    count=0;
  }
  for(int k=0;k<temp.size();k++){
      size_t a=temp[k]-'0';
        a=a<<count;
      _inputlist[k]->sop(a);
  }
  ++count;
  ++size;
}
if(isf&&!issim){
aigsim();
  posim();
  collectfec();
  if(_simLog!=0){
    for(int j=0;j<size;j++){
    for(int i=0;i<I;i++){
      (*_simLog)<<((_inputlist[i]->gop()>>j)&1);
      }
      *_simLog<<' ';
      for(int i=0;i<O;i++){
      (*_simLog)<<((_outputlist[i]->gop()>>j)&1);
      }
      *_simLog<<endl;
    }
  }
}
if(issim&&count==64){
  aigsim();
  posim();
  collectfec();
  if(_simLog!=0){
    for(int j=0;j<64;j++){
    for(int i=0;i<I;i++){
      (*_simLog)<<((_inputlist[i]->gop()>>j)&1);
      }
      *_simLog<<' ';
      for(int i=0;i<O;i++){
      (*_simLog)<<((_outputlist[i]->gop()>>j)&1);
      }
      *_simLog<<endl;
    }
  }
}
cout<<size<<" patterns simulated."<<endl;
sortfec();
}
/*************************************************/
/*   Private member functions about Simulation   */
/*************************************************/
void 
CirMgr::setrandom(){
  resetallop();
  for(int i=0;i<_inputlist.size();i++){
    unsigned long long x;
    randnum(x);
    _inputlist[i]->setop(x);
  }
  aigsim();
  posim();
  collectfec();
}
void 
CirMgr::aigsim(){
  for(int i=0;i<_dfslist.size();i++){
    if(_dfslist[i]->getType()==AIG_GATE){
    size_t input0,input1;
    if((_dfslist[i]->isinvert(_dfslist[i]->fanin(),0))){
      input0=~(_dfslist[i]->faninlistref()[0]->gop());
    }
    else{
      input0=(_dfslist[i]->faninlistref()[0]->gop());
    }
    if(_dfslist[i]->isinvert(_dfslist[i]->fanin(),1)){
      input1=~(_dfslist[i]->faninlistref()[1]->gop());
    }
    else{
      input1=(_dfslist[i]->faninlistref()[1]->gop());
    }
    input0&=input1;
    _dfslist[i]->sop(input0);
    }
  }
  issim=1;
}
void
CirMgr::posim(){
  for(int i=0;i<_outputlist.size();i++){
    size_t in=0;
    if(_outputlist[i]->isinvert(_outputlist[i]->fanin(),0)){
      in=~(_outputlist[i]->faninlistref()[0]->gop());
    }
    else{
      in=(_outputlist[i]->faninlistref()[0]->gop());
    }
    _outputlist[i]->sop(in);
  }
}
void 
CirMgr::initfec(){
  vector<vector<CirGate*> >().swap(_fecg);
  myhash=new HashSet<FECkey>();
  vector<CirGate*> x;
  x.reserve(A);
  getfecg().reserve(A);
  getfecg().push_back(x);
  int l=0;
  getfecg()[0].push_back(_Gates[0]);
  for(int i=0;i<_dfslist.size();i++){
    if(_dfslist[i]->getType()==(AIG_GATE)){
    getfecg()[0].push_back(_dfslist[i]);
    _dfslist[i]->setfec(l);
    }
  }
}
void
CirMgr::collectfec(){
  int size=_fecg.size();
  for(int i=0;i<size;i++){
    if(_fecg[i].size()>1){
    myhash->init(getHashSize(_fecg[i].size()));
    bool add=0;
    FECkey f(getfecg()[i][0]->id(),getfecg()[i][0]->gop(),false);
    myhash->insert(f);
    vector<CirGate*> temp;
    for(int j=1;j<_fecg[i].size();j++){
    FECkey f1(getfecg()[i][j]->id(),getfecg()[i][j]->gop(),false);
    FECkey f2(getfecg()[i][j]->id(),~(getfecg()[i][j]->gop()),true);
    if(myhash->check(f1)){
      myhash->query(f1);
      if(f1.getcirgate()!=getfecg()[i][0]){
        int a=f1.getcirgate()->getfec();
          setfec(a,getfecg()[i][j]);
          temp.push_back(getfecg()[i][j]);
          add=1;
          if(f1.inv()){
      getfecg()[i][j]->setinversefec(true);
          }
          else{
      getfecg()[i][j]->setinversefec(false);
          }
      }
      else{
      getfecg()[i][j]->setinversefec(false);
        continue;
      }
    }
    else if(myhash->check(f2)){
      myhash->query(f2);
      if(f2.getcirgate()!=getfecg()[i][0]){
        int a=f2.getcirgate()->getfec();
          setfec(a,getfecg()[i][j]);
          temp.push_back(getfecg()[i][j]);
          add=1;
        if(f2.inv()){
      getfecg()[i][j]->setinversefec(false);
          }
          else{
      getfecg()[i][j]->setinversefec(true);
          }
      }
      else{
      getfecg()[i][j]->setinversefec(true);
      continue;
      }
    }
    else if(!myhash->check(f2)&&!myhash->check(f1)){
      myhash->insert(f1);
      myhash->insert(f2);
      getfecg()[i][j]->setinversefec(false);
      temp.push_back(getfecg()[i][j]);
      vector<CirGate*>y;
      _fecg.push_back(y);
      int l=int(_fecg.size()-1);
      int s=_fecg.size()-1;
      setfec(s,getfecg()[i][j]);
      getfecg()[i][j]->setfec(l);
      add=1;
      }
    }
    if(add){
    for(int k=0;k<temp.size();k++){
    removefec(i,temp[k]);
    }
    }
    myhash->reset();
  }
  }
}
