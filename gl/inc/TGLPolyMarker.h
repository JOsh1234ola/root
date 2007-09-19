// @(#)root/gl:$Id: TGLPolyMarker.h,v 1.2 2006/08/23 14:39:40 brun Exp $
// Author:  Timur Pocheptsov  03/08/2004
// NOTE: This code moved from obsoleted TGLSceneObject.h / .cxx - see these
// attic files for previous CVS history

/*************************************************************************
 * Copyright (C) 1995-2006, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGLPolyMarker
#define ROOT_TGLPolyMarker

#ifndef ROOT_TGLLogicalShape
#include "TGLLogicalShape.h"
#endif

#include <vector>

class TBuffer3D;

////////////////////////////////////////////////////////////////////////
class TGLPolyMarker : public TGLLogicalShape 
{
private:
   std::vector<Double_t> fVertices;
   UInt_t   fStyle;
   Double_t fSize;

protected:
   void DirectDraw(const TGLDrawFlags & flags) const;  
   
public:
   TGLPolyMarker(const TBuffer3D & buffer);

   virtual ELODAxes SupportedLODAxes() const { return kLODAxesNone; }
   virtual Bool_t   IgnoreSizeForOfInterest() const { return kTRUE; }

private:
   void DrawStars()const;

   ClassDef(TGLPolyMarker,0) // a polymarker logical shape
};

#endif
