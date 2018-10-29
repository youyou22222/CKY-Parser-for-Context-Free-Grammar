#include "rule.h"

Rule::Rule(std::string _l, std::string _r1, std::string _r2):
        left(_l), right1(_r1), right2(_r2){};

Rule::Rule(std::string _l, std::string _r1):
        left(_l), right1(_r1), right2(""){};
