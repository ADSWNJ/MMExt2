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
// <http://www.gnu.org/licenses/>
// ==============================================================

#include "MMExt2_Core.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>

using namespace MMExt2;
using namespace std;

#define TOKEN_VALUE 186

MMExt2_Core gCore;
map<string, bool> MMExt2_Core::m_bools;
map<string, int> MMExt2_Core::m_ints;
map<string, double> MMExt2_Core::m_doubles;
map<string, VECTOR3> MMExt2_Core::m_VECTOR3s;
map<string, MATRIX3> MMExt2_Core::m_MATRIX3s;
map<string, MATRIX4> MMExt2_Core::m_MATRIX4s;
map<string, string> MMExt2_Core::m_strings;
map<string, const MMStruct*> MMExt2_Core::m_MMStructs;
map<string, char> MMExt2_Core::m_types;
const char MMExt2_Core::m_token = char(TOKEN_VALUE);

MMExt2_Core::MMExt2_Core()
{}

MMExt2_Core::~MMExt2_Core()
{}

template<class T>
static bool MMExt2_Core::SearchMap(const string& get, const string& id, const map<string, T>& mapToSearch, T* returnValue)
{
  if (id.length() == 0) return false;
  map<string, T>::const_iterator it = mapToSearch.find(id);
  if (it != mapToSearch.end()) {
    *returnValue = it->second;
    return true;
  } else {
    return false;
  }
}

template<class T>
static bool MMExt2_Core::SearchMapDelete(const string &id, map<string, T>& mapToSearch)
{
  //map<string, T>::iterator it = mapToSearch.find(id);
  //if (it == mapToSearch.end()) return false;
  return (mapToSearch.erase(id)>0);
  //return true;
}

bool MMExt2_Core::Find(const string& fGet, const string& fMod, const string& fVar, const string& fVes, int* fIx, bool skp, string* rMod, string* rVar, string* rVes, char* rTyp) {
  int foundIx = 0;
  string itVes, itMod, itVar;
  for (const auto& it : m_types) {
    istringstream is(it.first);
    getline(is, itVes, m_token);
    getline(is, itMod, m_token);
    getline(is, itVar, m_token);
    if (((fMod == "*") || (fMod == itMod)) && ((fVar == "*") || (fVar == itVar)) && ((fVes == "") || (fVes == itVes))  && ((!skp) || (fGet != itMod))) {
      if (foundIx >= *fIx) {
        *rTyp = it.second;
        *rMod = itMod;
        *rVar = itVar;
        *rVes = itVes;
        return true;
      } else {
        foundIx++;
      }
    }
  }
  return false; 
}

bool MMExt2_Core::Put(const string& id, const bool& val)                         {if (!Delete(id)) return false;  m_types[id] = 'b';  m_bools[id] = val;      return true;}
bool MMExt2_Core::Put(const string& id, const int& val)                          {if (!Delete(id)) return false;  m_types[id] = 'i';  m_ints[id] = val;       return true;}
bool MMExt2_Core::Put(const string& id, const double& val)                       {if (!Delete(id)) return false;  m_types[id] = 'd';  m_doubles[id] = val;    return true;}
bool MMExt2_Core::Put(const string& id, const string& val)                       {if (!Delete(id)) return false;  m_types[id] = 's';  m_strings[id] = val;    return true;}
bool MMExt2_Core::Put(const string& id, const VECTOR3& val)                      {if (!Delete(id)) return false;  m_types[id] = 'v';  m_VECTOR3s[id] = val;   return true;}
bool MMExt2_Core::Put(const string& id, const MATRIX3& val)                      {if (!Delete(id)) return false;  m_types[id] = '3';  m_MATRIX3s[id] = val;   return true;}
bool MMExt2_Core::Put(const string& id, const MATRIX4& val)                      {if (!Delete(id)) return false;  m_types[id] = '4';  m_MATRIX4s[id] = val;   return true;}
bool MMExt2_Core::Put(const string& id, const MMStruct* val)                     {if (!Delete(id)) return false;  m_types[id] = 'X';  m_MMStructs[id] = val;  return true;}

