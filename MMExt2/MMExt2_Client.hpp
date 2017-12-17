// ====================================================================
//         ORBITER AUX LIBRARY: MMExt2 v2 (MMExt2)
//
// MMExt 2 allows Orbiter modules to communicate with each other,
// using predefined module and variable names, with no static binding or
// dependencies. If the MMExt2 module is not installed, all clients
// putting and getting variables will work standalone with no issues. 
//
// Copyright  (C) 2014-2017 Szymon "Enjo" Ender and Andrew "ADSWNJ" Stokes
//
//                         All rights reserved
//
// MMExt2 v2 is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// MMExt2 v2 is distributed in the hope that it will
// be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with MMExt2. If not, see
// <http://www.gnu.org/licenses/>.
// ====================================================================

#ifndef MMExt2_Client_H
#define MMExt2_Client_H
#include "windows.h"
#include "orbitersdk.h"
#include <string>

namespace MMExt2
{
  /*
  Purpose:

  Gets data from other MFD's using the common MMExt2 library. Data can be passed by val
  or by reference. For pass by reference, the MMExt2 library implements several further
  checks to ensure safety and to deal with potential version or definition mismatches.

  Developer Instructions:

  1. #include "EnjoLib/MMExt2.hpp" where you need to get data from MMExt2.

  2. For get by val ... call EnjoLib::MMExt2().Get(MFDname, var, &val, {vessel}),
  where:	MFDname is a string literal for the sending MFD or module (e.g. "BaseSyncMFD"),
  var is a string literal for the variable you are looking for
  &val is the address of where you want the val to be received, and is one of the following
  types: bool, int, double, VECTOR3, MATRIX3, and MATRIX4.
  vessel is VESSEL* pointer to the vessel you want to get data from (defaults to your vessel).

  The return is a bool indicating success or failure. If successful, the val will have the new data.

  3. For get by reference (meaning you are addressing memory directly in the other DLL), the author
  of the module PUTting the data will give you a header file with a data structure like this:

  #pragma pack(push)
  #pragma pack(8)
  struct XYZ : public EnjoLib::MMExt2Base {
  XYZ():EnjoLib::MMExt2Base(13,sizeof(XYZ)) {};  // version 13
  ...
  ... your data structures (don't use STL's like std::string
  ... or std::map, etc as these tend not to be byte-compatible
  ... across compiler versions)
  ...
  };
  #pragma pack(pop)

  (Obviously with their structure name instead of XYZ). Include this header into your code to give you
  the latest definition of the structure and version you are accessing. Make a note of the version number
  on the MMExt2Base constructor (i.e. 13 in this case), as you need it for your ModMsgGetByRef()
  call.

  In your code, define a pointer to a constant XYZ structure (replace XYZ, of course), such as:

  constant struct xyz *remoteData;

  This tells the compiler that you guarantee not to write data via the pointer. (If you wanted to do this,
  and the other module author wanted to cooperate, then have both sides ModMsgGetByRef the other side
  and write local / read remote.)

  Then, call EnjoLib::MMExt2().ModMsgGetByRef(MFDname, structName, structVer, &structPtr,
  {vessel}), where the structName and structVer are given to you by the author of the PUTting module,
  and the other parameters are the same as the ModMsgGet(). In addition to looking up the data and returning
  it if found, the library will do additional checks to ensure the structure has a valid
  inheritance from the ModuleMessagigngBaseStruct, the structure version number is identical, and
  the structure size is also correct. (The PUT side should increment the version any time the structure
  is changed, which will prevent you accessing a wrong verion of the structure, and the sizeof()
  check is there as a further failsafe). So if you get a "true" back from ModMsgGetByRef(), you can be
  reasonably sure that everything is safe and you will not crash the Orbiter when you reference the data.

  4. To link your code, make sure your Linker, Input, AdditionalDependencies includes MMExt2.lib.
  Have a look at http://orbiter-forum.com/showthread.php?t=34971 for advice on setting up Property Pages
  for Orbiter development (highly recommended, and it makes things much easier). If you do this, then
  you can edit the Linker Input Additional Directories in the orbiter_vs2005 proprty page and this will
  let you resolve any ModuleMessagigngExt for this and future projects.
  */
  struct MMStruct {
  public:
    MMStruct(unsigned int sVer, unsigned int sSize) : _sVer(sVer), _sSize(sSize) {};
    virtual ~MMStruct() {};
    bool IsCorrectVersion(unsigned int sVer) const { return sVer == _sVer; }
    bool IsCorrectSize(unsigned int sSize) const { return sSize == _sSize; }
  private:
    unsigned int _sVer;
    unsigned int _sSize;
  };


