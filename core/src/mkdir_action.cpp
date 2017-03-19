#include "stdafx.h"
#include <hammer/core/build_environment.h>
#include <hammer/core/fs_helpers.h>
#include <hammer/core/mkdir_action.h>

#include <utility>

namespace hammer {

mkdir_action::mkdir_action(location_t dir_to_create)
  : build_action("mkdir")
  , dir_to_create_(std::move(dir_to_create))
{
}

bool
mkdir_action::execute_impl(const build_node& /*node*/,
                           const build_environment& environment) const
{
  environment.create_directories(
    relative_path(dir_to_create_, environment.current_directory()));
  return true; // FIXME: That not always true :)
}

std::string
mkdir_action::target_tag(const build_node& /*node*/,
                         const build_environment& environment) const
{
  location_t t = relative_path(dir_to_create_, environment.current_directory());
  return t.string();
}
} // namespace hammer
