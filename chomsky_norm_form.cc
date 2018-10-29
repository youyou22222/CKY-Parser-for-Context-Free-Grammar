#include"chomsky_norm_form.h"
void Grammar::build_cfg(std::string cfg_str, std::string production, std::string sep){
    std::regex re_newline("\n+\\s*");
    std::vector<std::string> prod;
    split(cfg_str, prod, re_newline);
    for(std::vector<std::string>::iterator it = prod.begin(); it != prod.end(); ++it){
        std::vector<std::string> tmp;
        std::regex re2("\\s*->\\s*");
        split(*it, tmp, re2);
        assert(tmp.size() == 2);

        std::string head = tmp[0];
        std::string symbol_str = tmp[1];
        std::vector<std::string> symbols;
        split(symbol_str, symbols, std::regex("\\s*\\|\\s*"));
        for(auto symbol : symbols){
            std::vector<std::string> sym;
            split(symbol, sym, std::regex("\\s+"));
            cfg.insert(CfgRule(head, sym));
        }
    }
}

void Grammar::add_cfg_rule(CfgRule r){
    cfg.insert(r);
}

void Grammar::add_cfg_rule(std::string head, std::vector<std::string> symbols){
    cfg.insert(CfgRule(head, symbols));
}

bool Grammar::is_head_symbol(const std::string &sym){
    for(auto it = cnf.begin(); it != cnf.end(); ++it){
        if(sym.compare(it ->head) == 0)
            return true;
    }
    return false;
}

void Grammar::convert_to_cnf(){
    cnf= cfg;
    std::unordered_set<std::string>nonterminals = get_nontermianls();

    //remove unary rules like "A -> B"

    auto unary_rules = find_unary_rule(nonterminals);

    for(std::unordered_set<CfgRule>::iterator it = unary_rules.begin(); 
          it != unary_rules.end(); ++it){
        std::string symbol = it ->symbols[0];
        cnf.erase(*it);

        auto head_by_symbol_rules = rules_head_by(symbol);

        for(auto it2 = head_by_symbol_rules.begin(); 
                it2 != head_by_symbol_rules.end(); ++it2){
            cnf.insert(CfgRule(it ->head, it2 ->symbols));
        }
        // rules not like "C -> aBa"
      //  auto rules_not = rule_not_tail_by(symbol); 

        bool rules_not = rule_not_tail_by(symbol);
        if(rules_not){
            for(auto it2 = head_by_symbol_rules.begin();
                    it2 != head_by_symbol_rules.end(); ++it2)
                cnf.erase(*it2);
        }

    }

   // move termianls to their own rules
   while(true){
   	   bool flag = false;
	   for(auto it = cnf.begin(); it != cnf.end(); ++it){
			if(it ->symbols.size() < 2){
				continue;
			}	
			std::unordered_set<CfgRule> tmp;
			auto symbols = it ->symbols;
			for(int i = 0; i < symbols.size(); ++i){
				if(!is_head_symbol(symbols[i])){
					std::string sym = symbols[i];
					std::string new_head = to_upper(sym);
					tmp.insert(it, CfgRule(new_head, {sym}));
					symbols[i] = new_head;
					flag = true;
				}
			}

			if(flag){
				std::string head = it ->head;
				cnf.erase(it);
				cnf.insert(tmp.begin(), tmp.end());
				cnf.insert(CfgRule(head, symbols));
				break;
			}
	   }
	   if(!flag) break;
   }

	
   // ensure there are only two non-terminals per rule
   while(true){
   	   bool flag = false;
	   for(auto it = cnf.begin(); it != cnf.end(); ++it){
			if(it ->symbols.size() <= 2){
				continue;
			}	

			flag = true;
			auto symbols = it ->symbols;
			//std::string new_head = symbols[symbols.size()-2] + "_" +  symbols[symbols.size()-1];
			std::string new_head = to_upper(symbols[0] + "_" +  symbols[1]);
			//std::vector<std::string> new_syms{symbols[symbols.size()-2],
			//									symbols[symbols.size()-1]};
			std::vector<std::string> new_syms{symbols[0],
												symbols[1]};
			cnf.insert(CfgRule(new_head, new_syms));
			auto it2 = symbols.begin();
			it2 += 2;
			
			std::vector<std::string> syms(it2, symbols.end());
			syms.insert(syms.begin(), new_head);
			cnf.insert(CfgRule(it ->head, syms));
			cnf.erase(it);
			break;
	   }
	   if(!flag) break;
   }
}
