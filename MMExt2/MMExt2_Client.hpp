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

  Bi-directional data interchange between Orbiter modules (e.g. MFDs, Vessels). 

  Developer Instructions:

  1. Install the ModuleMessagingExt addon from OrbitHangar, to get this file in your Orbitersdk\Include folder. 

  2. #include "MMExt2_Client.hpp" where you need to get data from MMExt2 (e.g. in your class header file). 

  3. Choose if you want to use the Basic or Advanced interface: 

     Basic: Put, Get, Delete on the following types: int, bool, double, VECTOR3, MATRIX3, MATRIX4, std::string

     Advanced: all of basic, plus the option to Put and Delete for other vessels, plus put, get, delete of
     MMStruct-derived structures. 

  For Basic Interface:

  4. In your class definition, declare the Module Messaging interface with:    MMExt2::Basic mm; 

  5. In your class constructor, initialize the interface with:   mm.Init(MY_MODULE_NAME);
     (where MY_MODULE_NAME is a char string for your module name ... e.g. mm.Init("GS2") )

  6. Where you want to PUT a value to Module Messaging:

        RET = mm.Put(VARNAME, VALUE); 
        ... where VARNAME is a char * constant for the variable name (e.g. "BurnTime")
                  VALUE is a literal value or a pass-by-reference to one of the 7 base types,
                  RET = true if MM is installed and the put was successful (ignore if you don't care!)

  7. Where you want to DELETE a value from Module Messaging (e.g. if not longer valid):

        RET = mm.Delete(VARNAME);
        ... where RET = true if MM is installed and the delete was successful (ignore if you don't care!)
        ... note: the function deletes all variables with VARNAME, regardless of type. 

  8. Where you want to GET a value from Module Messaging (i.e. get from another module:

        RET = mm.Get(MODULE, VARNAME, &VALUE);
        ... where MODULE is a char * constant for the module you are getting from
                  &VALUE is the address for the return data type
                  RET = true if MM is installed and the get was successful (ignore if you don't care!)
        

  For the Advanced Interface:

  9. Declare your interface with:   MMExt2::Advanced mm;   (instead of MMExt2::Basic mm;)
  
  10. Init is the same as the basic init:   mm.Init(MY_MODULE_NAME);

  11. The Put, Delete, Get all take an optional extra parameter for the vessel you are interacting with
  (e.g. if you want to maintain different Module Messaging values for multiple vessels in the simulation). 

  12. You are additionally able to put and get a structure. **WARNING** this allows clients read-only
      access directly to your data structure without needing to do repreated GET functions. Although this sounds
      great, you need to be careful to implement it properly, or you will cause crashes to desktop. Still interested?
      Read on.

  For Advanced Interface Put for Structures:

  13. You *must* derive from MMExt2::MMStruct. Create a .hpp definition file for your struct and put into your
      .zip package so it unzips into Orbitersdk\include ... e.g. <MY_MODULE>_Exports.hpp. 
      
      Define your structure as follows:

      #pragma once
      #pragma pack(push)
      #pragma pack(8)
      #define MY_STRUCT_VERSION 1
      struct MY_STRUCT_NAME : public MMExt2::MMStruct {
          MyStruct() : MMExt2::MMStruct(MY_STRUCT_VERSION, sizeof(MyStruct)) {};
           ... add your elements here ... 
           ... but keep things with simple data types only and no code, so it works across compilers ...
           ... especially do not include standard template library elements like std::string, std::vector ...
      };
      #pragma pack(pop)

      ... where MY_STRUCT_NAME is whatever you want, and 
                MY_STRUCT_VERSION is a version number for your structure (increment it when you change
                    the structure, to ensure that clients not linked to your latest version intentionally
                    break cleanly - i.e. RET = false - until they re-compile with your updated data). 

      The #pragma pack lines ensure a consistent packing for the structure, across Visual C++ compilers.
      The structure derivation from MMExt2::MMStruct allows Module Messaging to do extra safety checks
      to ensure the Put and Get modules are both using the same version of the struct, and the same size. 

  14. Instantiate your structure in a persistent place in your code (i.e. do not allow it to be destroyed for the
      whole execution of the simulation, as other modules will be referencing your data directly) 
  
  15. To make your structure accessible, do:

          RET = mm.PutMMStruct(VARNAME, &STRUCT); 
          where ... &STRUCT is the address of your MMStruct-derived structure (e.g. &mms)
                    RET is true if MM is present, and the structure PUT was successful. 

  16. Note ... there is no Delete function for MMStruct structs, intentionally, as once you have exposed the
      struct, you must assume that one or more other modules have attached to your struct and are relying on it. 
      If you need to flag the struct as invalid, consider putting a boolen in the structure defintion (e.g
      bool IsValid;)

  For Advanced Interface Get for Structures:

  17. Include the header file for the structure definition - e.g. #include "<OTHER_MODULE>_Exports.hpp" from the
      provider of the structure. 

  18. Declare a constant pointer to the structure - e.g. 

      const OTHER_MODULE_STRUCT_NAME *p_OTHER_MODULE_struct;
      ... where MY_STRUCT_NAME is the name gicen by the other module's export file. 

      Note ... this *must* be a const pointer, as you are not allowed to change the other module's private
      data structure. If you need 2-way interchange, then Put your own module back and agree a message passing
      protocol with the other developer. 

  19. To get the remote structure, do:

          RET = mm.GetMMStruct(MODULENAME, VARNAME, &p_OTHER_MODULE_struct, MY_STRUCT_VERSION, sizeof(OTHER_MODULE_STRUCT_NAME);
          ... where MODULENAME is the other module's name
                    VARNAME is the structure name variable
                    &p_OTHER_MODULE_struct is the address of your struct pointer
                    MY_STRUCT_VERSION ... is the version number defined in the export file (note: when the structure 
                      changes version, you will need to re-compile and re-release your module to attach to the new structure)
                    sizeof(OTHER_MODULE_STRUCT_NAME) ... is a final safety check to make sure your compiler's view of the size
                      of the structure matches the Put module compiler's view of the same structure. 
                    and RET is true if it all works as expected!

   That's it. No additional linkage is needed for youtr module, and there is guaranteed no "Error 126" DLL not found from this
   code. All that happens if your user does not install ModuleMessagingExt is that you will get consistent FALSE responses on all
   Put, Get, and Delete calls. 
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
    void Init   (const char *moduleName)                                                                      { m_mod = moduleName; m_imp.__Init(); }
    bool Put    (const char* var, const char val[], const VESSEL* ves = oapiGetFocusInterface())        const { return m_imp.__Put(m_mod, var, std::string(val), ves); }
    template<typename T>
    bool Put    (const char* var, const T& val, const VESSEL* ves = oapiGetFocusInterface())            const { return m_imp.__Put(m_mod, var, val, ves); }
    template<typename T>
    bool Get    (const char* mod, const char* var, T* val, const VESSEL* ves = oapiGetFocusInterface()) const { return m_imp.__Get(std::string(mod), var, val, ves); }
    bool Delete (const char* var, const VESSEL* ves = oapiGetFocusInterface())                          const { return m_imp.__Del(m_mod, var, ves); }

    template<typename T>
    bool PutMMStruct(const char* var, const T val, const VESSEL* ves = oapiGetFocusInterface()) const {
      const MMStruct *pSafeStruct = val;
      return m_imp.__Put(m_mod, var, pSafeStruct, ves);
    }

    template<typename T>
    bool GetMMStruct(const char* mod, const char* var, T* val, const unsigned int ver, const unsigned int siz, const VESSEL* ves = oapiGetFocusInterface()) const {
      const MMStruct *pMMStruct;
      if (!m_imp.__Get(std::string(mod), var, &pMMStruct, ves)) return false;
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
    void Init   (const char *moduleName)                        { m_mod = moduleName; m_ves = oapiGetFocusInterface(); m_imp.__Init();    }
    bool Put    (const char* var, const char val[])        const { return m_imp.__Put(m_mod, var, std::string(val), m_ves); }
    template<typename T>
    bool Put    (const char* var, const T& val)            const { return m_imp.__Put(m_mod, var, val, m_ves); }
    template<typename T>
    bool Get    (const char* mod, const char* var, T* val) const { return m_imp.__Get(std::string(mod), var, val, m_ves); }
    bool Delete (const char* var)                          const { return m_imp.__Del(m_mod, var, m_ves); }
  private:
    Implementation m_imp;
    const VESSEL *m_ves;
    std::string m_mod;
  };


}
#endif // MMExt2_Client_H
