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
static bool MMExt2_Core::SearchMap(const char* moduleName, const char* vesselName, const char* varName, const map<string, T>& mapToSearch, T* returnValue)
{
  const string & id = MakeID(moduleName, vesselName, varName);
  map<string, T>::const_iterator it = mapToSearch.find(id);
  if (it != mapToSearch.end()) {
    *returnValue = it->second;
    return true;
  } else {
    return false;
  }
}

template<class T>
static bool MMExt2_Core::SearchMapDelete(const char* moduleName, const char* vesselName, const char* varName, map<string, T>& mapToSearch)
{
  const string & id = MakeID(moduleName, vesselName, varName);
  map<string, T>::iterator it = mapToSearch.find(id);
  if (it != mapToSearch.end()) {
    mapToSearch.erase(it);
    return true;
  } else {
    return false;
  }
}


/*string MMExt2_Core::MakeID(const ModuleMessagingExtPut& sender, const char* myVessel, const char* varName)
{
return MakeID(sender.ModuleMessagingGetModuleName(), myVessel, varName);
}*/

string MMExt2_Core::MakeID(const char* moduleName, const char* vesselName, const char* varName)
{
string idName = string(vesselName) + m_token + moduleName + m_token + varName;
transform(idName.begin(), idName.end(), idName.begin(), ::tolower);
return idName;
}

