#include "stdafx.h"
#include <hammer/core/product_argument_writer.h>
#include <hammer/core/build_node.h>
#include <hammer/core/basic_target.h>
#include <hammer/core/target_type.h>
#include <hammer/core/build_environment.h>
#include <hammer/core/fs_helpers.h>
#include <hammer/core/main_target.h>

namespace hammer{

product_argument_writer::product_argument_writer(const std::string& name,
                                                 const target_type& t,
                                                 output_strategy::value strategy)
   : targets_argument_writer(name, t),
     output_strategy_(strategy)
{
}

argument_writer* product_argument_writer::clone() const
{
   return new product_argument_writer(*this);
}

bool product_argument_writer::accept(const basic_target& t) const
{
   return t.type().equal_or_derived_from(this->source_type());
}

void product_argument_writer::write_impl(std::ostream& output, const build_node& node, const build_environment& environment) const
{
   for(build_node::targets_t::const_iterator i = node.products_.begin(), last = node.products_.end(); i != last; ++i)
   {
      if (accept(**i))
      {
         // FIXME: �� ���� ������ ����� ����� ����� ��������� ������ ��� ��������, � �� �� ������ �����������
//         location_t product_path = relative_path((**i).mtarget()->intermediate_dir(), environment.current_directory()) / (**i).name().to_string();
         switch(output_strategy_)
         {
            case output_strategy::RELATIVE_TO_MAIN_TARGET:
            {
               location_t product_path = relative_path((**i).get_main_target()->intermediate_dir(), (**i).get_main_target()->location()) / (**i).name().to_string();
               output << product_path.string<std::string>();
               break;
            }

            case output_strategy::FULL_UNC_PATH:
            {
               location_t product_path = (**i).get_main_target()->intermediate_dir() / (**i).name().to_string();
               product_path.normalize();
               output << "\\\\?\\" << product_path.string<std::string>();
               break;
            }

            default:
               throw std::runtime_error("Unknown output strategy for product");
         }

         break;
      }
   }
}

}