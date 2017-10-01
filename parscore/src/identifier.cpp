#include "stdafx.h"
#include <hammer/parscore/identifier.h>
#include <cstring>
#include <cassert>
#include <antlr3.h>

namespace hammer{ namespace parscore{

identifier::identifier(const char* v)
   : v_(v),
     length_(std::strlen(v)),
     no_lok_(true)
{

}

identifier::identifier(const ANTLR3_COMMON_TOKEN_struct* v)
   : lok_(v),
     v_(NULL),
     length_(v->stop - v->start + 1),
     no_lok_(false)
{
}

identifier::identifier()
   : v_(NULL), 
     no_lok_(true)
{
}

bool identifier::operator < (const identifier& rhs) const
{
   if (!valid() && rhs.valid())
      return  true;

   if (valid() && !rhs.valid())
      return false;

   if (!valid() && !rhs.valid())
      return false;

   return to_string() < rhs.to_string();
}

bool identifier::operator == (const identifier& rhs) const
{
   if (this == &rhs)
      return true;

   if (valid() && rhs.valid())
      return to_string() == rhs.to_string();
   else
      return false;
}

std::string identifier::to_string() const
{
   if (no_lok_)
      return std::string(v_, v_ + length_);
   else {
      const pANTLR3_STRING s = lok_.antlr_token_->getText(const_cast<pANTLR3_COMMON_TOKEN>(lok_.antlr_token_));
      return std::string(s->chars, s->chars + s->len);
   }
};

std::ostream& operator << (std::ostream& os, const identifier& v)
{
   os << v.to_string();
   return os;
}

}}
