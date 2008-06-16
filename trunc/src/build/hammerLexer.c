/** \file
 *  This C source file was generated by $ANTLR version 3.1b1
 *
 *     -  From the grammar source file : hammer.g
 *     -                            On : 2008-06-16 21:00:51
 *     -                 for the lexer : hammerLexerLexer *
 * Editing it, at least manually, is not wise. 
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
*/
/* -----------------------------------------
 * Include the ANTLR3 generated header file.
 */
#include    "hammerLexer.h"
/* ----------------------------------------- */


/** String literals used by hammerLexer that we must do things like MATCHS() with.
 *  C will normally just lay down 8 bit characters, and you can use L"xxx" to
 *  get wchar_t, but wchar_t is 16 bits on Windows, which is not UTF32 and so
 *  we perform this little trick of defining the literals as arrays of UINT32
 *  and passing in the address of these.
 */
static ANTLR3_UCHAR	lit_1[]  = { 0x5C, 0x22,  ANTLR3_STRING_TERMINATOR};




/* MACROS that hide the C interface implementations from the
 * generated code, which makes it a little more understandable to the human eye.
 * I am very much against using C pre-processor macros for function calls and bits
 * of code as you cannot see what is happening when single stepping in debuggers
 * and so on. The exception (in my book at least) is for generated code, where you are
 * not maintaining it, but may wish to read and understand it. If you single step it, you know that input()
 * hides some indirect calls, but is always referring to the input stream. This is
 * probably more readable than ctx->input->istream->input(snarfle0->blarg) and allows me to rejig
 * the runtime interfaces without changing the generated code too often, without
 * confusing the reader of the generated output, who may not wish to know the gory
 * details of the interface inheritance.
 */
 
#define		CTX	ctx

/* Aids in accessing scopes for grammar programmers
 */
#undef	SCOPE_TYPE
#undef	SCOPE_STACK
#undef	SCOPE_TOP
#define	SCOPE_TYPE(scope)   phammerLexer_##scope##_SCOPE
#define SCOPE_STACK(scope)  phammerLexer_##scope##Stack
#define	SCOPE_TOP(scope)    ctx->phammerLexer_##scope##Top
#define	SCOPE_SIZE(scope)			(ctx->SCOPE_STACK(scope)->size(ctx->SCOPE_STACK(scope)))
#define SCOPE_INSTANCE(scope, i)	(ctx->SCOPE_STACK(scope)->get(ctx->SCOPE_STACK(scope),i))

 
/* Macros for accessing things in a lexer
 */
#undef	    LEXER
#undef	    RECOGNIZER		    
#undef	    RULEMEMO		    
#undef	    GETCHARINDEX
#undef	    GETLINE
#undef	    GETCHARPOSITIONINLINE
#undef	    EMIT
#undef	    EMITNEW
#undef	    MATCHC
#undef	    MATCHS
#undef	    MATCHRANGE
#undef	    LTOKEN
#undef	    HASFAILED
#undef	    FAILEDFLAG
#undef	    INPUT
#undef	    STRSTREAM
#undef	    LA
#undef	    HASEXCEPTION
#undef	    EXCEPTION
#undef	    CONSTRUCTEX
#undef	    CONSUME
#undef	    LRECOVER
#undef	    MARK
#undef	    REWIND
#undef	    REWINDLAST
#undef	    BACKTRACKING
#undef		MATCHANY
#undef		MEMOIZE
#undef		HAVEPARSEDRULE
#undef		GETTEXT
#undef		INDEX
#undef		SEEK
#undef		PUSHSTREAM
#undef		POPSTREAM
#undef		SETTEXT
#undef		SETTEXT8

