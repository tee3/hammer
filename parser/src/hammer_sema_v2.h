/** \file
 *  This C header file was generated by $ANTLR version 3.1.1
 *
 *     -  From the grammar source file : hammer_sema_v2.gt
 *     -                            On : 2019-01-13 11:13:19
 *     -           for the tree parser : hammer_sema_v2TreeParser *
 * Editing it, at least manually, is not wise. 
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The tree parser hammer_sema_v2 has the callable functions (rules) shown below,
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
 * a parser context typedef phammer_sema_v2, which is returned from a call to hammer_sema_v2New().
 *
 * The methods in phammer_sema_v2 are  as follows:
 *
 *  - const hammer::ast::hamfile*      phammer_sema_v2->hamfile(phammer_sema_v2)
 *  - void      phammer_sema_v2->statement(phammer_sema_v2)
 *  - const hammer::ast::statement*      phammer_sema_v2->top_level_rule_invocation(phammer_sema_v2)
 *  - void      phammer_sema_v2->target_def_prefix(phammer_sema_v2)
 *  - void      phammer_sema_v2->arguments(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->argument(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->named_argument(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->expression(phammer_sema_v2)
 *  - void      phammer_sema_v2->list_of(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->list_element(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->public_expression(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->expression_a(phammer_sema_v2)
 *  - const hammer::ast::path*      phammer_sema_v2->path(phammer_sema_v2)
 *  - void      phammer_sema_v2->path_root_name(phammer_sema_v2)
 *  - void      phammer_sema_v2->path_elements(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->path_element(phammer_sema_v2)
 *  - void      phammer_sema_v2->path_trailing_slash(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->wildcard(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->wildcard_elem(phammer_sema_v2)
 *  - void      phammer_sema_v2->public_tag(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->rule_invocation(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->struct_(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->struct_field(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->named_field(phammer_sema_v2)
 *  - const hammer::ast::feature*      phammer_sema_v2->feature(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->feature_value(phammer_sema_v2)
 *  - const hammer::ast::path*      phammer_sema_v2->target_ref_impl(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->target_ref(phammer_sema_v2)
 *  - const hammer::ast::path*      phammer_sema_v2->target_ref_impl_impl(phammer_sema_v2)
 *  - void      phammer_sema_v2->target_ref_spec(phammer_sema_v2)
 *  - void      phammer_sema_v2->target_ref_build_request(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->condition(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->condition_condition(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->condition_result(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->logical_or(phammer_sema_v2)
 *  - const hammer::ast::expression*      phammer_sema_v2->logical_and(phammer_sema_v2)
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
#ifndef	_hammer_sema_v2_H
#define _hammer_sema_v2_H
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
typedef struct hammer_sema_v2_Ctx_struct hammer_sema_v2, * phammer_sema_v2;




#include <hammer/sema/actions.h>
#include <hammer/parscore/identifier.h>
#include <hammer/ast/requirement_set.h>
#include <hammer/ast/path.h>
#include <hammer/ast/feature.h>
#include <hammer/ast/condition.h>

#define SEMA static_cast<hammer::sema::actions*>(PARSER->super)
#define DECLARE_TYPE(Type, name) Type name ( Type ::allocator_type{SEMA->get_context()});


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


/** Context tracking structure for hammer_sema_v2
 */
