// ================================================================================
//         ORBITER AUX LIBRARY: Module Messaging Extended v2
//                              DLL Interface Internal Header
//
// Copyright  (C) 2014-2018 Szymon "Enjo" Ender and Andrew "ADSWNJ" Stokes
//                         All rights reserved
//
// See MMExt2_Basic.hpp or MMExt2_Advanced.hpp  for license and usage information. 
// This is an internal implementation file. Do not include this directly
// in your code - i.e. just include the MMExt2_Basic.hpp or MMExt2_Advanced.hpp 
// ================================================================================
#pragma once
#ifndef MMExt2_Internal_H
#define MMExt2_Internal_H
#include "windows.h"
#include "orbitersdk.h"
#include <string>
#include <exception>
#include "__MMExt2_MMStruct.hpp"
#include "EnjoLib\ModuleMessagingExtBase.hpp"

using namespace std;
namespace MMExt2
{
  // Function prototypes for the DLL Interface
  typedef bool(*FUNC_MMEXT2_PUT_INT) (                 const char* mod, const char* var, const int& val,           const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_PUT_BOO) (                 const char* mod, const char* var, const bool& val,          const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_PUT_DBL) (                 const char* mod, const char* var, const double& val,        const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_PUT_VEC) (                 const char* mod, const char* var, const VECTOR3& val,       const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_PUT_MX3) (                 const char* mod, const char* var, const MATRIX3& val,       const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_PUT_MX4) (                 const char* mod, const char* var, const MATRIX4& val,       const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_PUT_OBJ) (                 const char* mod, const char* var, const OBJHANDLE& val,     const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_PUT_CST) (                 const char* mod, const char* var, const char *val,          const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_DEL_ANY) (                 const char* mod, const char* var,                           const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_GET_INT) (const char* cli, const char* mod, const char* var, int* val,                 const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_GET_BOO) (const char* cli, const char* mod, const char* var, bool* val,                const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_GET_DBL) (const char* cli, const char* mod, const char* var, double* val,              const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_GET_VEC) (const char* cli, const char* mod, const char* var, VECTOR3* val,             const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_GET_MX3) (const char* cli, const char* mod, const char* var, MATRIX3* val,             const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_GET_MX4) (const char* cli, const char* mod, const char* var, MATRIX4* val,             const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_GET_OBJ) (const char* cli, const char* mod, const char* var, OBJHANDLE* val,           const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_GET_CST) (const char* cli, const char* mod, const char* var, char* val, size_t *len,   const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_GET_VER) (const char* mod, char* var, size_t* len);
  typedef bool(*FUNC_MMEXT2_GET_LOG) (char* rFunc, char *rCli, size_t *lCli, char *rMod, size_t *lMod, char *rVar, size_t *lVar, char *rVes, size_t *lVes, bool* rSucc, int* ix,
                                      const char* cli, const bool skpSelf);
  typedef bool(*FUNC_MMEXT2_FIND)    (char *rTyp, char *rMod, size_t *lMod, char *rVar, size_t *lVar, OBJHANDLE* rOhv, int* ix,
                                      const char* cli, const char* mod, const char* var, const OBJHANDLE ohv, const bool skpSelf);
  typedef bool(*FUNC_MMEXT2_PUT_MMB) (const char* mod, const char* var, const EnjoLib::ModuleMessagingExtBase* val, const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_PUT_MMS) (const char* mod, const char* var, const MMStruct* val, const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_GET_MMB) (const char* cli, const char* mod, const char* var, const EnjoLib::ModuleMessagingExtBase** val, const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_GET_MMS) (const char* cli, const char* mod, const char* var, const MMStruct** val, const OBJHANDLE ohv);
  typedef bool(*FUNC_MMEXT2_RST_LOG) ();

