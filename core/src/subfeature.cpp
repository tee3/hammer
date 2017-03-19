#include "stdafx.h"
#include <hammer/core/subfeature.h>
#include <hammer/core/subfeature_def.h>

#include <utility>

namespace hammer {

subfeature::subfeature(const subfeature_def& def, std::string value)
  : definition_(&def)
  , value_(std::move(value))
{
}

subfeature::~subfeature() = default;

const std::string&
subfeature::name() const
{
  return definition_->name();
}

bool
subfeature::operator==(const subfeature& rhs) const
{
  return definition_->name() == rhs.definition().name() &&
         value_ == rhs.value();
}
} // namespace hammer
