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
#include <cstring>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

using namespace std;

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportGate() const
{
   string temp;
   temp=this->getTypeStr();
   temp+="("+to_string(this->id())+")";
   if(this->name()!=""){
      temp+="\""+this->name()+"\"";
   }
   temp+=",";
   temp+=" line "+to_string(getLineNo());
   cout << "==================================================" << endl;
   cout << "="<<' '<< left << setw(46) <<temp<<' '<< "=" << endl;
   cout << "==================================================" << endl;
   
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
      if(isinvert(_fanin,i)){
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
      if(isinvert(_fanout,i)){
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
