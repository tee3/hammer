#include "stdafx.h"
#include <cassert>
#include <hammer/core/feature_base.h>
#include <utility>

namespace hammer {

feature_base::feature_base(const feature_def* def, std::string value)
  : definition_(def)
  , value_(std::move(value))
{
  assert(def && "Definition cannot be NULL");
}
} // namespace hammer
