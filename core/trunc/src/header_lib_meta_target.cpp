#include "stdafx.h"
#include <hammer/core/header_lib_meta_target.h>
#include <hammer/core/feature_set.h>
#include <hammer/core/types.h>
#include <hammer/core/project.h>
#include <hammer/core/engine.h>
#include <hammer/core/type_registry.h>
#include <hammer/core/header_lib_main_target.h>

namespace hammer{

header_lib_meta_target::header_lib_meta_target(hammer::project* p, const pstring& name, 
                                               const requirements_decl& requirements,
                                               const requirements_decl& usage_requirements)
   : typed_meta_target(p, name, requirements, 
                       usage_requirements, 
                       p->engine()->get_type_registry().get(types::HEADER_LIB))
{

}

void header_lib_meta_target::compute_usage_requirements(feature_set& result, 
                                                        const feature_set& full_build_request,
                                                        const feature_set& computed_usage_requirements) const
{
   meta_target::compute_usage_requirements(result, full_build_request, computed_usage_requirements);
   result.join(computed_usage_requirements);
}

main_target* header_lib_meta_target::construct_main_target(const feature_set* properties) const
{
   main_target* mt = new header_lib_main_target(this, 
                                                name(), 
                                                &type(), 
                                                properties,
                                                project()->engine()->targets_pool());
   return mt;
}

}