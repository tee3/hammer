/** \file
 *  This C header file was generated by $ANTLR version 3.1b1
 *
 *     -  From the grammar source file : hammer.g
 *     -                            On : 2008-07-01 20:59:15
 *     -                 for the lexer : hammerLexerLexer *
 * Editing it, at least manually, is not wise. 
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The lexer hammerLexer has the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could begin
 * this rule.
 * 
 * For instance if you call the first (topmost) rule in a parser grammar, you will
 * get the results of a full parse, but calling a rule half way through the grammar will
 * allow you to pass part of a full token stream to the parser, such as for syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to function) via
 * a parser context typedef phammerLexer, which is returned from a call to hammerLexerNew().
 *
 * As this is a generated lexer, it is unlikely you will call it 'manually'. However
 * the methods are provided anyway.
 * * The methods in phammerLexer are  as follows:
 *
 *  - void      phammerLexer->T__20(phammerLexer)
 *  - void      phammerLexer->T__21(phammerLexer)
 *  - void      phammerLexer->T__22(phammerLexer)
 *  - void      phammerLexer->T__23(phammerLexer)
 *  - void      phammerLexer->T__24(phammerLexer)
 *  - void      phammerLexer->T__25(phammerLexer)
 *  - void      phammerLexer->ID(phammerLexer)
 *  - void      phammerLexer->COLON(phammerLexer)
 *  - void      phammerLexer->STRING(phammerLexer)
 *  - void      phammerLexer->COMMENT(phammerLexer)
 *  - void      phammerLexer->WS(phammerLexer)
 *  - void      phammerLexer->Tokens(phammerLexer)
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
#ifndef	_hammerLexer_H
#define _hammerLexer_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */
 
#ifdef __cplusplus
extern "C" {
#endif

// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct hammerLexer_Ctx_struct hammerLexer, * phammerLexer;



#ifdef	ANTLR3_WINDOWS
// Disable: Unreferenced parameter,							- Rules with parameters that are not used
//          constant conditional,							- ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable					- tree rewrite variables declared but not needed
//          Unreferenced local variable						- lexer rule declares but does not always use _type
//          potentially unitialized variable used			- retval always returned from a rule 
//			unreferenced local function has been removed	- susually getTokenNames or freeScope, they can go without warnigns
//
// These are only really displayed at warning level /W4 but that is the code ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from 4100, which is
// usually generated when a parser rule is given a parameter that it does not use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4189 )
#pragma warning( disable : 4505 )
#endif


/** Context tracking structure for hammerLexer
 */
struct hammerLexer_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_LEXER    pLexer;


     void (*mT__20)	(struct hammerLexer_Ctx_struct * ctx);
     void (*mT__21)	(struct hammerLexer_Ctx_struct * ctx);
     void (*mT__22)	(struct hammerLexer_Ctx_struct * ctx);
     void (*mT__23)	(struct hammerLexer_Ctx_struct * ctx);
     void (*mT__24)	(struct hammerLexer_Ctx_struct * ctx);
     void (*mT__25)	(struct hammerLexer_Ctx_struct * ctx);
     void (*mID)	(struct hammerLexer_Ctx_struct * ctx);
     void (*mCOLON)	(struct hammerLexer_Ctx_struct * ctx);
     void (*mSTRING)	(struct hammerLexer_Ctx_struct * ctx);
     void (*mCOMMENT)	(struct hammerLexer_Ctx_struct * ctx);
     void (*mWS)	(struct hammerLexer_Ctx_struct * ctx);
     void (*mTokens)	(struct hammerLexer_Ctx_struct * ctx);    const char * (*getGrammarFileName)();
    void	    (*free)   (struct hammerLexer_Ctx_struct * ctx);
        
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API phammerLexer hammerLexerNew         (pANTLR3_INPUT_STREAM instream);
ANTLR3_API phammerLexer hammerLexerNewSSD      (pANTLR3_INPUT_STREAM instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the lexer will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this out at the moment
 * so we just undef it here for now. That isn't the value we get back from C recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef	EOF
#undef	EOF
#endif
#ifdef	Tokens
#undef	Tokens
#endif 
#define REQUIREMENTS_DECL      10
#define STRING_LIST      7
#define T__25      25
#define T__24      24
#define T__23      23
#define T__22      22
#define T__21      21
#define CONDITIONAL_FEATURES      11
#define T__20      20
#define CONDITION      12
#define ID      16
#define EOF      -1
#define NULL_ARG      5
#define STRING_ARG      6
#define FEATURE      9
#define COLON      17
#define WS      15
#define SOURCES_DECL      14
#define PROJECT_REQUIREMENTS      13
#define FEATURE_LIST      8
#define RULE_CALL      4
#define COMMENT      19
#define STRING      18
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for hammerLexer
 * =============================================================================
 */
/** \} */

#endif

#ifdef __cplusplus
}
#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