#define	    LEXER					ctx->pLexer
#define	    RECOGNIZER			    LEXER->rec
#define		LEXSTATE				RECOGNIZER->state
#define		TOKSOURCE				LEXSTATE->tokSource
#define	    GETCHARINDEX()			LEXER->getCharIndex(LEXER)
#define	    GETLINE()				LEXER->getLine(LEXER)
#define	    GETTEXT()				LEXER->getText(LEXER)
#define	    GETCHARPOSITIONINLINE() LEXER->getCharPositionInLine(LEXER)
#define	    EMIT()					LEXSTATE->type = _type; LEXER->emit(LEXER)
#define	    EMITNEW(t)				LEXER->emitNew(LEXER, t)
#define	    MATCHC(c)				LEXER->matchc(LEXER, c)
#define	    MATCHS(s)				LEXER->matchs(LEXER, s)
#define	    MATCHRANGE(c1,c2)	    LEXER->matchRange(LEXER, c1, c2)
#define	    MATCHANY()				LEXER->matchAny(LEXER)
#define	    LTOKEN  				LEXSTATE->token
#define	    HASFAILED()				(LEXSTATE->failed == ANTLR3_TRUE)
#define	    BACKTRACKING			LEXSTATE->backtracking
#define	    FAILEDFLAG				LEXSTATE->failed
#define	    INPUT					LEXER->input
#define	    STRSTREAM				INPUT
#define		ISTREAM					INPUT->istream
#define		INDEX()					ISTREAM->index(ISTREAM)
#define		SEEK(n)					ISTREAM->seek(ISTREAM, n)
#define	    EOF_TOKEN				&(LEXSTATE->tokSource->eofToken)
#define	    HASEXCEPTION()			(LEXSTATE->error == ANTLR3_TRUE)
#define	    EXCEPTION				LEXSTATE->exception
#define	    CONSTRUCTEX()			RECOGNIZER->exConstruct(RECOGNIZER)
#define	    LRECOVER()				LEXER->recover(LEXER)
#define	    MARK()					ISTREAM->mark(ISTREAM)
#define	    REWIND(m)				ISTREAM->rewind(ISTREAM, m)
#define	    REWINDLAST()			ISTREAM->rewindLast(ISTREAM)
#define		MEMOIZE(ri,si)			RECOGNIZER->memoize(RECOGNIZER, ri, si)
#define		HAVEPARSEDRULE(r)		RECOGNIZER->alreadyParsedRule(RECOGNIZER, r)
#define		PUSHSTREAM(str)			LEXER->pushCharStream(LEXER, str)
#define		POPSTREAM()				LEXER->popCharStream(LEXER)
#define		SETTEXT(str)			LEXSTATE->text = str
#define		SKIP()					LEXSTATE->token = &(TOKSOURCE->skipToken)
#define		USER1					LEXSTATE->user1
#define		USER2					LEXSTATE->user2
#define		USER3					LEXSTATE->user3
#define		CUSTOM					LEXSTATE->custom
#define		RULEMEMO				LEXSTATE->ruleMemo
#define		DBG						RECOGNIZER->debugger

/* If we have been told we can rely on the standard 8 bit or 16 bit input
 * stream, then we can define our macros to use the direct pointers
 * in the input object, which is much faster than indirect calls. This
 * is really only significant to lexers with a lot of fragment rules (which
 * do not place LA(1) in a temporary at the moment) and even then
 * only if there is a lot of input (order of say 1M or so).
 */
#if	defined(ANTLR3_INLINE_INPUT_ASCII) || defined(ANTLR3_INLINE_INPUT_UTF16)

# ifdef	ANTLR3_INLINE_INPUT_ASCII

/* 8 bit "ASCII" (actually any 8 bit character set) */

#  define	    NEXTCHAR			((pANTLR3_UINT8)(INPUT->nextChar))
#  define	    DATAP				((pANTLR3_UINT8)(INPUT->data))

# else

#  define	    NEXTCHAR			((pANTLR3_UINT16)(INPUT->nextChar)) 
#  define	    DATAP				((pANTLR3_UINT16)(INPUT->data))

# endif

# define	    LA(n) ((NEXTCHAR + n) > (DATAP + INPUT->sizeBuf) ? ANTLR3_CHARSTREAM_EOF : (ANTLR3_UCHAR)(*(NEXTCHAR + n - 1)))
# define	    CONSUME()											\
{																	\
    if	(NEXTCHAR < (DATAP + INPUT->sizeBuf))						\
    {																\
		INPUT->charPositionInLine++;								\
		if  ((ANTLR3_UCHAR)(*NEXTCHAR) == INPUT->newlineChar)		\
		{															\
			INPUT->line++;											\
			INPUT->charPositionInLine	= 0;						\
			INPUT->currentLine		= (void *)(NEXTCHAR + 1);		\
		}															\
		INPUT->nextChar = (void *)(NEXTCHAR + 1);					\
    }																\
}

#else

// Pick up the input character by calling the input stream implementation.
//
#define	    CONSUME()				INPUT->istream->consume(INPUT->istream)
#define	    LA(n)					INPUT->istream->_LA(INPUT->istream, n)

#endif
#define		TOKTEXT(tok, txt)				tok, (pANTLR3_UINT8)txt

/* The 4 tokens defined below may well clash with your own #defines or token types. If so
 * then for the present you must use different names for your defines as these are hard coded
 * in the code generator. It would be better not to use such names internally, and maybe
 * we can change this in a forthcoming release. I deliberately do not #undef these
 * here as this will at least give you a redefined error somewhere if they clash.
 */
