#include <sstream>
#include <boost/format.hpp>
#include <boost/make_unique.hpp>
#include <boost/filesystem/path_traits.hpp>
#include <hammer/core/project.h>
#include <hammer/core/engine.h>
#include <hammer/core/feature_registry.h>
#include <hammer/core/feature_set.h>
#include <hammer/core/feature.h>

using namespace std;

namespace hammer {

struct project::aliases_impl {
   struct node;
   using node_ptr = std::unique_ptr<node>;
   using path_element = boost::filesystem::path;
   using nodes = boost::unordered_multimap<path_element, node_ptr>;

   struct node {
      const feature_set* requirements_;
      nodes nodes_;
      location_t full_fs_path_;
   };

   // return false on failure - same alias used twice
   bool add_alias(nodes& nodes,
                  const boost::filesystem::path& alias,
                  const location_t& fs_path,
                  const feature_set* requirements);
   bool add_alias(nodes& nodes,
                  boost::filesystem::path::const_iterator first,
                  boost::filesystem::path::const_iterator last,
                  const location_t& fs_path,
                  const feature_set* requirements);
   void load_project(loaded_projects& result,
                     engine& e,
                     boost::filesystem::path::const_iterator first,
                     boost::filesystem::path::const_iterator last);
   void load_project(loaded_projects& result,
                     nodes& nodes,
                     engine& e,
                     boost::filesystem::path::const_iterator first,
                     boost::filesystem::path::const_iterator last);

