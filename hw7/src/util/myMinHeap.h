/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
   MinHeap(size_t s = 0) { if (s != 0) _data.reserve(s); }
   ~MinHeap() {}

   void clear() { _data.clear(); }

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const { return _data[i]; }   
   Data& operator [] (size_t i) { return _data[i]; }

   size_t size() const { return _data.size(); }

   // TODO
   const Data& min() const { return _data[0]; }
   void insert(const Data& d) {
      _data.push_back(d);
      size_t t=size();
      t--;
      while(t>=1){
         int p=(t-1)/2;
         if (!(d < _data[p])){
         break;
         }
         _data[t]=_data[p]; 
         t=p;
      }
      _data[t]=d;
   }
   void delMin() { 
      delData(0);
   }
   void delData(size_t i) { 
      if(i!=0&&_data[_data.size()-1]<_data[(i-1)/2]){//up
         size_t parent=(i-1)/2;
         while(i>=1&&_data[size()-1]<_data[parent]){
            _data[i]=_data[parent];
            i=parent;
            parent=(i-1)/2;
         }
         _data[i]=_data[size()-1];
         _data.pop_back();
      }
      else{//down
      size_t r=i*2+1;
      size_t l=i*2+2;
      size_t smallest=l;
      size_t n =size()-1;
      while(l<=n){
         if(l<n&&_data[r]<_data[l]){
            smallest=r;
         }
         if(_data[n]<_data[smallest]){
            break;
         }
         _data[i]=_data[smallest];
         i=smallest;
         l=2*i+1;
         r=2*i+2;
         smallest=l;
      }
      _data[i]=_data[size()-1];
      _data.pop_back();
      }
   }
   
private:
   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
