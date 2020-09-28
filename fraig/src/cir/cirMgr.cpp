/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;

enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

static bool
parseError(CirParseError err)
{
   switch (err) {
      case EXTRA_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Extra space character is detected!!" << endl;
         break;
      case MISSING_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing space character!!" << endl;
         break;
      case ILLEGAL_WSPACE: // for non-space white space character
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal white space char(" << errInt
              << ") is detected!!" << endl;
         break;
      case ILLEGAL_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
              << errMsg << "!!" << endl;
         break;
      case ILLEGAL_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
              << errMsg << "\"!!" << endl;
         break;
      case ILLEGAL_SYMBOL_TYPE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal symbol type (" << errMsg << ")!!" << endl;
         break;
      case ILLEGAL_SYMBOL_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Symbolic name contains un-printable char(" << errInt
              << ")!!" << endl;
         break;
      case MISSING_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing " << errMsg << "!!" << endl;
         break;
      case MISSING_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
              << errMsg << "\"!!" << endl;
         break;
      case MISSING_NEWLINE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": A new line is expected here!!" << endl;
         break;
      case MISSING_DEF:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
              << " definition!!" << endl;
         break;
      case CANNOT_INVERTED:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": " << errMsg << " " << errInt << "(" << errInt/2
              << ") cannot be inverted!!" << endl;
         break;
      case MAX_LIT_ID:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
              << endl;
         break;
      case REDEF_GATE:
         cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
              << "\" is redefined, previously defined as "
              << errGate->getTypeStr() << " in line " << errGate->getLineNo()
              << "!!" << endl;
         break;
      case REDEF_SYMBOLIC_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
              << errMsg << errInt << "\" is redefined!!" << endl;
         break;
      case REDEF_CONST:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Cannot redefine const (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_SMALL:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too small (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_BIG:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too big (" << errInt << ")!!" << endl;
         break;
      default: break;
   }
   return false;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
bool
CirMgr::readCircuit(const string& fileName)
{
   lineNo=1;
   ifstream fin(fileName);
   string a;
   fin>>a>>M>>I>>L>>O>>A;
   if(a!="aag"){
      return false;
   }
   _Gates.reserve(M+O+1);
   for(int i=0;i<M+O+1;i++){
      _Gates.push_back(0);
   }
   _Gates[0]=new CirGate(CONST_GATE);
   _Gates[0]->setline(lineNo);
   _Gates[0]->setid(0);
   lineNo++;
   for(int i=0;i<I;i++){
      unsigned id;
       fin>>id;
       _Gates[id/2]=new CirGate(PI_GATE);
       _Gates[id/2]->setline(lineNo);
       _Gates[id/2]->setid(id/2);
       _input.push_back(id/2);
       _inputlist.push_back(_Gates[id/2]);
       lineNo++;
   }
   for(int i=0;i<O;i++){
      unsigned id;
       fin>>id;
       _Gates[M+i+1]=new CirGate(PO_GATE);
       _Gates[M+1+i]->setline(lineNo);
       _Gates[M+1+i]->setid(M+1+i);
       _Gates[M+1+i]->setfanin(id);
       _outputwrite.push_back(id);
       _output.push_back(M+1+i);
       _outputlist.push_back(_Gates[M+1+i]);

       if(_Gates[id/2]==0){
          _Gates[id/2]=new CirGate();
          _Gates[id/2]->setid(id/2);

       }
       _Gates[M+1+i]->setfaninlist(_Gates[id/2]);
       _Gates[id/2]->setfanout(2*(M+1+i)+id%2);
       _Gates[id/2]->setfanoutlist(_Gates[M+1+i]);
       lineNo++;
   }
   for(int i=0;i<A;i++){
      unsigned l,m,r;
       fin>>l>>m>>r;
       if(_Gates[l/2]!=0){
          _Gates[l/2]->settype(AIG_GATE);
       }
       else{
       _Gates[l/2]=new CirGate(AIG_GATE);
       }
       _Gates[l/2]->setline(lineNo);
       _Gates[l/2]->setid(l/2);
       _Gates[l/2]->setfanin(m);
       _Gates[l/2]->setfanin(r);
       
       

       if(_Gates[m/2]==0){
          _Gates[m/2]=new CirGate();
          _Gates[m/2]->setid(m/2);
       }
       _Gates[l/2]->setfaninlist(_Gates[m/2]);
       _Gates[m/2]->setfanout(l+m%2);
       _Gates[m/2]->setfanoutlist(_Gates[l/2]);
       if(_Gates[r/2]==0){
          _Gates[r/2]=new CirGate();
          _Gates[r/2]->setid(r/2);

       }
       _Gates[l/2]->setfaninlist(_Gates[r/2]);
       _Gates[r/2]->setfanout(l+r%2);
       _Gates[r/2]->setfanoutlist(_Gates[l/2]);
       lineNo++;
   }
       string n1,n2;
       while(fin>>n1>>n2){
       if(n1=="c"){
          break;
       }
       if(n1[0]=='i'||n1[0]=='o'){
       string c = n1.substr(1,n1.length());
       unsigned int number=(unsigned int)(stoul(c));
       if(n1[0]=='o'){
          _Gates[M+1+number]->setname(n2);
       }
       if(n1[0]=='i'){
          _Gates[_input[number]]->setname(n2);
       }
       }
       }
       createdfs();
       sortfanout();
       issim=0;
       
   return true;
}

/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
/*********************
Circuit Statistics
==================
  PI          20
  PO          12
  AIG        130
------------------
  Total      162
*********************/
void
CirMgr::printSummary() const
{
cout << endl;
cout << "Circuit Statistics" << endl;
cout<<"==================" << endl;
cout<< "  PI "  <<right << setw(11) << I << endl;
cout<< "  PO "  <<right << setw(11) << O << endl;
cout<< "  AIG"  <<right << setw(11) << A << endl;
cout<< "------------------" << endl;
cout<< "  Total"  <<  right << setw(9) << I+O+A << endl;
}

void
CirMgr::printNetlist() const
{
   cout<<endl;
   printdfs(_dfslist,true);
}
void
CirMgr::createdfs(){
   for(int i=0;i<_Gates.size();i++){
      if(_Gates[i]!=0){
      _Gates[i]->resetindfs();
      }
   }
   if(!_dfslist.empty()){
      GateList().swap(_dfslist);
   }
   sgismark();
for(int i=0;i<_outputlist.size();i++){
   _outputlist[i]->dfs(_dfslist,_gismarked);
}
for(int j=0;j<_dfslist.size();j++){
   _dfslist[j]->setindfs();
}
}

void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit:";
   for(unsigned i=0;i<_input.size();i++){
      cout<<" "<<_input[i];
   }
   cout << endl;
}

void
CirMgr::printPOs() const
{
  cout << "POs of the circuit:";
   for(unsigned i=0;i<_output.size();i++){
      cout<<" "<<_output[i];
   }
   cout << endl;
}

void
CirMgr::printFloatGates() const
{
   bool b=0;
   for(int i=0;i<_Gates.size();i++){
      if(_Gates[i]!=0){
      if(_Gates[i]->checkfloat()&&_Gates[i]->getType()==AIG_GATE){
         if(!b){
         cout << "Gates with floating fanin(s):";
         b=1;
      }
         cout<<' '<<_Gates[i]->id();
      }
   }
   }
   if(b){
   cout<<endl;
   }
   bool a=0;
   for(int i=0;i<_Gates.size();i++){
      if(_Gates[i]!=0&&_Gates[i]->getType()!=PO_GATE&&_Gates[i]->getType()!=CONST_GATE){
      if(_Gates[i]->checknotused()){
         if(!a){
            cout<<"Gates defined but not used  :";
            a=1;
         }
         cout<<' '<<_Gates[i]->id();
      }
   }
   }
   cout<<endl;
}

bool 
CirMgr::cmp(CirGate*& a ,CirGate*& b){
   bool c=(a->id())<(b->id());
   return c;
}
bool
CirMgr::cmp1(GateList&a,GateList&b){
   bool c=a[0]->id()<b[0]->id();
   return c;
}
void 
CirMgr::sortfec(){
   vector<GateList> t;
   for(int i=0;i<_fecg.size();i++){
      if(_fecg[i].size()<=1){
         t.push_back(_fecg[i]);
      }
   }
   for(int i=0;i<t.size();i++){
      removefecg(t[i]);
   }
   for(int i=0;i<_fecg.size();i++){
      sort(_fecg[i].begin(),_fecg[i].end(),cirMgr->cmp);
   }
   sort(_fecg.begin(),_fecg.end(),cmp1);
   for(int i=0;i<_fecg.size();i++){
      for(int j=0;j<_fecg[i].size();j++){
         _fecg[i][j]->setfec(i);
         if(_fecg[i][0]->getinverse()){
            if(j==0){
               continue;
            }
            if(_fecg[i][j]->getinverse()){
               _fecg[i][j]->setinversefec(false);
            }
            else if(!_fecg[i][j]->getinverse()){
               _fecg[i][j]->setinversefec(true);
            }

         }
      }
   _fecg[i][0]->setinversefec(false);
   }
}
void 
CirMgr::sortfanout(){
    for(int i=0;i<_Gates.size();i++){
      if(_Gates[i]->fanout().size()>1){
        sort((_Gates[i]->fanoutlistref()).begin(),((_Gates[i]->fanoutlistref()).end()),cirMgr->cmp);
        sort((_Gates[i]->fanout()).begin(),(_Gates[i]->fanout()).end());
      }
    }
  }
void
CirMgr::printFECPairs() const
{
   if(!issim){
      return;
   }
   int count=0;
   for(int i=0;i<_fecg.size();i++){
      if(_fecg[i].size()>1){
         cout<<'['<<count<<"] ";
         for(int j=0;j<_fecg[i].size();j++){
            if(j==_fecg[i].size()-1){
               if(_fecg[i][j]->getinverse()){
               cout<<"!"<<_fecg[i][j]->id();
            }
            else{
               cout<<_fecg[i][j]->id();
            }
            }
            else{
            if(_fecg[i][j]->getinverse()){
               cout<<"!"<<_fecg[i][j]->id()<<' ';
            }
            else{
               cout<<_fecg[i][j]->id()<<' ';
            }
            }
         }
         count++;
      cout<<endl;
      }
   }
}

void
CirMgr::writeAag(ostream& outfile) const
{
   int counta=0;
   _gismarked++;
   for(int i=0;i<_outputlist.size();i++){
      countdfs(_outputlist[i],_gismarked);
   }
   for(int i=0;i<_Gates.size();i++){
      if(_Gates[i]!=0&&_Gates[i]->getType()==AIG_GATE&&_Gates[i]->marked()==_gismarked){
         ++counta;
      }
   }
   outfile <<  "aag "  << M << ' ' << I << ' '<< 0 << ' ' << O << ' ' << counta << endl;
   for(int i=0;i<_input.size();i++){
      outfile<<_input[i]*2<<endl;
   }
   for(int i=0;i<_outputwrite.size();i++){
      outfile<<_outputwrite[i]<<endl;
   }
   _gismarked++;
   for(int i=0;i<_outputlist.size();i++){
      adfs(_outputlist[i],outfile,_gismarked);
   }
   for(int i=0;i<_input.size();i++){
      if(_inputlist[i]->name()!= ""){
      outfile<<'i'<<i<<' '<<_inputlist[i]->name()<<endl;
      }
   }
   for(int i=0;i<_output.size();i++){
      if(_outputlist[i]->name()!= ""){
      outfile<<'o'<<i<<' '<<_outputlist[i]->name()<<endl;
      }
   }
   outfile<<'c';
}

void
CirMgr::writeGate(ostream& outfile, CirGate *g) const
{
   int counta=0;
   vector<int> vi;
   _gismarked++;
   idfs(g,_gismarked);
   for(int i=0;i<_input.size();i++){
      if(_inputlist[i]->marked()==_gismarked){
         vi.push_back(_input[i]);
      }
   }
   sort(vi.begin(),vi.end());
   _gismarked++;
   countdfs(g,_gismarked);
   for(int i=0;i<_Gates.size();i++){
      if(_Gates[i]!=0&&_Gates[i]->getType()==AIG_GATE&&_Gates[i]->marked()==_gismarked){
         ++counta;
      }
   }
   outfile <<"aag "<<g->id()<<' '<<vi.size()<<' '<< 0 <<' '<< 1 <<' '<<counta<< endl;
   for(int i=0;i<vi.size();i++){
      outfile<<vi[i]*2<<endl;
   }
   outfile<<g->id()*2<<endl;
   _gismarked++;
      adfs(g,outfile,_gismarked);
   outfile<<"o0 "<<g->id()<<endl;
   outfile<<"c";
}

CirGate*
CirMgr::getGate(unsigned int gid) const{
   if(gid>(M+O)){
      return 0;
   }
   else if(_Gates[gid]==0){
      return 0;
   }
   else if(_Gates[gid]->getType()==UNDEF_GATE){
      return 0;
   }
   else{
      return _Gates[gid];
   }
}

void
CirMgr::printdfs(vector<CirGate*> gatelist,bool reset)const{
   static int index;
   if(reset==true) index=0;
   for(int k=0;k<gatelist.size();k++){
   cout<<"["<<index<<"] ";
   if(gatelist[k]->faninlist().size()==0){
   cout<<left<<setw(4)<<gatelist[k]->getTypeStr()<<gatelist[k]->id();
   }
   else
   cout<<left<<setw(4)<<gatelist[k]->getTypeStr()<<gatelist[k]->id()<<' ';
   for(int i=0;i<gatelist[k]->faninlist().size();i++){
      string fl,un="";
      if(gatelist[k]->faninlist()[i]->getType()==UNDEF_GATE){
         fl='*';
      }
      if(gatelist[k]->isinvert(gatelist[k]->fanin(),i)){
         un="!";
      }
      if(gatelist[k]->faninlist().size()==1){
      cout<<fl<<un<<gatelist[k]->faninlist()[i]->id();
      }
      else if(i==1){
      cout<<fl<<un<<gatelist[k]->faninlist()[i]->id();
      }
      else if(i==0){
      cout<<fl<<un<<gatelist[k]->faninlist()[i]->id()<<' ';
      }
   }
   if(gatelist[k]->name()!=""){
      cout<<"("<<gatelist[k]->name()<<")";
   }
   cout<<endl;
   index++;
   }
}

void
CirMgr::countdfs(CirGate* gate,int marked)const{
   if(gate->marked()==marked){
      return;
   }
   gate->setmarked(marked);
   for(int i=0;i<gate->faninlist().size();i++){
      countdfs((gate->faninlist()[i]),marked);
   }
}
void
CirMgr::adfs(CirGate* gate,ostream& outfile,int marked)const{
   if(gate->marked()==marked){
      return;
   }
   gate->setmarked(marked);
   for(int i=0;i<gate->faninlist().size();i++){
      adfs((gate->faninlist()[i]),outfile,marked);
   }
   if(gate->isAig()){
      outfile<<gate->id()*2<<' ';
      for(int i=0;i<gate->fanin().size()-1;i++){
         outfile<<gate->fanin()[i]<<' ';
      }
      outfile<<gate->fanin()[gate->fanin().size()-1];
      outfile<<endl;
   }
}
void
CirMgr::idfs(CirGate* gate,int marked)const{
   if(gate->marked()==marked){
      return;
   }
   gate->setmarked(marked);
   for(int i=0;i<gate->faninlist().size();i++){
      idfs((gate->faninlist()[i]),marked);
   }
}