  class Internal {
  public:
    Internal(const char *mod);
    ~Internal();
    bool IsInit() const {return m_initialized;};
    bool _Put( const char* var, const int &val,                          const OBJHANDLE ohv = NULL) const   { return ((m_fPI) && ((*m_fPI)(m_mod, var, val,         _GetOhv(ohv)))); }
    bool _Put( const char* var, const bool &val,                         const OBJHANDLE ohv = NULL) const   { return ((m_fPB) && ((*m_fPB)(m_mod, var, val,         _GetOhv(ohv)))); }
    bool _Put( const char* var, const double &val,                       const OBJHANDLE ohv = NULL) const   { return ((m_fPD) && ((*m_fPD)(m_mod, var, val,         _GetOhv(ohv)))); }
    bool _Put( const char* var, const VECTOR3 &val,                      const OBJHANDLE ohv = NULL) const   { return ((m_fPV) && ((*m_fPV)(m_mod, var, val,         _GetOhv(ohv)))); }
    bool _Put( const char* var, const MATRIX3 &val,                      const OBJHANDLE ohv = NULL) const   { return ((m_fP3) && ((*m_fP3)(m_mod, var, val,         _GetOhv(ohv)))); }
    bool _Put( const char* var, const MATRIX4 &val,                      const OBJHANDLE ohv = NULL) const   { return ((m_fP4) && ((*m_fP4)(m_mod, var, val,         _GetOhv(ohv)))); }
    bool _Put( const char* var, const OBJHANDLE &val,                    const OBJHANDLE ohv = NULL) const   { return ((m_fPO) && ((*m_fPO)(m_mod, var, val,         _GetOhv(ohv)))); }
    bool _Put( const char* var, const string &val,                       const OBJHANDLE ohv = NULL) const   { return ((m_fPS) && ((*m_fPS)(m_mod, var, val.c_str(), _GetOhv(ohv)))); }
    bool _Del( const char* var,                                          const OBJHANDLE ohv = NULL) const   { return ((m_fDA) && ((*m_fDA)(m_mod, var,              _GetOhv(ohv)))); }
    bool _Get( const char* mod, const char* var, int* val,               const OBJHANDLE ohv = NULL) const   { return ((m_fGI) && ((*m_fGI)(m_mod, mod, var, val,    _GetOhv(ohv)))); }
    bool _Get( const char* mod, const char* var, bool* val,              const OBJHANDLE ohv = NULL) const   { return ((m_fGB) && ((*m_fGB)(m_mod, mod, var, val,    _GetOhv(ohv)))); }
    bool _Get( const char* mod, const char* var, double* val,            const OBJHANDLE ohv = NULL) const   { return ((m_fGD) && ((*m_fGD)(m_mod, mod, var, val,    _GetOhv(ohv)))); }
    bool _Get( const char* mod, const char* var, VECTOR3* val,           const OBJHANDLE ohv = NULL) const   { return ((m_fGV) && ((*m_fGV)(m_mod, mod, var, val,    _GetOhv(ohv)))); }
    bool _Get( const char* mod, const char* var, MATRIX3* val,           const OBJHANDLE ohv = NULL) const   { return ((m_fG3) && ((*m_fG3)(m_mod, mod, var, val,    _GetOhv(ohv)))); }
    bool _Get( const char* mod, const char* var, MATRIX4* val,           const OBJHANDLE ohv = NULL) const   { return ((m_fG4) && ((*m_fG4)(m_mod, mod, var, val,    _GetOhv(ohv)))); }
    bool _Get( const char* mod, const char* var, OBJHANDLE* val,         const OBJHANDLE ohv = NULL) const   { return ((m_fGO) && ((*m_fGO)(m_mod, mod, var, val,    _GetOhv(ohv)))); }
    bool _Get( const char* mod, const char* var, string* val,            const OBJHANDLE ohv = NULL) const;
    bool _GetVer(string* ver) const;
    bool _GetLog(char *rfunc, string *rcli, string *rmod, string *rvar, string *rves, bool *rsucc, int *ix, const bool skipSelf);
    bool _RstLog() { return ((m_fRL) && (*m_fRL)()); }
    bool _Find(char *rTyp, string* rMod, string* rVar, OBJHANDLE* rOhv, int *ix, const char* mod, const char* var, const OBJHANDLE ohv, const bool skipSelf);
    void _UpdMod(const char *mod);
    bool _Put(const char* var, const EnjoLib::ModuleMessagingExtBase* val, const OBJHANDLE ohv = NULL) const { return ((m_fPY) && ((*m_fPY)(m_mod, var, val, _GetOhv(ohv)))); }
    bool _Put(const char* var, const MMStruct* val, const OBJHANDLE ohv = NULL) const { return ((m_fPX) && ((*m_fPX)(m_mod, var, val, _GetOhv(ohv)))); }
    bool _Get(const char* mod, const char* var, const EnjoLib::ModuleMessagingExtBase** val, const OBJHANDLE ohv = NULL) const { return ((m_fGY) && ((*m_fGY)(m_mod, mod, var, val, _GetOhv(ohv)))); }
    bool _Get(const char* mod, const char* var, const MMStruct** val, const OBJHANDLE ohv = NULL) const { return ((m_fGX) && ((*m_fGX)(m_mod, mod, var, val, _GetOhv(ohv)))); }
  private:
    FUNC_MMEXT2_PUT_INT m_fPI;
    FUNC_MMEXT2_PUT_BOO m_fPB;
    FUNC_MMEXT2_PUT_DBL m_fPD;
    FUNC_MMEXT2_PUT_VEC m_fPV;
    FUNC_MMEXT2_PUT_MX3 m_fP3;
    FUNC_MMEXT2_PUT_MX4 m_fP4;
    FUNC_MMEXT2_PUT_OBJ m_fPO;
    FUNC_MMEXT2_PUT_CST m_fPS;
    FUNC_MMEXT2_GET_INT m_fGI;
    FUNC_MMEXT2_GET_BOO m_fGB;
    FUNC_MMEXT2_GET_DBL m_fGD;
    FUNC_MMEXT2_GET_VEC m_fGV;
    FUNC_MMEXT2_GET_MX3 m_fG3;
    FUNC_MMEXT2_GET_MX4 m_fG4;
    FUNC_MMEXT2_GET_OBJ m_fGO;
    FUNC_MMEXT2_GET_CST m_fGS;
    FUNC_MMEXT2_DEL_ANY m_fDA;
    FUNC_MMEXT2_GET_VER m_fVR; 
    FUNC_MMEXT2_GET_LOG m_fGL;
    FUNC_MMEXT2_FIND    m_fFA;
    FUNC_MMEXT2_PUT_MMB m_fPY;
    FUNC_MMEXT2_PUT_MMS m_fPX;
    FUNC_MMEXT2_GET_MMB m_fGY;
    FUNC_MMEXT2_GET_MMS m_fGX;
    FUNC_MMEXT2_RST_LOG m_fRL;
    bool m_initialized;
    HMODULE m_hDLL;
    char* m_mod;
    const OBJHANDLE _GetOhv(const OBJHANDLE v) const;
  };
  // End of class definition

