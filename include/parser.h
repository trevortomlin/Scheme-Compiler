#ifndef parser_h_guard
#define parser_h_guard

#include "treenode.h"
#include "token.h"
#include "lexer.h"
#include <set>

class parser{

    lexer *lex;

    token current_token, next_token;

    std::set<std::string> variables;

    public:

        parser(lexer *_lex);
        ~parser();

        void advance();

        treenode parse();
        treenode parse_expression();
        treenode parse_variable(token t);
        treenode parse_literal(token t);
        treenode parse_procedure_call(token t);
        treenode parse_lambda_expression(token t);
        treenode parse_conditional(token t);
        treenode parse_assignment(token t);
        treenode parse_derived_expression(token t);
        treenode parse_macro_use(token t);
        treenode parse_macro_block(token t);
        treenode parser::parse_datum(token t);

        bool tokenIsVariable(token t);

};

#endif 