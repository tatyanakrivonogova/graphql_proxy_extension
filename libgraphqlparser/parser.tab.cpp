// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "parser.tab.hpp"


// Unqualified %code blocks.
#line 157 "parser.ypp"

#include "lexer.h"
#include "syntaxdefs.h"

#line 51 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 143 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"

  /// Build a parser object.
  GraphQLParserImpl::GraphQLParserImpl (bool enableSchema_yyarg, Node **outAST_yyarg, const char **outError_yyarg, void *scanner_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      enableSchema (enableSchema_yyarg),
      outAST (outAST_yyarg),
      outError (outError_yyarg),
      scanner (scanner_yyarg)
  {}

  GraphQLParserImpl::~GraphQLParserImpl ()
  {}

  GraphQLParserImpl::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  GraphQLParserImpl::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
    , location (that.location)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  GraphQLParserImpl::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_MOVE_REF (location_type) l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  GraphQLParserImpl::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (value_type) v, YY_RVREF (location_type) l)
    : Base (t)
    , value (YY_MOVE (v))
    , location (YY_MOVE (l))
  {}


  template <typename Base>
  GraphQLParserImpl::symbol_kind_type
  GraphQLParserImpl::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  GraphQLParserImpl::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  GraphQLParserImpl::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
    location = YY_MOVE (s.location);
  }

  // by_kind.
  GraphQLParserImpl::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  GraphQLParserImpl::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  GraphQLParserImpl::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  GraphQLParserImpl::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  GraphQLParserImpl::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  GraphQLParserImpl::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  GraphQLParserImpl::symbol_kind_type
  GraphQLParserImpl::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  GraphQLParserImpl::symbol_kind_type
  GraphQLParserImpl::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  GraphQLParserImpl::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  GraphQLParserImpl::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  GraphQLParserImpl::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  GraphQLParserImpl::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  GraphQLParserImpl::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  GraphQLParserImpl::symbol_kind_type
  GraphQLParserImpl::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  GraphQLParserImpl::stack_symbol_type::stack_symbol_type ()
  {}

  GraphQLParserImpl::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value), YY_MOVE (that.location))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  GraphQLParserImpl::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value), YY_MOVE (that.location))
  {
    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  GraphQLParserImpl::stack_symbol_type&
  GraphQLParserImpl::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }

  GraphQLParserImpl::stack_symbol_type&
  GraphQLParserImpl::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  GraphQLParserImpl::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    switch (yysym.kind ())
    {
      case symbol_kind::S_DIRECTIVE: // "directive"
#line 289 "parser.ypp"
                    { }
#line 358 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_ENUM: // "enum"
#line 289 "parser.ypp"
                    { }
#line 364 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_EXTEND: // "extend"
#line 289 "parser.ypp"
                    { }
#line 370 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_FALSE: // "false"
#line 289 "parser.ypp"
                    { }
#line 376 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_FRAGMENT: // "fragment"
#line 289 "parser.ypp"
                    { }
#line 382 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_IMPLEMENTS: // "implements"
#line 289 "parser.ypp"
                    { }
#line 388 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_INPUT: // "input"
#line 289 "parser.ypp"
                    { }
#line 394 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_INTERFACE: // "interface"
#line 289 "parser.ypp"
                    { }
#line 400 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_MUTATION: // "mutation"
#line 289 "parser.ypp"
                    { }
#line 406 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_NULL: // "null"
#line 289 "parser.ypp"
                    { }
#line 412 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_QUERY: // "query"
#line 289 "parser.ypp"
                    { }
#line 418 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_ON: // "on"
#line 289 "parser.ypp"
                    { }
#line 424 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_SCALAR: // "scalar"
#line 289 "parser.ypp"
                    { }
#line 430 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_SCHEMA: // "schema"
#line 289 "parser.ypp"
                    { }
#line 436 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_SUBSCRIPTION: // "subscription"
#line 289 "parser.ypp"
                    { }
#line 442 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_TRUE: // "true"
#line 289 "parser.ypp"
                    { }
#line 448 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_TYPE: // "type"
#line 289 "parser.ypp"
                    { }
#line 454 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_UNION: // "union"
#line 289 "parser.ypp"
                    { }
#line 460 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_VARIABLE: // VARIABLE
#line 289 "parser.ypp"
                    { }
#line 466 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 289 "parser.ypp"
                    { }
#line 472 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_FLOAT: // FLOAT
#line 289 "parser.ypp"
                    { }
#line 478 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_STRING: // STRING
#line 289 "parser.ypp"
                    { }
#line 484 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 289 "parser.ypp"
                    { }
#line 490 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_start: // start
#line 291 "parser.ypp"
                    { }
#line 496 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_fragment_name: // fragment_name
#line 292 "parser.ypp"
                    { delete (yysym.value.name); }
#line 502 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_name: // name
#line 292 "parser.ypp"
                    { delete (yysym.value.name); }
#line 508 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_name_opt: // name_opt
#line 292 "parser.ypp"
                    { delete (yysym.value.name); }
#line 514 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_document: // document
#line 291 "parser.ypp"
                    { }
#line 520 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_definition_list: // definition_list
#line 292 "parser.ypp"
                    { delete (yysym.value.definitionList); }
#line 526 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_definition: // definition
#line 292 "parser.ypp"
                    { delete (yysym.value.definition); }
#line 532 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_schema_gate: // schema_gate
#line 292 "parser.ypp"
                    { delete (yysym.value.definition); }
#line 538 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_operation_definition: // operation_definition
#line 292 "parser.ypp"
                    { delete (yysym.value.operationDefinition); }
#line 544 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_operation_type: // operation_type
#line 290 "parser.ypp"
                    { free((void *)(yysym.value.heapStr)); }
#line 550 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_variable_definitions: // variable_definitions
#line 292 "parser.ypp"
                    { delete (yysym.value.variableDefinitionList); }
#line 556 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_variable_definition_list: // variable_definition_list
#line 292 "parser.ypp"
                    { delete (yysym.value.variableDefinitionList); }
#line 562 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_variable: // variable
#line 292 "parser.ypp"
                    { delete (yysym.value.variable); }
#line 568 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_variable_definition: // variable_definition
#line 292 "parser.ypp"
                    { delete (yysym.value.variableDefinition); }
#line 574 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_default_value_opt: // default_value_opt
#line 292 "parser.ypp"
                    { delete (yysym.value.value); }
#line 580 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_default_value: // default_value
#line 292 "parser.ypp"
                    { delete (yysym.value.value); }
#line 586 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_selection_set: // selection_set
#line 292 "parser.ypp"
                    { delete (yysym.value.selectionSet); }
#line 592 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_selection_set_opt: // selection_set_opt
#line 292 "parser.ypp"
                    { delete (yysym.value.selectionSet); }
#line 598 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_selection_list: // selection_list
#line 292 "parser.ypp"
                    { delete (yysym.value.selectionList); }
#line 604 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_selection: // selection
#line 292 "parser.ypp"
                    { delete (yysym.value.selection); }
#line 610 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_field: // field
#line 292 "parser.ypp"
                    { delete (yysym.value.field); }
#line 616 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_arguments: // arguments
#line 292 "parser.ypp"
                    { delete (yysym.value.argumentList); }
#line 622 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_arguments_opt: // arguments_opt
#line 292 "parser.ypp"
                    { delete (yysym.value.argumentList); }
#line 628 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_argument_list: // argument_list
#line 292 "parser.ypp"
                    { delete (yysym.value.argumentList); }
#line 634 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_argument: // argument
#line 292 "parser.ypp"
                    { delete (yysym.value.argument); }
#line 640 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_fragment_spread: // fragment_spread
#line 292 "parser.ypp"
                    { delete (yysym.value.fragmentSpread); }
#line 646 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_inline_fragment: // inline_fragment
#line 292 "parser.ypp"
                    { delete (yysym.value.inlineFragment); }
#line 652 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_fragment_definition: // fragment_definition
#line 292 "parser.ypp"
                    { delete (yysym.value.fragmentDefinition); }
#line 658 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_type_condition: // type_condition
#line 292 "parser.ypp"
                    { delete (yysym.value.namedType); }
#line 664 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_value: // value
#line 292 "parser.ypp"
                    { delete (yysym.value.value); }
#line 670 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_int_value: // int_value
#line 292 "parser.ypp"
                    { delete (yysym.value.intValue); }
#line 676 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_float_value: // float_value
#line 292 "parser.ypp"
                    { delete (yysym.value.floatValue); }
#line 682 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_string_value: // string_value
#line 292 "parser.ypp"
                    { delete (yysym.value.stringValue); }
#line 688 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_value_const: // value_const
#line 292 "parser.ypp"
                    { delete (yysym.value.value); }
#line 694 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_boolean_value: // boolean_value
#line 292 "parser.ypp"
                    { delete (yysym.value.booleanValue); }
#line 700 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_null_value: // null_value
#line 292 "parser.ypp"
                    { delete (yysym.value.nullValue); }
#line 706 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_enum_value: // enum_value
#line 292 "parser.ypp"
                    { delete (yysym.value.enumValue); }
#line 712 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_list_value: // list_value
#line 292 "parser.ypp"
                    { delete (yysym.value.arrayValue); }
#line 718 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_value_list: // value_list
#line 292 "parser.ypp"
                    { delete (yysym.value.valueList); }
#line 724 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_list_value_const: // list_value_const
#line 292 "parser.ypp"
                    { delete (yysym.value.arrayValue); }
#line 730 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_value_const_list: // value_const_list
#line 292 "parser.ypp"
                    { delete (yysym.value.valueList); }
#line 736 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_object_value: // object_value
#line 292 "parser.ypp"
                    { delete (yysym.value.objectValue); }
#line 742 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_object_field_list: // object_field_list
#line 292 "parser.ypp"
                    { delete (yysym.value.objectFieldList); }
#line 748 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_object_field: // object_field
#line 292 "parser.ypp"
                    { delete (yysym.value.objectField); }
#line 754 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_object_value_const: // object_value_const
#line 292 "parser.ypp"
                    { delete (yysym.value.objectValue); }
#line 760 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_object_field_const_list: // object_field_const_list
#line 292 "parser.ypp"
                    { delete (yysym.value.objectFieldList); }
#line 766 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_object_field_const: // object_field_const
#line 292 "parser.ypp"
                    { delete (yysym.value.objectField); }
#line 772 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_directives: // directives
#line 292 "parser.ypp"
                    { delete (yysym.value.directiveList); }
#line 778 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_directives_opt: // directives_opt
#line 292 "parser.ypp"
                    { delete (yysym.value.directiveList); }
#line 784 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_directive_list: // directive_list
#line 292 "parser.ypp"
                    { delete (yysym.value.directiveList); }
#line 790 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_directive: // directive
#line 292 "parser.ypp"
                    { delete (yysym.value.directive); }
#line 796 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_type: // type
#line 292 "parser.ypp"
                    { delete (yysym.value.type); }
#line 802 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_type_name: // type_name
#line 292 "parser.ypp"
                    { delete (yysym.value.namedType); }
#line 808 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_list_type: // list_type
#line 292 "parser.ypp"
                    { delete (yysym.value.listType); }
#line 814 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_non_null_type: // non_null_type
#line 292 "parser.ypp"
                    { delete (yysym.value.nonNullType); }
#line 820 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_schema_definition: // schema_definition
#line 292 "parser.ypp"
                    { delete (yysym.value.schemaDefinition); }
#line 826 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_operation_type_definition_list: // operation_type_definition_list
#line 292 "parser.ypp"
                    { delete (yysym.value.operationTypeDefinitionList); }
#line 832 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_operation_type_definition: // operation_type_definition
#line 292 "parser.ypp"
                    { delete (yysym.value.operationTypeDefinition); }
#line 838 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_scalar_type_definition: // scalar_type_definition
#line 292 "parser.ypp"
                    { delete (yysym.value.scalarTypeDefinition); }
#line 844 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_object_type_definition: // object_type_definition
#line 292 "parser.ypp"
                    { delete (yysym.value.objectTypeDefinition); }
#line 850 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_implements_interfaces_opt: // implements_interfaces_opt
#line 292 "parser.ypp"
                    { delete (yysym.value.typeNameList); }
#line 856 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_type_name_list: // type_name_list
#line 292 "parser.ypp"
                    { delete (yysym.value.typeNameList); }
#line 862 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_field_definition: // field_definition
#line 292 "parser.ypp"
                    { delete (yysym.value.fieldDefinition); }
#line 868 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_field_definition_list: // field_definition_list
#line 292 "parser.ypp"
                    { delete (yysym.value.fieldDefinitionList); }
#line 874 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_arguments_definition_opt: // arguments_definition_opt
#line 292 "parser.ypp"
                    { delete (yysym.value.inputValueDefinitionList); }
#line 880 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_arguments_definition: // arguments_definition
#line 292 "parser.ypp"
                    { delete (yysym.value.inputValueDefinitionList); }
#line 886 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_input_value_definition_list: // input_value_definition_list
#line 292 "parser.ypp"
                    { delete (yysym.value.inputValueDefinitionList); }
#line 892 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_input_value_definition: // input_value_definition
#line 292 "parser.ypp"
                    { delete (yysym.value.inputValueDefinition); }
#line 898 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_interface_type_definition: // interface_type_definition
#line 292 "parser.ypp"
                    { delete (yysym.value.interfaceTypeDefinition); }
#line 904 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_union_type_definition: // union_type_definition
#line 292 "parser.ypp"
                    { delete (yysym.value.unionTypeDefinition); }
#line 910 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_union_members: // union_members
#line 292 "parser.ypp"
                    { delete (yysym.value.typeNameList); }
#line 916 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_enum_type_definition: // enum_type_definition
#line 292 "parser.ypp"
                    { delete (yysym.value.enumTypeDefinition); }
#line 922 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_enum_value_definition: // enum_value_definition
#line 292 "parser.ypp"
                    { delete (yysym.value.enumValueDefinition); }
#line 928 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_enum_value_definition_list: // enum_value_definition_list
#line 292 "parser.ypp"
                    { delete (yysym.value.enumValueDefinitionList); }
#line 934 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_input_object_type_definition: // input_object_type_definition
#line 292 "parser.ypp"
                    { delete (yysym.value.inputObjectTypeDefinition); }
#line 940 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_type_extension_definition: // type_extension_definition
#line 292 "parser.ypp"
                    { delete (yysym.value.typeExtensionDefinition); }
#line 946 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_directive_definition: // directive_definition
#line 292 "parser.ypp"
                    { delete (yysym.value.directiveDefinition); }
#line 952 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_directive_locations: // directive_locations
#line 292 "parser.ypp"
                    { delete (yysym.value.nameList); }
#line 958 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      default:
        break;
    }
  }

