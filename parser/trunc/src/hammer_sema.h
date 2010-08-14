/** \file
 *  This C header file was generated by $ANTLR version 3.1.1
 *
 *     -  From the grammar source file : hammer_sema.gt
 *     -                            On : 2010-08-14 16:03:12
 *     -           for the tree parser : hammer_semaTreeParser *
 * Editing it, at least manually, is not wise. 
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The tree parser hammer_sema has the callable functions (rules) shown below,
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
 * a parser context typedef phammer_sema, which is returned from a call to hammer_semaNew().
 *
 * The methods in phammer_sema are  as follows:
 *
 *  - const hammer::ast::hamfile*      phammer_sema->hamfile(phammer_sema)
 *  - const hammer::ast::project_def*      phammer_sema->project_def(phammer_sema)
 *  - void      phammer_sema->statement(phammer_sema)
 *  - const hammer::ast::expression*      phammer_sema->target_def_or_rule_call(phammer_sema)
 *  - void      phammer_sema->arguments(phammer_sema)
 *  - const hammer::ast::expression*      phammer_sema->argument(phammer_sema)
 *  - const hammer::ast::expression*      phammer_sema->named_expression(phammer_sema)
 *  - const hammer::ast::expression*      phammer_sema->expression(phammer_sema)
 *  - const hammer::ast::requirement_set*      phammer_sema->requirement_set(phammer_sema)
 *  - const hammer::ast::requirement*      phammer_sema->requirement(phammer_sema)
 *  - const hammer::ast::requirement*      phammer_sema->conditional_requirement(phammer_sema)
 *  - const hammer::ast::requirement*      phammer_sema->simple_requirement(phammer_sema)
 *  - const hammer::ast::feature*      phammer_sema->feature(phammer_sema)
 *  - const hammer::ast::expression*      phammer_sema->feature_value(phammer_sema)
 *  - void      phammer_sema->list_of(phammer_sema)
 *  - const hammer::ast::expression*      phammer_sema->list_of_impl(phammer_sema)
 *  - const hammer::ast::expression*      phammer_sema->target_ref(phammer_sema)
 *  - void      phammer_sema->public_tag(phammer_sema)
 *  - void      phammer_sema->target_ref_name(phammer_sema)
 *  - const hammer::ast::requirement_set*      phammer_sema->target_ref_requirements(phammer_sema)
 *  - const hammer::ast::path_like_seq*      phammer_sema->path_like_seq(phammer_sema)
 *  - void      phammer_sema->path_like_seq_impl(phammer_sema)
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
#ifndef	_hammer_sema_H
#define _hammer_sema_H
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
typedef struct hammer_sema_Ctx_struct hammer_sema, * phammer_sema;




#include <hammer/sema/actions.h>
#include <hammer/parscore/identifier.h>
#include <hammer/ast/requirement_set.h>
#include <hammer/ast/requirement.h>
#include <hammer/ast/path_like_seq.h>

#define SEMA static_cast<const hammer::sema::actions*>(PARSER->super)


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


/** Context tracking structure for hammer_sema
 */
struct hammer_sema_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_TREE_PARSER	    pTreeParser;


     const hammer::ast::hamfile* (*hamfile)	(struct hammer_sema_Ctx_struct * ctx);
     const hammer::ast::project_def* (*project_def)	(struct hammer_sema_Ctx_struct * ctx);
     void (*statement)	(struct hammer_sema_Ctx_struct * ctx, hammer::ast::statements_t* statements);
     const hammer::ast::expression* (*target_def_or_rule_call)	(struct hammer_sema_Ctx_struct * ctx);
     void (*arguments)	(struct hammer_sema_Ctx_struct * ctx, hammer::ast::expressions_t* args);
     const hammer::ast::expression* (*argument)	(struct hammer_sema_Ctx_struct * ctx);
     const hammer::ast::expression* (*named_expression)	(struct hammer_sema_Ctx_struct * ctx);
     const hammer::ast::expression* (*expression)	(struct hammer_sema_Ctx_struct * ctx);
     const hammer::ast::requirement_set* (*requirement_set)	(struct hammer_sema_Ctx_struct * ctx);
     const hammer::ast::requirement* (*requirement)	(struct hammer_sema_Ctx_struct * ctx);
     const hammer::ast::requirement* (*conditional_requirement)	(struct hammer_sema_Ctx_struct * ctx);
     const hammer::ast::requirement* (*simple_requirement)	(struct hammer_sema_Ctx_struct * ctx);
     const hammer::ast::feature* (*feature)	(struct hammer_sema_Ctx_struct * ctx);
     const hammer::ast::expression* (*feature_value)	(struct hammer_sema_Ctx_struct * ctx);
     void (*list_of)	(struct hammer_sema_Ctx_struct * ctx, hammer::ast::expressions_t* args);
     const hammer::ast::expression* (*list_of_impl)	(struct hammer_sema_Ctx_struct * ctx);
     const hammer::ast::expression* (*target_ref)	(struct hammer_sema_Ctx_struct * ctx);
     void (*public_tag)	(struct hammer_sema_Ctx_struct * ctx, hammer::parscore::source_location* tag_loc);
     void (*target_ref_name)	(struct hammer_sema_Ctx_struct * ctx, hammer::parscore::identifier* name);
     const hammer::ast::requirement_set* (*target_ref_requirements)	(struct hammer_sema_Ctx_struct * ctx);
     const hammer::ast::path_like_seq* (*path_like_seq)	(struct hammer_sema_Ctx_struct * ctx);
     void (*path_like_seq_impl)	(struct hammer_sema_Ctx_struct * ctx, std::pair<pANTLR3_COMMON_TOKEN, pANTLR3_COMMON_TOKEN>* bounds);
    // Delegated rules
    const char * (*getGrammarFileName)();
    void	    (*free)   (struct hammer_sema_Ctx_struct * ctx);
        
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API phammer_sema hammer_semaNew         (pANTLR3_COMMON_TREE_NODE_STREAM instream);
ANTLR3_API phammer_sema hammer_semaNewSSD      (pANTLR3_COMMON_TREE_NODE_STREAM instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the tree parser will work with.
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
#define TARGET_REF      8
#define T__29      29
#define TARGET_NAME      9
#define ARGUMENTS      11
#define STRING_ID      27
#define LIST_OF      15
#define EMPTY_EXPRESSION      14
#define CONDITION      19
#define IMPLICIT_PROJECT_DEF      6
#define ID      22
#define EOF      -1
#define FEATURE      20
#define REQUIREMENT      18
#define COLON      23
#define T__30      30
#define T__31      31
#define T__32      32
#define T__33      33
#define WS      21
#define SLASH      24
#define TARGET_DECL_OR_RULE_CALL      7
#define T__34      34
#define EMPTY_TARGET_NAME      10
#define T__35      35
#define T__36      36
#define EXPLICIT_PROJECT_DEF      5
#define T__37      37
#define T__38      38
#define T__39      39
#define PUBLIC_TAG      25
#define HAMFILE      4
#define PATH_LIKE_SEQ      16
#define REQUIREMENT_SET      17
#define COMMENT      28
#define NAMED_EXPRESSION      12
#define EXPRESSION      13
#define STRING      26
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for hammer_sema
 * =============================================================================
 */
/** \} */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
