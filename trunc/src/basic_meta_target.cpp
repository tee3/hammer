#include "stdafx.h"
#include "basic_meta_target.h"
#include "feature_set.h"
#include "feature.h"
#include "requirements_decl.h"
#include "type.h"
#include "project.h"
#include "engine.h"
#include "type_registry.h"
#include "source_target.h"
#include "main_target.h"

using namespace std;

namespace hammer{

basic_meta_target::basic_meta_target(hammer::project* p,
                                     const pstring& name, 
                                     const requirements_decl& req, 
                                     const requirements_decl& usage_req)
                                     : 
                                     project_(p),
                                     name_(name),
                                     requirements_(req),
                                     usage_requirements_(usage_req),
                                     is_explicit_(false)
{
   // FIXME: ����� ������� ���� ��� ����-�� � ������ �����, ��� ��� ��� �� ������������ �����, ����� ����� ���������� ������ ��� ���
   requirements_.setup_path_data(this);
   usage_requirements_.setup_path_data(this);
}

void basic_meta_target::insert(const pstring& source)
{
   sources_.push_back(source);
}

void basic_meta_target::sources(const sources_decl& s)
{
   sources_ = s;
}

const location_t& basic_meta_target::location() const 
{
   return project()->location();
}

void basic_meta_target::instantiate_simple_targets(const sources_decl& targets, 
                                                   const feature_set& build_request,
                                                   const main_target& owner, 
                                                   std::vector<basic_target*>* result) const
{
   const type_registry& tr = project_->engine()->get_type_registry();
   for(sources_decl::const_iterator i = targets.begin(), last = targets.end(); i != last; ++i)
   {
      const hammer::type* tp = i->type(tr);
      if (tp == 0)
         throw std::runtime_error("Can't resolve type from source '" + i->target_path_.to_string() + "'.");

      source_target* st = new(project_->engine()) source_target(&owner, i->target_path_, tp, &owner.properties());
      result->push_back(st);
   }
}

void basic_meta_target::instantiate_meta_targets(const meta_targets_t& targets, 
                                                 const feature_set& build_request,
                                                 const main_target* owner, 
                                                 std::vector<basic_target*>* result, 
                                                 feature_set* usage_requirments) const
{
   // FIXME: ���� � result ��� ���� ������������������� ���� �� ����� ��������� � ������ ����������� ��� ���� ��������
   // ���� ��� ��������� ������ �� ������ ���������� ���������������, ���� ���, �� ������ ����������
   for(meta_targets_t::const_iterator i = targets.begin(), last = targets.end(); i != last; ++i)
      i->first->instantiate(owner, i->second == NULL ? build_request : *build_request.join(*i->second), 
                            result, usage_requirments);
}

void basic_meta_target::split_one_source(sources_decl* simple_targets,
                                         meta_targets_t* meta_targets,
                                         const source_decl& source,
                                         const feature_set& build_request,
                                         const type_registry& tr) const
{
   if (const type* t = source.type(tr))
      simple_targets->push_back(source.target_path_); 
   else
      resolve_meta_target_source(source, build_request, simple_targets, meta_targets);
}

void basic_meta_target::split_sources(sources_decl* simple_targets, meta_targets_t* meta_targets,
                                      const sources_decl& sources, const feature_set& build_request) const
{
   const type_registry& tr = project_->engine()->get_type_registry();
   for(sources_decl::const_iterator i = sources.begin(), last = sources.end(); i != last; ++i)
      split_one_source(simple_targets, meta_targets, *i, build_request, tr);
}      

void basic_meta_target::resolve_meta_target_source(const source_decl& source,
                                                   const feature_set& build_request,
                                                   sources_decl* simple_targets,
                                                   meta_targets_t* meta_targets) const
{
   const feature_set* build_request_with_source_properties = (source.properties_ == NULL ? &build_request : build_request.join(*source.properties_));

	// check that source is simple one ID. Maybe its source or maybe its target ID.
   if (source.target_name_.empty() && 
       !source.target_path_.empty())
	{
		if (const basic_meta_target* m = project_->find_target(source.target_path_))
		{
         m = project_->select_best_alternative(source.target_path_, *build_request_with_source_properties);
         m->transfer_sources(simple_targets, meta_targets, 
                             *build_request_with_source_properties, source.properties_);
			meta_targets->push_back(make_pair(m, source.properties_));
			return;
		}
   }

	// source has target_name_ only when it was explicitly requested (./foo//bar) where target_name_ == "bar"
   const hammer::project& target_project = project_->engine()->load_project(source.target_path_.to_string(), *project_);
   if (source.target_name_.empty()) 
   {
      hammer::project::selected_targets_t selected_targets(target_project.select_best_alternative(*build_request_with_source_properties));
      for(hammer::project::selected_targets_t::const_iterator i = selected_targets.begin(), last = selected_targets.end(); i != last; ++i)
	   {
		   if (!(**i).is_explicit())
		   {
			   (**i).transfer_sources(simple_targets, meta_targets, 
                                   *build_request_with_source_properties, source.properties_);
			   meta_targets->push_back(make_pair(*i, source.properties_));
		   }
	   }
   }
   else
   {
      const basic_meta_target* m = target_project.select_best_alternative(source.target_name_, *build_request_with_source_properties);
      m->transfer_sources(simple_targets, meta_targets, 
                          *build_request_with_source_properties, source.properties_);
      meta_targets->push_back(make_pair(m, source.properties_));
      return;
   }
}

void basic_meta_target::transfer_sources(sources_decl* simple_targets, 
                                         meta_targets_t* meta_targets,
                                         const feature_set& build_request,
                                         const feature_set* additional_build_properties) const
{

}

basic_meta_target::~basic_meta_target()
{
}

}