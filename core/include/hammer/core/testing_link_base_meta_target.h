#pragma once
#include <hammer/core/typed_meta_target.h>

namespace hammer {

class testing_link_base_meta_target : public typed_meta_target {
   public:
      testing_link_base_meta_target(hammer::project* p,
                                    const std::string& name,
                                    const requirements_decl& req,
                                    const target_type& t);
   protected:
      void instantiate_impl(const main_target* owner,
                            const feature_set& build_request,
                            std::vector<basic_target*>* result,
                            feature_set* usage_requirements) const override;
};

}
