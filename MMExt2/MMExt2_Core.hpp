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


#include <map>
#include <string>
#include <OrbiterSDK.h>

#define DLLEXPIMP __declspec(dllexport)

namespace MMExt2
{
/*
	Purpose:

	Internal implementation of the Module Messaging Library.

	Developer Instructions:

	None. Do not call this directly or try to use this directly.
*/

  struct MMStruct {
  public:
    MMStruct(unsigned int sVer, unsigned int sSize) : _sVer(sVer), _sSize(sSize) {};
    virtual ~MMStruct() {};
  private:
    bool IsCorrectVersion(unsigned int sVer) const { return sVer == _sVer; }
    bool IsCorrectSize(unsigned int sSize) const { return sSize == _sSize; }
    unsigned int _sVer;
    unsigned int _sSize;
  };

	class MMExt2_Core
	{
	public:
		MMExt2_Core();
		virtual ~MMExt2_Core();

    static bool Put(const char* moduleName, const char* varName, const int& value, const char* vesselName);
    static bool Put(const char* moduleName, const char* varName, const bool& value, const char* vesselName);
    static bool Put(const char* moduleName, const char* varName, const double& value, const char* vesselName);
    static bool Put(const char* moduleName, const char* varName, const VECTOR3& value, const char* vesselName);
    static bool Put(const char* moduleName, const char* varName, const MATRIX3& value, const char* vesselName);
    static bool Put(const char* moduleName, const char* varName, const MATRIX4& value, const char* vesselName);
    static bool Put(const char* moduleName, const char* varName, const std::string& value, const char* vesselName);
    static bool Put(const char* moduleName, const char* varName, const MMStruct* value, const char* vesselName);

    static bool Get(const char* moduleName, const char* varName, int* value, const char* vesselName);
    static bool Get(const char* moduleName, const char* varName, bool* value, const char* vesselName);
    static bool Get(const char* moduleName, const char* varName, double* value, const char* vesselName);
    static bool Get(const char* moduleName, const char* varName, VECTOR3* value, const char* vesselName);
    static bool Get(const char* moduleName, const char* varName, MATRIX3* value, const char* vesselName);
    static bool Get(const char* moduleName, const char* varName, MATRIX4* value, const char* vesselName);
    static bool Get(const char* moduleName, const char* varName, std::string* value, const char* vesselName);
    static bool Get(const char* moduleName, const char* varName, const MMStruct** value, const char* vesselName);


    static bool Delete(const char* moduleName, const char* varName, const char* vesselName);

	protected:
	private:
		static std::string MakeID(const char* moduleName, const char* vesselName, const char* varName);
		template<class T> static bool SearchMap(const char* moduleName, const char* vesselName,
												const char* varName, const std::map<std::string, T>& mapToSearch, T* returnValue);
    template<class T> static bool SearchMapDelete(  const char* moduleName, const char* vesselName,
                                                        const char* varName, std::map<std::string, T>& mapToSearch);

		static const char m_token;
		static std::map<std::string, bool> m_bools;
		static std::map<std::string, int> m_ints;
		static std::map<std::string, double> m_doubles;
		static std::map<std::string, VECTOR3> m_VECTOR3s;
		static std::map<std::string, MATRIX3> m_MATRIX3s;
		static std::map<std::string, MATRIX4> m_MATRIX4s;
    static std::map<std::string, std::string> m_strings;
    static std::map<std::string, const MMStruct*> m_MMStructs;
	};
}
