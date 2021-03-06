#include "stdafx.h"
#include <hammer/core/basic_meta_target.h>
#include <hammer/core/feature_set.h>
#include <hammer/core/feature.h>
#include <hammer/core/requirements_decl.h>
#include <hammer/core/target_type.h>
#include <hammer/core/project.h>
#include <hammer/core/engine.h>
#include <hammer/core/type_registry.h>
#include <hammer/core/source_target.h>
#include <hammer/core/main_target.h>
#include <hammer/core/feature_registry.h>
#include <hammer/core/fs_helpers.h>

using namespace std;

namespace hammer{

basic_meta_target::basic_meta_target(hammer::project* p,
                                     const std::string& name,
                                     const requirements_decl& req,
                                     const requirements_decl& usage_req)
                                     :
                                     project_(p),
                                     name_(name),
                                     requirements_(req),
                                     usage_requirements_(usage_req),
                                     is_explicit_(false)
{
   requirements_.setup_path_data(p);
   usage_requirements_.setup_path_data(p);
}

void basic_meta_target::sources(const sources_decl& s)
{
   sources_ = s;
}

void basic_meta_target::add_sources(const sources_decl& s)
{
   sources_.insert(s);
}

const location_t& basic_meta_target::location() const
{
   return get_project().location();
}

void basic_meta_target::instantiate_simple_targets(const sources_decl& targets,
                                                   const feature_set& build_request,
                                                   const main_target& owner,
                                                   std::vector<basic_target*>* result) const
{
   for (const source_decl& sd : targets) {
      const hammer::target_type* tp = sd.type();
      if (!tp)
         throw std::runtime_error("Can't resolve type from source '" + sd.target_path() + "'.");

      location_t source_location = (owner.location() / sd.target_path()).normalize();
      basic_target* st = create_simple_target(owner, source_location, *tp, &owner.properties());
      result->push_back(st);
   }
}

basic_target*
basic_meta_target::create_simple_target(const main_target& owner,
                                        const location_t& source_location,
                                        const target_type& tp,
                                        const feature_set* properties) const
{
   return new source_target(&owner, source_location.branch_path(), source_location.filename().string(), &tp, properties);
}

void instantiate_meta_targets(const meta_targets_t& targets,
                              const feature_set& build_request,
                              const main_target* owner,
                              std::vector<basic_target*>* result,
                              feature_set* usage_requirments)
{
   for (auto& t : targets) {
      t.first->instantiate(owner, t.second == NULL ? build_request : *build_request.join(*t.second),
                           result, usage_requirments);
   }
}

void basic_meta_target::split_one_source(sources_decl* simple_targets,
                                         meta_targets_t* meta_targets,
                                         const source_decl& source,
                                         const feature_set& build_request,
                                         const type_registry& tr) const
{
   if (source.type())
      simple_targets->push_back(source);
   else
      resolve_meta_target_source(source, build_request, meta_targets);
}

void basic_meta_target::split_sources(sources_decl* simple_targets, meta_targets_t* meta_targets,
                                      const sources_decl& sources, const feature_set& build_request) const
{
   const type_registry& tr = get_engine().get_type_registry();
   for(sources_decl::const_iterator i = sources.begin(), last = sources.end(); i != last; ++i)
      split_one_source(simple_targets, meta_targets, *i, build_request, tr);
}

// trying to avoid feature_set conning if possible
static
const feature_set*
apply_project_dependencies(const feature_set* properties,
                           const std::string& target_ref,
                           const project& p ) {
   for (auto& d : p.dependencies()) {
      if (boost::regex_match(target_ref, d.target_ref_mask_)) {
         feature_set* result = properties->clone();
         result->join(*d.properties_);
         return result;
      }
   }

   return properties;
}

static
loaded_projects
resolve_project_local_reference(const source_decl& s,
                                const project& current_project) {
   const project& top = [&] () -> const project& {
      auto result = &s.owner_project();
      while(result && !result->publishable())
         result = result->parent_;

      if (!result)
         throw std::runtime_error("While resolving project local reference '" + s.target_path() + "' at '" + current_project.location().string() + "' - can't find parent publishable project");

      return *result;
   }();

   if (s.target_path().size() - 1 < top.name().size() ||
       !std::equal(top.name().begin(), top.name().end(), s.target_path().begin() + 1))
   {
      throw std::runtime_error("Can't resolve project local reference '" + s.target_path() + "'. Top project id is '" + top.name() + "'");
   }

   // in case we specify ^projectID we have to return only projects from projectID itself
   // otherwise path looks like ^projectID/some_other_path and we need to load only some_other_path
   if (top.name().size() == s.target_path().size() - 1)
      return top.load_project({});
   else
      return top.load_project(s.target_path().substr(top.name().size() + 2));
}

void basic_meta_target::resolve_meta_target_source(const source_decl& source,
                                                   const feature_set& build_request,
                                                   meta_targets_t* meta_targets) const
{
   const feature_set* build_request_with_source_properties = (source.properties() == NULL ? &build_request : build_request.join(*source.properties()));

   if (looks_like_local_target_ref(source)) {
      const std::string target_name = source.target_name().empty() ? source.target_path() : source.target_name();
      if (project_->find_target(target_name)) {
         project::selected_target selected_target = project_->select_best_alternative(target_name, *build_request_with_source_properties, /*allow_locals=*/true);
         meta_targets->push_back({selected_target.target_, source.properties()});
         return;
		}
   }

   // source has target_name_ only when it was explicitly requested (./foo//bar) where target_name_ == "bar"
   loaded_projects suitable_projects =
      source.is_project_local_reference() ? resolve_project_local_reference(source, get_project())
                                          : get_project().load_project(source.target_path());

   build_request_with_source_properties = apply_project_dependencies(build_request_with_source_properties, source.target_path(), get_project());

   if (source.target_name().empty()) {
      try {
         hammer::project::selected_targets_t selected_targets(suitable_projects.select_best_alternative(*build_request_with_source_properties));
         for(hammer::project::selected_targets_t::const_iterator i = selected_targets.begin(), last = selected_targets.end(); i != last; ++i)
            meta_targets->push_back({i->target_, source.properties()});
      } catch(const std::exception& e) {
         throw std::runtime_error("While resolving meta target '" + source.target_path() +
                                  "' at '" + location().string() + "\n" + e.what());
      }
   } else {
      project::selected_target selected_target = suitable_projects.select_best_alternative(source.target_name(),
                                                                                           *build_request_with_source_properties,
                                                                                           source.locals_allowed());
      meta_targets->push_back({selected_target.target_, source.properties()});
   }
}

basic_meta_target::~basic_meta_target()
{
}

const feature_set& basic_meta_target::resolve_undefined_features(const feature_set& fs) const
{
   const feature_set* without_undefined = fs.has_undefined_features()
                                             ? get_project().try_resolve_local_features(fs)
                                             : &fs;
   if (without_undefined->has_undefined_features())
      throw std::runtime_error("Target '" + name() + "' at location '" +
                               location().string() + "' has been instantiated with unknown features");

   return *without_undefined;
}

void basic_meta_target::instantiate(const main_target* owner,
                                    const feature_set& build_request,
                                    std::vector<basic_target*>* result,
                                    feature_set* usage_requirements) const
{
   if (is_cachable(owner)) {
      for (auto& i : instantiation_cache_)
         if (i.build_request_->compatible_with(build_request)) {
            result->insert(result->end(), i.instantiated_targets_.begin(), i.instantiated_targets_.end());
            usage_requirements->join(*i.computed_usage_requirements_);
            return;
         }

      cached_instantiation_data_t cache_item;
      cache_item.build_request_ = &build_request;
      cache_item.computed_usage_requirements_ = get_engine().feature_registry().make_set();

      instantiate_impl(owner,
                       build_request,
                       &cache_item.instantiated_targets_,
                       cache_item.computed_usage_requirements_);
      instantiation_cache_.push_back(cache_item);

      result->insert(result->end(), cache_item.instantiated_targets_.begin(), cache_item.instantiated_targets_.end());
      usage_requirements->join(*cache_item.computed_usage_requirements_);

      return;
   } else {
      instantiate_impl(owner,
                       build_request,
                       result,
                       usage_requirements);
   }
}

engine& basic_meta_target::get_engine() const
{
   return project_->get_engine();
}

static
bool has_slash(const std::string& s)
{
   for (const char c : s)
      if (c == '/')
         return true;

   return false;
}

void adjust_dependency_features_sources(feature_set& set_to_adjust,
                                        const basic_meta_target& relative_to_target)
{
   for (feature* f : set_to_adjust) {
      if (!f->attributes().dependency)
         continue;

      const source_decl& source = f->get_dependency_data().source_;
      if (source.type() == nullptr /*source is meta-target*/ &&
          !has_slash(source.target_path()) &&
          !source.is_project_local_reference())
      {
         source_decl adjusted_source = source;
         adjusted_source.target_path("./", nullptr);
         adjusted_source.target_name(source.target_path());

         f->set_dependency_data(adjusted_source, &relative_to_target.get_project());
      }
   }
}

}
