#include "stdafx.h"
#include <iostream>
#include <hammer/core/engine.h>
#include <hammer/core/hammer_walker_context.h>
#include <hammer/core/type_registry.h>
#include <hammer/core/types.h>
#include <hammer/core/basic_target.h>
#include <hammer/core/feature_registry.h>
#include <hammer/core/feature_set.h>
#include <hammer/core/feature.h>
#include <hammer/core/parser.h>
#include <boost/bind.hpp>
#include <hammer/core/obj_meta_target.h>
#include <hammer/core/lib_meta_target.h>
#include <hammer/core/typed_meta_target.h>
#include <hammer/core/alias_meta_target.h>
#include <hammer/core/version_alias_meta_target.h>
#include <hammer/core/target_version_alias_meta_target.h>
#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/format.hpp>
#include <hammer/core/generator_registry.h>
#include <hammer/core/project_requirements_decl.h>
#include "wildcard.hpp"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/regex.hpp>
#include <hammer/core/fs_helpers.h>
#include <hammer/core/header_lib_meta_target.h>
#include <hammer/core/pch_meta_target.h>
#include <hammer/core/copy_meta_target.h>
#include <hammer/core/testing_meta_target.h>
#include <hammer/core/testing_intermediate_meta_target.h>
#include <hammer/core/testing_compile_fail_meta_target.h>
#include <hammer/core/toolset_manager.h>
#include <hammer/core/scaner_manager.h>
#include <hammer/core/default_output_location_strategy.h>
#include <hammer/core/prebuilt_lib_meta_target.h>
#include <hammer/core/file_meta_target.h>
#include <hammer/core/warehouse_impl.h>
#include "builtin_features.h"

// v2
#include <hammer/parser/parser.h>
#include <hammer/sema/actions_impl.h>
#include <hammer/core/diagnostic.h>
#include <hammer/ast/context.h>
#include <hammer/core/rule_manager.h>
#include <hammer/core/ast2objects.h>
#include "builtin_rules.h"

using namespace std;
namespace fs = boost::filesystem;
using namespace boost::assign;

