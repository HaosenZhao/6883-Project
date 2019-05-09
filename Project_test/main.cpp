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
#include "StockDic.h"

using namespace std;


int main() {
    map<string, Stock> Stock_dic;
    if (read_surprise(Stock_dic))
        cout << "Read Surprise Finished" << endl;


    if (fillin_price(Stock_dic))
        cout << "Read Price Finished" << endl;

    vector<Stock *> Beat_group;
    vector<Stock *> Meet_group;
    vector<Stock *> Miss_group;
    for (auto it = Stock_dic.begin(); it != Stock_dic.end(); it++) {
        if (it->first == "SPY")
            continue;
        if (it->second.get_surprise() >= 5)
            Beat_group.push_back(&it->second);
        else if (it->second.get_surprise() <= -1)
            Miss_group.push_back(&it->second);
        else
            Meet_group.push_back(&it->second);
    }


    fillin_AR(Stock_dic);

    cout << "CAAR for Beat " <<  ave_CAAR(Beat_group,1000) << endl;
    cout << "CAAR for Meet " << ave_CAAR(Meet_group,1000) << endl;
    cout << "CAAR for Miss " << ave_CAAR(Miss_group,1000) << endl;

//    map<string,double> apple = Stock_dic.find("AAPL")->second.get_price_lst();
//    vector<double> AR = Stock_dic.find("AAPL")->second.get_AR();
//    auto ap_ptr = apple.begin();
//    ap_ptr++;
//    for(auto i = AR.begin(); i != AR.end(); i++) {
//        cout << ap_ptr->first <<" "<< *i << endl;
//        ap_ptr++;
//    }

    return 0;
}