struct hammer_sema_v2_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_TREE_PARSER	    pTreeParser;


     const hammer::ast::hamfile* (*hamfile)	(struct hammer_sema_v2_Ctx_struct * ctx);
     void (*statement)	(struct hammer_sema_v2_Ctx_struct * ctx, hammer::ast::statements_t* statements);
     const hammer::ast::statement* (*top_level_rule_invocation)	(struct hammer_sema_v2_Ctx_struct * ctx);
     void (*target_def_prefix)	(struct hammer_sema_v2_Ctx_struct * ctx, hammer::parscore::source_location* explicit_tag, hammer::parscore::source_location* local_tag);
     void (*arguments)	(struct hammer_sema_v2_Ctx_struct * ctx, hammer::ast::expressions_t* args);
     const hammer::ast::expression* (*argument)	(struct hammer_sema_v2_Ctx_struct * ctx);
     const hammer::ast::expression* (*named_argument)	(struct hammer_sema_v2_Ctx_struct * ctx);
     const hammer::ast::expression* (*expression)	(struct hammer_sema_v2_Ctx_struct * ctx);
     void (*list_of)	(struct hammer_sema_v2_Ctx_struct * ctx, hammer::ast::expressions_t* args);
     const hammer::ast::expression* (*list_element)	(struct hammer_sema_v2_Ctx_struct * ctx);
     const hammer::ast::expression* (*public_expression)	(struct hammer_sema_v2_Ctx_struct * ctx);
     const hammer::ast::expression* (*expression_a)	(struct hammer_sema_v2_Ctx_struct * ctx);
     const hammer::ast::path* (*path)	(struct hammer_sema_v2_Ctx_struct * ctx);
     void (*path_root_name)	(struct hammer_sema_v2_Ctx_struct * ctx, hammer::parscore::token& root_name);
     void (*path_elements)	(struct hammer_sema_v2_Ctx_struct * ctx, hammer::ast::expressions_t& elements);
     const hammer::ast::expression* (*path_element)	(struct hammer_sema_v2_Ctx_struct * ctx);
     void (*path_trailing_slash)	(struct hammer_sema_v2_Ctx_struct * ctx, hammer::parscore::token& trailing_slash);
     const hammer::ast::expression* (*wildcard)	(struct hammer_sema_v2_Ctx_struct * ctx);
     const hammer::ast::expression* (*wildcard_elem)	(struct hammer_sema_v2_Ctx_struct * ctx);
     void (*public_tag)	(struct hammer_sema_v2_Ctx_struct * ctx, hammer::parscore::source_location* tag_loc);
     const hammer::ast::expression* (*rule_invocation)	(struct hammer_sema_v2_Ctx_struct * ctx);
     const hammer::ast::expression* (*struct_)	(struct hammer_sema_v2_Ctx_struct * ctx);
     const hammer::ast::expression* (*struct_field)	(struct hammer_sema_v2_Ctx_struct * ctx);
     const hammer::ast::expression* (*named_field)	(struct hammer_sema_v2_Ctx_struct * ctx);
     const hammer::ast::feature* (*feature)	(struct hammer_sema_v2_Ctx_struct * ctx);
     const hammer::ast::expression* (*feature_value)	(struct hammer_sema_v2_Ctx_struct * ctx);
     const hammer::ast::path* (*target_ref_impl)	(struct hammer_sema_v2_Ctx_struct * ctx, hammer::parscore::source_location* project_local_ref_tag, hammer::parscore::identifier* target_name, hammer::ast::features_t* build_request);
     const hammer::ast::expression* (*target_ref)	(struct hammer_sema_v2_Ctx_struct * ctx);
     const hammer::ast::path* (*target_ref_impl_impl)	(struct hammer_sema_v2_Ctx_struct * ctx, hammer::parscore::identifier* target_name, hammer::ast::features_t* build_request);
     void (*target_ref_spec)	(struct hammer_sema_v2_Ctx_struct * ctx, hammer::parscore::identifier* target_name, hammer::ast::features_t* build_request);
     void (*target_ref_build_request)	(struct hammer_sema_v2_Ctx_struct * ctx, hammer::ast::features_t* build_request);
     const hammer::ast::expression* (*condition)	(struct hammer_sema_v2_Ctx_struct * ctx);
     const hammer::ast::expression* (*condition_condition)	(struct hammer_sema_v2_Ctx_struct * ctx);
     const hammer::ast::expression* (*condition_result)	(struct hammer_sema_v2_Ctx_struct * ctx);
     const hammer::ast::expression* (*logical_or)	(struct hammer_sema_v2_Ctx_struct * ctx);
     const hammer::ast::expression* (*logical_and)	(struct hammer_sema_v2_Ctx_struct * ctx);
    // Delegated rules
    const char * (*getGrammarFileName)();
    void	    (*free)   (struct hammer_sema_v2_Ctx_struct * ctx);
        
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API phammer_sema_v2 hammer_sema_v2New         (pANTLR3_COMMON_TREE_NODE_STREAM instream);
ANTLR3_API phammer_sema_v2 hammer_sema_v2NewSSD      (pANTLR3_COMMON_TREE_NODE_STREAM instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

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
#define T__50      50
#define Hamfile      4
#define EmptyField      12
#define Explicit      7
#define PathTrailingSlash      20
#define Feature      15
#define Wildcard      24
#define T__55      55
#define Slash      30
#define List      14
#define T__51      51
#define NamedArgument      10
#define DoubleAsterix      36
#define T__52      52
#define T__53      53
#define T__54      54
#define STRING_1      38
#define Arguments      8
#define Local      6
#define EmptyArgument      9
#define Asterix      35
#define Rule      5
#define Id      28
#define RuleInvocation      17
#define ProjectLocalRefTag      31
#define LogicalOr      26
#define COMMENT      41
#define NamedField      13
#define PathRootName      19
#define LogicalAnd      25
#define STRING_ID      39
#define Structure      11
#define WS      27
#define DoubleSlash      33
#define EOF      -1
#define Condition      16
#define Path      18
#define PublicTag      29
#define TargetRefSpec      22
#define PathUri      32
#define STRING_ID1      40
#define T__48      48
#define T__49      49
#define QuestionMark      34
#define TargetRef      21
#define T__44      44
#define T__45      45
#define STRING      37
#define T__46      46
#define T__47      47
#define T__42      42
#define T__43      43
#define TargetRefBuildRequest      23
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for hammer_sema_v2
 * =============================================================================
 */
/** \} */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
