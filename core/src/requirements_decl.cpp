#include "stdafx.h"
#include <boost/ptr_container/ptr_vector.hpp>
#include <hammer/core/feature.h>
#include <hammer/core/feature_registry.h>
#include <hammer/core/feature_set.h>
#include <hammer/core/requirements_decl.h>
#include <utility>

namespace hammer {

struct requirements_decl::impl_t
{
  impl_t() {}

  using requirements_t = boost::ptr_vector<requirement_base>;
  using const_iterator = requirements_t::const_iterator;
  using iterator = requirements_t::iterator;

  impl_t* clone() const;
  requirements_t requirements_;
  mutable unsigned int ref_counter_{ 1 };
};

requirements_decl::impl_t*
requirements_decl::impl_t::clone() const
{
  std::unique_ptr<impl_t> result(new impl_t);

  for (const auto& requirement : requirements_) {
    result->requirements_.push_back(requirement.clone()); //  FIX: memory leak
  }

  return result.release();
}

requirements_decl::requirements_decl()
  : impl_(new impl_t)
{
}

requirements_decl::requirements_decl(const requirements_decl& rhs)
  : impl_(rhs.impl_)
{
  ++impl_->ref_counter_;
}

requirements_decl&
requirements_decl::operator=(const requirements_decl& rhs)
{
  if (impl_ != rhs.impl_) {
    if (--impl_->ref_counter_ == 0) {
      delete impl_;
    }

    impl_ = rhs.impl_;
    ++impl_->ref_counter_;
  }

  return *this;
}

void
requirements_decl::add(std::unique_ptr<requirement_base> r)
{
  if (impl_->ref_counter_ > 1) {
    impl_t* old = impl_;
    impl_ = impl_->clone();
    --old->ref_counter_;
  }

  impl_->requirements_.push_back(std::move(r));
}

void
requirements_decl::add(const feature& f)
{
  std::unique_ptr<requirement_base> r(
    new just_feature_requirement(const_cast<feature*>(&f)));
  add(std::move(r));
}

requirements_decl::~requirements_decl()
{
  if (--impl_->ref_counter_ == 0) {
    delete impl_;
  }
}

void
requirements_decl::eval(const feature_set& build_request,
                        feature_set* result,
                        feature_set* public_result) const
{
  for (impl_t::requirements_t::const_iterator i = impl_->requirements_.begin(),
                                              last = impl_->requirements_.end();
       i != last;
       ++i) {
    i->eval(build_request, result, public_result);
  }
}

void
linear_and_condition::eval(const feature_set& build_request,
                           feature_set* result,
                           feature_set* public_result) const
{
  bool satisfy = true;
  for (auto feature : features_) {
    if (build_request.contains(*feature) != build_request.end() ||
        result->contains(*feature) != result->end()) {
      continue;
    }
    if (build_request.find((*feature).name()) != build_request.end() ||
        result->find((*feature).name()) != result->end() ||
        (*feature).definition().get_default() != (*feature).value()) {
      satisfy = false;
      break;
    }
  }

  if (satisfy) {
    result->join(result_);

    if (is_public() && public_result != nullptr) {
      public_result->join(result_);
    }
  }
}

void
linear_and_condition::setup_path_data(const basic_meta_target* t)
{
  if ((result_->attributes().path != 0u) ||
      (result_->attributes().dependency != 0u)) {
    result_->get_path_data().target_ = t;
  }
}

void
just_feature_requirement::eval(const feature_set& /*build_request*/,
                               feature_set* result,
                               feature_set* public_result) const
{
  result->join(f_);

  if (is_public() && public_result != nullptr) {
    public_result->join(f_);
  }
}

void
just_feature_requirement::setup_path_data(const basic_meta_target* t)
{
  if ((f_->attributes().path != 0u) || (f_->attributes().dependency != 0u)) {
    f_->get_path_data().target_ = t;
  }
}

void
linear_and_condition::add(feature* c)
{
  features_.push_back(c);
}

void
requirements_decl::setup_path_data(const basic_meta_target* t)
{
  for (auto& requirement : impl_->requirements_) {
    requirement.setup_path_data(t);
  }
}

void
requirements_decl::insert_infront(const requirements_decl& v)
{
  for (impl_t::const_iterator i = v.impl_->requirements_.begin(),
                              last = v.impl_->requirements_.end();
       i != last;
       ++i) {
    impl_->requirements_.insert(impl_->requirements_.begin(), i->clone());
  }
}

void
requirements_decl::insert(const requirements_decl& v)
{
  for (impl_t::const_iterator i = v.impl_->requirements_.begin(),
                              last = v.impl_->requirements_.end();
       i != last;
       ++i) {
    impl_->requirements_.insert(impl_->requirements_.end(), i->clone());
  }
}
} // namespace hammer
