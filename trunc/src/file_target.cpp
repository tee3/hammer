#include "stdafx.h"
#include <boost/filesystem/operations.hpp>
#include <boost/date_time/posix_time/conversion.hpp>
#include "file_target.h"
#include "main_target.h"

namespace hammer
{

std::vector<boost::intrusive_ptr<build_node> > file_target::generate()
{
   return std::vector<boost::intrusive_ptr<build_node> >();
}

void 
file_target::timestamp_info_impl(timestamp_info_t::getter_policy_t how_to_get) const
{
   boost::filesystem::path p(mtarget()->location() / name().to_string());
   timestamp_info_.is_unknown_ = false;
   if (exists(p))
      timestamp_info_.timestamp_ = boost::posix_time::from_time_t(last_write_time(p));
   else
      timestamp_info_.timestamp_ = boost::posix_time::ptime(boost::date_time::pos_infin);
}

}