  typedef bool(*FUNC_MMEXT2_PUT_INT) (const char* mod, const char* var, const int& val,     const char* ves);
  typedef bool(*FUNC_MMEXT2_PUT_BOO) (const char* mod, const char* var, const bool& val,    const char* ves);
  typedef bool(*FUNC_MMEXT2_PUT_DBL) (const char* mod, const char* var, const double& val,  const char* ves);
  typedef bool(*FUNC_MMEXT2_PUT_VEC) (const char* mod, const char* var, const VECTOR3& val, const char* ves);
  typedef bool(*FUNC_MMEXT2_PUT_MX3) (const char* mod, const char* var, const MATRIX3& val, const char* ves);
  typedef bool(*FUNC_MMEXT2_PUT_MX4) (const char* mod, const char* var, const MATRIX4& val, const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_INT) (const char* mod, const char* var, int* val,           const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_BOO) (const char* mod, const char* var, bool* val,          const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_DBL) (const char* mod, const char* var, double* val,        const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_VEC) (const char* mod, const char* var, VECTOR3* val,       const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_MX3) (const char* mod, const char* var, MATRIX3* val,       const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_MX4) (const char* mod, const char* var, MATRIX4* val,       const char* ves);
  typedef bool(*FUNC_MMEXT2_DEL_ANY) (const char* mod, const char* var,                     const char* ves);

  typedef bool(*FUNC_MMEXT2_PUT_CST) (const char* mod, const char* var, const char *val, const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_CST) (const char* mod, const char* var, char **val, const size_t len, const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_CSL) (const char* mod, const char* var, size_t *val, const char* ves);

  typedef bool(*FUNC_MMEXT2_PUT_MMS) (const char* mod, const char* var, const MMStruct* val, const char* ves);
  typedef bool(*FUNC_MMEXT2_GET_MMS) (const char* mod, const char* var, const MMStruct** val, const char* ves);


  class Implementation {

  public:
    friend class Basic;
    friend class Advanced;
    ~Implementation() { __Exit(); }
    bool IsInit() const { return m_initialized; }

  private:
    bool m_initialized; 
    HMODULE m_hDLL;

    FUNC_MMEXT2_PUT_INT m_fncPut_INT;
    FUNC_MMEXT2_PUT_BOO m_fncPut_BOO;
    FUNC_MMEXT2_PUT_DBL m_fncPut_DBL;
    FUNC_MMEXT2_PUT_VEC m_fncPut_VEC;
    FUNC_MMEXT2_PUT_MX3 m_fncPut_MX3;
    FUNC_MMEXT2_PUT_MX4 m_fncPut_MX4;
    FUNC_MMEXT2_PUT_CST m_fncPut_CST;
    FUNC_MMEXT2_PUT_MMS m_fncPut_MMS;

    FUNC_MMEXT2_GET_INT m_fncGet_INT;
    FUNC_MMEXT2_GET_BOO m_fncGet_BOO;
    FUNC_MMEXT2_GET_DBL m_fncGet_DBL;
    FUNC_MMEXT2_GET_VEC m_fncGet_VEC;
    FUNC_MMEXT2_GET_MX3 m_fncGet_MX3;
    FUNC_MMEXT2_GET_MX4 m_fncGet_MX4;
    FUNC_MMEXT2_GET_CST m_fncGet_CST;
    FUNC_MMEXT2_GET_CSL m_fncGet_CSL;
    FUNC_MMEXT2_GET_MMS m_fncGet_MMS;

    FUNC_MMEXT2_DEL_ANY m_fncDel_ANY;

    bool __Put(std::string mod, const char* var, const int &val,     const VESSEL* ves) const { return ((m_fncPut_INT) && ((*m_fncPut_INT)(mod.c_str(), var, val, ves->GetName()))); }
    bool __Put(std::string mod, const char* var, const bool &val,    const VESSEL* ves) const { return ((m_fncPut_BOO) && ((*m_fncPut_BOO)(mod.c_str(), var, val, ves->GetName()))); }
    bool __Put(std::string mod, const char* var, const double &val,  const VESSEL* ves) const { return ((m_fncPut_DBL) && ((*m_fncPut_DBL)(mod.c_str(), var, val, ves->GetName()))); }
    bool __Put(std::string mod, const char* var, const VECTOR3 &val, const VESSEL* ves) const { return ((m_fncPut_VEC) && ((*m_fncPut_VEC)(mod.c_str(), var, val, ves->GetName()))); }
    bool __Put(std::string mod, const char* var, const MATRIX3 &val, const VESSEL* ves) const { return ((m_fncPut_MX3) && ((*m_fncPut_MX3)(mod.c_str(), var, val, ves->GetName()))); }
    bool __Put(std::string mod, const char* var, const MATRIX4 &val, const VESSEL* ves) const { return ((m_fncPut_MX4) && ((*m_fncPut_MX4)(mod.c_str(), var, val, ves->GetName()))); }
    bool __Put(std::string mod, const char* var, const MMStruct* val, const VESSEL* ves)   const { return ((m_fncPut_MMS) && ((*m_fncPut_MMS)(mod.c_str(), var, val, ves->GetName()))); }

