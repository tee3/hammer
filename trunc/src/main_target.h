#if !defined(h_e7c984c0_a4f6_42e2_89b9_ec70352d581c)
#define h_e7c984c0_a4f6_42e2_89b9_ec70352d581c

#include "basic_target.h"
#include "pstring.h"
#include <vector>

namespace hammer
{
   class meta_target;
   class feature_set;

   class main_target : public basic_target
   {
      public:
         typedef std::vector<basic_target*> sources_t;

         main_target(const meta_target* mt, 
                     const pstring& name, 
                     const hammer::type* t, 
                     const feature_set* props,
                     pool& p);

         void sources(const std::vector<basic_target*>& srcs);
         const sources_t& sources() const { return sources_; }
         const hammer::meta_target* meta_target() const { return mt_; }
         const feature_set* properties() const { return properties_; }

      private:
         const hammer::meta_target* mt_;
         sources_t sources_;
         const feature_set* properties_;
   };
}

#endif //h_e7c984c0_a4f6_42e2_89b9_ec70352d581c
