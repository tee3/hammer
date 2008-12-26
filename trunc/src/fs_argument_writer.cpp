#include "stdafx.h"
#include "fs_argument_writer.h"
#include "feature_set.h"
#include "build_node.h"

namespace hammer{

void fs_argument_writer::write_impl(std::ostream& output, const build_node& node, const build_environment& environment) const
{
   const feature_set& build_request = node.build_request();
   for(patterns_t::const_iterator i = patterns_.begin(), last = patterns_.end(); i != last; ++i)
      if (build_request.contains(*i->first))
         output << i->second << ' ';
}

fs_argument_writer& fs_argument_writer::add(const feature_set* pattern, const std::string& what_write)
{
   patterns_.push_back(std::make_pair(pattern, what_write));
   return *this;
}

fs_argument_writer& fs_argument_writer::add(const std::string& pattern, const std::string& what_write)
{
   add(parse_simple_set(pattern, *feature_registry_), what_write);
   return *this;
}

fs_argument_writer* fs_argument_writer::clone() const
{
   return new fs_argument_writer(*this);
}

}