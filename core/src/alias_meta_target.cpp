#include "stdafx.h"
#include <hammer/core/alias_meta_target.h>
#include <hammer/core/main_target.h>
#include <hammer/core/project.h>
#include <hammer/core/engine.h>
#include <hammer/core/feature_registry.h>
#include <hammer/core/feature_set.h>
#include <hammer/core/feature.h>
#include <hammer/core/fs_helpers.h>

using namespace std;

namespace hammer{

alias_meta_target::alias_meta_target(hammer::project* p, const pstring& name, 
                                     const sources_decl& sources,
                                     const requirements_decl& req,
                                     const requirements_decl& usage_req) 
                                     : basic_meta_target(p, name, req, usage_req)
{
   this->sources(sources);
}
   
void alias_meta_target::instantiate_impl(const main_target* owner, 
                                         const feature_set& build_request,
                                         std::vector<basic_target*>* result, 
                                         feature_set* usage_requirements) const
{
   if (owner != NULL) {
      this->usage_requirements().eval(owner->properties(), usage_requirements);

      for (const source_decl& sd : sources()) {
         feature* f = get_engine()->feature_registry().create_feature("source", "");
         source_decl new_sd = sd;

         // apply build request to a target
         if (new_sd.properties())
            new_sd.properties()->join(build_request);
         else
            new_sd.properties(build_request.clone());

         f->set_dependency_data(new_sd, this);

         usage_requirements->join(f);
      }

      adjust_dependency_features_sources(*usage_requirements, *this, owner);
   } else {
      // top level alias instantiation
      vector<basic_target*> sources;
      sources_decl simple_targets;
      meta_targets_t meta_targets;
      split_sources(&simple_targets, &meta_targets, this->sources(), build_request);
      instantiate_meta_targets(meta_targets, build_request, NULL, &sources, usage_requirements);
      result->insert(result->end(), sources.begin(), sources.end());
   }
}

}
