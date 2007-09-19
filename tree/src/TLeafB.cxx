// @(#)root/tree:$Id: TLeafB.cxx,v 1.17 2006/07/13 05:31:13 pcanal Exp $
// Author: Rene Brun   12/01/96

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// A TLeaf for an 8 bit Integer data type.                              //
//////////////////////////////////////////////////////////////////////////

#include "TLeafB.h"
#include "TBranch.h"
#include "Riostream.h"

ClassImp(TLeafB)

//______________________________________________________________________________
TLeafB::TLeafB()
: TLeaf()
, fMinimum(0)
, fMaximum(0)
, fValue(0)
, fPointer(0)
{
   // -- Default constructor.
}

//______________________________________________________________________________
TLeafB::TLeafB(const char* name, const char* type)
: TLeaf(name, type)
, fMinimum(0)
, fMaximum(0)
, fValue(0)
, fPointer(0)
{
   // -- Create a LeafB.
   fLenType = 1;
}

//______________________________________________________________________________
TLeafB::~TLeafB()
{
   // -- Destructor.
   if (ResetAddress(0, kTRUE)) {
      delete[] fValue;
      fValue = 0;
   }
   // Note: We do not own this, the user's object does.
   fPointer = 0;
}

//______________________________________________________________________________
void TLeafB::Export(TClonesArray* list, Int_t n)
{
   // -- Export element from local leaf buffer to a ClonesArray.

   for (Int_t i = 0, j = 0; i < n; i++, j += fLen) {
      memcpy(((char*) list->UncheckedAt(i)) + fOffset, &fValue[j], fLen);
   }
}

//______________________________________________________________________________
void TLeafB::FillBasket(TBuffer& b)
{
   // -- Pack leaf elements into Basket output buffer.

   Int_t len = GetLen();
   if (fPointer) {
      fValue = *fPointer;
   }
   if (IsRange()) {
      if (fValue[0] > fMaximum) {
         fMaximum = fValue[0];
      }
   }
   if (IsUnsigned()) {
      for (Int_t i = 0; i < len; i++) {
         b << (UChar_t) fValue[i];
      }
   } else {
      b.WriteFastArray(fValue, len);
   }
}

//______________________________________________________________________________
const char *TLeafB::GetTypeName() const
{
   // -- Returns name of leaf type.

   if (fIsUnsigned) {
      return "UChar_t";
   }
   return "Char_t";
}

//______________________________________________________________________________
void TLeafB::Import(TClonesArray *list, Int_t n)
{
   // -- Import element from ClonesArray into local leaf buffer.

   for (Int_t i = 0, j = 0; i < n; i++, j+= fLen) {
      memcpy(&fValue[j], ((char*) list->UncheckedAt(i)) + fOffset, fLen);
   }
}

//______________________________________________________________________________
void TLeafB::PrintValue(Int_t l) const
{
   // -- Prints leaf value.

   char* value = (char*) GetValuePointer();
   printf("%d", (Int_t) value[l]);
}

//______________________________________________________________________________
void TLeafB::ReadBasket(TBuffer &b)
{
   // -- Read leaf elements from Basket input buffer.

   if (!fLeafCount && (fNdata == 1)) {
      b >> fValue[0];
   } else {
      if (fLeafCount) {
         Int_t len = Int_t(fLeafCount->GetValue());
         if (len > fLeafCount->GetMaximum()) {
            Error("ReadBasket", "leaf: '%s' len: %d max: %d", GetName(), len, fLeafCount->GetMaximum());
            len = fLeafCount->GetMaximum();
         }
         fNdata = len * fLen;
         b.ReadFastArray(fValue, len*fLen);
      } else {
         b.ReadFastArray(fValue, fLen);
      }
   }
}

//______________________________________________________________________________
void TLeafB::ReadBasketExport(TBuffer& b, TClonesArray* list, Int_t n)
{
   // -- Read leaf elements from Basket input buffer and export buffer to TClonesArray objects.

   b.ReadFastArray(fValue, n*fLen);

   for (Int_t i = 0, j = 0; i < n; i++, j += fLen) {
      memcpy(((char*) list->UncheckedAt(i)) + fOffset, &fValue[j], fLen);
   }
}

//______________________________________________________________________________
void TLeafB::ReadValue(ifstream &s)
{
   // -- Read a string from ifstream s and store it into the branch buffer.
   char* value = (char*) GetValuePointer();
   s >> value;
}

//______________________________________________________________________________
void TLeafB::SetAddress(void *addr)
{
   // -- Set value buffer address.

   // Check ownership of the value buffer and
   // calculate a new size for it.
   if (ResetAddress(addr)) {
      // -- We owned the old value buffer, delete it.
      delete[] fValue;
      fValue = 0;
   }
   if (addr) {
      // -- We have been provided a new value buffer.
      if (TestBit(kIndirectAddress)) {
         // -- The data member is a pointer to an array.
         fPointer = (Char_t**) addr;
         // Calculate the maximum size we have ever needed
         // for the value buffer.
         Int_t ncountmax = fLen;
         if (fLeafCount) {
            ncountmax = (fLeafCount->GetMaximum() + 1) * fLen;
         }
         // Reallocate the value buffer if needed.
         if ((fLeafCount && (Int_t(fLeafCount->GetValue()) < ncountmax)) ||
             (fNdata < ncountmax) ||
             (*fPointer == 0)) {
            // -- Reallocate.
            // Note:
            //      1) For a varying length array we do this based on
            //         an indirect estimator of the size of the value
            //         buffer since we have no record of how large it
            //         actually is.  If the current length of the
            //         varying length array is less than it has been
            //         in the past, then reallocate the value buffer
            //         to the larger of either the calculated new size
            //         or the maximum size it has ever been.
            //
            //      2) The second condition checks if the new value
            //         buffer size calculated by ResetAddress() is
            //         smaller than the most we have ever used, and
            //         if it is, then we increase the new size and
            //         reallocate.
            //
            //      3) The third condition is checking to see if we
            //         have been given a value buffer, if not then
            //         we must allocate one.
            //
            if (fNdata < ncountmax) {
               fNdata = ncountmax;
            }
            delete[] *fPointer;
            *fPointer = 0;
            *fPointer = new Char_t[fNdata];
         }
         fValue = *fPointer;
      } else {
         // -- The data member is fixed size.
         // FIXME: What about fPointer???
         fValue = (char*) addr;
      }
   } else {
      // -- We must create the value buffer ourselves.
      // Note: We are using the size calculated by ResetAddress().
      fValue = new char[fNdata];
      // FIXME: Why initialize at all?
      fValue[0] = 0;
   }
}

