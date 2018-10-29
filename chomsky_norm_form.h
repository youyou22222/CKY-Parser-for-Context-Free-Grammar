#ifndef SYNTACTIC_PARSE_H
#define SYNTACTIC_PARSE_H
#include<unordered_set>
#include<string>
#include<functional>
#include<vector>
#include<iostream>
#include<regex>
#include<cassert>

void split(std::string inputs,  std::vector<std::string> &res, std::regex sep = std::regex("[\\s+]"));

std::string to_upper(const std::string &word);


class CfgRule{
    public:
        CfgRule(std::string head_, std::vector<std::string> symbols_)
            :head(head_),symbols(symbols_.begin(), symbols_.end()){}

        friend bool operator == (const CfgRule lrule, const CfgRule rrule){
            if(lrule.head == rrule.head && 
                    lrule.symbols.size() == rrule.symbols.size()){
                for(int i = 0; i < lrule.symbols.size(); ++i){
                    if(lrule.symbols[i].compare(rrule.symbols[i]) != 0)
                        return false;
                }

                return true;
            }

            return false;
        }
    public:
        std::string head;
        std::vector<std::string> symbols;
};

namespace std{
    template <>
    struct hash<CfgRule>{
        size_t operator()(const CfgRule &rule)const{
            size_t h = std::hash<std::string>{}(rule.head);
            for(int i = 0; i < rule.symbols.size(); ++i){
                size_t s = std::hash<std::string>{}(rule.symbols[i]);
                h ^= (s << 1);
            }
            return h;
        }
    };
}

class Grammar{

    public:
        Grammar(std::string cfg_str, std::string production ="->", std::string sep="|"){
           build_cfg(cfg_str, production, sep);
        }

        void add_cfg_rule(CfgRule);
        void add_cfg_rule(std::string head, std::vector<std::string> symbols);

        void build_cfg(std::string cfg_str, std::string production="->", std::string sep="|");
        void convert_to_cnf();

        
        // return grammar rule in chomsky norm form
        std::unordered_set<CfgRule> get_cnf(){
            return cnf;
        }
        void print(std::string cfgs){
			if(cfgs == "cfg"){
//            std::cout << "***********************context free grammar: *********************\n";
				for(auto it = cfg.begin(); it != cfg.end(); ++it){
					std::cout << it ->head << " -> ";
					int j = 0;
					for(j = 0; j < it ->symbols.size()-1; ++j){
						std::cout << it ->symbols[j] << " ";
					}
					std::cout << it ->symbols[j] << "\n";
				}
			}
            //std::cout << "*************************chomsky norm grammar: **********************:\n";
			else {
				 for(auto it = cnf.begin(); it != cnf.end(); ++it){
					std::cout << it ->head << " -> ";
					int j = 0;
					for(j = 0; j < it ->symbols.size()-1; ++j){
						std::cout << it ->symbols[j] << " ";
					}
					std::cout << it ->symbols[j] << "\n";
				 }
			}
		}


    private:
        std::unordered_set<std::string> get_nontermianls(){
            std::unordered_set<std::string> nonterminals;
            for(std::unordered_set<CfgRule>::iterator it = cnf.begin(); it != cnf.end(); ++it){
                nonterminals.insert((*it).head);
            }
            return nonterminals;
        }

        std::unordered_set<CfgRule> find_unary_rule(std::unordered_set<std::string> &nonterminals){
            std::unordered_set<CfgRule> res;
            for(auto it = cnf.begin(); it != cnf.end(); ++it){
                if(it ->symbols.size() == 1 && 
                        nonterminals.find( (it ->symbols)[0]) != nonterminals.end()){
                    //std::cout << "insert:" <<  it ->head << " -> " << it ->symbols[0] << "\n";
                    res.insert(*it);
                }
            }
            return res;
        }

        std::unordered_set<CfgRule> rules_head_by(std::string symbol){
            std::unordered_set<CfgRule> res;
            for(auto it = cnf.begin(); it != cnf.end(); ++it){
                if(symbol.compare(it ->head) == 0){
                    res.insert(*it);
                }
            }

            return res;
        }

        bool rule_not_tail_by(std::string symbol){
            for(auto it = cnf.begin(); it != cnf.end(); ++it){
                for(auto sym : it ->symbols){
                    if(symbol.compare(sym) == 0){
                        return false;
                    }
                }
            }
            return true;
        }

        bool is_head_symbol(const std::string &sym);

    private:
        std::unordered_set<CfgRule> cfg;
        std::unordered_set<CfgRule> cnf;
};

#endif
