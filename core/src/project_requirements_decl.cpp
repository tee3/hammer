#include "stdafx.h"
#include <hammer/core/project_requirements_decl.h>

#include <utility>

namespace hammer {

project_requirements_decl::project_requirements_decl(std::string name,
                                                     const requirements_decl& r)
  : name_(std::move(name))
  , requirements_(r)
{
}
} // namespace hammer
