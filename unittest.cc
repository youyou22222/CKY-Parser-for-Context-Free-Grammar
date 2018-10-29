#include<iostream>
#include<string>
#include<regex>
#include<cassert>
#include"chomsky_norm_form.h"

int main(int argc, char **argv){

    
    std::string cfg_s = "S -> NP VP | Aux NP VP | VP\n"
                         "NP -> Pronoun | Proper-Noun | Det Nominal\n" 
                         "Nominal -> Noun | Nominal apple Noun book | Nominal PP\n" 
                         "VP -> Verb | Verb finsh NP | Verb NP PP | Verb PP | VP PP\n" 
                         "PP -> Preposition NP\n" 
                         "Det -> that | this | a\n" 
                         "Noun -> book | flight | meal | money\n" 
                         "Verb -> book | include | prefer\n" 
                         "Pronoun -> I | she | me\n" 
                         "Proper-Noun -> Houston | TWA\n" 
                         "Aux -> does\n" 
                         "Preposition -> from | to | on | near | through"; 

    Grammar gram(cfg_s);
    
    gram.convert_to_cnf();
    gram.print("cnfn");

    return 0;
}


