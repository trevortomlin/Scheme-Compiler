#include "parser.h"
#include "treenode.h"
#include "token.h"
#include <memory>
#include <queue>

treenode root("root");

void parser::print_errors(treenode t) {

    std::cout << t.value << std::endl;

    if (t.children.size() != 0) {

        for (auto &child : t.children) {
            
            parser::print_errors(child);    

        }

    }

}

treenode parser::check_tree(treenode t) {

    treenode error("Errors");

    // bfs looking for errors.

    std::queue<treenode> q;

    q.push(t);

    while (!q.empty()) {

        treenode s = q.front();
        q.pop();
        
        if (s.value.substr(0, 16) == "NOT VALID TOKEN.") error.insert(s);

        for (auto &child : s.children) {

            q.push(child);

        }

    }

    // There are no errors
    if (error.children.empty()) return t;

    return error;

}

treenode parser::parse(){

    while(current_token.type != token::TOKEN_EOF){

        treenode node = parse_expression();

        root.insert(node);

        advance();
    }

    std::cout << "\r\nAbstract Syntax Tree:" << std::endl;
    treenode::printTree("", root);

    std::cout << "\n";

    return parser::check_tree(root);

}

void parser::advance(){

    current_token = next_token;
    next_token = lex->advance();

    std::cout << current_token.value << " Character: " << current_token.p.charNum << " Line: " << current_token.p.lineNum << ".\n";

}

