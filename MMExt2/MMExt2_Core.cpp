// ==============================================================
//                ORBITER AUX LIBRARY: ModuleMessagingExt
//             http://sf.net/projects/enjomitchsorbit
//
// Allows Orbiter modules to communicate with each other,
// using predefined module and variable names.
//
// Copyright  (C) 2014-2018 Szymon "Enjo" Ender and Andrew "ADSWNJ" Stokes
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
#include <sstream>

using namespace MMExt2;
using namespace std;

#define MMEXT2_VERSION_NUMBER "v2.0"
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
map<string, const EnjoLib::ModuleMessagingExtBase*> MMExt2_Core::m_MMBases;
map<string, char> MMExt2_Core::m_types;
vector<string>  MMExt2_Core::m_activitylog;
const char MMExt2_Core::m_token = char(TOKEN_VALUE);

MMExt2_Core::MMExt2_Core() {}

MMExt2_Core::~MMExt2_Core() {}

inline std::string _Id(const char* mod, const char* var, const char* ves) {
  const char token = char(TOKEN_VALUE);
  string id = "", s_ves = ves, s_mod = mod, s_var = var;
  if (s_ves.length() == 0 || s_mod.length() == 0 || s_var.length() == 0) return id;
  if (s_ves.find(token) != -1 || s_mod.find(token) != -1 || s_var.find(token) != -1) return id;
  id = s_ves + token + s_mod + token + s_var;
  return id;
}

inline void _RemoteCopy(char* rS, size_t *rLenS, const string &lS) {
  if (lS.length() < *rLenS) strcpy_s(rS, *rLenS, lS.c_str());
  *rLenS = lS.length() + 1;
}

template<class T>
static bool MMExt2_Core::SearchMap(const string& get, const string& id, const map<string, T>& mapToSearch, T* returnValue) {
  if (id.length() == 0) return false;
  map<string, T>::const_iterator it = mapToSearch.find(id);
  if (it != mapToSearch.end()) {
    *returnValue = it->second;
    return Log(get, "G", true, id);
  } else {
    return Log(get, "G", false, id);
  }
}

template<class T>
static bool MMExt2_Core::SearchMapDelete(const string &id, map<string, T>& mapToSearch) {
  return (mapToSearch.erase(id)>0);
}

template<class T>
static bool MMExt2_Core::PutMap(const string& cli, const string& id, const char& typ, map<string, T> &mapToStore, const T &val) {
  if (!Delete(cli, id, typ)) return false;
  m_types[id] = typ;
  mapToStore[id] = val;
  return Log(cli, "P", true, id);
}

bool MMExt2_Core::Put(const string& cli, const string& id, const bool& val)      { return PutMap<bool>(cli, id, 'b', m_bools, val); }
bool MMExt2_Core::Put(const string& cli, const string& id, const int& val)       { return PutMap<int>(cli, id, 'i', m_ints, val); }
bool MMExt2_Core::Put(const string& cli, const string& id, const double& val)    { return PutMap<double>(cli, id, 'd', m_doubles, val); }
bool MMExt2_Core::Put(const string& cli, const string& id, const string& val)    { return PutMap<string>(cli, id, 's', m_strings, val); }
bool MMExt2_Core::Put(const string& cli, const string& id, const VECTOR3& val)   { return PutMap<VECTOR3>(cli, id, 'v', m_VECTOR3s, val); }
bool MMExt2_Core::Put(const string& cli, const string& id, const MATRIX3& val)   { return PutMap<MATRIX3>(cli, id, '3', m_MATRIX3s, val); }
bool MMExt2_Core::Put(const string& cli, const string& id, const MATRIX4& val)   { return PutMap<MATRIX4>(cli, id, '4', m_MATRIX4s, val); }

bool MMExt2_Core::Put(const string& cli, const string& id, const MMStruct* val)  {
  if (!Delete(cli, id, 'x')) return false;
  m_types[id] = 'x';
  m_MMStructs[id] = val;
  return Log(cli, "P", true, id);
}

