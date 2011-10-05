#if !defined(h_eb13d599_e718_4e48_b326_bc0cd73665de)
#define h_eb13d599_e718_4e48_b326_bc0cd73665de

#include "generator.h"

namespace hammer
{
   class pch_generator : public generator
   {
      public:
         pch_generator(hammer::engine& e,
                       const std::string& name,
                       const consumable_types_t& source_types,
                       const producable_types_t& target_types,
                       bool composite,
                       const feature_set* c = 0);
         
         virtual build_nodes_t
         construct(const target_type& type_to_construct, 
                   const feature_set& props,
                   const std::vector<boost::intrusive_ptr<build_node> >& sources,
                   const basic_target* t,
                   const pstring* composite_target_name,
                   const main_target& owner) const;

      private:
         const target_type& c_type_;
         const target_type& cpp_type_;
         const target_type& h_type_;
   };
}

#endif //h_eb13d599_e718_4e48_b326_bc0cd73665de