/*
void MMExt2_Core::Put(const ModuleMessagingExtPut& sender, const char* varName, bool var, const VESSEL* myVessel)
{
m_bools[MakeID(sender, myVessel, varName)] = var;
}
void MMExt2_Core::Put(const ModuleMessagingExtPut& sender, const char* varName, int var, const VESSEL* myVessel)
{
m_ints[MakeID(sender, myVessel, varName)] = var;
}
void MMExt2_Core::Put(const ModuleMessagingExtPut& sender, const char* varName, double var, const VESSEL* myVessel)
{
m_doubles[MakeID(sender, myVessel, varName)] = var;
}
void MMExt2_Core::Put(const ModuleMessagingExtPut& sender, const char* varName, const VECTOR3& var, const VESSEL* myVessel)
{
m_vectors[MakeID(sender, myVessel, varName)] = var;
}
void MMExt2_Core::Put(const ModuleMessagingExtPut& sender, const char* varName, const MATRIX3& var, const VESSEL* myVessel)
{
m_matrices3[MakeID(sender, myVessel, varName)] = var;
}
void MMExt2_Core::Put(const ModuleMessagingExtPut& sender, const char* varName, const MATRIX4& var, const VESSEL* myVessel)
{
m_matrices4[MakeID(sender, myVessel, varName)] = var;
}
void MMExt2_Core::Put(const ModuleMessagingExtPut& sender, const char* varName, const ModuleMessagingExtBase* var, const VESSEL* myVessel)
{
m_basepointers[MakeID(sender, myVessel, varName)] = var;
}
void MMExt2_Core::Put(const ModuleMessagingExtPut& sender, const char* varName, string var, const VESSEL* myVessel)
{
m_strings[MakeID(sender, myVessel, varName)] = var;
}


bool MMExt2_Core::Delete(const ModuleMessagingExtPut& sender, const char* varName, bool value, const VESSEL* myVessel)
{
return SearchMapDelete<bool>(sender.ModuleMessagingGetModuleName(), myVessel, varName, m_bools);
}
bool MMExt2_Core::Delete(const ModuleMessagingExtPut& sender, const char* varName, int value, const VESSEL* myVessel)
{
return SearchMapDelete<int>(sender.ModuleMessagingGetModuleName(), myVessel, varName, m_ints);
}
bool MMExt2_Core::Delete(const ModuleMessagingExtPut& sender, const char* varName, double value, const VESSEL* myVessel)
{
return SearchMapDelete<double>(sender.ModuleMessagingGetModuleName(), myVessel, varName, m_doubles);
}
bool MMExt2_Core::Delete(const ModuleMessagingExtPut& sender, const char* varName, const VECTOR3 & value, const VESSEL* myVessel)
{
return SearchMapDelete<VECTOR3>(sender.ModuleMessagingGetModuleName(), myVessel, varName, m_vectors);
}
bool MMExt2_Core::Delete(const ModuleMessagingExtPut& sender, const char* varName, const MATRIX3 & value, const VESSEL* myVessel)
{
return SearchMapDelete<MATRIX3>(sender.ModuleMessagingGetModuleName(), myVessel, varName, m_matrices3);
}
bool MMExt2_Core::Delete(const ModuleMessagingExtPut& sender, const char* varName, const MATRIX4 & value, const VESSEL* myVessel)
{
return SearchMapDelete<MATRIX4>(sender.ModuleMessagingGetModuleName(), myVessel, varName, m_matrices4);
}
bool MMExt2_Core::Delete(const ModuleMessagingExtPut& sender, const char* varName, const ModuleMessagingExtBase* value, const VESSEL* myVessel)
{
return SearchMapDelete<const ModuleMessagingExtBase *>(sender.ModuleMessagingGetModuleName(), myVessel, varName, m_basepointers);
}
bool MMExt2_Core::Delete(const ModuleMessagingExtPut& sender, const char* varName, string value, const VESSEL* myVessel)
{
return SearchMapDelete<string>(sender.ModuleMessagingGetModuleName(), myVessel, varName, m_strings);
}

bool MMExt2_Core::Get(const char* moduleName, const char* varName, bool* value, const VESSEL* myVessel)
{
return SearchMap<bool>(moduleName, myVessel, varName, m_bools, value);
}
bool MMExt2_Core::Get(const char* moduleName, const char* varName, int* value, const VESSEL* myVessel)
{
return SearchMap<int>(moduleName, myVessel, varName, m_ints, value);
}
bool MMExt2_Core::Get(const char* moduleName, const char* varName, double* value, const VESSEL* myVessel)
{
return SearchMap<double>(moduleName, myVessel, varName, m_doubles, value);
}
bool MMExt2_Core::Get(const char* moduleName, const char* varName, VECTOR3* value, const VESSEL* myVessel)
{
return SearchMap<VECTOR3>(moduleName, myVessel, varName, m_vectors, value);
}
bool MMExt2_Core::Get(const char* moduleName, const char* varName, MATRIX3* value, const VESSEL* myVessel)
{
return SearchMap<MATRIX3>(moduleName, myVessel, varName, m_matrices3, value);
}
bool MMExt2_Core::Get(const char* moduleName, const char* varName, MATRIX4* value, const VESSEL* myVessel)
{
return SearchMap<MATRIX4>(moduleName, myVessel, varName, m_matrices4, value);
}
bool MMExt2_Core::Get(const char* moduleName, const char* varName, const ModuleMessagingExtBase** value, const VESSEL* myVessel)
{
return SearchMap<const ModuleMessagingExtBase *>(moduleName, myVessel, varName, m_basepointers, value);
}
bool MMExt2_Core::Get(const char* moduleName, const char* varName, string *value, const VESSEL* myVessel)
{
return SearchMap<string>(moduleName, myVessel, varName, m_strings, value);
}

*/
#define MMEXT2_PUT_GET(__ty, __in, __out) __MMEXT2_PUT_GET(__ty, __in, __out)
#define __MMEXT2_PUT_GET(__ty, __in, __out) \
bool MMExt2_Core::Put(const char* moduleName, const char* varName, const __ty __in value, const char* vesselName)\
{  m_##__ty##s[MakeID(moduleName, vesselName, varName)] = value;  return true;  };\
bool MMExt2_Core::Get(const char* moduleName, const char* varName, __ty __out value, const char* vesselName)\
{ return SearchMap<__ty>(moduleName, vesselName, varName, m_##__ty##s, value); }

MMEXT2_PUT_GET(int, , *);
MMEXT2_PUT_GET(bool, , *);
MMEXT2_PUT_GET(double, , *);
MMEXT2_PUT_GET(string, , *);
MMEXT2_PUT_GET(VECTOR3, &, *);
MMEXT2_PUT_GET(MATRIX3, &, *);
MMEXT2_PUT_GET(MATRIX4, &, *);


bool MMExt2_Core::Delete(const char* moduleName, const char* varName, const char* vesselName)
{
#define MMEXT2_DEL(__ty) __MMEXT2_DEL(__ty)
#define __MMEXT2_DEL(__ty) delFound = (SearchMapDelete<__ty>(moduleName, vesselName, varName, m_##__ty##s)? true : delFound)

    bool delFound = false;
    MMEXT2_DEL(int);
    MMEXT2_DEL(bool);
    MMEXT2_DEL(double);
    MMEXT2_DEL(string);
    MMEXT2_DEL(VECTOR3);
    MMEXT2_DEL(MATRIX3);
    MMEXT2_DEL(MATRIX4);
    return delFound;
};

MMExt2_Core gCore;

// 
// STATIC ENTRY POINTS
// If you chnage the interface, make a new V2, V3 entry and fix up the compatibility for these original ones. 
//

#define MMEXT2_INTERFACE_PUT_GET(__ty, __in, __out) __MMEXT2_INTERFACE_PUT_GET(__ty, __in, __out)
#define __MMEXT2_INTERFACE_PUT_GET(__ty, __in, __out) \
DLLCLBK bool ModMsgPut_##__ty##_v1(const char* moduleName, const char* varName, const __ty __in value, const char* vesselName) {\
return gCore.Put(moduleName, varName, value, vesselName);\
}\
DLLCLBK bool ModMsgGet_##__ty##_v1(const char* moduleName, const char* varName, __ty __out value, const char* vesselName) {\
  return gCore.Get(moduleName, varName, value, vesselName);\
}

MMEXT2_INTERFACE_PUT_GET(int, , *);
MMEXT2_INTERFACE_PUT_GET(bool, , *);
MMEXT2_INTERFACE_PUT_GET(double, , *);
MMEXT2_INTERFACE_PUT_GET(VECTOR3, &, *);
MMEXT2_INTERFACE_PUT_GET(MATRIX3, &, *);
MMEXT2_INTERFACE_PUT_GET(MATRIX4, &, *);

// Special handling for char* into string
DLLCLBK bool ModMsgPut_c_str_v1(const char* moduleName, const char* varName, const char* value, const char* vesselName) {
  string str = value;
  return gCore.Put(moduleName, varName, str, vesselName);
}
DLLCLBK bool ModMsgGet_c_str_v1(const char* moduleName, const char* varName, char **value, const size_t valueMaxLen, const char* vesselName) {
  string str;
  if (!gCore.Get(moduleName, varName, &str, vesselName)) {
    *value[0] = 0;
    return false;
  }
  errno_t err = strcpy_s(*value, valueMaxLen, str.c_str());
  return (!err);
}


DLLCLBK bool ModMsgDel_any_v1(const char* moduleName, const char* varName, const char* vesselName) {
  return gCore.Delete(moduleName, varName, vesselName);
}