bool MMExt2_Core::Put(const string& cli, const string& id, const EnjoLib::ModuleMessagingExtBase* val) {
  if (!Delete(cli, id, 'y')) return false;
  m_types[id] = 'y';
  m_MMBases[id] = val;
  return Log(cli, "P", true, id);
}

bool MMExt2_Core::Get(const string& cli, const string& id, int* val)             { return SearchMap<int>(    cli, id, m_ints,     val); }
bool MMExt2_Core::Get(const string& cli, const string& id, bool* val)            { return SearchMap<bool>(   cli, id, m_bools,    val); }
bool MMExt2_Core::Get(const string& cli, const string& id, double* val)          { return SearchMap<double>( cli, id, m_doubles,  val); }
bool MMExt2_Core::Get(const string& cli, const string& id, string* val)          { return SearchMap<string>( cli, id, m_strings,  val); }
bool MMExt2_Core::Get(const string& cli, const string& id, VECTOR3* val)         { return SearchMap<VECTOR3>(cli, id, m_VECTOR3s, val); }
bool MMExt2_Core::Get(const string& cli, const string& id, MATRIX3* val)         { return SearchMap<MATRIX3>(cli, id, m_MATRIX3s, val); }
bool MMExt2_Core::Get(const string& cli, const string& id, MATRIX4* val)         { return SearchMap<MATRIX4>(cli, id, m_MATRIX4s, val); }
bool MMExt2_Core::Get(const string& cli, const string& id, const MMStruct** val) { return SearchMap<const MMStruct *>(cli, id, m_MMStructs, val); }
bool MMExt2_Core::Get(const string& cli, const string& id, const EnjoLib::ModuleMessagingExtBase** val) { return SearchMap<const EnjoLib::ModuleMessagingExtBase *>(cli, id, m_MMBases, val); }

bool MMExt2_Core::Delete(const string& cli, const string& id, const char& c) {
  if (id.length() == 0) return false;
  if (m_types.count(id)) {
    if (m_types[id] == 'x' || m_types[id] == 'y') return Log(cli, "D", false, id);
    if (m_types[id] == c) return true;
    if (!DeleteType(id, m_types[id])) return false;
    m_types.erase(id);
    return Log(cli, "D", true, id);
  }
  return true;
}

bool MMExt2_Core::DeleteType(const string &id, const char type) {
  bool delFound = false;
  switch (type) {
  case 'i':    delFound = SearchMapDelete<int>(id, m_ints);          break;
  case 'b':    delFound = SearchMapDelete<bool>(id, m_bools);        break;
  case 'd':    delFound = SearchMapDelete<double>(id, m_doubles);    break;
  case 's':    delFound = SearchMapDelete<string>(id, m_strings);    break;
  case 'v':    delFound = SearchMapDelete<VECTOR3>(id, m_VECTOR3s);  break;
  case '3':    delFound = SearchMapDelete<MATRIX3>(id, m_MATRIX3s);  break;
  case '4':    delFound = SearchMapDelete<MATRIX4>(id, m_MATRIX4s);  break;
  }
  return delFound;
}

bool MMExt2_Core::Log(const string& cli, const string& act, const bool& res, const string& id) {
  string s = id;
  if (s == "") s = string() + "*" + m_token + "*" + m_token + "*";
  string logmsg = cli + m_token + act + m_token + (res?"S":"F") + m_token + s; 
  auto it = find(m_activitylog.begin(),m_activitylog.end(),logmsg);
  if (it == m_activitylog.end()) m_activitylog.push_back(logmsg);
  return res;
}

