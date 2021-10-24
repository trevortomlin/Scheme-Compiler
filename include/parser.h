#ifndef parser_h_guard
#define parser_h_guard

#include "treenode.h"
#include "token.h"

class parser{

    public:

        treenode parse_token(token t);
        treenode parse_expression();
        treenode parse_variable();
        treenode parse_literal();
        treenode parse_procedure_call();
        treenode parse_lambda_expression();
        treenode parse_conditional();
        treenode parse_assignment();
        treenode parse_derived_expression();
        treenode parse_macro_use();
        treenode parse_macro_block();

};

#endif 