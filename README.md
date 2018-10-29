# CKY-Parsing-for-Context-Free-Grammar

A CKY Parsing Algorithm for Context Free Grammar. Specifically, the Context Free Grammar is first to be converted to Chomsky Norm Form
and then Apply CKY Algotrithm to the input sentence. 
The result is synstactic tree structure of the input sentence.

Examples:

```
#include "cyk.h"
#include"chomsky_norm_form.h"
#include <iostream>

int main(){
       std::string cfg_s = "S -> NP VP | Aux NP VP | VP\n"
                         "NP -> Pronoun | Proper-Noun | Det Nominal\n" 
                         "Nominal -> Noun | Nominal apple Noun book | Nominal PP\n" 
                         "VP -> Verb | Verb finsh NP | Verb NP PP | Verb PP | VP PP\n" 
                         "PP -> Preposition NP\n" 
                         "Det -> that | this | a | the\n" 
                         "Noun -> book | flight | meal | money\n" 
                         "Verb -> book | include | prefer\n" 
                         "Pronoun -> I | she | me\n" 
                         "Proper-Noun -> Houston | TWA\n" 
                         "Aux -> does\n" 
                         "Preposition -> from | to | on | near | through"; 
        
        //Buld context free grammar from strings
        Grammar grammar(cfg_s);
        // convert context free grammar to Chomsky Norm Form
        grammar.convert_to_cnf();
        auto cnf = grammar.get_cnf();

        RuleVector rules;
        for(auto it = cnf.begin(); it != cnf.end(); ++it){

            if(it ->symbols.size() == 2){

                rules.push_back(new Rule(it ->head,it ->symbols[0], it->symbols[1]));
            }else{
                rules.push_back(new Rule(it ->head,it ->symbols[0]));
            }
        }
        
        // input sentence "book the flight through Houston"
        StringVector sents {"book", "the", "flight", "through", "Houston"};
        //apply cyk algorithm
        std::vector<BTreeNode*> parses = cyk(sents, rules);
        // print
        for(auto tree : parses){
                std::cout << tree->string_repr() << std::endl;
        }

        //remember to destroy the rules
        for(auto rule : rules){
            if(rule)
                delete rule;
        }
        return 0;

```


Compile:  1. make  
Excute:   2. ./main   
Out:      3. ('S', '('VERB_NP', '('Verb', ''book'')', '('NP', '('Det', ''the'')', '('Nominal', ''flight'')')')', '('PP', '('Preposition', ''through'')', '('NP', ''Houston'')')')

