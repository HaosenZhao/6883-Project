#define _CRT_SECURE_NO_WARNINGS
#include "curl/curl.h"
#include "Stock.h"
#include "StockDic.h"
#include "Calculation.h"
#include "gnuplot.h"
#include "Matrix.h"

using namespace std;

int main() {
	srand(time(NULL));
	StockDic sp500;
	vector<double> beatCAAR;
	vector<double> meetCAAR;
	vector<double> missCAAR;
	vector<Stock *> Beat_group;
	vector<Stock *> Meet_group;
	vector<Stock *> Miss_group;
	vector<Stock *> Select;
	vector<double> temp;
	map<string, Stock> Stock_dic;
	map<string, double> price_list;
	int k = 1;
	string ticker;
	int option =0, option2 = 0, option3 = 0, option4 = 0;
	if (sp500.read_surprise())
		cout << "Read Surprise Finished" << endl;
	if (sp500.fillin_price())
		cout << "Read Price Finished" << endl;
	sp500.fillin_AR();
	Stock_dic = sp500.getDic();
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
	beatCAAR = averageCAAR(Beat_group);
	meetCAAR = averageCAAR(Meet_group);
	missCAAR = averageCAAR(Miss_group);
	cout << "CAAR for Beat " << beatCAAR << endl;
	cout << "CAAR for Meet " << meetCAAR << endl;
	cout << "CAAR for Miss " << missCAAR << endl;
	plotResults(missCAAR, meetCAAR, beatCAAR);
	while (k) {
		cout << "Please choose from the following tasks (You must choose 1 first to enable other tasks.) " << endl;
		cout << "1. Retrieve historical price data for all stocks." << endl;
		cout << "2. Obtain one stock information from one group." << endl;
		cout << "3. Show AAR or CAAR from one group." << endl;
		cout << "4. Show the Excel graph with CAAR for all 3 groups." << endl;
		cout << "5. Exit." << endl;
		cin >> option;
		if (option == 1) {  //Retrieve historical price data for all stocks
			if (sp500.read_surprise())
				cout << "Read Surprise Finished" << endl;
			if (sp500.fillin_price())
				cout << "Read Price Finished" << endl;
			sp500.fillin_AR();
			Stock_dic = sp500.getDic();
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
		}
		else if (option == 2) {  // Obtain one stock information from one group
			cout << "Please choose one group from beat, meet and miss:" << endl;
			cout << "Input 1: beat   Input 2: meet   Input 3: miss" << endl;
			cin >> option2;
			if (option2 == 1) Select = Beat_group;
			else if (option2 == 2) Select = Meet_group;
			else if (option2 == 3) Select = Miss_group;
			else {
				cout << "Invalid Input" << endl;
				cin.clear();
				cin.ignore();
				continue;
			}
			for (int i = 0; i != Select.size(); i++) {
				cout << Select[i]->get_ticker() << "    ";
			}
			cout << endl;
			cout << "Please choose one stock ticker from above" << endl;
			cin >> ticker;
			for (int i = 0; i != Select.size(); i++) {
				if (ticker == Select[i]->get_ticker()) {
					cout << "Company Name: " << (*Select[i]).get_name() << endl;
					cout << "Announcement Date: " << Select[i]->get_date() << endl;
					cout << "Surprise: " << Select[i]->get_surprise() << endl;
					cout << "Price Range from -60 to 60 trading days of announcement: " << endl;
					price_list = Select[i]->get_price_lst();
					for (auto itr = price_list.begin(); itr != price_list.end(); itr++) {
						cout << itr->first << "    " << itr->second << endl;
					}
				}
			}
		}
		else if (option == 3) {    //Show AAR or CAAR from one group
			cout << "Please choose one group from beat, meet and miss:" << endl;
			cout << "Input 1: beat   Input 2: meet   Input 3: miss" << endl;
			cin >> option3;
			if (option3 == 1) Select = Beat_group;
			else if (option3 == 2) Select = Meet_group;
			else if (option3 == 3) Select = Miss_group;
			else {
				cout << "Invalid Input" << endl;
				cin.clear();
				cin.ignore();
				break;
			}
			cout << "Please choose from AAR and CAAR :" << endl;
			cout << "Input 1: AAR    Input 2: CAAR" << endl;
			cin >> option4;
			if (option4 == 1) {
				temp = averageAAR(Select);
				cout << temp << endl;
			}
			else if (option4 == 2) {
				temp = averageCAAR(Select);
				cout << temp << endl;
			}
			else {
				cout << "Invalid Input" << endl;
				cin.clear();
				cin.ignore();
			}
		}
		else if (option == 4) {
            beatCAAR = averageCAAR(Beat_group);
            meetCAAR = averageCAAR(Meet_group);
            missCAAR = averageCAAR(Miss_group);
            //cout << "CAAR for Beat " << beatCAAR << endl;
            //cout << "CAAR for Meet " << meetCAAR << endl;
            //cout << "CAAR for Miss " << missCAAR << endl;
            plotResults(missCAAR, meetCAAR, beatCAAR);
		}
		else if (option == 5) {
			k = 0;
		}
		else {
			cout << "Invalid Input" << endl;
			cin.clear();
			cin.ignore();
		}
	}

    return 0;
}