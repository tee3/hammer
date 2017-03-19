#include "stdafx.h"
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_lists.hpp>
#include <hammer/core/basic_meta_target.h>
#include <hammer/core/engine.h>
#include <hammer/core/feature.h>
#include <hammer/core/feature_registry.h>
#include <hammer/core/feature_set.h>
#include <hammer/core/fs_helpers.h>
#include <hammer/core/location.h>
#include <hammer/core/sources_decl.h>
#include <hammer/core/subfeature.h>
#include <iterator>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace hammer {

feature_set::feature_set(feature_registry* fr)
  : fr_(fr)
  , has_undefined_(false)
{
}

feature_set&
feature_set::join(const char* name, const char* value)
{
  return join(fr_->create_feature(name, value));
}

feature_set&
feature_set::join(feature* f)
{
  if (f->attributes().undefined_ != 0u) {
    has_undefined_ = true;
  }

  if (f->attributes().free == 0u) {
    auto i = find(f->name());
    if (i != end()) {
      if ((**i).value() != f->value()) {
        *i = f;
        if (f->attributes().composite != 0u) {
          f->definition().expand_composites(f->value(), this);
        }
      }
    } else {
      features_.push_back(f);
      if (f->attributes().composite != 0u) {
        f->definition().expand_composites(f->value(), this);
      }
    }
  } else {
    if (find(*f) == end()) {
      features_.push_back(f);
    }
  }

  return *this;
}

const feature&
feature_set::get(const char* name_) const
{
  auto f = find(name_);
  if (f == features_.end()) {
    throw runtime_error("feature '" + string(name_) + "not founded");
  }

  return **f;
}

feature_set::iterator
feature_set::deconstify(const_iterator i)
{
  auto result = features_.begin();
  std::advance(
    result, std::distance<feature_set::const_iterator>(features_.begin(), i));
  return result;
}

feature_set::iterator
feature_set::find(const char* name)
{
  return deconstify(static_cast<const feature_set*>(this)->find(name));
}

feature_set::const_iterator
feature_set::find(const char* name) const
{
  return find(features_.begin(), name);
}

feature_set::const_iterator
feature_set::find(const_iterator from, const char* name) const
{
  for (auto i = from, last = features_.end(); i != last; ++i) {
    if ((**i).definition().name() == name) {
      return i;
    }
  }

  return features_.end();
}

feature_set::iterator
feature_set::find(iterator from, const char* name)
{
  return deconstify(static_cast<const feature_set*>(this)->find(from, name));
}

const feature*
feature_set::find(const char* name, const char* value) const
{
  for (auto feature : features_) {
    if ((*feature).definition().name() == name && (*feature).value() == value) {
      return feature;
    }
  }

  return nullptr;
}

void
feature_set::join_impl(feature_set* lhs, const feature_set& rhs) const
{
  for (auto rh : rhs) {
    lhs->join(rh);
  }
}

feature_set*
feature_set::join(const feature_set& rhs) const
{
  assert(fr_);
  feature_set* result = fr_->make_set();
  result->features_ = features_;
  join_impl(result, rhs);
  return result;
}

feature_set&
feature_set::join(const feature_set& rhs)
{
  join_impl(this, rhs);
  return *this;
}

feature_set*
feature_set::clone() const
{
  feature_set* result = fr_->make_set();
  result->features_ = features_;
  return result;
}

void
feature_set::copy_propagated(const feature_set& v)
{
  for (auto i : v) {
    if (i->attributes().propagated != 0u) {
      join(i);
    }
  }
}

feature_set::const_iterator
feature_set::find(const feature& f) const
{
  for (auto i = find(f.name()), last = end(); i != last;) {
    if (**i == f) {
      return i;
    }

    if (f.attributes().free != 0u) {
      i = find(++i, f.name());
    } else {
      break;
    }
  }

  return end();
}

feature_set::const_iterator
feature_set::contains(const feature& f) const
{
  for (auto i = find(f.name()), last = end(); i != last;) {
    if ((**i).contains(f)) {
      return i;
    }

    if (f.attributes().free != 0u) {
      i = find(++i, f.name());
    } else {
      break;
    }
  }

  return end();
}

