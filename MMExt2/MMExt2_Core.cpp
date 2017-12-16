// ==============================================================
//                ORBITER AUX LIBRARY: ModuleMessagingExt
//             http://sf.net/projects/enjomitchsorbit
//
// Allows Orbiter modules to communicate with each other,
// using predefined module and variable names.
//
// Copyright  (C) 2014-2016 Szymon "Enjo" Ender and Andrew "ADSWNJ" Stokes
//
//                         All rights reserved
//
// ModuleMessagingExt is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// ModuleMessagingExt is distributed in the hope that it will
// be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with ModuleMessagingExt. If not, see
// <http://www.gnu.org/licenses/>.
// ==============================================================

#include "MMExt2_Core.hpp"
//#include "ModuleMessagingExtPut.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>

using namespace ModuleMessagingExt;
using namespace std;

map<string, bool> MMExt2_Core::m_bools;
map<string, int> MMExt2_Core::m_ints;
map<string, double> MMExt2_Core::m_doubles;
map<string, VECTOR3> MMExt2_Core::m_VECTOR3s;
map<string, MATRIX3> MMExt2_Core::m_MATRIX3s;
map<string, MATRIX4> MMExt2_Core::m_MATRIX4s;
map<string, string> MMExt2_Core::m_strings;
//map<string, const ModuleMessagingExtBase*> MMExt2_Core::m_basepointers;
const char MMExt2_Core::m_token = '`';

MMExt2_Core::MMExt2_Core()
{}

MMExt2_Core::~MMExt2_Core()
{}

template<class T>
static bool MMExt2_Core::SearchMap(const char* mod, const char* ves, const char* var, const map<string, T>& mapToSearch, T* returnValue)
{
  const string & id = MakeID(mod, ves, var);
  map<string, T>::const_iterator it = mapToSearch.find(id);
  if (it != mapToSearch.end()) {
    *returnValue = it->second;
    return true;
  } else {
    return false;
  }
}

template<class T>
static bool MMExt2_Core::SearchMapDelete(const char* mod, const char* ves, const char* var, map<string, T>& mapToSearch)
{
  const string & id = MakeID(mod, ves, var);
  map<string, T>::iterator it = mapToSearch.find(id);
  if (it != mapToSearch.end()) {
    mapToSearch.erase(it);
    return true;
  } else {
    return false;
  }
}

string MMExt2_Core::MakeID(const char* mod, const char* ves, const char* var)
{
string idName = string(ves) + m_token + mod + m_token + var;
transform(idName.begin(), idName.end(), idName.begin(), ::tolower);
return idName;
}

/*

void MMExt2_Core::Put(const ModuleMessagingExtPut& sender, const char* var, const ModuleMessagingExtBase* var, const VESSEL* myVessel)
{
m_basepointers[MakeID(sender, myVessel, var)] = var;
}


bool MMExt2_Core::Delete(const ModuleMessagingExtPut& sender, const char* var, const ModuleMessagingExtBase* val, const VESSEL* myVessel)
{
return SearchMapDelete<const ModuleMessagingExtBase *>(sender.ModuleMessagingGetModuleName(), myVessel, var, m_basepointers);
}

bool MMExt2_Core::Get(const char* mod, const char* var, const ModuleMessagingExtBase** val, const VESSEL* myVessel)
{
return SearchMap<const ModuleMessagingExtBase *>(mod, myVessel, var, m_basepointers, val);
}


*/

bool MMExt2_Core::Put(const char* mod, const char* var, const int& val,     const char* ves) { m_ints[    MakeID(mod, ves, var)] = val;  return true; }
bool MMExt2_Core::Put(const char* mod, const char* var, const bool& val,    const char* ves) { m_bools[   MakeID(mod, ves, var)] = val;  return true; }
bool MMExt2_Core::Put(const char* mod, const char* var, const double& val,  const char* ves) { m_doubles[ MakeID(mod, ves, var)] = val;  return true; }
bool MMExt2_Core::Put(const char* mod, const char* var, const string &val,  const char* ves) { m_strings[ MakeID(mod, ves, var)] = val;  return true; }
bool MMExt2_Core::Put(const char* mod, const char* var, const VECTOR3& val, const char* ves) { m_VECTOR3s[MakeID(mod, ves, var)] = val;  return true; }
bool MMExt2_Core::Put(const char* mod, const char* var, const MATRIX3& val, const char* ves) { m_MATRIX3s[MakeID(mod, ves, var)] = val;  return true; }
bool MMExt2_Core::Put(const char* mod, const char* var, const MATRIX4& val, const char* ves) { m_MATRIX4s[MakeID(mod, ves, var)] = val;  return true; }

