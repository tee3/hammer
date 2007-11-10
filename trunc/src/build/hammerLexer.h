/** \file
 *  This C header file was generated by $ANTLR version 3.0.1
 *
 *     -  From the grammar source file : D:\\Projects\\C++\\hammer\\src\\hammer.g
 *     -                            On : 2007-11-10 14:39:53
 *     -                 for the lexer : hammerLexerLexer *
 * Editing it, at least manually, is not wise. 
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 * View this file with tabs set to 8 (:set ts=8 in gvim) and indent at 4 (:set sw=4 in gvim)
 *
 * The lexer hammerLexerhas the callable functions (rules) shown below,
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
 * the entry points are provided anyway.
 * * The entry points for hammerLexer are  as follows:
 *
 *  - void      phammerLexer->T14(phammerLexer)
 *  - void      phammerLexer->T15(phammerLexer)
 *  - void      phammerLexer->T16(phammerLexer)
 *  - void      phammerLexer->T17(phammerLexer)
 *  - void      phammerLexer->T18(phammerLexer)
 *  - void      phammerLexer->T19(phammerLexer)
 *  - void      phammerLexer->T20(phammerLexer)
 *  - void      phammerLexer->T21(phammerLexer)
 *  - void      phammerLexer->T22(phammerLexer)
 *  - void      phammerLexer->T23(phammerLexer)
 *  - void      phammerLexer->T24(phammerLexer)
 *  - void      phammerLexer->T25(phammerLexer)
 *  - void      phammerLexer->T26(phammerLexer)
 *  - void      phammerLexer->T27(phammerLexer)
 *  - void      phammerLexer->T28(phammerLexer)
 *  - void      phammerLexer->T29(phammerLexer)
 *  - void      phammerLexer->T30(phammerLexer)
 *  - void      phammerLexer->T31(phammerLexer)
 *  - void      phammerLexer->T32(phammerLexer)
 *  - void      phammerLexer->T33(phammerLexer)
 *  - void      phammerLexer->T34(phammerLexer)
 *  - void      phammerLexer->T35(phammerLexer)
 *  - void      phammerLexer->T36(phammerLexer)
 *  - void      phammerLexer->T37(phammerLexer)
 *  - void      phammerLexer->T38(phammerLexer)
 *  - void      phammerLexer->T39(phammerLexer)
 *  - void      phammerLexer->T40(phammerLexer)
 *  - void      phammerLexer->T41(phammerLexer)
 *  - void      phammerLexer->ASSIGN(phammerLexer)
 *  - void      phammerLexer->ID(phammerLexer)
 *  - void      phammerLexer->JAM_STRING(phammerLexer)
 *  - void      phammerLexer->JAM_COMMENT(phammerLexer)
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

#ifdef	WIN32
// Disable: Unreferenced parameter,                - Rules with parameters that are not used
//          constant conditional,                  - ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable        - tree rewrite vairables declared but not needed
//          Unreferenced local variable            - lexer rulle decalres but does not always use _type
//          potentially unitialized variable used  - retval always returned from a rule 
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
#pragma warning( disable : 4701 )
#endif



/** Context tracking structure for hammerLexer
 */
typedef struct hammerLexer_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_LEXER    pLexer;

    void (*mT14)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT15)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT16)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT17)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT18)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT19)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT20)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT21)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT22)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT23)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT24)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT25)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT26)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT27)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT28)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT29)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT30)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT31)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT32)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT33)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT34)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT35)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT36)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT37)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT38)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT39)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT40)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mT41)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mASSIGN)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mID)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mJAM_STRING)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mJAM_COMMENT)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mWS)	(struct hammerLexer_Ctx_struct * ctx);
    void (*mTokens)	(struct hammerLexer_Ctx_struct * ctx);    unsigned char * (*getGrammarFileName)();
    void	    (*free)   (struct hammerLexer_Ctx_struct * ctx);
        
}
    hammerLexer, * phammerLexer;

/* Function protoypes for the lexer functions that external translation units
 * may wish to call.
 */
ANTLR3_API phammerLexer hammerLexerNew         (pANTLR3_INPUT_STREAM     instream);/** Symbolic definitions of all the tokens that the lexer will work with.
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
#define T21      21
#define T14      14
#define T29      29
#define T33      33
#define T22      22
#define T36      36
#define WS      13
#define PROJECT_DEF      5
#define T28      28
#define T23      23
#define T40      40
#define T35      35
#define SOURCES      8
#define T34      34
#define T20      20
#define T25      25
#define T37      37
#define T18      18
#define T26      26
#define T15      15
#define EOF      -1
#define T32      32
#define ASSIGN      10
#define JAM_STRING      11
#define LIB      6
#define T17      17
#define Tokens      42
#define T31      31
#define PROJECT      4
#define T16      16
#define T38      38
#define T27      27
#define T41      41
#define T30      30
#define T24      24
#define EXE      7
#define T19      19
#define T39      39
#define JAM_COMMENT      12
#define ID      9
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

/* End of token definitions for hammerLexer
 * =============================================================================
 */
/** \} */

#endif
/* END - Note:Keep extra linefeed to satisfy UNIX systems */
