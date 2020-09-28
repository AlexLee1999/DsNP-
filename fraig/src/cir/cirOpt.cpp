/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir optimization functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include <algorithm>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Please keep "CirMgr::sweep()" and "CirMgr::optimize()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/**************************************************/
/*   Public member functions about optimization   */
/**************************************************/
// Remove unused gates
// DFS list should NOT be changed
// UNDEF, float and unused list may be changed
void
CirMgr::sweep()
{
  createdfs();
  IdList().swap(_sweepgatelist);
  for(int k=0;k<_Gates.size();k++){
    if(_Gates[k]!=0&&!_Gates[k]->isindfs()&&_Gates[k]->getType()!=PI_GATE&&_Gates[k]->getType()!=CONST_GATE){
      for(int i=0;i<_Gates[k]->fanout().size();i++){
        for(int j=0;j<_Gates[k]->fanoutlist()[i]->fanin().size();j++){
        if(_Gates[k]->fanoutlist()[i]->faninlist()[j]->id()==_Gates[k]->id()){
          bool inv=0;
          if(_Gates[k]->fanout()[i]%2==1){inv=1;}
          _Gates[k]->fanoutlist()[i]->deletefanin(_Gates[k]->id()*2+inv);
          _Gates[k]->fanoutlist()[i]->deletefaninlist(_Gates[k]);
        }
      }
      }
      for(int i=0;i<_Gates[k]->fanin().size();i++){
        for(int j=0;j<_Gates[k]->faninlist()[i]->fanout().size();j++){
        if(_Gates[k]->faninlist()[i]->fanoutlist()[j]->id()==_Gates[k]->id()){
          bool inv=0;
          if(_Gates[k]->fanin()[i]%2==1){inv=1;}
          _Gates[k]->faninlist()[i]->deletefanout(_Gates[k]->id()*2+inv);
          _Gates[k]->faninlist()[i]->deletefanoutlist(_Gates[k]);
        }
      }
      }
      cout<<"Sweeping: "<<_Gates[k]->getTypeStr()<<'('<<k<<')'<<"removed..."<<endl;
      _sweepgatelist.push_back(k);
    }
  }
  for(int i=0;i<_sweepgatelist.size();i++){
  if(_Gates[_sweepgatelist[i]]->getType()==AIG_GATE){
        CirGate* g =_Gates[_sweepgatelist[i]];
        _Gates[_sweepgatelist[i]]=0;
        --A;
        delete g;
      }
  }
  createdfs();
}