    bool __Get(std::string mod, const char* var, int* val,     const VESSEL* ves)       const { return ((m_fncGet_INT) && ((*m_fncGet_INT)(mod.c_str(), var, val, ves->GetName()))); }
    bool __Get(std::string mod, const char* var, bool* val,    const VESSEL* ves)       const { return ((m_fncGet_BOO) && ((*m_fncGet_BOO)(mod.c_str(), var, val, ves->GetName()))); }
    bool __Get(std::string mod, const char* var, double* val,  const VESSEL* ves)       const { return ((m_fncGet_DBL) && ((*m_fncGet_DBL)(mod.c_str(), var, val, ves->GetName()))); }
    bool __Get(std::string mod, const char* var, VECTOR3* val, const VESSEL* ves)       const { return ((m_fncGet_VEC) && ((*m_fncGet_VEC)(mod.c_str(), var, val, ves->GetName()))); }
    bool __Get(std::string mod, const char* var, MATRIX3* val, const VESSEL* ves)       const { return ((m_fncGet_MX3) && ((*m_fncGet_MX3)(mod.c_str(), var, val, ves->GetName()))); }
    bool __Get(std::string mod, const char* var, MATRIX4* val, const VESSEL* ves)       const { return ((m_fncGet_MX4) && ((*m_fncGet_MX4)(mod.c_str(), var, val, ves->GetName()))); }
    bool __Get(std::string mod, const char* var, const MMStruct** val, const VESSEL* ves)     const { return ((m_fncGet_MMS) && ((*m_fncGet_MMS)(mod.c_str(), var, val, ves->GetName()))); }

    bool __Del(std::string mod, const char* var, const VESSEL* ves) const {      return ((m_fncDel_ANY) && ((*m_fncDel_ANY)(mod.c_str(), var, ves->GetName())));    };

    bool __Put(std::string mod, const char* var, const std::string &val, const VESSEL* ves) const { return ((m_fncPut_CST) && ((*m_fncPut_CST)(mod.c_str(), var, val.c_str(), ves->GetName()))); }
    bool __Get(std::string mod, const char* var, std::string* val, const VESSEL* ves) const {
      if (!m_fncGet_CSL || !m_fncGet_CST) return false;
      size_t csl;
      if (!(*m_fncGet_CSL)(mod.c_str(), var, &csl, ves->GetName())) return false;

      char *p1 = static_cast<char *>(malloc(csl));
      char **p2 = &p1;
      if (!(*m_fncGet_CST)(mod.c_str(), var, p2, csl, ves->GetName())) return false;
      *val = p1;
      free(p1);
      return true;
    };
    bool __Get(std::string mod, const char* var, size_t* val, const VESSEL* ves) const { return ((m_fncGet_CSL) && ((*m_fncGet_CSL)(mod.c_str(), var, val, ves->GetName()))); }


