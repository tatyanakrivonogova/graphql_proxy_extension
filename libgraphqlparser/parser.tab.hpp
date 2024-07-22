// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton interface for Bison LALR(1) parsers in C++

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


/**
 ** \file /home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.hpp
 ** Define the yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_YY_HOME_KANADES_POSTGRES_CONTRIB_GRAPHQL_PROXY_LIBGRAPHQLPARSER_PARSER_TAB_HPP_INCLUDED
# define YY_YY_HOME_KANADES_POSTGRES_CONTRIB_GRAPHQL_PROXY_LIBGRAPHQLPARSER_PARSER_TAB_HPP_INCLUDED
// "%code requires" blocks.
#line 20 "parser.ypp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include "Ast.h"

using facebook::graphql::ast::Node;
using facebook::graphql::ast::Name;
using facebook::graphql::ast::Definition;
using facebook::graphql::ast::Document;
using facebook::graphql::ast::OperationDefinition;
using facebook::graphql::ast::VariableDefinition;
using facebook::graphql::ast::Variable;
using facebook::graphql::ast::SelectionSet;
using facebook::graphql::ast::Selection;
using facebook::graphql::ast::Field;
using facebook::graphql::ast::Argument;
using facebook::graphql::ast::FragmentSpread;
using facebook::graphql::ast::InlineFragment;
using facebook::graphql::ast::FragmentDefinition;
using facebook::graphql::ast::Value;
using facebook::graphql::ast::IntValue;
using facebook::graphql::ast::FloatValue;
using facebook::graphql::ast::StringValue;
using facebook::graphql::ast::BooleanValue;
using facebook::graphql::ast::NullValue;
using facebook::graphql::ast::EnumValue;
using facebook::graphql::ast::ListValue;
using facebook::graphql::ast::ObjectValue;
using facebook::graphql::ast::ObjectField;
using facebook::graphql::ast::Directive;
using facebook::graphql::ast::Type;
using facebook::graphql::ast::NamedType;
using facebook::graphql::ast::ListType;
using facebook::graphql::ast::NonNullType;

// Experimental schema support.
using facebook::graphql::ast::SchemaDefinition;
using facebook::graphql::ast::ScalarTypeDefinition;
using facebook::graphql::ast::ObjectTypeDefinition;
using facebook::graphql::ast::InterfaceTypeDefinition;
using facebook::graphql::ast::UnionTypeDefinition;
using facebook::graphql::ast::EnumTypeDefinition;
using facebook::graphql::ast::InputObjectTypeDefinition;
using facebook::graphql::ast::TypeExtensionDefinition;
using facebook::graphql::ast::DirectiveDefinition;
using facebook::graphql::ast::SchemaDefinition;
using facebook::graphql::ast::OperationTypeDefinition;
using facebook::graphql::ast::ScalarTypeDefinition;
using facebook::graphql::ast::ObjectTypeDefinition;
using facebook::graphql::ast::FieldDefinition;
using facebook::graphql::ast::InputValueDefinition;
using facebook::graphql::ast::InterfaceTypeDefinition;
using facebook::graphql::ast::UnionTypeDefinition;
using facebook::graphql::ast::EnumTypeDefinition;
using facebook::graphql::ast::EnumValueDefinition;
using facebook::graphql::ast::InputObjectTypeDefinition;
using facebook::graphql::ast::TypeExtensionDefinition;
using facebook::graphql::ast::DirectiveDefinition;

union yystype {                                         \
    const char *str;                                    \
    const char *heapStr;                                \
    Name *name;                                         \
    Definition *definition;                             \
    Document *document;                                 \
    OperationDefinition *operationDefinition;           \
    VariableDefinition *variableDefinition;             \
    Variable *variable;                                 \
    SelectionSet *selectionSet;                     \
    Selection *selection;                           \
    Field *field;                                   \
    Argument *argument;                             \
    FragmentSpread *fragmentSpread;                 \
    InlineFragment *inlineFragment;                 \
    FragmentDefinition *fragmentDefinition;         \
    Value *value;                                   \
    IntValue *intValue;                             \
    FloatValue *floatValue;                         \
    StringValue *stringValue;                       \
    BooleanValue *booleanValue;                     \
    NullValue *nullValue;                           \
    EnumValue *enumValue;                           \
    ListValue *arrayValue;                          \
    ObjectValue *objectValue;                       \
    ObjectField *objectField;                       \
    Directive *directive;                           \
    Type *type;                                     \
    NamedType *namedType;                           \
    ListType *listType;                             \
    NonNullType *nonNullType;                       \
    \
    std::vector<std::unique_ptr<Definition>> *definitionList;       \
    std::vector<std::unique_ptr<VariableDefinition>> *variableDefinitionList; \
    std::vector<std::unique_ptr<Selection>> *selectionList;         \
    std::vector<std::unique_ptr<Field>> *fieldList;                 \
    std::vector<std::unique_ptr<Argument>> *argumentList;           \
    std::vector<std::unique_ptr<Value>> *valueList;                 \
    std::vector<std::unique_ptr<ObjectField>> *objectFieldList;     \
    std::vector<std::unique_ptr<Directive>> *directiveList;         \
                                                                    \
    SchemaDefinition *schemaDefinition;                             \
    ScalarTypeDefinition *scalarTypeDefinition;                     \
    ObjectTypeDefinition *objectTypeDefinition;                     \
    InterfaceTypeDefinition *interfaceTypeDefinition;               \
    UnionTypeDefinition *unionTypeDefinition;                       \
    EnumTypeDefinition *enumTypeDefinition;                         \
    InputObjectTypeDefinition *inputObjectTypeDefinition;           \
    TypeExtensionDefinition *typeExtensionDefinition;               \
    DirectiveDefinition *directiveDefinition;                       \
    OperationTypeDefinition *operationTypeDefinition;               \
    InputValueDefinition *inputValueDefinition;                     \
    FieldDefinition *fieldDefinition;                               \
    EnumValueDefinition *enumValueDefinition;                       \
    \
    std::vector<std::unique_ptr<OperationTypeDefinition>> *operationTypeDefinitionList; \
    std::vector<std::unique_ptr<NamedType>> *typeNameList;          \
    std::vector<std::unique_ptr<InputValueDefinition>> *inputValueDefinitionList; \
    std::vector<std::unique_ptr<FieldDefinition>> *fieldDefinitionList; \
    std::vector<std::unique_ptr<Name>> *nameList;          \
    std::vector<std::unique_ptr<EnumValueDefinition>> *enumValueDefinitionList; \
};

#define YYSTYPE union yystype
#define YYLTYPE yy::location


#line 180 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.hpp"


# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif
# include "location.hh"


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

namespace yy {
#line 315 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.hpp"




  /// A Bison parser.
  class GraphQLParserImpl
  {
  public:
#ifdef YYSTYPE
# ifdef __GNUC__
#  pragma GCC message "bison: do not #define YYSTYPE in C++, use %define api.value.type"
# endif
    typedef YYSTYPE value_type;
#else
    /// Symbol semantic values.

#endif
    /// Backward compatibility (Bison 3.8).
    typedef value_type semantic_type;

    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

      location_type location;
    };

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        TOK_YYEMPTY = -2,
    TOK_EOF = 0,                   // EOF
    TOK_YYerror = 256,             // error
    TOK_YYUNDEF = 257,             // "invalid token"
    TOK_DIRECTIVE = 258,           // "directive"
    TOK_ENUM = 259,                // "enum"
    TOK_EXTEND = 260,              // "extend"
    TOK_FALSE = 261,               // "false"
    TOK_FRAGMENT = 262,            // "fragment"
    TOK_IMPLEMENTS = 263,          // "implements"
    TOK_INPUT = 264,               // "input"
    TOK_INTERFACE = 265,           // "interface"
    TOK_MUTATION = 266,            // "mutation"
    TOK_NULL = 267,                // "null"
    TOK_QUERY = 268,               // "query"
    TOK_ON = 269,                  // "on"
    TOK_SCALAR = 270,              // "scalar"
    TOK_SCHEMA = 271,              // "schema"
    TOK_SUBSCRIPTION = 272,        // "subscription"
    TOK_TRUE = 273,                // "true"
    TOK_TYPE = 274,                // "type"
    TOK_UNION = 275,               // "union"
    TOK_BANG = 276,                // "!"
    TOK_LPAREN = 277,              // "("
    TOK_RPAREN = 278,              // ")"
    TOK_ELLIPSIS = 279,            // "..."
    TOK_COLON = 280,               // ":"
    TOK_EQUAL = 281,               // "="
    TOK_AT = 282,                  // "@"
    TOK_LBRACKET = 283,            // "["
    TOK_RBRACKET = 284,            // "]"
    TOK_LBRACE = 285,              // "{"
    TOK_PIPE = 286,                // "|"
    TOK_RBRACE = 287,              // "}"
    TOK_VARIABLE = 288,            // VARIABLE
    TOK_INTEGER = 289,             // INTEGER
    TOK_FLOAT = 290,               // FLOAT
    TOK_STRING = 291,              // STRING
    TOK_IDENTIFIER = 292           // IDENTIFIER
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::token_kind_type token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 38, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // EOF
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_DIRECTIVE = 3,                         // "directive"
        S_ENUM = 4,                              // "enum"
        S_EXTEND = 5,                            // "extend"
        S_FALSE = 6,                             // "false"
        S_FRAGMENT = 7,                          // "fragment"
        S_IMPLEMENTS = 8,                        // "implements"
        S_INPUT = 9,                             // "input"
        S_INTERFACE = 10,                        // "interface"
        S_MUTATION = 11,                         // "mutation"
        S_NULL = 12,                             // "null"
        S_QUERY = 13,                            // "query"
        S_ON = 14,                               // "on"
        S_SCALAR = 15,                           // "scalar"
        S_SCHEMA = 16,                           // "schema"
        S_SUBSCRIPTION = 17,                     // "subscription"
        S_TRUE = 18,                             // "true"
        S_TYPE = 19,                             // "type"
        S_UNION = 20,                            // "union"
        S_BANG = 21,                             // "!"
        S_LPAREN = 22,                           // "("
        S_RPAREN = 23,                           // ")"
        S_ELLIPSIS = 24,                         // "..."
        S_COLON = 25,                            // ":"
        S_EQUAL = 26,                            // "="
        S_AT = 27,                               // "@"
        S_LBRACKET = 28,                         // "["
        S_RBRACKET = 29,                         // "]"
        S_LBRACE = 30,                           // "{"
        S_PIPE = 31,                             // "|"
        S_RBRACE = 32,                           // "}"
        S_VARIABLE = 33,                         // VARIABLE
        S_INTEGER = 34,                          // INTEGER
        S_FLOAT = 35,                            // FLOAT
        S_STRING = 36,                           // STRING
        S_IDENTIFIER = 37,                       // IDENTIFIER
        S_YYACCEPT = 38,                         // $accept
        S_start = 39,                            // start
        S_fragment_name = 40,                    // fragment_name
        S_name = 41,                             // name
        S_name_opt = 42,                         // name_opt
        S_document = 43,                         // document
        S_definition_list = 44,                  // definition_list
        S_definition = 45,                       // definition
        S_schema_gate = 46,                      // schema_gate
        S_operation_definition = 47,             // operation_definition
        S_operation_type = 48,                   // operation_type
        S_variable_definitions = 49,             // variable_definitions
        S_variable_definition_list = 50,         // variable_definition_list
        S_variable = 51,                         // variable
        S_variable_definition = 52,              // variable_definition
        S_default_value_opt = 53,                // default_value_opt
        S_default_value = 54,                    // default_value
        S_selection_set = 55,                    // selection_set
        S_selection_set_opt = 56,                // selection_set_opt
        S_selection_list = 57,                   // selection_list
        S_selection = 58,                        // selection
        S_field = 59,                            // field
        S_arguments = 60,                        // arguments
        S_arguments_opt = 61,                    // arguments_opt
        S_argument_list = 62,                    // argument_list
        S_argument = 63,                         // argument
        S_fragment_spread = 64,                  // fragment_spread
        S_inline_fragment = 65,                  // inline_fragment
        S_fragment_definition = 66,              // fragment_definition
        S_type_condition = 67,                   // type_condition
        S_value = 68,                            // value
        S_int_value = 69,                        // int_value
        S_float_value = 70,                      // float_value
        S_string_value = 71,                     // string_value
        S_value_const = 72,                      // value_const
        S_boolean_value = 73,                    // boolean_value
        S_null_value = 74,                       // null_value
        S_enum_value = 75,                       // enum_value
        S_list_value = 76,                       // list_value
        S_value_list = 77,                       // value_list
        S_list_value_const = 78,                 // list_value_const
        S_value_const_list = 79,                 // value_const_list
        S_object_value = 80,                     // object_value
        S_object_field_list = 81,                // object_field_list
        S_object_field = 82,                     // object_field
        S_object_value_const = 83,               // object_value_const
        S_object_field_const_list = 84,          // object_field_const_list
        S_object_field_const = 85,               // object_field_const
        S_directives = 86,                       // directives
        S_directives_opt = 87,                   // directives_opt
        S_directive_list = 88,                   // directive_list
        S_directive = 89,                        // directive
        S_type = 90,                             // type
        S_type_name = 91,                        // type_name
        S_list_type = 92,                        // list_type
        S_non_null_type = 93,                    // non_null_type
        S_schema_definition = 94,                // schema_definition
        S_operation_type_definition_list = 95,   // operation_type_definition_list
        S_operation_type_definition = 96,        // operation_type_definition
        S_scalar_type_definition = 97,           // scalar_type_definition
        S_object_type_definition = 98,           // object_type_definition
        S_implements_interfaces_opt = 99,        // implements_interfaces_opt
        S_type_name_list = 100,                  // type_name_list
        S_field_definition = 101,                // field_definition
        S_field_definition_list = 102,           // field_definition_list
        S_arguments_definition_opt = 103,        // arguments_definition_opt
        S_arguments_definition = 104,            // arguments_definition
        S_input_value_definition_list = 105,     // input_value_definition_list
        S_input_value_definition = 106,          // input_value_definition
        S_interface_type_definition = 107,       // interface_type_definition
        S_union_type_definition = 108,           // union_type_definition
        S_union_members = 109,                   // union_members
        S_enum_type_definition = 110,            // enum_type_definition
        S_enum_value_definition = 111,           // enum_value_definition
        S_enum_value_definition_list = 112,      // enum_value_definition_list
        S_input_object_type_definition = 113,    // input_object_type_definition
        S_type_extension_definition = 114,       // type_extension_definition
        S_directive_definition = 115,            // directive_definition
        S_directive_locations = 116              // directive_locations
      };
    };

    /// (Internal) symbol kind.
    typedef symbol_kind::symbol_kind_type symbol_kind_type;

    /// The number of tokens.
    static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol kind
    /// via kind ().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol () YY_NOEXCEPT
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value (std::move (that.value))
        , location (std::move (that.location))
      {}
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);
      /// Constructor for valueless symbols.
      basic_symbol (typename Base::kind_type t,
                    YY_MOVE_REF (location_type) l);

      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    YY_RVREF (value_type) v,
                    YY_RVREF (location_type) l);

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }



      /// Destroy contents, and record that is empty.
      void clear () YY_NOEXCEPT
      {
        Base::clear ();
      }

      /// The user-facing name of this symbol.
      std::string name () const YY_NOEXCEPT
      {
        return GraphQLParserImpl::symbol_name (this->kind ());
      }

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      value_type value;

      /// The location.
      location_type location;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_kind
    {
      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Default constructor.
      by_kind () YY_NOEXCEPT;

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that) YY_NOEXCEPT;
#endif

      /// Copy constructor.
      by_kind (const by_kind& that) YY_NOEXCEPT;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t) YY_NOEXCEPT;



      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
    };

    /// Backward compatibility for a private implementation detail (Bison 3.6).
    typedef by_kind by_type;

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_kind>
    {};

    /// Build a parser object.
    GraphQLParserImpl (bool enableSchema_yyarg, Node **outAST_yyarg, const char **outError_yyarg, void *scanner_yyarg);
    virtual ~GraphQLParserImpl ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    GraphQLParserImpl (const GraphQLParserImpl&) = delete;
    /// Non copyable.
    GraphQLParserImpl& operator= (const GraphQLParserImpl&) = delete;
#endif

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static std::string symbol_name (symbol_kind_type yysymbol);



    class context
    {
    public:
      context (const GraphQLParserImpl& yyparser, const symbol_type& yyla);
      const symbol_type& lookahead () const YY_NOEXCEPT { return yyla_; }
      symbol_kind_type token () const YY_NOEXCEPT { return yyla_.kind (); }
      const location_type& location () const YY_NOEXCEPT { return yyla_.location; }

      /// Put in YYARG at most YYARGN of the expected tokens, and return the
      /// number of tokens stored in YYARG.  If YYARG is null, return the
      /// number of expected tokens (guaranteed to be less than YYNTOKENS).
      int expected_tokens (symbol_kind_type yyarg[], int yyargn) const;

    private:
      const GraphQLParserImpl& yyparser_;
      const symbol_type& yyla_;
    };

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    GraphQLParserImpl (const GraphQLParserImpl&);
    /// Non copyable.
    GraphQLParserImpl& operator= (const GraphQLParserImpl&);
#endif


    /// Stored state numbers (used for stacks).
    typedef short state_type;

    /// The arguments of the error message.
    int yy_syntax_error_arguments_ (const context& yyctx,
                                    symbol_kind_type yyarg[], int yyargn) const;

    /// Generate an error message.
    /// \param yyctx     the context in which the error occurred.
    virtual std::string yysyntax_error_ (const context& yyctx) const;
    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT;

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT;

    static const short yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_kind_type enum.
    static symbol_kind_type yytranslate_ (int t) YY_NOEXCEPT;

    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *yystr);

    /// For a symbol, its name in clear.
    static const char* const yytname_[];


    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const short yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const unsigned char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const short yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const short yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const short yytable_[];

    static const short yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const signed char yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const signed char yyr1_[];

    // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
    static const signed char yyr2_[];


#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r) const;
    /// Print the state stack on the debug stream.
    virtual void yy_stack_print_ () const;

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol kind, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200) YY_NOEXCEPT
        : seq_ (n)
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack (const stack&) = delete;
      /// Non copyable.
      stack& operator= (const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.begin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.end ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range) YY_NOEXCEPT
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack (const stack&);
      /// Non copyable.
      stack& operator= (const stack&);
#endif
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1) YY_NOEXCEPT;

    /// Constants.
    enum
    {
      yylast_ = 955,     ///< Last index in yytable_.
      yynnts_ = 79,  ///< Number of nonterminal symbols.
      yyfinal_ = 74 ///< Termination state number.
    };


    // User arguments.
    bool enableSchema;
    Node **outAST;
    const char **outError;
    void *scanner;

  };


} // yy
#line 1063 "/home/kanades/postgres/contrib/graphql_proxy/libgraphqlparser/parser.tab.hpp"




#endif // !YY_YY_HOME_KANADES_POSTGRES_CONTRIB_GRAPHQL_PROXY_LIBGRAPHQLPARSER_PARSER_TAB_HPP_INCLUDED