#define	    UP	    ANTLR3_TOKEN_UP
#define	    DOWN    ANTLR3_TOKEN_DOWN
#define	    EOR	    ANTLR3_TOKEN_EOR
#define	    INVALID ANTLR3_TOKEN_INVALID


/* =============================================================================
 * Functions to create and destroy scopes. First come the rule scopes, followed
 * by the global declared scopes.
 */



/* ============================================================================= */

/* =============================================================================
 * Start of recognizer
 */


/* Forward declare the locally static matching functions we have generated and any predicate functions.
 */
static ANTLR3_INLINE void	mT__19    (phammerLexer ctx);
static ANTLR3_INLINE void	mT__20    (phammerLexer ctx);
static ANTLR3_INLINE void	mT__21    (phammerLexer ctx);
static ANTLR3_INLINE void	mT__22    (phammerLexer ctx);
static ANTLR3_INLINE void	mT__23    (phammerLexer ctx);
static ANTLR3_INLINE void	mT__24    (phammerLexer ctx);
static ANTLR3_INLINE void	mT__25    (phammerLexer ctx);
static ANTLR3_INLINE void	mID    (phammerLexer ctx);
static ANTLR3_INLINE void	mSTRING    (phammerLexer ctx);
static ANTLR3_INLINE void	mCOMMENT    (phammerLexer ctx);
static ANTLR3_INLINE void	mWS    (phammerLexer ctx);
static ANTLR3_INLINE void	mTokens    (phammerLexer ctx);
static void	hammerLexerFree(phammerLexer ctx);

/* =========================================================================
 * Lexer matching rules end.
 * =========================================================================
 */



static void
hammerLexerFree  (phammerLexer ctx)
{
    LEXER->free(LEXER);
    
    ANTLR3_FREE(ctx);
}

/** \brief Name of the grammar file that generated this code
 */
static const char fileName[] = "hammer.g";

/** \brief Return the name of the grammar file that generated this code.
 */
static const char * getGrammarFileName()
{
	return fileName;
}

/** \brief Create a new lexer called hammerLexer
 *
 * \param[in]    instream Pointer to an initialized input stream
 * \return 
 *     - Success phammerLexer initialized for the lex start
 *     - Fail NULL
 */
ANTLR3_API phammerLexer hammerLexerNew         
(pANTLR3_INPUT_STREAM instream)
{
	// See if we can create a new lexer with the standard constructor
	//
	return hammerLexerNewSSD(instream, NULL);
}

/** \brief Create a new lexer called hammerLexer
 *
 * \param[in]    instream Pointer to an initialized input stream
 * \param[state] state Previously created shared recognizer stat
 * \return 
 *     - Success phammerLexer initialized for the lex start
 *     - Fail NULL
 */
ANTLR3_API phammerLexer hammerLexerNewSSD         
(pANTLR3_INPUT_STREAM instream, pANTLR3_RECOGNIZER_SHARED_STATE state)
{
    phammerLexer lexCtx; // Context structure we will build and return

    lexCtx = (phammerLexer) ANTLR3_MALLOC(sizeof(hammerLexer));

    if  (lexCtx == NULL)
    {
        // Failed to allocate memory for lexer context
        return  NULL;
    }

    /* -------------------------------------------------------------------
     * Memory for basic structure is allocated, now to fill in
     * in base ANTLR3 structures. We initialize the function pointers
     * for the standard ANTLR3 lexer function set, but upon return
     * from here, the programmer may set the pointers to provide custom
     * implementations of each function. 
     *
     * We don't use the macros defined in hammerLexer.h here so you can get a sense
     * of what goes where.
     */
    
    /* Create a base lexer, using the supplied input stream
     */
    lexCtx->pLexer	= antlr3LexerNewStream(ANTLR3_SIZE_HINT, instream, state);
    
    /* Check that we allocated the memory correctly
     */
    if	(lexCtx->pLexer == NULL)
    {
		ANTLR3_FREE(lexCtx);
		return  NULL;
    }
    /* Install the implementation of our hammerLexer interface
     */
    lexCtx->mT__19	= mT__19;
    lexCtx->mT__20	= mT__20;
    lexCtx->mT__21	= mT__21;
    lexCtx->mT__22	= mT__22;
    lexCtx->mT__23	= mT__23;
    lexCtx->mT__24	= mT__24;
    lexCtx->mT__25	= mT__25;
    lexCtx->mID	= mID;
    lexCtx->mSTRING	= mSTRING;
    lexCtx->mCOMMENT	= mCOMMENT;
    lexCtx->mWS	= mWS;
    lexCtx->mTokens	= mTokens;
    
    /** When the nextToken() call is made to this lexer's pANTLR3_TOKEN_SOURCE
     *  it will call mTokens() in this generated code, and will pass it the ctx
     * pointer of this lexer, not the context of the base lexer, so store that now.
     */
    lexCtx->pLexer->ctx	    = lexCtx;
    
    /**Install the token matching function
     */
    lexCtx->pLexer->mTokens = (void (*) (void *))(mTokens);
    
    lexCtx->getGrammarFileName	= getGrammarFileName;
    lexCtx->free		= hammerLexerFree;

    
    


    /* Return the newly built lexer to the caller
     */
    return  lexCtx;
}
 

