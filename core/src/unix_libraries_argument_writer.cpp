#include "stdafx.h"
#include <hammer/core/basic_target.h>
#include <hammer/core/build_environment.h>
#include <hammer/core/build_node.h>
#include <hammer/core/engine.h>
#include <hammer/core/target_type.h>
#include <hammer/core/type_registry.h>
#include <hammer/core/types.h>
#include <hammer/core/unix_libraries_argument_writer.h>

namespace hammer {

unix_libraries_argument_writer::unix_libraries_argument_writer(
  const std::string& name,
  linker_type::value linker,
  engine& e)
  : argument_writer(name)
  , linker_type_(linker)
  , shared_lib_type_(&e.get_type_registry().get(types::SHARED_LIB))
  , static_lib_type_(&e.get_type_registry().get(types::STATIC_LIB))
  , searched_shared_lib_type_(
      &e.get_type_registry().get(types::SEARCHED_SHARED_LIB))
  , searched_static_lib_type_(
      &e.get_type_registry().get(types::SEARCHED_STATIC_LIB))
  , import_lib_type_(&e.get_type_registry().get(types::IMPORT_LIB))
{
}

unix_libraries_argument_writer*
unix_libraries_argument_writer::clone() const
{
  return new unix_libraries_argument_writer(*this);
}

void
unix_libraries_argument_writer::write_impl(
  std::ostream& output,
  const build_node& node,
  const build_environment& /*environment*/) const
{
  // bool first = true; // unused variable first
  output << "-Wl,--start-group ";
  for (const auto& source : node.sources_) {
    if (source.source_target_->type().equal_or_derived_from(
          *searched_static_lib_type_)) {
      output << " -Wl,-Bstatic -l" << source.source_target_->name();
    } else if (source.source_target_->type().equal_or_derived_from(
                 *searched_shared_lib_type_)) {
      output << " -Wl,-Bdynamic -l" << source.source_target_->name();
    } else if (source.source_target_->type().equal_or_derived_from(
                 *static_lib_type_) ||
               source.source_target_->type().equal_or_derived_from(
                 *shared_lib_type_) ||
               source.source_target_->type().equal_or_derived_from(
                 *import_lib_type_)) {
      location_t source_path =
        source.source_target_->location() / source.source_target_->name();
      source_path.normalize();
      output << R"( ")" << source_path.string() << '"';
    }
  }

  output << " -Wl,--end-group ";
}
} // namespace hammer
