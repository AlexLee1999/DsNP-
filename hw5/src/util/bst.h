/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>
#include <iostream>
using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
   // TODO: design your own class!!
   friend class BSTree<T>;
   friend class BSTree<T>::iterator;

   BSTreeNode(const T& d, BSTreeNode<T>* p=0, BSTreeNode<T>* r = 0, BSTreeNode<T>* l = 0):
      _data(d), _parent(p),_right(r), _left(l) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   BSTreeNode<T>* _parent;
   BSTreeNode<T>*  _right;
   BSTreeNode<T>*  _left;
   
};


template <class T>
class BSTree
{
   // TODO: design your own class!!
   
   public:
   BSTree():_root(0),_tail(0){}
   ~BSTree(){clear();}
   class iterator { 
      friend class BSTree;
      public:
      iterator(BSTreeNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i): _node(i._node) {}
      ~iterator() {}

      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { 
         _node=successor(_node);
         return *(this); 
         }
      iterator operator ++ (int) { 
         iterator temp=*this;
         _node=successor(_node);
         return temp;
      }
      iterator& operator -- () { 
         _node=predecessor(_node);
         return *(this); 
         } 
      iterator operator -- (int) { 
         iterator temp=*this;
         _node=predecessor(_node);
         return temp; 
         }
      iterator& operator = (const iterator& i) { 
         _node=i._node;
         return *(this); 
         }
      bool operator != (const iterator& i) const { return i._node==_node?false:true; }
      bool operator == (const iterator& i) const { return i._node==_node?true:false; }

      private:
      BSTreeNode<T>* _node;

      BSTreeNode<T>* min (BSTreeNode<T>* x)const {
         while(x->_left){
            x=x->_left;
         }
         return x;
      }
      BSTreeNode<T>* successor(BSTreeNode<T> * x){
         if(x->_right!=0){
            return min(x->_right);
         }
         else{
            BSTreeNode<T> * y=x->_parent;
            while(y!=0&&x==y->_right){
               x=y;
               y=y->_parent;
            }
            return y;
         }
      }
      BSTreeNode<T>* max (BSTreeNode<T>* x){
         while(x->_right!=0){
            x=x->_right;
         }
         return x;
      }
      BSTreeNode<T>* predecessor(BSTreeNode<T> * x){
         if(x->_left){
            return max(x->_left);
         }
         else{
            BSTreeNode<T> * y=x->_parent;
            while(y!=0&&x==y->_left){
               x=y;
               y=y->_parent;
            }
            return y;
         }
      }
      
   };
   iterator begin() const { return empty()? end():iterator(min(_root)); }
   iterator end() const { return iterator(_tail); }
   bool empty() const { return _root==0 ?true:false; }
   size_t size() const { 
      int count=0;
      for (iterator i=begin();i!=end();++i){
         ++count;
      } 
      return size_t(count);
      }

   void insert(const T& x) {
      if(empty()){
         T y;
         _root=new BSTreeNode<T>(x);
         _tail=new BSTreeNode<T>(y);
         _tail->_parent=_root;
         _root->_right=_tail;
      }
      else{
      BSTreeNode<T>* temp=_root;
      BSTreeNode<T>* last=0;
      bool r,l,t=0;
      while(temp){
      if(x<=temp->_data){
         last=temp;
         temp=temp->_left;
         l=1;
         r=0;
         if(!temp){
            break;
         }
      }
      else if (x>temp->_data){
         if(temp->_right==_tail){
            t=1;
            l=0;
            r=0;
            break;
         }
         last=temp;
         temp=temp->_right;
         l=0;
         r=1;
         if(!temp){
            break;
         }
      }
      }
      if(r){
         temp=new BSTreeNode<T>(x);
         temp->_parent=last;
         temp->_parent->_right=temp;
      }
      if(l){
         temp=new BSTreeNode<T>(x);
         temp->_parent=last;
         temp->_parent->_left=temp;
      }
      if(t){
         BSTreeNode<T>* t1= new BSTreeNode<T>(x);
         t1->_parent=temp;
         t1->_right=_tail;
         t1->_right->_parent=t1;
         t1->_parent->_right=t1;
      }
      }
      
   }
   void pop_front() { 
      erase(begin());
   }
   void pop_back() { 

      erase(--end());
   }

   // return false if nothing to erase
   bool erase(iterator pos) { 
      if(empty())
      return false; 
      else{
         BSTreeNode<T>* del=pos._node;
         if((pos._node->_left==0&&pos._node->_right==_tail&&pos._node->_parent==0)){
            _root=0;
         }
         else if((pos._node->_left==0&&pos._node->_right==0)){
            if(pos._node==pos._node->_parent->_left){
               pos._node->_parent->_left=0;
            }
            else{
               pos._node->_parent->_right=0;
            }
         }
         else if((pos._node->_left==0&&pos._node->_right==_tail)){
            pos._node->_parent->_right=_tail;
            _tail->_parent=pos._node->_parent;
         }
         else if(pos._node->_left==0&& pos._node->_right!=0&&pos._node->_right!=_tail){
            transplant(pos._node,pos._node->_right);
         }
         else if((pos._node->_right==0&&pos._node->_left!=0)){
            transplant(pos._node,pos._node->_left);
         }
         else if((pos._node->_left!=0&&pos._node->_right==_tail)){
            transplant(pos._node,pos._node->_left);
            _tail->_parent=max(pos._node->_left);
            _tail->_parent->_right=_tail;
         }
         
         else{
            BSTreeNode<T>* temp= min(pos._node->_right);
            if(temp->_parent!=pos._node){
               transplant(temp,temp->_right);
               temp->_right=pos._node->_right;
               temp->_right->_parent=temp;
            }
            transplant(pos._node,temp);
            temp->_left=pos._node->_left;
            temp->_left->_parent=temp;
         }
      delete del;
      
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
      BSTreeNode<T>* temp=_root;
      while(temp){
      if(x<temp->_data){
         temp=temp->_left;
      }
      else if (x>temp->_data){
         temp=temp->_right;
      }
      else{
         return iterator(temp);
      }
      }
      return end(); 
      }

   void clear() { 
      if(empty()){
         return;
      }
      del(_root);
      _root=0;
   } 
   void sort(){}
   void print()const{

   }
   private:
   BSTreeNode<T>* _root; 
   BSTreeNode<T>* _tail;
   BSTreeNode<T>* min (BSTreeNode<T>* x)const {
         while(x->_left){
            x=x->_left;
         }
         return x;
      }
     
      BSTreeNode<T>* max(BSTreeNode<T>* x){
         while(x->_right){
            if(x->_right==_tail){
               return x;
            }
            x=x->_right;
         }
         return x;
      }
      
      void transplant(BSTreeNode<T>* x,BSTreeNode<T>* y){
         if (x->_parent==0){
            _root=y;
         }
         else if(x==x->_parent->_left){
            x->_parent->_left=y;
         }
         else{
            x->_parent->_right=y;
         }
         if(y!=0)
         y->_parent=x->_parent;
      }
      void del(BSTreeNode<T>* t){
         if(!t){return;}
         del(t->_left);
         del(t->_right);
         delete t;
      }
};

#endif // BST_H
