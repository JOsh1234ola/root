// @(#)root/pyroot:$Id: MethodProxy.h,v 1.3 2005/09/09 05:19:10 brun Exp $
// Author: Wim Lavrijsen, Jan 2005

#ifndef PYROOT_METHODPROXY_H
#define PYROOT_METHODPROXY_H

// ROOT
#include "DllImport.h"

// Bindings
#include "PyCallable.h"

// Standard
#include <map>
#include <string>
#include <vector>


namespace PyROOT {

/** Method proxy object to hold ROOT methods
      @author  WLAV
      @date    01/07/2005
      @version 1.0
 */

   class MethodProxy {
   public:
      typedef std::map< Long_t, Int_t >  DispatchMap_t;
      typedef std::vector< PyCallable* > Methods_t;

      struct MethodInfo_t {
         MethodInfo_t() : fIsSorted( kFALSE ) { fRefCount = new int(1); }
         ~MethodInfo_t() { delete fRefCount; }

         std::string                 fName;
         MethodProxy::DispatchMap_t  fDispatchMap;
         MethodProxy::Methods_t      fMethods;
         std::string                 fDoc;
         Bool_t                      fIsSorted;

         int* fRefCount;
      };

   public:
      void Set( const std::string& name, std::vector< PyCallable* >& methods );

      const std::string& GetName() const { return fMethodInfo->fName; }
      void AddMethod( PyCallable* pc ) {
         fMethodInfo->fIsSorted = kFALSE; fMethodInfo->fMethods.push_back( pc ); }

   public:               // public, as the python C-API works with C structs
      PyObject_HEAD
      ObjectProxy*   fSelf;        // The instance it is bound to, or NULL
      MethodInfo_t*  fMethodInfo;

   private:              // private, as the python C-API will handle creation
      MethodProxy() {}
   };


//- method proxy type and type verification ----------------------------------
   R__EXTERN PyTypeObject MethodProxy_Type;

   template< typename T >
   inline Bool_t MethodProxy_Check( T* object )
   {
      return object && PyObject_TypeCheck( object, &MethodProxy_Type );
   }

   template< typename T >
   inline Bool_t MethodProxy_CheckExact( T* object )
   {
      return object && object->ob_type == &MethodProxy_Type;
   }

//- creation -----------------------------------------------------------------
   inline MethodProxy* MethodProxy_New(
         const std::string& name, std::vector< PyCallable* >& methods )
   {
      MethodProxy* pymeth = (MethodProxy*)MethodProxy_Type.tp_new( &MethodProxy_Type, 0, 0 );
      pymeth->Set( name, methods );
      return pymeth;
   }

   inline MethodProxy* MethodProxy_New( const std::string& name, PyCallable* method )
   {
      std::vector< PyCallable* > p;
      p.push_back( method );
      return MethodProxy_New( name, p );
   }

} // namespace PyROOT

#endif // !PYROOT_METHODPROXY_H
