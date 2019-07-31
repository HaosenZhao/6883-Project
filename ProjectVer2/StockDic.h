#ifndef PROJECTVER1_STOCKDIC_H
#define PROJECTVER1_STOCKDIC_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <iterator>
#include <vector>
#include <list>
#include <iomanip>
#include <locale>
#include <algorithm>
#include <random>
#include "curl/curl.h"
#include "Stock.h"

using namespace std;
class StockDic{
    map<string, Stock> Stock_dic;
public:
    bool read_surprise();
    bool fillin_price();
    void fillin_AR();
    map<string, Stock> getDic(){
        return Stock_dic;
    }

};
#endif //PROJECTVER1_STOCKDIC_H
