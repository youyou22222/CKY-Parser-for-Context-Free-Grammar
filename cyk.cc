#include "cyk.h"

/*
  Given the backpointer, return the possible parse trees
 */
std::vector<BTreeNode*> reconstruct(std::vector<std::vector<BPTableEntry>> bp, int i, int j, std::string symbol){
        /*
          bp: the backpointer table
         */

        if(bp.at(i).at(j)[symbol].size() == 1 && bp.at(i).at(j)[symbol][0]->is_single()){ //the leaf
                BPSingleEntry *e = (BPSingleEntry *)bp.at(i).at(j)[symbol][0];
                BTreeNode *t = new BTreeNode {symbol,
                                              new BTreeNode(e->name, NULL, NULL),
                                              NULL};
                return std::vector<BTreeNode*> {t};
        }
        else{ //internal node
                std::vector<BTreeNode*> trees = std::vector<BTreeNode*> {};

                for(auto temp : bp.at(i).at(j)[symbol]){
                        BPTupleEntry *e = (BPTupleEntry *)temp;
                        for(auto ltree : reconstruct(bp, e->l1, e->l2, e->lname)){
                                for(auto rtree : reconstruct(bp, e->r1, e->r2, e->rname)){
                                        trees.push_back(new BTreeNode(symbol, ltree, rtree));
                                }
                        }
                }
                return trees;
        }
}

std::vector<BTreeNode*> cyk(const StringVector &  words, RuleVector & rules){
        // build indexed rules
        IndexedRules indexed_rules;

        for(auto i = 0; i < rules.size(); ++i){
                IndexedRulesKey key = {rules[i]->right1, rules[i]->right2};
                if(indexed_rules.find(key)  == indexed_rules.end()){ // key not exist
                        indexed_rules[key] = StringVector {};
                }
                indexed_rules[key].push_back(rules[i]->left);
        }


        // Create the 2D dynamic programming table in which each cell contains a vector of produced symbol(string)
        // The table dimension should be N x N, where N is the word count
        std::vector<std::vector<StringVector>> table;

        // The backpointer
        std::vector<std::vector<BPTableEntry>> bp;


        //init the containers
        int N = words.size()+1;
        for(int i = 0; i < N; ++i){
                table.push_back(std::vector<StringVector> {});
                bp.push_back(std::vector<BPTableEntry> {});
                for(int j=0; j < N; ++j){
                        table.at(i).push_back(StringVector {});
                        bp.at(i).push_back(BPTableEntry {});
                }
        }

        //start filling in the cell contents
        for(int j=1; j < N; ++j){

                std::string word = words.at(j-1);
                IndexedRulesKey key = {word, ""};
                StringVector matching_symbols = indexed_rules[key];

                for(auto &A_symbol : matching_symbols){
                        table.at(j-1).at(j).push_back(A_symbol);

                        if(bp.at(j-1).at(j).find(A_symbol) == bp.at(j-1).at(j).end()){ //if key does not exist, create one
                                bp.at(j-1).at(j)[A_symbol] = std::vector<BPTableEntryItem*> {};
                        }
                        bp.at(j-1).at(j)[A_symbol].push_back(new BPSingleEntry(word));
                }

                for(int i=j-2; i>=0; --i){
                        for(int k = i+1; k < j; ++k){
                                if(table.at(i).at(k).size() > 0 &&  table.at(k).at(j).size() > 0){ //if table entry is not empty
                                        StringVector B_symbols = table.at(i).at(k);
                                        StringVector C_symbols = table.at(k).at(j);

                                        for(auto B_symbol : B_symbols){
                                                for(auto C_symbol : C_symbols){
                                                        IndexedRulesKey key = {B_symbol, C_symbol};
                                                        StringVector matching_symbols = indexed_rules[key];
                                                        for(auto A_symbol : matching_symbols) {
                                                                table.at(i).at(j).push_back(A_symbol);
                                                                if(bp.at(i).at(j).find(A_symbol) == bp.at(i).at(j).end()){ //if key ont exist, create one
                                                                        bp.at(i).at(j)[A_symbol] = std::vector<BPTableEntryItem*> {};
                                                                }
                                                                bp.at(i).at(j)[A_symbol].push_back(new BPTupleEntry(i, k, B_symbol, k, j, C_symbol));
                                                        }
                                                }
                                        }
                                }
                        }
                }
        }
        return reconstruct(bp, 0, N-1, "S");
}
