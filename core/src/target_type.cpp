#include "stdafx.h"
#include <algorithm>
#include <boost/format.hpp>
#include <cassert>
#include <hammer/core/feature_set.h>
#include <hammer/core/target_type.h>
#include <hammer/core/type_registry.h>
#include <utility>

using namespace std;

namespace hammer {

target_type::target_type(type_tag tag,
                         const suffix_def& suffix,
                         const prefix_def& prefix)
  : tag_(std::move(tag))
  , suffixes_(1, suffix)
  , prefixes_(1, prefix)
  , base_(nullptr)
{
}

target_type::target_type(type_tag tag,
                         const suffix_def& suffix,
                         const target_type& base,
                         const prefix_def& prefix)
  : tag_(std::move(tag))
  , suffixes_(1, suffix)
  , prefixes_(1, prefix)
  , base_(&base)
{
}

target_type::target_type(type_tag tag, suffixes_t suffixes, prefixes_t prefixes)
  : tag_(std::move(tag))
  , suffixes_(std::move(suffixes))
  , base_(nullptr)
  , prefixes_(std::move(prefixes))
{
}

target_type::target_type(type_tag tag,
                         suffixes_t suffixes,
                         const target_type& base,
                         prefixes_t prefixes)
  : tag_(std::move(tag))
  , suffixes_(std::move(suffixes))
  , base_(&base)
  , prefixes_(std::move(prefixes))
{
}

bool
target_type::equal(const target_type& rhs) const
{
  return tag() == tag() && suffixes().size() == rhs.suffixes().size() &&
         std::equal(
           suffixes().begin(), suffixes().end(), rhs.suffixes().begin());
}

bool
target_type::suffix_def::operator==(const suffix_def& rhs) const
{
  return suffix_ == rhs.suffix_ && *condition_ == *rhs.condition_;
}

bool
target_type::equal_or_derived_from(const target_type& rhs) const
{
  assert(owner_ != nullptr &&
         "To perform this operation owner_ must be not NULL.");

  if (owner_ != rhs.owner_) {
    return equal(rhs);
  }

  if (this == &rhs) {
    return true;
  }
  if (base() != nullptr) {
    return base()->equal_or_derived_from(rhs);
  } else {
    return false;
  }
}

bool
target_type::equal_or_derived_from(const type_tag& rhs) const
{
  assert(owner_ != nullptr &&
         "To perform this operation owner_ must be not NULL.");

  return equal_or_derived_from(owner_->get(rhs));
}

bool
target_type::operator==(const target_type& rhs) const
{
  assert(owner_ != nullptr &&
         "To perform this operation owner_ must be not NULL.");
  if (owner_ != rhs.owner_) {
    return equal(rhs);
  }

  return this == &rhs;
}

const std::string&
target_type::suffix_for(const std::string& s,
                        const feature_set& /*environment*/) const
{
  for (target_type::suffixes_t::const_iterator i = suffixes_.begin(),
                                               last = suffixes_.end();
       i != last;
       ++i) {
    string::size_type p = s.rfind(i->suffix_);
    if (p != string::npos && p + i->suffix_.size() == s.size()) {
      return i->suffix_;
    }
  }

  throw std::runtime_error(
    (boost::format("Type '%s' can't find suffix '%s'.") % tag_.name() % s)
      .str());
}

const std::string&
target_type::suffix_for(const feature_set& environment) const
{
  for (const auto& suffixe : suffixes_) {
    if (suffixe.condition_ == nullptr ||
        environment.contains(*suffixe.condition_)) {
      return suffixe.suffix_;
    }
  }

  throw std::runtime_error(
    (boost::format("Type '%s' can't find suffix in environment[FIXME]") %
     tag_.name())
      .str());
}

static std::string empty_prefix;
const std::string&
target_type::prefix_for(const feature_set& environment) const
{
  for (const auto& prefixe : prefixes_) {
    if (prefixe.condition_ == nullptr ||
        environment.contains(*prefixe.condition_)) {
      return prefixe.suffix_;
    }
  }

  return empty_prefix;
}

std::unique_ptr<target_type>
target_type::clone(const type_registry& tr) const
{
  if (base() != nullptr && base()->owner_ != &tr) {
    throw std::logic_error(
      "[type] Can't clone because base type is not registered in owner.");
  }

  std::unique_ptr<target_type> result(
    base() == nullptr
      ? new target_type(tag(), suffixes())
      : new target_type(tag(), suffixes(), *base(), prefixes()));
  result->owner_ = &tr;
  return std::move(result);
}
} // namespace hammer
