#include "stdafx.h"
#include <hammer/core/basic_target.h>
#include <hammer/core/build_node.h>

#include <algorithm>
#include <cassert>

namespace hammer {

const basic_target*
build_node::find_product(const basic_target* t) const
{
  auto i = std::find(products_.begin(), products_.end(), t);
  if (i != products_.end()) {
    return *i;
  }
  {
    return nullptr;
  }
}

const feature_set&
build_node::build_request() const
{
  assert(!products_.empty());
  return products_.front()->properties();
}

void
build_node::up_to_date(boost::tribool::value_t v)
{
  up_to_date_ = v != 0u;
}
} // namespace hammer