parser::parser(lexer *_lex) : lex(_lex) {

    std::cout << "Tokens:" << '\n';

    current_token = lex->advance();
    next_token = lex->advance();

    std::cout << current_token.value << " Character: " << current_token.p.charNum << " Line: " << current_token.p.lineNum << ".\n";

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
    if (current_token.type == token::TOKEN_IDENTIFIER && tokenIsVariable(current_token)) { return treenode(current_token.value); }

    // Literal Quotation 
    //???quotation??? ?????? ??????datum???
    else if (current_token.type == token::TOKEN_SINGLE_QUOTE) {

        return parse_literal(current_token);

    }

    // quasiquote
    else if (current_token.type == token::TOKEN_BACK_QUOTE) {}

    else if (current_token.type == token::TOKEN_L_PAREN) {

        //???quotation??? ?????? (quote ???datum???)
        if (next_token.value == "quote") { return parse_literal(current_token); }

        //???lambda expression??? ?????? (lambda ???formals??? ???body???
        else if (next_token.value == "lambda") { return parse_lambda_expression(); }

        //???conditional??? ?????? (if ???test??? ???consequent??? ???alternate???)
        else if (next_token.value == "if") { return parse_conditional(); }

        //???assignment??? ?????? (set! ???variable??? ???expression???)
        else if (next_token.value == "set!") { return parse_assignment(); }

        // derived expressions
        else if (next_token.value == "cond") { 

                // (cond ???cond clause???+) |
                // (cond ???cond clause???* (else ???sequence???))

                // Skip (cond
                advance();
                advance();

                treenode cond = treenode("cond");

                do {

                    if (current_token.type == token::TOKEN_L_PAREN &&
                        next_token.value == "else") {
                        
                        // Skip (
                        advance();
                        cond.insert(treenode(current_token.value));
                        advance();
                        cond.insert(parse_expression());
                        advance();
                        return cond;

                    }

                    else {

                        cond.insert(parse_cond_clause());

                    }


                } while(current_token.type != token::TOKEN_R_PAREN);


                if (cond.children.empty()) { return treenode("Error. Cond."); }

                // Skip )
                advance();

                return cond;

        }

        else if (next_token.value == "case") {

            treenode case_node = treenode("case");

            // Skip ( case
            advance();
            advance();

            case_node.insert(parse_expression());

            // Skip expression
            advance();

            // (case ???expression??????case clause???+)
            // case ???expression??????case clause???* (else ???sequence???))
    
            while (current_token.type != token::TOKEN_R_PAREN) {


                if (current_token.type == token::TOKEN_L_PAREN &&
                    next_token.value == "else"){
                        
                    // Skip ( else
                    advance();
                    advance();  

                    // Parse Sequence
                    while (current_token.type != token::TOKEN_R_PAREN) {

                        case_node.insert(parse_expression());
                        advance();

                    }

                    // Skip ))
                    advance();
                    advance();

                    return case_node;

                }

                else {

                    case_node.insert(parse_case_clause());

                }


            }

            return case_node;

        }

        else if (next_token.value == "and" ||
                 next_token.value == "or") {

            treenode rel_node = treenode(next_token.value);

            // Skip (and
            advance();
            advance();

            while (current_token.type != token::TOKEN_R_PAREN) {

                rel_node.insert(parse_expression());

            }

            // Skip )
            advance();

            return rel_node;

        }

        else if (next_token.value == "let" || 
                 next_token.value == "let*" ||
                 next_token.value == "letrec") {

            // Skip (
            advance();

            // let or let* or letrec
            treenode let = treenode(current_token.value);

            // (let ???variable??? (???binding spec???*) ???body???)
            if (current_token.value == "let" && tokenIsVariable(next_token)) {
                let.insert(treenode(next_token.value));
                advance();
                advance();
            }

            if (current_token.type != token::TOKEN_L_PAREN) { return treenode("Error. Let."); }

            // Skip (
            advance();

            while (current_token.type != token::TOKEN_R_PAREN) {
                
                if (!tokenIsVariable(current_token)) {

                    return treenode("Error. Let.");

                }

                let.insert(treenode(current_token.value));
                let.insert(parse_expression());

                advance();
                advance();

            }

            // Skip )
            advance();

            let.insert(parse_body());

            // Skip )
            advance();

            return let;

        }

        else if (next_token.value == "begin") {

            // Skip ( begin
            advance();
            advance();

            treenode begin = treenode("begin");

            while (current_token.type != token::TOKEN_R_PAREN) {

                begin.insert(parse_expression());

            }

            // Skip )
            advance();

            return begin;

        }

        else if (next_token.value == "do") {
            
            // Skip (do
            advance();
            advance();

            if (current_token.type != token::TOKEN_L_PAREN) { return treenode("Error. Do."); }

            // Skip (
            advance();

            treenode do_node("do");

            while(current_token.type != token::TOKEN_R_PAREN) {

                do_node.insert(parse_iteration_spec());
                advance();

            }

            // Skip )
            advance();

            if (current_token.type != token::TOKEN_L_PAREN) { return treenode("Error. Do."); }

            // Skip (
            advance();

            do_node.insert(parse_expression());

            advance();

            if (current_token.type != token::TOKEN_R_PAREN) {

                // Do result
                while(current_token.type != token::TOKEN_R_PAREN) {

                    do_node.insert(parse_expression());

                }

            }

            // Skip )
            advance();

            //Command
            while(current_token.type != token::TOKEN_R_PAREN) {

                do_node.insert(parse_expression());

            }

            //Skip )
            advance();

            return do_node;

        }

        else if (next_token.value == "delay") {

            // Skip (delay
            advance();
            advance();

            treenode delay = treenode("delay");

            delay.insert(parse_expression());

            // Skip )
            advance();

            return delay;

        }

        else if (next_token.value == "quasiquote") {}

        //???macro use??? ?????? (???keyword??? ???datum???*)
        else if (next_token.type == token::TOKEN_IDENTIFIER && !tokenIsVariable(next_token)) { return parse_macro_use(); }

        else {

            return parse_procedure_call();

        }

    }

    // Literal Self-evaluating
    else if (current_token.type == token::TOKEN_BOOLEAN ||
             current_token.type == token::TOKEN_NUMBER ||
             current_token.type == token::TOKEN_CHARACTER ||
             current_token.type == token::TOKEN_STRING) {

        return treenode(current_token.value); 

    }

    else return treenode("NOT VALID TOKEN. '" + current_token.value + "' " + "At line " + std::to_string(current_token.p.lineNum) + " char " + std::to_string(current_token.p.charNum) + "."); 

}

