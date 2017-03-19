#include "stdafx.h"
#include <hammer/core/basic_target.h>
#include <hammer/core/build_action.h>
#include <hammer/core/build_environment.h>
#include <hammer/core/build_node.h>

using namespace std;

namespace hammer {

bool
build_action::execute(const build_node& node,
                      const build_environment& environment) const
{
  string tag(target_tag(node, environment));
  environment.output_stream() << name() << ' ' << tag << '\n';

  bool execution_result = false;
  try {
    execution_result = execute_impl(node, environment);
  } catch (const std::exception& e) {
    environment.output_stream() << "error: " << e.what() << '\n';
  }

  if (!execution_result) {
    environment.output_stream() << "...failed " << name() << ' ' << tag << '\n';
    environment.output_stream() << "...cleaning " << tag << '\n';
    clean_on_fail(node, environment);
  }

  return execution_result;
}

build_action::~build_action() = default;

void
build_action::clean_on_fail(const build_node& node,
                            const build_environment& environment) const
{
  for (auto product : node.products_) {
    location_t target_path = (*product).location() / (*product).name();
    environment.remove(target_path);
  }
}
} // namespace hammer