/* =========================================================================
 * Functions to match the lexer grammar defined tokens from the input stream
 */

//   Comes from: 7:7: ( ';' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__19
 *
 * Looks to match the characters the constitute the token T__19
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == ANTLR3_TRUE if an exception was thrown.
 */
static ANTLR3_INLINE
void mT__19(phammerLexer ctx)
{
	ANTLR3_UINT32	_type;

   
    _type	    = T__19;
       
    
    // hammer.g:7:7: ( ';' )
    // hammer.g:7:9: ';'
    {
        MATCHC(';'); 


    }

	LEXSTATE->type = _type;

    // This is where rules clean up and exit
    //
    goto ruleT__19Ex; /* Prevent compiler warnings */
    ruleT__19Ex: ;

}
// $ANTLR end T__19

//   Comes from: 8:7: ( ':' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__20
 *
 * Looks to match the characters the constitute the token T__20
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == ANTLR3_TRUE if an exception was thrown.
 */
static ANTLR3_INLINE
void mT__20(phammerLexer ctx)
{
	ANTLR3_UINT32	_type;

   
    _type	    = T__20;
       
    
    // hammer.g:8:7: ( ':' )
    // hammer.g:8:9: ':'
    {
        MATCHC(':'); 


    }

	LEXSTATE->type = _type;

    // This is where rules clean up and exit
    //
    goto ruleT__20Ex; /* Prevent compiler warnings */
    ruleT__20Ex: ;

}
// $ANTLR end T__20

//   Comes from: 9:7: ( ',' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__21
 *
 * Looks to match the characters the constitute the token T__21
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == ANTLR3_TRUE if an exception was thrown.
 */
static ANTLR3_INLINE
void mT__21(phammerLexer ctx)
{
	ANTLR3_UINT32	_type;

   
    _type	    = T__21;
       
    
    // hammer.g:9:7: ( ',' )
    // hammer.g:9:9: ','
    {
        MATCHC(','); 


    }

	LEXSTATE->type = _type;

    // This is where rules clean up and exit
    //
    goto ruleT__21Ex; /* Prevent compiler warnings */
    ruleT__21Ex: ;

}
// $ANTLR end T__21

//   Comes from: 10:7: ( '<' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__22
 *
 * Looks to match the characters the constitute the token T__22
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == ANTLR3_TRUE if an exception was thrown.
 */
static ANTLR3_INLINE
void mT__22(phammerLexer ctx)
{
	ANTLR3_UINT32	_type;

   
    _type	    = T__22;
       
    
    // hammer.g:10:7: ( '<' )
    // hammer.g:10:9: '<'
    {
        MATCHC('<'); 


    }

	LEXSTATE->type = _type;

    // This is where rules clean up and exit
    //
    goto ruleT__22Ex; /* Prevent compiler warnings */
    ruleT__22Ex: ;

}
// $ANTLR end T__22

//   Comes from: 11:7: ( '>' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__23
 *
 * Looks to match the characters the constitute the token T__23
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == ANTLR3_TRUE if an exception was thrown.
 */
static ANTLR3_INLINE
void mT__23(phammerLexer ctx)
{
	ANTLR3_UINT32	_type;

   
    _type	    = T__23;
       
    
    // hammer.g:11:7: ( '>' )
    // hammer.g:11:9: '>'
    {
        MATCHC('>'); 


    }

	LEXSTATE->type = _type;

    // This is where rules clean up and exit
    //
    goto ruleT__23Ex; /* Prevent compiler warnings */
    ruleT__23Ex: ;

}
// $ANTLR end T__23

//   Comes from: 12:7: ( '[' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__24
 *
 * Looks to match the characters the constitute the token T__24
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == ANTLR3_TRUE if an exception was thrown.
 */
static ANTLR3_INLINE
void mT__24(phammerLexer ctx)
{
	ANTLR3_UINT32	_type;

   
    _type	    = T__24;
       
    
    // hammer.g:12:7: ( '[' )
    // hammer.g:12:9: '['
    {
        MATCHC('['); 


    }

	LEXSTATE->type = _type;

    // This is where rules clean up and exit
    //
    goto ruleT__24Ex; /* Prevent compiler warnings */
    ruleT__24Ex: ;

}
// $ANTLR end T__24

