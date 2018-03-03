// =======================================================================
//         ORBITER AUX LIBRARY: Module Messaging Extended v2a
//                              Basic Interface
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
#ifndef MMExt2_Basic_H
#define MMExt2_Basic_H
#include "windows.h"
#include "orbitersdk.h"
#include <string>

#include ".\MMExt2\__MMExt2_Internal.hpp"
using namespace std;
namespace MMExt2
{
  class Basic
  {
  public:
    Basic(const string mod) : m_i(mod) {};
    bool Get(const string mod, const string var, bool* val) const      { return m_i._Get(mod, var, val); }
    bool Get(const string mod, const string var, int* val) const       { return m_i._Get(mod, var, val); }
    bool Get(const string mod, const string var, double* val) const    { return m_i._Get(mod, var, val); }
    bool Get(const string mod, const string var, VECTOR3* val) const   { return m_i._Get(mod, var, val); }
    bool Get(const string mod, const string var, MATRIX3* val) const   { return m_i._Get(mod, var, val); }
    bool Get(const string mod, const string var, MATRIX4* val) const   { return m_i._Get(mod, var, val); }
    bool Get(const string mod, const string var, OBJHANDLE* val) const { return m_i._Get(mod, var, val); }
    bool Get(const string mod, const string var, string* val) const    { return m_i._Get(mod, var, val); }
    bool Delete(const string var) const                                { return m_i._Del(var); }
    bool Put(const string var, const bool& val) const                  { return m_i._Put(var, val); }
    bool Put(const string var, const int& val) const                   { return m_i._Put(var, val); }
    bool Put(const string var, const double& val) const                { return m_i._Put(var, val); }
    bool Put(const string var, const VECTOR3& val) const               { return m_i._Put(var, val); }
    bool Put(const string var, const MATRIX3& val) const               { return m_i._Put(var, val); }
    bool Put(const string var, const MATRIX4& val) const               { return m_i._Put(var, val); }
    bool Put(const string var, const OBJHANDLE& val) const             { return m_i._Put(var, val); }
    bool Put(const string var, const string& val) const                { return m_i._Put(var, val); }
    bool Put(const string var, const char* val) const                  { return m_i._Put(var, string(val)); }
    int  ObjType(const OBJHANDLE& val) const                           { return m_i._ObjType(val); }
  private:
    Internal m_i;
  };
}
#endif // MMExt2_Basic_H
