#ifndef PROJECT_STOCK_H
#define PROJECT_STOCK_H

#include <list>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class Stock{
    string ticker;
    string name;
    string announcement_date;
    double surprise;
    map<string,double> price_lst;
    vector<double> AR;
public:
    Stock(string n, string t, string d, double s){
        name = n;
        ticker = t;
        announcement_date = d;
        surprise = s;
    }
    double get_surprise(){
        return surprise;
    }
    string get_ticker(){
        return ticker;
    }
    string get_date(){
        return announcement_date;
    }
    string get_name(){
        return name;
    }
    map<string,double> get_price_lst(){
        return price_lst;
    }
    void set_price_lst(map<string,double> lst){
        price_lst = lst;
    }
    vector<double> get_AR(){
        return AR;
    }
    void set_AR(vector<double> AR_){
        AR = AR_;
    }
};

#endif