namespace hammer{

engine::engine()
   :  feature_registry_(0),
      rule_manager_(new rule_manager)

{
   auto_ptr<hammer::feature_registry> fr(new hammer::feature_registry());

   resolver_.insert("project", boost::function<void (project*, vector<string>&, project_requirements_decl*, project_requirements_decl*)>(boost::bind(&engine::project_rule, this, _1, _2, _3, _4)));
   resolver_.insert("lib", boost::function<void (project*, vector<string>&, sources_decl*, requirements_decl*, feature_set*, requirements_decl*)>(boost::bind(&engine::lib_rule, this, _1, _2, _3, _4, _5, _6)));
   resolver_.insert("searched-shared-lib", boost::function<void (project*, vector<string>&, sources_decl*, string&, requirements_decl*, requirements_decl*)>(boost::bind(&engine::searched_shared_lib_rule, this, _1, _2, _3, _4, _5, _6)));
   resolver_.insert("searched-static-lib", boost::function<void (project*, string&, sources_decl*, string&, requirements_decl*, requirements_decl*)>(boost::bind(&engine::searched_static_lib_rule, this, _1, _2, _3, _4, _5, _6)));
   resolver_.insert("prebuilt-lib", boost::function<void (project*, string&, sources_decl*, string&, requirements_decl*, requirements_decl*)>(boost::bind(&engine::prebuilt_lib_rule, this, _1, _2, _3, _4, _5, _6)));
   resolver_.insert("file", boost::function<void (project*, vector<string>&, string&, requirements_decl*, requirements_decl*)>(boost::bind(&engine::file_rule, this, _1, _2, _3, _4, _5)));
   resolver_.insert("header-lib", boost::function<void (project*, vector<string>&, sources_decl*, requirements_decl*, feature_set*, requirements_decl*)>(boost::bind(&engine::header_lib_rule, this, _1, _2, _3, _4, _5, _6)));
   resolver_.insert("exe", boost::function<void (project*, vector<string>&, sources_decl&, requirements_decl*, feature_set*, requirements_decl*)>(boost::bind(&engine::exe_rule, this, _1, _2, _3, _4, _5, _6)));
   resolver_.insert("obj", boost::function<void (project*, string&, sources_decl&, requirements_decl*, feature_set*, requirements_decl*)>(boost::bind(&engine::obj_rule, this, _1, _2, _3, _4, _5, _6)));
   resolver_.insert("pch", boost::function<void (project*, string&, sources_decl&, requirements_decl*, feature_set*, requirements_decl*)>(boost::bind(&engine::pch_rule, this, _1, _2, _3, _4, _5, _6)));
   resolver_.insert("copy", boost::function<void (project*, string&, sources_decl&, requirements_decl*, feature_set*, requirements_decl*)>(boost::bind(&engine::copy_rule, this, _1, _2, _3, _4, _5, _6)));
   resolver_.insert("alias", boost::function<void (project*, string&, sources_decl*, requirements_decl*, feature_set*, requirements_decl*)>(boost::bind(&engine::alias_rule, this, _1, _2, _3, _4, _5, _6)));
   resolver_.insert("version-alias", boost::function<void (project*, string&, string&, const string*)>(boost::bind(&engine::version_alias_rule, this, _1, _2, _3, _4)));
   resolver_.insert("target-version-alias", boost::function<void (project*, string&, string&, const string*)>(boost::bind(&engine::target_version_alias_rule, this, _1, _2, _3, _4)));
   resolver_.insert("test-suite", boost::function<void (project*, string&, sources_decl&, sources_decl*)>(boost::bind(&engine::test_suite_rule, this, _1, _2, _3, _4)));
   resolver_.insert("testing.run", boost::function<sources_decl (project*, sources_decl*, std::vector<string>*, std::vector<string>*, requirements_decl*, string*)>(boost::bind(&engine::testing_run_rule, this, _1, _2, _3, _4, _5, _6)));
   resolver_.insert("testing.compile-fail", boost::function<sources_decl (project*, const sources_decl&, requirements_decl*, requirements_decl*, requirements_decl*)>(boost::bind(&engine::testing_compile_fail_rule, this, _1, _2, _3, _4, _5)));
   resolver_.insert("import", boost::function<void (project*, vector<string>&)>(boost::bind(&engine::import_rule, this, _1, _2)));
   resolver_.insert("feature.feature", boost::function<void (project*, vector<string>&, vector<string>*, vector<string>*)>(boost::bind(&engine::feature_feature_rule, this, _1, _2, _3, _4)));
   resolver_.insert("feature.subfeature", boost::function<void (project*, string&, string&)>(boost::bind(&engine::feature_subfeature_rule, this, _1, _2, _3)));
   resolver_.insert("feature.local", boost::function<void (project*, vector<string>&, vector<string>*, vector<string>*)>(boost::bind(&engine::feature_local_rule, this, _1, _2, _3, _4)));
   resolver_.insert("feature.compose", boost::function<void (project*, feature&, feature_set&)>(boost::bind(&engine::feature_compose_rule, this, _1, _2, _3)));
   resolver_.insert("variant", boost::function<void (project*, string&, string*, feature_set&)>(boost::bind(&engine::variant_rule, this, _1, _2, _3, _4)));
   resolver_.insert("glob", boost::function<sources_decl (project*, std::vector<string>&, std::vector<string>*)>(boost::bind(&engine::glob_rule, this, _1, _2, _3, false)));
   resolver_.insert("rglob", boost::function<sources_decl (project*, std::vector<string>&, std::vector<string>*)>(boost::bind(&engine::glob_rule, this, _1, _2, _3, true)));
   resolver_.insert("explicit", boost::function<void (project*, const string&)>(boost::bind(&engine::explicit_rule, this, _1, _2)));
   resolver_.insert("use-project", boost::function<void (project*, const string&, const string&, feature_set*)>(boost::bind(&engine::use_project_rule, this, _1, _2, _3, _4)));
   resolver_.insert("setup-warehouse", boost::function<void (project*, const string&, const string&, const string*)>(boost::bind(&engine::setup_warehouse_rule, this, _1, _2, _3, _4)));

   details::install_builtin_rules(*rule_manager_);

   {
      feature_attributes ft = {0}; ft.free = 1;
      fr->add_feature_def("__searched_lib_name", vector<string>(), ft);
   }

   {
      // used to mark targets that belong to pch meta target. Needed for distinguishing PCH and OBJ generators
      feature_attributes ft = {0}; ft.free = 1;
      fr->add_feature_def("__pch", vector<string>(), ft);
   }

   {
      feature_attributes ft = {0};
      ft.propagated = true;
      fr->add_feature_def("host-os", list_of("windows")("linux"), ft);
   }

#if defined(_WIN32)
   fr->get_def("host-os").set_default("windows");
#else
   fr->get_def("host-os").set_default("linux");
#endif

   feature_registry_ = fr.release();

   load_hammer_script(g_builtin_features, "builtin_features");

   switch(sizeof(nullptr)) {
      case 4:
         feature_registry_->get_def("address-model").set_default("32");
         break;
      case 8:
         feature_registry_->get_def("address-model").set_default("64");
         break;
   }

   type_registry_.reset(new type_registry);

   generators_.reset(new generator_registry);

   toolset_manager_.reset(new hammer::toolset_manager);
   scanner_manager_.reset(new hammer::scanner_manager);
   output_location_strategy_.reset(new default_output_location_strategy);
}

project* engine::get_upper_project(const location_t& project_path)
{
   // FIXME: BUG: boost parent_path() on "foo/bar/.  produce "foo/bar" instead of "foo"
   location_t upper_path = project_path.filename() == "." ? project_path.parent_path().parent_path() : project_path.parent_path();
   // FIXME: BUG: boost parent_path() can produce "E:" path and than "E:" / "foo" give as "E:foo" which is wrong
   if (upper_path.empty() || upper_path == project_path.root_name())
      return NULL;

   if (exists(upper_path / "hamfile"))
      return &load_project(upper_path);

   if (exists(upper_path / "hamroot"))
      return &load_project(upper_path);

   if (upper_path.has_parent_path())
      return get_upper_project(upper_path);
   else
      return NULL;
}

static location_t make_location(location_t::const_iterator first, location_t::const_iterator last)
{
   location_t result;
   for(; first != last; ++first)
      result /= *first;

   return result;
}

void engine::resolve_project_alias(resolved_project_symlinks_t& symlinks,
                                   location_t::const_iterator first, location_t::const_iterator last,
                                   global_project_links_t& symlink_storage)
{
   if (first == last)
      return;

   global_project_links_t::iterator i = symlink_storage.find(*first);
   if (i != symlink_storage.end())
   {
      ++first; // eat head and leave tail
      resolve_project_alias(symlinks, first, last, i->second->project_symlinks_);
      // We walk to see most longer alias and returns alias data for founded.
      // Only from one node.
      if (symlinks.empty() && !i->second->aliases_data_.empty())
         symlinks.push_back(resolved_project_symlink_t(make_location(first, last), i->second->aliases_data_));
   }
}

void engine::resolve_project_alias(resolved_project_symlinks_t& symlinks,
                                   const location_t& project_symlink)
{
   // ���������� ��������� ����
   resolve_project_alias(symlinks, ++project_symlink.begin(), project_symlink.end(), global_project_links_);
}

engine::loaded_projects_t
engine::load_project(location_t project_path,
                     const project& from_project)
{
   try {
      loaded_projects_t result(try_load_project(project_path, from_project));
      if (result.empty())
         throw std::runtime_error((boost::format("%s(0): error: can't load project '%s'")
                                      % from_project.location().string()
                                      % project_path).str());
      return result;
   } catch(const std::exception& e) {
      // FIXME: this is wrong approach, but I need to know where was a problem
      cerr << "While loading project '" << project_path << "'\n";
      throw;
   }
}

engine::loaded_projects_t
engine::try_load_project(location_t project_path,
                         const project& from_project)
{
   if (project_path.has_root_path()) {
      while(true) {
         loaded_projects_t result;
         resolved_project_symlinks_t symlinks;
         resolve_project_alias(symlinks, project_path);
         for(resolved_project_symlinks_t::const_iterator i = symlinks.begin(), last = symlinks.end(); i != last; ++i)
            for(project_alias_node::aliases_data_t::const_iterator j = i->symlinks_data_->begin(), j_last = i->symlinks_data_->end(); j != j_last; ++j)
               result += try_load_project(i->tail_, *j);

         if (warehouse_ && warehouse_->has_project(project_path, string())) {
            // If we can find project in warehouse than we need to add alternatives to resulting set of projects
            const project* materialized_warehouse_project = nullptr;
            for (const project* p : result) {
               if (warehouse_->project_from_warehouse(*p)) {
                  materialized_warehouse_project = p;
                  break;
               }
            }

            if (!materialized_warehouse_project) {
               boost::shared_ptr<project> not_yet_materialized_versions = warehouse_->load_project(*this, project_path);
               // we need to check if we already inserted this project before. When we doing warehouse::download_and_install we check
               // if project know to engine and that trigger loading project again, but engine may already manadge this project loaded before
               auto i = projects_.find(not_yet_materialized_versions->location());
               if (i == projects_.end()) {
                  // FIXME: Maybe we shouldn't add warehouse projects into engine, and manages them by warehouse
                  projects_.insert({not_yet_materialized_versions->location(), not_yet_materialized_versions});
                  result.push_back(not_yet_materialized_versions.get());
               } else
                  result.push_back(i->second.get());
            }
         } else
            return result;

         if (!result.empty())
            return result;
      }
   } else {
      location_t resolved_use_path, tail_path;
      resolve_use_project(resolved_use_path, tail_path,
                          from_project, project_path);
      location_t next_load_path(from_project.location() / resolved_use_path);
      next_load_path.normalize();
      if (tail_path.empty())
         return try_load_project(next_load_path);
      else
         return try_load_project(tail_path, load_project(next_load_path));
   }
}

engine::loaded_projects_t
engine::try_load_project(const location_t& tail_path,
                         const project_alias_data& symlink)
{
   project& p = load_project(symlink.location_);

   if (!tail_path.empty())
      return try_load_project(tail_path, p);
   else
      return loaded_projects_t(&p);
}

void engine::resolve_use_project(location_t& resolved_use_path, location_t& tail_path,
                                 const hammer::project& p, const location_t& path_to_resolve)
{
   use_project_data_t::const_iterator i = use_project_data_.find(&p);
   if (i == use_project_data_.end())
   {
      resolved_use_path = path_to_resolve;
      return;
   }

   use_project_data_t::mapped_type::const_iterator largets_use_iter = i->second.find(*path_to_resolve.begin());
   if (largets_use_iter == i->second.end())
   {
      resolved_use_path = path_to_resolve;
      return;
   }
   location_t larget_use_path = *path_to_resolve.begin();
   location_t::const_iterator to_resolve_begin = ++path_to_resolve.begin(),
                              to_resolve_end = path_to_resolve.end();

   for(;to_resolve_begin != to_resolve_end; ++to_resolve_begin)
   {
      location_t to_resolve = larget_use_path / *to_resolve_begin;
      use_project_data_t::mapped_type::const_iterator next_resolve_iter = i->second.find(to_resolve);
      if (next_resolve_iter == i->second.end())
         break;

      larget_use_path = to_resolve;
      largets_use_iter = next_resolve_iter;
   }

   resolved_use_path = largets_use_iter->second;

   // FIXME: stupid boost::filesystem::path can't be constructed from two iterators
   for(;to_resolve_begin != to_resolve_end; ++to_resolve_begin)
      tail_path /= *to_resolve_begin;
}

project& engine::load_project(location_t project_path)
{
   loaded_projects_t result(try_load_project(project_path));
   if (result.empty())
      throw  runtime_error("Can't load project at '"  + project_path.string() + ": no such path.");

   return result.front();
}

void engine::load_hammer_script(location_t filepath)
{
   filepath.normalize();
   if (!exists(filepath))
      return throw std::runtime_error("Hammer script '" + filepath.string() + "' doesn't exists.");

   projects_t::iterator i = projects_.find(filepath);
   if (i != projects_.end())
      throw std::runtime_error("Hammer script '" + filepath.string() + "' already loaded.");

   hammer_walker_context ctx;

   try
   {
      ctx.engine_ = this;
      ctx.location_ = filepath;
      ctx.project_ = new project(this);
      ctx.project_->location(filepath);
      ctx.call_resolver_ = &resolver_;

      parser p(this);
      if (!p.parse(filepath.string().c_str()))
         throw  runtime_error("Can't load script at '"  + filepath.string() + ": parser errors");

      p.walk(&ctx);
      assert(ctx.project_);
      insert(ctx.project_);
   }
   catch(...)
   {
      delete ctx.project_;
      throw;
   }
}

void engine::load_hammer_script(const string& script_body,
                                const string& script_name)
{
   hammer_walker_context ctx;

   try {
      ctx.engine_ = this;
      ctx.location_ = script_name;
      ctx.project_ = new project(this);
      ctx.project_->location(script_name);
      ctx.call_resolver_ = &resolver_;

      parser p(this);
      if (!p.parse_raw_script(script_body, script_name))
         throw  runtime_error("Can't parse raw script '" + script_name + "': parser errors");

      p.walk(&ctx);
      assert(ctx.project_);
      insert(ctx.project_);
   } catch(const std::exception& e) {
      delete ctx.project_;
      throw std::runtime_error("Failed to load '" + script_name + "' raw script: " + e.what());
   }
}

std::unique_ptr<project>
engine::load_project_v2(const location_t& project_path,
                        const project* upper_project)
{
   ostringstream s;
   streamed_diagnostic diag(project_path.native(), s);
   ast::context ast_ctx;
   sema::actions_impl actions(ast_ctx, *rule_manager_, diag);
   ast_hamfile_ptr ast = parse_hammer_script(project_path, actions);

   if (diag.error_count())
      throw std::runtime_error("Parse errors: " + s.str());

   std::unique_ptr<project> loaded_project(new project(this));
   loaded_project->location(project_path.branch_path());
   if (upper_project) {
      loaded_project->requirements().insert_infront(upper_project->requirements());
      loaded_project->usage_requirements().insert_infront(upper_project->usage_requirements());
   }

   invocation_context invc_ctx = { *loaded_project, diag, *rule_manager_ };

   ast2objects(invc_ctx, *ast);

   return loaded_project;
}

static
bool is_hamfile_v2(const location_t& filepath)
{
   boost::filesystem::ifstream f(filepath);
   string first_line;
   getline(f, first_line);

   return boost::regex_match(first_line, boost::regex("^#pragma\\s+parser\\s+v2\\s*$"));
}

engine::loaded_projects_t engine::try_load_project(location_t project_path)
{
   location_t path_with_dot(project_path);
   path_with_dot /= ".";
   path_with_dot.normalize();

   projects_t::iterator i = projects_.find(path_with_dot);
   if (i != projects_.end())
      return loaded_projects_t(i->second.get());

   hammer_walker_context ctx;
   try
   {
      project_path.normalize();
      ctx.engine_ = this;
      ctx.location_ = project_path;
      ctx.project_ = new project(this);
      ctx.project_->location(path_with_dot);
      ctx.call_resolver_ = &resolver_;
      project* upper_project = NULL;

      if (!exists(project_path))
      {
         upper_project = get_upper_project(project_path);
         if (upper_project == NULL)
            return loaded_projects_t();
      }

      location_t project_file = project_path / "hamfile";
      bool is_top_level = false;
      if (!exists(project_file))
      {
         project_file = project_path / "hamroot";
         is_top_level = true;
      }
      else
         if (upper_project == NULL)
            upper_project = get_upper_project(project_path);

      if (!exists(project_file))
         return loaded_projects_t();

      if (is_hamfile_v2(project_file)) {
         std::unique_ptr<project> loaded_project = load_project_v2(project_file, is_top_level ? nullptr : upper_project);
         if (is_top_level)
            loaded_project->set_root(true);

         insert(loaded_project.get());
         project* p = loaded_project.release();

         return loaded_projects_t(p);
      } else {
         parser p(this);
         if (!p.parse(project_file.string().c_str()))
            throw  runtime_error("Can't load project at '"  + project_path.string() + ": parser errors");

         // �� ���� ��� �� ������ ������� ��� ������ ����� ��� ����� ��� ��������� �����������
         // � ��������� �� ��� ����� �������
         if (!is_top_level) {
            if (upper_project) {
               ctx.project_->requirements().insert_infront(upper_project->requirements());
               ctx.project_->usage_requirements().insert_infront(upper_project->usage_requirements());
            }
         } else
            ctx.project_->set_root(true);

         p.walk(&ctx);
         assert(ctx.project_);
         insert(ctx.project_);

         return loaded_projects_t(ctx.project_);
      }
   }
   catch(...)
   {
      delete ctx.project_;
      throw;
   }
}

void engine::insert(project* p)
{
   projects_.insert(make_pair(p->location(), boost::shared_ptr<project>(p)));
}

engine::~engine()
{
   delete feature_registry_;
}

boost::filesystem::path find_root(const boost::filesystem::path& initial_path)
{
   boost::filesystem::path p(initial_path);

   while(true)
   {
      if (p.empty())
         throw runtime_error("Can't find boost-build.jam");

      if (exists(p / "boost-build.jam"))
         return p;

      p = p.branch_path();
   };
}

void engine::project_rule(project* p, std::vector<string>& name,
                          project_requirements_decl* req, project_requirements_decl* usage_req)
{
   assert(name.size() == size_t(1));
   p->name(name[0]);

   // ����� ������������� ������� ��� ��� �� ����� ��������
   // engine ��� ��������� �������������� �������� ������� �� ��� ��������
   if (req)
   {
      req->requirements().setup_path_data(p);
      p->requirements().insert(req->requirements());
   }

   if (usage_req)
   {
      usage_req->requirements().setup_path_data(p);
      p->usage_requirements().insert(usage_req->requirements());
   }
}

void engine::lib_rule(project* p, std::vector<string>& name, sources_decl* sources, requirements_decl* requirements,
                      feature_set* default_build, requirements_decl* usage_requirements)
{
   auto_ptr<basic_meta_target> mt(new lib_meta_target(p, name.at(0), requirements ? *requirements : requirements_decl(),
                                                      usage_requirements ? *usage_requirements : requirements_decl()));
   if (sources)
      mt->sources(*sources);

   p->add_target(mt);
}

void engine::searched_shared_lib_rule(project* p,
                                      std::vector<std::string>& name,
                                      sources_decl* sources,
                                      std::string& lib_name,
                                      requirements_decl* requirements,
                                      requirements_decl* usage_requirements)
{
   auto_ptr<basic_meta_target> mt(new searched_lib_meta_target(p,
                                                               name.at(0),
                                                               lib_name,
                                                               requirements ? *requirements : requirements_decl(),
                                                               usage_requirements ? *usage_requirements : requirements_decl(),
                                                               get_type_registry().get(types::SEARCHED_SHARED_LIB)));
   if (sources)
      mt->sources(*sources);

   p->add_target(mt);
}

void engine::searched_static_lib_rule(project* p,
                                      std::string& name,
                                      sources_decl* sources,
                                      std::string& lib_name,
                                      requirements_decl* requirements,
                                      requirements_decl* usage_requirements)
{
   auto_ptr<basic_meta_target> mt(new searched_lib_meta_target(p,
                                                               name,
                                                               lib_name,
                                                               requirements ? *requirements : requirements_decl(),
                                                               usage_requirements ? *usage_requirements : requirements_decl(),
                                                               get_type_registry().get(types::SEARCHED_STATIC_LIB)));
   if (sources)
      mt->sources(*sources);

   p->add_target(mt);
}

void engine::prebuilt_lib_rule(project* p,
                               std::string& name,
                               sources_decl* sources,
                               std::string& lib_filename,
                               requirements_decl* requirements,
                               requirements_decl* usage_requirements)
{
   auto_ptr<basic_meta_target> mt(new prebuilt_lib_meta_target(p,
                                                               name,
                                                               lib_filename,
                                                               requirements ? *requirements : requirements_decl(),
                                                               usage_requirements ? *usage_requirements : requirements_decl()));
   if (sources)
      mt->sources(*sources);

   p->add_target(mt);
}

void engine::file_rule(project* p,
                       std::vector<std::string>& name,
                       std::string& filename,
                       requirements_decl* requirements,
                       requirements_decl* usage_requirements)
{
   auto_ptr<basic_meta_target> mt(new file_meta_target(p,
                                                       name.at(0),
                                                       filename,
                                                       requirements ? *requirements : requirements_decl(),
                                                       usage_requirements ? *usage_requirements : requirements_decl()));
   p->add_target(mt);
}

void engine::header_lib_rule(project* p, std::vector<std::string>& name, sources_decl* sources, requirements_decl* requirements,
                             feature_set* default_build, requirements_decl* usage_requirements)
{
   auto_ptr<basic_meta_target> mt(new header_lib_meta_target(p, name.at(0), requirements ? *requirements : requirements_decl(),
                                                             usage_requirements ? *usage_requirements : requirements_decl()));
   if (sources)
      mt->sources(*sources);

   p->add_target(mt);
}

void engine::exe_rule(project* p, std::vector<std::string>& name, sources_decl& sources, requirements_decl* requirements,
                      feature_set* default_build, requirements_decl* usage_requirements)
{
   auto_ptr<basic_meta_target> mt(new typed_meta_target(p, name.at(0), requirements ? *requirements : requirements_decl(),
                                                        usage_requirements ? *usage_requirements : requirements_decl(),
                                                        get_type_registry().get(types::EXE)));
   mt->sources(sources);
   p->add_target(mt);
}

void engine::obj_rule(project* p, std::string& name, sources_decl& sources, requirements_decl* requirements,
                      feature_set* default_build, requirements_decl* usage_requirements)
{
   auto_ptr<basic_meta_target> mt(new obj_meta_target(p, name, requirements ? *requirements : requirements_decl(),
                                                      usage_requirements ? *usage_requirements : requirements_decl()));
   mt->sources(sources);
   p->add_target(mt);
}

void engine::pch_rule(project* p, std::string& name, sources_decl& sources, requirements_decl* requirements,
                      feature_set* default_build, requirements_decl* usage_requirements)
{
   auto_ptr<basic_meta_target> mt(new pch_meta_target(p, name, requirements ? *requirements : requirements_decl(),
      usage_requirements ? *usage_requirements : requirements_decl()));
   mt->sources(sources);
   p->add_target(mt);
}

void engine::copy_rule(project* p, std::string& name, sources_decl& sources, requirements_decl* requirements,
                       feature_set* default_build, requirements_decl* usage_requirements)
{
   auto_ptr<basic_meta_target> mt(new copy_meta_target(p,
                                                       name,
                                                       requirements ? *requirements : requirements_decl(),
                                                       usage_requirements ? *usage_requirements : requirements_decl()));
   mt->sources(sources);
   p->add_target(mt);

}

void engine::import_rule(project* p, std::vector<std::string>& name)
{

}

static feature_attributes resolve_attributes(std::vector<std::string>* attributes)
{
   typedef std::vector<std::string>::const_iterator iter;
   feature_attributes result = {0};

   if (attributes == NULL)
      return result;

   iter i = find(attributes->begin(), attributes->end(), "propagated");
   if (i != attributes->end())
      result.propagated = true;

   i = find(attributes->begin(), attributes->end(), "composite");
   if (i != attributes->end())
      result.composite = true;

   i = find(attributes->begin(), attributes->end(), "free");
   if (i != attributes->end())
      result.free = true;

   i = find(attributes->begin(), attributes->end(), "path");
   if (i != attributes->end())
      result.path = true;

   i = find(attributes->begin(), attributes->end(), "incidental");
   if (i != attributes->end())
      result.incidental = true;

   i = find(attributes->begin(), attributes->end(), "optional");
   if (i != attributes->end())
      result.optional = true;

   i = find(attributes->begin(), attributes->end(), "symmetric");
   if (i != attributes->end())
      result.symmetric = true;

   i = find(attributes->begin(), attributes->end(), "dependency");
   if (i != attributes->end())
      result.dependency = true;

   i = find(attributes->begin(), attributes->end(), "no-defaults");
   if (i != attributes->end())
      result.no_defaults = true;

   i = find(attributes->begin(), attributes->end(), "no-checks");
   if (i != attributes->end())
      result.no_checks = true;

   i = find(attributes->begin(), attributes->end(), "generated");
   if (i != attributes->end())
      result.generated = true;

   return result;
}

void engine::feature_feature_rule(project* p, std::vector<std::string>& name,
                                  std::vector<std::string>* values,
                                  std::vector<std::string>* attributes)
{
   if (name.empty() || name.size() > 1)
      throw std::runtime_error("[feature.feature] Bad feature name.");

   feature_registry_->add_feature_def(name[0], values ? *values : std::vector<std::string>(), resolve_attributes(attributes));
}

void engine::feature_subfeature_rule(project* p,
                                     std::string& feature_name,
                                     std::string& subfeature_name)
{
   feature_def& def = feature_registry_->get_def(feature_name);
   def.add_subfeature(subfeature_name);
}

void engine::feature_local_rule(project* p, std::vector<std::string>& name,
                                std::vector<std::string>* values,
                                std::vector<std::string>* attributes)
{
   if (name.empty() || name.size() > 1)
      throw std::runtime_error("[feature.feature] Bad feature name.");

   p->local_feature_registry().add_feature_def(name[0], values ? *values : std::vector<std::string>(), resolve_attributes(attributes));
}

void engine::feature_compose_rule(project* p, feature& f, feature_set& components)
{
   feature_set* cc = components.clone();
   feature_registry_->get_def(f.definition().name()).compose(f.value(), cc);
}

void engine::variant_rule(project* p, std::string& variant_name, std::string* base, feature_set& components)
{
   feature_def& def = feature_registry_->get_def("variant");
   def.extend_legal_values(variant_name);

   if (base == NULL)
      def.compose(variant_name, &components);
   else
   {
      feature_set* composite_features = feature_registry_->make_set();
      def.expand_composites(*base, composite_features);
      composite_features->join(components);
      def.compose(variant_name, composite_features);
   }
}

void engine::alias_rule(project* p,
                        std::string& name,
                        sources_decl* sources,
                        requirements_decl* requirements,
                        feature_set* default_build,
                        requirements_decl* usage_requirements)
{
   auto_ptr<basic_meta_target> mt(new alias_meta_target(p, name,
                                                        sources == NULL ? sources_decl() : *sources,
                                                        requirements ? *requirements : requirements_decl(),
                                                        usage_requirements ? *usage_requirements : requirements_decl()));
   p->add_target(mt);
}

void engine::version_alias_rule(project* p,
                                std::string& name,
                                std::string& version,
                                const std::string* target_path)
{
   auto_ptr<basic_meta_target> mt(new version_alias_meta_target(p, name, version, target_path));
   p->add_target(mt);
}

void engine::target_version_alias_rule(project* p,
                                       std::string& name,
                                       std::string& version,
                                       const std::string* target_path)
{
   auto_ptr<basic_meta_target> mt(new target_version_alias_meta_target(p, name, version, target_path));
   p->add_target(mt);
}

void engine::test_suite_rule(project* p,
                             std::string& name,
                             sources_decl& sources,
                             sources_decl* propagated_sources)
{
   sources_decl empty_source;
   sources_decl& real_propagated_sources = (propagated_sources == NULL ? empty_source : *propagated_sources);
   feature_set* additional_sources_set = feature_registry_->make_set();
   for(sources_decl::const_iterator i = real_propagated_sources.begin(), last = real_propagated_sources.end(); i != last; ++i)
   {
      feature* f = feature_registry_->create_feature("testing.additional-source", "");
      f->set_dependency_data(*i, p);
      additional_sources_set->join(f);
   }

   for(sources_decl::iterator i = sources.begin(), last = sources.end(); i != last; ++i)
      if (i->properties() != NULL)
         i->properties()->join(*additional_sources_set);
      else
         i->properties(additional_sources_set);

   auto_ptr<basic_meta_target> mt(new alias_meta_target(p, name,
                                                        sources,
                                                        requirements_decl(),
                                                        requirements_decl()));

   p->add_target(mt);
}

sources_decl engine::testing_run_rule(project* p,
                                      sources_decl* sources,
                                      std::vector<std::string>* args,
                                      std::vector<std::string>* input_files,
                                      requirements_decl* requirements,
                                      std::string* target_name)
{
   string real_target_name;
   if (target_name != NULL)
      real_target_name = *target_name;
   else
      if (sources != NULL && !sources->empty())
         real_target_name = location_t(sources->begin()->target_path()).stem().string();
      else
         throw std::runtime_error("Target must have either sources or target name");

   const string& exe_name = real_target_name;
   auto_ptr<basic_meta_target> intermediate_exe(
      new testing_intermediate_meta_target(p,
                                           exe_name,
                                           requirements != NULL ? *requirements
                                                                : requirements_decl(),
                                           requirements_decl(),
                                           get_type_registry().get(types::EXE)));

   intermediate_exe->sources(sources == NULL ? sources_decl() : *sources);
   intermediate_exe->set_explicit(true);

   p->add_target(intermediate_exe);

   requirements_decl run_requirements;

   if (input_files != NULL)
      for(vector<std::string>::const_iterator i = input_files->begin(), last = input_files->end(); i != last; ++i)
         run_requirements.add(*this->feature_registry().create_feature("testing.input-file", *i));

   if (args != NULL)
      for(vector<std::string>::const_iterator i = args->begin(), last = args->end(); i != last; ++i)
         run_requirements.add(*this->feature_registry().create_feature("testing.argument", *i));

   auto_ptr<basic_meta_target> run_target(
      new testing_meta_target(p,
                              real_target_name + ".runner",
                              run_requirements,
                              requirements_decl(),
                              get_type_registry().get(types::TESTING_RUN_PASSED)));

   sources_decl run_sources;
   run_sources.push_back(exe_name, get_type_registry());
   run_target->sources(run_sources);

   source_decl run_target_source(run_target->name(),
                                 std::string(),
                                 NULL /*to signal that this is meta target*/,
                                 NULL);

   p->add_target(run_target);

   sources_decl result;
   result.push_back(run_target_source);

   return result;
}

sources_decl
engine::testing_compile_fail_rule(project* p,
                                  const sources_decl& sources,
                                  requirements_decl* requirements,
                                  requirements_decl* default_build,
                                  requirements_decl* usage_requirements)
{
   const string target_name = location_t(sources.begin()->target_path()).stem().string();
   auto_ptr<basic_meta_target> mt(new testing_compile_fail_meta_target(p,
                                                                       target_name,
                                                                       requirements ? *requirements : requirements_decl(),
                                                                       usage_requirements ? *usage_requirements : requirements_decl()));
   mt->sources(sources);

   const source_decl compile_source(mt->name(),
                                    std::string(),
                                    NULL /*to signal that this is meta target*/,
                                    NULL);

   p->add_target(mt);

   sources_decl result;
   result.push_back(compile_source);

   return result;
}

static void glob_impl(sources_decl& result,
                      const fs::path& searching_path,
                      const fs::path& relative_path,
                      const boost::dos_wildcard& wildcard,
                      const std::vector<std::string>* exceptions,
                      engine& e)
{
   for(fs::directory_iterator i(searching_path), last = fs::directory_iterator(); i != last; ++i)
   {
      if (!is_directory(*i) && wildcard.match(i->path().filename()) &&
          !(exceptions != 0 && find(exceptions->begin(), exceptions->end(), i->path().filename().string()) != exceptions->end()))
      {
         result.push_back((relative_path / i->path().filename()).string(), e.get_type_registry());
      }
   }
}

static void rglob_impl(sources_decl& result,
                       const fs::path& searching_path,
                       fs::path relative_path,
                       const boost::dos_wildcard& wildcard,
                       const std::vector<std::string>* exceptions,
                       engine& e)
{
   int level = 0;
   for(fs::recursive_directory_iterator i(searching_path), last = fs::recursive_directory_iterator(); i != last; ++i)
   {
      while(level > i.level())
      {
         --level;
         relative_path = relative_path.branch_path();
      }

      if (is_directory(i.status()))
      {
         relative_path /= i->path().filename();
         ++level;
      }
      else
         if (wildcard.match(i->path().filename()) &&
             !(exceptions != 0 &&
               find(exceptions->begin(), exceptions->end(), i->path().filename().string()) != exceptions->end()))
         {
            result.push_back((relative_path / i->path().filename()).string(), e.get_type_registry());
         }
   }
}

sources_decl engine::glob_rule(project* p, std::vector<std::string>& patterns,
                               std::vector<std::string>* exceptions, bool recursive)
{
   using namespace boost::filesystem;
   sources_decl result;

   typedef std::vector<std::string>::const_iterator iter;
   for(const std::string& pattern : patterns)
   {
      string::size_type mask_pos = pattern.find_first_of("*?");
      if (mask_pos == string::npos)
         throw runtime_error("[glob] You must specify patterns to match");
      string::size_type separator_pos = pattern.find_last_of("/\\", mask_pos);
      path relative_path(separator_pos == string::npos ? path() : path(pattern.begin(),
                                                                       pattern.begin() + separator_pos));
      path searching_path(p->location() / relative_path);
      boost::dos_wildcard wildcard(string(pattern.begin() + mask_pos, pattern.end()));
      if (recursive)
         rglob_impl(result, searching_path, relative_path, wildcard, exceptions, *this);
      else
         glob_impl(result, searching_path, relative_path, wildcard, exceptions, *this);
   }

   result.unique();
   return result;
}

void engine::explicit_rule(project* p, const std::string& target_name)
{
   basic_meta_target* target = p->find_target(target_name);
   if (target == 0)
      throw std::runtime_error("target '" + target_name + "' not found.");

   p->mark_as_explicit(target_name);
}

void engine::use_project(const project& p, const std::string& project_id_alias, const location_t& project_location)
{
   use_project_rule(const_cast<project*>(&p), project_id_alias, project_location.string(), feature_registry().make_set());
}

void engine::add_project_alias(project* p,
                               const std::string& project_id_alias,
                               const std::string& project_location,
                               feature_set* props)
{
   location_t l(project_id_alias);
   if (!l.has_root_path())
   {
      use_project_data_t::iterator i = use_project_data_.find(p);
      if (i != use_project_data_.end())
      {
         use_project_data_t::mapped_type::const_iterator j = i->second.find(project_location);
         if (j != i->second.end())
            throw std::runtime_error("alias '" + project_id_alias + "' already mapped to location '" + j->second + "'.");
      }

      use_project_data_[p].insert(make_pair(location_t(l), project_location));
   }
   else
   {
      // go thought global map and create nodes that doesn't exists
      global_project_links_t* project_links_node = &global_project_links_;
      global_project_links_t::iterator alias_data_home;
      for(location_t::const_iterator i = ++l.begin(), last = l.end(); i != last; ++i)
      {
         alias_data_home = project_links_node->find(*i);
         if (alias_data_home == project_links_node->end())
         {
            boost::shared_ptr<project_alias_node> node(new project_alias_node);
            // FIXME stupid bug in ptr_map::insert
            location_t tmp(*i);
            alias_data_home = project_links_node->insert(make_pair(tmp, node)).first;
            project_links_node = &node->project_symlinks_;
         }
         else
            project_links_node = &alias_data_home->second->project_symlinks_;
      }

      project_alias_data alias_data;
      alias_data.location_ = p->location() / project_location;
      alias_data.location_.normalize();
      alias_data.properties_ = props;

      alias_data_home->second->aliases_data_.push_back(alias_data);

      reversed_global_project_links_.insert(make_pair(alias_data.location_, alias_data));
   }
}

void engine::use_project_rule(project* p,
                              const std::string& project_id_alias,
                              const std::string& project_location,
                              feature_set* props)
{
   add_project_alias(p, project_id_alias, project_location, props);
}

void engine::setup_warehouse_rule(project* p,
                                  const std::string& name,
                                  const std::string& url,
                                  const std::string* storage_dir_)
{
   if (warehouse_)
      throw std::runtime_error("You can setup only one warehouse to use");

   string storage_dir;
   if (storage_dir_ && !storage_dir_->empty()) {
      fs::path sd(*storage_dir_);
      if (sd.is_absolute())
         storage_dir = sd.string();
      else {
         sd = p->location() / sd;
         sd.normalize();
         storage_dir = sd.string();
      }
   }

   warehouse_.reset(new warehouse_impl(name, url, storage_dir));
   load_project(storage_dir);
}

project::selected_targets_t
engine::loaded_projects_t::select_best_alternative(const feature_set& build_request) const
{
   project::selected_targets_t result;
   for(projects_t::const_iterator i = projects_.begin(), last = projects_.end(); i != last; ++i)
   {
      project::selected_targets_t targets((**i).select_best_alternative(build_request));
      for(project::selected_targets_t::const_iterator t = targets.begin(), t_last = targets.end(); t != t_last; ++t)
         if (!t->target_->is_explicit())
            result.push_back(*t);
   }

   if (result.empty()) {
      stringstream s;
      dump_for_hash(s, build_request);
      throw std::runtime_error("Can't select best alternative - no one founded\n"
                               "Build request: " + s.str());
   }

   // Check for targets with same name. They already have same symbolic names so we should check names.
   sort(result.begin(), result.end(), [](const project::selected_target& lhs, const project::selected_target& rhs) {
      return lhs.target_->name() < rhs.target_->name();
   });

   auto first = result.begin();
   auto second = ++result.begin();
   for(; second != result.end();) {
      if (first->target_->name() == second->target_->name()) {
         stringstream s;
         dump_for_hash(s, build_request);
         throw std::runtime_error("Can't select best alternative for target '"+ first->target_->name() + "' from projects:\n"
                                  "1) '" + first->target_->location().string() + "' \n"
                                  "2) '" + second->target_->location().string() + "' \n"
                                  "Build request: " + s.str());
      } else {
         ++first;
         ++second;
      }
   }

   return result;
}

project::selected_target
engine::loaded_projects_t::select_best_alternative(const std::string& target_name,
                                                   const feature_set& build_request) const
{
   project::selected_targets_t result;
   for(projects_t::const_iterator i = projects_.begin(), last = projects_.end(); i != last; ++i)
   {
      project::selected_target st = (**i).try_select_best_alternative(target_name, build_request);
      if (st.target_ != NULL)
         result.push_back(st);
   }

   if (result.empty()) {
      stringstream s;
      s << "Can't select best alternative for target '"+ target_name + "' - no one founded. \n"
           "Projects to search are:\n";
      for (const project* p : projects_)
         s << "'" << p->location().string() << "'\n";
      s << "Build request: ";
      dump_for_hash(s, build_request);

      throw std::runtime_error(s.str());
   }

   if (result.size() == 1)
      return result.front();

   sort(result.begin(), result.end(), [](const project::selected_target& lhs, const project::selected_target& rhs) {
      return lhs.resolved_build_request_rank_ > rhs.resolved_build_request_rank_;
   });

   if (result[0].resolved_build_request_rank_ != result[1].resolved_build_request_rank_)
      return result.front();
   else {
      stringstream s;
      dump_for_hash(s, build_request);
      throw std::runtime_error("Can't select best alternative for target '"+ result[0].target_->name() + "' from projects:\n"
                               "1) '" + result[0].target_->location().string() + "' \n"
                               "2) '" + result[1].target_->location().string() + "' \n"
                               "Build request: " + s.str());
   }
}

void engine::output_location_strategy(boost::shared_ptr<hammer::output_location_strategy>& strategy)
{
   if (!strategy)
      output_location_strategy_.reset(new default_output_location_strategy);
   else
      output_location_strategy_ = strategy;
}

}
