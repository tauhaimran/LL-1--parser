#ifndef CFGMANUAL_H
#define CFGMANUAL_H

#include <iostream>
#include "CFG.h"

/* Grammar Definitions
S -> StmtList
StmtList -> Stmt StmtList | ε
Stmt -> id = Expr ; | if ( Cond ) { StmtList }
Expr -> Term ExprPrime
ExprPrime -> + Term ExprPrime | - Term ExprPrime | ε
Term -> id | number
Cond -> Expr RelOp Expr
RelOp -> > | < | == | !=
*/

//*********************** DEFINING GRAMMAR *********************//
void createManualCFG(CFG& cfg) {

    // Non-terminals
    State S("S", "start-state");
    State StmtList("StmtList", "non-terminal");
    State Stmt("Stmt", "non-terminal");
    State Expr("Expr", "non-terminal");
    State ExprPrime("ExprPrime", "non-terminal");
    State Term("Term", "non-terminal");
    State Cond("Cond", "non-terminal");
    State RelOp("RelOp", "non-terminal");
    State epsilon("epsilon", "terminal"); // Epsilon production

    // Terminals
    State id("id", "terminal");
    State number("number", "terminal");
    State assign("=", "terminal");
    State plus("+", "terminal");
    State minus("-", "terminal");
    State semicolon(";", "terminal");
    State if_kw("if", "terminal");
    State lparen("(", "terminal");
    State rparen(")", "terminal");
    State lbrace("{", "terminal");
    State rbrace("}", "terminal");
    State greater(">", "terminal");
    State less("<", "terminal");
    State eqeq("==", "terminal");
    State noteq("!=", "terminal");

    // Production Rules
    ProductionRule rule1(S, {StmtList});
    ProductionRule rule2(StmtList, {Stmt, StmtList});
    ProductionRule rule3(StmtList, {epsilon});
    ProductionRule rule4(Stmt, {id, assign, Expr, semicolon});
    ProductionRule rule5(Stmt, {if_kw, lparen, Cond, rparen, lbrace, StmtList, rbrace});
    ProductionRule rule6(Expr, {Term, ExprPrime});
    ProductionRule rule7(ExprPrime, {plus, Term, ExprPrime});
    ProductionRule rule8(ExprPrime, {minus, Term, ExprPrime});
    ProductionRule rule9(ExprPrime, {epsilon});
    ProductionRule rule10(Term, {id});
    ProductionRule rule11(Term, {number});
    ProductionRule rule12(Cond, {Expr, RelOp, Expr});
    ProductionRule rule13(RelOp, {greater});
    ProductionRule rule14(RelOp, {less});
    ProductionRule rule15(RelOp, {eqeq});
    ProductionRule rule16(RelOp, {noteq});

    // Start symbol
    cfg = CFG(S);

    // Add non-terminals
    cfg.addNonTerminal(StmtList);
    cfg.addNonTerminal(Stmt);
    cfg.addNonTerminal(Expr);
    cfg.addNonTerminal(ExprPrime);
    cfg.addNonTerminal(Term);
    cfg.addNonTerminal(Cond);
    cfg.addNonTerminal(RelOp);

    // Add terminals
    cfg.addTerminal(id);
    cfg.addTerminal(number);
    cfg.addTerminal(assign);
    cfg.addTerminal(plus);
    cfg.addTerminal(minus);
    cfg.addTerminal(semicolon);
    cfg.addTerminal(if_kw);
    cfg.addTerminal(lparen);
    cfg.addTerminal(rparen);
    cfg.addTerminal(lbrace);
    cfg.addTerminal(rbrace);
    cfg.addTerminal(greater);
    cfg.addTerminal(less);
    cfg.addTerminal(eqeq);
    cfg.addTerminal(noteq);

    // Add production rules
    cfg.addProductionRule(rule1);
    cfg.addProductionRule(rule2);
    cfg.addProductionRule(rule3);
    cfg.addProductionRule(rule4);
    cfg.addProductionRule(rule5);
    cfg.addProductionRule(rule6);
    cfg.addProductionRule(rule7);
    cfg.addProductionRule(rule8);
    cfg.addProductionRule(rule9);
    cfg.addProductionRule(rule10);
    cfg.addProductionRule(rule11);
    cfg.addProductionRule(rule12);
    cfg.addProductionRule(rule13);
    cfg.addProductionRule(rule14);
    cfg.addProductionRule(rule15);
    cfg.addProductionRule(rule16);
}

// Function to initialize the parsing table manually
void initializeParsingTable(ParsingTable& table) {
    // Manually add entries to the parsing table for your grammar
    
    // S -> StmtList
    table.addEntry("S", "id", "StmtList");
    table.addEntry("S", "if", "StmtList");

    // StmtList -> Stmt StmtList
    table.addEntry("StmtList", "id", "Stmt StmtList");
    table.addEntry("StmtList", "if", "Stmt StmtList");

    // StmtList -> epsilon
    table.addEntry("StmtList", ";", "epsilon");
    table.addEntry("StmtList", "}", "epsilon");

    // Stmt -> id = Expr ;
    table.addEntry("Stmt", "id", "id = Expr ;");

    // Stmt -> if ( Cond ) { StmtList }
    table.addEntry("Stmt", "if", "if ( Cond ) { StmtList }");

    // Expr -> Term ExprPrime
    table.addEntry("Expr", "id", "Term ExprPrime");
    table.addEntry("Expr", "number", "Term ExprPrime");

    // ExprPrime -> + Term ExprPrime
    table.addEntry("ExprPrime", "+", "+ Term ExprPrime");

    // ExprPrime -> - Term ExprPrime
    table.addEntry("ExprPrime", "-", "- Term ExprPrime");

    // ExprPrime -> epsilon
    table.addEntry("ExprPrime", "id", "epsilon");
    table.addEntry("ExprPrime", "number", "epsilon");

    // Term -> id
    table.addEntry("Term", "id", "id");

    // Term -> number
    table.addEntry("Term", "number", "number");

    // Cond -> id ExprPrime RelOp Expr
    table.addEntry("Cond", "id", "id ExprPrime RelOp Expr");

    // Cond -> number ExprPrime RelOp Expr
    table.addEntry("Cond", "number", "number ExprPrime RelOp Expr");

    // RelOp -> >
    table.addEntry("RelOp", ">", ">");

    // RelOp -> <
    table.addEntry("RelOp", "<", "<");

    // RelOp -> ==
    table.addEntry("RelOp", "==", "==");

    // RelOp -> !=
    table.addEntry("RelOp", "!=", "!=");
}

#endif // CFGMANUAL_H
// CFGMANUAL_H