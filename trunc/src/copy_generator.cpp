#include "stdafx.h"
#include "copy_generator.h"
#include "engine.h"
#include "type_registry.h"
#include <boost/assign/list_of.hpp>
#include "types.h"
#include "copy_main_target.h"
#include "copy_target.h"
#include "generated_target.h"
#include <set>
#include "np_helpers.h"
#include "types.h"
#include "build_action.h"
#include "type.h"
#include "build_environment.h"
#include "fs_helpers.h"
#include "product_argument_writer.h"

using namespace boost::assign;

namespace hammer{
namespace{
   
   class copy_action : public build_action
   {
      public:
         copy_action(const type& tag_type) 
            : build_action("copy file"),
              tag_writer_("", tag_type)
         {}

         virtual std::string target_tag(const build_node& node, const build_environment& environment) const;

      protected:
         virtual bool execute_impl(const build_node& node, const build_environment& environment) const;
         virtual void clean_on_fail(const build_node& node, const build_environment& environment) const;
      
      private:
         product_argument_writer tag_writer_;
   };

   std::string copy_action::target_tag(const build_node& node, const build_environment& environment) const
   {
      std::ostringstream s;
      tag_writer_.write(s, node, environment);
      return s.str();
   }

   bool copy_action::execute_impl(const build_node& node, const build_environment& environment) const
   {
      assert(node.products_.size() == 1);
      assert(node.sources_.size() == 1);
      assert(dynamic_cast<const copy_target*>(node.products_.front()) != NULL);

      location_t destination = node.products_.front()->location() / 
                               node.products_.front()->name().to_string();
      location_t source = node.sources_.front().source_target_->location() / 
                          node.sources_.front().source_target_->name().to_string();
      destination.normalize();
      source.normalize();
      destination = relative_path(destination, environment.current_directory());
      source = relative_path(source, environment.current_directory()); 
      environment.copy(source, destination);

      return true;
   }

   void copy_action::clean_on_fail(const build_node& node, const build_environment& environment) const
   {

   }
}

copy_generator::copy_generator(hammer::engine& e)
   : generator(e, "copy", consumable_types_t(),
                          list_of<produced_type>(e.get_type_registry().get(types::COPIED), 0),
                          true)
{
   std::auto_ptr<build_action> a(new copy_action(e.get_type_registry().get(types::COPIED)));
   action(a);
}

typedef std::vector<boost::intrusive_ptr<build_node> > nodes_t;
typedef std::set<const build_node*> visited_nodes_t;

static void collect_nodes(build_node::sources_t& result, 
                          visited_nodes_t& visited_nodes,
                          const nodes_t& sources, 
                          const copy_main_target::types_t& types_to_collect,
                          bool recursive)
{
   nodes_t new_sources;
   visited_nodes_t local_visited_nodes;

   for(nodes_t::const_iterator i = sources.begin(), last = sources.end(); i != last; ++i)
   {
      if (visited_nodes.find(i->get()) == visited_nodes.end())
      {
         local_visited_nodes.insert(i->get());
         for(build_node::sources_t::const_iterator s = (**i).sources_.begin(), s_last = (**i).sources_.end(); s != s_last; ++s)
            new_sources.push_back(s->source_node_);
      }
      else
         continue;

      for(build_node::targets_t::const_iterator p = (**i).products_.begin(), p_last = (**i).products_.end(); p != p_last; ++p)
      {
         copy_main_target::types_t::const_iterator t = std::find(types_to_collect.begin(), types_to_collect.end(), &(**p).type());
         if (t != types_to_collect.end())
            result.push_back(build_node::source_t(*p, *i));
      }
   }

   if (recursive && !new_sources.empty())
   {
      remove_dups(new_sources);
      visited_nodes.insert(local_visited_nodes.begin(), local_visited_nodes.end());
      collect_nodes(result, visited_nodes, new_sources, types_to_collect, recursive);
   }
}

nodes_t copy_generator::construct(const type& target_type, 
                                  const feature_set& props,
                                  const nodes_t& sources,
                                  const basic_target* t,
                                  const pstring* composite_target_name,
                                  const main_target& owner) const
{
   assert(dynamic_cast<const copy_main_target*>(&owner));
   assert(composite_target_name);

   const copy_main_target& true_owner = static_cast<const copy_main_target&>(owner);
   
   nodes_t result;
   build_node::sources_t collected_nodes;
   visited_nodes_t visited_nodes;
   collect_nodes(collected_nodes, visited_nodes, sources, 
                 true_owner.types_to_copy(), true_owner.recursive());

   result.reserve(collected_nodes.size());
   for(build_node::sources_t::const_iterator i = collected_nodes.begin(), last = collected_nodes.end(); i != last; ++i)
   {
      boost::intrusive_ptr<build_node> new_node(new build_node);
      new_node->targeting_type_ = &target_type;
      new_node->action(action());
      new_node->sources_.push_back(*i);

      pstring new_name = make_name(this->engine().pstring_pool(), *composite_target_name, i->source_target_->type());
      copy_target* new_target = new copy_target(&owner, new_name, &target_type, &props);
      new_node->products_.push_back(new_target);

      result.push_back(new_node);
   }

   return result;
}

}