bool MMExt2_Core::Get(const char* mod, const char* var, int* val,     const char* ves) { return SearchMap<int>(    mod, ves, var, m_ints,     val); }
bool MMExt2_Core::Get(const char* mod, const char* var, bool* val,    const char* ves) { return SearchMap<bool>(   mod, ves, var, m_bools,    val); }
bool MMExt2_Core::Get(const char* mod, const char* var, double* val,  const char* ves) { return SearchMap<double>( mod, ves, var, m_doubles,  val); }
bool MMExt2_Core::Get(const char* mod, const char* var, string* val,  const char* ves) { return SearchMap<string>( mod, ves, var, m_strings,  val); }
bool MMExt2_Core::Get(const char* mod, const char* var, VECTOR3* val, const char* ves) { return SearchMap<VECTOR3>(mod, ves, var, m_VECTOR3s, val); }
bool MMExt2_Core::Get(const char* mod, const char* var, MATRIX3* val, const char* ves) { return SearchMap<MATRIX3>(mod, ves, var, m_MATRIX3s, val); }
bool MMExt2_Core::Get(const char* mod, const char* var, MATRIX4* val, const char* ves) { return SearchMap<MATRIX4>(mod, ves, var, m_MATRIX4s, val); }

bool MMExt2_Core::Delete(const char* mod, const char* var, const char* ves)
{
    bool delFound = false;
    delFound = (SearchMapDelete<int>(mod, ves, var, m_ints) ? true : delFound);
    delFound = (SearchMapDelete<bool>(mod, ves, var, m_bools) ? true : delFound);
    delFound = (SearchMapDelete<double>(mod, ves, var, m_doubles) ? true : delFound);
    delFound = (SearchMapDelete<string>(mod, ves, var, m_strings) ? true : delFound);
    delFound = (SearchMapDelete<VECTOR3>(mod, ves, var, m_VECTOR3s) ? true : delFound);
    delFound = (SearchMapDelete<MATRIX3>(mod, ves, var, m_MATRIX3s) ? true : delFound);
    delFound = (SearchMapDelete<MATRIX4>(mod, ves, var, m_MATRIX4s) ? true : delFound);
    return delFound;
}

MMExt2_Core gCore;

// 
// STATIC ENTRY POINTS
// If you change this interface, make a new V2, V3 set of entry points and fix up the compatibility for all apps using these original ones. 
//

DLLCLBK bool ModMsgPut_int_v1(     const char* mod, const char* var, const int& val,     const char* ves) { return gCore.Put(mod, var, val, ves); }
DLLCLBK bool ModMsgPut_bool_v1(    const char* mod, const char* var, const bool& val,    const char* ves) { return gCore.Put(mod, var, val, ves); }
DLLCLBK bool ModMsgPut_double_v1(  const char* mod, const char* var, const double& val,  const char* ves) { return gCore.Put(mod, var, val, ves); }
DLLCLBK bool ModMsgPut_VECTOR3_v1( const char* mod, const char* var, const VECTOR3& val, const char* ves) { return gCore.Put(mod, var, val, ves); }
DLLCLBK bool ModMsgPut_MATRIX3_v1( const char* mod, const char* var, const MATRIX3& val, const char* ves) { return gCore.Put(mod, var, val, ves); }
DLLCLBK bool ModMsgPut_MATRIX4_v1( const char* mod, const char* var, const MATRIX4& val, const char* ves) { return gCore.Put(mod, var, val, ves); }

DLLCLBK bool ModMsgGet_int_v1(     const char* mod, const char* var, int* val,     const char* ves) { return gCore.Get(mod, var, val, ves); }
DLLCLBK bool ModMsgGet_bool_v1(    const char* mod, const char* var, bool* val,    const char* ves) { return gCore.Get(mod, var, val, ves); }
DLLCLBK bool ModMsgGet_double_v1(  const char* mod, const char* var, double* val,  const char* ves) { return gCore.Get(mod, var, val, ves); }
DLLCLBK bool ModMsgGet_VECTOR3_v1( const char* mod, const char* var, VECTOR3* val, const char* ves) { return gCore.Get(mod, var, val, ves); }
DLLCLBK bool ModMsgGet_MATRIX3_v1( const char* mod, const char* var, MATRIX3* val, const char* ves) { return gCore.Get(mod, var, val, ves); }
DLLCLBK bool ModMsgGet_MATRIX4_v1( const char* mod, const char* var, MATRIX4* val, const char* ves) { return gCore.Get(mod, var, val, ves); }

DLLCLBK bool ModMsgDel_any_v1(const char* mod, const char* var, const char* ves) { return gCore.Delete(mod, var, ves); }

#define MAX_STR_LEN 256
// Special handling for string (do not want to expose the string implementation across compiler versions due to lack of ABI; therefore use char* as the interface)
DLLCLBK bool ModMsgPut_c_str_v1(const char* mod, const char* var, const char* val, const char* ves) {
  string str = val;
  if (str.length() > MAX_STR_LEN - 1) return false;
  return gCore.Put(mod, var, str, ves);
}
DLLCLBK bool ModMsgGet_c_str_v1(const char* mod, const char* var, char *val[], const char* ves) {
  string str;
  if (!gCore.Get(mod, var, &str, ves)) {
    return false;
  }
  strcpy_s(*val, MAX_STR_LEN, str.c_str());
  return true;
}
