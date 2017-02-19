#include "stdafx.h"
#include "hammer_walker_impl.h"
#include <hammer/core/hammer_walker_context.h>
#include <hammer/core/engine.h>
#include <memory>
#include <vector>
#include <hammer/core/meta_target.h>
#include <hammer/core/type_registry.h>
#include <hammer/core/types.h>
#include <hammer/core/call_resolver.h>
#include <boost/ptr_container/ptr_vector.hpp>
#include <hammer/core/feature_set.h>
#include <hammer/core/feature_registry.h>
#include <hammer/core/feature.h>
#include <hammer/core/project_requirements_decl.h>
#include <hammer/core/sources_decl.h>
#include <antlr3commontoken.h>
#include <antlr3input.h>

namespace fs = boost::filesystem;

void* hammer_make_args_list(void* context)
{
   return new hammer::args_list_t();
}

void* hammer_rule_call(void* context, const char* rule_name, int local, void* args_list_in)
{
   std::auto_ptr<hammer::args_list_t> args_list(static_cast<hammer::args_list_t*>(args_list_in));
   hammer::hammer_walker_context* ctx = static_cast<hammer::hammer_walker_context*>(context);
   args_list->insert(args_list->begin(), new hammer::call_resolver_call_arg<hammer::project>(ctx->project_, false));

   if (local)
      ctx->project_->add_targets_as_local(true);

   std::auto_ptr<hammer::call_resolver_call_arg_base> result(ctx->call_resolver_->invoke(rule_name, *args_list));

   if (local)
      ctx->project_->add_targets_as_local(false);

   return result.release();
}

void hammer_delete_rule_result(void* result)
{
   delete static_cast<hammer::call_resolver_call_arg_base*>(result);
}

void* hammer_make_null_arg()
{
   return new hammer::call_resolver_call_arg<hammer::null_arg>(0, false);
}

void* hammer_make_string_list()
{
   return new call_resolver_call_arg<std::vector<std::string> >(new std::vector<std::string>(), true);
}

void* hammer_make_feature_list(void* context)
{
   hammer::hammer_walker_context* ctx = static_cast<hammer::hammer_walker_context*>(context);
   return new hammer::call_resolver_call_arg<hammer::feature_set>(new hammer::feature_set(&ctx->engine_->feature_registry()), false);
}

void* hammer_make_requirements_decl()
{
   return new hammer::requirements_decl;
}

void hammer_add_arg_to_args_list(void* args_list, void* arg)
{
   hammer::args_list_t* args_list_ = static_cast<hammer::args_list_t*>(args_list);
   hammer::call_resolver_call_arg_base* arg_ = static_cast<hammer::call_resolver_call_arg_base*>(arg);
   args_list_->push_back(arg_);
}

void hammer_add_id_to_string_list(void* context, void* string_list, const char* id)
{
   hammer::hammer_walker_context* ctx = static_cast<hammer::hammer_walker_context*>(context);
   hammer::call_resolver_call_arg<std::vector<std::string> >* string_list_ = static_cast<hammer::call_resolver_call_arg<std::vector<std::string> >*>(string_list);
   string_list_->value()->push_back(id);
}

void hammer_add_feature_to_list(void* context, void* args_list, const char* feature_name, const char* feature_value)
{
   //hammer::hammer_walker_context* ctx = static_cast<hammer::hammer_walker_context*>(context); // unused variable ctx
   hammer::call_resolver_call_arg<hammer::feature_set>* args_list_ = static_cast<hammer::call_resolver_call_arg<hammer::feature_set>*>(args_list);
   args_list_->value()->join(feature_name, feature_value);
}

void hammer_add_feature_argument(void* context, void* args_list, const char* feature_name, const char* feature_value)
{
   hammer::hammer_walker_context* ctx = static_cast<hammer::hammer_walker_context*>(context);
   hammer::args_list_t* args_list_ = static_cast<hammer::args_list_t*>(args_list);
   const hammer::feature_def* fdef = ctx->engine_->feature_registry().find_def_from_full_name(feature_name);
   hammer::call_resolver_call_arg<hammer::feature>* arg;
   if (fdef == NULL)
      arg = new hammer::call_resolver_call_arg<hammer::feature>(ctx->project_->local_feature_registry().create_feature(feature_name, feature_value), false);
   else
      arg = new hammer::call_resolver_call_arg<hammer::feature>(ctx->engine_->feature_registry().create_feature(feature_name, feature_value), false);

   args_list_->push_back(arg);
}

