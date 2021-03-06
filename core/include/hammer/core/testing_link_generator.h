#pragma once
#include <hammer/core/testing_compile_link_base_generator.h>

namespace hammer {

class testing_link_generator : public testing_compile_link_base_generator {
   public:
      testing_link_generator(engine& e,
                             std::unique_ptr<generator> link_generator);
};

}
