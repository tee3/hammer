#include "stdafx.h"
#include <hammer/core/engine.h>
#include <hammer/core/main_target.h>
#include <hammer/core/project.h>
#include <hammer/core/typed_meta_target.h>

namespace hammer {

main_target*
typed_meta_target::construct_main_target(const main_target* /*owner*/,
                                         const feature_set* properties) const
{
  auto* mt = new main_target(this, name(), type_, properties);
  return mt;
}
} // namespace hammer
