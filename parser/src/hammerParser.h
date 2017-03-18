/** \file
 *  This C header file was generated by $ANTLR version 3.1.1
 *
 *     -  From the grammar source file : hammer.g
 *     -                            On : 2016-11-27 09:55:58
 *     -                for the parser : hammerParserParser *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle,
 * jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The parser hammerParser has the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could
 * begin
 * this rule.
 *
 * For instance if you call the first (topmost) rule in a parser grammar, you
 * will
 * get the results of a full parse, but calling a rule half way through the
 * grammar will
 * allow you to pass part of a full token stream to the parser, such as for
 * syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to
 * function) via
 * a parser context typedef phammerParser, which is returned from a call to
 * hammerParserNew().
 *
 * The methods in phammerParser are  as follows:
 *
 *  - hammerParser_hamfile_return      phammerParser->hamfile(phammerParser)
 *  - hammerParser_target_decl_or_rule_call_return
 * phammerParser->target_decl_or_rule_call(phammerParser)
 *  - hammerParser_target_decl_or_rule_call_impl_return
 * phammerParser->target_decl_or_rule_call_impl(phammerParser)
 *  - hammerParser_arguments_return      phammerParser->arguments(phammerParser)
 *  - hammerParser_args_leaf_return      phammerParser->args_leaf(phammerParser)
 *  - hammerParser_non_empty_argument_return
 * phammerParser->non_empty_argument(phammerParser)
 *  - hammerParser_named_argument_return
 * phammerParser->named_argument(phammerParser)
 *  - hammerParser_named_argument_expression_return
 * phammerParser->named_argument_expression(phammerParser)
 *  - hammerParser_argument_return      phammerParser->argument(phammerParser)
 *  - hammerParser_argument_name_return
 * phammerParser->argument_name(phammerParser)
 *  - hammerParser_expression_return phammerParser->expression(phammerParser)
 *  - hammerParser_feature_return      phammerParser->feature(phammerParser)
 *  - hammerParser_feature_value_return
 * phammerParser->feature_value(phammerParser)
 *  - hammerParser_requirement_set_return
 * phammerParser->requirement_set(phammerParser)
 *  - hammerParser_requirement_return phammerParser->requirement(phammerParser)
 *  - hammerParser_requirement_impl_return
 * phammerParser->requirement_impl(phammerParser)
 *  - hammerParser_conditional_requirement_return
 * phammerParser->conditional_requirement(phammerParser)
 *  - hammerParser_condition_return      phammerParser->condition(phammerParser)
 *  - hammerParser_path_like_seq_return
 * phammerParser->path_like_seq(phammerParser)
 *  - hammerParser_path_like_seq_impl_return
 * phammerParser->path_like_seq_impl(phammerParser)
 *  - hammerParser_target_ref_return phammerParser->target_ref(phammerParser)
 *  - hammerParser_target_ref_impl_return
 * phammerParser->target_ref_impl(phammerParser)
 *  - hammerParser_target_name_seq_return
 * phammerParser->target_name_seq(phammerParser)
 *  - hammerParser_target_requirements_return
 * phammerParser->target_requirements(phammerParser)
 *  - hammerParser_list_of_return      phammerParser->list_of(phammerParser)
 *  - hammerParser_list_of_impl_return
 * phammerParser->list_of_impl(phammerParser)
 *  - hammerParser_public_tag_return phammerParser->public_tag(phammerParser)
 *
 *
 *
 *
 *
 *
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
#ifndef _hammerParser_H
#define _hammerParser_H

/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include <antlr3.h>

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
typedef struct hammerParser_Ctx_struct hammerParser, *phammerParser;

#ifdef ANTLR3_WINDOWS
// Disable: Unreferenced parameter,
// -
// Rules
// with parameters that are not used
//          constant conditional,
//          -
//          ANTLR
//          realizes
//          that a prediction is always true (synpred usually)
//          initialized but unused variable -
//          tree
//          rewrite
//          variables declared but not needed
//          Unreferenced local variable -
//          lexer
//          rule
//          declares but does not always use _type
//          potentially unitialized variable used			- retval
//          always
//          returned from a rule
//			unreferenced local function has been removed	-
// susually
// getTokenNames or freeScope, they can go without warnigns
//
// These are only really displayed at warning level /W4 but that is the code
// ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from
// 4100, which is
// usually generated when a parser rule is given a parameter that it does not
// use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning(disable : 4100)
#pragma warning(disable : 4101)
#pragma warning(disable : 4127)
#pragma warning(disable : 4189)
#pragma warning(disable : 4505)
#endif

/* ========================
 * BACKTRACKING IS ENABLED
 * ========================
 */
typedef struct hammerParser_hamfile_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_hamfile_return;

typedef struct hammerParser_target_decl_or_rule_call_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_target_decl_or_rule_call_return;

typedef struct hammerParser_target_decl_or_rule_call_impl_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_target_decl_or_rule_call_impl_return;

typedef struct hammerParser_arguments_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_arguments_return;

typedef struct hammerParser_args_leaf_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_args_leaf_return;

typedef struct hammerParser_non_empty_argument_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_non_empty_argument_return;

typedef struct hammerParser_named_argument_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_named_argument_return;

typedef struct hammerParser_named_argument_expression_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_named_argument_expression_return;

typedef struct hammerParser_argument_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_argument_return;

typedef struct hammerParser_argument_name_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_argument_name_return;

typedef struct hammerParser_expression_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_expression_return;

typedef struct hammerParser_feature_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_feature_return;

typedef struct hammerParser_feature_value_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_feature_value_return;

typedef struct hammerParser_requirement_set_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_requirement_set_return;

typedef struct hammerParser_requirement_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_requirement_return;

typedef struct hammerParser_requirement_impl_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_requirement_impl_return;

