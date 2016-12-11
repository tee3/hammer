#if !defined(h_a7ca86c5_dbb5_4306_82c3_a5718516cf52)
#define h_a7ca86c5_dbb5_4306_82c3_a5718516cf52

#include "targets_argument_writer.h"

namespace hammer
{
   class basic_target;

   class product_argument_writer : public targets_argument_writer
   {
      public:
         struct output_strategy{ enum value{ RELATIVE_TO_MAIN_TARGET, RELATIVE_TO_WORKING_DIR, FULL_PATH, FULL_UNC_PATH }; };

         product_argument_writer(const std::string& name, 
                                 const target_type& t,
                                 output_strategy::value strategy = output_strategy::RELATIVE_TO_MAIN_TARGET);
         virtual argument_writer* clone() const;

      protected:
         virtual void write_impl(std::ostream& output, const build_node& node, const build_environment& environment) const;
         virtual bool accept(const basic_target& t) const;
      
      private:
         output_strategy::value output_strategy_;
   };
}

#endif //h_a7ca86c5_dbb5_4306_82c3_a5718516cf52