    void __Init() {
      if (m_initialized) return; 
      m_initialized = true; 
      if (!(m_hDLL = LoadLibraryA(".\\Modules\\MMExt2.dll"))) return;
      m_fncPut_INT = (FUNC_MMEXT2_PUT_INT)GetProcAddress(m_hDLL, "ModMsgPut_int_v1");
      m_fncPut_BOO = (FUNC_MMEXT2_PUT_BOO)GetProcAddress(m_hDLL, "ModMsgPut_bool_v1");
      m_fncPut_DBL = (FUNC_MMEXT2_PUT_DBL)GetProcAddress(m_hDLL, "ModMsgPut_double_v1");
      m_fncPut_VEC = (FUNC_MMEXT2_PUT_VEC)GetProcAddress(m_hDLL, "ModMsgPut_VECTOR3_v1");
      m_fncPut_MX3 = (FUNC_MMEXT2_PUT_MX3)GetProcAddress(m_hDLL, "ModMsgPut_MATRIX3_v1");
      m_fncPut_MX4 = (FUNC_MMEXT2_PUT_MX4)GetProcAddress(m_hDLL, "ModMsgPut_MATRIX4_v1");
      m_fncPut_CST = (FUNC_MMEXT2_PUT_CST)GetProcAddress(m_hDLL, "ModMsgPut_c_str_v1");
      m_fncPut_MMS = (FUNC_MMEXT2_PUT_MMS)GetProcAddress(m_hDLL, "ModMsgPut_MMStruct_v1");
      m_fncGet_INT = (FUNC_MMEXT2_GET_INT)GetProcAddress(m_hDLL, "ModMsgGet_int_v1");
      m_fncGet_BOO = (FUNC_MMEXT2_GET_BOO)GetProcAddress(m_hDLL, "ModMsgGet_bool_v1");
      m_fncGet_DBL = (FUNC_MMEXT2_GET_DBL)GetProcAddress(m_hDLL, "ModMsgGet_double_v1");
      m_fncGet_VEC = (FUNC_MMEXT2_GET_VEC)GetProcAddress(m_hDLL, "ModMsgGet_VECTOR3_v1");
      m_fncGet_MX3 = (FUNC_MMEXT2_GET_MX3)GetProcAddress(m_hDLL, "ModMsgGet_MATRIX3_v1");
      m_fncGet_MX4 = (FUNC_MMEXT2_GET_MX4)GetProcAddress(m_hDLL, "ModMsgGet_MATRIX4_v1");
      m_fncGet_CST = (FUNC_MMEXT2_GET_CST)GetProcAddress(m_hDLL, "ModMsgGet_c_str_v1");
      m_fncGet_CSL = (FUNC_MMEXT2_GET_CSL)GetProcAddress(m_hDLL, "ModMsgGet_c_str_len_v1");
      m_fncGet_MMS = (FUNC_MMEXT2_GET_MMS)GetProcAddress(m_hDLL, "ModMsgGet_MMStruct_v1");
      m_fncDel_ANY = (FUNC_MMEXT2_DEL_ANY)GetProcAddress(m_hDLL, "ModMsgDel_any_v1");
    };
    void __Exit() {
      if (m_hDLL) FreeLibrary(m_hDLL);
      m_fncPut_INT = NULL;
      m_fncPut_BOO = NULL;
      m_fncPut_DBL = NULL;
      m_fncPut_VEC = NULL;
      m_fncPut_MX3 = NULL;
      m_fncPut_MX4 = NULL;
      m_fncPut_MMS = NULL;
      m_fncGet_INT = NULL;
      m_fncGet_BOO = NULL;
      m_fncGet_DBL = NULL;
      m_fncGet_VEC = NULL;
      m_fncGet_MX3 = NULL;
      m_fncGet_MX4 = NULL;
      m_fncPut_CST = NULL;
      m_fncGet_CST = NULL;
      m_fncGet_CSL = NULL;
      m_fncGet_MMS = NULL;
      m_fncDel_ANY = NULL;
    };

  };



  class Advanced {
  public:
    void Init   (const char *moduleName)                                                               { m_mod = moduleName; m_imp.__Init(); }
    bool Put    (const char* var, const char val[], const VESSEL* ves = oapiGetFocusInterface()) const { return m_imp.__Put(m_mod, var, std::string(val), ves); }
    template<typename T>
    bool Put    (const char* var, const T& val, const VESSEL* ves = oapiGetFocusInterface())     const { return m_imp.__Put(m_mod, var, val, ves); }
    template<typename T>
    bool Get    (const char* var, T* val, const VESSEL* ves = oapiGetFocusInterface())           const { return m_imp.__Get(m_mod, var, val, ves); }
    bool Delete (const char* var, const VESSEL* ves = oapiGetFocusInterface())                   const { return m_imp.__Del(m_mod, var, ves); }
    void SetMod (const char *moduleName)                                                               { m_imp.__Init(); m_mod = moduleName; }

    template<typename T>
    bool PutMMStruct(const char* var, const T val, const VESSEL* ves = oapiGetFocusInterface()) const {
      const MMStruct *pSafeStruct = val;
      return m_imp.__Put(m_mod, var, pSafeStruct, ves);
    }

    template<typename T>
    bool GetMMStruct(const char* var, T* val, const unsigned int ver, const unsigned int siz, const VESSEL* ves = oapiGetFocusInterface()) const {
      const MMStruct *pMMStruct;
      if (!m_imp.__Get(m_mod, var, &pMMStruct, ves)) return false;
      if (!pMMStruct->IsCorrectSize(siz) || !pMMStruct->IsCorrectVersion(ver)) return false;
      *val = dynamic_cast<T>(pMMStruct);
      return (val != NULL);
    }


  private:
    Implementation m_imp;
    std::string m_mod;
  };

  class Basic
  {
  public:
    void Init   (const char *moduleName)                  { m_mod = moduleName; m_ves = oapiGetFocusInterface(); m_imp.__Init();    }
    bool Put    (const char* var, const char val[]) const { return m_imp.__Put(m_mod, var, std::string(val), m_ves); }
    template<typename T>
    bool Put    (const char* var, const T& val)     const { return m_imp.__Put(m_mod, var, val, m_ves); }
    template<typename T>
    bool Get    (const char* var, T* val)           const { return m_imp.__Get(m_mod, var, val, m_ves); }
    bool Delete (const char* var)                   const { return m_imp.__Del(m_mod, var, m_ves); }
  private:
    Implementation m_imp;
    const VESSEL *m_ves;
    std::string m_mod;
  };


}
#endif // MMExt2_Client_H
