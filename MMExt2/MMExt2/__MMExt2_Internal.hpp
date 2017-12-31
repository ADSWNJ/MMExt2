// ================================================================================
//         ORBITER AUX LIBRARY: Module Messaging Extended v2
//                              DLL Interface Internal Header
//
// Copyright  (C) 2014-2017 Szymon "Enjo" Ender and Andrew "ADSWNJ" Stokes
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
  typedef bool(*FUNC_MMEXT2_PUT_INT) (const char* mod,                   const char* var, const int& val,           const char* ves);
  typedef bool(*FUNC_MMEXT2_PUT_BOO) (const char* mod,                   const char* var, const bool& val,          const char* ves);
  typedef bool(*FUNC_MMEXT2_PUT_DBL) (const char* mod,                   const char* var, const double& val,        const char* ves);
  typedef bool(*FUNC_MMEXT2_PUT_VEC) (const char* mod,                   const char* var, const VECTOR3& val,       const char* ves);
  typedef bool(*FUNC_MMEXT2_PUT_MX3) (const char* mod,                   const char* var, const MATRIX3& val,       const char* ves);
  typedef bool(*FUNC_MMEXT2_PUT_MX4) (const char* mod,                   const char* var, const MATRIX4& val,       const char* ves);
  typedef bool(*FUNC_MMEXT2_PUT_CST) (const char* mod,                   const char* var, const char *val,          const char* ves);
  typedef bool(*FUNC_MMEXT2_DEL_ANY) (const char* mod,  const char* var,                                            const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_INT) (const char* cli,  const char* mod, const char* var, int* val,                 const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_BOO) (const char* cli,  const char* mod, const char* var, bool* val,                const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_DBL) (const char* cli,  const char* mod, const char* var, double* val,              const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_VEC) (const char* cli,  const char* mod, const char* var, VECTOR3* val,             const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_MX3) (const char* cli,  const char* mod, const char* var, MATRIX3* val,             const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_MX4) (const char* cli,  const char* mod, const char* var, MATRIX4* val,             const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_CST) (const char* cli,  const char* mod, const char* var, char* val, size_t *len,   const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_VER) (const char* mod, char* var, size_t* len);
  typedef bool(*FUNC_MMEXT2_GET_LOG) (const char* cli, const int ix, char* rfunc, bool* rsucc,
                                      char *rcli, size_t *lcli, char *rmod, size_t *lmod, char *rvar, size_t *lvar, char *rves, size_t *lves);
  typedef bool(*FUNC_MMEXT2_FIND)    (const char* cli, const char* fmod, const char* fvar, const char* fves, bool skp,
                                      int* ix, char *typ, char *mod, size_t *lmod, char *var, size_t *lvar, char *ves, size_t *lves);

  typedef bool(*FUNC_MMEXT2_PUT_MMB) (const char* mod, const char* var, const EnjoLib::ModuleMessagingExtBase* val, const char* ves);
  typedef bool(*FUNC_MMEXT2_PUT_MMS) (const char* mod, const char* var, const MMStruct* val, const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_MMB) (const char* cli, const char* mod, const char* var, const EnjoLib::ModuleMessagingExtBase** val, const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_MMS) (const char* cli, const char* mod, const char* var, const MMStruct** val, const char* ves);

  class Internal {
  public:
    Internal(const char *mod);
    ~Internal();
    bool IsInit() const {return m_initialized;};
    bool _Put( const char* var, const int &val,                          const VESSEL* ves = NULL) const   {return ((m_fPI) && ((*m_fPI)(m_mod, var, val,         _GetVes(ves))));}
    bool _Put( const char* var, const bool &val,                         const VESSEL* ves = NULL) const   {return ((m_fPB) && ((*m_fPB)(m_mod, var, val,         _GetVes(ves))));}
    bool _Put( const char* var, const double &val,                       const VESSEL* ves = NULL) const   {return ((m_fPD) && ((*m_fPD)(m_mod, var, val,         _GetVes(ves))));}
    bool _Put( const char* var, const VECTOR3 &val,                      const VESSEL* ves = NULL) const   {return ((m_fPV) && ((*m_fPV)(m_mod, var, val,         _GetVes(ves))));}
    bool _Put( const char* var, const MATRIX3 &val,                      const VESSEL* ves = NULL) const   {return ((m_fP3) && ((*m_fP3)(m_mod, var, val,         _GetVes(ves))));}
    bool _Put( const char* var, const MATRIX4 &val,                      const VESSEL* ves = NULL) const   {return ((m_fP4) && ((*m_fP4)(m_mod, var, val,         _GetVes(ves))));}
    bool _Put( const char* var, const string &val,                       const VESSEL* ves = NULL) const   {return ((m_fPS) && ((*m_fPS)(m_mod, var, val.c_str(), _GetVes(ves))));}
    bool _Del( const char* var,                                          const VESSEL* ves = NULL) const   {return ((m_fDA) && ((*m_fDA)(m_mod, var,              _GetVes(ves))));}
    bool _Get( const char* mod, const char* var, int* val,               const VESSEL* ves = NULL) const   {return ((m_fGI) && ((*m_fGI)(m_mod, mod, var, val,    _GetVes(ves))));}
    bool _Get( const char* mod, const char* var, bool* val,              const VESSEL* ves = NULL) const   {return ((m_fGB) && ((*m_fGB)(m_mod, mod, var, val,    _GetVes(ves))));}
    bool _Get( const char* mod, const char* var, double* val,            const VESSEL* ves = NULL) const   {return ((m_fGD) && ((*m_fGD)(m_mod, mod, var, val,    _GetVes(ves))));}
    bool _Get( const char* mod, const char* var, VECTOR3* val,           const VESSEL* ves = NULL) const   {return ((m_fGV) && ((*m_fGV)(m_mod, mod, var, val,    _GetVes(ves))));}
    bool _Get( const char* mod, const char* var, MATRIX3* val,           const VESSEL* ves = NULL) const   {return ((m_fG3) && ((*m_fG3)(m_mod, mod, var, val,    _GetVes(ves))));}
    bool _Get( const char* mod, const char* var, MATRIX4* val,           const VESSEL* ves = NULL) const   {return ((m_fG4) && ((*m_fG4)(m_mod, mod, var, val,    _GetVes(ves))));}
    bool _Get( const char* mod, const char* var, string* val,            const VESSEL* ves = NULL) const;
    bool _GetVer(string* ver) const;
    bool _GetLog(const int ix, char *rfunc, bool *rsucc, string *rcli, string *rmod, string *rvar, string *rves);
    bool _Find(const char* fMod, const char* fVar, int *ix, char *typ, string* mod, string* var, VESSEL** ves, bool skipSelf = true, const VESSEL* fVes = NULL);
    void _UpdMod(const char *mod);
    bool _Put(const char* var, const EnjoLib::ModuleMessagingExtBase* val, const VESSEL* ves = NULL) const { return ((m_fPY) && ((*m_fPY)(m_mod, var, val, _GetVes(ves)))); }
    bool _Put(const char* var, const MMStruct* val, const VESSEL* ves = NULL) const { return ((m_fPX) && ((*m_fPX)(m_mod, var, val, _GetVes(ves)))); }
    bool _Get(const char* mod, const char* var, const EnjoLib::ModuleMessagingExtBase** val, const VESSEL* ves = NULL) const { return ((m_fGY) && ((*m_fGY)(m_mod, mod, var, val, _GetVes(ves)))); }
    bool _Get(const char* mod, const char* var, const MMStruct** val, const VESSEL* ves = NULL) const { return ((m_fGX) && ((*m_fGX)(m_mod, mod, var, val, _GetVes(ves)))); }
  private:
    FUNC_MMEXT2_PUT_INT m_fPI;
    FUNC_MMEXT2_PUT_BOO m_fPB;
    FUNC_MMEXT2_PUT_DBL m_fPD;
    FUNC_MMEXT2_PUT_VEC m_fPV;
    FUNC_MMEXT2_PUT_MX3 m_fP3;
    FUNC_MMEXT2_PUT_MX4 m_fP4;
    FUNC_MMEXT2_PUT_CST m_fPS;
    FUNC_MMEXT2_GET_INT m_fGI;
    FUNC_MMEXT2_GET_BOO m_fGB;
    FUNC_MMEXT2_GET_DBL m_fGD;
    FUNC_MMEXT2_GET_VEC m_fGV;
    FUNC_MMEXT2_GET_MX3 m_fG3;
    FUNC_MMEXT2_GET_MX4 m_fG4;
    FUNC_MMEXT2_GET_CST m_fGS;
    FUNC_MMEXT2_DEL_ANY m_fDA;
    FUNC_MMEXT2_GET_VER m_fVR; 
    FUNC_MMEXT2_GET_LOG m_fGL;
    FUNC_MMEXT2_FIND    m_fFA;
    FUNC_MMEXT2_PUT_MMB m_fPY;
    FUNC_MMEXT2_PUT_MMS m_fPX;
    FUNC_MMEXT2_GET_MMB m_fGY;
    FUNC_MMEXT2_GET_MMS m_fGX;
    bool m_initialized;
    HMODULE m_hDLL;
    mutable char* m_vn;
    char* m_mod;
    const char*_GetVes(const VESSEL* v) const;
  };
  // End of class definition

  // Inline implementation definition
  inline bool Internal::_Get(const char* mod, const char* var, std::string* val, const VESSEL* ves) const {
    *val = "";
    if (!m_fGS) return false;
    const size_t mxln = 64;
    size_t csl = mxln;
    char buf[mxln];
    if (!(*m_fGS)(m_mod, mod, var, buf, &csl, (ves ? ves->GetName() : m_vn))) return false;
    if (csl <= mxln) {
      *val = buf;
      return true;
    }
    // long string support
    char *p1 = static_cast<char *>(malloc(csl));
    if (!(*m_fGS)(m_mod, mod, var, p1, &csl, (ves ? ves->GetName() : m_vn))) return false;
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

  inline bool Internal::_Find(const char* fMod, const char* fVar, int *ix, char *typ, string* mod, string* var, VESSEL** ves, bool skipSelf, const VESSEL* fVes) {
    *mod = "";
    *var = "";
    *ves = NULL;
    if (!m_fFA) return false;
    const size_t mxln = 64;
    size_t lmod = mxln, lvar = mxln, lves = mxln;
    bool needBig;
    bool repeat;
    char bmod[mxln], bvar[mxln], bves[mxln];
    char *pmod, *pvar, *pves;
    OBJHANDLE ov;

    do {
      needBig = false;
      pmod = bmod; pvar = bvar; pves = bves;

      if (!(*m_fFA)(m_mod, fMod, fVar, (fVes ? fVes->GetName() : "*"), skipSelf, ix, typ, pmod, &lmod, pvar, &lvar, pves, &lves)) return false;
      if (lmod > mxln || lvar > mxln || lves > mxln) {
        needBig = true;
        pmod = static_cast<char *>(malloc(lmod));
        pvar = static_cast<char *>(malloc(lvar));
        pves = static_cast<char *>(malloc(lves));
        if (!(*m_fFA)(m_mod, fMod, fVar, (fVes ? fVes->GetName() : "*"), skipSelf, ix, typ, pmod, &lmod, pvar, &lvar, pves, &lves)) {
          free(pmod); free(pvar); free(pves);
          return false;
        };
      }

      ov = oapiGetVesselByName(pves);
      if (!ov) {
        if (needBig) {
          free(pmod); free(pvar); free(pves);
        }
        (*ix)++;
        repeat = true;
      } else {
        repeat = false;
      }
    } while (repeat);

    *mod = pmod;
    *var = pvar;
    *ves = oapiGetVesselInterface(ov);
    if (needBig) {
      free(pmod); free(pvar); free(pves);
    }
    (*ix)++;
    return true;
  }

  inline bool Internal::_GetLog(const int ix, char *rfunc, bool *rsucc, string *rcli, string *rmod, string *rvar, string *rves) {
    if (!m_fGL) return false;
    bool needBig = false;
    const size_t mxln = 64;
    size_t lcli = mxln, lmod = mxln, lvar = mxln, lves = mxln;
    char cli[mxln], mod[mxln], var[mxln], ves[mxln];
    char *pcli = cli, *pmod = mod, *pvar = var, *pves = ves;
    if (!(*m_fGL)(m_mod, ix, rfunc, rsucc, pcli, &lcli, pmod, &lmod, pvar, &lvar, pves, &lves)) return false;
    if (lcli > mxln || lmod > mxln || lvar > mxln || lves > mxln) {
        needBig = true;
        pcli = static_cast<char *>(malloc(lcli));
        pmod = static_cast<char *>(malloc(lmod));
        pvar = static_cast<char *>(malloc(lvar));
        pves = static_cast<char *>(malloc(lves));
        if (!(*m_fGL)(m_mod, ix, rfunc, rsucc, pcli, &lcli, pmod, &lmod, pvar, &lvar, pves, &lves)) {
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

  inline const char* Internal::_GetVes(const VESSEL* v) const {
      if (v) return v->GetName();
      if (m_vn) return m_vn;
      VESSEL* this_v = oapiGetFocusInterface();
      m_vn = _strdup(this_v->GetName());
      return m_vn;
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
    m_fPS = (FUNC_MMEXT2_PUT_CST)GetProcAddress(m_hDLL, "ModMsgPut_c_str_v1");
    m_fGI = (FUNC_MMEXT2_GET_INT)GetProcAddress(m_hDLL, "ModMsgGet_int_v1");
    m_fGB = (FUNC_MMEXT2_GET_BOO)GetProcAddress(m_hDLL, "ModMsgGet_bool_v1");
    m_fGD = (FUNC_MMEXT2_GET_DBL)GetProcAddress(m_hDLL, "ModMsgGet_double_v1");
    m_fGV = (FUNC_MMEXT2_GET_VEC)GetProcAddress(m_hDLL, "ModMsgGet_VECTOR3_v1");
    m_fG3 = (FUNC_MMEXT2_GET_MX3)GetProcAddress(m_hDLL, "ModMsgGet_MATRIX3_v1");
    m_fG4 = (FUNC_MMEXT2_GET_MX4)GetProcAddress(m_hDLL, "ModMsgGet_MATRIX4_v1");
    m_fGS = (FUNC_MMEXT2_GET_CST)GetProcAddress(m_hDLL, "ModMsgGet_c_str_v1");
    m_fDA = (FUNC_MMEXT2_DEL_ANY)GetProcAddress(m_hDLL, "ModMsgDel_any_v1");
    m_fVR = (FUNC_MMEXT2_GET_VER)GetProcAddress(m_hDLL, "ModMsgGet_ver_v1");
    m_fGL = (FUNC_MMEXT2_GET_LOG)GetProcAddress(m_hDLL, "ModMsgGet_log_v1");
    m_fFA = (FUNC_MMEXT2_FIND)   GetProcAddress(m_hDLL, "ModMsgFind_v1");
    m_fPY = (FUNC_MMEXT2_PUT_MMB)GetProcAddress(m_hDLL, "ModMsgPut_MMBase_v1");
    m_fPX = (FUNC_MMEXT2_PUT_MMS)GetProcAddress(m_hDLL, "ModMsgPut_MMStruct_v1");
    m_fGY = (FUNC_MMEXT2_GET_MMB)GetProcAddress(m_hDLL, "ModMsgGet_MMBase_v1");
    m_fGX = (FUNC_MMEXT2_GET_MMS)GetProcAddress(m_hDLL, "ModMsgGet_MMStruct_v1");
    m_initialized = true;
  };

  inline Internal::~Internal() {
    if (m_hDLL) FreeLibrary(m_hDLL);
    if (m_mod) free(m_mod);
    if (m_vn) free(m_vn);
  };
  // End of inline implementation definition

}
#endif // MMExt2_Internal_H
