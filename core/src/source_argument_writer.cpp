#include "stdafx.h"
#include <hammer/core/build_environment.h>
#include <hammer/core/build_node.h>
#include <hammer/core/fs_helpers.h>
#include <hammer/core/main_target.h>
#include <hammer/core/source_argument_writer.h>
#include <hammer/core/target_type.h>

#include <utility>

namespace hammer {

source_argument_writer::source_argument_writer(const std::string& name,
                                               const target_type& t,
                                               bool exact_type,
                                               output_strategy os,
                                               std::string quoting_string,
                                               std::string prefix)
  : targets_argument_writer(name, t)
  , exact_type_(exact_type)
  , output_strategy_(os)
  , quoting_string_(std::move(quoting_string))
  , prefix_(std::move(prefix))
{
}

argument_writer*
source_argument_writer::clone() const
{
  return new source_argument_writer(*this);
}

bool
source_argument_writer::accept(const basic_target& source) const
{
  if (exact_type_) {
    return source.type() == this->source_type();
  }
  {
    return source.type().equal_or_derived_from(this->source_type());
  }
}

void
source_argument_writer::write_impl(std::ostream& output,
                                   const build_node& node,
                                   const build_environment& environment) const
{
  bool first = true;
  for (const auto& source : node.sources_) {
    if (accept(*source.source_target_)) {
      if (!first) {
        output << ' ';
      } else {
        first = false;
      }

      switch (output_strategy_) {
        case RELATIVE_TO_MAIN_TARGET: {
          location_t source_path =
            relative_path(source.source_target_->location(),
                          source.source_target_->get_main_target()->location());
          source_path /= source.source_target_->name();
          source_path.normalize();
          output << quoting_string_ << prefix_ << source_path.string()
                 << quoting_string_;
          break;
        }

        case FULL_PATH: {
          location_t source_path =
            source.source_target_->location() / source.source_target_->name();
          source_path.normalize();
          output << quoting_string_ << prefix_ << source_path.string()
                 << quoting_string_;
          break;
        }

        case RELATIVE_TO_WORKING_DIR: {
          location_t source_path = relative_path(
            source.source_target_->location(),
            environment.working_directory(*source.source_target_));
          source_path /= source.source_target_->name();
          source_path.normalize();
          output << quoting_string_ << prefix_ << source_path.string()
                 << quoting_string_;
          break;
        }

        case WITHOUT_PATH: {
          location_t source_path(source.source_target_->name());
          output << quoting_string_ << prefix_ << source_path.string()
                 << quoting_string_;
          break;
        }

        default:
          throw std::runtime_error("Unknown output strategy");
      }
    }
  }
}
} // namespace hammer
