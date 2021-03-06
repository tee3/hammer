#include "stdafx.h"
#include <boost/thread/mutex.hpp>
#include <boost/ptr_container/ptr_unordered_map.hpp>
#include <hammer/core/build_environment.h>

namespace hammer{

struct build_environment::impl_t
{
   typedef boost::ptr_unordered_map<std::ostream*, std::ostream> streams_t;
   
   impl_t() : should_buffer_(false) {}   

   boost::mutex m_;
   bool should_buffer_;
   streams_t streams_;
   std::stringstream buffer_;
};

build_environment::build_environment() : impl_(new impl_t)
{
}

build_environment::~build_environment()
{
   delete impl_;
}

std::ostream& build_environment::begin_use_output_stream() const
{
   boost::mutex::scoped_lock lk(impl_->m_);
   if (impl_->should_buffer_)
   {
      std::ostream* s = new std::stringstream;
      impl_->streams_.insert(s, s);
      return *s;
   }
   else
   {
      impl_->should_buffer_ = true;
      return output_stream();
   }
}

void build_environment::end_use_output_stream(std::ostream& s) const
{
   boost::mutex::scoped_lock lk(impl_->m_);
   if (&s == &output_stream())
   {
      impl_->should_buffer_ = false;
      output_stream() << impl_->buffer_.str();
      impl_->buffer_.str(std::string());
   }
   else
   {
      impl_->buffer_ << static_cast<std::stringstream&>(s).str();
      assert(impl_->streams_.find(&s) != impl_->streams_.end());
      impl_->streams_.erase(&s);
   }
}

}
