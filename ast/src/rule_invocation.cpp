#include "stdafx.h"
#include <hammer/ast/rule_invocation.h>
#include <hammer/ast/visitor.h>

namespace hammer{namespace ast{

bool rule_invocation::accept(visitor& v) const
{
   return v.visit(*this);
}

}}
