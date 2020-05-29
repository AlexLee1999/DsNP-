/****************************************************************************
  FileName     [ cmdReader.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Define command line reader member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cassert>
#include <cstring>
#include "cmdParser.h"
using namespace std;
//----------------------------------------------------------------------
//    Extrenal funcitons
//----------------------------------------------------------------------
void mybeep();
char mygetc(istream&);
ParseChar getChar(istream&);
//----------------------------------------------------------------------
//    Member Function for class Parser
//----------------------------------------------------------------------
void
CmdParser::readCmd()
{
   if (_dofile.is_open()) {
      readCmdInt(_dofile);
      _dofile.close();
   }
   else
      readCmdInt(cin);
}
void
CmdParser::readCmdInt(istream& istr)
{
   resetBufAndPrintPrompt();
   _tempCmdStored=false;
   while (1) {
      ParseChar pch = getChar(istr);
      if (pch == INPUT_END_KEY) break;
      switch (pch) {
         case LINE_BEGIN_KEY :
         case HOME_KEY       : moveBufPtr(_readBuf); cout<<'\r'<<"cmd> ";break;
         case LINE_END_KEY   :
         case END_KEY        :

         
         for(char*k=_readBufPtr;k<_readBufEnd;k++){
            cout<<*k;
         }
         moveBufPtr(_readBufEnd);
         break;
         case BACK_SPACE_KEY : /* TODO */ 
         if(_readBufPtr>_readBuf){
          char* j=_readBufPtr;
          int count = -1;
          while(j>_readBuf){
          cout<<'\b';
           j--;
          }
          for(char*i=_readBufPtr;i<=_readBufEnd;i++){
            *(i-1)=*i;
          count++;
           }
            _readBufEnd--;
            for(char*k=_readBuf;k<=_readBufEnd;k++){
         cout<<*k;
            }
           cout<<' '<<'\b';
          for(int l=0;l<count;l++){
               cout<<'\b';
           }
               _readBufPtr--;
            if(_historyIdx==(int)_history.size()-1&&_tempCmdStored){
      _history.pop_back();
      _tempCmdStored=false;
      _historyIdx=(int)_history.size();
   }
             }
             else{
                mybeep();
             }
             break;
         case DELETE_KEY     : deleteChar(); break;
         case NEWLINE_KEY    : addHistory();
                               cout << char(NEWLINE_KEY);
                               resetBufAndPrintPrompt();
                               deleteLine();
                                break;
         case ARROW_UP_KEY   : moveToHistory(_historyIdx - 1); break;
         case ARROW_DOWN_KEY : moveToHistory(_historyIdx + 1); break;
         case ARROW_RIGHT_KEY: 
         if(moveBufPtr(_readBufPtr+1)){
         cout<<*(_readBufPtr-1); 
         }
         break;
         case ARROW_LEFT_KEY : 
         
         if(moveBufPtr(_readBufPtr-1)){
            cout<<'\b';
         }
         else{
            cout<<'\b';
            cout<<'\b';
            cout<<'\b';
            cout<<'\b';
            cout<<'\b';
            cout<< "cmd> ";
         };
         break;
         case PG_UP_KEY      : moveToHistory(_historyIdx - PG_OFFSET); break;
         case PG_DOWN_KEY    : moveToHistory(_historyIdx + PG_OFFSET); break;
         case TAB_KEY        : /* TODO */ 
        
         insertChar(' ',TAB_POSITION-((_readBufPtr-_readBuf)%TAB_POSITION));
         
         break;
         case INSERT_KEY     : // not yet supported; fall through to UNDEFINE
         case UNDEFINED_KEY:   mybeep(); break;
         default:  // printable character
            insertChar(char(pch)); break;
      }
      #ifdef TA_KB_SETTING
      taTestOnly();
      #endif
   }
}
// This function moves _readBufPtr to the "ptr" pointer
// It is used by left/right arrowkeys, home/end, etc.
//
// Suggested steps:
// 1. Make sure ptr is within [_readBuf, _readBufEnd].
//    If not, make a beep sound and return false. (DON'T MOVE)
// 2. Move the cursor to the left or right, depending on ptr
// 3. Update _readBufPtr accordingly. The content of the _readBuf[] will
//    not be changed
//
// [Note] This function can also be called by other member functions below
//        to move the _readBufPtr to proper position.
bool
CmdParser::moveBufPtr(char* const ptr)
{
   // TODO... 
   if(!(_readBuf > ptr|| ptr>_readBufEnd)){
      _readBufPtr=ptr;
      return true;
   }
   else{
      mybeep();
      return false;
   }
}
// [Notes]
// 1. Delete the char at _readBufPtr
// 2. mybeep() and return false if at _readBufEnd
// 3. Move the remaining string left for one character
// 4. The cursor should stay at the same position
// 5. Remember to update _readBufEnd accordingly.
// 6. Don't leave the tailing character.
// 7. Call "moveBufPtr(...)" if needed.
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteChar()---
//
// cmd> This is he command
//              ^
//
bool
CmdParser::deleteChar()
{
   // TODO...
   if(_readBufPtr<_readBufEnd){
      int c=-1;
          char* j=_readBufPtr;
          while(j>_readBuf){
          cout<<'\b';
           j--;
          }
          for(char*i=_readBufPtr;i<_readBufEnd;i++){
            *(i)=*(i+1);
          c++;
           }
            _readBufEnd--;
            for(char*k=_readBuf;k<=_readBufEnd;k++){
         cout<<*k;
            }
           cout<<' '<<'\b';
          for(int l=0;l<c;l++){
               cout<<'\b';
           }
             }
             if(_historyIdx==(int)_history.size()-1&&_tempCmdStored){
    _history.pop_back();
      _tempCmdStored=false;
      _historyIdx=(int)_history.size();
   }
             else{
                mybeep();
             }
   return true;
}
// 1. Insert character 'ch' for "repeat" times at _readBufPtr
// 2. Move the remaining string right for "repeat" characters
// 3. The cursor should move right for "repeats" positions afterwards
// 4. Default value for "repeat" is 1. You should assert that (repeat >= 1).
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling insertChar('k', 3) ---
//
// cmd> This is kkkthe command
//                 ^
//
void
CmdParser::insertChar(char ch, int repeat)
{
   // TODO...
   if(repeat==1){
   if(_readBufPtr==_readBufEnd){
   *_readBufPtr=ch;
   _readBufPtr++;
   _readBufEnd++;
   cout<<ch;
   }
   else{
      cout<<ch;
     for(char* j=_readBufPtr;j<_readBufEnd;j++){
        cout<<*j;  
     }
     _readBufEnd++;
      for(char* i=_readBufEnd;i>_readBufPtr;i--){
         *i=*(i-1);
      }
     *_readBufPtr=ch;
     int c= _readBufEnd-_readBufPtr;
     for(int k=1;k<c;k++){
        cout<<'\b';
     }
     _readBufPtr++;
   }
   }
   else{
       if(_readBufPtr==_readBufEnd){
   for(int i=0;i<repeat;i++){
   cout<<ch;
   *(_readBufPtr)=ch;
     _readBufPtr++;
   _readBufEnd++;
   }
   }
   else{
      for(int i=0;i<repeat;i++){
      _readBufEnd++;
      }
      for(char* i=_readBufEnd;i>_readBufPtr;i--){
         *i=*(i-repeat);
      }
      for(int i=0;i<repeat;i++){
     *(_readBufPtr+i)=ch;}
     int d=_readBufEnd-_readBufPtr;
     for(char* j=_readBufPtr;j<_readBufEnd;j++){
        cout<<*j;
     }
     for(int k=repeat;k<d;k++){
        cout<<'\b';
     }
     for(int i=0;i<repeat;i++){
     _readBufPtr++;
     }
   }
   }
   assert(repeat >= 1);
   if(_historyIdx==(int)_history.size()-1&&_tempCmdStored){
      _history.pop_back();
      _tempCmdStored=false;
      _historyIdx=(int)_history.size();
   }
}
// 1. Delete the line that is currently shown on the screen
// 2. Reset _readBufPtr and _readBufEnd to _readBuf
// 3. Make sure *_readBufEnd = 0
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteLine() ---
//
// cmd>
//      ^
//
void
CmdParser::deleteLine()
{
   int cc=_readBufEnd-_readBufPtr;
   for(int i=0;i<cc;i++){
      cout<<*(_readBufPtr+i);
   }
   moveBufPtr(_readBufEnd);
   while(_readBufPtr>_readBuf){
          char* j=_readBufPtr;
          int count = -1;
          while(j>_readBuf){
          cout<<'\b';
           j--;
          }
          for(char*i=_readBufPtr;i<=_readBufEnd;i++){
            *(i-1)=*i;
          count++;
           }
            _readBufEnd--;
            for(char*k=_readBuf;k<=_readBufEnd;k++){
         cout<<*k;
            }
           cout<<' '<<'\b';
          for(int l=0;l<count;l++){
               cout<<'\b';
           }
       _readBufPtr--;
             }
   strcpy(_readBuf,"");
   _readBufPtr =_readBuf; 
   _readBufEnd = _readBuf;
   *_readBufEnd=0;
   *_readBufPtr=0;
   *_readBuf='\0';
}
// This functions moves _historyIdx to index and display _history[index]
// on the screen.
//
// Need to consider:
// If moving up... (i.e. index < _historyIdx)
// 1. If already at top (i.e. _historyIdx == 0), beep and do nothing.
// 2. If at bottom, temporarily record _readBuf to history.
//    (Do not remove spaces, and set _tempCmdStored to "true")
// 3. If index < 0, let index = 0.
//
// If moving down... (i.e. index > _historyIdx)
// 1. If already at bottom, beep and do nothing
// 2. If index >= _history.size(), let index = _history.size() - 1.
//
// Assign _historyIdx to index at the end.
//
// [Note] index should not = _historyIdx
//
void
CmdParser::moveToHistory(int index)
{
   // TODO...
   if(index<_historyIdx&&_historyIdx==0&&index<0){
      mybeep();
   }
   else if((index>_historyIdx&& _historyIdx ==(int)_history.size()&&!_tempCmdStored)){
      mybeep();
   }
   else if((index>_historyIdx&& _historyIdx ==(int)_history.size()-1&&_tempCmdStored)){
      mybeep();
   }
   else{
      if(index<0&&index<_historyIdx){
         index=0;
      }
      else if(index>_historyIdx && index>=(int)_history.size()){
         index=_history.size()-1;
      }
      else if(index<_historyIdx&&_historyIdx==(int)_history.size()){
         _history.push_back(_readBuf);
         _tempCmdStored=true;
      }
      _historyIdx=index;
      retrieveHistory();
   }
}
// This function adds the string in _readBuf to the _history.
// The size of _history may or may not change. Depending on whether 
// there is a temp history string.
//
// 1. Remove ' ' at the beginning and end of _readBuf
// 2. If not a null string, add string to _history.
//    Be sure you are adding to the right entry of _history.
// 3. If it is a null string, don't add anything to _history.
// 4. Make sure to clean up "temp recorded string" (added earlier by up/pgUp,
//    and reset _tempCmdStored to false
// 5. Reset _historyIdx to _history.size() // for future insertion
//
void
CmdParser::addHistory()
{
   // TODO...
   if(!_tempCmdStored){
   string str (_readBuf);
    string* s=new string(_readBuf);
    str.erase(0,str.find_first_not_of(" "));
   str.erase(str.find_last_not_of(" ") + 1);
   if(str[0] != '\0'){
   
   _history.push_back(str);
   _historyIdx=(int)_history.size();
   _tempCmdStored =false;
 deleteLine();
 cout<<*s;
  delete s;
   }
}
else{
   _history.pop_back();
   string str (_readBuf);
   string* s=new string(_readBuf);
   str.erase(0,str.find_first_not_of(" "));
   str.erase(str.find_last_not_of(" ") + 1);
   if(str[0] != '\0'){
   _history.push_back(str);
  deleteLine();
 cout<<*s;
     _historyIdx=(int)_history.size();
   _tempCmdStored =false;
   delete s;
   }
}
}
// 1. Replace current line with _history[_historyIdx] on the screen
// 2. Set _readBufPtr and _readBufEnd to end of line
//
// [Note] Do not change _history.size().
//
void
CmdParser::retrieveHistory()
{
   deleteLine();
   strcpy(_readBuf, _history[_historyIdx].c_str());
   cout << _readBuf;
   _readBufPtr = _readBufEnd = _readBuf + _history[_historyIdx].size();
}
