#include "stdafx.h"
#include <boost/checked_delete.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tokenizer.hpp>
#include <cassert>
#include <cstdlib>
#include <hammer/core/feature.h>
#include <hammer/core/feature_registry.h>
#include <hammer/core/feature_set.h>
#include <hammer/core/subfeature.h>
#include <list>
#include <map>
#include <utility>

using std::string;
using std::unique_ptr;
using namespace boost;
using namespace boost::multi_index;

namespace hammer {

struct feature_registry::impl_t
{
  ~impl_t();

  struct generic_less_comparator
  {
    static const bool equality_result = false;

    template<typename T1, typename T2>
    bool operator()(const T1& lhs, const T2& rhs) const
    {
      return lhs < rhs;
    }
  };

  struct generic_greater_equal_comparator
  {
    static const bool equality_result = true;

    template<typename T1, typename T2>
    bool operator()(const T1& lhs, const T2& rhs) const
    {
      return !(lhs < rhs || lhs == rhs);
    }
  };

  struct subfeature_storage_item
  {
    const feature_def* feature_def_{};
    boost::shared_ptr<subfeature> subfeature_;
  };

  struct subfeature_find_data
  {
    subfeature_find_data(const feature_def& fdef,
                         const subfeature_def& sdef,
                         const std::string& value)
      : fdef_(fdef)
      , sdef_(sdef)
      , value_(value)
    {
    }

    const feature_def& fdef_;
    const subfeature_def& sdef_;
    const std::string& value_;
  };

  struct subfeature_storage_item_comparator
  {
    bool operator()(const subfeature_storage_item& lhs,
                    const subfeature_storage_item& rhs) const
    {
      if (lhs.feature_def_ != rhs.feature_def_) {
        return lhs.feature_def_ < rhs.feature_def_;
      }

      if (lhs.subfeature_.get() != rhs.subfeature_.get()) {
        return lhs.subfeature_.get() < rhs.subfeature_.get();
      }

      return false;
    }

    template<typename Comparator>
    bool compare_impl(const subfeature_storage_item& lhs,
                      const subfeature_find_data& rhs,
                      Comparator comparator) const
    {
      if (lhs.feature_def_ != &rhs.fdef_) {
        return comparator(lhs.feature_def_, &rhs.fdef_);
      }

      if (&lhs.subfeature_->definition() != &rhs.sdef_) {
        return comparator(&lhs.subfeature_->definition(), &rhs.sdef_);
      }

      if (lhs.subfeature_->value() != rhs.value_) {
        return comparator(lhs.subfeature_->value(), rhs.value_);
      }

      return false;
    }

    bool operator()(const subfeature_storage_item& lhs,
                    const subfeature_find_data& rhs) const
    {
      return compare_impl(lhs, rhs, generic_less_comparator());
    }

    bool operator()(const subfeature_find_data& lhs,
                    const subfeature_storage_item& rhs) const
    {
      return compare_impl(rhs, lhs, generic_greater_equal_comparator());
    }
  };

  struct find_feature_data
  {
    find_feature_data(const feature_def& def,
                      const string& value,
                      feature::subfeatures_t subfeatures)
      : def_(def)
      , value_(value)
      , subfeatures_(std::move(subfeatures))
    {
    }

    const feature_def& def_;
    const string& value_;
    const feature::subfeatures_t subfeatures_;
  };

  struct feature_storage_comparator
  {
    bool operator()(const feature& lhs, const feature& rhs) const
    {
      if (&lhs == &rhs) {
        return false;
      }

      if (&lhs.definition() != &rhs.definition()) {
        return &lhs.definition() < &rhs.definition();
      }

      if (lhs.value() != rhs.value()) {
        return lhs.value() < rhs.value();
      }

      if (lhs.subfeatures().size() != rhs.subfeatures().size()) {
        return lhs.subfeatures().size() < rhs.subfeatures().size();
      }

      // FIXME: this is due msvc-8.0 debug iterator bug
      if (lhs.subfeatures().empty()) {
        return false;
      }

      for (auto i = lhs.subfeatures().begin(),
                last = lhs.subfeatures().end(),
                outer = rhs.subfeatures().begin();
           i != last;
           ++i, ++outer) {
        if (*i != *outer) {
          return *i < *outer;
        }
      }

      return false;
    }

    template<typename Comparator>
    bool compare_impl(const feature& lhs,
                      const find_feature_data& rhs,
                      Comparator comparator) const
    {
      if (&lhs.definition() != &rhs.def_) {
        return comparator(&lhs.definition(), &rhs.def_);
      }

      if (lhs.value() != rhs.value_) {
        return comparator(lhs.value(), rhs.value_);
      }

      if (lhs.subfeatures().size() != rhs.subfeatures_.size()) {
        return comparator(lhs.subfeatures().size(), rhs.subfeatures_.size());
      }

      // FIXME: this is due msvc-8.0 debug iterator bug
      if (lhs.subfeatures().empty()) {
        return false;
      }

      using iter = feature::subfeatures_t::const_iterator;
      for (auto i = lhs.subfeatures().begin(),
                last = lhs.subfeatures().end(),
                outer = rhs.subfeatures_.begin();
           i != last;
           ++i, ++outer) {
        if (*i != *outer) {
          return comparator(*i, *outer);
        }
      }

      return false;
    }

