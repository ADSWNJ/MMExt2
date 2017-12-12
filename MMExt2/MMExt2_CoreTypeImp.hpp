#define MMEXT2_XMAP(value) __MMEXT2_XMAP(value)
#define __MMEXT2_XMAP(value) m_##value##s

bool MMExt2_Core::Put(const char* moduleName, const char* varName, const MMEXT2_TYPE MMEXT2_INSFX value, const char* vesselName)
{


  MMEXT2_XMAP(MMEXT2_TYPE)[MakeID(moduleName, vesselName, varName)] = value;
  return true;
}


bool MMExt2_Core::Get(const char* moduleName, const char* varName, MMEXT2_TYPE MMEXT2_OUTSFX value, const char* vesselName)
{
  return SearchMap<MMEXT2_TYPE>(moduleName, vesselName, varName, MMEXT2_XMAP(MMEXT2_TYPE), value);
}

#undef MMEXT2_TYPE
#undef MMEXT2_MAP
