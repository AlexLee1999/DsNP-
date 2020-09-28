/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"

using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

bool
initMemCmd()
{
   if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
         cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
         cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
         cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
      )) {
      cerr << "Registering \"mem\" commands fails... exiting" << endl;
      return false;
   }
   return true;
}


//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
CmdExecStatus
MTResetCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size()) {
      int b;
      if (!myStr2Int(token, b) || b < int(toSizeT(sizeof(MemTestObj)))) {
         cerr << "Illegal block size (" << token << ")!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
      #ifdef MEM_MGR_H
      mtest.reset(toSizeT(b));
      #else
      mtest.reset();
      #endif // MEM_MGR_H
   }
   else
      mtest.reset();
   return CMD_EXEC_DONE;
}

void
MTResetCmd::usage(ostream& os) const
{  
   os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void
MTResetCmd::help() const
{  
   cout << setw(15) << left << "MTReset: " 
        << "(memory test) reset memory manager" << endl;
}


//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
CmdExecStatus
MTNewCmd::exec(const string& option)
{
   // TODO
   vector<string> tokens;
   if(!(CmdExec::lexOptions(option,tokens)))
   return CMD_EXEC_ERROR;
   if(tokens.empty()){
      return CmdExec::errorOption(CMD_OPT_MISSING, "");
   }
   if(myStrNCmp("-Array",tokens[0],2)!=0){
   int t;
   bool a =myStr2Int(tokens[0],t);
   t=(size_t)t;
   if(!a||t<=0){
      return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[0]);
   }

   if(tokens.size()>1){
   if(myStrNCmp("-Array",tokens[1],2)!=0){
      return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[1]);
   }
   else{
      if(tokens.size()<=2){
         return CmdExec::errorOption(CMD_OPT_MISSING, "");
      }
      int array;
      bool b;
      b=myStr2Int(tokens[2],array);
      array=(size_t)array;
      if(!b||array<=0){
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[2]);
      }
      try{
      mtest.newArrs(t,array);
      }
      catch(bad_alloc& exception){
         return CMD_EXEC_ERROR;
      }
      return CMD_EXEC_DONE;
   }
   }
   mtest.newObjs(t);
   // Use try-catch to catch the bad_alloc exception
   return CMD_EXEC_DONE;
   }
   else{
   int t;
   bool a =myStr2Int(tokens[2],t);
   t=(size_t)t;
   if(!a||t<=0){
      return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[2]);
   }
   if(tokens.size()>1){
   if(myStrNCmp("-Array",tokens[0],2)!=0){
      return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[0]);
   }
   else{
      if(tokens.size()<=2){
         return CmdExec::errorOption(CMD_OPT_MISSING, "");
      }
      int array;
      bool b;
      b=myStr2Int(tokens[1],array);
      array=(size_t)array;
      if(!b||array<=0){
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[1]);
      }
      try{
      mtest.newArrs(t,array);
      }
      catch(bad_alloc& exception){
         return CMD_EXEC_ERROR;
      }
      return CMD_EXEC_DONE;
   }
   }
   // Use try-catch to catch the bad_alloc exception
   return CMD_EXEC_DONE;
   }
}
void
MTNewCmd::usage(ostream& os) const
{  
   os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void
MTNewCmd::help() const
{  
   cout << setw(15) << left << "MTNew: " 
        << "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus
MTDeleteCmd::exec(const string& option)
{
   // TODO
   vector<string> tokens;
   if(!(CmdExec::lexOptions(option,tokens)))
   return CMD_EXEC_ERROR;
   if(tokens.empty()||tokens.size()<2){
      return CmdExec::errorOption(CMD_OPT_MISSING, "");
   }
   if(tokens.size()>3){
      return CmdExec::errorOption(CMD_OPT_EXTRA,"");
   }
   if((myStrNCmp("-Array",tokens[0],2)!=0)){
   if(myStrNCmp("-Index",tokens[0],2)==0){
      int id;
      bool a= myStr2Int(tokens[1],id);
      id=(size_t)id;
      if(!a||id<0){
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[1]);
      }
      if(tokens.size()==2){
         if(mtest.getObjListSize()>id){
         mtest.deleteObj(id);
         return CMD_EXEC_DONE;
         }
         else{
            cerr<<"Size of object list ("<<mtest.getObjListSize()<<") is <= "<<id<<"!!";
            return CMD_EXEC_ERROR;
         }
      }
      else if(tokens.size()==3){
         if(myStrNCmp("-Array",tokens[2],2)==0){
            if(mtest.getArrListSize()>id){
            mtest.deleteArr(id);
            return CMD_EXEC_DONE;
            }
            else{
            cerr<<"Size of array list ("<<mtest.getArrListSize()<<") is <= "<<id<<"!!";
            return CMD_EXEC_ERROR;
         }
         }
         else{
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[3]);
         }
      }
   }
   else if(myStrNCmp("-Random",tokens[0],2)==0){
      int number;
      bool b=myStr2Int(tokens[1],number);
      if(!b||number<0){
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[1]);
      }
      if(tokens.size()==2){
         if(mtest.getObjListSize()==0){
            cerr<<"Size of object list is 0!!";
            return CMD_EXEC_ERROR;
         }
         else{
            for(int i=0;i<number;i++){
               mtest.deleteObj((size_t)rnGen(mtest.getObjListSize()));
            }
         }
      }
      else if(tokens.size()==3){
         if(myStrNCmp("-Array",tokens[2],2)==0){
            if(mtest.getArrListSize()==0){
            cerr<<"Size of array list is 0!!";
            return CMD_EXEC_ERROR;
            }
            else{
            for(int i=0;i<number;i++){
               mtest.deleteArr((size_t)rnGen(mtest.getArrListSize()));
            }
         }
         }
         else{
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[3]);
         }
      }
   }
   else{
      return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[0]);
   }
   return CMD_EXEC_DONE;
   }
   else{
      if(myStrNCmp("-Index",tokens[1],2)==0){
      int id;
      bool a= myStr2Int(tokens[2],id);
      id=(size_t)id;
      if(!a||id<0){
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[2]);
      }
      if(tokens.size()==3){
         if(myStrNCmp("-Array",tokens[0],2)==0){
            if(mtest.getArrListSize()>id){
            mtest.deleteArr(id);
            return CMD_EXEC_DONE;
            }
            else{
            cerr<<"Size of array list ("<<mtest.getObjListSize()<<") is <= "<<id<<"!!";
            return CMD_EXEC_ERROR;
         }
         }
         else{
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[0]);
         }
      }
   }
   else if(myStrNCmp("-Random",tokens[1],2)==0){
      int number;
      bool b=myStr2Int(tokens[2],number);
      if(!b||number<0){
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[2]);
      }
      
      if(tokens.size()==3){
         if(myStrNCmp("-Array",tokens[0],2)==0){
            if(mtest.getArrListSize()==0){
            cerr<<"Size of array list is 0!!";
            return CMD_EXEC_ERROR;
            }
            else{
            for(int i=0;i<number;i++){
               mtest.deleteArr((size_t)rnGen(mtest.getArrListSize()));
            }
         }
         }
         else{
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[0]);
         }
      }
   }
   else{
      return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[0]);
   }
   return CMD_EXEC_DONE;
   }
}

void
MTDeleteCmd::usage(ostream& os) const
{  
   os << "Usage: MTDelete <-Index (size_t objId) | "
      << "-Random (size_t numRandId)> [-Array]" << endl;
}

void
MTDeleteCmd::help() const
{  
   cout << setw(15) << left << "MTDelete: " 
        << "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
CmdExecStatus
MTPrintCmd::exec(const string& option)
{
   // check option
   if (option.size())
      return CmdExec::errorOption(CMD_OPT_EXTRA, option);
   mtest.print();

   return CMD_EXEC_DONE;
}

void
MTPrintCmd::usage(ostream& os) const
{  
   os << "Usage: MTPrint" << endl;
}

void
MTPrintCmd::help() const
{  
   cout << setw(15) << left << "MTPrint: " 
        << "(memory test) print memory manager info" << endl;
}


