#include "stdafx.h"
#include <hammer/core/basic_target.h>
#include <hammer/core/collect_nodes.h>
#include <hammer/core/generator.h>

using namespace std;

namespace hammer {

using nodes_t = std::vector<boost::intrusive_ptr<build_node>>;
using visited_nodes_t = std::set<const build_node*>;

void
collect_nodes(build_node::sources_t& result,
              std::set<const build_node*>& visited_nodes,
              const std::vector<boost::intrusive_ptr<build_node>>& sources,
              const std::vector<const target_type*>& types_to_collect,
              bool recursive)
{
  nodes_t new_sources;
  visited_nodes_t local_visited_nodes;

  for (const auto& source : sources) {
    if (visited_nodes.find(source.get()) == visited_nodes.end()) {
      local_visited_nodes.insert(source.get());
      for (build_node::sources_t::const_iterator
             s = (*source).sources_.begin(),
             s_last = (*source).sources_.end();
           s != s_last;
           ++s) {
        new_sources.push_back(s->source_node_);
      }

      new_sources.insert(new_sources.end(),
                         (*source).dependencies_.begin(),
                         (*source).dependencies_.end());
    } else {
      continue;
    }

    for (build_node::targets_t::const_iterator
           p = (*source).products_.begin(),
           p_last = (*source).products_.end();
         p != p_last;
         ++p) {
      auto t = std::find(
        types_to_collect.begin(), types_to_collect.end(), &(**p).type());
      if (t != types_to_collect.end()) {
        result.emplace_back(*p, source);
      }
    }
  }

  if (recursive && !new_sources.empty()) {
    remove_dups(new_sources);
    visited_nodes.insert(local_visited_nodes.begin(),
                         local_visited_nodes.end());
    collect_nodes(
      result, visited_nodes, new_sources, types_to_collect, recursive);
  }
}
} // namespace hammer
