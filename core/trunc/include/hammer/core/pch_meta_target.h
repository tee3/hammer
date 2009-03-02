#if !defined(h_0aada422_e54f_43f6_b5e9_782c376a5d02)
#define h_0aada422_e54f_43f6_b5e9_782c376a5d02

#include "meta_target.h"

namespace hammer
{
   class pch_meta_target : public meta_target
   {
      public:
         pch_meta_target(hammer::project* p, const pstring& name, 
                         const requirements_decl& req, 
                         const requirements_decl& usage_req);
      protected:
         virtual sources_decl compute_additional_sources(const main_target& owner) const;
         virtual main_target* construct_main_target(const main_target* owner, const feature_set* properties) const;
         virtual void compute_usage_requirements(feature_set& result, 
                                                 const feature_set& full_build_request,
                                                 const feature_set& computed_usage_requirements,
                                                 const main_target* owner) const;
         virtual bool is_cachable(const main_target* owner) const { return false; }

      private:
         mutable main_target* last_constructed_main_target_;
   };
}

#endif //h_0aada422_e54f_43f6_b5e9_782c376a5d02
