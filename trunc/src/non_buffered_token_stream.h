#pragma once

#include <antlr3tokenstream.h>
#include "hammer_lexer_context.h"

namespace hammer
{
   namespace details
   {
      struct non_buffered_token_stream
      {
         static pANTLR3_COMMON_TOKEN_STREAM create(ANTLR3_UINT32 hint, pANTLR3_TOKEN_SOURCE source);
         
         void (*base_free) (struct ANTLR3_COMMON_TOKEN_STREAM_struct * tokenStream);
         hammer_lexer_context ctx_;
      };

      bool is_lexing_sources_decl(pANTLR3_LEXER lexer);
   }
}