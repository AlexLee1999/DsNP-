/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

using namespace std;

// TODO: Keep "CirGate::reportGate()", "CirGate::reportFanin()" and
//       "CirGate::reportFanout()" for cir cmds. Feel free to define
//       your own variables and functions.

extern CirMgr *cirMgr;

void tostring(const size_t& a,string &b){
      size_t c=a;
    for(int i=0;i<64;i++){
       if(i%8==0&&i!=0){
        b="_"+b;
       }
      if(c%2==1){
        b="1"+b;
      }
      else{
        b="0"+b;
      }
      c/=2;
    }
    
}
/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportGate() const
{
   cout << "================================================================================" << endl;
   if(name()!="")
   cout << "= "<<getTypeStr()<<'('<<id()<<')'<<"\""<<name()<<"\","<<" line "<<getLineNo()<< endl;
   else
   {
   cout << "= "<<getTypeStr()<<'('<<id()<<')'<<","<<" line "<<getLineNo()<< endl;
   
   }
   
   cout<< "= FECs:";
   if(cirMgr->iss()!=0&&getType()!=PO_GATE&&getType()!=PI_GATE){
   for(int i=0;i<cirMgr->getfecg()[_fec].size();i++){
      if(cirMgr->getfecg()[_fec][i]->id()!=id()){
         if(inversefec){
            if(cirMgr->getfecg()[_fec][i]->getinverse()){
            cout<<' '<<cirMgr->getfecg()[_fec][i]->id();
         }
         else{
            cout<<' '<<'!'<<cirMgr->getfecg()[_fec][i]->id();
         }
         }
         else{
         if(cirMgr->getfecg()[_fec][i]->getinverse()){
            cout<<' '<<'!'<<cirMgr->getfecg()[_fec][i]->id();
         }
         else{
            cout<<' '<<cirMgr->getfecg()[_fec][i]->id();
         }
      }
      }
   }
   }
   cout<<endl;
   string s;
   tostring(_output,s);
   cout<< "= Value: "<<s<<endl;
   cout << "================================================================================" << endl;
}
void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);
   int mark=cirMgr->sfanin();
   printfanin(level,1,mark,0);
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
   int mark=cirMgr->sfanout();
   printfanout(level,1,mark,0);
}
void 
CirGate::printfanin(int level,int called,int marked,bool inv)const{
   
   if(level==0){
      if(inv){
      cout<<'!'<<this->getTypeStr()<<' '<<this->id()<<endl;
      return;
      }
      else{
      cout<<this->getTypeStr()<<' '<<this->id()<<endl;
      return;
      }
   }
   else if(marked==this->isfanin()){
      if(inv){
      cout<<'!'<<this->getTypeStr()<<' '<<this->id()<<' '<<"(*)"<<endl;
      return;
      }
      else{
      cout<<this->getTypeStr()<<' '<<this->id()<<' '<<"(*)"<<endl;
      return;
      }
   }
   else {
      if(inv){
      cout<<'!'<<this->getTypeStr()<<' '<<this->id()<<endl;
      }
      else{
      cout<<this->getTypeStr()<<' '<<this->id()<<endl;
      }
      inv=false;
   for(int i=0;i<faninlist().size();i++){
      this->setisfanin(marked);
      for(int k=0;k<called;k++){
         cout<<"  ";
      }
      if(_fanin[i]%2==1){
         inv=1;
         _faninlist[i]->printfanin(level-1,called+1,marked,1);
      }
      else{
         _faninlist[i]->printfanin(level-1,called+1,marked,0);
      }
      
   }
   }
}
void 
CirGate::printfanout(int level,int called,int marked,bool inv)const{

   if(level==0){
      if(inv){
      cout<<'!'<<this->getTypeStr()<<' '<<this->id()<<endl;
      return;
      }
      else{
      cout<<this->getTypeStr()<<' '<<this->id()<<endl;
      return;
      }
   }
   else if(marked==this->isfanout()){
      if(inv){
      cout<<'!'<<this->getTypeStr()<<' '<<this->id()<<' '<<"(*)"<<endl;
      return;
      }
      else{
      cout<<this->getTypeStr()<<' '<<this->id()<<' '<<"(*)"<<endl;
      return;
      }
   }
   else {
      if(inv){
      cout<<'!'<<this->getTypeStr()<<' '<<this->id()<<endl;
      }
      else{
      cout<<this->getTypeStr()<<' '<<this->id()<<endl;
      }
      inv=false;
   for(int i=0;i<fanoutlist().size();i++){
      this->setisfanout(marked);
      for(int k=0;k<called;k++){
         cout<<"  ";
      }
      if(_fanout[i]%2==1){
         inv=1;
         _fanoutlist[i]->printfanout(level-1,called+1,marked,1);
      }
      else{
         _fanoutlist[i]->printfanout(level-1,called+1,marked,0);
      }
      
   }
   }
}
void 
CirGate::dfs(vector<CirGate*> & gatelist,int marked){
   if(this->marked()==marked){
      return;
   }
   if(this->getType()==UNDEF_GATE){
      return;
   }
   this->setmarked(marked);
   for(int i=0;i<this->faninlist().size();i++){
      (this->faninlist()[i])->dfs(gatelist,marked);
   }
   gatelist.push_back(this);
}

