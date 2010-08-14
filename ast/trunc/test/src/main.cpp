#include "stdafx.h"
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/bind.hpp>
#include <boost/assign/std/vector.hpp>
#include <iostream>
#include <hammer/parser/parser.h>
#include <hammer/sema/actions_impl.h>
#include <hammer/ast/context.h>
#include <hammer/ast/ast_xml_printer.h>
#include <hammer/ast/hamfile.h>
#include <hammer/core/rule_manager.h>
#include <hammer/core/diagnostic.h>

boost::filesystem::path test_data_path;

using namespace std; 
using namespace hammer;
using namespace boost::unit_test;
using namespace boost::assign;
namespace fs = boost::filesystem;

static void lib_rule(const parscore::identifier& id)
{

}

void test_function(const fs::path& hamfile)
{
   rule_manager rule_manager;
   stringstream error_stream;
   streamed_diagnostic diag(error_stream);
   vector<parscore::identifier> lib_arg_names;
   lib_arg_names += "target-name";
   rule_manager.add_target("lib", boost::function<void(const parscore::identifier&)>(&lib_rule),
                           lib_arg_names);
   ast::context ctx(rule_manager, diag);
   sema::actions_impl actions(ctx);
   const ast::hamfile* ast_top = parser::parser::parse(hamfile, actions);

   fs::ofstream f(hamfile.branch_path() / (hamfile.filename() + ".ast"));
   ast::ast_xml_printer printer(f);
   ast_top->accept(printer);
}

boost::unit_test::test_suite*
init_unit_test_suite( int argc, char* argv[] )
{
   if (argc < 2)
   {
      cout << "Need test data directory path\n";
      return NULL;
   }
       
   test_data_path = fs::path(argv[1]); 
   if (!test_data_path.has_root_path())
   {
      test_data_path = fs::current_path() / test_data_path;
      test_data_path.normalize();
   }


   // Because we have massive memleaks we disable leak reporting until we resolve memleaks
//   _CrtSetDbgFlag(_CrtSetDbgFlag( _CRTDBG_REPORT_FLAG ) & ~_CRTDBG_LEAK_CHECK_DF);
//   _CrtSetBreakAlloc(1935);

   test_suite* ts = BOOST_TEST_SUITE("main");
   for(fs::directory_iterator i(test_data_path); i != fs::directory_iterator(); ++i)
      if (i->path().filename() != ".svn" &&
          extension(i->path()) == ".ham")
      {
         ts->add(make_test_case(boost::bind(&test_function, i->path()), i->path().filename()));
      }

   framework::master_test_suite().add(ts);

   return 0;
}                             

