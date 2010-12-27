#ifndef __SMARTOBJ_H__
#define __SMARTOBJ_H__
#include <new>
#include <cstdlib>
#include <map>
using namespace std;

// Class for double-free preventing
class SmartObj {
public:
   SmartObj();
   ~SmartObj();

   virtual const SmartObj * clone() const ;
   virtual void release() ;
   void * operator new(size_t size);
   void   operator delete(void* ptr);

   static int getRefAmount(const void*);
   static int setRefAmount(const void* , int);
   static int addRefAmount(const void* , int);
   static void print() ;
protected:
   //mutable int _ref;
private:
   static map<const void* , int> mapping ;
};

template<class T>
class SmartPtr {
   public:
   T * ptr ;
   public :
   explicit SmartPtr(const T* p = NULL) {
      cout << "[" << __func__ << "] p = " << p << endl ;
      if(p != NULL) {
         ptr = (T *) p->clone() ;
      } else {
         ptr = NULL ;
      }
   }

   SmartPtr(const SmartPtr& p) {
      cout << "[" << __func__ << "] p.ptr = " << p.ptr << endl ;
      this->ptr = NULL ;
      (*this) = p ;
   }

   ~SmartPtr() {
      cout << "[" << __func__ << "]" << endl ;
      if(this->ptr != NULL) {
         this->release() ;
      }
   }

   T& operator* () {return *ptr;}
   T* operator->() {return  ptr;}

   SmartPtr& operator= (const SmartPtr& rhs) {
      cout << "[" << __func__ << "(SmartPtr&)]" << endl ;
      if(this->ptr != NULL) {
         this->release() ;
      }

      this->ptr = (T *)(rhs.ptr->clone()) ;
      return *this ;
   }
/*
   SmartPtr& operator= (const T* rhs) {
      cout << "[" << __func__ << "(T*)]" << endl ;
      if(this->ptr != NULL) {
         this->release() ;
      }

      this->ptr = (T *)(rhs->clone()) ;
      return *this ;
   }
*/
   const T& operator* () const {return *ptr;}
   const T* operator->() const {return  ptr;}

   private:
   void release() {
      if(this->ptr != NULL) {
         this->ptr->release() ;
         if (SmartObj::getRefAmount(this->ptr) == 0) {
            delete this->ptr ;
         }
      }
   }
} ;

#endif /* __SMARTOBJ_H__ */