//   Comes from: 13:7: ( ']' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start T__25
 *
 * Looks to match the characters the constitute the token T__25
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == ANTLR3_TRUE if an exception was thrown.
 */
static ANTLR3_INLINE
void mT__25(phammerLexer ctx)
{
	ANTLR3_UINT32	_type;

   
    _type	    = T__25;
       
    
    // hammer.g:13:7: ( ']' )
    // hammer.g:13:9: ']'
    {
        MATCHC(']'); 


    }

	LEXSTATE->type = _type;

    // This is where rules clean up and exit
    //
    goto ruleT__25Ex; /* Prevent compiler warnings */
    ruleT__25Ex: ;

}
// $ANTLR end T__25

//   Comes from: 59:5: ( ( 'a' .. 'z' | 'A' .. 'Z' | '0' .. '9' | '.' | '-' | '_' | '=' | '/' | '*' )+ | STRING )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start ID
 *
 * Looks to match the characters the constitute the token ID
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == ANTLR3_TRUE if an exception was thrown.
 */
static ANTLR3_INLINE
void mID(phammerLexer ctx)
{
	ANTLR3_UINT32	_type;

   
    _type	    = ID;
       
    
    {
        //  hammer.g:59:5: ( ( 'a' .. 'z' | 'A' .. 'Z' | '0' .. '9' | '.' | '-' | '_' | '=' | '/' | '*' )+ | STRING )
        
        ANTLR3_UINT32 alt2;

        alt2=2;


        {
            int LA2_0 = LA(1);
            if ( (LA2_0 == '*' || ((LA2_0 >= '-') && (LA2_0 <= '9')) || LA2_0 == '=' || ((LA2_0 >= 'A') && (LA2_0 <= 'Z')) || LA2_0 == '_' || ((LA2_0 >= 'a') && (LA2_0 <= 'z'))) ) 
            {
                alt2=1;
            }
            else if ( (LA2_0 == '"') ) 
            {
                alt2=2;
            }
            else 
            {
            
                CONSTRUCTEX();
                EXCEPTION->type         = ANTLR3_NO_VIABLE_ALT_EXCEPTION;
                EXCEPTION->message      = (void *)"";
                EXCEPTION->decisionNum  = 2;
                EXCEPTION->state        = 0;


                goto ruleIDEx;
            }
        }
        switch (alt2) 
        {
    	case 1:
    	    // hammer.g:59:9: ( 'a' .. 'z' | 'A' .. 'Z' | '0' .. '9' | '.' | '-' | '_' | '=' | '/' | '*' )+
    	    {
    	        // hammer.g:59:9: ( 'a' .. 'z' | 'A' .. 'Z' | '0' .. '9' | '.' | '-' | '_' | '=' | '/' | '*' )+
    	        {
    	            int cnt1=0;

    	            for (;;)
    	            {
    	                int alt1=2;
    	        	{
    	        	   /* dfaLoopbackState(k,edges,eotPredictsAlt,description,stateNumber,semPredState)
    	        	    */
    	        	    int LA1_0 = LA(1);
    	        	    if ( (LA1_0 == '*' || ((LA1_0 >= '-') && (LA1_0 <= '9')) || LA1_0 == '=' || ((LA1_0 >= 'A') && (LA1_0 <= 'Z')) || LA1_0 == '_' || ((LA1_0 >= 'a') && (LA1_0 <= 'z'))) ) 
    	        	    {
    	        	        alt1=1;
    	        	    }

    	        	}
    	        	switch (alt1) 
    	        	{
    	        	    case 1:
    	        	        // hammer.g:
    	        	        {
    	        	            if ( LA(1) == '*' || ((LA(1) >= '-') && (LA(1) <= '9')) || LA(1) == '=' || ((LA(1) >= 'A') && (LA(1) <= 'Z')) || LA(1) == '_' || ((LA(1) >= 'a') && (LA(1) <= 'z')) )
    	        	            {
    	        	                CONSUME();

    	        	            }
    	        	            else 
    	        	            {
    	        	                CONSTRUCTEX();
    	        	                EXCEPTION->type         = ANTLR3_MISMATCHED_SET_EXCEPTION;
    	        	                EXCEPTION->name         = (void *)ANTLR3_MISMATCHED_SET_NAME;

    	        	                LRECOVER();    goto ruleIDEx;
    	        	            }


    	        	        }
    	        	        break;

    	        	    default:
    	        	    
    	        		if ( cnt1 >= 1 )
    	        		{
    	        		    goto loop1;
    	        		}
    	        		/* mismatchedSetEx()
    	        		 */
    	        		CONSTRUCTEX();
    	        		EXCEPTION->type = ANTLR3_EARLY_EXIT_EXCEPTION;
    	        		EXCEPTION->name = (void *)ANTLR3_EARLY_EXIT_NAME;


    	        		goto ruleIDEx;
    	        	}
    	        	cnt1++;
    	            }
    	            loop1: ;	/* Jump to here if this rule does not match */
    	        }

    	    }
    	    break;
    	case 2:
    	    // hammer.g:59:81: STRING
    	    {
    	        /* 59:81: STRING */
    	        mSTRING(ctx ); 


    	    }
    	    break;

        }
    }
	LEXSTATE->type = _type;

    // This is where rules clean up and exit
    //
    goto ruleIDEx; /* Prevent compiler warnings */
    ruleIDEx: ;

}
// $ANTLR end ID

