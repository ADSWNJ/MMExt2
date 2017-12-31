// =======================================================================
//         ORBITER AUX LIBRARY: Module Messaging Extended v2
//                              MMStruct interchange struct header
//
// Copyright  (C) 2014-2017 Szymon "Enjo" Ender and Andrew "ADSWNJ" Stokes
//                         All rights reserved
//
// See MMExt2_Advanced.hpp for license and usage information. 
// This is an internal implementation file. Do not include this directly
// in your code. To use, refer to the documentation in the Orbitersdk\Doc
// folder. 
// =======================================================================
#pragma once
#ifndef MMExt2_MMStruct_H
#define MMExt2_MMStruct_H
namespace MMExt2
{
  struct MMStruct {
  public:
    MMStruct(unsigned int sVer, unsigned int sSize) : _sVer(sVer), _sSize(sSize) {};
    virtual ~MMStruct() {};
    bool IsCorrectVersion(unsigned int sVer) const { return sVer == _sVer; }
    bool IsCorrectSize(unsigned int sSize) const { return sSize == _sSize; }
  private:
    unsigned int _sVer;
    unsigned int _sSize;
  };
}
#endif // MMExt2_MMStruct_H