bool MMExt2_Core::GetLog(const string& get, const int ix, char *rFunc, bool *rSuccess, string *rCli, string *rMod, string* rVar, string* rVes) {
  if (ix == 0) Log(get, "L", true, "");
  int rIx = m_activitylog.size() - ix - 1;
  if (rIx < 0) return false;
  string func, success;
  istringstream is(m_activitylog[rIx]);
  getline(is, *rCli, m_token);
  getline(is, func, m_token);
  getline(is, success, m_token);
  getline(is, *rVes, m_token);
  getline(is, *rMod, m_token);
  getline(is, *rVar, m_token);
  *rFunc = func[0];
  *rSuccess = (success == "S");
  return true;
}

bool MMExt2_Core::GetVer(const char* mod, char* val, size_t *len) {
  string s = string() + "MMExt " + MMEXT2_VERSION_NUMBER + " - " + __DATE__;
  if (*len > s.length()) {
    strcpy(val, s.c_str());
  };
  *len = s.length() + 1;
  return Log(mod, "V", true, "");
}

bool MMExt2_Core::Find(const string& fGet, const string& fMod, const string& fVar, const string& fVes, int* fIx, bool skp, string* rMod, string* rVar, string* rVes, char* rTyp) {
  int foundIx = 0;
  string itVes, itMod, itVar;
  for (const auto& it : m_types) {
    istringstream is(it.first);
    getline(is, itVes, m_token);
    getline(is, itMod, m_token);
    getline(is, itVar, m_token);
    if (((fMod == "*") || (fMod == itMod)) && ((fVar == "*") || (fVar == itVar)) && ((fVes == "*") || (fVes == itVes)) && ((!skp) || (fGet != itMod))) {
      if (foundIx >= *fIx) {
        *rTyp = it.second;
        *rMod = itMod;
        *rVar = itVar;
        *rVes = itVes;
        if (*fIx == 0) Log(fGet, "F", true, fVes + m_token + fMod + m_token + fVar);
        return true;
      } else {
        foundIx++;
      }
    }
  }
  return false;
}

// 
// STATIC ENTRY POINTS FOR MMExt2_Internal
// If you change this interface, make a new V2, V3 set of entry points and fix up the compatibility for all apps using these original ones. 
//

DLLCLBK bool ModMsgGet_ver_v1(                      const char* mod,                  char* val, size_t *len)                { return gCore.GetVer(mod, val, len); };
DLLCLBK bool ModMsgPut_int_v1(                      const char* mod, const char* var, const int& val,       const char* ves) { return gCore.Put(string(mod), _Id(mod, var, ves), val); }
DLLCLBK bool ModMsgPut_bool_v1(                     const char* mod, const char* var, const bool& val,      const char* ves) { return gCore.Put(string(mod), _Id(mod, var, ves), val); }
DLLCLBK bool ModMsgPut_double_v1(                   const char* mod, const char* var, const double& val,    const char* ves) { return gCore.Put(string(mod), _Id(mod, var, ves), val); }
DLLCLBK bool ModMsgPut_VECTOR3_v1(                  const char* mod, const char* var, const VECTOR3& val,   const char* ves) { return gCore.Put(string(mod), _Id(mod, var, ves), val); }
DLLCLBK bool ModMsgPut_MATRIX3_v1(                  const char* mod, const char* var, const MATRIX3& val,   const char* ves) { return gCore.Put(string(mod), _Id(mod, var, ves), val); }
DLLCLBK bool ModMsgPut_MATRIX4_v1(                  const char* mod, const char* var, const MATRIX4& val,   const char* ves) { return gCore.Put(string(mod), _Id(mod, var, ves), val); }
DLLCLBK bool ModMsgPut_MMStruct_v1(                 const char* mod, const char* var, const MMStruct* val,  const char* ves) { return gCore.Put(string(mod), _Id(mod, var, ves), val); }
DLLCLBK bool ModMsgPut_MMBase_v1(                   const char* mod, const char* var, const EnjoLib::ModuleMessagingExtBase* val, const char* ves)
                                                                                                                             { return gCore.Put(string(mod), _Id(mod, var, ves), val); }