   nodes nodes_;
   project::aliases_t aliases_;
};

// FIXME: Need to figure out why all projects have to end on dot
static
location_t
normalize_project_location(const location_t& l)
{
   // all project paths must end on dot
   std::string path = l.string();
   if (!path.empty() && *path.rbegin() != '.') {
      location_t location_{l / "."};
      location_.normalize();
      return location_;
   } else
      return l;
}

project::project(hammer::engine& e,
                 const project* parent,
                 const std::string& name,
                 const location_t& location,
                 const requirements_decl& local_req,
                 const requirements_decl& local_usage_req,
                 const dependencies_t& dependencies)
   : parent_(parent),
     dependencies_(dependencies),
     engine_(e),
     name_(name),
     location_(normalize_project_location(location)),
     aliases_(new aliases_impl)
{
   local_requirements(local_req);
   local_usage_requirements(local_usage_req);
}

project::project(engine& e,
                 const project* parent,
                 const location_t& l)
   : project(e, parent, {}, l, {}, {}, {})
{
}

static
requirements_decl
join_requirements(const requirements_decl rhs,
                  requirements_decl lhs) {
   lhs.insert_infront(rhs);
   return lhs;
}

void project::local_requirements(const requirements_decl& req) {
   local_requirements_ = req;
   requirements_ = parent_ ? join_requirements(parent_->requirements(), req) : req;
}

void project::local_usage_requirements(const requirements_decl& req) {
   local_usage_requirements_ = req;
   usage_requirements_ = parent_ ? join_requirements(parent_->usage_requirements(), req) : req;
}

void project::dependencies(const dependencies_t& v) {
   if (parent_) {
      dependencies_ = parent_->dependencies();
      dependencies_.insert(dependencies_.end(), v.begin(), v.end());
   } else
      dependencies_ = v;
}

void project::dependencies(dependencies_t&& v) {
   if (parent_) {
      dependencies_ = parent_->dependencies();
      dependencies_.insert(dependencies_.end(), make_move_iterator(v.begin()), make_move_iterator(v.end()));
   } else
      dependencies_ = std::move(v);
}

string
find_version(engine& e,
             const requirements_decl& meta_requirements) {
   feature_set* requirements = e.feature_registry().make_set();
   feature_set& build_request = *e.feature_registry().make_set();
   feature_set* usage_requirements = e.feature_registry().make_set();

   meta_requirements.eval(build_request, requirements, usage_requirements);
   auto i = requirements->find("version");
   if (i != requirements->end())
      return (**i).value();
   else
      return {};
}

bool project::publishable() const {
   return !find_version(get_engine(), local_requirements()).empty();
}

string
project::publishable_version() const
{
   string version = find_version(get_engine(), local_requirements());
   if (version.empty())
      throw std::runtime_error("Project is not publishable");

   return version;
}

project::~project() {

}

void project::add_target(std::unique_ptr<basic_meta_target> t)
{
   t->requirements().insert_infront(requirements());
   t->usage_requirements().insert_infront(usage_requirements());
   targets_.insert(std::make_pair(t->name(), move(t)));
}

const basic_meta_target* project::find_target(const std::string& name) const
{
   return const_cast<project*>(this)->find_target(name);
}

basic_meta_target* project::find_target(const std::string& name)
{
   auto i = targets_.find(name);
   if (i == targets_.end())
      return 0;
   else
      return i->second.get();
}

void project::add_alias(const location_t& alias,
                        const location_t& fs_path,
                        const feature_set* requirements)
{
   if (alias.has_root_path())
      throw std::runtime_error("[project::add_alias]: alias can be only relative");

   auto ia2 = find_if(alias.begin(), alias.end(), [](const location_t& e) {
      return e == ".";
   });
   auto ia = find_if(alias.begin(), alias.end(), [](const location_t& e) {
      return e == "..";
   });
   auto ifp = find_if(fs_path.begin(), fs_path.end(), [](const location_t& e) {
      return e == "..";
   });

   if (ia2 != alias.end())
      throw std::runtime_error("[project::add_alias]: alias can't contain . elements");

   if (ia != alias.end() || ifp != fs_path.end())
      throw std::runtime_error("[project::add_alias]: alias/fs_path can't contain .. elements");

   if (!aliases_->add_alias(aliases_->nodes_, alias, fs_path.has_root_path() ? fs_path : location() / fs_path, requirements))
      throw std::runtime_error("Alias path '" + alias.string() + "' already added" );
}

const project::aliases_t&
project::aliases() const
{
   return aliases_->aliases_;
}

loaded_projects
project::load_project(const location_t& path) const
{
   if (path.has_root_path())
      return get_engine().load_project(engine::global_project_ref{path});

   loaded_projects result;
   // load projects that matches aliases
   aliases_->load_project(result, get_engine(), path.begin(), path.end());
   if (result.empty()) {
      // load project from filesystem
      result = get_engine().try_load_project(location() / path);
      if (!result.empty()) {
         // project path has been fully resolved, but we need to include transparent proxies from final project
         result.front().aliases_->load_project(result, get_engine(), path.begin(), path.end());
      }
   }

   return result;
}

// -1 == not suitable
// > -1 == suitable with computed rank. Zero IS valid rank
static
int compute_alternative_rank(const feature_set& target_properties,
                             const feature_set& build_request)
{
   unsigned rank = 0;
   for(const feature* tf : target_properties) {
      if (!(tf->attributes().free ||
            tf->attributes().incidental ||
            tf->attributes().undefined_))
      {
         feature_set::const_iterator bf = build_request.find(tf->name());
         if (bf != build_request.end())
            if (tf->value() != (**bf).value())
               return -1;
            else
               ++rank;
         else {
            // feature is not in build_request
            if (tf->name() == "override")
               rank += 10000;
            else
               rank += tf->definition().defaults_contains(tf->value());
         }
      }
   }

   return rank;
}

project::selected_target
project::select_best_alternative(const std::string& target_name,
                                 const feature_set& build_request,
                                 const bool allow_locals) const
{

   selected_target result = try_select_best_alternative(target_name, build_request, allow_locals);
   if (result.target_ == NULL)
      throw std::runtime_error("Can't select alternative for target '" + target_name + "'.");

   return result;
}

static
bool s_great(const project::selected_target& lhs,
             const project::selected_target& rhs)
{
   return lhs.resolved_requirements_rank_ > rhs.resolved_requirements_rank_;
}

#if !defined(_WIN32)
[[ noreturn ]]
#endif
static
void error_cannot_choose_alternative(const project& p,
                                     const std::string& target_name,
                                     const feature_set& build_request_param)
{
   auto fmt = boost::format("Failed to selecting best alternatives for target '%s' in project '%s'\n"
                            "Build request: %s\n");

   stringstream s;
   dump_for_hash(s, build_request_param);
   throw std::runtime_error((fmt % target_name % p.location()
                                 % s.str()).str());
}

static
int compute_requirements_rank(const feature_set& requirements)
{
   int rank = 0;
   for (const feature* f : requirements) {
      if (f->attributes().free ||
          f->attributes().incidental ||
          f->attributes().undefined_)
         continue;

      if (!f->definition().defaults_contains(f->value()))
          return -1;

      ++rank;
   }

   return rank;
}

project::selected_target
project::try_select_best_alternative(const std::string& target_name,
                                     const feature_set& build_request_param,
                                     const bool allow_locals) const
{
   const feature_set& build_request = build_request_param.has_undefined_features()
                                         ? *try_resolve_local_features(build_request_param)
                                         : build_request_param;

   boost::iterator_range<targets_t::const_iterator> r = targets_.equal_range(target_name);

   if (r.empty())
      return {};

   vector<selected_target> selected_targets;

   for(targets_t::const_iterator first = r.begin(), last = r.end(); first != last; ++first) {
      if (!allow_locals && first->second->is_local())
         continue;

      feature_set* fs = engine_.feature_registry().make_set();
      first->second->requirements().eval(build_request, fs);
      int rank = compute_alternative_rank(*fs, build_request);
      if (rank != -1)
         selected_targets.push_back(selected_target(first->second.get(), fs, rank));
   }

   sort(selected_targets.begin(), selected_targets.end(), s_great);
   if (selected_targets.empty())
      return selected_target();

   if (selected_targets.size() == 1)
      return selected_targets.front();

   if (selected_targets[0].resolved_requirements_rank_ != selected_targets[1].resolved_requirements_rank_)
      return selected_targets.front();

   // okay, ranks are same, so we need to find target with best computed requirements
   vector<selected_target> selected_targets_2;
   for (const selected_target& t : selected_targets) {
      const int rank = compute_requirements_rank(*t.resolved_requirements_);
      if (rank >= 0)
         selected_targets_2.push_back(selected_target(t.target_, t.resolved_requirements_, static_cast<unsigned>(rank)));
   }

   sort(selected_targets_2.begin(), selected_targets_2.end(), s_great);

   if (selected_targets_2.size() == 1)
      return selected_targets_2.front();

   if (!selected_targets_2.empty() &&
       selected_targets_2[0].resolved_requirements_rank_ != selected_targets_2[1].resolved_requirements_rank_)
   {
      return selected_targets_2.front();
   } else
      error_cannot_choose_alternative(*this, target_name, build_request_param);
}

void project::instantiate(const std::string& target_name,
                          const feature_set& build_request,
                          std::vector<basic_target*>* result) const
{
   selected_target best_target = select_best_alternative(target_name, build_request);
   feature_set* usage_requirements = engine_.feature_registry().make_set();
   best_target.target_->instantiate(0, build_request, result, usage_requirements);
}

bool project::operator == (const project& rhs) const
{
   return this == &rhs;
}

project::selected_targets_t
project::select_best_alternative(const feature_set& build_request) const
{
   selected_targets_t result;

   targets_t::const_iterator first = targets_.begin(), last = targets_.end();
   while(first != last)
   {
      selected_target t = try_select_best_alternative(first->second->name(), build_request);
      if (t.target_ != NULL)
         result.push_back(t);

      // we just processed targets with name equal to 'first->second->name()', lets move to
      // group of targets with different name
      targets_t::const_iterator next = first; std::advance(next, 1);
      while(next != last && first->first == next->first)
         ++first, ++next;

      first = next;
   }

   return result;
}

feature_set*
project::try_resolve_local_features(const feature_set& fs) const
{
   feature_set* result = engine_.feature_registry().make_set();
   for(feature_set::const_iterator i = fs.begin(), last = fs.end(); i != last; ++i)
   {
      if ((**i).attributes().undefined_)
      {
         const feature_def* def = local_feature_registry_.find_def_from_full_name((**i).name().c_str());
         if (def != NULL)
            result->join(local_feature_registry_.create_feature((**i).name(), (**i).value()));
         else
            result->join(*i);
      }
      else
         result->join(*i);
   }

   return result;
}

bool project::aliases_impl::add_alias(nodes& nodes,
                                      const boost::filesystem::path& alias,
                                      const location_t& fs_path,
                                      const feature_set* requirements)
{
   const bool r = add_alias(nodes, alias.begin(), alias.end(), fs_path, requirements);
   if (r)
      aliases_.push_back({alias, fs_path, requirements ? requirements->clone() : nullptr});

   return r;
}

bool project::aliases_impl::add_alias(nodes& nodes,
                                      location_t::const_iterator first,
                                      location_t::const_iterator last,
                                      const location_t& fs_path,
                                      const feature_set* requirements)
{
   auto has_same_fs_path = [&]() {
      for (auto& n : nodes) {
         if (n.second->full_fs_path_ == fs_path)
            return true;
      }

      return false;
   };

   if (first == last) {
      // special case for transparent proxies - they don't have alias
      std::unique_ptr<node> new_node{ new node{ requirements ? requirements->clone() : nullptr, aliases_impl::nodes{}, fs_path } };
      if (has_same_fs_path())
         return false;
      else {
         nodes.insert({ {}, std::move(new_node) });
         return true;
      }
   }

   auto sfirst = first;
   auto i = nodes.find(*first);
   if (i == nodes.end()) {
      if (++first == last) {
         if (has_same_fs_path())
            return false;
         else {
            std::unique_ptr<node> new_node{ new node{ requirements ? requirements->clone() : nullptr, aliases_impl::nodes{}, fs_path } };
            nodes.insert({ *sfirst, std::move(new_node) });
            return true;
         }
      } else {
         if (has_same_fs_path())
            return false;
         else {
            std::unique_ptr<node> new_node{ new node{ nullptr, aliases_impl::nodes{}, {} } };
            i = nodes.insert({ *sfirst, std::move(new_node) });
            return add_alias(i->second->nodes_, first, last, fs_path, requirements);
         }
      }
   } else {
      if (++first == last) {
         node& n = *i->second;
         if (n.full_fs_path_.empty()) {
            if (requirements)
               n.requirements_ = requirements->clone();
            n.full_fs_path_ = fs_path;

            return true;
         } else {
            // node has been found and path already set - we need one more node to describe alternative
            if (has_same_fs_path())
               return false;
            else {
               std::unique_ptr<node> new_node{ new node{ requirements ? requirements->clone() : nullptr, aliases_impl::nodes{}, fs_path } };
               nodes.insert({ *sfirst, std::move(new_node) });
               return true;
            }
         }
      } else
         return add_alias(i->second->nodes_, first, last, fs_path, requirements);
   }
}

void project::aliases_impl::load_project(loaded_projects& result,
                                         engine& e,
                                         boost::filesystem::path::const_iterator first,
                                         boost::filesystem::path::const_iterator last)
{
   // first of all add transparent proxied projects
   auto ri = nodes_.equal_range({});
   for (auto i = ri.first; i != ri.second; ++i) {
      project& p = e.load_project(i->second->full_fs_path_);
      result.push_back(&p);
      if (first != last) {
         // FIXME: stupid boost::filesystem::path can't be constructed from two iterators
         boost::filesystem::path pp;
         for (auto f = first; f != last; ++f)
            pp /= *f;
         result += p.load_project(pp);
      }
   }

   return load_project(result, nodes_, e, first, last);
}

void project::aliases_impl::load_project(loaded_projects& result,
                                         nodes& nodes,
                                         engine& e,
                                         boost::filesystem::path::const_iterator first,
                                         boost::filesystem::path::const_iterator last)
{
   // ignore dots if any
   while (first != last && *first == ".")
      ++first;

   if (first == last)
      return;

   auto ri = nodes.equal_range(*first);
   for (auto i = ri.first; i != ri.second; ++i) {
      node& n = *i->second;

      if (!n.full_fs_path_.empty()) {
         auto first_copy = first;
         project& p = e.load_project(n.full_fs_path_);
         if (++first_copy == last) {
            result.push_back(&p);
            // project path has been fully resolved, but we need to include transparent proxies from final project
            p.aliases_->load_project(result, e, last, last);
            continue;
         } else
            p.aliases_->load_project(result, e, first_copy, last);
      }

      auto first_copy = first;
      if (++first_copy == last)
         return;

      load_project(result, n.nodes_, e, first_copy, last);
   }
}

void loaded_projects::push_back(project* v)
{
   // no duplicates allowed, but project::load_project will try to add some projects multiple times
   // so we need to be prepared
   if (find(projects_.begin(), projects_.end(), v) == projects_.end())
      projects_.push_back(v);
}

loaded_projects&
loaded_projects::operator +=(const loaded_projects& rhs) {
   for (project* v : rhs)
      push_back(v);

   return *this;
}

project::selected_targets_t
loaded_projects::select_best_alternative(const feature_set& build_request) const
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
loaded_projects::select_best_alternative(const std::string& target_name,
                                         const feature_set& build_request,
                                         bool allow_locals) const
{
   project::selected_targets_t result;
   for(projects_t::const_iterator i = projects_.begin(), last = projects_.end(); i != last; ++i)
   {
      project::selected_target st = (**i).try_select_best_alternative(target_name, build_request, allow_locals);
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
      return lhs.resolved_requirements_rank_ > rhs.resolved_requirements_rank_;
   });

   if (result[0].resolved_requirements_rank_ != result[1].resolved_requirements_rank_)
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

}