    bool operator()(const feature& lhs, const find_feature_data& rhs) const
    {
      return compare_impl(lhs, rhs, generic_less_comparator());
    }

    bool operator()(const find_feature_data& lhs, const feature& rhs) const
    {
      return compare_impl(rhs, lhs, generic_greater_equal_comparator());
    }
  };

  struct feature_storage_key_extractor
  {
    using result_type = const hammer::feature&;
    result_type operator()(const shared_ptr<feature>& v) const { return *v; }
  };

  typedef std::map<std::string, std::unique_ptr<feature_def>> defs_t;
  using feature_set_storage_t = std::list<feature_set*>;
  using non_cached_features_t = boost::ptr_vector<feature>;

  typedef multi_index_container<
    boost::shared_ptr<feature>,
    indexed_by<ordered_unique<feature_storage_key_extractor,
                              feature_storage_comparator>>>
    features_t;
  using main_feature_index_t = features_t::nth_index<0>::type;

  typedef multi_index_container<
    subfeature_storage_item,
    indexed_by<ordered_unique<identity<subfeature_storage_item>,
                              subfeature_storage_item_comparator>>>
    subfeatures_t;
  using main_subfeature_index_t = subfeatures_t::nth_index<0>::type;

  feature_def* find_def(const std::string& name);
  feature* find_feature(const std::string& name, const string& value);
  feature* find_feature(const feature& f, const subfeature& sf);
  subfeature& create_subfeature(const feature& f,
                                const string& name,
                                const string& value);

