#include "hammer_lexer_context.h"
#include "stdafx.h"

namespace hammer {
namespace details {

bool
is_source_lexing(pANTLR3_LEXER lexer)
{
  auto* ctx = static_cast<hammer_lexer_context*>(lexer->super);
  if (ctx->source_lexing_.empty()) {
    return false;
  }

  return ctx->source_lexing_.top();
}
} // namespace details
} // namespace hammer