//   Comes from: 62:9: ( '\"' ( '\\\\\"' | ~ ( '\"' | '\\n' | '\\r' ) )* '\"' )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start STRING
 *
 * Looks to match the characters the constitute the token STRING
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == ANTLR3_TRUE if an exception was thrown.
 */
static ANTLR3_INLINE
void mSTRING(phammerLexer ctx)
{
	ANTLR3_UINT32	_type;

        
    // hammer.g:62:9: ( '\"' ( '\\\\\"' | ~ ( '\"' | '\\n' | '\\r' ) )* '\"' )
    // hammer.g:62:11: '\"' ( '\\\\\"' | ~ ( '\"' | '\\n' | '\\r' ) )* '\"'
    {
        MATCHC('"'); 


        // hammer.g:62:15: ( '\\\\\"' | ~ ( '\"' | '\\n' | '\\r' ) )*

        for (;;)
        {
            int alt3=3;
            {
               /* dfaLoopbackState(k,edges,eotPredictsAlt,description,stateNumber,semPredState)
                */
                int LA3_0 = LA(1);
                if ( (LA3_0 == '\\') ) 
                {
                    {
                       /* dfaLoopbackState(k,edges,eotPredictsAlt,description,stateNumber,semPredState)
                        */
                        int LA3_2 = LA(2);
                        if ( (LA3_2 == '"') ) 
                        {
                            {
                               /* dfaLoopbackState(k,edges,eotPredictsAlt,description,stateNumber,semPredState)
                                */
                                int LA3_4 = LA(3);
                                if ( (((LA3_4 >= 0x0000) && (LA3_4 <= '\t')) || ((LA3_4 >= 0x000B) && (LA3_4 <= '\f')) || ((LA3_4 >= 0x000E) && (LA3_4 <= 0xFFFE))) ) 
                                {
                                    alt3=1;
                                }

                                
                                else 
                                {
                            	alt3=2;
                                }

                            }
                        }
                        else if ( (((LA3_2 >= 0x0000) && (LA3_2 <= '\t')) || ((LA3_2 >= 0x000B) && (LA3_2 <= '\f')) || ((LA3_2 >= 0x000E) && (LA3_2 <= '!')) || ((LA3_2 >= '#') && (LA3_2 <= 0xFFFE))) ) 
                        {
                            alt3=2;
                        }

                    }
                }
                else if ( (((LA3_0 >= 0x0000) && (LA3_0 <= '\t')) || ((LA3_0 >= 0x000B) && (LA3_0 <= '\f')) || ((LA3_0 >= 0x000E) && (LA3_0 <= '!')) || ((LA3_0 >= '#') && (LA3_0 <= '[')) || ((LA3_0 >= ']') && (LA3_0 <= 0xFFFE))) ) 
                {
                    alt3=2;
                }

            }
            switch (alt3) 
            {
        	case 1:
        	    // hammer.g:62:16: '\\\\\"'
        	    {
        	        MATCHS(lit_1); 



        	    }
        	    break;
        	case 2:
        	    // hammer.g:62:24: ~ ( '\"' | '\\n' | '\\r' )
        	    {
        	        if ( ((LA(1) >= 0x0000) && (LA(1) <= '\t')) || ((LA(1) >= 0x000B) && (LA(1) <= '\f')) || ((LA(1) >= 0x000E) && (LA(1) <= '!')) || ((LA(1) >= '#') && (LA(1) <= 0xFFFE)) )
        	        {
        	            CONSUME();

        	        }
        	        else 
        	        {
        	            CONSTRUCTEX();
        	            EXCEPTION->type         = ANTLR3_MISMATCHED_SET_EXCEPTION;
        	            EXCEPTION->name         = (void *)ANTLR3_MISMATCHED_SET_NAME;

        	            LRECOVER();    goto ruleSTRINGEx;
        	        }


        	    }
        	    break;

        	default:
        	    goto loop3;	/* break out of the loop */
        	    break;
            }
        }
        loop3: ; /* Jump out to here if this rule does not match */

        MATCHC('"'); 


    }



    // This is where rules clean up and exit
    //
    goto ruleSTRINGEx; /* Prevent compiler warnings */
    ruleSTRINGEx: ;

}
// $ANTLR end STRING