  defs_t defs_;
  feature_set_storage_t feature_set_list_;
  features_t features_;
  non_cached_features_t non_cached_features_;
  subfeatures_t subfeatures_;
  feature_set* singleton_{};
};

feature_def*
feature_registry::impl_t::find_def(const std::string& name)
{
  auto i = defs_.find(name);
  if (i == defs_.end()) {
    return nullptr;
  }
  {
    return i->second.get();
  }
}

feature*
feature_registry::impl_t::find_feature(const string& name, const string& value)
{
  feature_def* def = find_def(name);
  if (def == nullptr) {
    return nullptr;
  }

  main_feature_index_t::iterator i = features_.get<0>().find(
    find_feature_data(*def, value, feature::subfeatures_t()));
  if (i == features_.get<0>().end()) {
    return nullptr;
  }
  {
    return i->get();
  }
}

subfeature&
feature_registry::impl_t::create_subfeature(const feature& f,
                                            const string& name,
                                            const string& value)
{
  const subfeature_def* sdef = f.definition().find_subfeature(name);
  if (sdef == nullptr) {
    throw std::runtime_error("Feature '" + f.name() +
                             "' does not have subfeature '" + name + "'.");
  }

  if (!sdef->is_legal_value(f.value(), value)) {
    throw std::runtime_error("Value '" + value +
                             "' is not a legal value for subfeature '" + name +
                             "'.");
  }

  main_subfeature_index_t::iterator i = subfeatures_.get<0>().find(
    subfeature_find_data(f.definition(), *sdef, value));
  if (i != subfeatures_.get<0>().end()) {
    return *i->subfeature_;
  }
  subfeature_storage_item item;
  item.feature_def_ = &f.definition();
  item.subfeature_.reset(new subfeature(*sdef, value));
  subfeature* result = item.subfeature_.get();
  subfeatures_.get<0>().insert(item);
  return *result;
}

feature*
feature_registry::impl_t::find_feature(const feature& f, const subfeature& sf)
{
  feature::subfeatures_t subfeatures(f.subfeatures());
  subfeatures.push_back(&sf);
  main_feature_index_t::iterator i = features_.get<0>().find(
    find_feature_data(f.definition(), f.value(), subfeatures));
  if (i == features_.get<0>().end()) {
    return nullptr;
  }
  {
    return i->get();
  }
}

feature_registry::impl_t::~impl_t()
{
  for (auto& i : feature_set_list_) {
    delete i;
  }
}

using impl_t = feature_registry::impl_t;

feature_registry::feature_registry()
  : impl_(new impl_t)
{
  impl_->singleton_ = make_set();
}

feature_registry::~feature_registry()
{
  delete impl_;
}

feature_set*
feature_registry::make_set()
{
  unique_ptr<feature_set> r(new feature_set(this));
  impl_->feature_set_list_.push_back(r.get());

  return r.release();
}

const feature_set&
feature_registry::singleton() const
{
  return *impl_->singleton_;
}

feature_def&
feature_registry::add_feature_def(
  const std::string& name,
  const feature_def::legal_values_t& legal_values,
  feature_attributes attributes)
{
  auto i = impl_->defs_.find(name);
  if (i != impl_->defs_.end()) {
    throw std::runtime_error("Definition for feature '" + name +
                             "' already registered");
  }

  auto r = impl_->defs_.insert(std::move(std::make_pair(
    name,
    unique_ptr<feature_def>(new feature_def(name, legal_values, attributes)))));
  return *r.first->second;
}

feature*
feature_registry::simply_create_feature(const std::string& name,
                                        const std::string& value)
{
  feature* result;
  const feature_def* maybe_def = find_def(name.c_str());
  if (maybe_def == nullptr) {
    // no such feature definition found
    // create def with undefined attribute
    feature_attributes fa = { 0 };
    fa.undefined_ = fa.no_checks = 1;
    maybe_def = &add_feature_def(name, feature_def::legal_values_t(), fa);
  }

  const feature_def& def = *maybe_def;

  if ((def.attributes().path != 0u) || (def.attributes().dependency != 0u) ||
      (def.attributes().generated != 0u) ||
      (def.attributes().undefined_ != 0u)) {
    unique_ptr<feature> f(new feature(&def, value));
    result = f.get();
    impl_->non_cached_features_.push_back(f.get());
    f.release();
  } else {
    result = impl_->find_feature(name, value);
    if (result == nullptr) {
      shared_ptr<feature> f(new feature(&def, value));
      result = f.get();
      impl_->features_.get<0>().insert(f);
    }
  }

  return result;
}

feature*
feature_registry::create_feature(const std::string& name,
                                 const std::string& value)
{
  if (name.empty()) {
    throw std::runtime_error("Can't create feature without name");
  }

  const feature_def* posible_feature = find_def(name.c_str());

  if (posible_feature != nullptr &&
      ((posible_feature->attributes().free != 0u) ||
       posible_feature->is_legal_value(value))) {
    return simply_create_feature(name, value);
  }

  typedef boost::tokenizer<boost::char_separator<char>, const char*> tokenizer;
  tokenizer tok(value.c_str(),
                value.c_str() + value.size(),
                boost::char_separator<char>("-"));
  tokenizer::const_iterator first = tok.begin(), last = tok.end();
  if (first != last) {
    feature* result = simply_create_feature(name, *first);
    const string feature_value = *first;
    ++first;

    for (; first != last; ++first) {
      const subfeature_def* sdef =
        result->definition().find_subfeature_for_value(feature_value, *first);
      if (sdef == nullptr) {
        throw std::runtime_error("Can't find subfeature with legal value '" +
                                 *first + "' for feature '" + name + "'.");
      }

      result = create_feature(*result, sdef->name(), *first);
    }

    return result;
  }

  return simply_create_feature(name, value);
}

feature_set*
feature_registry::add_defaults(feature_set* s)
{
  using iter = impl_t::defs_t::const_iterator;
  for (iter i = impl_->defs_.begin(), last = impl_->defs_.end(); i != last;
       ++i) {
    if ((i->second->attributes().optional == 0u) &&
        (i->second->attributes().free == 0u) &&
        (i->second->attributes().no_defaults == 0u) &&
        s->find(i->first.c_str()) == s->end()) {
      s->join(create_feature(i->first, i->second->get_default()));
    }
  }

  return s;
}

feature_def&
feature_registry::get_def(const std::string& name)
{
  feature_def* result = impl_->find_def(name);
  if (result == nullptr) {
    throw std::runtime_error("There is no feature definition for feature '" +
                             name + "'.");
  } else {
    return *result;
  }
}

const feature_def*
feature_registry::find_def(const char* feature_name) const
{
  return impl_->find_def(feature_name);
}

const feature_def*
feature_registry::find_def_from_full_name(const char* feature_name) const
{
  const feature_def* result = find_def(feature_name);
  if (result != nullptr) {
    return result;
  }
  // FIXME: performance hit
  string feature_name_str(feature_name);
  string::size_type p = feature_name_str.find('-');
  if (p != string::npos) {
    return find_def(feature_name_str.substr(0, p).c_str());
  }
  {
    return nullptr;
  }
}

feature*
feature_registry::create_feature(const feature& f,
                                 const string& subfeature_name,
                                 const string& subfeature_value)
{
  subfeature& sf =
    impl_->create_subfeature(f, subfeature_name, subfeature_value);
  feature* result = impl_->find_feature(f, sf);
  if (result == nullptr) {
    feature::subfeatures_t new_subfeatures(f.subfeatures());
    new_subfeatures.push_back(&sf);
    shared_ptr<feature> shared_result(
      new feature(&f.definition(), f.value(), new_subfeatures));
    result = shared_result.get();
    impl_->features_.get<0>().insert(shared_result);
  }

  return result;
}

feature*
feature_registry::clone_feature(const feature& f)
{
  feature* result = create_feature(f.name(), f.value());
  if (f.attributes().dependency != 0u) {
    result->set_dependency_data(f.get_dependency_data().source_,
                                f.get_path_data().target_);
  }

  if (f.attributes().path != 0u) {
    result->get_path_data() = f.get_path_data();
  }

  if (f.attributes().generated != 0u) {
    result->get_generated_data() = f.get_generated_data();
  }

  return result;
}
} // namespace hammer
