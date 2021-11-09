#include "parser.h"
#include "treenode.h"
#include "token.h"

treenode root("root");
treenode *current_node =  root;

treenode parser::parse(){

    while(current_token.type != token::TOKEN_EOF){
        //parse_expression();
        treenode node = parse_expression();
        std::cout << node.value << std::endl;

        current_node->insert(node)

        advance();
    }

    return root;
}

void parser::advance(){

    current_token = next_token;
    next_token = lex->advance();

}

parser::parser(lexer *_lex) : lex(_lex) {

    //lex = _lex;

    current_token = lex->advance();
    next_token = lex->advance();

    std::cout << "(" << current_token.type << ", " << "\"" << current_token.value << "\")" << std::endl;

    std::cout << root.value << std::endl;

    // Might want to move this to another location eventually.

    //Adding syntactic keywords to variable set.
    variables.insert("else");
    variables.insert("=>");
    variables.insert("define");
    variables.insert("unquote");
    variables.insert("unquote-splicing");

    //Adding expression keywords to variable set.
    variables.insert("quote");
    variables.insert("lambda");
    variables.insert("if");
    variables.insert("set!");
    variables.insert("begin");
    variables.insert("cond");
    variables.insert("and");
    variables.insert("or");
    variables.insert("case");
    variables.insert("let");
    variables.insert("let*");
    variables.insert("letrec");
    variables.insert("do");
    variables.insert("delay");
    variables.insert("quasiquote");

}

parser::~parser(){

    //delete lex;

}

treenode parser::parse_expression(){

    // Variable Node
    if (!tokenIsVariable(current_token)) { return treenode(current_token.value); }

    // Parentheses
    else if (current_token.type == token::TOKEN_L_PAREN || current_token.type == token::TOKEN_R_PAREN) { return treenode(current_token.value); }

    else{ return treenode("NOT VALID TOKEN."); }

}

// This should go in another class eventually.
bool parser::tokenIsVariable(token t) {

    const bool is_in = variables.find(t.value) != variables.end();
    return is_in;

}