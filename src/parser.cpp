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

    //std::cout << "(" << current_token.type << ", " << "\"" << current_token.value << "\")" << std::endl;

    //std::cout << root.value << std::endl;

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
    if (current_token.type == token::TOKEN_IDENTIFIER && !tokenIsVariable(current_token)) { return treenode(current_token.value); }

    // Literal Quotation 
    //〈quotation〉 −→ ’〈datum〉
    // current_token.type == token::TOKEN_L_PAREN && next_token.value == "quote" ||
    else if (current_token.type == token::TOKEN_SINGLE_QUOTE) {

        return parse_literal(current_token);

    }

    else if (current_token.type == token::TOKEN_L_PAREN) {

        //〈quotation〉 −→ (quote 〈datum〉)
        if (next_token.value == "quote") { return parse_literal(current_token); }

        //〈lambda expression〉 −→ (lambda 〈formals〉 〈body〉
        if (next_token.value == "lambda") { return parse_lambda_expression(); }

        //〈conditional〉 −→ (if 〈test〉 〈consequent〉 〈alternate〉)
        if (next_token.value == "if") { return parse_conditional(); }
        
        //〈conditional〉 −→ (if 〈test〉 〈consequent〉 〈alternate〉)
        //if (next_token.value == "lambda") { return parse_lambda_expression(current_token); }

        //〈assignment〉 −→ (set! 〈variable〉 〈expression〉)
        //if (next_token.value == "lambda") { return parse_lambda_expression(current_token); }

        return parse_procedure_call();

    }

    // else if (current_token.type == token::TOKEN_L_PAREN || current_token.type == token::TOKEN_R_PAREN) {

    //         return treenode(current_token.value); 

    // }

    // Literal Self-evaluating
    else if (current_token.type == token::TOKEN_BOOLEAN ||
             current_token.type == token::TOKEN_NUMBER ||
             current_token.type == token::TOKEN_CHARACTER ||
             current_token.type == token::TOKEN_STRING) {

        return treenode(current_token.value); 

    }

    else{ return treenode("NOT VALID TOKEN."); }

}

treenode parser::parse_conditional() {

    // Skip ( if
    advance();
    advance();

    treenode cond = treenode("cond");

    cond.insert(parse_expression());
    advance();
    cond.insert(parse_expression());
    advance();

    if (current_token.type != token::TOKEN_R_PAREN) {

        cond.insert(parse_expression());
        advance();

    }

    return cond;

}

// This should go in another class eventually.
bool parser::tokenIsVariable(token t) {

    const bool is_in = variables.find(t.value) != variables.end();
    return is_in;

}

treenode parser::parse_lambda_expression() {

    treenode lambda = treenode("lambda"); 

    // Skip ( lambda
    advance();
    advance();

    lambda.insert(parse_lambda_formals());

    advance();

    lambda.insert(parse_body());

    return lambda;

}

treenode parser::parse_lambda_formals(){

    treenode formals = treenode("formals");

    // Error
    if (current_token.type != token::TOKEN_L_PAREN) { std::cout << "Error." << std::endl; return treenode("ERROR."); }

    // Skip (
    advance();

    while (next_token.type != token::TOKEN_R_PAREN) {

            advance();

            if (current_token.type == token::TOKEN_PERIOD) {

                // Error
                if (formals.children.empty()) { return treenode("ERROR."); }

                formals.insert(treenode(current_token.value));
                advance();
                formals.insert(treenode(current_token.value));
                break;

            }

            else {
                // Error
                if (!tokenIsVariable(current_token)) { return treenode("ERROR."); }

                formals.insert(treenode(current_token.value));

            }
            
        }

    // Skip )
    advance();

    return formals;

}

treenode parser::parse_body(){

    //〈body〉 −→ 〈definition〉* 〈sequence
    treenode body = treenode("body");

    // definition
    if (current_token.type == token::TOKEN_L_PAREN &&
        next_token.value == "define") {

            advance();
            body.insert(parse_define());


    }

    return body;

}

treenode parser::parse_define() {

    if (current_token.value != "define") { return treenode("ERROR."); }

    treenode define = treenode("define");

    // skip define
    advance();

    //〈definition〉 −→ (define 〈variable〉 〈expression〉)
    if (tokenIsVariable(current_token)){

        define.insert(treenode(current_token.value));
        advance();
        define.insert(parse_expression());

    }

    // (define (〈variable〉 〈def formals〉) 〈body〉)
    else if (current_token.type == token::TOKEN_L_PAREN) {

        define.insert(treenode(current_token.value));
        advance();
        define.insert(parse_define_formals());
        advance();
        define.insert(parse_body());

    }

    // (begin 〈definition〉*)
    else if (current_token.type != token::TOKEN_R_PAREN) {

        define.insert(parse_define());

    }

    advance();
    return define;

}

treenode parser::parse_define_formals() {

    treenode formals = treenode("formals");

    while (next_token.type != token::TOKEN_R_PAREN) {

            advance();

            if (current_token.type == token::TOKEN_PERIOD) {

                formals.insert(treenode(current_token.value));
                advance();
                formals.insert(treenode(current_token.value));
                break;

            }

            else {
                // Error
                if (!tokenIsVariable(current_token)) { return treenode("ERROR."); }

                formals.insert(treenode(current_token.value));

            }
            
        }

    // Skip )
    advance();

    return formals;

}

treenode parser::parse_procedure_call() {

    treenode procedure = treenode("procedure");

    while (next_token.type != token::TOKEN_R_PAREN) {

        advance();
        procedure.insert(parse_expression());


    }

    advance();

    return procedure;

}

treenode parser::parse_literal(token t) {


    // Literal Quotation `<datum>
    if (t.type == token::TOKEN_SINGLE_QUOTE) {

        treenode node = treenode(current_token.value);
        advance();
        node.insert(parse_datum(current_token));
        //treenode node = treenode(parse_datum(current_token));
        return node;

    }

    // Literal Quotation (quote <datum>)
    else if (t.type == token::TOKEN_L_PAREN && next_token.value == "quote" ) {

        treenode node = treenode(next_token.value);
        advance();
        advance();
        node.insert(parse_datum(current_token));
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

        //treenode node = treenode(current_token.value);
        //node.insert(parse_compound_datum(t));
        treenode node = treenode(parse_compound_datum(t));
        return node;

    }

}

treenode parser::parse_compound_datum(token t) {

    // Vector #(〈datum〉*)
    if (t.type == token::TOKEN_VECTOR_CONSTANT) {

        treenode vector = treenode("vector");

        // No datum
        //if (next_token.type = token::TOKEN_R_PAREN) { return vector; }

        while (next_token.type != token::TOKEN_R_PAREN) {

            advance();
            vector.insert(parse_datum(current_token));
            
        }

        advance();

        return vector;

    }

    // List
    // (〈datum〉*) | (〈datum〉+ . 〈datum〉)
    else if (t.type == token::TOKEN_L_PAREN) {

        treenode list = treenode("list");

        while (next_token.type != token::TOKEN_R_PAREN) {

            advance();


            if (current_token.type == token::TOKEN_PERIOD) {

                //Error
                if (list.children.empty()){ return treenode("ERROR."); }

                list.insert(treenode(current_token.value));
                advance();
                list.insert(parse_datum(current_token));
                break;

            }

            else {

                list.insert(parse_datum(current_token));

            }
            
        }

        advance();

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