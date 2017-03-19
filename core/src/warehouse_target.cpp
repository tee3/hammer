#include <hammer/core/engine.h>
#include <hammer/core/feature_set.h>
#include <hammer/core/meta_target.h>
#include <hammer/core/warehouse_target.h>

namespace hammer {

warehouse_target::warehouse_target(const basic_meta_target& mt,
                                   const std::string& name,
                                   const feature_set& build_request)
  : main_target(&mt, name, nullptr, build_request.clone())
{
}

warehouse_target::~warehouse_target() = default;

build_nodes_t
warehouse_target::generate() const
{
  throw warehouse_unresolved_target_exception();
}

warehouse_unresolved_target_exception::warehouse_unresolved_target_exception()
  : std::runtime_error("Build tree contain unresolved warehouse lib")
{
}
} // namespace hammer
