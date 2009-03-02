#if !defined(h_7553548b_5699_4e9c_892a_29792be6110b)
#define h_7553548b_5699_4e9c_892a_29792be6110b

#include <hammer/core/main_target.h>

namespace hammer
{
   class obj_main_target : public main_target
   {
      public:
         obj_main_target(const hammer::meta_target* mt,
                         const main_target& owner,
                         const pstring& name, 
                         const hammer::type* t, 
                         const feature_set* props,
                         pool& p);

      protected:
         virtual location_t intermediate_dir_impl() const;
      
      private:
         const main_target& owner_;
   };
}

#endif //h_7553548b_5699_4e9c_892a_29792be6110b