typedef struct hammerParser_conditional_requirement_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_conditional_requirement_return;

typedef struct hammerParser_condition_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_condition_return;

typedef struct hammerParser_path_like_seq_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_path_like_seq_return;

typedef struct hammerParser_path_like_seq_impl_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_path_like_seq_impl_return;

typedef struct hammerParser_target_ref_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_target_ref_return;

typedef struct hammerParser_target_ref_impl_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_target_ref_impl_return;

typedef struct hammerParser_target_name_seq_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_target_name_seq_return;

typedef struct hammerParser_target_requirements_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_target_requirements_return;

typedef struct hammerParser_list_of_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_list_of_return;

typedef struct hammerParser_list_of_impl_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_list_of_impl_return;

typedef struct hammerParser_public_tag_return_struct
{
  /** Generic return elements for ANTLR3 rules that are not in tree parsers or
 * returning trees
 */
  pANTLR3_COMMON_TOKEN start;
  pANTLR3_COMMON_TOKEN stop;
  pANTLR3_BASE_TREE tree;

} hammerParser_public_tag_return;

/** Context tracking structure for hammerParser
 */
struct hammerParser_Ctx_struct
{
  /** Built in ANTLR3 context tracker contains all the generic elements
 *  required for context tracking.
 */
  pANTLR3_PARSER pParser;

  hammerParser_hamfile_return (*hamfile)(struct hammerParser_Ctx_struct* ctx);
  hammerParser_target_decl_or_rule_call_return (*target_decl_or_rule_call)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_target_decl_or_rule_call_impl_return (
    *target_decl_or_rule_call_impl)(struct hammerParser_Ctx_struct* ctx);
  hammerParser_arguments_return (*arguments)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_args_leaf_return (*args_leaf)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_non_empty_argument_return (*non_empty_argument)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_named_argument_return (*named_argument)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_named_argument_expression_return (*named_argument_expression)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_argument_return (*argument)(struct hammerParser_Ctx_struct* ctx);
  hammerParser_argument_name_return (*argument_name)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_expression_return (*expression)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_feature_return (*feature)(struct hammerParser_Ctx_struct* ctx);
  hammerParser_feature_value_return (*feature_value)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_requirement_set_return (*requirement_set)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_requirement_return (*requirement)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_requirement_impl_return (*requirement_impl)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_conditional_requirement_return (*conditional_requirement)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_condition_return (*condition)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_path_like_seq_return (*path_like_seq)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_path_like_seq_impl_return (*path_like_seq_impl)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_target_ref_return (*target_ref)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_target_ref_impl_return (*target_ref_impl)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_target_name_seq_return (*target_name_seq)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_target_requirements_return (*target_requirements)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_list_of_return (*list_of)(struct hammerParser_Ctx_struct* ctx);
  hammerParser_list_of_impl_return (*list_of_impl)(
    struct hammerParser_Ctx_struct* ctx);
  hammerParser_public_tag_return (*public_tag)(
    struct hammerParser_Ctx_struct* ctx);
  ANTLR3_BOOLEAN(*synpred1_hammer)
  (struct hammerParser_Ctx_struct* ctx);
  ANTLR3_BOOLEAN(*synpred2_hammer)
  (struct hammerParser_Ctx_struct* ctx);
  ANTLR3_BOOLEAN(*synpred3_hammer)
  (struct hammerParser_Ctx_struct* ctx);
  ANTLR3_BOOLEAN(*synpred4_hammer)
  (struct hammerParser_Ctx_struct* ctx);
  ANTLR3_BOOLEAN(*synpred5_hammer)
  (struct hammerParser_Ctx_struct* ctx);
  ANTLR3_BOOLEAN(*synpred6_hammer)
  (struct hammerParser_Ctx_struct* ctx);
  // Delegated rules
  const char* (*getGrammarFileName)();
  void (*free)(struct hammerParser_Ctx_struct* ctx);
  /* @headerFile.members() */
  pANTLR3_BASE_TREE_ADAPTOR adaptor;
  pANTLR3_VECTOR_FACTORY vectors;
  /* End @headerFile.members() */
};

// Function protoypes for the constructor functions that external translation
// units
// such as delegators and delegates may wish to call.
//
ANTLR3_API phammerParser
hammerParserNew(pANTLR3_COMMON_TOKEN_STREAM instream);
ANTLR3_API phammerParser
hammerParserNewSSD(pANTLR3_COMMON_TOKEN_STREAM instream,
                   pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the parser will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this
 * out at the moment
 * so we just undef it here for now. That isn't the value we get back from C
 * recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef EOF
#undef EOF
#endif
#ifdef Tokens
#undef Tokens
#endif
#define EMPTY_EXPRESSION 12
#define COMMENT 27
#define T__33 33
#define T__34 34
#define STRING_ID 26
#define T__35 35
#define T__36 36
#define ID 21
#define WS 19
#define EOF -1
#define T__30 30
#define T__31 31
#define T__32 32
#define NAMED_EXPRESSION 10
#define REQUIREMENT 16
#define PUBLIC_TAG 24
#define PATH_LIKE_SEQ 14
#define SLASH 23
#define TARGET_NAME 7
#define EMPTY_TARGET_NAME 8
#define COLON 22
#define ARGUMENTS 9
#define EXP_END 20
#define T__28 28
#define T__29 29
#define REQUIREMENT_SET 15
#define FEATURE 18
#define HAMFILE 4
#define STRING 25
#define CONDITION 17
#define TARGET_REF 6
#define TARGET_DECL_OR_RULE_CALL 5
#define LIST_OF 13
#define EXPRESSION 11
#ifdef EOF
#undef EOF
#define EOF ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for hammerParser
 * =============================================================================
 */
/** \} */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
