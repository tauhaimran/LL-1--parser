#include <iostream>
#include <fstream>
#include "CFG.h"
#include "CFGReader.h"

int main() {

//*********************** DEFINING GRAMMAR *********************//
    // Non-terminals
    State Program("Program", "start-state");
    State Declarations("Declarations", "non-terminal");
    State Declaration("Declaration", "non-terminal");
    State Statements("Statements", "non-terminal");
    State Statement("Statement", "non-terminal");
    State Assignment("Assignment", "non-terminal");
    State IfStatement("IfStatement", "non-terminal");
    State Expression("Expression", "non-terminal");
    State ExpressionPrime("Expression'", "non-terminal");
    State Term("Term", "non-terminal");
    State Condition("Condition", "non-terminal");
    State RelOp("RelOp", "non-terminal");
    State ID("ID", "non-terminal");
    State NUMBER("NUMBER", "non-terminal");

    // Terminals
    State _int("int", "terminal");
    State equal("=", "terminal");
    State plus("+", "terminal");
    State minus("-", "terminal");
    State lparen("(", "terminal");
    State rparen(")", "terminal");
    State lbrace("{", "terminal");
    State rbrace("}", "terminal");
    State greater(">", "terminal");
    State less("<", "terminal");
    State eqeq("==", "terminal");
    State gte(">=", "terminal");
    State lte("<=", "terminal");
    State neq("!=", "terminal");
    State identifier("identifier", "terminal");
    State number("number", "terminal");
    State semicolon(";", "terminal");

    // Production Rules
    ProductionRule rule1(Program, { Declarations, Statements });

    ProductionRule rule2(Declarations, { Declaration, Declarations });
    ProductionRule rule3(Declarations, { /* epsilon */ });

    ProductionRule rule4(Declaration, { _int, ID, semicolon });

    ProductionRule rule5(Statements, { Statement, Statements });
    ProductionRule rule6(Statements, { /* epsilon */ });

    ProductionRule rule7(Statement, { Assignment, semicolon });
    ProductionRule rule8(Statement, { IfStatement });

    ProductionRule rule9(Assignment, { ID, equal, Expression });

    ProductionRule rule10(IfStatement, { _int, lparen, Condition, rparen, lbrace, Statements, rbrace });

    ProductionRule rule11(Expression, { Term, ExpressionPrime });

    ProductionRule rule12(ExpressionPrime, { plus, Term, ExpressionPrime });
    ProductionRule rule13(ExpressionPrime, { minus, Term, ExpressionPrime });
    ProductionRule rule14(ExpressionPrime, { /* epsilon */ });

    ProductionRule rule15(Term, { NUMBER });
    ProductionRule rule16(Term, { ID });

    ProductionRule rule17(Condition, { Expression, RelOp, Expression });

    ProductionRule rule18(RelOp, { greater });
    ProductionRule rule19(RelOp, { less });
    ProductionRule rule20(RelOp, { eqeq });
    ProductionRule rule21(RelOp, { gte });
    ProductionRule rule22(RelOp, { lte });
    ProductionRule rule23(RelOp, { neq });


        CFG cfg(Program); // Start symbol

    // Add non-terminals
    cfg.addNonTerminal(Declarations);
    cfg.addNonTerminal(Declaration);
    cfg.addNonTerminal(Statements);
    cfg.addNonTerminal(Statement);
    cfg.addNonTerminal(Assignment);
    cfg.addNonTerminal(IfStatement);
    cfg.addNonTerminal(Expression);
    cfg.addNonTerminal(ExpressionPrime);
    cfg.addNonTerminal(Term);
    cfg.addNonTerminal(Condition);
    cfg.addNonTerminal(RelOp);
    cfg.addNonTerminal(ID);
    cfg.addNonTerminal(NUMBER);

    // Add terminals
    cfg.addTerminal(_int);
    cfg.addTerminal(equal);
    cfg.addTerminal(plus);
    cfg.addTerminal(minus);
    cfg.addTerminal(lparen);
    cfg.addTerminal(rparen);
    cfg.addTerminal(lbrace);
    cfg.addTerminal(rbrace);
    cfg.addTerminal(greater);
    cfg.addTerminal(less);
    cfg.addTerminal(eqeq);
    cfg.addTerminal(gte);
    cfg.addTerminal(lte);
    cfg.addTerminal(neq);
    cfg.addTerminal(identifier);
    cfg.addTerminal(number);
    cfg.addTerminal(semicolon);

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
    cfg.addProductionRule(rule17);
    cfg.addProductionRule(rule18);
    cfg.addProductionRule(rule19);
    cfg.addProductionRule(rule20);
    cfg.addProductionRule(rule21);
    cfg.addProductionRule(rule22);
    cfg.addProductionRule(rule23);


    //testing cfg
    //cfg.display();
    cfg.leftFactoring();
    //cfg.eliminateLeftRecursion();
    cfg.display();

    return 0;
}