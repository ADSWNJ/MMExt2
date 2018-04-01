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

#pragma warning( default : 4571 ) // Enables exception on try/catch with no SEH enabled - i.e. C++ Code Generation, Enable C++ Exceptions, Yes with SEH exceptions (/EHa).

using namespace MMExt2;
using namespace std;

#define MMEXT2_VERSION_NUMBER "v2.1"
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
map<string, OBJHANDLE> MMExt2_Core::m_OBJHANDLEs;
map<string, char> MMExt2_Core::m_types;
vector<string>  MMExt2_Core::m_activitylog;
const char MMExt2_Core::m_token = char(TOKEN_VALUE);

MMExt2_Core::MMExt2_Core() {}

MMExt2_Core::~MMExt2_Core() {}

inline int _ObjType(const OBJHANDLE& val) {
  int obj_type = OBJTP_INVALID;
  try {
    obj_type = oapiGetObjectType(val); // if bad pointer, we can Access Violate on this call, so be very defensive around it. 
  }
  catch (...) { // If you get C4571 compile error here, you must enable /EHa to catch structured exceptions- i.e. C++ Code Generation, Enable C++ Exceptions, Yes with SEH exceptions (/EHa).
    obj_type = OBJTP_INVALID;
  }
  return obj_type;
}

inline bool _IsVessel(OBJHANDLE ohv) {
  return (_ObjType(ohv) == OBJTP_VESSEL); // Looking for 10. If 0, then objtype is INVALID. Check if they are sending an OBJHANDLE or a VESSEL* ... we want an OBJHANDLE. 
}

inline std::string _Id(const char* mod, const char* var, const OBJHANDLE ohv, const bool allowNullOhv = false) {
  const char token = char(TOKEN_VALUE);
  string id = "", s_ohv = "", s_mod = mod, s_var = var;
  if (!allowNullOhv) {
    if (!_IsVessel(ohv)) return id;
  }
  char c_ohv[32];
  sprintf_s(c_ohv, 32, "%p", ohv);
  s_ohv = c_ohv;
  if (s_mod.length() == 0 || s_var.length() == 0) return id;
  if (s_mod.find(token) != -1 || s_var.find(token) != -1) return id;
  id = s_ohv + token + s_mod + token + s_var;
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
static bool MMExt2_Core::PutMap(const string& cli, const string& id, const char& typ, map<string, T> &mapToStore, const T& val) {
  if (!Delete(cli, id, typ)) return false;
  m_types[id] = typ;
  mapToStore[id] = val;
  return Log(cli, "P", true, id);
}

bool MMExt2_Core::Put(const string& cli, const string& id, const bool& val)      { return PutMap<bool>(     cli, id, 'b', m_bools,      val); }
bool MMExt2_Core::Put(const string& cli, const string& id, const int& val)       { return PutMap<int>(      cli, id, 'i', m_ints,       val); }
bool MMExt2_Core::Put(const string& cli, const string& id, const double& val)    { return PutMap<double>(   cli, id, 'd', m_doubles,    val); }
bool MMExt2_Core::Put(const string& cli, const string& id, const string& val)    { return PutMap<string>(   cli, id, 's', m_strings,    val); }
bool MMExt2_Core::Put(const string& cli, const string& id, const VECTOR3& val)   { return PutMap<VECTOR3>(  cli, id, 'v', m_VECTOR3s,   val); }
bool MMExt2_Core::Put(const string& cli, const string& id, const MATRIX3& val)   { return PutMap<MATRIX3>(  cli, id, '3', m_MATRIX3s,   val); }
bool MMExt2_Core::Put(const string& cli, const string& id, const MATRIX4& val)   { return PutMap<MATRIX4>(  cli, id, '4', m_MATRIX4s,   val); }

bool MMExt2_Core::Put(const string& cli, const string& id, const OBJHANDLE& val) {
  if (!ValidateObjHandle(cli, id, val)) return false;
  return PutMap<OBJHANDLE>(cli, id, 'o', m_OBJHANDLEs, val);
}

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

bool MMExt2_Core::Get(const string& cli, const string& id, int* val)             { return SearchMap<int>(      cli, id, m_ints,       val); }
bool MMExt2_Core::Get(const string& cli, const string& id, bool* val)            { return SearchMap<bool>(     cli, id, m_bools,      val); }
bool MMExt2_Core::Get(const string& cli, const string& id, double* val)          { return SearchMap<double>(   cli, id, m_doubles,    val); }
bool MMExt2_Core::Get(const string& cli, const string& id, string* val)          { return SearchMap<string>(   cli, id, m_strings,    val); }
bool MMExt2_Core::Get(const string& cli, const string& id, VECTOR3* val)         { return SearchMap<VECTOR3>(  cli, id, m_VECTOR3s,   val); }
bool MMExt2_Core::Get(const string& cli, const string& id, MATRIX3* val)         { return SearchMap<MATRIX3>(  cli, id, m_MATRIX3s,   val); }
bool MMExt2_Core::Get(const string& cli, const string& id, MATRIX4* val)         { return SearchMap<MATRIX4>(  cli, id, m_MATRIX4s,   val); }

bool MMExt2_Core::Get(const string& cli, const string& id, OBJHANDLE* val){
  bool ret = SearchMap<OBJHANDLE>(cli, id, m_OBJHANDLEs, val);
  if (!ret) return false;
  return ValidateObjHandle(cli, id, *val);
}

bool MMExt2_Core::ValidateObjHandle(const string& cli, const string& id, const OBJHANDLE obj) {
  return (ObjType("", id, obj) != OBJTP_INVALID);
}

int MMExt2_Core::ObjType(const string& cli, const string& id, const OBJHANDLE& val) {
  int obj_type = _ObjType(val);
  if (obj_type == OBJTP_INVALID) Delete("{core}", id, '\0'); // Expunge bad objects from the core
  if (cli!="") Log(cli, "T", (obj_type == OBJTP_INVALID), id);
  return obj_type;
}


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
  case 'i':    delFound = SearchMapDelete<int>(id, m_ints);              break;
  case 'b':    delFound = SearchMapDelete<bool>(id, m_bools);            break;
  case 'd':    delFound = SearchMapDelete<double>(id, m_doubles);        break;
  case 's':    delFound = SearchMapDelete<string>(id, m_strings);        break;
  case 'v':    delFound = SearchMapDelete<VECTOR3>(id, m_VECTOR3s);      break;
  case 'o':    delFound = SearchMapDelete<OBJHANDLE>(id, m_OBJHANDLEs);  break;
  case '3':    delFound = SearchMapDelete<MATRIX3>(id, m_MATRIX3s);      break;
  case '4':    delFound = SearchMapDelete<MATRIX4>(id, m_MATRIX4s);      break;
  }
  return delFound;
}