  // Inline implementation definition
  inline bool Internal::_Get(const char* mod, const char* var, std::string* val, const OBJHANDLE ohv) const {
    *val = "";
    if (!m_fGS) return false;
    const size_t mxln = 64;
    size_t csl = mxln;
    char buf[mxln];
    if (!(*m_fGS)(m_mod, mod, var, buf, &csl, _GetOhv(ohv))) return false;
    if (csl <= mxln) {
      *val = buf;
      return true;
    }
    // long string support
    char *p1 = static_cast<char *>(malloc(csl));
    if (!(*m_fGS)(m_mod, mod, var, p1, &csl, _GetOhv(ohv))) return false;
    *val = p1;
    free(p1);
    return true;
  };


  inline bool Internal::_GetVer(std::string* ver) const {
    *ver = "";
    if (!m_fVR) return false;
    const size_t mxln = 64;
    size_t csl = mxln;
    char buf[mxln];
    if (!(*m_fVR)(m_mod, buf, &csl)) return false;
    if (csl <= mxln) {
        *ver = buf;
        return true;
    }
    // long string support
    char *p1 = static_cast<char *>(malloc(csl));
    if (!(*m_fVR)(m_mod, p1, &csl)) return false;
    *ver = p1;
    free(p1);
    return true;
  };