void hammer_add_string_arg_to_args_list(void* context, void* args_list, const char* id)
{
   hammer::hammer_walker_context* ctx = static_cast<hammer::hammer_walker_context*>(context);
   hammer::call_resolver_call_arg<string>* arg = new call_resolver_call_arg<std::string>(new std::string(id), false);
   hammer::args_list_t* args_list_ = static_cast<hammer::args_list_t*>(args_list);
   args_list_->push_back(arg);
}

void* hammer_create_feature(void* context, const char* feature_name, const char* feature_value)
{
   hammer::hammer_walker_context* ctx = static_cast<hammer::hammer_walker_context*>(context);
   const hammer::feature_def* fdef = ctx->engine_->feature_registry().find_def_from_full_name(feature_name);
   if (fdef != NULL)
      return ctx->engine_->feature_registry().create_feature(feature_name, feature_value == NULL ? "" : feature_value);
   else
      return ctx->project_->local_feature_registry().create_feature(feature_name, feature_value == NULL ? "" : feature_value);
}

void* hammer_make_requirements_condition()
{
   return new hammer::linear_and_condition;
}

void* hammer_make_project_requirements_decl(const char* id, void* rdecl)
{
   hammer::requirements_decl* r = static_cast<hammer::requirements_decl*>(rdecl);
   hammer::project_requirements_decl* result = new hammer::project_requirements_decl(id, *r);
   delete r;
   return result;
}

void* hammer_make_requirements_decl_arg(void* rdecl)
{
   hammer::requirements_decl* r = static_cast<hammer::requirements_decl*>(rdecl);
   return new hammer::call_resolver_call_arg<hammer::requirements_decl>(r, true);
}

void* hammer_make_project_requirements_decl_arg(void* pdecl)
{
   hammer::project_requirements_decl* p = static_cast<hammer::project_requirements_decl*>(pdecl);
   return new hammer::call_resolver_call_arg<hammer::project_requirements_decl>(p, true);
}

void hammer_add_conditional_to_rdecl(void* condition, char is_public, void* rdecl)
{
   hammer::requirements_decl* r = static_cast<hammer::requirements_decl*>(rdecl);
   std::auto_ptr<hammer::requirement_base> c(static_cast<hammer::linear_and_condition*>(condition));
   c->set_public(is_public);
   r->add(c);
}

void hammer_add_feature_to_rdecl(void* feature, char is_public, void* rdecl)
{
   hammer::requirements_decl* r = static_cast<hammer::requirements_decl*>(rdecl);
   std::auto_ptr<hammer::requirement_base> nr(new hammer::just_feature_requirement(static_cast<hammer::feature*>(feature)));
   nr->set_public(is_public);
   r->add(nr);
}

void hammer_set_condition_result(void* condition, void* feature)
{
   hammer::linear_and_condition* c = static_cast<hammer::linear_and_condition*>(condition);
   c->result(static_cast<hammer::feature*>(feature));
}

void hammer_add_feature_to_condition(void* feature, void* condition)
{
   hammer::linear_and_condition* c = static_cast<hammer::linear_and_condition*>(condition);
   c->add(static_cast<hammer::feature*>(feature));
}

void* hammer_make_sources_decl()
{
   return new hammer::sources_decl();
}

void hammer_add_source_to_sources_decl(void* result, void* sd)
{
   hammer::source_decl* source_decl = static_cast<hammer::source_decl*>(sd);
   static_cast<hammer::sources_decl*>(result)->push_back(*source_decl);
   delete source_decl;
}

void* hammer_make_sources_decl_arg(void* s)
{
   return new hammer::call_resolver_call_arg<hammer::sources_decl>(s, true);
}

void hammer_add_rule_result_to_source_decl(void* rule_result, void* sources)
{
   hammer::call_resolver_call_arg<hammer::sources_decl>* rr = static_cast<hammer::call_resolver_call_arg<hammer::sources_decl>*>(rule_result);
   static_cast<hammer::sources_decl*>(sources)->transfer_from(*rr->value());
}

