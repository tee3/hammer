#include "stdafx.h"
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index_container.hpp>
#include <hammer/core/basic_target.h>
#include <hammer/core/build_action.h>
#include <hammer/core/engine.h>
#include <hammer/core/generated_target.h>
#include <hammer/core/generator.h>
#include <hammer/core/main_target.h>
#include <hammer/core/meta_target.h>
#include <hammer/core/np_helpers.h>
#include <hammer/core/target_type.h>
#include <utility>

namespace hammer {

generator::generator(hammer::engine& e,
                     std::string name,
                     consumable_types_t source_types,
                     producable_types_t target_types,
                     bool composite,
                     const feature_set* c)
  : engine_(&e)
  , name_(std::move(name))
  , source_types_(std::move(source_types))
  , target_types_(std::move(target_types))
  , composite_(composite)
  , constraints_(c)
  , include_composite_generators_(false)
{
}

bool
generator::is_consumable(const target_type& t) const
{
  if (consumable_types().empty()) {
    return true;
  }

  for (const auto& i : consumable_types()) {
    if (t.equal_or_derived_from(*i.type_)) {
      return true;
    }
  }

  return false;
}

basic_target*
generator::create_target(const main_target* mt,
                         const build_node::sources_t& /*sources*/,
                         const std::string& n,
                         const target_type* t,
                         const feature_set* f) const
{
  return new generated_target(mt, n, t, f);
}

build_nodes_t
generator::construct(const target_type& type_to_construct,
                     const feature_set& props,
                     const build_nodes_t& sources,
                     const basic_target* source_target,
                     const std::string* composite_target_name,
                     const main_target& owner) const
{
  if (source_target == nullptr) {
    boost::intrusive_ptr<build_node> result(new build_node(owner, composite_));
    result->action(action());

    using iter = build_nodes_t::const_iterator;
    for (const auto& source : sources) {
      bool node_added = false;
      for (build_node::targets_t::const_iterator
             p_i = (*source).products_.begin(),
             p_last = (*source).products_.end();
           p_i != p_last;
           ++p_i) {
        if (is_consumable((**p_i).type())) {
          result->sources_.emplace_back(*p_i, source);
          if (!node_added) {
            result->down_.push_back(source);
            node_added = true;
          }
        }
      }
    }

    unsigned p = 0;
    for (auto i = target_types_.begin(), last = target_types_.end(); i != last;
         ++i, ++p) {
      std::string new_name = make_product_name(*composite_target_name,
                                               *i->type_,
                                               props,
                                               i->need_tag_ ? &owner : nullptr,
                                               /*primary_target=*/p == 0);

      result->products_.push_back(
        create_target(&owner, result->sources_, new_name, i->type_, &props));
    }

    result->targeting_type_ = &type_to_construct;
    return build_nodes_t(1, result);
  }
  std::string new_name =
    make_product_name(*source_target,
                      type_to_construct,
                      props,
                      producable_types().front().need_tag_ ? &owner : nullptr);
  assert(sources.size() == 1);

  boost::intrusive_ptr<build_node> result(new build_node(owner, composite_));
  result->action(action());

  result->sources_.emplace_back(source_target, sources.front());
  result->down_.push_back(sources.front());
  result->products_.push_back(create_target(&owner,
                                            result->sources_,
                                            new_name,
                                            producable_types().front().type_,
                                            &props));
  result->targeting_type_ = &type_to_construct;
  return build_nodes_t(1, result);
}

generator::~generator() = default;

void
remove_dups(build_node::nodes_t& nodes)
{
  using namespace boost::multi_index;
  using node_t = boost::intrusive_ptr<build_node>;
  typedef multi_index_container<
    node_t,
    indexed_by<sequenced<>, ordered_unique<identity<node_t>>>>
    container_t;

  if (nodes.empty()) {
    return;
  }

  container_t c;
  container_t::nth_index<0>::type& idx = c.get<0>();
  for (build_node::nodes_t::const_reverse_iterator i = nodes.rbegin(),
                                                   last = nodes.rend();
       i != last;
       ++i) {
    idx.push_back(*i);
  }

  build_node::nodes_t result;
  result.reserve(c.get<0>().size());
  using idx_0_t = container_t::nth_index<0>::type;
  for (idx_0_t::const_reverse_iterator i = c.get<0>().rbegin(),
                                       last = c.get<0>().rend();
       i != last;
       ++i) {
    result.push_back(*i);
  }

  nodes.swap(result);
}

generator::producable_types_t
make_product_types(engine& e, const std::vector<type_tag>& types)
{
  generator::producable_types_t result;
  for (const type_tag& t : types) {
    result.emplace_back(e.get_type_registry().get(t));
  }

  return result;
}

generator::consumable_types_t
make_consume_types(engine& e, const std::vector<type_tag>& types)
{
  generator::consumable_types_t result;
  for (const type_tag& t : types) {
    result.emplace_back(e.get_type_registry().get(t));
  }

  return result;
}
} // namespace hammer
