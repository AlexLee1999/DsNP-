/****************************************************************************
  FileName     [ p2Json.h]
  PackageName  [ p2 ]
  Synopsis     [ Header file for class Json JsonElem ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2018-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#ifndef P2_JSON_H
#define P2_JSON_H
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

class JsonElem
{
public:
   // TODO: define constructor & member functions on your own
   JsonElem() {}
   JsonElem(const string &k, int v) : _key(k), _value(v) {}

   friend ostream &operator<<(ostream &, const JsonElem &);
   string getkey()
   {
      return _key;
   }
   int getvalue()
   {
      return _value;
   }

private:
   string _key; // DO NOT change this definition. Use it to store key.
   int _value;  // DO NOT change this definition. Use it to store value.
};
class Json
{
public:
   // TODO: define constructor & member functions on your own
   bool read(const string &);
   bool print();
   bool add(const string &, int);
   //bool exit();
   int getsum();
   int maxnumber();
   int maxindex();
   int minnumber();
   int minindex();
   float getave();
   bool min();
   bool sum();
   bool ave();
   bool max();

private:
   vector<JsonElem> _obj; // DO NOT change this definition.
                          // Use it to store JSON elements.
   vector<string> buff;
};

#endif // P2_TABLE_H
