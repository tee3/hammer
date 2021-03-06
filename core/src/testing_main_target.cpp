#include "stdafx.h"
#include <cassert>
#include <hammer/core/testing_main_target.h>
#include <hammer/core/engine.h>
#include <hammer/core/output_location_strategy.h>

namespace hammer{

testing_main_target::testing_main_target(const basic_meta_target* mt,
                                         const std::string& name,
                                         const target_type* t,
                                         const feature_set* props)
   : main_target(mt, name, t, props)
{}

location_t testing_main_target::intermediate_dir_impl() const
{
   assert(!sources().empty());
   return get_engine().output_location_strategy().compute_output_location(dynamic_cast<const main_target&>(*sources().front()));
}

}