treenode parser::parse_iteration_spec(){

    //???iteration spec??? ?????? (???variable??? ???init??? ???step???)
    // | (???variable??? ???init???

    if (current_token.type != token::TOKEN_L_PAREN) { return treenode("Error. Iteration spec."); }

    // Skip (
    advance();

    treenode is = treenode("iteration-spec");


    tokenIsVariable(current_token) ? is.insert(current_token.value) : is.insert(treenode("Error. Iteration spec"));

    advance();

    // init
    is.insert(parse_expression());

    advance();

    if (next_token.type == token::TOKEN_R_PAREN) {

        return is;

    }

    else {

        // step
        is.insert(parse_expression());

        advance();

        return is;

    }

}

treenode parser::parse_case_clause() {

    // ???case clause??? ?????? ((???datum???*) ???sequence???)

    if (current_token.type != token::TOKEN_L_PAREN) { return treenode("Error. Case Clause."); }

    // Skip (
    advance();

     if (current_token.type != token::TOKEN_L_PAREN) { return treenode("Error. Case Clause."); }

    // Skip (
    advance();

    treenode case_clause = treenode("case-clause");

    while (current_token.type != token::TOKEN_R_PAREN) {

        case_clause.insert(parse_datum(current_token));
        advance();

    }

    // Skip )
    advance();

    while (current_token.type != token::TOKEN_R_PAREN) {

        case_clause.insert(parse_expression());
        advance();

    }

    // Skip )
    advance();

    return case_clause;

}

treenode parser::parse_cond_clause() {

    treenode cc = treenode("cond-clause");


    if (current_token.type != token::TOKEN_L_PAREN) { return treenode("Error. Conditional Clause."); }

    // Skip (
    advance();

    cc.insert(parse_expression());

    // (???test???)
    if (current_token.type == token::TOKEN_R_PAREN) { return cc; }

    // (???test??? => ???recipient???)
    else if (current_token.value == "=>") {

        cc.insert(treenode(current_token.value));
        advance();
        cc.insert(parse_expression());
        return cc;

    }

    //???cond clause??? ?????? (???test??? ???sequence???)
    else {

        cc.insert(parse_expression());
        return cc;

    }

}

treenode parser::parse_macro_block() {

    //???macro block????????? (let-syntax (???syntax spec???*) ???body???)
    // (letrec-syntax (???syntax spec???*) ???body???)

    if (current_token.type != token::TOKEN_L_PAREN 
        || next_token.value == "let-syntax"
        || next_token.value == "letrc-syntax") { return treenode("Error. Macro Block."); }

    treenode mb = treenode("macro-block");

    // Skip ( let-syntax
    advance();
    advance();

    while (current_token.type == token::TOKEN_L_PAREN){
    
        mb.insert(parse_syntax_spec());

        advance();

    }

    mb.insert(parse_body());

    return mb;

}

treenode parser::parse_syntax_spec() {

    //???syntax spec??? ?????? (???keyword??? ???transformer spec???)

    if (current_token.type != token::TOKEN_L_PAREN) { return treenode("Error. Syntax Spec."); }

    treenode ss = treenode("syntax-spec");

    //  skip (
    advance();

    //  keyword
    ss.insert(treenode(current_token.value));

    // transformer spec
    ss.insert(parse_transformer_spec());

    // skip )
    advance();

    return ss;

}

treenode parser::parse_transformer_spec() {

    // ???transformer spec??? ?????? (syntax-rules (???identifier???*) ???syntax rule???*)

    if (current_token.type != token::TOKEN_L_PAREN || next_token.value == "syntax-rules") { return treenode("Error. Transformer Specs."); }

    treenode ts = treenode("transformer-spec");  

    // Skip ( syntax-rules
    advance();
    advance();
    
    if (current_token.type != token::TOKEN_L_PAREN) { return treenode("Error. Transformer Specs."); }

    // Skip (
    advance();

    // Identifiers
    while (current_token.type != token::TOKEN_R_PAREN) {

        if (current_token.type != token::TOKEN_IDENTIFIER) { return treenode("Error. Transformer Specs."); }

        ts.insert(treenode(current_token.value));
        advance();

    }

    // ???syntax rule???*)
    while (current_token.type != token::TOKEN_R_PAREN) {

        ts.insert(parse_syntax_rules());

    }

    return ts;

}