bool MMExt2_Core::Get(const string& get, const string& id, int* val)             {return SearchMap<int>(    get, id, m_ints,     val);}
bool MMExt2_Core::Get(const string& get, const string& id, bool* val)            {return SearchMap<bool>(   get, id, m_bools,    val);}
bool MMExt2_Core::Get(const string& get, const string& id, double* val)          {return SearchMap<double>( get, id, m_doubles,  val);}
bool MMExt2_Core::Get(const string& get, const string& id, string* val)          {return SearchMap<string>( get, id, m_strings,  val);}
bool MMExt2_Core::Get(const string& get, const string& id, VECTOR3* val)         {return SearchMap<VECTOR3>(get, id, m_VECTOR3s, val);}
bool MMExt2_Core::Get(const string& get, const string& id, MATRIX3* val)         {return SearchMap<MATRIX3>(get, id, m_MATRIX3s, val);}
bool MMExt2_Core::Get(const string& get, const string& id, MATRIX4* val)         {return SearchMap<MATRIX4>(get, id, m_MATRIX4s, val);}
bool MMExt2_Core::Get(const string& get, const string& id, const MMStruct** val) {return SearchMap<const MMStruct *>(get, id, m_MMStructs, val);}

bool MMExt2_Core::Delete(const string& id) {
  if (id.length() == 0) return false;
  if (m_types.count(id)) {
    if (m_types[id] == 'X') return false;
    if (!DeleteType(id, m_types[id])) return false;
    m_types.erase(id);
  }
  return true;
}

bool MMExt2_Core::DeleteType(const string &id, const char type) {
    bool delFound = false;
    switch (type) {
    case 'i':
      delFound = (SearchMapDelete<int>(id, m_ints) ? true : delFound);
      break;
    case 'b':
      delFound = (SearchMapDelete<bool>(id, m_bools) ? true : delFound);
      break;
    case 'd':
      delFound = (SearchMapDelete<double>(id, m_doubles) ? true : delFound);
      break;
    case 's':
      delFound = (SearchMapDelete<string>(id, m_strings) ? true : delFound);
      break;
    case 'v':
      delFound = (SearchMapDelete<VECTOR3>(id, m_VECTOR3s) ? true : delFound);
      break;
    case '3':
      delFound = (SearchMapDelete<MATRIX3>(id, m_MATRIX3s) ? true : delFound);
      break;
    case '4':
      delFound = (SearchMapDelete<MATRIX4>(id, m_MATRIX4s) ? true : delFound);
    }
    return delFound;
}





// 
// STATIC ENTRY POINTS
// If you change this interface, make a new V2, V3 set of entry points and fix up the compatibility for all apps using these original ones. 
//

inline std::string _ID(const char* mod, const char* var, const char* ves) {
  const char token = char(TOKEN_VALUE);
  string id = "", s_ves = ves, s_mod = mod, s_var = var;
  if (s_ves.length() == 0 || s_mod.length() == 0 || s_var.length() == 0) return id;
  if (s_ves.find(token) != -1 || s_mod.find(token) != -1 || s_var.find(token) != -1) return id;
  id = s_ves + token + s_mod + token + s_var;
  return id;
}

DLLCLBK bool ModMsgPut_int_v1(                      const char* mod, const char* var, const int& val,       const char* ves) { return gCore.Put(_ID(mod, var, ves), val); }
DLLCLBK bool ModMsgPut_bool_v1(                     const char* mod, const char* var, const bool& val,      const char* ves) { return gCore.Put(_ID(mod, var, ves), val); }
DLLCLBK bool ModMsgPut_double_v1(                   const char* mod, const char* var, const double& val,    const char* ves) { return gCore.Put(_ID(mod, var, ves), val); }
DLLCLBK bool ModMsgPut_VECTOR3_v1(                  const char* mod, const char* var, const VECTOR3& val,   const char* ves) { return gCore.Put(_ID(mod, var, ves), val); }
DLLCLBK bool ModMsgPut_MATRIX3_v1(                  const char* mod, const char* var, const MATRIX3& val,   const char* ves) { return gCore.Put(_ID(mod, var, ves), val); }
DLLCLBK bool ModMsgPut_MATRIX4_v1(                  const char* mod, const char* var, const MATRIX4& val,   const char* ves) { return gCore.Put(_ID(mod, var, ves), val); }
DLLCLBK bool ModMsgPut_MMStruct_v1(                 const char* mod, const char* var, const MMStruct* val,  const char* ves) { return gCore.Put(_ID(mod, var, ves), val); }