bool MMExt2_Core::Log(const string& cli, const string& act, const bool& res, const string& id) {
  string sohv, ves, mod, var;
  OBJHANDLE ohv;
  string s = (id == ""? string() + "00000000" + m_token + "*" + m_token + "*" : id);

  istringstream is(s);
  getline(is, sohv, m_token);
  int ret = sscanf(sohv.c_str(), "%p", &ohv);
  if (ret != 1) return false;
  getline(is, mod, m_token);
  getline(is, var, m_token);
  if (!ohv) {
    ves = "*";
  } else {
    if (!_IsVessel(ohv)) return false;
    ves = oapiGetVesselInterface(ohv)->GetName();
  }

  s = string() + ves + m_token + mod + m_token + var;
  string logmsg = cli + m_token + act + m_token + (res?"S":"F") + m_token + s; 
  auto it = find(m_activitylog.begin(),m_activitylog.end(),logmsg);
  if (it == m_activitylog.end()) m_activitylog.push_back(logmsg);
  return res;
}

bool MMExt2_Core::GetLog(char *rFunc, string *rCli, string *rMod, string* rVar, string* rVes, bool *rSuccess, int* ix, const string& cli, bool skp) {
  if (*ix == 0) Log(cli, "L", true, "");
  int rIx;
  bool repeat;
  do {
    repeat = false;
    rIx = m_activitylog.size() - *ix - 1;
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
    if (skp && (*rCli == cli)) {
      (*ix)++;
      repeat = true;
    }
  } while (repeat);
  (*ix)++;
  return true;
}

