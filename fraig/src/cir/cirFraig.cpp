/****************************************************************************
  FileName     [ cirFraig.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir FRAIG functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2012-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "sat.h"
#include "myHashMap.h"
#include "./../util/myHashSet.h"
#include "util.h"
#include "sat.h"

using namespace std;

// TODO: Please keep "CirMgr::strash()" and "CirMgr::fraig()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/*******************************************/
/*   Public member functions about fraig   */
/*******************************************/
// _floatList may be changed.
// _unusedList and _undefList won't be changed
void
CirMgr::strash()
{
  IdList().swap(_strlist);
  HashSet<HashKey>myHash(getHashSize(_dfslist.size()));
  for(int i=0;i<_dfslist.size();i++){
    if(_dfslist[i]->getType()==AIG_GATE){
    HashKey h(_dfslist[i]->id(),_dfslist[i]->fanin()[0],_dfslist[i]->fanin()[1]);
    if(!myHash.check(h)){
        myHash.insert(h);
    }
    else{
      myHash.query(h);
      bool inv1=0;
      if(_dfslist[i]->fanin()[0]%2==1){inv1=1;}
      _dfslist[i]->faninlist()[0]->deletefanout(_dfslist[i]->id()*2+inv1);
      _dfslist[i]->faninlist()[0]->deletefanoutlist(_dfslist[i]);
      bool inv2=0;
      if(_dfslist[i]->fanin()[1]%2==1){inv2=1;}
      _dfslist[i]->faninlist()[1]->deletefanout(_dfslist[i]->id()*2+inv2);
      _dfslist[i]->faninlist()[1]->deletefanoutlist(_dfslist[i]);
      for(int k=0;k<_dfslist[i]->fanout().size();k++){
        h.getcirgate()->setfanoutlist(_dfslist[i]->fanoutlist()[k]);
        h.getcirgate()->setfanout(_dfslist[i]->fanout()[k]);
      }
      for(int j=0;j<_dfslist[i]->fanoutlist().size();j++){
        bool inv=0;
        if(_dfslist[i]->fanout()[j]%2==1){inv=1;}
        int l=_dfslist[i]->id()*2+inv;
        _dfslist[i]->fanoutlist()[j]->deletefanin(l);
        _dfslist[i]->fanoutlist()[j]->deletefaninlist(_dfslist[i]);
        _dfslist[i]->fanoutlist()[j]->setfaninlist(h.getcirgate());
        _dfslist[i]->fanoutlist()[j]->setfanin(h.getid()*2+inv);
      }
      _strlist.push_back(i);
      cout<<h.getid()<<" merging "<<_dfslist[i]->id()<<endl;
    }
  }
  }
  for(int i=0;i<_strlist.size();i++){
  if(_Gates[_dfslist[_strlist[i]]->id()]->getType()==AIG_GATE){
        CirGate* g =_Gates[_dfslist[_strlist[i]]->id()];
        _Gates[_dfslist[_strlist[i]]->id()]=0;
        --A;
        delete g;
      }
  }
  createdfs();
}

void
CirMgr::fraig()
{
IdList().swap(_deletelist);
_sat.initialize();
_Gates[0]->setvar(_sat.newVar());
createvar();
int i=0;
while(!finishedfraig()/*i<20*/){
bool b=0;
runfraig(b);
aigsim();
posim();
collectfec();
//sortfec();
createdfs();
createvar();
i++;
}
for(int i=0;i<_deletelist.size();i++){
  if(_Gates[_deletelist[i]]->getType()==AIG_GATE){
        CirGate* g =_Gates[_deletelist[i]];
        _Gates[_deletelist[i]]=0;
        --A;
        delete g;
      }
  }
  createdfs();
  vector<GateList> t;
  for(int i=0;i<_fecg.size();i++){
      if(_fecg[i].size()<=1){
         t.push_back(_fecg[i]);
      }
   }
   for(int i=0;i<t.size();i++){
      removefecg(t[i]);
   }
}


