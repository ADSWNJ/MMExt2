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

using namespace std;

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

    static bool Put(const string& id, const bool& val);
    static bool Put(const string& id, const int& val);
    static bool Put(const string& id, const double& val);
    static bool Put(const string& id, const string& val);
    static bool Put(const string& id, const VECTOR3& val);
    static bool Put(const string& id, const MATRIX3& val);
    static bool Put(const string& id, const MATRIX4& val);
    static bool Put(const string& id, const MMStruct* val);

    static bool Get(const string& get, const string& id, int* val);
    static bool Get(const string& get, const string& id, bool* val);
    static bool Get(const string& get, const string& id, double* val);
    static bool Get(const string& get, const string& id, string* val);
    static bool Get(const string& get, const string& id, VECTOR3* val);
    static bool Get(const string& get, const string& id, MATRIX3* val);
    static bool Get(const string& get, const string& id, MATRIX4* val);
    static bool Get(const string& get, const string& id, const MMStruct** val);

    static bool MMExt2_Core::Delete(const string& s);
    static bool Find(const string& fGet, const string& fMod, const string& fVar, const string& fVes, int* fIx, bool skp, string* rMod, string* rVar, string* rVes, char* rTyp);

	protected:
	private:
    static bool DeleteType(const string &id, const char type);

		template<class T> static bool SearchMap(const string& get, const string& id, const map<string, T>& mapToSearch, T* returnValue);
    template<class T> static bool SearchMapDelete(const string &id, map<string, T>& mapToSearch);

		static const char m_token;
		static map<string, bool> m_bools;
		static map<string, int> m_ints;
		static map<string, double> m_doubles;
		static map<string, VECTOR3> m_VECTOR3s;
		static map<string, MATRIX3> m_MATRIX3s;
		static map<string, MATRIX4> m_MATRIX4s;
    static map<string, string> m_strings;
    static map<string, const MMStruct*> m_MMStructs;
    static map<string, char> MMExt2_Core::m_types;
	};
}