void* hammer_make_path()
{
   return new fs::path();
}

void hammer_add_to_path(void* p, const char* token)
{
   fs::path* pd = static_cast<fs::path*>(p);
   *pd /= token;
}

void* hammer_make_path_arg(void* p)
{
   hammer::call_resolver_call_arg<fs::path>* result = new hammer::call_resolver_call_arg<fs::path>(static_cast<fs::path*>(p), true);
   return result;
}

void* hammer_make_feature_set_arg(void* fs)
{
   return new hammer::call_resolver_call_arg<hammer::feature_set>(fs, false);
}

void* hammer_make_feature_arg(void* f)
{
   return new hammer::call_resolver_call_arg<hammer::feature>(f, false);
}

void* hammer_make_source_decl()
{
   return new hammer::source_decl;
}

typedef std::pair<pANTLR3_COMMON_TOKEN, pANTLR3_COMMON_TOKEN> target_path_t;

void hammer_source_decl_set_target_path(void* context, void* sd, void* tp)
{
   hammer::hammer_walker_context* ctx = static_cast<hammer::hammer_walker_context*>(context);
   hammer::source_decl* source_decl = static_cast<hammer::source_decl*>(sd);
   target_path_t* target_path_tokens = static_cast<target_path_t*>(tp);

   if (target_path_tokens->second == NULL)
      target_path_tokens->second = target_path_tokens->first;

   pANTLR3_STRING s = target_path_tokens->first->input->substr(target_path_tokens->first->input,
                                                               target_path_tokens->first->start,
                                                               target_path_tokens->second->stop);
   std::string target_path(reinterpret_cast<const char*>(s->chars));
   source_decl->target_path(target_path, ctx->engine_->get_type_registry().resolve_from_target_name(target_path));

   delete target_path_tokens;
   s->factory->destroy(s->factory, s);
}

void hammer_source_decl_set_target_name(void* context, void* sd, const char* id)
{
   hammer::hammer_walker_context* ctx = static_cast<hammer::hammer_walker_context*>(context);
   hammer::source_decl* source_decl = static_cast<hammer::source_decl*>(sd);

   if (id != NULL)
      source_decl->target_name(id);
}

void hammer_source_decl_set_target_properties(void* sd, void* fs)
{
   hammer::source_decl* source_decl = static_cast<hammer::source_decl*>(sd);
   source_decl->properties(static_cast<hammer::feature_set*>(fs));
}

void hammer_source_decl_set_public(void* sd)
{
   hammer::source_decl* source_decl = static_cast<hammer::source_decl*>(sd);
   source_decl->set_public(true);
}

void* hammer_make_target_path()
{
   return new target_path_t();
}

void hammer_add_to_target_path(void* context, void* tp, pANTLR3_BASE_TREE node)
{
   //hammer::hammer_walker_context* ctx = static_cast<hammer::hammer_walker_context*>(context); // unused variable ctx
   target_path_t* target_path = static_cast<target_path_t*>(tp);

   if (target_path->first == NULL)
      target_path->first = node->getToken(node);
   else
      target_path->second = node->getToken(node);
}

void* hammer_make_feature_set(void* context)
{
   hammer::hammer_walker_context* ctx = static_cast<hammer::hammer_walker_context*>(context);
   return ctx->engine_->feature_registry().make_set();
}

void hammer_add_feature_to_feature_set(void* feature_set, void* feature)
{
   static_cast<hammer::feature_set*>(feature_set)->join(static_cast<hammer::feature*>(feature));
}

void hammer_feature_set_dependency_data(void* f, void* sd_)
{
   hammer::source_decl* sd = static_cast<hammer::source_decl*>(sd_);
   static_cast<hammer::feature*>(f)->set_dependency_data(*sd, nullptr);
   delete sd;
}

void hammer_on_nested_rule_enter(void* context)
{
   hammer::hammer_walker_context* ctx = static_cast<hammer::hammer_walker_context*>(context);
   ctx->project_->add_targets_as_explicit(true);
}

void hammer_on_nested_rule_leave(void* context)
{
   hammer::hammer_walker_context* ctx = static_cast<hammer::hammer_walker_context*>(context);
   ctx->project_->add_targets_as_explicit(false);
}
