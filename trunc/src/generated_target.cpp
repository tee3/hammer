#include "stdafx.h"
#include "generated_target.h"
#include "main_target.h"

namespace hammer{

generated_target::generated_target(const main_target* mt, const pstring& n, 
                                   const hammer::type* t, const feature_set* f) : basic_target(mt, n, t, f)
{
}

const pstring& generated_target::location() const
{
   return mtarget()->intermediate_dir();
}

boost::intrusive_ptr<build_node> generated_target::generate()
{
   throw std::logic_error("generated target cannot be generated again.");
}

}