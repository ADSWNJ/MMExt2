// =======================================================================
//         ORBITER AUX LIBRARY: Module Messaging Extended v2a
//                              Advanced Interface
//
// MMExt v2 allows Orbiter modules to communicate with each other,
// using predefined module and variable names, with no static binding or
// dependencies. See your .\Orbitersdk\doc\ModuleMessagingExt.pdf
// for full details. 
//
// Copyright  (C) 2014-2018 Szymon "Enjo" Ender and Andrew "ADSWNJ" Stokes
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
#define _myOhv oapiGetFocusInterface()->GetHandle()

using namespace std;
namespace MMExt2
{

  class Advanced {
  public:
    Advanced(const char *mod) : m_i(mod) {};
    template<typename T> bool Get(const string mod, const string var, T* val, const OBJHANDLE ohv = _myOhv) const  { return m_i._Get(mod, var, val, ohv); }
    bool Delete(const string var, const OBJHANDLE ohv = _myOhv) const                                              { return m_i._Del(var, ohv); }
    bool Put(const string var, const char *val,   const OBJHANDLE ohv = _myOhv) const                              { return m_i._Put(var, string(val), ohv); }
    bool Put(const string var, const string &val, const OBJHANDLE ohv = _myOhv) const                              { return m_i._Put(var, val, ohv); }
    template<typename T> bool Put(const string var, const T& val, const OBJHANDLE ohv = _myOhv) const              { return m_i._Put(var, val, ohv); }

    template<typename T> bool PutMMStruct(const string var, const T val, const OBJHANDLE ohv = _myOhv) const;
    template<typename T> bool GetMMStruct(const string mod, const string var, T* val, const unsigned int ver,
                                          const unsigned int siz, const OBJHANDLE ohv = _myOhv) const;

    //Remove support for old EnjoLib::ModuleMessagingExtBase. Please use MMStruct from now on. 
    //template<typename T> bool PutMMBase(  const string var, const T val, const OBJHANDLE ohv = _myOhv) const;
    //template<typename T> bool GetMMBase(  const string mod, const string var, T* val, const unsigned int ver,
    //                                      const unsigned int siz, const OBJHANDLE ohv = _myOhv) const;

    bool GetLog(char *rFunc, string *rCli, string *rMod, string *rVar, string *rVes,
                bool *rSucc, int *ix, const bool skipSelf = true)                                                  { return m_i._GetLog(rFunc, rCli, rMod, rVar, rVes, rSucc, ix, skipSelf); }
    bool RstLog()                                                                                                  { return m_i._RstLog(); }
    bool GetVersion(string* ver) const                                                                             { return m_i._GetVer(ver); }
    bool Find(char *rTyp, string* rMod, string* rVar, OBJHANDLE* rOhv, int *ix, 
              const string mod, const string var, const OBJHANDLE ohv = NULL, const bool skipSelf = true)          { return m_i._Find(rTyp, rMod, rVar, rOhv, ix, mod, var, ohv, skipSelf); }
    void UpdMod(const string mod)                                                                                  { return m_i._UpdMod(mod); }
    int  ObjType(const OBJHANDLE& val) const                                                                       { return m_i._ObjType(val); }
  private:
    Internal m_i;
  };
  
  // Inline implementation allows this to be included in multiple compilation units 
  // Compiler and linker will determine best way to combine the compilation units
  template<typename T> inline bool Advanced::PutMMStruct(const string var, const T val, const OBJHANDLE ohv) const {
    const MMStruct *pSafeStruct = val;
    return m_i._Put(var, pSafeStruct, ohv);
  }

  template<typename T> inline bool Advanced::GetMMStruct(const string mod, const string var, T* val, const unsigned int ver, const unsigned int siz, const OBJHANDLE ohv) const {
    const MMStruct *pMMStruct;
    if (!m_i._Get(mod, var, &pMMStruct, ohv)) return false;
    if (!pMMStruct->IsCorrectSize(siz) || !pMMStruct->IsCorrectVersion(ver)) return false;
    *val = dynamic_cast<T>(pMMStruct);
    return (val != NULL);
  }

  // Deprecated support for MMBase
  /*
  template<typename T> inline bool Advanced::PutMMBase(const string var, const T val, const OBJHANDLE ohv) const {
    const EnjoLib::ModuleMessagingExtBase *pSafeStruct = val;
    return m_i._Put(var, pSafeStruct, ohv);
  }

  template<typename T> inline bool Advanced::GetMMBase(const string mod, const string var, T* val, const unsigned int ver, const unsigned int siz, const OBJHANDLE ohv) const {
    const  EnjoLib::ModuleMessagingExtBase *pMMStruct;
    if (!m_i._Get(mod, var, &pMMStruct, ohv)) return false;
    if (!pMMStruct->IsCorrectSize(siz) || !pMMStruct->IsCorrectVersion(ver)) return false;
    *val = dynamic_cast<T>(pMMStruct);
    return (val != NULL);
  }
  */
}
#endif // MMExt2_Advanced_H