#if YYDEBUG
  template <typename Base>
  void
  GraphQLParserImpl::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        switch (yykind)
    {
      case symbol_kind::S_DIRECTIVE: // "directive"
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 986 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_ENUM: // "enum"
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 992 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_EXTEND: // "extend"
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 998 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_FALSE: // "false"
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1004 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_FRAGMENT: // "fragment"
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1010 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_IMPLEMENTS: // "implements"
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1016 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_INPUT: // "input"
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1022 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_INTERFACE: // "interface"
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1028 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_MUTATION: // "mutation"
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1034 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_NULL: // "null"
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1040 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_QUERY: // "query"
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1046 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_ON: // "on"
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1052 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_SCALAR: // "scalar"
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1058 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_SCHEMA: // "schema"
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1064 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_SUBSCRIPTION: // "subscription"
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1070 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_TRUE: // "true"
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1076 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_TYPE: // "type"
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1082 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_UNION: // "union"
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1088 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_VARIABLE: // VARIABLE
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1094 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_INTEGER: // INTEGER
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1100 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_FLOAT: // FLOAT
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1106 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_STRING: // STRING
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1112 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 294 "parser.ypp"
                 { yyoutput << (yysym.value.str); }
#line 1118 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
        break;

      default:
        break;
    }
        yyo << ')';
      }
  }
#endif

  void
  GraphQLParserImpl::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  GraphQLParserImpl::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  GraphQLParserImpl::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  GraphQLParserImpl::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  GraphQLParserImpl::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  GraphQLParserImpl::debug_level_type
  GraphQLParserImpl::debug_level () const
  {
    return yydebug_;
  }

  void
  GraphQLParserImpl::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  GraphQLParserImpl::state_type
  GraphQLParserImpl::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  GraphQLParserImpl::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  GraphQLParserImpl::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  GraphQLParserImpl::operator() ()
  {
    return parse ();
  }

  int
  GraphQLParserImpl::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location, scanner));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // start: document
#line 298 "parser.ypp"
                          { *outAST = (yystack_[0].value.document); }
