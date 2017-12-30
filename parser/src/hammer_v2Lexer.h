/** \file
 *  This C header file was generated by $ANTLR version 3.1.1
 *
 *     -  From the grammar source file : hammer_v2.g
 *     -                            On : 2017-12-26 09:58:36
 *     -                 for the lexer : hammer_v2LexerLexer *
 * Editing it, at least manually, is not wise. 
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The lexer hammer_v2Lexer has the callable functions (rules) shown below,
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
 * a parser context typedef phammer_v2Lexer, which is returned from a call to hammer_v2LexerNew().
 *
 * As this is a generated lexer, it is unlikely you will call it 'manually'. However
 * the methods are provided anyway.
 * * The methods in phammer_v2Lexer are  as follows:
 *
 *  -  void      phammer_v2Lexer->T__36(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->T__37(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->T__38(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->T__39(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->T__40(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->T__41(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->T__42(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->T__43(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->T__44(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->T__45(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->T__46(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->T__47(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->T__48(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->T__49(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->T__50(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->Slash(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->DoubleSlash(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->PublicTag(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->Local(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->Explicit(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->QuestionMark(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->Asterix(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->Id(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->STRING(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->STRING_ID(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->STRING_1(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->STRING_ID1(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->COMMENT(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->WS(phammer_v2Lexer)
 *  -  void      phammer_v2Lexer->Tokens(phammer_v2Lexer)
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
#ifndef	_hammer_v2Lexer_H
#define _hammer_v2Lexer_H
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
typedef struct hammer_v2Lexer_Ctx_struct hammer_v2Lexer, * phammer_v2Lexer;



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


/** Context tracking structure for hammer_v2Lexer
 */
struct hammer_v2Lexer_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_LEXER    pLexer;


     void (*mT__36)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mT__37)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mT__38)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mT__39)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mT__40)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mT__41)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mT__42)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mT__43)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mT__44)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mT__45)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mT__46)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mT__47)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mT__48)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mT__49)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mT__50)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mSlash)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mDoubleSlash)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mPublicTag)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mLocal)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mExplicit)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mQuestionMark)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mAsterix)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mId)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mSTRING)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mSTRING_ID)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mSTRING_1)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mSTRING_ID1)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mCOMMENT)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mWS)	(struct hammer_v2Lexer_Ctx_struct * ctx);
     void (*mTokens)	(struct hammer_v2Lexer_Ctx_struct * ctx);    const char * (*getGrammarFileName)();
    void	    (*free)   (struct hammer_v2Lexer_Ctx_struct * ctx);
        
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API phammer_v2Lexer hammer_v2LexerNew         (pANTLR3_INPUT_STREAM instream);
ANTLR3_API phammer_v2Lexer hammer_v2LexerNewSSD      (pANTLR3_INPUT_STREAM instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

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
#define T__50      50
#define Hamfile      4
#define Explicit      7
#define Feature      14
#define Wildcard      21
#define Slash      27
#define List      13
#define NamedArgument      10
#define STRING_1      32
#define Arguments      8
#define Local      6
#define EmptyArgument      9
#define Asterix      30
#define Rule      5
#define Id      25
#define StructureField      12
#define RuleInvocation      16
#define LogicalOr      23
#define COMMENT      35
#define T__37      37
#define T__38      38
#define T__39      39
#define LogicalAnd      22
#define STRING_ID      33
#define T__36      36
#define Structure      11
#define WS      24
#define DoubleSlash      28
#define EOF      -1
#define Condition      15
#define Path      17
#define PublicTag      26
#define TargetRefSpec      19
#define STRING_ID1      34
#define T__48      48
#define T__49      49
#define QuestionMark      29
#define TargetRef      18
#define T__44      44
#define T__45      45
#define STRING      31
#define T__46      46
#define T__47      47
#define T__40      40
#define T__41      41
#define T__42      42
#define T__43      43
#define TargetRefBuildRequest      20
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for hammer_v2Lexer
 * =============================================================================
 */
/** \} */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
