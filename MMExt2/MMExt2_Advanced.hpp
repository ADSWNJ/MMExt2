// =======================================================================
//         ORBITER AUX LIBRARY: Module Messaging Extended v2
//                              Advanced Interface
//
// MMExt v2 allows Orbiter modules to communicate with each other,
// using predefined module and variable names, with no static binding or
// dependencies. See your .\Orbitersdk\doc\ModuleMessagingExt.pdf
// for full details. 
//
// Copyright  (C) 2014-2017 Szymon "Enjo" Ender and Andrew "ADSWNJ" Stokes
//                         All rights reserved
//
// MMExt v2 is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// MMExt v2 is distributed in the hope that it will
// be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with MMExt2. If not, see
// <http://www.gnu.org/licenses/>
// =======================================================================

#pragma once
#ifndef MMExt2_Advanced_H
#define MMExt2_Advanced_H
#include "windows.h"
#include "orbitersdk.h"
#include <string>
#include ".\MMExt2\__MMExt2_Internal.hpp"
#define _MYV oapiGetFocusInterface()

namespace MMExt2
{

  class Advanced {
  public:
    Advanced(const char *mod) : m_i(mod) {};
    template<typename T> bool Get(const char* mod, const char* var, T* val, const VESSEL* ves = _MYV) const       {return m_i._Get(mod, var, val, ves);}
    bool Delete(const char* var, const VESSEL* ves = _MYV) const                                                  {return m_i._Del(var, ves);}
    bool Put(const char* var, const char *val, const VESSEL* ves = _MYV) const                                    {return m_i._Put(var, std::string(val), ves);}
    template<typename T> bool Put(const char* var, const T& val, const VESSEL* ves = _MYV) const                  {return m_i._Put(var, val, ves);}

    template<typename T> bool PutMMStruct(const char* var, const T val, const VESSEL* ves = _MYV) const;
    template<typename T> bool GetMMStruct(const char* mod, const char* var, T* val, const unsigned int ver,
                                          const unsigned int siz, const VESSEL* ves = _MYV) const;

    template<typename T> bool PutMMBase(  const char* var, const T val, const VESSEL* ves = _MYV) const;
    template<typename T> bool GetMMBase(  const char* mod, const char* var, T* val, const unsigned int ver,
                                          const unsigned int siz, const VESSEL* ves = _MYV) const;

    bool GetLog(const int ix, char *rfunc, bool *rsucc, string *rcli, string *rmod, string *rvar, string *rves)   {return m_i._GetLog(ix, rfunc, rsucc, rcli, rmod, rvar, rves);}
    bool GetVersion(string* ver) const { return m_i._GetVer(ver); }
    bool Find(const char* fMod, const char* fVar, int *ix,
              char *typ, string* mod, string* var, VESSEL** ves, bool skipSelf, const VESSEL* fVes)               {return m_i._Find(fMod, fVar, ix, typ, mod, var, ves, skipSelf, fVes);}
    void UpdMod(const char* mod)                                                                                  {return m_i._UpdMod(mod);}
  private:
    Internal m_i;
  };

  // Inline implementation allows this to be included in multiple compilation units 
  // Compiler and linker will determine best way to combine the compilation units
  template<typename T> inline bool Advanced::PutMMStruct(const char* var, const T val, const VESSEL* ves) const {
    const MMStruct *pSafeStruct = val;
    return m_i._Put(var, pSafeStruct, ves);
  }
  template<typename T> inline bool Advanced::GetMMStruct(const char* mod, const char* var, T* val, const unsigned int ver, const unsigned int siz, const VESSEL* ves) const {
    const MMStruct *pMMStruct;
    if (!m_i._Get(mod, var, &pMMStruct, ves)) return false;
    if (!pMMStruct->IsCorrectSize(siz) || !pMMStruct->IsCorrectVersion(ver)) return false;
    *val = dynamic_cast<T>(pMMStruct);
    return (val != NULL);
  }

  template<typename T> inline bool Advanced::PutMMBase(const char* var, const T val, const VESSEL* ves) const {
    const EnjoLib::ModuleMessagingExtBase *pSafeStruct = val;
    return m_i._Put(var, pSafeStruct, ves);
  }

  template<typename T> inline bool Advanced::GetMMBase(const char* mod, const char* var, T* val, const unsigned int ver, const unsigned int siz, const VESSEL* ves) const {
    const  EnjoLib::ModuleMessagingExtBase *pMMStruct;
    if (!m_i._Get(mod, var, &pMMStruct, ves)) return false;
    if (!pMMStruct->IsCorrectSize(siz) || !pMMStruct->IsCorrectVersion(ver)) return false;
    *val = dynamic_cast<T>(pMMStruct);
    return (val != NULL);
  }
}
#endif // MMExt2_Advanced_H