#line 1371 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 3: // fragment_name: "directive"
#line 303 "parser.ypp"
                          { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1377 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 4: // fragment_name: "enum"
#line 304 "parser.ypp"
                     { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1383 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 5: // fragment_name: "extend"
#line 305 "parser.ypp"
                       { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1389 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 6: // fragment_name: "false"
#line 306 "parser.ypp"
                      { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1395 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 7: // fragment_name: "fragment"
#line 307 "parser.ypp"
                         { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1401 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 8: // fragment_name: IDENTIFIER
#line 308 "parser.ypp"
                           { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1407 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 9: // fragment_name: "implements"
#line 309 "parser.ypp"
                           { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1413 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 10: // fragment_name: "input"
#line 310 "parser.ypp"
                      { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1419 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 11: // fragment_name: "interface"
#line 311 "parser.ypp"
                          { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1425 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 12: // fragment_name: "mutation"
#line 312 "parser.ypp"
                         { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1431 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 13: // fragment_name: "null"
#line 313 "parser.ypp"
                     { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1437 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 14: // fragment_name: "query"
#line 314 "parser.ypp"
                      { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1443 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 15: // fragment_name: "scalar"
#line 315 "parser.ypp"
                       { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1449 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 16: // fragment_name: "schema"
#line 316 "parser.ypp"
                       { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1455 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 17: // fragment_name: "subscription"
#line 317 "parser.ypp"
                             { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1461 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 18: // fragment_name: "true"
#line 318 "parser.ypp"
                     { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1467 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 19: // fragment_name: "type"
#line 319 "parser.ypp"
                     { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1473 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 20: // fragment_name: "union"
#line 320 "parser.ypp"
                      { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1479 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 21: // name: fragment_name
#line 323 "parser.ypp"
      { (yylhs.value.name) = (yystack_[0].value.name); }
#line 1485 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 22: // name: "on"
#line 324 "parser.ypp"
                   { (yylhs.value.name) = new Name(yystack_[0].location, strdup((yystack_[0].value.str))); }
#line 1491 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 23: // name_opt: %empty
#line 328 "parser.ypp"
                       {(yylhs.value.name) = nullptr;}
#line 1497 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 24: // name_opt: name
#line 329 "parser.ypp"
                { (yylhs.value.name) = (yystack_[0].value.name); }
#line 1503 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 25: // document: definition_list
#line 334 "parser.ypp"
                                { (yylhs.value.document) = new Document(yylhs.location, (yystack_[0].value.definitionList)); }
#line 1509 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 26: // definition_list: definition
#line 337 "parser.ypp"
                           { (yylhs.value.definitionList) = new std::vector<std::unique_ptr<Definition>>(); (yylhs.value.definitionList)->emplace_back((yystack_[0].value.definition)); }
#line 1515 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 27: // definition_list: definition_list definition
#line 338 "parser.ypp"
                                           { (yystack_[1].value.definitionList)->emplace_back((yystack_[0].value.definition)); (yylhs.value.definitionList) = (yystack_[1].value.definitionList); }
#line 1521 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 28: // definition: operation_definition
#line 341 "parser.ypp"
                                     { (yylhs.value.definition) = static_cast<Definition *>((yystack_[0].value.operationDefinition)); }
#line 1527 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 29: // definition: fragment_definition
#line 342 "parser.ypp"
                                    { (yylhs.value.definition) = static_cast<Definition *>((yystack_[0].value.fragmentDefinition)); }
#line 1533 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 30: // definition: schema_gate
#line 343 "parser.ypp"
                            {
            if (!enableSchema) {
              error(yylhs.location, "schema support disabled");
              // %destructor doesn't work with YYERROR. See
              // https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
              delete (yylhs.value.definition);
              YYERROR;
            }
            (yylhs.value.definition) = static_cast<Definition *>((yystack_[0].value.definition));
          }
#line 1548 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 31: // schema_gate: schema_definition
#line 355 "parser.ypp"
                                  { (yylhs.value.definition) = static_cast<Definition *>((yystack_[0].value.schemaDefinition)); }
#line 1554 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 32: // schema_gate: scalar_type_definition
#line 356 "parser.ypp"
                                       { (yylhs.value.definition) = static_cast<Definition *>((yystack_[0].value.scalarTypeDefinition)); }
#line 1560 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 33: // schema_gate: object_type_definition
#line 357 "parser.ypp"
                                       { (yylhs.value.definition) = static_cast<Definition *>((yystack_[0].value.objectTypeDefinition)); }
#line 1566 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 34: // schema_gate: interface_type_definition
#line 358 "parser.ypp"
                                          { (yylhs.value.definition) = static_cast<Definition *>((yystack_[0].value.interfaceTypeDefinition)); }
#line 1572 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 35: // schema_gate: union_type_definition
#line 359 "parser.ypp"
                                      { (yylhs.value.definition) = static_cast<Definition *>((yystack_[0].value.unionTypeDefinition)); }
#line 1578 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 36: // schema_gate: enum_type_definition
#line 360 "parser.ypp"
                                     { (yylhs.value.definition) = static_cast<Definition *>((yystack_[0].value.enumTypeDefinition)); }
#line 1584 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 37: // schema_gate: input_object_type_definition
#line 361 "parser.ypp"
                                             { (yylhs.value.definition) = static_cast<Definition *>((yystack_[0].value.inputObjectTypeDefinition)); }
#line 1590 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 38: // schema_gate: type_extension_definition
#line 362 "parser.ypp"
                                          { (yylhs.value.definition) = static_cast<Definition *>((yystack_[0].value.typeExtensionDefinition)); }
#line 1596 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 39: // schema_gate: directive_definition
#line 363 "parser.ypp"
                                     { (yylhs.value.definition) = static_cast<Definition *>((yystack_[0].value.directiveDefinition)); }
#line 1602 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 40: // operation_definition: selection_set
#line 369 "parser.ypp"
                              { (yylhs.value.operationDefinition) = new OperationDefinition(yylhs.location, strdup("query"), nullptr, nullptr, nullptr, (yystack_[0].value.selectionSet)); }
#line 1608 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 41: // operation_definition: operation_type name_opt selection_set
#line 370 "parser.ypp"
                                                      { (yylhs.value.operationDefinition) = new OperationDefinition(yylhs.location, (yystack_[2].value.heapStr), (yystack_[1].value.name), nullptr, nullptr, (yystack_[0].value.selectionSet)); }
#line 1614 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 42: // operation_definition: operation_type name_opt variable_definitions selection_set
#line 371 "parser.ypp"
                                                                           { (yylhs.value.operationDefinition) = new OperationDefinition(yylhs.location, (yystack_[3].value.heapStr), (yystack_[2].value.name), (yystack_[1].value.variableDefinitionList), nullptr, (yystack_[0].value.selectionSet)); }
#line 1620 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 43: // operation_definition: operation_type name_opt directives selection_set
#line 372 "parser.ypp"
                                                                 { (yylhs.value.operationDefinition) = new OperationDefinition(yylhs.location, (yystack_[3].value.heapStr), (yystack_[2].value.name), nullptr, (yystack_[1].value.directiveList), (yystack_[0].value.selectionSet)); }
#line 1626 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 44: // operation_definition: operation_type name_opt variable_definitions directives selection_set
#line 373 "parser.ypp"
                                                                                      { (yylhs.value.operationDefinition) = new OperationDefinition(yylhs.location, (yystack_[4].value.heapStr), (yystack_[3].value.name), (yystack_[2].value.variableDefinitionList), (yystack_[1].value.directiveList), (yystack_[0].value.selectionSet)); }
#line 1632 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 45: // operation_type: "query"
#line 376 "parser.ypp"
                      { (yylhs.value.heapStr) = strdup((yystack_[0].value.str)); }
#line 1638 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 46: // operation_type: "mutation"
#line 377 "parser.ypp"
                         { (yylhs.value.heapStr) = strdup((yystack_[0].value.str)); }
#line 1644 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 47: // operation_type: "subscription"
#line 378 "parser.ypp"
                             { (yylhs.value.heapStr) = strdup((yystack_[0].value.str)); }
#line 1650 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 48: // variable_definitions: "(" variable_definition_list ")"
#line 382 "parser.ypp"
                                                 { (yylhs.value.variableDefinitionList) = (yystack_[1].value.variableDefinitionList); }
#line 1656 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 49: // variable_definition_list: variable_definition
#line 386 "parser.ypp"
                                    { (yylhs.value.variableDefinitionList) = new std::vector<std::unique_ptr<VariableDefinition>>(); (yylhs.value.variableDefinitionList)->emplace_back((yystack_[0].value.variableDefinition)); }
#line 1662 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 50: // variable_definition_list: variable_definition_list variable_definition
#line 387 "parser.ypp"
                                                             { (yystack_[1].value.variableDefinitionList)->emplace_back((yystack_[0].value.variableDefinition)); (yylhs.value.variableDefinitionList) = (yystack_[1].value.variableDefinitionList); }
#line 1668 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 51: // variable: VARIABLE
#line 390 "parser.ypp"
                         { (yylhs.value.variable) = new Variable(yylhs.location, new Name(yystack_[0].location, strdup((yystack_[0].value.str)))); }
#line 1674 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 52: // variable_definition: variable ":" type default_value_opt
#line 394 "parser.ypp"
                                                    { (yylhs.value.variableDefinition) = new VariableDefinition(yylhs.location, (yystack_[3].value.variable), (yystack_[1].value.type), (yystack_[0].value.value)); }
#line 1680 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 53: // default_value_opt: %empty
#line 398 "parser.ypp"
                       { (yylhs.value.value) = nullptr; }
#line 1686 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 54: // default_value_opt: default_value
#line 399 "parser.ypp"
                { (yylhs.value.value) = (yystack_[0].value.value); }
#line 1692 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 55: // default_value: "=" value_const
#line 402 "parser.ypp"
                                { (yylhs.value.value) = (yystack_[0].value.value); }
#line 1698 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 56: // selection_set: "{" selection_list "}"
#line 406 "parser.ypp"
                                         { (yylhs.value.selectionSet) = new SelectionSet(yylhs.location, (yystack_[1].value.selectionList)); }
#line 1704 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 57: // selection_set_opt: %empty
#line 410 "parser.ypp"
                       { (yylhs.value.selectionSet) = nullptr; }
#line 1710 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 58: // selection_set_opt: selection_set
#line 411 "parser.ypp"
                { (yylhs.value.selectionSet) = (yystack_[0].value.selectionSet); }
#line 1716 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 59: // selection_list: selection
#line 413 "parser.ypp"
                          { (yylhs.value.selectionList) = new std::vector<std::unique_ptr<Selection>>(); (yylhs.value.selectionList)->emplace_back((yystack_[0].value.selection)); }
#line 1722 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 60: // selection_list: selection_list selection
#line 414 "parser.ypp"
                                         { (yystack_[1].value.selectionList)->emplace_back((yystack_[0].value.selection)); (yylhs.value.selectionList) = (yystack_[1].value.selectionList); }
#line 1728 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 61: // selection: field
#line 417 "parser.ypp"
                      { (yylhs.value.selection) = static_cast<Selection *>((yystack_[0].value.field)); }
#line 1734 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 62: // selection: fragment_spread
#line 418 "parser.ypp"
                                { (yylhs.value.selection) = static_cast<Selection *>((yystack_[0].value.fragmentSpread)); }
#line 1740 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 63: // selection: inline_fragment
#line 419 "parser.ypp"
                                { (yylhs.value.selection) = static_cast<Selection *>((yystack_[0].value.inlineFragment)); }
#line 1746 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 64: // field: name arguments_opt directives_opt selection_set_opt
#line 422 "parser.ypp"
                                                                    { (yylhs.value.field) = new Field(yylhs.location, nullptr, (yystack_[3].value.name), (yystack_[2].value.argumentList), (yystack_[1].value.directiveList), (yystack_[0].value.selectionSet)); }
#line 1752 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 65: // field: name ":" name arguments_opt directives_opt selection_set_opt
#line 423 "parser.ypp"
                                                                             { (yylhs.value.field) = new Field(yylhs.location, (yystack_[5].value.name), (yystack_[3].value.name), (yystack_[2].value.argumentList), (yystack_[1].value.directiveList), (yystack_[0].value.selectionSet)); }
#line 1758 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 66: // arguments: "(" argument_list ")"
#line 426 "parser.ypp"
                                      { (yylhs.value.argumentList) = (yystack_[1].value.argumentList); }
#line 1764 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 67: // arguments_opt: %empty
#line 429 "parser.ypp"
                       { (yylhs.value.argumentList) = nullptr; }
#line 1770 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 68: // arguments_opt: arguments
#line 430 "parser.ypp"
                          { (yylhs.value.argumentList) = (yystack_[0].value.argumentList); }
#line 1776 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 69: // argument_list: argument
#line 433 "parser.ypp"
                         { (yylhs.value.argumentList) = new std::vector<std::unique_ptr<Argument>>(); (yylhs.value.argumentList)->emplace_back((yystack_[0].value.argument)); }
#line 1782 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 70: // argument_list: argument_list argument
#line 434 "parser.ypp"
                                       { (yystack_[1].value.argumentList)->emplace_back((yystack_[0].value.argument)); (yylhs.value.argumentList) = (yystack_[1].value.argumentList); }
#line 1788 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 71: // argument: name ":" value
#line 437 "parser.ypp"
                               { (yylhs.value.argument) = new Argument(yylhs.location, (yystack_[2].value.name), (yystack_[0].value.value)); }
#line 1794 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 72: // fragment_spread: "..." fragment_name directives_opt
#line 442 "parser.ypp"
                                                   { (yylhs.value.fragmentSpread) = new FragmentSpread(yylhs.location, (yystack_[1].value.name), (yystack_[0].value.directiveList)); }
#line 1800 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 73: // inline_fragment: "..." "on" type_condition directives_opt selection_set
#line 446 "parser.ypp"
                                                                       { (yylhs.value.inlineFragment) = new InlineFragment(yylhs.location, (yystack_[2].value.namedType), (yystack_[1].value.directiveList), (yystack_[0].value.selectionSet)); }
#line 1806 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 74: // inline_fragment: "..." directives_opt selection_set
#line 447 "parser.ypp"
                                                   { (yylhs.value.inlineFragment) = new InlineFragment(yylhs.location, nullptr, (yystack_[1].value.directiveList), (yystack_[0].value.selectionSet)); }
#line 1812 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 75: // fragment_definition: "fragment" fragment_name "on" type_condition directives_opt selection_set
#line 451 "parser.ypp"
                                                                                          { (yylhs.value.fragmentDefinition) = new FragmentDefinition(yylhs.location, (yystack_[4].value.name), (yystack_[2].value.namedType), (yystack_[1].value.directiveList), (yystack_[0].value.selectionSet)); }
#line 1818 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 76: // type_condition: type_name
#line 454 "parser.ypp"
                { (yylhs.value.namedType) = (yystack_[0].value.namedType); }
#line 1824 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 77: // value: variable
#line 458 "parser.ypp"
                         { (yylhs.value.value) = static_cast<Value *>((yystack_[0].value.variable)); }
#line 1830 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 78: // value: int_value
#line 459 "parser.ypp"
                          { (yylhs.value.value) = static_cast<Value *>((yystack_[0].value.intValue)); }
#line 1836 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 79: // value: float_value
#line 460 "parser.ypp"
                            { (yylhs.value.value) = static_cast<Value *>((yystack_[0].value.floatValue)); }
#line 1842 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 80: // value: string_value
#line 461 "parser.ypp"
                             { (yylhs.value.value) = static_cast<Value *>((yystack_[0].value.stringValue)); }
#line 1848 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 81: // value: boolean_value
#line 462 "parser.ypp"
                              { (yylhs.value.value) = static_cast<Value *>((yystack_[0].value.booleanValue)); }
#line 1854 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 82: // value: null_value
#line 463 "parser.ypp"
                           { (yylhs.value.value) = static_cast<Value *>((yystack_[0].value.nullValue)); }
#line 1860 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 83: // value: enum_value
#line 464 "parser.ypp"
                           { (yylhs.value.value) = static_cast<Value *>((yystack_[0].value.enumValue)); }
#line 1866 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 84: // value: list_value
#line 465 "parser.ypp"
                           { (yylhs.value.value) = static_cast<Value *>((yystack_[0].value.arrayValue)); }
#line 1872 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 85: // value: object_value
#line 466 "parser.ypp"
                             { (yylhs.value.value) = static_cast<Value *>((yystack_[0].value.objectValue)); }
#line 1878 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 86: // int_value: INTEGER
#line 469 "parser.ypp"
                        { (yylhs.value.intValue) = new IntValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 1884 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 87: // float_value: FLOAT
#line 472 "parser.ypp"
                      { (yylhs.value.floatValue) = new FloatValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 1890 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 88: // string_value: STRING
#line 475 "parser.ypp"
                       { (yylhs.value.stringValue) = new StringValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 1896 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 89: // value_const: int_value
#line 478 "parser.ypp"
                          { (yylhs.value.value) = static_cast<Value *>((yystack_[0].value.intValue)); }
#line 1902 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 90: // value_const: float_value
#line 479 "parser.ypp"
                            { (yylhs.value.value) = static_cast<Value *>((yystack_[0].value.floatValue)); }
#line 1908 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 91: // value_const: string_value
#line 480 "parser.ypp"
                             { (yylhs.value.value) = static_cast<Value *>((yystack_[0].value.stringValue)); }
#line 1914 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 92: // value_const: boolean_value
#line 481 "parser.ypp"
                              { (yylhs.value.value) = static_cast<Value *>((yystack_[0].value.booleanValue)); }
#line 1920 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 93: // value_const: null_value
#line 482 "parser.ypp"
                           { (yylhs.value.value) = static_cast<Value *>((yystack_[0].value.nullValue)); }
#line 1926 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 94: // value_const: enum_value
#line 483 "parser.ypp"
                           { (yylhs.value.value) = static_cast<Value *>((yystack_[0].value.enumValue)); }
#line 1932 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 95: // value_const: list_value_const
#line 484 "parser.ypp"
                                 { (yylhs.value.value) = static_cast<Value *>((yystack_[0].value.arrayValue)); }
#line 1938 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 96: // value_const: object_value_const
#line 485 "parser.ypp"
                                   { (yylhs.value.value) = static_cast<Value *>((yystack_[0].value.objectValue)); }
#line 1944 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 97: // boolean_value: "true"
#line 488 "parser.ypp"
                     { (yylhs.value.booleanValue) = new BooleanValue(yylhs.location, true); }
#line 1950 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 98: // boolean_value: "false"
#line 489 "parser.ypp"
                      { (yylhs.value.booleanValue) = new BooleanValue(yylhs.location, false); }
#line 1956 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 99: // null_value: "null"
#line 492 "parser.ypp"
                     { (yylhs.value.nullValue) = new NullValue(yylhs.location); }
#line 1962 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 100: // enum_value: "directive"
#line 495 "parser.ypp"
                          { (yylhs.value.enumValue) = new EnumValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 1968 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 101: // enum_value: "enum"
#line 496 "parser.ypp"
                     { (yylhs.value.enumValue) = new EnumValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 1974 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 102: // enum_value: "extend"
#line 497 "parser.ypp"
                       { (yylhs.value.enumValue) = new EnumValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 1980 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 103: // enum_value: "fragment"
#line 498 "parser.ypp"
                         { (yylhs.value.enumValue) = new EnumValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 1986 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 104: // enum_value: IDENTIFIER
#line 499 "parser.ypp"
                           { (yylhs.value.enumValue) = new EnumValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 1992 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 105: // enum_value: "implements"
#line 500 "parser.ypp"
                           { (yylhs.value.enumValue) = new EnumValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 1998 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 106: // enum_value: "input"
#line 501 "parser.ypp"
                      { (yylhs.value.enumValue) = new EnumValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 2004 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 107: // enum_value: "interface"
#line 502 "parser.ypp"
                          { (yylhs.value.enumValue) = new EnumValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 2010 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 108: // enum_value: "mutation"
#line 503 "parser.ypp"
                         { (yylhs.value.enumValue) = new EnumValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 2016 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 109: // enum_value: "on"
#line 504 "parser.ypp"
                   { (yylhs.value.enumValue) = new EnumValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 2022 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 110: // enum_value: "query"
#line 505 "parser.ypp"
                      { (yylhs.value.enumValue) = new EnumValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 2028 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 111: // enum_value: "scalar"
#line 506 "parser.ypp"
                       { (yylhs.value.enumValue) = new EnumValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 2034 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 112: // enum_value: "schema"
#line 507 "parser.ypp"
                       { (yylhs.value.enumValue) = new EnumValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 2040 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 113: // enum_value: "subscription"
#line 508 "parser.ypp"
                             { (yylhs.value.enumValue) = new EnumValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 2046 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 114: // enum_value: "type"
#line 509 "parser.ypp"
                     { (yylhs.value.enumValue) = new EnumValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 2052 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 115: // enum_value: "union"
#line 510 "parser.ypp"
                      { (yylhs.value.enumValue) = new EnumValue(yylhs.location, strdup((yystack_[0].value.str))); }
#line 2058 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 116: // list_value: "[" "]"
#line 517 "parser.ypp"
                       { (yylhs.value.arrayValue) = new ListValue(yylhs.location, new std::vector<std::unique_ptr<Value>>()); }
#line 2064 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 117: // list_value: "[" value_list "]"
#line 518 "parser.ypp"
                                   { (yylhs.value.arrayValue) = new ListValue(yylhs.location, (yystack_[1].value.valueList)); }
#line 2070 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 118: // value_list: value
#line 521 "parser.ypp"
                      { (yylhs.value.valueList) = new std::vector<std::unique_ptr<Value>>(); (yylhs.value.valueList)->emplace_back((yystack_[0].value.value)); }
#line 2076 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 119: // value_list: value_list value
#line 522 "parser.ypp"
                                 { (yystack_[1].value.valueList)->emplace_back((yystack_[0].value.value)); (yylhs.value.valueList) = (yystack_[1].value.valueList); }
#line 2082 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 120: // list_value_const: "[" "]"
#line 526 "parser.ypp"
                        { (yylhs.value.arrayValue) = new ListValue(yylhs.location, new std::vector<std::unique_ptr<Value>>()); }
#line 2088 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 121: // list_value_const: "[" value_const_list "]"
#line 527 "parser.ypp"
                                         { (yylhs.value.arrayValue) = new ListValue(yylhs.location, (yystack_[1].value.valueList)); }
#line 2094 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 122: // value_const_list: value_const
#line 531 "parser.ypp"
                            { (yylhs.value.valueList) = new std::vector<std::unique_ptr<Value>>(); (yylhs.value.valueList)->emplace_back((yystack_[0].value.value)); }
#line 2100 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 123: // value_const_list: value_const_list value_const
#line 532 "parser.ypp"
                                             { (yystack_[1].value.valueList)->emplace_back((yystack_[0].value.value)); (yylhs.value.valueList) = (yystack_[1].value.valueList); }
#line 2106 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 124: // object_value: "{" "}"
#line 537 "parser.ypp"
                        { (yylhs.value.objectValue) = new ObjectValue(yylhs.location, new std::vector<std::unique_ptr<ObjectField>>()); }
#line 2112 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 125: // object_value: "{" object_field_list "}"
#line 538 "parser.ypp"
                                          { (yylhs.value.objectValue) = new ObjectValue(yylhs.location, (yystack_[1].value.objectFieldList)); }
#line 2118 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 126: // object_field_list: object_field
#line 542 "parser.ypp"
                             { (yylhs.value.objectFieldList) = new std::vector<std::unique_ptr<ObjectField>>(); (yylhs.value.objectFieldList)->emplace_back((yystack_[0].value.objectField)); }
#line 2124 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 127: // object_field_list: object_field_list object_field
#line 543 "parser.ypp"
                                               { (yystack_[1].value.objectFieldList)->emplace_back((yystack_[0].value.objectField)); (yylhs.value.objectFieldList) = (yystack_[1].value.objectFieldList); }
#line 2130 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 128: // object_field: name ":" value
#line 546 "parser.ypp"
                               { (yylhs.value.objectField) = new ObjectField(yylhs.location, (yystack_[2].value.name), (yystack_[0].value.value)); }
#line 2136 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 129: // object_value_const: "{" "}"
#line 550 "parser.ypp"
                        { (yylhs.value.objectValue) = new ObjectValue(yylhs.location, new std::vector<std::unique_ptr<ObjectField>>()); }
#line 2142 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 130: // object_value_const: "{" object_field_const_list "}"
#line 551 "parser.ypp"
                                                { (yylhs.value.objectValue) = new ObjectValue(yylhs.location, (yystack_[1].value.objectFieldList)); }
#line 2148 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 131: // object_field_const_list: object_field_const
#line 555 "parser.ypp"
                                   { (yylhs.value.objectFieldList) = new std::vector<std::unique_ptr<ObjectField>>(); (yylhs.value.objectFieldList)->emplace_back((yystack_[0].value.objectField)); }
#line 2154 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 132: // object_field_const_list: object_field_const_list object_field_const
#line 556 "parser.ypp"
                                                           { (yystack_[1].value.objectFieldList)->emplace_back((yystack_[0].value.objectField)); (yylhs.value.objectFieldList) = (yystack_[1].value.objectFieldList); }
#line 2160 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 133: // object_field_const: name ":" value_const
#line 559 "parser.ypp"
                                         { (yylhs.value.objectField) = new ObjectField(yylhs.location, (yystack_[2].value.name), (yystack_[0].value.value)); }
#line 2166 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 134: // directives: directive_list
#line 564 "parser.ypp"
                { (yylhs.value.directiveList) = (yystack_[0].value.directiveList); }
#line 2172 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 135: // directives_opt: %empty
#line 567 "parser.ypp"
                       { (yylhs.value.directiveList) = nullptr; }
#line 2178 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 136: // directives_opt: directives
#line 568 "parser.ypp"
                { (yylhs.value.directiveList) = (yystack_[0].value.directiveList); }
#line 2184 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 137: // directive_list: directive
#line 571 "parser.ypp"
                          { (yylhs.value.directiveList) = new std::vector<std::unique_ptr<Directive>>(); (yylhs.value.directiveList)->emplace_back((yystack_[0].value.directive)); }
#line 2190 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 138: // directive_list: directive_list directive
#line 572 "parser.ypp"
                                         { (yystack_[1].value.directiveList)->emplace_back((yystack_[0].value.directive)); (yylhs.value.directiveList) = (yystack_[1].value.directiveList); }
#line 2196 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 139: // directive: "@" name arguments_opt
#line 575 "parser.ypp"
                                       { (yylhs.value.directive) = new Directive(yylhs.location, (yystack_[1].value.name), (yystack_[0].value.argumentList)); }
#line 2202 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 140: // type: type_name
#line 580 "parser.ypp"
                          { (yylhs.value.type) = static_cast<Type *>((yystack_[0].value.namedType)); }
#line 2208 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 141: // type: list_type
#line 581 "parser.ypp"
                          { (yylhs.value.type) = static_cast<Type *>((yystack_[0].value.listType)); }
#line 2214 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 142: // type: non_null_type
#line 582 "parser.ypp"
                              { (yylhs.value.type) = static_cast<Type *>((yystack_[0].value.nonNullType)); }
#line 2220 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 143: // type_name: name
#line 585 "parser.ypp"
                     { (yylhs.value.namedType) = new NamedType(yylhs.location, (yystack_[0].value.name)); }
#line 2226 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 144: // list_type: "[" type "]"
#line 588 "parser.ypp"
                             { (yylhs.value.listType) = new ListType(yylhs.location, (yystack_[1].value.type)); }
#line 2232 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 145: // non_null_type: type_name "!"
#line 591 "parser.ypp"
                              { (yylhs.value.nonNullType) = new NonNullType(yylhs.location, (yystack_[1].value.namedType)); }
#line 2238 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 146: // non_null_type: list_type "!"
#line 592 "parser.ypp"
                              { (yylhs.value.nonNullType) = new NonNullType(yylhs.location, (yystack_[1].value.listType)); }
#line 2244 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 147: // schema_definition: "schema" directives_opt "{" operation_type_definition_list "}"
#line 597 "parser.ypp"
                                                                                { (yylhs.value.schemaDefinition) = new SchemaDefinition(yylhs.location, (yystack_[3].value.directiveList), (yystack_[1].value.operationTypeDefinitionList)); }
#line 2250 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 148: // operation_type_definition_list: operation_type_definition
#line 601 "parser.ypp"
                                          { (yylhs.value.operationTypeDefinitionList) = new std::vector<std::unique_ptr<OperationTypeDefinition>>(); (yylhs.value.operationTypeDefinitionList)->emplace_back((yystack_[0].value.operationTypeDefinition)); }
#line 2256 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 149: // operation_type_definition_list: operation_type_definition_list operation_type_definition
#line 602 "parser.ypp"
                                                                         { (yystack_[1].value.operationTypeDefinitionList)->emplace_back((yystack_[0].value.operationTypeDefinition)); (yylhs.value.operationTypeDefinitionList) = (yystack_[1].value.operationTypeDefinitionList); }
#line 2262 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 150: // operation_type_definition: operation_type ":" type_name
#line 606 "parser.ypp"
                                             { (yylhs.value.operationTypeDefinition) = new OperationTypeDefinition(yylhs.location, (yystack_[2].value.heapStr), (yystack_[0].value.namedType)); }
#line 2268 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 151: // scalar_type_definition: "scalar" name directives_opt
#line 609 "parser.ypp"
                                                   { (yylhs.value.scalarTypeDefinition) = new ScalarTypeDefinition(yylhs.location, (yystack_[1].value.name), (yystack_[0].value.directiveList)); }
#line 2274 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 152: // object_type_definition: "type" name implements_interfaces_opt directives_opt "{" field_definition_list "}"
#line 612 "parser.ypp"
                                                                                                         { (yylhs.value.objectTypeDefinition) = new ObjectTypeDefinition(yylhs.location, (yystack_[5].value.name), (yystack_[4].value.typeNameList), (yystack_[3].value.directiveList), (yystack_[1].value.fieldDefinitionList)); }
#line 2280 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 153: // implements_interfaces_opt: %empty
#line 615 "parser.ypp"
                                  { (yylhs.value.typeNameList) = nullptr; }
#line 2286 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 154: // implements_interfaces_opt: "implements" type_name_list
#line 616 "parser.ypp"
                                          { (yylhs.value.typeNameList) = (yystack_[0].value.typeNameList); }
#line 2292 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 155: // type_name_list: type_name
#line 619 "parser.ypp"
                          { (yylhs.value.typeNameList) = new std::vector<std::unique_ptr<NamedType>>(); (yylhs.value.typeNameList)->emplace_back((yystack_[0].value.namedType)); }
#line 2298 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 156: // type_name_list: type_name_list type_name
#line 620 "parser.ypp"
                                         { (yystack_[1].value.typeNameList)->emplace_back((yystack_[0].value.namedType)); (yylhs.value.typeNameList) = (yystack_[1].value.typeNameList); }
#line 2304 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 157: // field_definition: name arguments_definition_opt ":" type directives_opt
#line 623 "parser.ypp"
                                                                        { (yylhs.value.fieldDefinition) = new FieldDefinition(yylhs.location, (yystack_[4].value.name), (yystack_[3].value.inputValueDefinitionList), (yystack_[1].value.type), (yystack_[0].value.directiveList)); }
#line 2310 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 158: // field_definition_list: field_definition
#line 627 "parser.ypp"
                                         { (yylhs.value.fieldDefinitionList) = new std::vector<std::unique_ptr<FieldDefinition>>(); (yylhs.value.fieldDefinitionList)->emplace_back((yystack_[0].value.fieldDefinition)); }
#line 2316 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 159: // field_definition_list: field_definition_list field_definition
#line 628 "parser.ypp"
                                                       { (yystack_[1].value.fieldDefinitionList)->emplace_back((yystack_[0].value.fieldDefinition)); (yylhs.value.fieldDefinitionList) = (yystack_[1].value.fieldDefinitionList); }
#line 2322 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 160: // arguments_definition_opt: %empty
#line 631 "parser.ypp"
                                 { (yylhs.value.inputValueDefinitionList) = nullptr; }
#line 2328 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 161: // arguments_definition_opt: arguments_definition
#line 632 "parser.ypp"
                                     { (yylhs.value.inputValueDefinitionList) = (yystack_[0].value.inputValueDefinitionList); }
#line 2334 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 162: // arguments_definition: "(" input_value_definition_list ")"
#line 635 "parser.ypp"
                                                          { (yylhs.value.inputValueDefinitionList) = (yystack_[1].value.inputValueDefinitionList); }
#line 2340 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 163: // input_value_definition_list: input_value_definition
#line 638 "parser.ypp"
                                                    { (yylhs.value.inputValueDefinitionList) = new std::vector<std::unique_ptr<InputValueDefinition>>(); (yylhs.value.inputValueDefinitionList)->emplace_back((yystack_[0].value.inputValueDefinition)); }
#line 2346 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 164: // input_value_definition_list: input_value_definition_list input_value_definition
#line 639 "parser.ypp"
                                                                   { (yystack_[1].value.inputValueDefinitionList)->emplace_back((yystack_[0].value.inputValueDefinition)); (yylhs.value.inputValueDefinitionList) = (yystack_[1].value.inputValueDefinitionList); }
#line 2352 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 165: // input_value_definition: name ":" type default_value_opt directives_opt
#line 642 "parser.ypp"
                                                                       { (yylhs.value.inputValueDefinition) = new InputValueDefinition(yylhs.location, (yystack_[4].value.name), (yystack_[2].value.type), (yystack_[1].value.value), (yystack_[0].value.directiveList)); }
#line 2358 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 166: // interface_type_definition: "interface" name directives_opt "{" field_definition_list "}"
#line 644 "parser.ypp"
                                                                                       { (yylhs.value.interfaceTypeDefinition) = new InterfaceTypeDefinition(yylhs.location, (yystack_[4].value.name), (yystack_[3].value.directiveList), (yystack_[1].value.fieldDefinitionList)); }
#line 2364 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 167: // union_type_definition: "union" name directives_opt "=" union_members
#line 647 "parser.ypp"
                                                                   { (yylhs.value.unionTypeDefinition) = new UnionTypeDefinition(yylhs.location, (yystack_[3].value.name), (yystack_[2].value.directiveList), (yystack_[0].value.typeNameList)); }
#line 2370 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 168: // union_members: type_name
#line 650 "parser.ypp"
                          { (yylhs.value.typeNameList) = new std::vector<std::unique_ptr<NamedType>>();  (yylhs.value.typeNameList)->emplace_back((yystack_[0].value.namedType)); }
#line 2376 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 169: // union_members: union_members "|" type_name
#line 651 "parser.ypp"
                                            { (yystack_[2].value.typeNameList)->emplace_back((yystack_[0].value.namedType)); (yylhs.value.typeNameList) = (yystack_[2].value.typeNameList); }
#line 2382 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 170: // enum_type_definition: "enum" name directives_opt "{" enum_value_definition_list "}"
#line 654 "parser.ypp"
                                                                                  { (yylhs.value.enumTypeDefinition) = new EnumTypeDefinition(yylhs.location, (yystack_[4].value.name), (yystack_[3].value.directiveList), (yystack_[1].value.enumValueDefinitionList)); }
#line 2388 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 171: // enum_value_definition: name directives_opt
#line 657 "parser.ypp"
                                           { (yylhs.value.enumValueDefinition) = new EnumValueDefinition(yylhs.location, (yystack_[1].value.name), (yystack_[0].value.directiveList)); }
#line 2394 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 172: // enum_value_definition_list: enum_value_definition
#line 661 "parser.ypp"
                                      { (yylhs.value.enumValueDefinitionList) = new std::vector<std::unique_ptr<EnumValueDefinition>>(); (yylhs.value.enumValueDefinitionList)->emplace_back((yystack_[0].value.enumValueDefinition)); }
#line 2400 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 173: // enum_value_definition_list: enum_value_definition_list enum_value_definition
#line 662 "parser.ypp"
                                                                 { (yystack_[1].value.enumValueDefinitionList)->emplace_back((yystack_[0].value.enumValueDefinition)); (yylhs.value.enumValueDefinitionList) = (yystack_[1].value.enumValueDefinitionList); }
#line 2406 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 174: // input_object_type_definition: "input" name directives_opt "{" input_value_definition_list "}"
#line 665 "parser.ypp"
                                                                                            { (yylhs.value.inputObjectTypeDefinition) = new InputObjectTypeDefinition(yylhs.location, (yystack_[4].value.name), (yystack_[3].value.directiveList), (yystack_[1].value.inputValueDefinitionList)); }
#line 2412 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 175: // type_extension_definition: "extend" object_type_definition
#line 668 "parser.ypp"
                                                         { (yylhs.value.typeExtensionDefinition) = new TypeExtensionDefinition(yylhs.location, (yystack_[0].value.objectTypeDefinition)); }
#line 2418 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 176: // directive_definition: "directive" "@" name arguments_definition_opt "on" directive_locations
#line 671 "parser.ypp"
                                                                                         { (yylhs.value.directiveDefinition) = new DirectiveDefinition(yylhs.location, (yystack_[3].value.name), (yystack_[2].value.inputValueDefinitionList), (yystack_[0].value.nameList)); }
#line 2424 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 177: // directive_locations: name
#line 675 "parser.ypp"
                     { (yylhs.value.nameList) = new std::vector<std::unique_ptr<Name>>(); (yylhs.value.nameList)->emplace_back((yystack_[0].value.name)); }
#line 2430 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;

  case 178: // directive_locations: directive_locations "|" name
#line 676 "parser.ypp"
                                             { (yystack_[2].value.nameList)->emplace_back((yystack_[0].value.name)); (yylhs.value.nameList) = (yystack_[2].value.nameList); }
#line 2436 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"
    break;


#line 2440 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  GraphQLParserImpl::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  GraphQLParserImpl::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  GraphQLParserImpl::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // GraphQLParserImpl::context.
  GraphQLParserImpl::context::context (const GraphQLParserImpl& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  GraphQLParserImpl::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  GraphQLParserImpl::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  GraphQLParserImpl::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short GraphQLParserImpl::yypact_ninf_ = -228;

  const signed char GraphQLParserImpl::yytable_ninf_ = -1;

  const short
  GraphQLParserImpl::yypact_[] =
  {
     198,   -11,   895,     6,   918,   895,   895,  -228,  -228,   895,
      13,  -228,   895,   895,   440,    50,  -228,   198,  -228,  -228,
    -228,   895,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,
    -228,  -228,  -228,   895,  -228,  -228,  -228,  -228,  -228,  -228,
    -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,
    -228,  -228,  -228,  -228,    13,  -228,    58,    13,    13,    13,
     895,  -228,    44,    13,  -228,    79,    13,   475,    74,   405,
    -228,  -228,  -228,  -228,  -228,  -228,  -228,    76,    54,    52,
     895,    65,    67,  -228,    94,   100,  -228,   895,    13,    84,
     895,    13,    89,   895,   895,  -228,    13,  -228,  -228,    87,
      88,  -228,    89,   895,   109,  -228,   895,  -228,    13,  -228,
     895,   895,  -228,    99,    68,  -228,  -228,   895,    95,   895,
      13,  -228,  -228,   101,   510,  -228,    94,    89,  -228,    -9,
     103,  -228,  -228,    89,  -228,   104,   545,  -228,   895,    13,
    -228,   580,    89,   615,    54,  -228,   650,   895,  -228,  -228,
    -228,   895,  -228,   102,    89,   265,  -228,  -228,    13,  -228,
    -228,  -228,  -228,   685,  -228,   685,  -228,  -228,  -228,   106,
    -228,  -228,  -228,  -228,  -228,   105,  -228,  -228,  -228,   720,
     895,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,
    -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,
     150,   755,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,
    -228,  -228,  -228,  -228,  -228,  -228,    89,   685,   112,   110,
     111,  -228,   112,   895,   685,  -228,  -228,  -228,  -228,   228,
    -228,   115,   790,  -228,  -228,   113,   370,  -228,  -228,  -228,
    -228,    13,  -228,    13,  -228,  -228,   265,  -228,  -228,  -228,
     300,   825,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,
    -228,  -228,  -228,  -228,  -228,  -228,   335,  -228,   118,   860,
    -228,  -228,  -228,   370,  -228,  -228,  -228
  };

  const unsigned char
  GraphQLParserImpl::yydefact_[] =
  {
       0,     0,     0,     0,     0,     0,     0,    46,    45,     0,
     135,    47,     0,     0,     0,     0,     2,    25,    26,    30,
      28,    23,    40,    29,    31,    32,    33,    34,    35,    36,
      37,    38,    39,     0,     3,     4,     5,     6,     7,     9,
      10,    11,    12,    13,    14,    22,    15,    16,    17,    18,
      19,    20,     8,    21,   135,   175,     0,   135,   135,   135,
       0,   136,     0,   134,   137,   153,   135,   135,    67,     0,
      59,    61,    62,    63,     1,    27,    24,     0,   160,     0,
       0,     0,     0,   151,    67,     0,   138,     0,   135,     0,
       0,   135,     0,     0,     0,    68,   135,    56,    60,     0,
       0,    41,     0,     0,     0,   161,     0,   143,   135,    76,
       0,     0,   139,     0,     0,   148,   155,   154,     0,     0,
     135,    72,    74,     0,     0,    69,    67,    57,    51,     0,
       0,    49,    42,     0,    43,     0,     0,   163,     0,   135,
     172,     0,     0,     0,   160,   158,     0,     0,   147,   149,
     156,     0,   168,   167,     0,     0,    66,    70,   135,    58,
      64,    48,    50,     0,    44,     0,   162,   164,   177,   176,
     171,   170,   173,    75,   174,     0,   166,   159,   150,     0,
       0,    73,   100,   101,   102,    98,   103,   105,   106,   107,
     108,    99,   110,   109,   111,   112,   113,    97,   114,   115,
       0,     0,    86,    87,    88,   104,    77,    71,    78,    79,
      80,    81,    82,    83,    84,    85,    57,     0,    53,   140,
     141,   142,    53,     0,     0,   152,   169,   116,   118,     0,
     124,     0,     0,   126,    65,     0,     0,    52,    54,   145,
     146,   135,   178,   135,   117,   119,     0,   125,   127,   144,
       0,     0,    89,    90,    91,    55,    92,    93,    94,    95,
      96,   165,   157,   128,   120,   122,     0,   129,     0,     0,
     131,   121,   123,     0,   130,   132,   133
  };

  const short
  GraphQLParserImpl::yypgoto_[] =
  {
    -228,  -228,     1,    -2,  -228,  -228,  -228,   128,  -228,  -228,
     -52,  -228,  -228,   -84,    17,   -87,  -228,   -64,   -69,  -228,
      81,  -228,  -228,   -82,  -228,    24,  -228,  -228,  -228,    61,
    -194,  -227,  -218,  -193,  -224,  -189,  -180,  -161,  -228,  -228,
    -228,  -228,  -228,  -228,   -61,  -228,  -228,   -97,   -12,   -37,
    -228,   119,  -164,   -53,  -228,  -228,  -228,  -228,    59,  -228,
     171,  -228,  -228,  -138,    25,    31,  -228,    71,   -29,  -228,
    -228,  -228,  -228,    47,  -228,  -228,  -228,  -228,  -228
  };

  const short
  GraphQLParserImpl::yydefgoto_[] =
  {
       0,    15,    53,   107,    77,    16,    17,    18,    19,    20,
      21,   100,   129,   206,   131,   237,   238,    22,   160,    69,
      70,    71,    95,    96,   124,   125,    72,    73,    23,   108,
     207,   208,   209,   210,   255,   211,   212,   213,   214,   229,
     259,   266,   215,   232,   233,   260,   269,   270,    61,    62,
      63,    64,   218,   219,   220,   221,    24,   114,   115,    25,
      26,    88,   117,   145,   146,   104,   105,   136,   137,    27,
      28,   153,    29,   140,   141,    30,    31,    32,   169
  };

  const short
  GraphQLParserImpl::yytable_[] =
  {
      54,   222,   112,    57,    58,    56,   228,    59,   177,   252,
      65,    66,    68,   101,   161,   130,    33,    79,   253,    76,
      81,    82,    83,   252,   128,    12,   265,   109,   122,    89,
      92,    78,   253,   113,   116,   245,   132,   109,   134,   252,
      60,   177,   272,   254,   158,   130,   252,   256,   253,   276,
      74,   118,   263,   235,   121,   253,   257,   254,    84,   127,
     243,   256,   113,   159,   150,   102,   152,    68,    91,   164,
     257,   142,    80,   254,    85,   258,   103,   256,   173,     7,
     254,     8,   106,   154,   256,    11,   257,    87,   133,   258,
     181,   123,   126,   257,   178,   110,    93,   111,    99,    94,
     148,   135,   170,    60,   139,   258,    14,   167,   135,   144,
     119,     7,   258,     8,   167,    60,    93,    11,    14,    14,
     128,   216,   123,   138,   147,   151,   155,   226,   163,   165,
     224,   239,   240,   180,   135,   241,   168,   223,   236,   139,
     246,   135,   249,   273,   144,    75,   162,   234,   157,   144,
      98,   120,   159,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   248,   275,   149,    55,   175,   179,   144,   200,   227,
     201,   143,    86,   128,   202,   203,   204,   205,   172,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   231,
       0,     1,     2,     3,   261,     4,   262,     5,     6,     7,
       0,     8,     0,     9,    10,    11,     0,    12,    13,     0,
       0,   242,     0,     0,     0,     0,     0,     0,    14,     0,
     231,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   268,
       0,     0,     0,     0,     0,     0,   200,   244,   201,     0,
       0,   128,   202,   203,   204,   205,     0,   268,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,     0,     0,     0,     0,
       0,     0,     0,   200,     0,   201,     0,     0,   128,   202,
     203,   204,   205,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,     0,     0,     0,     0,     0,     0,     0,   250,   264,
     251,     0,     0,     0,   202,   203,   204,   205,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,     0,     0,     0,     0,
       0,     0,     0,   250,   271,   251,     0,     0,     0,   202,
     203,   204,   205,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,     0,     0,     0,     0,     0,     0,     0,   250,     0,
     251,     0,     0,     0,   202,   203,   204,   205,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,    97,     0,     0,
       0,     0,    52,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    90,
      46,    47,    48,    49,    50,    51,     0,     0,     0,     0,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,     0,     0,   156,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,     0,     0,   166,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   171,     0,     0,     0,     0,    52,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   174,     0,     0,
       0,     0,    52,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   176,     0,     0,     0,     0,    52,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,     0,     0,     0,     0,
       0,     0,     0,   217,     0,     0,     0,     0,     0,     0,
       0,     0,    52,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   225,     0,     0,     0,     0,    52,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   230,     0,     0,
       0,     0,    52,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   247,     0,     0,     0,     0,    52,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   267,     0,     0,
       0,     0,    52,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   274,     0,     0,     0,     0,    52,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,     0,     0,     0,     0,
       0,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    52,    46,    47,    48,    49,    50,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52
  };

  const short
  GraphQLParserImpl::yycheck_[] =
  {
       2,   165,    84,     5,     6,     4,   200,     9,   146,   236,
      12,    13,    14,    77,    23,    99,    27,    54,   236,    21,
      57,    58,    59,   250,    33,    19,   250,    80,    92,    66,
      67,    33,   250,    85,    87,   229,   100,    90,   102,   266,
      27,   179,   266,   236,   126,   129,   273,   236,   266,   273,
       0,    88,   246,   217,    91,   273,   236,   250,    60,    96,
     224,   250,   114,   127,   117,    77,   119,    69,    67,   133,
     250,   108,    14,   266,    30,   236,    22,   266,   142,    11,
     273,    13,    30,   120,   273,    17,   266,     8,   100,   250,
     154,    93,    94,   273,   147,    30,    22,    30,    22,    25,
      32,   103,   139,    27,   106,   266,    30,   136,   110,   111,
      26,    11,   273,    13,   143,    27,    22,    17,    30,    30,
      33,   158,   124,    14,    25,    30,    25,   180,    25,    25,
      25,    21,    21,    31,   136,   222,   138,    31,    26,   141,
      25,   143,    29,    25,   146,    17,   129,   216,   124,   151,
      69,    90,   216,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,   232,   269,   114,     3,   144,   151,   179,    28,    29,
      30,   110,    63,    33,    34,    35,    36,    37,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
      -1,     3,     4,     5,   241,     7,   243,     9,    10,    11,
      -1,    13,    -1,    15,    16,    17,    -1,    19,    20,    -1,
      -1,   223,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
     232,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,   251,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    30,    -1,
      -1,    33,    34,    35,    36,    37,    -1,   269,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    30,    -1,    -1,    33,    34,
      35,    36,    37,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      30,    -1,    -1,    -1,    34,    35,    36,    37,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    29,    30,    -1,    -1,    -1,    34,
      35,    36,    37,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      30,    -1,    -1,    -1,    34,    35,    36,    37,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    37,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    37,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    37,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    37,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    37,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    37,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    37,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    37,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    37,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    37,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37
  };

  const signed char
  GraphQLParserImpl::yystos_[] =
  {
       0,     3,     4,     5,     7,     9,    10,    11,    13,    15,
      16,    17,    19,    20,    30,    39,    43,    44,    45,    46,
      47,    48,    55,    66,    94,    97,    98,   107,   108,   110,
     113,   114,   115,    27,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    37,    40,    41,    98,    40,    41,    41,    41,
      27,    86,    87,    88,    89,    41,    41,    24,    41,    57,
      58,    59,    64,    65,     0,    45,    41,    42,    41,    87,
      14,    87,    87,    87,    41,    30,    89,     8,    99,    87,
      14,    40,    87,    22,    25,    60,    61,    32,    58,    22,
      49,    55,    86,    22,   103,   104,    30,    41,    67,    91,
      30,    30,    61,    48,    95,    96,    91,   100,    87,    26,
      67,    87,    55,    41,    62,    63,    41,    87,    33,    50,
      51,    52,    55,    86,    55,    41,   105,   106,    14,    41,
     111,   112,    87,   105,    41,   101,   102,    25,    32,    96,
      91,    30,    91,   109,    87,    25,    23,    63,    61,    55,
      56,    23,    52,    25,    55,    25,    23,   106,    41,   116,
      87,    32,   111,    55,    32,   103,    32,   101,    91,   102,
      31,    55,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      28,    30,    34,    35,    36,    37,    51,    68,    69,    70,
      71,    73,    74,    75,    76,    80,    87,    28,    90,    91,
      92,    93,    90,    31,    25,    32,    91,    29,    68,    77,
      32,    41,    81,    82,    56,    90,    26,    53,    54,    21,
      21,    53,    41,    90,    29,    68,    25,    32,    82,    29,
      28,    30,    69,    70,    71,    72,    73,    74,    75,    78,
      83,    87,    87,    68,    29,    72,    79,    32,    41,    84,
      85,    29,    72,    25,    32,    85,    72
  };

  const signed char
  GraphQLParserImpl::yyr1_[] =
  {
       0,    38,    39,    40,    40,    40,    40,    40,    40,    40,
      40,    40,    40,    40,    40,    40,    40,    40,    40,    40,
      40,    41,    41,    42,    42,    43,    44,    44,    45,    45,
      45,    46,    46,    46,    46,    46,    46,    46,    46,    46,
      47,    47,    47,    47,    47,    48,    48,    48,    49,    50,
      50,    51,    52,    53,    53,    54,    55,    56,    56,    57,
      57,    58,    58,    58,    59,    59,    60,    61,    61,    62,
      62,    63,    64,    65,    65,    66,    67,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    69,    70,    71,    72,
      72,    72,    72,    72,    72,    72,    72,    73,    73,    74,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    76,    76,    77,    77,
      78,    78,    79,    79,    80,    80,    81,    81,    82,    83,
      83,    84,    84,    85,    86,    87,    87,    88,    88,    89,
      90,    90,    90,    91,    92,    93,    93,    94,    95,    95,
      96,    97,    98,    99,    99,   100,   100,   101,   102,   102,
     103,   103,   104,   105,   105,   106,   107,   108,   109,   109,
     110,   111,   112,   112,   113,   114,   115,   116,   116
  };

  const signed char
  GraphQLParserImpl::yyr2_[] =
  {
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     4,     4,     5,     1,     1,     1,     3,     1,
       2,     1,     4,     0,     1,     2,     3,     0,     1,     1,
       2,     1,     1,     1,     4,     6,     3,     0,     1,     1,
       2,     3,     3,     5,     3,     6,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     3,     1,     2,
       2,     3,     1,     2,     2,     3,     1,     2,     3,     2,
       3,     1,     2,     3,     1,     0,     1,     1,     2,     3,
       1,     1,     1,     1,     3,     2,     2,     5,     1,     2,
       3,     3,     7,     0,     2,     1,     2,     5,     1,     2,
       0,     1,     3,     1,     2,     5,     6,     5,     1,     3,
       6,     2,     1,     2,     6,     2,     6,     1,     3
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const GraphQLParserImpl::yytname_[] =
  {
  "EOF", "error", "\"invalid token\"", "\"directive\"", "\"enum\"",
  "\"extend\"", "\"false\"", "\"fragment\"", "\"implements\"", "\"input\"",
  "\"interface\"", "\"mutation\"", "\"null\"", "\"query\"", "\"on\"",
  "\"scalar\"", "\"schema\"", "\"subscription\"", "\"true\"", "\"type\"",
  "\"union\"", "\"!\"", "\"(\"", "\")\"", "\"...\"", "\":\"", "\"=\"",
  "\"@\"", "\"[\"", "\"]\"", "\"{\"", "\"|\"", "\"}\"", "VARIABLE",
  "INTEGER", "FLOAT", "STRING", "IDENTIFIER", "$accept", "start",
  "fragment_name", "name", "name_opt", "document", "definition_list",
  "definition", "schema_gate", "operation_definition", "operation_type",
  "variable_definitions", "variable_definition_list", "variable",
  "variable_definition", "default_value_opt", "default_value",
  "selection_set", "selection_set_opt", "selection_list", "selection",
  "field", "arguments", "arguments_opt", "argument_list", "argument",
  "fragment_spread", "inline_fragment", "fragment_definition",
  "type_condition", "value", "int_value", "float_value", "string_value",
  "value_const", "boolean_value", "null_value", "enum_value", "list_value",
  "value_list", "list_value_const", "value_const_list", "object_value",
  "object_field_list", "object_field", "object_value_const",
  "object_field_const_list", "object_field_const", "directives",
  "directives_opt", "directive_list", "directive", "type", "type_name",
  "list_type", "non_null_type", "schema_definition",
  "operation_type_definition_list", "operation_type_definition",
  "scalar_type_definition", "object_type_definition",
  "implements_interfaces_opt", "type_name_list", "field_definition",
  "field_definition_list", "arguments_definition_opt",
  "arguments_definition", "input_value_definition_list",
  "input_value_definition", "interface_type_definition",
  "union_type_definition", "union_members", "enum_type_definition",
  "enum_value_definition", "enum_value_definition_list",
  "input_object_type_definition", "type_extension_definition",
  "directive_definition", "directive_locations", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  GraphQLParserImpl::yyrline_[] =
  {
       0,   298,   298,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   323,   324,   328,   329,   334,   337,   338,   341,   342,
     343,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     369,   370,   371,   372,   373,   376,   377,   378,   382,   386,
     387,   390,   394,   398,   399,   402,   406,   410,   411,   413,
     414,   417,   418,   419,   422,   423,   426,   429,   430,   433,
     434,   437,   442,   446,   447,   451,   454,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   469,   472,   475,   478,
     479,   480,   481,   482,   483,   484,   485,   488,   489,   492,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   517,   518,   521,   522,
     526,   527,   531,   532,   537,   538,   542,   543,   546,   550,
     551,   555,   556,   559,   564,   567,   568,   571,   572,   575,
     580,   581,   582,   585,   588,   591,   592,   597,   601,   602,
     606,   609,   612,   615,   616,   619,   620,   623,   627,   628,
     631,   632,   635,   638,   639,   642,   644,   647,   650,   651,
     654,   657,   661,   662,   665,   668,   671,   675,   676
  };

  void
  GraphQLParserImpl::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  GraphQLParserImpl::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  GraphQLParserImpl::symbol_kind_type
  GraphQLParserImpl::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37
    };
    // Last valid token kind.
    const int code_max = 292;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // yy
#line 3314 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.cpp"

#line 679 "parser.ypp"


void yy::GraphQLParserImpl::error(const yy::location &loc, const std::string &str) {
  std::ostringstream out;
  out << loc << ": " << str;
  if (outError) {
    *outError = strdup(out.str().c_str());
  }
}

/* Workaround for syntax_error ctor being marked inline, which causes link
   errors if used from lexer.lpp. */
yy::GraphQLParserImpl::syntax_error make_error(const yy::location &loc, const std::string &str) {
  return yy::GraphQLParserImpl::syntax_error(loc, str);
}
