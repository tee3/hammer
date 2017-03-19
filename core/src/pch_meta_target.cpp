#include "stdafx.h"
#include <hammer/core/engine.h>
#include <hammer/core/feature.h>
#include <hammer/core/feature_registry.h>
#include <hammer/core/feature_set.h>
#include <hammer/core/pch_main_target.h>
#include <hammer/core/pch_meta_target.h>
#include <hammer/core/project.h>
#include <hammer/core/types.h>

namespace hammer {

pch_meta_target::pch_meta_target(hammer::project* p,
                                 const std::string& name,
                                 const requirements_decl& req,
                                 const requirements_decl& usage_req)
  : meta_target(p, name, req, usage_req)
  , last_constructed_main_target_(nullptr)
  , last_instantiation_owner_(nullptr)
{
  set_explicit(true);
}

bool
pch_meta_target::is_cachable(const main_target* owner) const
{
  if (last_instantiation_owner_ == nullptr) {
    last_instantiation_owner_ = owner;
  } else if (last_instantiation_owner_ != owner) {
    throw std::runtime_error(
      "Using pch target in to different places is not allowed.");
  }

  return true;
}

main_target*
pch_meta_target::construct_main_target(const main_target* owner,
                                       const feature_set* properties) const
{
  if (owner == nullptr) {
    throw std::runtime_error("pch main target must have owner. Don't try to "
                             "build pch targets standalone.");
  }

  feature_set* modified_properties = properties->clone();
  feature* create_pch_feature =
    get_engine()->feature_registry().create_feature("pch", "create");
  modified_properties->join(create_pch_feature);
  modified_properties->join("__pch", "");

  last_constructed_main_target_ =
    new pch_main_target(this,
                        *owner,
                        name(),
                        &get_engine()->get_type_registry().get(types::PCH),
                        modified_properties);

  create_pch_feature->get_generated_data().target_ =
    last_constructed_main_target_;
  return last_constructed_main_target_;
}

void
pch_meta_target::compute_usage_requirements(
  feature_set& result,
  const main_target& constructed_target,
  const feature_set& /*build_request*/,
  const feature_set& /*computed_usage_requirements*/,
  const main_target* owner) const
{
  // adding pch feature to usage requirements to mark dependent targets as built
  // with pch
  this->usage_requirements().eval(constructed_target.properties(), &result);
  feature* pch_feature =
    get_engine()->feature_registry().create_feature("pch", "use");
  pch_feature->get_generated_data().target_ = last_constructed_main_target_;
  result.join(pch_feature);
  // add dependency on self to build pch before main target that use it
  if (owner == nullptr) {
    throw std::runtime_error("pch main target must have owner and cannot be "
                             "instantiated standalone.");
  }

  feature* self_dependency_feature =
    get_engine()->feature_registry().create_feature("dependency", "");
  for (const auto& i : owner->get_meta_target()->sources()) {
    if (i.type() == nullptr && // that meta target
        i.target_path() ==
          name()) // FIXME: skip self - should be more intelligent logic
    {
      self_dependency_feature->set_dependency_data(i, this);
      break;
    }
  }

  result.join(self_dependency_feature);
}

sources_decl
pch_meta_target::compute_additional_sources(const main_target& owner) const
{
  sources_decl result;
  for (const auto& i : owner.get_meta_target()->sources()) {
    if (i.type() == nullptr && // that meta target
        i.target_path() !=
          name()) // FIXME: skip self - should be more intelligent logic
    {
      result.push_back(i);
    }
  }

  return result;
}
} // namespace hammer
