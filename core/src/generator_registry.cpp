#include "stdafx.h"
#include <boost/format.hpp>
#include <hammer/core/feature.h>
#include <hammer/core/feature_set.h>
#include <hammer/core/generator_registry.h>
#include <hammer/core/main_target.h>
#include <hammer/core/subfeature.h>
#include <hammer/core/target_type.h>
#include <stdexcept>

using namespace std;
using namespace boost;

namespace hammer {

void
generator_registry::insert(std::unique_ptr<generator> g)
{
  std::string g_name = g->name();
  if (!generators_.emplace(g_name, std::move(g)).second) {
    throw std::runtime_error("Generator '" + g_name + "' already registered.");
  }
}

static int
compute_rank(const feature& from_build_request, const feature& from_constraints)
{
  if (from_build_request.value() != from_constraints.value()) {
    return -1;
  }

  int rank = 100;

  auto br_first = from_build_request.subfeatures().begin();
  auto br_last = from_build_request.subfeatures().end();

  for (; br_first != br_last; ++br_first) {
    const subfeature* cs =
      from_constraints.find_subfeature((**br_first).name());
    if ((cs == nullptr) || cs->value() != (**br_first).value()) {
      return -1;
    }

    rank += 1;
  }

  return rank;
}

static int
compute_rank(const feature_set& build_properties,
             const feature_set& constraints)
{
  int rank = 0;
  for (auto constraint : constraints) {
    auto i_in_build = build_properties.find((*constraint).name());
    if (i_in_build != build_properties.end()) {
      const int f_runk = compute_rank(**i_in_build, *constraint);
      if (f_runk == -1) {
        return -1;
      }

      rank += f_runk;
    } else {
      return -1;
    }
  }

  return rank;
}

generator_registry::viable_generators_t
generator_registry::find_viable_generators(
  const target_type& t,
  bool allow_composite,
  const feature_set& build_properties) const
{
  viable_generators_t result = find_viable_generators(
    t, allow_composite, build_properties, /*full_match=*/true);
  if (!result.empty()) {
    return result;
  }
  {
    return find_viable_generators(t,
                                  allow_composite,
                                  build_properties,
                                  /*full_match=*/false);
  }
}

generator_registry::viable_generators_t
generator_registry::find_viable_generators(const target_type& t,
                                           bool allow_composite,
                                           const feature_set& build_properties,
                                           bool full_match) const
{
  viable_generators_t result;
  int rank = 0; // rank show as the weight of generator (the more rank the more
  // generator suitable for generation this type of targets)
  for (const auto& i : generators_) {
    if ((i.second->is_composite() && !allow_composite) ||
        (!i.second->is_composite() && allow_composite)) {
      continue;
    }

    for (auto j = i.second->producable_types().begin(),
              j_last = i.second->producable_types().end();
         j != j_last;
         ++j) {
      if ((!full_match && j->type_->equal_or_derived_from(t)) ||
          (full_match && *j->type_ == t)) {
        int generator_rank =
          i.second->constraints() != nullptr
            ? compute_rank(build_properties, *i.second->constraints())
            : 0;
        if (generator_rank ==
            -1) { // build properties not satisfied generator constraints
          continue;
        }

        if (generator_rank > rank) {
          rank = generator_rank;
          result.clear();
        }

        if (rank == generator_rank) {
          result.emplace_back(make_pair(i.second.get(), j->type_));
        }
      }
    }
  }

  return result;
}

bool
generator_registry::transform(const generator& target_generator,
                              const generator& current_generator,
                              const basic_target* source_target,
                              build_node_ptr& source_node,
                              build_nodes_t* result,
                              const feature_set& props,
                              const main_target& owner) const
{
  for (auto i = current_generator.consumable_types().begin(),
            last = current_generator.consumable_types().end();
       i != last;
       ++i) {
    viable_generators_t vg(
      find_viable_generators(*i->type_,
                             current_generator.include_composite_generators(),
                             props,
                             /*full_match=*/true));
    if (vg.empty()) {
      continue;
    }

    for (viable_generators_t::const_iterator g_i = vg.begin(),
                                             g_last = vg.end();
         g_i != g_last;
         ++g_i) {
      if (g_i->first->is_consumable(source_target->type())) {
        build_nodes_t r(g_i->first->construct(*g_i->second,
                                              props,
                                              build_nodes_t(1, source_node),
                                              source_target,
                                              nullptr,
                                              owner));
        remove_dups(source_node->dependencies_);
        result->insert(result->end(), r.begin(), r.end());
        return true;
      }
      build_nodes_t this_result;
      if (transform(target_generator,
                    *g_i->first,
                    source_target,
                    source_node,
                    &this_result,
                    props,
                    owner)) {
        for (build_nodes_t::const_iterator r_i = this_result.begin(),
                                           r_last = this_result.end();
             r_i != r_last;
             ++r_i) {
          transform_to_consumable(
            target_generator, current_generator, *r_i, result, props, owner);
        }

        return true;
      }
    }

    // !!!!! This is copy paste from above!!!!
    vg =
      find_viable_generators(*i->type_,
                             current_generator.include_composite_generators(),
                             props,
                             /*full_match=*/false);
    if (vg.empty()) {
      continue;
    }

    for (viable_generators_t::const_iterator g_i = vg.begin(),
                                             g_last = vg.end();
         g_i != g_last;
         ++g_i) {
      if (g_i->first->is_consumable(source_target->type())) {
        build_nodes_t r(g_i->first->construct(*g_i->second,
                                              props,
                                              build_nodes_t(1, source_node),
                                              source_target,
                                              nullptr,
                                              owner));
        remove_dups(source_node->dependencies_);
        result->insert(result->end(), r.begin(), r.end());
        return true;
      }
      build_nodes_t this_result;
      if (transform(target_generator,
                    *g_i->first,
                    source_target,
                    source_node,
                    &this_result,
                    props,
                    owner)) {
        for (build_nodes_t::const_iterator r_i = this_result.begin(),
                                           r_last = this_result.end();
             r_i != r_last;
             ++r_i) {
          transform_to_consumable(
            target_generator, current_generator, *r_i, result, props, owner);
        }

        return true;
      }
    }
  }

  return false;
}

bool
generator_registry::transform_to_consumable(const generator& target_generator,
                                            const generator& current_generator,
                                            build_node_ptr source_node,
                                            build_nodes_t* result,
                                            const feature_set& props,
                                            const main_target& owner) const
{
  bool some_was_consumed = false;
  bool has_direct_consumable = false;
  for (build_node::targets_t::const_iterator
         i = source_node->products_.begin(),
         last = source_node->products_.end();
       i != last;
       ++i) {
    if (target_generator.is_consumable((**i).type()) ||
        current_generator.is_consumable((**i).type())) {
      if (!has_direct_consumable) {
        result->push_back(source_node);
        has_direct_consumable = true;
        some_was_consumed = true;
      }
    } else {
      bool res = transform(target_generator,
                           current_generator,
                           *i,
                           source_node,
                           result,
                           props,
                           owner);

      if (res) {
        some_was_consumed = true;
      }
    }
  }

  return some_was_consumed;
}

namespace {
struct generator_data
{
  explicit generator_data(const generator* g)
    : generator_(g)
    , all_consumed_(true)
  {
  }

