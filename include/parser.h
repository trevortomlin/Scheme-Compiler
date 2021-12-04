#ifndef parser_h_guard
#define parser_h_guard

#include "treenode.h"
#include "token.h"
#include "lexer.h"
#include <set>

class parser{

    public:

        parser(lexer *_lex);
        ~parser();

        bool tokenIsVariable(token t);

        treenode parse();


    private:

        lexer *lex;

        token current_token, next_token;

        std::set<std::string> variables;

        void advance();

        treenode parse_define();
        treenode parse_define_formals();
        treenode parse_expression();
        treenode parse_literal(token t);
        treenode parse_procedure_call();
        treenode parse_lambda_expression();
        treenode parse_lambda_formals();
        treenode parse_body();
        treenode parse_conditional();
        treenode parse_assignment();
        treenode parse_derived_expression();
        treenode parse_macro_use();
        treenode parse_macro_block();
        treenode parse_syntax_spec();
        treenode parse_syntax_rules();
        treenode parse_pattern();
        treenode parse_template();
        treenode parse_datum(token t);
        treenode parse_compound_datum(token t);
        treenode parse_cond_clause();
        treenode parse_case_clause();
};

#endif 