void
set_path_data(feature_set* f, const basic_meta_target* t)
{
  using iter = feature_set::iterator;
  for (auto& i : *f) {
    if ((*i).attributes().path != 0u) {
      (*i).get_path_data().target_ = t;
    }
  }
}

static void
extract_dependency_like_sources(sources_decl& result,
                                const feature_set& fs,
                                const basic_meta_target& relative_to_target,
                                const char* feature_name)
{
  auto i = fs.find(feature_name);
  while (i != fs.end()) {
    source_decl sd_copy = (**i).get_dependency_data().source_;

    if (!sd_copy.target_path().empty() && !sd_copy.target_path_is_global() &&
        relative_to_target.get_project() !=
          (**i).get_path_data().target_->get_project()) {
      const location_t full_target_path =
        ((**i).get_path_data().target_->location() / sd_copy.target_path())
          .normalize();
      const boost::filesystem::path p =
        relative_path(full_target_path, relative_to_target.location());
      sd_copy.target_path(p.string(), sd_copy.type());
    }

    result.push_back(sd_copy);
    i = fs.find(++i, feature_name);
  }
}

void
extract_sources(sources_decl& result,
                const feature_set& fs,
                const basic_meta_target& relative_to_target)
{
  extract_dependency_like_sources(result, fs, relative_to_target, "source");
  extract_dependency_like_sources(result, fs, relative_to_target, "library");
}

void
extract_dependencies(sources_decl& result,
                     const feature_set& fs,
                     const basic_meta_target& relative_to_target)
{
  extract_dependency_like_sources(result, fs, relative_to_target, "dependency");
}

void
extract_uses(sources_decl& result,
             const feature_set& fs,
             const basic_meta_target& relative_to_target)
{
  extract_dependency_like_sources(result, fs, relative_to_target, "use");
}

// FIXME: this is wrong. If we compare two sets with same free features that
// placed in different order
// than we don't detects that difference
bool
feature_set::operator==(const feature_set& rhs) const
{
  if (this == &rhs) {
    return true;
  }

  if (size() != rhs.size()) {
    return false;
  }

  for (auto rh : rhs) {
    if (find(*rh) == end()) {
      return false;
    }
  }

  return true;
}

bool
feature_set::compatible_with(const feature_set& rhs) const
{
  if (this == &rhs) {
    return true;
  }

  const feature_set* rhs_p = &rhs;
  const feature_set* lhs_p = this;

  if (size() < rhs.size()) {
    swap(lhs_p, rhs_p);
  }

  for (auto i : *lhs_p) {
    if (rhs_p->find(*i) == rhs_p->end()) {
      if (((*i).attributes().free != 0u) ||
          ((*i).attributes().generated != 0u) ||
          ((*i).attributes().undefined_ != 0u) ||
          ((*i).attributes().no_defaults != 0u)) {
        return false;
      }
      if (rhs_p->find((*i).name()) == rhs_p->end()) {
        if ((*i).definition().get_default() != (*i).value()) {
          return false;
        }
      } else {
        return false;
      }
    }
  }

  return true;
}

void
feature_set::clear()
{
  features_.clear();
  has_undefined_ = false;
}

bool
feature_set::contains(const feature_set& rhs) const
{
  if (this == &rhs) {
    return true;
  }

  for (auto rh : rhs) {
    if (find(*rh) == end()) {
      return false;
    }
  }

  return true;
}

feature_set*
parse_simple_set(const std::string& s, feature_registry& r)
{
  using namespace boost::spirit::classic;

  vector<string> feature_names, feature_values;
  if (parse(s.begin(),
            s.end(),
            list_p('<' >> (+(anychar_p - '>'))[push_back_a(feature_names)] >>
                     '>' >> (+(anychar_p - '/'))[push_back_a(feature_values)],
                   ch_p('/')))
        .full) {
    feature_set* result = r.make_set();
    for (vector<string>::const_iterator i = feature_names.begin(),
                                        v_i = feature_values.begin(),
                                        last = feature_names.end();
         i != last;
         ++i, ++v_i) {
      result->join(i->c_str(), v_i->c_str());
    }

    return result;
  }
  {
    throw std::runtime_error("Can't parse simple feature set from '" + s + "'");
  }
}