// Recursively simplifying from POs;
// _dfsList needs to be reconstructed afterwards
// UNDEF gates may be delete if its fanout becomes empty...
void
CirMgr::optimize()
{
  IdList().swap(_optlist);

  for(int i=0;i<_dfslist.size();i++){
    if(_dfslist[i]->getType()==AIG_GATE){
    for(int j=0;j<_dfslist[i]->faninlist().size();j++){
        if(_dfslist[i]->fanin()[j]==1&&!(bothinvert(_dfslist[i]->fanin()[0],_dfslist[i]->fanin()[1])&&(_dfslist[i]->fanin()[0]!=_dfslist[i]->fanin()[1]))){//case A
        if(j==1){
          for(int k=0;k<_dfslist[i]->faninlist()[0]->fanoutlist().size();k++){
          if(_dfslist[i]->faninlist()[0]->fanoutlist()[k]->id()==_dfslist[i]->id()){
            bool inv=0;
            if(_dfslist[i]->fanin()[0]%2==1){
              inv=1;
            }
            _dfslist[i]->faninlist()[0]->setagainfanout(k,_dfslist[i]->fanout(),_dfslist[i]->fanoutlist(),inv);
          }
          }
          bool isinv=0;
            if(_dfslist[i]->fanin()[1]%2==1){
              isinv=1;
            }
            if(isinv){
          _dfslist[i]->faninlist()[1]->deletefanout(_dfslist[i]->id()*2+1);
            }
            else{
          _dfslist[i]->faninlist()[1]->deletefanout(_dfslist[i]->id()*2);
            }
            _dfslist[i]->faninlist()[1]->deletefanoutlist(_dfslist[i]);

          for(int k=0;k<_dfslist[i]->fanoutlist().size();k++){
            for(int h=0;h<_dfslist[i]->fanoutlist()[k]->fanin().size();h++){
            if(_dfslist[i]->fanoutlist()[k]->faninlist()[h]->id()==_dfslist[i]->id()){
              bool inv=0;
            if(_dfslist[i]->fanout()[k]%2==1){
              inv=1;
            }
            _dfslist[i]->fanoutlist()[k]->setagainfanin(h,_dfslist[i]->fanin()[0],_dfslist[i]->faninlist()[0],inv);
            }
          }
          }
        }
        else if(j==0){
          for(int k=0;k<_dfslist[i]->faninlist()[1]->fanoutlist().size();k++){
          if(_dfslist[i]->faninlist()[1]->fanoutlist()[k]->id()==_dfslist[i]->id()){
            bool inv=0;
            if(_dfslist[i]->fanin()[1]%2==1){
              inv=1;
            }
            _dfslist[i]->faninlist()[1]->setagainfanout(k,_dfslist[i]->fanout(),_dfslist[i]->fanoutlist(),inv);
          }
          }
          bool isinv=0;
            if(_dfslist[i]->fanin()[0]%2==1){
              isinv=1;
            }
            if(isinv){
          _dfslist[i]->faninlist()[0]->deletefanout(_dfslist[i]->id()*2+1);
            }
            else{
          _dfslist[i]->faninlist()[0]->deletefanout(_dfslist[i]->id()*2);
            }
            _dfslist[i]->faninlist()[0]->deletefanoutlist(_dfslist[i]);
          for(int k=0;k<_dfslist[i]->fanoutlist().size();k++){
            for(int h=0;h<_dfslist[i]->fanoutlist()[k]->fanin().size();h++){
            if(_dfslist[i]->fanoutlist()[k]->faninlist()[h]->id()==_dfslist[i]->id()){
               bool inv=0;
            if(_dfslist[i]->fanout()[k]%2==1){
              inv=1;
            }
            _dfslist[i]->fanoutlist()[k]->setagainfanin(h,_dfslist[i]->fanin()[1],_dfslist[i]->faninlist()[1],inv);
            }
          }
          }
        }
        _optlist.push_back(i);
        cout<<"merging  "<<_dfslist[i]->id()<<"  case A"<<endl;
      }//case A
      if(_dfslist[i]->fanin()[j]==0&&!(bothinvert(_dfslist[i]->fanin()[0],_dfslist[i]->fanin()[1]))&&(_dfslist[i]->fanin()[0]!=_dfslist[i]->fanin()[1])){//case B
        for(int k=0;k<_Gates[0]->fanoutlist().size();k++){
          if(_Gates[0]->fanoutlist()[k]->id()==_dfslist[i]->id()){
            
            _Gates[0]->setagainfanout(k,_dfslist[i]->fanout(),_dfslist[i]->fanoutlist(),false);
          }
        }
        if(j==1){
          for(int k=0;k<_dfslist[i]->faninlist()[0]->fanoutlist().size();k++){
            if(_dfslist[i]->faninlist()[0]->fanoutlist()[k]->id()==_dfslist[i]->id()){
          _dfslist[i]->faninlist()[0]->removefanout(k);
            }
          }
        }
        else if(j==0){
          for(int k=0;k<_dfslist[i]->faninlist()[1]->fanoutlist().size();k++){
            if(_dfslist[i]->faninlist()[1]->fanoutlist()[k]->id()==_dfslist[i]->id()){
          _dfslist[i]->faninlist()[1]->removefanout(k);
            }
          }
        }
        for(int k=0;k<_dfslist[i]->fanoutlist().size();k++){
          for(int h=0;h<_dfslist[i]->fanoutlist()[k]->fanin().size();h++){
            if(_dfslist[i]->fanoutlist()[k]->faninlist()[h]->id()==_dfslist[i]->id()){
               bool inv=0;
            if(_dfslist[i]->fanout()[k]%2==1){
              inv=1;
            }
            _dfslist[i]->fanoutlist()[k]->setagainfanin(h,0,_Gates[0],inv);
            }
          }
        }
        _optlist.push_back(i);
        cout<<"merging  "<<_dfslist[i]->id()<<"  case B"<<endl;

      }//case B
    }
      if(_dfslist[i]->getType()==AIG_GATE&&_dfslist[i]->fanin().size()!=0&&_dfslist[i]->fanin()[0]==_dfslist[i]->fanin()[1]){//case C
      _dfslist[i]->faninlist()[0]->deleteallfanoutlist(_dfslist[i]);
      bool isinv=0;
            if(_dfslist[i]->fanin()[1]%2==1){
              isinv=1;
            }
      if(isinv){
        _dfslist[i]->faninlist()[0]->deleteallfanout(_dfslist[i]->id()*2+1);
      }
      else{
        _dfslist[i]->faninlist()[0]->deleteallfanout(_dfslist[i]->id()*2);

      }
            _dfslist[i]->faninlist()[0]->setfanoutagain(_dfslist[i]->fanout(),_dfslist[i]->fanoutlist(),isinv);
      
      for(int k=0;k<_dfslist[i]->fanoutlist().size();k++){
            for(int h=0;h<(_dfslist[i]->fanoutlist()[k]->fanin()).size();h++){
            if(_dfslist[i]->fanoutlist()[k]->faninlist()[h]->id()==_dfslist[i]->id()){
               bool inv=0;
            if(_dfslist[i]->fanout()[k]%2==1){
              inv=1;
            }
            _dfslist[i]->fanoutlist()[k]->setagainfanin(h,_dfslist[i]->fanin()[1],_dfslist[i]->faninlist()[1],inv);
            }
          }
          }
          _optlist.push_back(i);
        cout<<"merging  "<<_dfslist[i]->id()<<"  case C"<<endl;

      }//Case C
      if(_dfslist[i]->getType()==AIG_GATE&&_dfslist[i]->fanin().size()!=0&&bothinvert(_dfslist[i]->fanin()[0],_dfslist[i]->fanin()[1])){
        
          _dfslist[i]->faninlist()[1]->deletefanout(_dfslist[i]->id()*2);
          _dfslist[i]->faninlist()[1]->deletefanout(_dfslist[i]->id()*2+1);
          _dfslist[i]->faninlist()[1]->deleteallfanoutlist(_dfslist[i]);

          for(int k=0;k<_dfslist[i]->fanoutlist().size();k++){
            for(int h=0;h<_dfslist[i]->fanoutlist()[k]->fanin().size();h++){
            if(_dfslist[i]->fanoutlist()[k]->faninlist()[h]->id()==_dfslist[i]->id()){
               bool inv=0;
            if(_dfslist[i]->fanout()[k]%2==1){
              inv=1;
            }
            _dfslist[i]->fanoutlist()[k]->setagainfanin(h,0,_Gates[0],inv);
            }
          }
          _Gates[0]->setfanout(_dfslist[i]->fanout()[k]);
          _Gates[0]->setfanoutlist(_dfslist[i]->fanoutlist()[k]);
          }
          

        _optlist.push_back(i);
        cout<<"merging  "<<_dfslist[i]->id()<<"  case D"<<endl;
        
      }
    }
   
  }
  for(int i=0;i<_optlist.size();i++){
  if(_Gates[_dfslist[_optlist[i]]->id()]->getType()==AIG_GATE){
        CirGate* g =_Gates[_dfslist[_optlist[i]]->id()];
        _Gates[_dfslist[_optlist[i]]->id()]=0;
        --A;
        delete g;
      }
  }
  createdfs();
}

/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/