DLLCLBK bool ModMsgDel_any_v1(     const char* mod, const char* var, const char* ves) { return gCore.Delete(mod, _Id(mod, var, ves)); }
DLLCLBK bool ModMsgGet_int_v1(     const char* cli, const char* mod, const char* var, int* val,             const char* ves) { return gCore.Get(string(cli), _Id(mod, var, ves), val); }
DLLCLBK bool ModMsgGet_bool_v1(    const char* cli, const char* mod, const char* var, bool* val,            const char* ves) { return gCore.Get(string(cli), _Id(mod, var, ves), val); }
DLLCLBK bool ModMsgGet_double_v1(  const char* cli, const char* mod, const char* var, double* val,          const char* ves) { return gCore.Get(string(cli), _Id(mod, var, ves), val); }
DLLCLBK bool ModMsgGet_VECTOR3_v1( const char* cli, const char* mod, const char* var, VECTOR3* val,         const char* ves) { return gCore.Get(string(cli), _Id(mod, var, ves), val); }
DLLCLBK bool ModMsgGet_MATRIX3_v1( const char* cli, const char* mod, const char* var, MATRIX3* val,         const char* ves) { return gCore.Get(string(cli), _Id(mod, var, ves), val); }
DLLCLBK bool ModMsgGet_MATRIX4_v1( const char* cli, const char* mod, const char* var, MATRIX4* val,         const char* ves) { return gCore.Get(string(cli), _Id(mod, var, ves), val); }
DLLCLBK bool ModMsgGet_MMStruct_v1(const char* cli, const char* mod, const char* var, const MMStruct** val, const char* ves) { return gCore.Get(string(cli), _Id(mod, var, ves), val); }
DLLCLBK bool ModMsgGet_MMBase_v1(const char* cli, const char* mod, const char* var, const EnjoLib::ModuleMessagingExtBase** val, const char* ves)
                                                                                                                             { return gCore.Get(string(cli), _Id(mod, var, ves), val); }

// Special handling for all string functions (do not want to expose the string implementation across compiler versions due to lack of ABI; therefore use char* as the interface)

DLLCLBK bool ModMsgPut_c_str_v1(const char* mod, const char* var, const char* val, const char* ves) {
  string str = val;
  return gCore.Put(string(mod), _Id(mod, var, ves), str);
}

DLLCLBK bool ModMsgGet_c_str_v1(const char* cli, const char* mod, const char* var, char *val, size_t *lVal, const char* ves) {
  string rVal;
  if (!gCore.Get(string(cli), _Id(mod, var, ves), &rVal)) return false;
  _RemoteCopy(val, lVal, rVal);
  return true;
}

DLLCLBK bool ModMsgFind_v1(const char* cli, const char* findMod, const char* findVar, const char* findVes,
                        bool skipSelf, int *fIx, char* typ,
                        char* mod, size_t* lMod, char* var, size_t* lVar, char* ves, size_t* lVes) {
  string fGet = cli, fMod = findMod, fVar = findVar, fVes = findVes, rMod, rVar, rVes;
  if (!gCore.Find(fGet, fMod, fVar, fVes, fIx, skipSelf, &rMod, &rVar, &rVes, typ)) return false; 
  _RemoteCopy(mod, lMod, rMod);
  _RemoteCopy(var, lVar, rVar);
  _RemoteCopy(ves, lVes, rVes);
  return true;
}

DLLCLBK bool ModMsgGet_log_v1(const char* get, const int ix, char* func, bool* success,
                              char *cli, size_t* lCli,
                              char* mod, size_t* lMod,
                              char* var, size_t* lVar,
                              char* ves, size_t* lVes) {
  string rGet = get, rCli, rMod, rVar, rVes;
  if (!gCore.GetLog(rGet, ix, func, success, &rCli, &rMod, &rVar, &rVes)) return false;

  _RemoteCopy(cli, lCli, rCli);
  _RemoteCopy(mod, lMod, rMod);
  _RemoteCopy(var, lVar, rVar);
  _RemoteCopy(ves, lVes, rVes);
  return true;
}
