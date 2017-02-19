#include "stdafx.h"
#include "enviroment.h"
#include <hammer/core/feature.h>
#include <hammer/core/subfeature.h>
#include <hammer/core/feature_registry.h>

struct feature_registry_test
{
   feature_registry registry_;
};

BOOST_FIXTURE_TEST_CASE(create_feature, feature_registry_test)
{
   feature_def& toolset_def = registry_.add_feature_def("toolset");
   toolset_def.extend_legal_values("msvc");
   BOOST_REQUIRE_THROW(registry_.add_feature_def("toolset");, std::exception);
   BOOST_REQUIRE_THROW(registry_.create_feature("", ""), std::exception);
};

struct complex_feature_registry_test : public feature_registry_test
{
   complex_feature_registry_test()
   {
      feature_def& toolset_def = registry_.add_feature_def("toolset");
      toolset_def.extend_legal_values("msvc");
      subfeature_def& version_def = toolset_def.add_subfeature("version");
      version_def.extend_legal_values("msvc", "8.0");
   }
};

BOOST_FIXTURE_TEST_CASE(create_subfeature, complex_feature_registry_test)
{
   feature* toolset = NULL;
   BOOST_REQUIRE_NO_THROW(toolset = registry_.create_feature("toolset", "msvc"));
   BOOST_REQUIRE(toolset != NULL);
   feature* same_toolset = NULL;
   BOOST_REQUIRE_NO_THROW(same_toolset = registry_.create_feature("toolset", "msvc"));
   BOOST_REQUIRE(same_toolset != NULL);
   BOOST_CHECK(toolset == same_toolset);
   BOOST_REQUIRE_NO_THROW(toolset = registry_.create_feature(*toolset, "version", "8.0"));
   const subfeature* version = NULL;
   BOOST_REQUIRE_NO_THROW(version = toolset->find_subfeature("version"));
   BOOST_CHECK_EQUAL(version->name(), "version");
   BOOST_CHECK_EQUAL(version->value(), "8.0");
}

BOOST_FIXTURE_TEST_CASE(parse_feature, complex_feature_registry_test)
{
   feature* toolset_msvc = NULL;
   BOOST_REQUIRE_NO_THROW(toolset_msvc = registry_.create_feature("toolset", "msvc"));
   BOOST_CHECK_EQUAL(toolset_msvc->name(), "toolset");
   BOOST_CHECK_EQUAL(toolset_msvc->value(), "msvc");

   BOOST_REQUIRE_NO_THROW(toolset_msvc = registry_.create_feature("toolset", "msvc-8.0"));
   BOOST_CHECK_EQUAL(toolset_msvc->name(), "toolset");
   BOOST_CHECK_EQUAL(toolset_msvc->value(), "msvc");
}