bool MMExt2_Core::ResetLog() {
  m_activitylog.clear();
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

bool MMExt2_Core::Find(char* rTyp, string* rMod, string* rVar, OBJHANDLE* rOhv, int* ix, const string& cli, const string& mod, const string& var, const OBJHANDLE ohv, bool skp) {
  int foundIx;
  string itsOhv, itMod, itVar, id;
  OBJHANDLE itOhv;
  bool doAgain;
  do {
    foundIx = 0;
    doAgain = false;
    for (const auto& it : m_types) {
      istringstream is(it.first);
      getline(is, itsOhv, m_token);
      int ret = sscanf(itsOhv.c_str(), "%p", &itOhv);
      if (ret != 1) return false;
      getline(is, itMod, m_token);
      getline(is, itVar, m_token);
      id = _Id((itMod).c_str(), (itVar).c_str(), itOhv);
      if (!ValidateObjHandle(cli, id, itOhv)) {
        doAgain = true; // invalidated pointers now on the m_types, so retry from beginning
        break;
      }

      if (((mod == "*") || (mod == itMod)) && ((var == "*") || (var == itVar)) && ((ohv == NULL) || (ohv == itOhv)) && ((!skp) || (cli != itMod))) {
        if (foundIx >= *ix) {
          *rTyp = it.second;
          *rMod = itMod;
          *rVar = itVar;
          *rOhv = itOhv;
          if (*rTyp == 'o') {
            OBJHANDLE obj_handle;
            SearchMap<OBJHANDLE>(cli, id, m_OBJHANDLEs, &obj_handle); // we know it will be true
            if (!ValidateObjHandle(cli, id, obj_handle)) {
              doAgain = true; // invalidated pointers now on the m_types
              break;
            }
          }
          if (*ix == 0) Log(cli, "F", true, _Id(mod.c_str(), var.c_str(), ohv, true));
          return true;
        }
      }
      foundIx++;
    }
  } while (doAgain);
  return false;
}

// 
// STATIC ENTRY POINTS FOR MMExt2_Internal
// If you change this interface, make a new V2, V3 set of entry points and fix up the compatibility for all apps using these original ones. 
//

DLLCLBK bool ModMsgGet_ver_v1(                       const char* mod,                  char* val, size_t *len)                    { return gCore.GetVer(mod, val, len); };
DLLCLBK bool ModMsgPut_int_v1(                       const char* mod, const char* var, const int& val,       const OBJHANDLE ohv) { return gCore.Put(string(mod), _Id(mod, var, ohv), val); }
DLLCLBK bool ModMsgPut_bool_v1(                      const char* mod, const char* var, const bool& val,      const OBJHANDLE ohv) { return gCore.Put(string(mod), _Id(mod, var, ohv), val); }
DLLCLBK bool ModMsgPut_double_v1(                    const char* mod, const char* var, const double& val,    const OBJHANDLE ohv) { return gCore.Put(string(mod), _Id(mod, var, ohv), val); }
DLLCLBK bool ModMsgPut_VECTOR3_v1(                   const char* mod, const char* var, const VECTOR3& val,   const OBJHANDLE ohv) { return gCore.Put(string(mod), _Id(mod, var, ohv), val); }
DLLCLBK bool ModMsgPut_MATRIX3_v1(                   const char* mod, const char* var, const MATRIX3& val,   const OBJHANDLE ohv) { return gCore.Put(string(mod), _Id(mod, var, ohv), val); }
DLLCLBK bool ModMsgPut_MATRIX4_v1(                   const char* mod, const char* var, const MATRIX4& val,   const OBJHANDLE ohv) { return gCore.Put(string(mod), _Id(mod, var, ohv), val); }
DLLCLBK bool ModMsgPut_OBJHANDLE_v1(                 const char* mod, const char* var, const OBJHANDLE& val, const OBJHANDLE ohv) { return gCore.Put(string(mod), _Id(mod, var, ohv), val); }
DLLCLBK bool ModMsgPut_MMStruct_v1(                  const char* mod, const char* var, const MMStruct* val,  const OBJHANDLE ohv) { return gCore.Put(string(mod), _Id(mod, var, ohv), val); }
DLLCLBK bool ModMsgPut_MMBase_v1(                    const char* mod, const char* var, const EnjoLib::ModuleMessagingExtBase* val, const OBJHANDLE ohv)
                                                                                                                                  { return gCore.Put(string(mod), _Id(mod, var, ohv), val); }
DLLCLBK bool ModMsgDel_any_v1(                       const char* mod, const char* var, const OBJHANDLE ohv)                       { return gCore.Delete(mod, _Id(mod, var, ohv)); }
DLLCLBK bool ModMsgGet_int_v1(      const char* cli, const char* mod, const char* var, int* val,             const OBJHANDLE ohv) { return gCore.Get(string(cli), _Id(mod, var, ohv), val); }
DLLCLBK bool ModMsgGet_bool_v1(     const char* cli, const char* mod, const char* var, bool* val,            const OBJHANDLE ohv) { return gCore.Get(string(cli), _Id(mod, var, ohv), val); }
DLLCLBK bool ModMsgGet_double_v1(   const char* cli, const char* mod, const char* var, double* val,          const OBJHANDLE ohv) { return gCore.Get(string(cli), _Id(mod, var, ohv), val); }
DLLCLBK bool ModMsgGet_VECTOR3_v1(  const char* cli, const char* mod, const char* var, VECTOR3* val,         const OBJHANDLE ohv) { return gCore.Get(string(cli), _Id(mod, var, ohv), val); }
DLLCLBK bool ModMsgGet_MATRIX3_v1(  const char* cli, const char* mod, const char* var, MATRIX3* val,         const OBJHANDLE ohv) { return gCore.Get(string(cli), _Id(mod, var, ohv), val); }
DLLCLBK bool ModMsgGet_MATRIX4_v1(  const char* cli, const char* mod, const char* var, MATRIX4* val,         const OBJHANDLE ohv) { return gCore.Get(string(cli), _Id(mod, var, ohv), val); }
DLLCLBK bool ModMsgGet_OBJHANDLE_v1(const char* cli, const char* mod, const char* var, OBJHANDLE* val,       const OBJHANDLE ohv) { return gCore.Get(string(cli), _Id(mod, var, ohv), val); }
DLLCLBK bool ModMsgGet_MMStruct_v1( const char* cli, const char* mod, const char* var, const MMStruct** val, const OBJHANDLE ohv) { return gCore.Get(string(cli), _Id(mod, var, ohv), val); }
DLLCLBK bool ModMsgGet_MMBase_v1(   const char* cli, const char* mod, const char* var, const EnjoLib::ModuleMessagingExtBase** val, const OBJHANDLE ohv)
                                                                                                                                  { return gCore.Get(string(cli), _Id(mod, var, ohv), val); }
DLLCLBK int ModMsgObj_typ_v1(const OBJHANDLE& val)                                                                                { return _ObjType(val); }


// Special handling for all string functions (do not want to expose the string implementation across compiler versions due to lack of ABI; therefore use char* as the interface)

DLLCLBK bool ModMsgPut_c_str_v1(const char* mod, const char* var, const char* val, const OBJHANDLE ohv) {
  string str = val;
  return gCore.Put(string(mod), _Id(mod, var, ohv), str);
}

DLLCLBK bool ModMsgGet_c_str_v1(const char* cli, const char* mod, const char* var, char *val, size_t *lVal, const OBJHANDLE ohv) {
  string rVal;
  if (!gCore.Get(string(cli), _Id(mod, var, ohv), &rVal)) return false;
  _RemoteCopy(val, lVal, rVal);
  return true;
}


DLLCLBK bool ModMsgFind_v1(char *rTyp, char *rMod, size_t *lMod, char *rVar, size_t *lVar, OBJHANDLE* rOhv, int* ix,
                           const char* cli, const char* mod, const char* var, const OBJHANDLE ohv, const bool skpSelf) {
  string iCli = cli, iMod = mod, iVar = var, irMod, irVar;
  if (!gCore.Find(rTyp, &irMod, &irVar, rOhv, ix, iCli, iMod, iVar, ohv, skpSelf)) return false;
  _RemoteCopy(rMod, lMod, irMod);
  _RemoteCopy(rVar, lVar, irVar);
  return true;
}

DLLCLBK bool ModMsgGet_log_v1(char* rFunc, 
                              char* rCli, size_t* lCli,
                              char* rMod, size_t* lMod,
                              char* rVar, size_t* lVar,
                              char* rVes, size_t* lVes,
                              bool* rSucc, int *ix,
                              const char* get, const bool skp ) {
  string iCli, iMod, iVar, iVes;
  if (!gCore.GetLog(rFunc, &iCli, &iMod, &iVar, &iVes, rSucc, ix, get, skp)) return false;

  _RemoteCopy(rCli, lCli, iCli);
  _RemoteCopy(rMod, lMod, iMod);
  _RemoteCopy(rVar, lVar, iVar);
  _RemoteCopy(rVes, lVes, iVes);
  return true;
}

DLLCLBK bool ModMsgRst_log_v1() { return gCore.ResetLog(); }