treenode parser::parse_syntax_rules() {

    //???syntax rule??? ?????? (???pattern??? ???template???)
    if (current_token.type != token::TOKEN_L_PAREN) { return treenode("Error. Syntax Rules."); }

    // skip (
    advance();

    treenode sr = treenode("syntax-rules");

    sr.insert(parse_pattern());

    sr.insert(parse_template());

    // Skip )
    advance();

    return sr;

}

treenode parser::parse_template(){

    /*
    ???template??? ?????? ???pattern identifier???
     | (???template element???*)
     | (???template element???+ . ???template???)
     | #(???template element???*)
     | ???template datum???
    */

    treenode template_node = treenode("template");

    //???pattern identifier???
    if (current_token.value == "...") {

        template_node.insert(treenode(current_token.value));

    }

    // (???template element???*) |
    // (???template element???+ . ???template???)
    else if (current_token.type == token::TOKEN_L_PAREN) {

        while (current_token.type != token::TOKEN_R_PAREN){

            advance();

            if (next_token.value == "...") {

                if (template_node.children.empty()) { return treenode("Error template element."); }

                template_node.insert(parse_template());
                template_node.insert(treenode(current_token.value));

                advance();

            }

            // (???template element???+ . ???template???)
            else if (current_token.type == token::TOKEN_PERIOD) {

                if (template_node.children.empty()) { return treenode("Error template element."); }

                template_node.insert(treenode(current_token.value));
                advance();
                template_node.insert(treenode(current_token.value));
                advance();

            }

            else {

                template_node.insert(parse_template());

            }

        }

    }

    // #(???template element???*)
    else if (current_token.type == token::TOKEN_VECTOR_CONSTANT) {

        while(current_token.type != token::TOKEN_R_PAREN) {

            advance();

            if (next_token.value == "...") {

                template_node.insert(parse_template());
                template_node.insert(next_token.value);
                advance();

            }

            else {

                template_node.insert(parse_template());

            }

        }

    }

    // Template datum (Pattern datum)
    else if (current_token.type == token::TOKEN_BOOLEAN ||
        current_token.type == token::TOKEN_NUMBER ||
        current_token.type == token::TOKEN_CHARACTER ||
        current_token.type == token::TOKEN_STRING) {

        template_node.insert(treenode(current_token.value));

    }

    return template_node;

}

treenode parser::parse_pattern(){

    /* 
    ???pattern??? ?????? ???pattern identifier???
     | (???pattern???*)
     | (???pattern???+ . ???pattern???)
     | (???pattern???* ???pattern??? ???ellipsis???)
     | #(???pattern???*)
     | #(???pattern???* ???pattern??? ???ellipsis???)
     | ???pattern datum???
    */

    treenode pattern = treenode("pattern");

    // Pattern identifier
    if (current_token.value == "...") {

        pattern.insert(treenode(current_token.value));

    }

    // (???pattern???*) | (???pattern???+ . ???pattern???) | (???pattern???* ???pattern??? ???ellipsis???)
    else if (current_token.type == token::TOKEN_L_PAREN) {

        while (current_token.type != token::TOKEN_R_PAREN) {

            advance();

            // (???pattern???+ . ???pattern???)
            if (current_token.type == token::TOKEN_PERIOD) {

                // Error
                if (pattern.children.empty()) { return treenode("ERROR. Pattern."); }

                pattern.insert(treenode(current_token.value));
                advance();
                pattern.insert(parse_pattern());
                break;

            }

            // (???pattern???* ???pattern??? ???ellipsis???)
            else if (current_token.value == "...") {

                pattern.insert(treenode(current_token.value));
                advance();
                break;

            }

            else {

                pattern.insert(parse_pattern());

            }   
        }
    }   

    // #(???pattern???*) | #(???pattern???* ???pattern??? ???ellipsis???)
    else if (current_token.type == token::TOKEN_VECTOR_CONSTANT) {

        while (current_token.type != token::TOKEN_R_PAREN) {

            advance();

            // #(???pattern???* ???pattern??? ???ellipsis???)
            if (current_token.value == "...") {

                if (pattern.children.size() < 2) { return treenode("ERROR. Pattern."); }

                pattern.insert(treenode(current_token.value));
                advance();
                break;

            }

            else {

                pattern.insert(parse_pattern());

            }   
        }
    }

    // Pattern datum
    else if (current_token.type == token::TOKEN_BOOLEAN ||
        current_token.type == token::TOKEN_NUMBER ||
        current_token.type == token::TOKEN_CHARACTER ||
        current_token.type == token::TOKEN_STRING) {

        pattern.insert(treenode(current_token.value));

    }

    return pattern;

}

