/** \file
 *  This C header file was generated by $ANTLR version 3.1b1
 *
 *     -  From the grammar source file : hammer.g
 *     -                            On : 2008-05-27 21:50:01
 *     -                for the parser : hammerParserParser *
 * Editing it, at least manually, is not wise. 
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The parser hammerParser has the callable functions (rules) shown below,
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
 * a parser context typedef phammerParser, which is returned from a call to hammerParserNew().
 *
 * The methods in phammerParser are  as follows:
 *
 *  - hammerParser_project_return      phammerParser->project(phammerParser)
 *  - hammerParser_rules_return      phammerParser->rules(phammerParser)
 *  - hammerParser_rule_return      phammerParser->rule(phammerParser)
 *  - hammerParser_rule_args_return      phammerParser->rule_args(phammerParser)
 *  - hammerParser_maybe_arg_return      phammerParser->maybe_arg(phammerParser)
 *  - hammerParser_rule_posible_args_return      phammerParser->rule_posible_args(phammerParser)
 *  - hammerParser_string_list_return      phammerParser->string_list(phammerParser)
 *  - hammerParser_feature_list_return      phammerParser->feature_list(phammerParser)
 *  - hammerParser_feature_return      phammerParser->feature(phammerParser)
 *  - hammerParser_string_return      phammerParser->string(phammerParser)
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
#ifndef	_hammerParser_H
#define _hammerParser_H
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
typedef struct hammerParser_Ctx_struct hammerParser, * phammerParser;



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
typedef struct hammerParser_project_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    hammerParser_project_return;

typedef struct hammerParser_rules_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    hammerParser_rules_return;

typedef struct hammerParser_rule_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    hammerParser_rule_return;

typedef struct hammerParser_rule_args_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    hammerParser_rule_args_return;

typedef struct hammerParser_maybe_arg_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    hammerParser_maybe_arg_return;

typedef struct hammerParser_rule_posible_args_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    hammerParser_rule_posible_args_return;

typedef struct hammerParser_string_list_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    hammerParser_string_list_return;

typedef struct hammerParser_feature_list_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    hammerParser_feature_list_return;

typedef struct hammerParser_feature_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    hammerParser_feature_return;

typedef struct hammerParser_string_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    hammerParser_string_return;




/** Context tracking structure for hammerParser
 */
struct hammerParser_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_PARSER   pParser;


     hammerParser_project_return (*project)	(struct hammerParser_Ctx_struct * ctx);
     hammerParser_rules_return (*rules)	(struct hammerParser_Ctx_struct * ctx);
     hammerParser_rule_return (*rule)	(struct hammerParser_Ctx_struct * ctx);
     hammerParser_rule_args_return (*rule_args)	(struct hammerParser_Ctx_struct * ctx);
     hammerParser_maybe_arg_return (*maybe_arg)	(struct hammerParser_Ctx_struct * ctx);
     hammerParser_rule_posible_args_return (*rule_posible_args)	(struct hammerParser_Ctx_struct * ctx);
     hammerParser_string_list_return (*string_list)	(struct hammerParser_Ctx_struct * ctx);
     hammerParser_feature_list_return (*feature_list)	(struct hammerParser_Ctx_struct * ctx);
     hammerParser_feature_return (*feature)	(struct hammerParser_Ctx_struct * ctx);
     hammerParser_string_return (*string)	(struct hammerParser_Ctx_struct * ctx);
    // Delegated rules
    const char * (*getGrammarFileName)();
    void	    (*free)   (struct hammerParser_Ctx_struct * ctx);
    /* @headerFile.members() */
    pANTLR3_BASE_TREE_ADAPTOR	adaptor;
    pANTLR3_VECTOR_FACTORY		vectors;
    /* End @headerFile.members() */
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API phammerParser hammerParserNew         (pANTLR3_COMMON_TOKEN_STREAM instream);
ANTLR3_API phammerParser hammerParserNewSSD      (pANTLR3_COMMON_TOKEN_STREAM instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the parser will work with.
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
#define STRING_LIST      6
#define WS      12
#define T__16      16
#define T__15      15
#define T__14      14
#define FEATURE_LIST      7
#define T__13      13
#define RULE_CALL      4
#define COMMENT      11
#define ID      9
#define EOF      -1
#define NULL_ARG      5
#define FEATURE      8
#define STRING      10
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for hammerParser
 * =============================================================================
 */
/** \} */

#endif

#ifdef __cplusplus
}
#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
