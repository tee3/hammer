#include "stdafx.h"
#include <hammer/core/generator.h>
#include <hammer/core/type.h>
#include <hammer/core/basic_target.h>
#include <hammer/core/main_target.h>
#include <hammer/core/meta_target.h>
#include <hammer/core/generated_target.h>
#include <hammer/core/engine.h>
#include <hammer/core/np_helpers.h>
#include <hammer/core/build_action.h>

namespace hammer{

generator::generator(hammer::engine& e, 
                     const std::string& name,
                     const consumable_types_t& source_types,
                     const producable_types_t& target_types,
                     bool composite,
                     const feature_set* c) : 
   engine_(&e), name_(name), source_types_(source_types),
   target_types_(target_types),
   composite_(composite),
   constraints_(c),
   include_composite_generators_(false)
{

}

bool generator::is_consumable(const type& t) const
{
   if (consumable_types().empty())
      return true;

   for(generator::consumable_types_t::const_iterator i = consumable_types().begin(), last = consumable_types().end(); i != last; ++i)
      if (i->type_->equal_or_derived_from(t))
         return true;

   return false;
}
 
std::vector<boost::intrusive_ptr<build_node> >
generator::construct(const type& target_type, 
                     const feature_set& props,
                     const std::vector<boost::intrusive_ptr<build_node> >& sources,
                     const basic_target* t,
                     const pstring* composite_target_name,
                     const main_target& owner) const
{
   if (!t)
   {
      boost::intrusive_ptr<build_node> result(new build_node);
      result->action(action());

      typedef std::vector<boost::intrusive_ptr<build_node> >::const_iterator iter;
      for(iter i = sources.begin(), last = sources.end(); i != last; ++i)
      {
         bool node_added = false;
         for(build_node::targets_t::const_iterator p_i = (**i).products_.begin(), p_last = (**i).products_.end(); p_i != p_last; ++p_i)
         {
            if (is_consumable((**p_i).type()))
            {
               result->sources_.push_back(build_node::source_t(*p_i, *i));
               if (!node_added)
               {
                  result->down_.push_back(*i);
                  node_added = true;
               }
            }
         }
      }

      for(producable_types_t::const_iterator i = target_types_.begin(), last = target_types_.end(); i != last; ++i)
      {
         pstring new_name = make_name(engine_->pstring_pool(), 
                                      *composite_target_name, 
                                      *i->type_,
                                      i->need_tag_ ? &props : NULL);
         result->products_.push_back(new generated_target(&owner, 
                                                          new_name, 
                                                          i->type_, &props));
      }

      result->targeting_type_ = &target_type;
      return std::vector<boost::intrusive_ptr<build_node> >(1, result);
   }
   else
   {
      pstring new_name = make_name(engine_->pstring_pool(), 
                                   t->name(), 
                                   t->type(), 
                                   target_type, 
                                   producable_types().front().need_tag_ ? &props : NULL);
      assert(sources.size() == 1);
      
      boost::intrusive_ptr<build_node> result(new build_node);
      result->action(action());

      result->sources_.push_back(build_node::source_t(t, sources.front()));
      result->down_.push_back(sources.front());
      result->products_.push_back(new generated_target(&owner, new_name, producable_types().front().type_, &props));
      result->targeting_type_ = &target_type;
      return std::vector<boost::intrusive_ptr<build_node> >(1, result);
  }
}

generator::~generator()
{

}

static bool less_by_addr(const boost::intrusive_ptr<build_node>& lhs,
                         const boost::intrusive_ptr<build_node>& rhs)
{
   return lhs.get() < rhs.get();
}

static bool equal_by_addr(const boost::intrusive_ptr<build_node>& lhs,
                          const boost::intrusive_ptr<build_node>& rhs)
{
   return lhs.get() == rhs.get();
}

void remove_dups(std::vector<boost::intrusive_ptr<build_node> >& nodes)
{
   std::sort(nodes.begin(), nodes.end(), &less_by_addr);
   nodes.erase(std::unique(nodes.begin(), nodes.end(), &equal_by_addr), nodes.end());
}

}