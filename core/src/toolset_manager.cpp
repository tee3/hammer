#include "stdafx.h"
#include <hammer/core/engine.h>
#include <hammer/core/feature_def.h>
#include <hammer/core/toolset.h>
#include <hammer/core/toolset_manager.h>

namespace hammer {

void
toolset_manager::add_toolset(std::unique_ptr<toolset> t)
{
  if (!toolsets_.emplace(t->name(), std::move(t)).second) {
    throw std::runtime_error("Toolset '" + t->name() + "' already registered");
  }
}

void
toolset_manager::init_toolset(engine& e,
                              const std::string& toolset_name,
                              const std::string& toolset_version,
                              const location_t* toolset_home) const
{
  auto i = toolsets_.find(toolset_name);
  if (i == toolsets_.end()) {
    throw std::runtime_error("Toolset '" + toolset_name +
                             "' is not registered");
  }

  i->second->init(e, toolset_version, toolset_home);
}

void
toolset_manager::autoconfigure(engine& e) const
{
  for (const auto& toolset : toolsets_) {
    if (const feature_def* fd = e.feature_registry().find_def("toolset")) {
      if (!fd->is_legal_value(toolset.first)) {
        toolset.second->autoconfigure(e);
      }
    }
  }
}

toolset_manager::~toolset_manager() = default;
} // namespace hammer
