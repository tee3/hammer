#pragma once
#include <hammer/core/testing_link_base_meta_target.h>

namespace hammer {

class testing_link_meta_target : public testing_link_base_meta_target {
   public:
      testing_link_meta_target(hammer::project* p,
                               const std::string& name,
                               const requirements_decl& req);
};

}