DLLCLBK bool ModMsgDel_any_v1(                      const char* mod, const char* var,                       const char* ves) { return gCore.Delete(_ID(mod, var, ves)); }

DLLCLBK bool ModMsgGet_int_v1(     const char* get, const char* mod, const char* var, int* val,             const char* ves) { return gCore.Get(string(get), _ID(mod, var, ves), val); }
DLLCLBK bool ModMsgGet_bool_v1(    const char* get, const char* mod, const char* var, bool* val,            const char* ves) { return gCore.Get(string(get), _ID(mod, var, ves), val); }
DLLCLBK bool ModMsgGet_double_v1(  const char* get, const char* mod, const char* var, double* val,          const char* ves) { return gCore.Get(string(get), _ID(mod, var, ves), val); }
DLLCLBK bool ModMsgGet_VECTOR3_v1( const char* get, const char* mod, const char* var, VECTOR3* val,         const char* ves) { return gCore.Get(string(get), _ID(mod, var, ves), val); }
DLLCLBK bool ModMsgGet_MATRIX3_v1( const char* get, const char* mod, const char* var, MATRIX3* val,         const char* ves) { return gCore.Get(string(get), _ID(mod, var, ves), val); }
DLLCLBK bool ModMsgGet_MATRIX4_v1( const char* get, const char* mod, const char* var, MATRIX4* val,         const char* ves) { return gCore.Get(string(get), _ID(mod, var, ves), val); }
DLLCLBK bool ModMsgGet_MMStruct_v1(const char* get, const char* mod, const char* var, const MMStruct** val, const char* ves) { return gCore.Get(string(get), _ID(mod, var, ves), val); }


// Special handling for string (do not want to expose the string implementation across compiler versions due to lack of ABI; therefore use char* as the interface)
DLLCLBK bool ModMsgPut_c_str_v1(const char* mod, const char* var, const char* val, const char* ves) {
  string str = val;
  return gCore.Put(_ID(mod, var, ves), str);
}
DLLCLBK bool ModMsgGet_c_str_v1(const char* get, const char* mod, const char* var, char *val, size_t *len, const char* ves) {
  string str;
  if (!gCore.Get(string(get), _ID(mod, var, ves), &str)) return false;
  if (str.length() < *len) strcpy_s(val, *len, str.c_str());
  *len = str.length() + 1;
  return true;
}

DLLCLBK bool ModMsgFind_v1(const char* get, const char* findMod, const char* findVar, const char* findVes,
                        bool skipSelf, int *fIx, char* typ,
                        char* mod, size_t* lMod, char* var, size_t* lVar, char* ves, size_t* lVes) {
  string fGet = get, fMod = findMod, fVar = findVar, fVes = (findVes?findVes:""), rMod, rVar, rVes;
  if (!gCore.Find(fGet, fMod, fVar, fVes, fIx, skipSelf, &rMod, &rVar, &rVes, typ)) return false; 
  if (rMod.length() < *lMod) strcpy_s(mod, *lMod, rMod.c_str());
  *lMod = rMod.length() + 1;
  if (rVar.length() < *lVar) strcpy_s(var, *lVar, rVar.c_str());
  *lVar = rVar.length() + 1;
  if (rVes.length() < *lVes) strcpy_s(ves, *lVes, rVes.c_str());
  *lVes = rVes.length() + 1;
  return true;
}