static bool
subf_less_by_name(const subfeature* lhs, const subfeature* rhs)
{
  return lhs->name() < rhs->name();
}

static bool
less_by_name(const feature* lhs, const feature* rhs)
{
  return lhs->name() < rhs->name();
}

static void
dump_value(std::ostream& s, const feature& f)
{
  if (f.attributes().path != 0u) {
    const feature::path_data& pd = f.get_path_data();
    location_t l(f.value());
    if (!l.has_root_name()) {
      l = pd.target_->location() / l;
      l.normalize();
    }

    s << l;
  } else if (f.attributes().dependency != 0u) {
    const feature::dependency_data& dd = f.get_dependency_data();
    s << dd.source_.target_path();
    if (!dd.source_.target_name().empty()) {
      s << "//" << dd.source_.target_name();
    }

    if (f.get_path_data().target_ != nullptr) {
      s << " " << f.get_path_data().target_->location();
    }
  } else {
    s << f.value();
  }
}

static void
dump_for_hash(std::ostream& s, const feature& f)
{
  s << '<' << f.name() << '>';
  dump_value(s, f);

  if (f.subfeatures().empty()) {
    return;
  }

  using subfeatures_t = vector<const hammer::subfeature*>;
  subfeatures_t subfeatures;
  for (auto i : f.subfeatures()) {
    subfeatures.push_back(i);
  }

  if (subfeatures.empty()) {
    return;
  }

  std::sort(subfeatures.begin(), subfeatures.end(), &subf_less_by_name);
  bool first = true;

  s << '(';
  for (subfeatures_t::const_iterator i = subfeatures.begin(),
                                     last = subfeatures.end();
       i != last;
       ++i) {
    if (!first) {
      s << ' ';
      first = false;
    }

    s << '<' << (**i).name() << '>' << (**i).value();
  }
  s << ')';
}

void
dump_for_hash(std::ostream& s, const feature_set& fs, bool dump_all)
{
  if (fs.empty()) {
    s << "<!empty!>";
    return;
  }

  using features_t = vector<const hammer::feature*>;
  features_t features;
  for (auto f : fs) {
    if (dump_all ||
        !(((*f).attributes().free != 0u) ||
          ((*f).attributes().incidental != 0u) ||
          ((*f).attributes().path != 0u) ||
          ((*f).attributes().dependency != 0u) ||
          ((*f).attributes().generated != 0u) ||
          ((*f).attributes().composite != 0u))) {
      features.push_back(f);
    }
  }

  std::stable_sort(features.begin(), features.end(), &less_by_name);

  bool first = true;
  for (features_t::const_iterator i = features.begin(), last = features.end();
       i != last;
       ++i) {
    if (!first) {
      s << (dump_all ? '\n' : ' ');
    } else {
      first = false;
    }

    dump_for_hash(s, **i);
  }
}

std::string
dump_for_hash(const feature_set& fs, bool dump_all)
{
  std::ostringstream s;
  dump_for_hash(s, fs, dump_all);
  return s.str();
}

void
feature_set::erase_all(const std::string& feature_name)
{
  for (auto i = features_.begin(); i != features_.end();) {
    if ((**i).name() == feature_name) {
      i = features_.erase(i);
    } else {
      ++i;
    }
  }

  has_undefined_ = false;
  const feature_def& def = fr_->get_def(feature_name);
  if (def.attributes().undefined_ != 0u) {
    for (features_t::const_iterator i = features_.begin(),
                                    last = features_.end();
         i != last;
         ++i) {
      if ((**i).attributes().undefined_ != 0u) {
        has_undefined_ = true;
        break;
      }
    }
  }
}

void
apply_build_request(feature_set& dest, const feature_set& build_request)
{
  for (auto& i : dest) {
    if ((*i).name() == "use") {
      const source_decl old = (*i).get_dependency_data().source_;
      feature_set& new_props =
        old.properties() == nullptr
          ? *build_request.clone()
          : old.properties()->clone()->join(build_request);
      (*i).set_dependency_data(
        source_decl(
          old.target_path(), old.target_name(), old.type(), &new_props),
        (*i).get_path_data().target_);
    }
  }
}
} // namespace hammer
