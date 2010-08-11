#if !defined(h_bd428576_c9b2_4eb5_8a5a_1fe3dd0e85cb)
#define h_bd428576_c9b2_4eb5_8a5a_1fe3dd0e85cb

#include <hammer/ast/expression.h>

namespace hammer{namespace ast{

class target_ref : public expression
{
   public:
      target_ref(parscore::source_location public_tag, 
                 const path_like_seq* head,
                 const parscore::identifier& target_name,
                 const requirement_set* requirements)
         : public_tag_(public_tag), 
           head_(head),
           target_name_(target_name),
           requirements_(requirements)
      {}

      const path_like_seq* head() const { return head_; }
      const parscore::identifier& target_name() const { return target_name_; }
      const requirement_set* requirements() const { return requirements_; }
      parscore::source_location public_tag() const { return public_tag_; }

      bool has_target_name() const;
      bool is_public() const { return public_tag_.valid(); }
      
      virtual bool accept(visitor& v) const;
   
   private:
      parscore::source_location public_tag_;
      const path_like_seq* head_;
      parscore::identifier target_name_;
      const requirement_set* requirements_;
};

}}

#endif //h_bd428576_c9b2_4eb5_8a5a_1fe3dd0e85cb