//   Comes from: 64:9: ( '#' (~ ( '\\n' | '\\r' ) )* ( '\\n' | '\\r' )? )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start COMMENT
 *
 * Looks to match the characters the constitute the token COMMENT
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == ANTLR3_TRUE if an exception was thrown.
 */
static ANTLR3_INLINE
void mCOMMENT(phammerLexer ctx)
{
	ANTLR3_UINT32	_type;

   
    _type	    = COMMENT;
       
    
    // hammer.g:64:9: ( '#' (~ ( '\\n' | '\\r' ) )* ( '\\n' | '\\r' )? )
    // hammer.g:64:11: '#' (~ ( '\\n' | '\\r' ) )* ( '\\n' | '\\r' )?
    {
        MATCHC('#'); 


        // hammer.g:64:15: (~ ( '\\n' | '\\r' ) )*

        for (;;)
        {
            int alt4=2;
            {
               /* dfaLoopbackState(k,edges,eotPredictsAlt,description,stateNumber,semPredState)
                */
                int LA4_0 = LA(1);
                if ( (((LA4_0 >= 0x0000) && (LA4_0 <= '\t')) || ((LA4_0 >= 0x000B) && (LA4_0 <= '\f')) || ((LA4_0 >= 0x000E) && (LA4_0 <= 0xFFFE))) ) 
                {
                    alt4=1;
                }

            }
            switch (alt4) 
            {
        	case 1:
        	    // hammer.g:64:16: ~ ( '\\n' | '\\r' )
        	    {
        	        if ( ((LA(1) >= 0x0000) && (LA(1) <= '\t')) || ((LA(1) >= 0x000B) && (LA(1) <= '\f')) || ((LA(1) >= 0x000E) && (LA(1) <= 0xFFFE)) )
        	        {
        	            CONSUME();

        	        }
        	        else 
        	        {
        	            CONSTRUCTEX();
        	            EXCEPTION->type         = ANTLR3_MISMATCHED_SET_EXCEPTION;
        	            EXCEPTION->name         = (void *)ANTLR3_MISMATCHED_SET_NAME;

        	            LRECOVER();    goto ruleCOMMENTEx;
        	        }


        	    }
        	    break;

        	default:
        	    goto loop4;	/* break out of the loop */
        	    break;
            }
        }
        loop4: ; /* Jump out to here if this rule does not match */


        // hammer.g:64:33: ( '\\n' | '\\r' )?
        {
            int alt5=2;
            {
                int LA5_0 = LA(1);
                if ( (LA5_0 == '\n' || LA5_0 == '\r') ) 
                {
                    alt5=1;
                }
            }
            switch (alt5) 
            {
        	case 1:
        	    // hammer.g:
        	    {
        	        if ( LA(1) == '\n' || LA(1) == '\r' )
        	        {
        	            CONSUME();

        	        }
        	        else 
        	        {
        	            CONSTRUCTEX();
        	            EXCEPTION->type         = ANTLR3_MISMATCHED_SET_EXCEPTION;
        	            EXCEPTION->name         = (void *)ANTLR3_MISMATCHED_SET_NAME;

        	            LRECOVER();    goto ruleCOMMENTEx;
        	        }


        	    }
        	    break;

            }
        }
        {
             LEXSTATE->channel = HIDDEN; 
        }

    }

	LEXSTATE->type = _type;

    // This is where rules clean up and exit
    //
    goto ruleCOMMENTEx; /* Prevent compiler warnings */
    ruleCOMMENTEx: ;

}
// $ANTLR end COMMENT

//   Comes from: 66:4: ( ( ' ' | '\\n' | '\\r' ) )
/** \brief Lexer rule generated by ANTLR3
 *
 * $ANTLR start WS
 *
 * Looks to match the characters the constitute the token WS
 * from the attached input stream.
 *
 *
 * \remark
 *  - lexer->error == ANTLR3_TRUE if an exception was thrown.
 */
