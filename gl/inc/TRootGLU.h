/* @(#)root/gl:$Id: TRootGLU.h,v 1.1.1.1 2000/05/16 17:00:47 rdm Exp $ */

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TRootGLU
#define ROOT_TRootGLU

#ifdef WIN32
#ifndef ROOT_Windows4Root
#include "Windows4Root.h"
#endif
#endif

// avoid inclusion of macros that confuse TVirtualX.h on Solaris
#define SUN_OGL_NO_VERTEX_MACROS

#include <GL/glu.h>

#endif
