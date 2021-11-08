#ifndef parser_h_guard
#define parser_h_guard

#include "treenode.h"
#include "token.h"
#include <set>

class parser{

    std::set<std::string> variables;

    public:

        parser();

        treenode parse_token(token t);
        treenode parse_expression(token t);
        treenode parse_variable(token t);
        treenode parse_literal(token t);
        treenode parse_procedure_call(token t);
        treenode parse_lambda_expression(token t);
        treenode parse_conditional(token t);
        treenode parse_assignment(token t);
        treenode parse_derived_expression(token t);
        treenode parse_macro_use(token t);
        treenode parse_macro_block(token t);

        bool tokenIsVariable(token t);

};

#endif 