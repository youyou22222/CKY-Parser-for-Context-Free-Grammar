#include <string>
#include <vector>

class Rule{ // 2st order production rule

public:

        Rule(std::string _l, std::string _r1, std::string _r2);

        Rule(std::string _l, std::string _r1);

        bool is_first_order(){
                return right2.length() == 0;
        }

        bool is_second_order(){
                return !is_first_order();
        }

        std::string left, right1, right2; // the two right terms
};

typedef std::vector<Rule*> RuleVector; //vector of rule **pointer** for dynamic type casting



