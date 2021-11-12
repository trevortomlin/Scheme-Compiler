#include "parser.h"
#include "treenode.h"
#include "token.h"

treenode root("root");

treenode parser::parse(){

    while(current_token.type != token::TOKEN_EOF){
        //parse_expression();
        treenode node = parse_expression();
        //std::cout << node.value << std::endl;

        root.insert(node);

        advance();
    }

    std::cout << "\r\nPrint Tree:" << std::endl;
    treenode::printTree("", root);

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
    else if (current_token.type == token::TOKEN_L_PAREN || current_token.type == token::TOKEN_R_PAREN) {

        // Literal Quotation
        if (current_token.type == token::TOKEN_L_PAREN && next_token.value == "quote") {

            return parse_literal(current_token);

        }

        else {

            return treenode(current_token.value); 

        }

    }

    // Literal Self-evaluating
    else if (current_token.type == token::TOKEN_BOOLEAN ||
             current_token.type == token::TOKEN_NUMBER ||
             current_token.type == token::TOKEN_CHARACTER ||
             current_token.type == token::TOKEN_STRING) {

        return treenode(current_token.value); 

    }

    else{ return treenode("NOT VALID TOKEN."); }

}

// This should go in another class eventually.
bool parser::tokenIsVariable(token t) {

    const bool is_in = variables.find(t.value) != variables.end();
    return is_in;

}

treenode parser::parse_literal(token t) {


    // Literal Quotation `<datum>
    if (t.type == token::TOKEN_SINGLE_QUOTE) {

        treenode node = treenode(current_token.value);
        node.insert(parse_datum(t));
        return node;

    }

    // Literal Quotation (quote <datum>)
    else if (t.type == token::TOKEN_L_PAREN && next_token.value == "quote" ) {

        treenode node = treenode(next_token.value);
        node.insert(parse_datum(t));
        return node;

    }

    else {

        // Throw error.
        std::cout << "Parse literal error." << std::endl;


    }


}

treenode parser::parse_datum(token t) {

    // Simple Datum
    if (current_token.type == token::TOKEN_BOOLEAN ||
        current_token.type == token::TOKEN_NUMBER ||
        current_token.type == token::TOKEN_CHARACTER ||
        current_token.type == token::TOKEN_STRING ||
        current_token.type == token::TOKEN_IDENTIFIER) {

        return treenode(current_token.value);         


    }

    // Compound Datum
    else {

        treenode node = treenode(current_token.value);
        node.insert(parse_compound_datum(t));
        return node;

    }

}

treenode parser::parse_compound_datum(token t) {

    // Vector #(〈datum〉*)
    if (t.type == token::TOKEN_VECTOR_CONSTANT) {

        treenode vector = treenode("vector");

        // No datum
        if (next_token.type = token::TOKEN_R_PAREN) { return vector; }

        while (current_token.type != token::TOKEN_R_PAREN) {
 
                vector.insert(parse_datum(t));
            
        }

        return vector;

    }

    // List
    // (〈datum〉*) | (〈datum〉+ . 〈datum〉)
    else if (t.type == token::TOKEN_L_PAREN) {

        treenode list = treenode("list");

        // No datum
        if (next_token.type = token::TOKEN_R_PAREN) { return list; }

        while (current_token.type != token::TOKEN_R_PAREN) {

            list.insert(parse_datum(t));

        }

        return list;

    }

    //〈abbreviation〉 −→ 〈abbrev prefix〉 〈datum〉
    //〈abbrev prefix〉 −→ ’ | ` | , | ,@
    else if (t.type == token::TOKEN_SINGLE_QUOTE ||
             t.type == token::TOKEN_BACK_QUOTE ||
             t.type == token::TOKEN_COMMA ||
             t.type == token::TOKEN_COMMA_AT) {

            treenode node = treenode(current_token.value);
            node.insert(parse_datum(t));
            return node;    

        }

}