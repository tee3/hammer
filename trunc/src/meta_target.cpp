#include "stdafx.h"
#include "meta_target.h"
#include "project.h"
#include "engine.h"
#include "type_registry.h"
#include "source_target.h"
#include "main_target.h"
#include "feature_set.h"
#include "feature_registry.h"
#include "feature.h"
#include "location.h"
#include "requirements_decl.h"

using namespace std;

namespace hammer{
   meta_target::meta_target(hammer::project* p, 
                            const pstring& name, 
                            const requirements_decl& props, 
                            const requirements_decl& usage_req) 
                           : basic_meta_target(p, name, props, usage_req)
   {
      requirements().insert_infront(p->requirements());
      usage_requirements().insert_infront(p->usage_requirements());
   }

   void meta_target::instantiate_meta_targets(sources_decl& simple_targets,
                                              feature_set& usage_requirements,
                                              std::vector<basic_target*>& instantiated_targets,
                                              const meta_targets_t& meta_targets,
                                              const feature_set& build_request,
                                              const main_target& owner_for_new_targets) const
   {
      feature_set& local_usage_requirements = *project()->engine()->feature_registry().make_set();
      basic_meta_target::instantiate_meta_targets(meta_targets, build_request, &owner_for_new_targets, 
                                                  &instantiated_targets, &local_usage_requirements);

      sources_decl sources_from_features;
      extract_sources(sources_from_features, local_usage_requirements);
      if (!sources_from_features.empty())
      {
         meta_targets_t new_meta_targets;
         split_sources(&simple_targets, &new_meta_targets, sources_from_features, build_request);
         instantiate_meta_targets(simple_targets, usage_requirements, 
                                  instantiated_targets, new_meta_targets, 
                                  build_request, owner_for_new_targets);
      }
      else
         usage_requirements.join(local_usage_requirements);
   }

   void meta_target::instantiate(const main_target* owner,
                                 const feature_set& build_request,
                                 std::vector<basic_target*>* result,
                                 feature_set* usage_requirements) const
   {
      feature_set* mt_fs = build_request.clone();
      requirements().eval(mt_fs, project()->engine()->feature_registry());
      project()->engine()->feature_registry().add_defaults(mt_fs);

      feature_set* build_request_for_dependencies = project()->engine()->feature_registry().make_set();
      build_request_for_dependencies->copy_propagated(build_request);
      build_request_for_dependencies->copy_propagated(*mt_fs);

      vector<basic_target*> instantiated_targets;
      sources_decl simple_targets;
      meta_targets_t meta_targets;

      sources_decl sources_from_requirements;
      extract_sources(sources_from_requirements, *mt_fs);

      split_sources(&simple_targets, &meta_targets, sources(), *build_request_for_dependencies); 
      split_sources(&simple_targets, &meta_targets, sources_from_requirements, *build_request_for_dependencies);

      main_target* mt = construct_main_target(mt_fs);
      instantiate_meta_targets(simple_targets, *usage_requirements, instantiated_targets, meta_targets, *build_request_for_dependencies, *mt);
      
      mt_fs->join(*usage_requirements);
      mt->properties(mt_fs);
      instantiate_simple_targets(simple_targets, *mt_fs, *mt, &instantiated_targets);
      mt->sources(instantiated_targets);
      this->usage_requirements().eval(*mt_fs, usage_requirements);
      
      result->push_back(mt);
   }
}