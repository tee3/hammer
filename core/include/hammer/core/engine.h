#if !defined(h_4b62efd8_cafb_41d3_a747_e981804129e5)
#define h_4b62efd8_cafb_41d3_a747_e981804129e5

#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <functional>
#include <deque>
#include "location.h"
#include "project.h"
#include "call_resolver.h"

namespace hammer
{
   class basic_target;
   class type_registry;
   class feature_registry;
   class generator_registry;
   class feature;
   class project_requirements_decl;
   class toolset_manager;
   class scanner_manager;
   class output_location_strategy;
   class warehouse;

   class engine : boost::noncopyable
   {
      public:
         class loaded_projects_t;

         engine();
         loaded_projects_t load_project(location_t project_path, const project& from_project);
         project& load_project(location_t project_path);
         loaded_projects_t try_load_project(location_t project_path, const project& from_project);
         void load_hammer_script(location_t filepath);
         void load_hammer_script(const std::string& script_body,
                                 const std::string& script_name);

         void insert(project* p);
         type_registry& get_type_registry() { return *type_registry_; }
         generator_registry& generators() const { return *generators_; }
         hammer::feature_registry& feature_registry() { return *feature_registry_; }
         const hammer::feature_registry& feature_registry() const { return *feature_registry_; }
         hammer::call_resolver& call_resolver() { return resolver_; }
         hammer::toolset_manager& toolset_manager() { return *toolset_manager_; }
         hammer::scanner_manager& scanner_manager() { return *scanner_manager_; }
         const hammer::scanner_manager& scanner_manager() const { return *scanner_manager_; }
         hammer::warehouse& warehouse() { return *warehouse_; }

         hammer::output_location_strategy& output_location_strategy() { return *output_location_strategy_; }
         void output_location_strategy(boost::shared_ptr<hammer::output_location_strategy>& strategy);
         void use_project(const project& p, const std::string& project_id_alias, const location_t& project_location);
         ~engine();

      private:
         typedef boost::unordered_map<const location_t,
                                      boost::shared_ptr<project>,
                                      std::hash<location_t>,
                                      location_equal_to> projects_t;
         struct project_alias_node;
         typedef boost::unordered_map<location_t,
                                      boost::shared_ptr<project_alias_node>,
                                      std::hash<location_t>,
                                      location_equal_to>  global_project_links_t;

         struct project_alias_data
         {
            location_t location_;
            const feature_set* properties_;
         };

         // map filesystem path on global alias node for a project
         typedef boost::unordered_map<location_t,
                                      project_alias_data,
                                      std::hash<location_t>,
                                      location_equal_to> reversed_global_project_links_t;

         struct project_alias_node
         {
            typedef std::vector<project_alias_data> aliases_data_t;

            // mapping associated with this node. One project symlink path such as /foo/bar
            // can be mapped on many different filesystem paths
            aliases_data_t aliases_data_;

            global_project_links_t project_symlinks_;
         };

         struct resolved_project_symlink_t
         {
            resolved_project_symlink_t(const location_t& tail,
                                       const project_alias_node::aliases_data_t& symlinks_data)
                                       : tail_(tail),
                                         symlinks_data_(&symlinks_data)
            {
            }

            // Unresolved part. bar is a tail_, if /foo/bar was requested and foo was founded.
            location_t tail_;
            const project_alias_node::aliases_data_t* symlinks_data_; // cannot be NULL
         };
         typedef std::vector<resolved_project_symlink_t> resolved_project_symlinks_t;

         typedef std::map<const project*,
                          boost::unordered_map<location_t /* alias */,
                                               std::string /* map to */,
                                               std::hash<location_t>,
                                               location_equal_to> > use_project_data_t;

         projects_t projects_;
         global_project_links_t global_project_links_;
         reversed_global_project_links_t reversed_global_project_links_;

         boost::shared_ptr<type_registry> type_registry_;
         hammer::feature_registry* feature_registry_;
         hammer::call_resolver resolver_;
         boost::shared_ptr<generator_registry> generators_;
         boost::shared_ptr<hammer::toolset_manager> toolset_manager_;
         boost::shared_ptr<hammer::scanner_manager> scanner_manager_;
         boost::shared_ptr<hammer::output_location_strategy> output_location_strategy_;

         use_project_data_t use_project_data_;
         boost::shared_ptr<hammer::warehouse> warehouse_;

