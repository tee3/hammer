#include "stdafx.h"
#include <boost/unordered_set.hpp>
#include <hammer/core/basic_target.h>
#include <hammer/core/cleaner.h>

namespace hammer {

struct cleaner::impl_t
{
  impl_t(engine& e, const build_environment& environment)
    : engine_(e)
    , environment_(environment)
  {
  }

  void clean_all(result& r, build_node& n);

  engine& engine_;
  const build_environment& environment_;

  boost::unordered_set<build_node*> visited_nodes_;
};

void
cleaner::impl_t::clean_all(result& r, build_node& n)
{
  for (auto& source : n.sources_) {
    if (visited_nodes_.find(source.source_node_.get()) ==
        visited_nodes_.end()) {
      visited_nodes_.insert(source.source_node_.get());
      clean_all(r, *source.source_node_);
    }
  }

  for (auto& product : n.products_) {
    (*product).clean(environment_);
    r.cleaned_target_count_++;
  }

  for (auto& dependencie : n.dependencies_) {
    if (visited_nodes_.find(dependencie.get()) == visited_nodes_.end()) {
      visited_nodes_.insert(dependencie.get());
      clean_all(r, *dependencie);
    }
  }
}

cleaner::cleaner(engine& e, const build_environment& environment)
  : impl_(new impl_t(e, environment))
{
}

cleaner::result
cleaner::clean_all(nodes_t& nodes)
{
  result r;
  impl_->visited_nodes_.clear();

  for (auto& node : nodes) {
    impl_->clean_all(r, *node);
  }

  return r;
}
} // namespace hammer
