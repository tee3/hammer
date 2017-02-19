#include <hammer/core/warehouse.h>
#include <hammer/core/warehouse_target.h>
#include <hammer/core/warehouse_meta_target.h>
#include <hammer/core/main_target.h>
#include <hammer/core/call_resolver.h>
#include <hammer/core/project.h>
#include <hammer/core/engine.h>
#include <hammer/core/feature_set.h>
#include <hammer/core/feature.h>
#include <unordered_set>
#include <boost/bind.hpp>
#include <algorithm>

namespace hammer{

const std::string warehouse::any_version = std::string();

static
void walk_over_targets(std::vector<const warehouse_target*>& result,
                       std::unordered_set<basic_target*>& visited,
                       const std::vector<basic_target*>& targets)
{
   for(std::vector<basic_target*>::const_iterator i = targets.begin(), last = targets.end(); i != last; ++i) {
      if (warehouse_target* t = dynamic_cast<warehouse_target*>(*i))
         result.push_back(t);
      else if (main_target* m = dynamic_cast<main_target*>(*i)) {
         if (visited.find(m) == visited.end()) {
            visited.insert(m);
            walk_over_targets(result, visited, m->sources());
            walk_over_targets(result, visited, m->dependencies());
         }
      }
   }
}

std::vector<const warehouse_target*>
find_all_warehouse_unresolved_targets(const std::vector<basic_target*>& targets)
{
   std::vector<const warehouse_target*> result;
   std::unordered_set<basic_target*> visited;

   walk_over_targets(result, visited, targets);

   return result;
}

static
std::vector<std::string>
collect_installed_versions(const project& p)
{
   std::vector<std::string> result;

   const feature_set& build_request = *p.get_engine()->feature_registry().make_set();
   for(const auto& t : p.targets()) {
      feature_set& requirements = *p.get_engine()->feature_registry().make_set();
      t.second->requirements().eval(build_request, &requirements);
      auto i = requirements.find("version");
      if (i != requirements.end())
         result.push_back((**i).value());
   }

   sort(result.begin(), result.end());
   result.erase(unique(result.begin(), result.end()), result.end());

   return result;
}

void add_traps(project& p,
               const std::string& public_id)
{
   warehouse& wh = p.get_engine()->warehouse();
   warehouse::versions_t all_versions = wh.get_package_versions(public_id);
   std::vector<std::string> installed_versions = collect_installed_versions(p);

   warehouse::versions_t not_installed_versions;

   sort(all_versions.begin(), all_versions.end(),
        [] (const warehouse::version_info& lhs, const warehouse::version_info& rhs)
      {
         return lhs.version_ < rhs.version_;
      });

   struct pred
   {
      bool operator()(const warehouse::version_info& lhs, const std::string& rhs) const { return lhs.version_ < rhs; }
      bool operator()(const std::string& lhs, const warehouse::version_info& rhs) const { return lhs < rhs.version_; }
      // These two need by msvc debug implementation
      bool operator()(const warehouse::version_info& lhs, const warehouse::version_info& rhs) const { return lhs.version_ < rhs.version_; }
      bool operator()(const std::string& lhs, const std::string& rhs) const { return lhs < rhs; }
   };

   set_difference(all_versions.begin(), all_versions.end(),
                  installed_versions.begin(), installed_versions.end(),
                  back_inserter(not_installed_versions), pred());

   for(const auto& v : not_installed_versions) {
      for (const std::string& target_name : v.targets_) {
         std::auto_ptr<basic_meta_target> trap_target(new warehouse_meta_target(p, target_name, v.version_));
         p.add_target(trap_target);
      }
   }
}

static
void warehouse_trap_rule(project* p,
                         string& public_id)
{
   warehouse& wh = p->get_engine()->warehouse();
   if (!wh.has_project("/" / location_t(public_id), warehouse::any_version))
      throw std::runtime_error("Can't find '" + public_id + "' in warehouse");

   add_traps(*p, public_id);
}

void install_warehouse_rules(call_resolver& resolver,
                             engine& engine)
{
   resolver.insert("warehouse-trap", boost::function<void (project*, string&)>(boost::bind(warehouse_trap_rule, _1, _2)));
}

}
