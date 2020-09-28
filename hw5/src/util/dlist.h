/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { 
         _node=_node->_next;
         return *(this); 
         }
      iterator operator ++ (int) { 
         iterator temp=*this;
         _node=_node->_next;
         return temp;
      }
      iterator& operator -- () { 
         _node=_node->_prev;
         return *(this); 
         } 
      iterator operator -- (int) { 
         iterator temp=*this;
         _node=_node->_prev;
         return temp; 
         }

      iterator& operator = (const iterator& i) { 
         _node=i._node;
         return *(this); 
         }

      bool operator != (const iterator& i) const { return i._node==_node?false:true; }
      bool operator == (const iterator& i) const { return i._node==_node?true:false; }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const { return iterator(_head); }
   iterator end() const { return iterator(_head->_prev); }
   bool empty() const { return iterator(_head)==end()?true:false; }
   size_t size() const { 
      int count=0;
      for (iterator i=begin();i!=end();i++){
         ++count;
      } 
      return size_t(count);
      }

   void push_back(const T& x) { 
      if(empty()){
      DListNode<T> *t=new DListNode<T>(x);
      t->_next=_head;
      t->_prev=_head;
      _head=t;
      _head->_next->_prev=_head;
      _head->_prev->_next=_head;
      }
      else{
      DListNode<T> *t=new DListNode<T>(x);
      t->_prev=_head->_prev->_prev;
      t->_next=_head->_prev;
      t->_prev->_next=t;
      t->_next->_prev=t;
      }
   

   }
   void pop_front() { 
      DListNode<T>* t= _head;
      _head->_prev->_next=_head->_next;
      _head->_next->_prev=_head->_prev;
      _head=_head->_next;
      delete t;

   }
   void pop_back() { 
      DListNode<T>* t= _head->_prev->_prev;
      _head->_prev->_prev->_prev->_next=_head->_prev;
      _head->_prev->_prev=_head->_prev->_prev->_prev;
      delete t;
   }

   // return false if nothing to erase
   bool erase(iterator pos) { 
      if(empty())
      return false; 
      else{
         if(pos==begin()){
            pop_front();
            return true;
         }
         pos._node->_prev->_next=pos._node->_next;
         pos._node->_next->_prev=pos._node->_prev;
         delete pos._node;
         return true;
      }
      }
   bool erase(const T& x) { 
      if(empty())
      return false; 
      else{
         iterator t=find(x);
         if(t!=end()){
         erase(t);
         }
         return true;
      }
      }

   iterator find(const T& x) { 
      for(iterator i=begin();i!=end();i++){
         if (*i== x){
            return i;
         }
      }
      return end(); 
      }

   void clear() { 
      if(empty()){
         return;
      }
      for (iterator i =begin();i!=end();i++){
         erase(i);
      }
   }  // delete all nodes except for the dummy node

   void sort() const { 
      if(size()==1||empty()){
         return;
      }
      for (iterator i=begin();i!=end();i++){
         iterator j=i;
         T temp=j._node->_data;
         iterator smallest=i;
         while(j!=end()){
            if(j._node->_data<temp){
               temp=j._node->_data;
               smallest=j;
            }
            j++;
         }
         swap(i,smallest);
      }
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions
   void swap (iterator& x,iterator& y) const{
      T temp;
      temp=x._node->_data;
      x._node->_data=y._node->_data;
      y._node->_data=temp;
     
   }
};

#endif // DLIST_H