  const generator* generator_;
  build_nodes_t transformed_sources_;
  bool all_consumed_;
};
} // namespace

build_nodes_t
generator_registry::construct(const main_target* mt) const
{
  using main_viable_generators_t = std::vector<generator_data>;
  viable_generators_t viable_generators(
    find_viable_generators(mt->type(), true, mt->properties()));
  main_viable_generators_t main_viable_generators;
  for (const viable_generators_t::value_type& g : viable_generators) {
    main_viable_generators.emplace_back(g.first);
  }

  if (main_viable_generators.empty()) {
    throw runtime_error("Can't find transformation to '" +
                        mt->type().tag().name() + "'.");
  }

  // generate target sources
  build_node::nodes_t generated_sources;
  for (auto i : mt->sources()) {
    build_nodes_t r((*i).generate());
    generated_sources.insert(generated_sources.end(), r.begin(), r.end());
  }

  // transform all sources using all viable generators
  for (build_node::nodes_t::const_iterator s = generated_sources.begin(),
                                           s_last = generated_sources.end();
       s != s_last;
       ++s) {
    for (auto& main_viable_generator : main_viable_generators) {
      main_viable_generator.all_consumed_ =
        main_viable_generator.all_consumed_ &&
        transform_to_consumable(*main_viable_generator.generator_,
                                *main_viable_generator.generator_,
                                *s,
                                &main_viable_generator.transformed_sources_,
                                mt->properties(),
                                *mt);
    }
  }

  // search for ONE good generator that consume all sources
  bool has_choosed_generator = false;
  main_viable_generators_t::const_iterator choosed_generator;
  for (main_viable_generators_t::const_iterator
         i = main_viable_generators.begin(),
         last = main_viable_generators.end();
       i != last;
       ++i) {
    // FIXME: error messages
    if (has_choosed_generator && i->all_consumed_) {
      throw runtime_error(
        "Found more than one transformations from sources to target.\n"
        "First  was '" +
        choosed_generator->generator_->name() + "', constraints: " +
        dump_for_hash(*choosed_generator->generator_->constraints(), true) +
        "\n"
        "Second was '" +
        i->generator_->name() + "', constraints: " +
        dump_for_hash(*i->generator_->constraints(), true));
    }

    if (i->all_consumed_) {
      choosed_generator = i;
      has_choosed_generator = true;
    }
  }

  // FIXME: error messages
  if (!has_choosed_generator) {
    throw runtime_error(
      (boost::format("Can't find transformation 'sources' -> '%s'.") %
       mt->type().tag().name())
        .str());
  }

  build_nodes_t r(choosed_generator->generator_->construct(
    mt->type(),
    mt->properties(),
    choosed_generator->transformed_sources_,
    nullptr,
    &mt->name(),
    *mt));
  if (!r.empty()) {
    return r;
  }

  // FIXME: error messages
  throw std::runtime_error("No viable generator found.");
}
} // namespace hammer
