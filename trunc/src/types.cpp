#include "stdafx.h"
#include "types.h"
#include "type.h"
#include <boost/assign/list_of.hpp>
#include <vector>
#include "type_registry.h"

using namespace std;
using namespace boost::assign;

namespace hammer{ namespace types{

const type_tag CPP("CPP");
const type_tag C("C");
const type_tag H("H");
const type_tag OBJ("OBJ");
const type_tag PCH("PCH");
const type_tag SHARED_LIB("SHARED_LIB");
const type_tag IMPORT_LIB("IMPORT_LIB");
const type_tag STATIC_LIB("STATIC_LIB");
const type_tag SEARCHED_LIB("SEARCHED_LIB");
const type_tag HEADER_LIB("HEADER_LIB");
const type_tag EXE("EXE");
const type_tag EXE_MANIFEST("EXE_MANIFEST");
const type_tag DLL_MANIFEST("DLL_MANIFEST");
const type_tag UNKNOWN("UNKNOWN");
const type_tag COPIED("COPIED");
const type_tag COPIED_TAG("COPIED_TAG");
const type_tag TESTING_OUTPUT("TESTING_OUTPUT");
const type_tag TESTING_RUN_PASSED("TESTING_RUN_PASSED");

   void register_standart_types(type_registry& tr)
   {
      const type tCPP(CPP, type::suffixes_t(list_of(".cpp")(".cc")));
      const type tC(C, ".c");
      const type tH(H, type::suffixes_t(list_of(".h")(".hpp")));
      const type tOBJ(OBJ, ".obj");
      const type tPCH(PCH, ".pch");
      const type tSHARED_LIB(SHARED_LIB, ".dll");
      const type tSTATIC_LIB(STATIC_LIB, ".lib");
      const type tIMPORT_LIB(IMPORT_LIB, ".lib");
      const type tSEARCHED_LIB(SEARCHED_LIB, "");
      const type tHEADER_LIB(HEADER_LIB, type::suffixes_t());
      const type tEXE(EXE, ".exe");
      const type tEXE_MANIFEST(EXE_MANIFEST, ".exe.manifest");
      const type tDLL_MANIFEST(DLL_MANIFEST, ".dll.manifest");
      const type tUNKNOWN(UNKNOWN, "");
      const type tCOPIED(COPIED, "");
      const type tCOPIED_TAG(COPIED_TAG, ".copy_tag");
      const type tTESTING_OUTPUT(TESTING_OUTPUT, ".test_output");
      const type tTESTING_RUN_PASSED(TESTING_RUN_PASSED, ".run_passed");
      
      tr.insert(tCPP);
      tr.insert(tC);
      tr.insert(tH);
      tr.insert(tOBJ);
      tr.insert(tPCH);
      tr.insert(tSHARED_LIB);
      tr.insert(tSTATIC_LIB);
      tr.insert(tIMPORT_LIB);
      tr.insert(tSEARCHED_LIB);
      tr.insert(tHEADER_LIB);
      tr.insert(tEXE);
      tr.insert(tEXE_MANIFEST);
      tr.insert(tDLL_MANIFEST);
      tr.insert(tUNKNOWN);
      tr.insert(tCOPIED);
      tr.insert(tCOPIED_TAG);
      tr.insert(tTESTING_OUTPUT);
      tr.insert(tTESTING_RUN_PASSED);
   }
}}