/********************************************/
/*   Private member functions about fraig   */
/********************************************/
void
CirMgr::createvar(){
  for(int i=0;i<_dfslist.size();i++){
  if(_dfslist[i]->getType()==UNDEF_GATE){
    _dfslist[i]->setvar(_Gates[0]->var());
  }
  if(_dfslist[i]->getType()==AIG_GATE||_dfslist[i]->getType()==PI_GATE||_dfslist[i]->getType()==CONST_GATE){
    _dfslist[i]->setvar(_sat.newVar());
  }
}
for(int i=0;i<_dfslist.size();i++){
  if(_dfslist[i]->getType()==AIG_GATE){
    _sat.addAigCNF(_dfslist[i]->var(),_dfslist[i]->faninlist()[0]->var(),(_dfslist[i]->fanin()[0])&1,
    _dfslist[i]->faninlist()[1]->var(),(_dfslist[i]->fanin()[1])&1);
  }
}
}
void
CirMgr::runfraig(bool &isdfs){
int count=0;
for(int i=0;i<_fecg.size();i++){
  if(_fecg[i].size()>1){
vector<CirGate*> t;
    for(int j=1;j<_fecg[i].size();j++){
int newvar=_sat.newVar();
_sat.addXorCNF(newvar,_fecg[i][0]->var(),_fecg[i][0]->getinverse(),_fecg[i][j]->var(),_fecg[i][j]->getinverse());
_sat.assumeRelease();
_sat.assertProperty(_Gates[0]->var(),false);
_sat.assumeProperty(newvar,true);
//cout<<_fecg[i][0]->id()<<' '<<_fecg[i][0]->getinverse()<<endl;
//cout<<_fecg[i][j]->id()<<' '<<_fecg[i][j]->getinverse()<<endl;
//cout<<_sat.assumpSolve()<<endl;
if(_sat.assumpSolve()==1){
for(int k=0;k<_inputlist.size();k++){
  size_t x=_sat.getValue(_inputlist[k]->var());
  if(x==-1){x=0;}
  x=x<<count;
  _inputlist[k]->sop(x);
  //cout<<k<<' '<<x<<endl;
}
++count;
if((count%64)==0||(j==(_fecg[i].size()-1))){
  //cout<<"runfraig"<<endl;
  
count=0;
break;
}
}
else{
mergegates(_fecg[i][0],_fecg[i][j]);
t.push_back(_fecg[i][j]);
isdfs=1;
    }
}
  for(int k=0;k<t.size();k++){
    removefec(i,t[k]);
  }
}
}
}
void
CirMgr::mergegates(CirGate*&x, CirGate*& y){
   bool inv1=0;
      if(y->fanin()[0]%2==1){inv1=1;}
      y->faninlist()[0]->deletefanout(y->id()*2+inv1);
      y->faninlist()[0]->deletefanoutlist(y);
      bool inv2=0;
      if(y->fanin()[1]%2==1){inv2=1;}
      y->faninlist()[1]->deletefanout(y->id()*2+inv2);
      y->faninlist()[1]->deletefanoutlist(y);
      for(int k=0;k<y->fanout().size();k++){
        x->setfanoutlist(y->fanoutlist()[k]);
        if(y->getinverse()){
          if(y->fanout()[k]%2==1){
        x->setfanout(y->fanout()[k]-1);
          }
          else{
            x->setfanout(y->fanout()[k]+1);
          }
        }
        else{
          x->setfanout(y->fanout()[k]);
        }
      }
      for(int j=0;j<y->fanoutlist().size();j++){
        bool inv=0;
        if(y->fanout()[j]%2==1){inv=1;}
        int l=y->id()*2+inv;
        y->fanoutlist()[j]->deletefanin(l);
        y->fanoutlist()[j]->deletefaninlist(y);
        y->fanoutlist()[j]->setfaninlist(x);
        if(y->getinverse()){
          if(y->fanout()[j]%2==1){
            y->fanoutlist()[j]->setfanin(x->id()*2);
          }
          else{
            y->fanoutlist()[j]->setfanin(x->id()*2+1);
          }
        }
        else{
          if(y->fanout()[j]%2==1){
            y->fanoutlist()[j]->setfanin(x->id()*2+1);
          }
          else{
            y->fanoutlist()[j]->setfanin(x->id()*2);
          }
        }
        
      }
      if(y->getinverse()){
      cout<<"Fraig: "<<x->id()<< " merging !"<<y->id()<<"..."<<endl; 
      }
      else{
      cout<<"Fraig: "<<x->id()<< " merging "<<y->id()<<"..."<<endl; 
      }
      _deletelist.push_back(y->id());
}
