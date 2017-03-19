#include <hammer/core/requirements_decl.h>
#include <hammer/core/warehouse_project.h>

namespace hammer {

warehouse_project::warehouse_project(engine& e,
                                     const std::string& name,
                                     const location_t& location)
  : project(&e, name, location, requirements_decl(), requirements_decl())
{
}

warehouse_project::~warehouse_project() = default;
} // namespace hammer