         loaded_projects_t try_load_project(const location_t& tail_path, const project_alias_data& symlink);
         loaded_projects_t try_load_project(location_t project_path);
         project* get_upper_project(const location_t& project_path);
         void resolve_project_alias(resolved_project_symlinks_t& symlinks,
                                    const location_t& project_symlink);
         void resolve_project_alias(resolved_project_symlinks_t& symlinks,
                                    location_t::const_iterator first, location_t::const_iterator last,
                                    global_project_links_t& symlink_storage);
         void resolve_use_project(location_t& resolved_use_path, location_t& tail_path,
                                  const hammer::project& project, const location_t& path_to_resolve);
         void project_rule(project* p, std::vector<std::string>& name, project_requirements_decl* req, project_requirements_decl* usage_req);
         void lib_rule(project* p, std::vector<std::string>& name, sources_decl* sources,
                       requirements_decl* fs, feature_set* default_build, requirements_decl* usage_requirements);
         void searched_shared_lib_rule(project* p, std::vector<std::string>& name,
                                       sources_decl* sources, std::string& lib_name,
                                       requirements_decl* requirements, requirements_decl* usage_requirements);
         void searched_static_lib_rule(project* p, std::string& name,
                                       sources_decl* sources, std::string& lib_name,
                                       requirements_decl* requirements, requirements_decl* usage_requirements);
         void prebuilt_lib_rule(project* p, std::string& name,
                                sources_decl* sources, std::string& lib_filename,
                                requirements_decl* requirements, requirements_decl* usage_requirements);
         void file_rule(project* p,
                        std::vector<std::string>& name,
                        std::string& filename,
                        requirements_decl* requirements,
                        requirements_decl* usage_requirements);
         void header_lib_rule(project* p, std::vector<std::string>& name, sources_decl* sources,
                              requirements_decl* fs, feature_set* default_build, requirements_decl* usage_requirements);
         void exe_rule(project* p, std::vector<std::string>& name, sources_decl& sources, requirements_decl* fs,
                       feature_set* default_build, requirements_decl* usage_requirements);
         void obj_rule(project* p, std::string& name, sources_decl& sources, requirements_decl* fs,
                       feature_set* default_build, requirements_decl* usage_requirements);
         void pch_rule(project* p, std::string& name, sources_decl& sources, requirements_decl* fs,
                       feature_set* default_build, requirements_decl* usage_requirements);
         void alias_rule(project* p,
                         std::string& name,
                         sources_decl* sources,
                         requirements_decl* fs,
                         feature_set* default_build,
                         requirements_decl* usage_requirements);
         void version_alias_rule(project* p,
                                 std::string& name,
                                 std::string& version,
                                 const std::string* target_path);
         void target_version_alias_rule(project* p,
                                        std::string& name,
                                        std::string& version,
                                        const std::string* target_path);
         void test_suite_rule(project* p,
                              std::string& name,
                              sources_decl& sources,
                              sources_decl* propagated_sources);
         sources_decl testing_run_rule(project* p,
                                       sources_decl* sources,
                                       std::vector<std::string>* args,
                                       std::vector<std::string>* input_files,
                                       requirements_decl* requirements,
                                       std::string* target_name);
         sources_decl testing_compile_fail_rule(project* p,
                                                const sources_decl& sources,
                                                requirements_decl* requirements,
                                                requirements_decl* default_build,
                                                requirements_decl* usage_requirements);
         void copy_rule(project* p, std::string& name, sources_decl& sources, requirements_decl* fs,
                        feature_set* default_build, requirements_decl* usage_requirements);
         void import_rule(project* p, std::vector<std::string>& name);
         void feature_feature_rule(project* p, std::vector<std::string>& name, std::vector<std::string>* values,
                                   std::vector<std::string>* attributes);
         void feature_subfeature_rule(project* p,
                                      std::string& feature_name,
                                      std::string& subfeature_name);
         void feature_local_rule(project* p, std::vector<std::string>& name, std::vector<std::string>* values,
                                 std::vector<std::string>* attributes);
         void feature_compose_rule(project* p, feature& f, feature_set& components);
         void variant_rule(project* p, std::string& variant_name, std::string* base, feature_set& components);
         sources_decl glob_rule(project* p, std::vector<std::string>& patterns,
                                std::vector<std::string>* exceptions, bool recursive);
         void explicit_rule(project* p, const std::string& target_name);
         void use_project_rule(project* p, const std::string& project_id_alias,
                               const std::string& project_location, feature_set* props);
         void setup_warehouse_rule(project* p,
                                   const std::string& name,
                                   const std::string& url,
                                   const std::string* storage_dir);
   };

   boost::filesystem::path find_root(const boost::filesystem::path& initial_path);

   class engine::loaded_projects_t
   {
         typedef std::vector<project*> projects_t;
      public:
         friend class engine;

         typedef projects_t::const_iterator const_iterator;

         explicit loaded_projects_t(project* v) : projects_(1, v) {}
         void push_back(project* v) { projects_.push_back(v); }
         const_iterator begin() const { return projects_.begin(); }
         const_iterator end() const { return projects_.end(); }
         project& front() const { return *projects_.front(); }
         bool is_single() const { return projects_.size() == 1; }
         loaded_projects_t& operator +=(const loaded_projects_t& rhs)
         {
            projects_.insert(projects_.end(), rhs.projects_.begin(), rhs.projects_.end());
            return *this;
         }
         project::selected_targets_t select_best_alternative(const feature_set& build_request) const;
         project::selected_target select_best_alternative(const std::string& target_name, const feature_set& build_request) const;
         feature_set* resolve_undefined_features(const feature_set& s);
         bool empty() const { return projects_.empty(); }

      private:
         projects_t projects_;

         loaded_projects_t() {};
   };
}

#endif //h_4b62efd8_cafb_41d3_a747_e981804129e5
