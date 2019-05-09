#include <iostream>
#include <string>
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

vector<int> random_generator(int size) {
    vector<int> a;
    for (int i = 0; i < size; i++)
        a.push_back(i);
    srand(NULL);
    random_shuffle(a.begin(),a.end());
    vector<int> top_30(a.begin(), a.begin() + 30);
    return top_30;
}

double CAAR(vector<Stock *> group) {
    vector<Stock *> selected;
    vector<int> selected_index = random_generator(group.size());
    for (int i = 0; i < selected_index.size(); i++) {
        selected.push_back(group[selected_index[i]]);
    }
    vector<double> AAR;
    for (int t = 0; t < 120; t++) {
        double counter = 0;
        for (int i = 0; i < selected.size(); i++) {
            vector<double> AR = selected[i]->get_AR();
            counter += AR[t];
        }
        counter /= selected.size();
        AAR.push_back(counter);
    }
    double counter = 0;
    for (int i = 0; i < AAR.size(); i++)
        counter += AAR[i];
    return counter;
}

double ave_CAAR(vector<Stock *> group, int times){
    double CAAR_total= 0;
    for (int i = 0; i < times; i++) {
        CAAR_total += CAAR(group);
    }
    return CAAR_total/times;
}

int main() {
    StockDic sp500;

    if (sp500.read_surprise())
        cout << "Read Surprise Finished" << endl;


    if (sp500.fillin_price())
        cout << "Read Price Finished" << endl;

    sp500.fillin_AR();

    map<string, Stock> Stock_dic = sp500.getDic();
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

    cout << "CAAR for Beat " <<  ave_CAAR(Beat_group,30) << endl;
    cout << "CAAR for Meet " << ave_CAAR(Meet_group,30) << endl;
    cout << "CAAR for Miss " << ave_CAAR(Miss_group,30) << endl;

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