  inline bool Internal::_Find(char *rTyp, string* rMod, string* rVar, OBJHANDLE* rOhv, int *ix, const char* mod, const char* var, const OBJHANDLE ohv, const bool skipSelf) {
    *rMod = "";
    *rVar = "";
    *rOhv = NULL;
    if (!m_fFA) return false;
    const size_t mxln = 64;
    size_t lmod = mxln, lvar = mxln;
    bool needBig;
    char bmod[mxln], bvar[mxln];
    char *pmod, *pvar;

    needBig = false;
    pmod = bmod;
    pvar = bvar;

    if (!(*m_fFA)(rTyp, pmod, &lmod, pvar, &lvar, rOhv, ix,  m_mod, mod, var, ohv, skipSelf)) return false;
    if (lmod > mxln || lvar > mxln) {
      needBig = true;
      pmod = static_cast<char *>(malloc(lmod));
      pvar = static_cast<char *>(malloc(lvar));
      if (!(*m_fFA)(rTyp, pmod, &lmod, pvar, &lvar, rOhv, ix, m_mod, mod, var, ohv, skipSelf)) {
        free(pmod);
        free(pvar);
        return false;
      };
    }
    *rMod = pmod;
    *rVar = pvar;
    if (needBig) {
      free(pmod);
      free(pvar);
    }
    (*ix)++;
    return true;
  }


  inline bool Internal::_GetLog(char *rfunc, string *rcli, string *rmod, string *rvar, string *rves, bool *rsucc, int *ix, const bool skipSelf) {
    if (!m_fGL) return false;
    bool needBig = false;
    const size_t mxln = 64;
    size_t lcli = mxln, lmod = mxln, lvar = mxln, lves = mxln;
    char cli[mxln], mod[mxln], var[mxln], ves[mxln];
    char *pcli = cli, *pmod = mod, *pvar = var, *pves = ves;
    if (!(*m_fGL)(rfunc, pcli, &lcli, pmod, &lmod, pvar, &lvar, pves, &lves, rsucc, ix, m_mod, skipSelf)) return false;
    if (lcli > mxln || lmod > mxln || lvar > mxln || lves > mxln) {
        needBig = true;
        pcli = static_cast<char *>(malloc(lcli));
        pmod = static_cast<char *>(malloc(lmod));
        pvar = static_cast<char *>(malloc(lvar));
        pves = static_cast<char *>(malloc(lves));
        if (!(*m_fGL)(rfunc, pcli, &lcli, pmod, &lmod, pvar, &lvar, pves, &lves, rsucc, ix, m_mod, skipSelf)) {
            free(pcli); free(pmod); free(pvar); free(pves);
            return false;
        }
    }
    *rcli = pcli;
    *rmod = pmod;
    *rvar = pvar;
    *rves = pves;
    if (needBig) {
        free(pcli); free(pmod); free(pvar); free(pves);
    }
    return true;
  }

  inline const OBJHANDLE Internal::_GetOhv(const OBJHANDLE ohv) const {
      if (ohv) return ohv;
      return oapiGetFocusInterface()->GetHandle();
  }

  inline void Internal::_UpdMod(const char *mod) {
      if (m_mod) {
          if (!strcmp(m_mod, mod)) return;
          free(m_mod);
      }
      m_mod = _strdup(mod);
  }

