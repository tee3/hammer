#pragma once
#include <hammer/core/generator.h>

namespace hammer {

class generator_registry;

class header_lib_generator : public generator {
   public:
      header_lib_generator(hammer::engine& e,
                           const std::string& name,
                           const consumable_types_t& source_types,
                           const producable_types_t& target_types);
      build_nodes_t
      construct(const target_type& type_to_construct,
                const feature_set& props,
                const std::vector<boost::intrusive_ptr<build_node> >& sources,
                const basic_build_target* t,
                const std::string* composite_target_name,
                const main_target& owner) const override;
   private:
      const target_type& header_type_;
      const target_type& shared_lib_;
      const target_type& static_lib_;
      const target_type& searched_lib_;
};

void add_header_lib_generator(engine& e,
                              generator_registry& gr);

}