static ANTLR3_INLINE
void mWS(phammerLexer ctx)
{
	ANTLR3_UINT32	_type;

   
    _type	    = WS;
       
    
    // hammer.g:66:4: ( ( ' ' | '\\n' | '\\r' ) )
    // hammer.g:66:6: ( ' ' | '\\n' | '\\r' )
    {
        if ( LA(1) == '\n' || LA(1) == '\r' || LA(1) == ' ' )
        {
            CONSUME();

        }
        else 
        {
            CONSTRUCTEX();
            EXCEPTION->type         = ANTLR3_MISMATCHED_SET_EXCEPTION;
            EXCEPTION->name         = (void *)ANTLR3_MISMATCHED_SET_NAME;

            LRECOVER();    goto ruleWSEx;
        }


    }

	LEXSTATE->type = _type;

    // This is where rules clean up and exit
    //
    goto ruleWSEx; /* Prevent compiler warnings */
    ruleWSEx: ;

}
// $ANTLR end WS

/** This is the entry point in to the lexer from an object that
 *  wants to generate the next token, such as a pCOMMON_TOKEN_STREAM
 */
static void 
mTokens(phammerLexer ctx)
{
    {
        //  hammer.g:1:8: ( T__19 | T__20 | T__21 | T__22 | T__23 | T__24 | T__25 | ID | COMMENT | WS )
        
        ANTLR3_UINT32 alt6;

        alt6=10;

        switch ( LA(1) ) 
        {
        case ';':
        	{
        		alt6=1;
        	}
            break;
        case ':':
        	{
        		alt6=2;
        	}
            break;
        case ',':
        	{
        		alt6=3;
        	}
            break;
        case '<':
        	{
        		alt6=4;
        	}
            break;
        case '>':
        	{
        		alt6=5;
        	}
            break;
        case '[':
        	{
        		alt6=6;
        	}
            break;
        case ']':
        	{
        		alt6=7;
        	}
            break;
        case '"':
        case '*':
        case '-':
        case '.':
        case '/':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '=':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
        case '_':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
        	{
        		alt6=8;
        	}
            break;
        case '#':
        	{
        		alt6=9;
        	}
            break;
        case '\n':
        case '\r':
        case ' ':
        	{
        		alt6=10;
        	}
            break;

        default:
            CONSTRUCTEX();
            EXCEPTION->type         = ANTLR3_NO_VIABLE_ALT_EXCEPTION;
            EXCEPTION->message      = (void *)"";
            EXCEPTION->decisionNum  = 6;
            EXCEPTION->state        = 0;


            goto ruleTokensEx;
        }

        switch (alt6) 
        {
    	case 1:
    	    // hammer.g:1:10: T__19
    	    {
    	        /* 1:10: T__19 */
    	        mT__19(ctx ); 


    	    }
    	    break;
    	case 2:
    	    // hammer.g:1:16: T__20
    	    {
    	        /* 1:16: T__20 */
    	        mT__20(ctx ); 


    	    }
    	    break;
    	case 3:
    	    // hammer.g:1:22: T__21
    	    {
    	        /* 1:22: T__21 */
    	        mT__21(ctx ); 


    	    }
    	    break;
    	case 4:
    	    // hammer.g:1:28: T__22
    	    {
    	        /* 1:28: T__22 */
    	        mT__22(ctx ); 


    	    }
    	    break;
    	case 5:
    	    // hammer.g:1:34: T__23
    	    {
    	        /* 1:34: T__23 */
    	        mT__23(ctx ); 


    	    }
    	    break;
    	case 6:
    	    // hammer.g:1:40: T__24
    	    {
    	        /* 1:40: T__24 */
    	        mT__24(ctx ); 


    	    }
    	    break;
    	case 7:
    	    // hammer.g:1:46: T__25
    	    {
    	        /* 1:46: T__25 */
    	        mT__25(ctx ); 


    	    }
    	    break;
    	case 8:
    	    // hammer.g:1:52: ID
    	    {
    	        /* 1:52: ID */
    	        mID(ctx ); 


    	    }
    	    break;
    	case 9:
    	    // hammer.g:1:55: COMMENT
    	    {
    	        /* 1:55: COMMENT */
    	        mCOMMENT(ctx ); 


    	    }
    	    break;
    	case 10:
    	    // hammer.g:1:63: WS
    	    {
    	        /* 1:63: WS */
    	        mWS(ctx ); 


    	    }
    	    break;

        }
    }

    
    goto ruleTokensEx; /* Prevent compiler warnings */
ruleTokensEx: ;
}

/* =========================================================================
 * Lexer matching rules end.
 * =========================================================================
 */
/* End of Lexer code
 * ================================================
 * ================================================
 */ 


/* End of code
 * =============================================================================
 */
