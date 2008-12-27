#include "stdafx.h"
#include "pch_meta_target.h"
#include "types.h"
#include "project.h"
#include "engine.h"
#include "pch_main_target.h"
#include "feature_registry.h"
#include "feature.h"
#include "feature_set.h"

namespace hammer
{

pch_meta_target::pch_meta_target(hammer::project* p, const pstring& name, 
                                 const requirements_decl& req, 
                                 const requirements_decl& usage_req)
  : meta_target(p, name, req, usage_req), 
    last_constructed_main_target_(NULL)
{
}

main_target* pch_meta_target::construct_main_target(const feature_set* properties) const
{
   feature_set* modified_properties = properties->clone();
   feature* create_pch_feature = project()->engine()->feature_registry().create_feature("pch", "create");
   modified_properties->join(create_pch_feature);
   modified_properties->join("__pch", "");
   
   last_constructed_main_target_ = new pch_main_target(this, 
                                                       name(), 
                                                       &project()->engine()->get_type_registry().get(types::PCH), 
                                                       modified_properties,
                                                       project()->engine()->targets_pool());

   create_pch_feature->get_generated_data().target_ = last_constructed_main_target_;
   return last_constructed_main_target_;
}

void pch_meta_target::compute_usage_requirements(feature_set& result, 
                                                 const feature_set& full_build_request,
                                                 const feature_set& computed_usage_requirements) const
{
   // adding pch feature to usage requirements to mark dependent targets as built with pch
   this->usage_requirements().eval(full_build_request, &result);
   feature* pch_feature = project()->engine()->feature_registry().create_feature("pch", "use");
   pch_feature->get_generated_data().target_ = last_constructed_main_target_;
   result.join(pch_feature);
}

sources_decl pch_meta_target::compute_additional_sources(const main_target& owner) const
{
   sources_decl result;
   for(sources_decl::const_iterator i = owner.meta_target()->sources().begin(), last =  owner.meta_target()->sources().end(); i!= last; ++i)
      if (i->type() == NULL && // that meta target
          i->target_path() != name()) // FIXME: skip self - should be more intelligent logic
      {
         result.push_back(*i);
      }

   return result;
}

}