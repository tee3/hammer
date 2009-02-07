#if !defined(h_2be370e7_22b2_4eb1_9167_8042b1065c54)
#define h_2be370e7_22b2_4eb1_9167_8042b1065c54

#include "main_target.h"

namespace hammer
{
   class header_lib_main_target : public main_target
   {
      public:
         header_lib_main_target(const hammer::meta_target* mt, 
                                const pstring& name, 
                                const hammer::type* t, 
                                const feature_set* props,
                                pool& p);
      
      protected:
         virtual void add_additional_dependencies(hammer::build_node& generated_node) const {}
   };
}

#endif //h_2be370e7_22b2_4eb1_9167_8042b1065c54