treenode parser::parse_macro_use() {

    treenode mu = treenode("macro-use");

    advance();

    mu.insert(treenode(current_token.value));

    advance();

    while (current_token.type != token::TOKEN_R_PAREN) {

        mu.insert(parse_datum(current_token));
        advance();

    }

    return mu;

}

treenode parser::parse_assignment() {

    treenode assign = treenode("assign");

    // Skip ( set!
    advance();
    advance();

    assign.insert(treenode(current_token.value));

    advance();

    assign.insert(parse_expression());

    advance();

    return assign;


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
    return !is_in;

}

treenode parser::parse_lambda_expression() {

    treenode lambda = treenode("lambda"); 

    // Skip ( lambda
    advance();
    advance();

    lambda.insert(parse_lambda_formals());

    //advance();

    lambda.insert(parse_body());

    return lambda;

}

treenode parser::parse_lambda_formals(){

    treenode formals = treenode("formals");

    // Error
    // if (current_token.type != token::TOKEN_L_PAREN) { std::cout << "Error." << std::endl; return treenode("ERROR."); }

    // Skip (
    //???formals????????????variable???
    if (current_token.type != token::TOKEN_L_PAREN) { 
        formals.insert(treenode(current_token.value));
        advance(); 
        return formals;
    }

    while (current_token.type != token::TOKEN_R_PAREN) {

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

    //???body??? ?????? ???definition???* ???sequence???
    treenode body = treenode("body");

    // definition*
    while (current_token.type == token::TOKEN_L_PAREN &&
        next_token.value == "define") {

            advance();
            body.insert(parse_define());

    }

    advance();

    //???sequence??? ?????? ???command???* ???expression)
    while(current_token.type != token::TOKEN_R_PAREN) {

        body.insert(parse_expression());
        advance();

    }

    // Skip )
    advance();

    return body;

}

treenode parser::parse_define() {

    if (current_token.value != "define") { return treenode("ERROR. '" + current_token.value + "'"); }

    treenode define = treenode("define");

    // skip define
    advance();

    //???definition??? ?????? (define ???variable??? ???expression???)
    if (tokenIsVariable(current_token)){

        define.insert(treenode(current_token.value));
        advance();
        define.insert(parse_expression());

    }

    // (define (???variable??? ???def formals???) ???body???)
    else if (current_token.type == token::TOKEN_L_PAREN) {

        define.insert(treenode(current_token.value));
        advance();
        define.insert(parse_define_formals());
        advance();
        define.insert(parse_body());

    }

    // This is incorrect.
    // (begin ???definition???*)
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

        treenode node = treenode(parse_compound_datum(t));
        return node;

    }

}

treenode parser::parse_compound_datum(token t) {

    // Vector #(???datum???*)
    if (t.type == token::TOKEN_VECTOR_CONSTANT) {

        treenode vector = treenode("vector");

        while (next_token.type != token::TOKEN_R_PAREN) {

            advance();
            vector.insert(parse_datum(current_token));
            
        }

        advance();

        return vector;

    }

    // List
    // (???datum???*) | (???datum???+ . ???datum???)
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

    //???abbreviation??? ?????? ???abbrev prefix??? ???datum???
    //???abbrev prefix??? ?????? ??? | ` | , | ,@
    else if (t.type == token::TOKEN_SINGLE_QUOTE ||
             t.type == token::TOKEN_BACK_QUOTE ||
             t.type == token::TOKEN_COMMA ||
             t.type == token::TOKEN_COMMA_AT) {

            treenode node = treenode(current_token.value);
            node.insert(parse_datum(t));
            return node;    

        }

}