  inline Internal::Internal(const char *mod) {
    if (m_initialized) return;
    m_mod = _strdup(mod);
    if (!(m_hDLL = LoadLibraryA(".\\Modules\\MMExt2.dll"))) return;
    m_fPI = (FUNC_MMEXT2_PUT_INT)GetProcAddress(m_hDLL, "ModMsgPut_int_v1");
    m_fPB = (FUNC_MMEXT2_PUT_BOO)GetProcAddress(m_hDLL, "ModMsgPut_bool_v1");
    m_fPD = (FUNC_MMEXT2_PUT_DBL)GetProcAddress(m_hDLL, "ModMsgPut_double_v1");
    m_fPV = (FUNC_MMEXT2_PUT_VEC)GetProcAddress(m_hDLL, "ModMsgPut_VECTOR3_v1");
    m_fP3 = (FUNC_MMEXT2_PUT_MX3)GetProcAddress(m_hDLL, "ModMsgPut_MATRIX3_v1");
    m_fP4 = (FUNC_MMEXT2_PUT_MX4)GetProcAddress(m_hDLL, "ModMsgPut_MATRIX4_v1");
    m_fPO = (FUNC_MMEXT2_PUT_OBJ)GetProcAddress(m_hDLL, "ModMsgPut_OBJHANDLE_v1");
    m_fPS = (FUNC_MMEXT2_PUT_CST)GetProcAddress(m_hDLL, "ModMsgPut_c_str_v1");
    m_fGI = (FUNC_MMEXT2_GET_INT)GetProcAddress(m_hDLL, "ModMsgGet_int_v1");
    m_fGB = (FUNC_MMEXT2_GET_BOO)GetProcAddress(m_hDLL, "ModMsgGet_bool_v1");
    m_fGD = (FUNC_MMEXT2_GET_DBL)GetProcAddress(m_hDLL, "ModMsgGet_double_v1");
    m_fGV = (FUNC_MMEXT2_GET_VEC)GetProcAddress(m_hDLL, "ModMsgGet_VECTOR3_v1");
    m_fG3 = (FUNC_MMEXT2_GET_MX3)GetProcAddress(m_hDLL, "ModMsgGet_MATRIX3_v1");
    m_fG4 = (FUNC_MMEXT2_GET_MX4)GetProcAddress(m_hDLL, "ModMsgGet_MATRIX4_v1");
    m_fGO = (FUNC_MMEXT2_GET_OBJ)GetProcAddress(m_hDLL, "ModMsgGet_OBJHANDLE_v1");
    m_fGS = (FUNC_MMEXT2_GET_CST)GetProcAddress(m_hDLL, "ModMsgGet_c_str_v1");
    m_fDA = (FUNC_MMEXT2_DEL_ANY)GetProcAddress(m_hDLL, "ModMsgDel_any_v1");
    m_fVR = (FUNC_MMEXT2_GET_VER)GetProcAddress(m_hDLL, "ModMsgGet_ver_v1");
    m_fGL = (FUNC_MMEXT2_GET_LOG)GetProcAddress(m_hDLL, "ModMsgGet_log_v1");
    m_fFA = (FUNC_MMEXT2_FIND)   GetProcAddress(m_hDLL, "ModMsgFind_v1");
    m_fPY = (FUNC_MMEXT2_PUT_MMB)GetProcAddress(m_hDLL, "ModMsgPut_MMBase_v1");
    m_fPX = (FUNC_MMEXT2_PUT_MMS)GetProcAddress(m_hDLL, "ModMsgPut_MMStruct_v1");
    m_fGY = (FUNC_MMEXT2_GET_MMB)GetProcAddress(m_hDLL, "ModMsgGet_MMBase_v1");
    m_fGX = (FUNC_MMEXT2_GET_MMS)GetProcAddress(m_hDLL, "ModMsgGet_MMStruct_v1");
    m_fRL = (FUNC_MMEXT2_RST_LOG)GetProcAddress(m_hDLL, "ModMsgRst_log_v1");
    m_initialized = true;
  };

  inline Internal::~Internal() {
    if (m_hDLL) FreeLibrary(m_hDLL);
    if (m_mod) free(m_mod);
  };
  // End of inline implementation definition

}
#endif // MMExt2_Internal_H
