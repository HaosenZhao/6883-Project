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

map<string, Stock> Stock_dic;

struct MemoryStruct {
    char *memory;
    size_t size;
};

void *myrealloc(void *ptr, size_t size) {
    if (ptr)
        return realloc(ptr, size);
    else
        return malloc(size);
}

int write_data(void *ptr, size_t size, size_t nmemb, void *data) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *) data;

    mem->memory = (char *) myrealloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory) {
        memcpy(&(mem->memory[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
    }
    return realsize;
}

string getTimeinSeconds(string Time) {
    std::tm t = {0};
    std::istringstream ssTime(Time);
    char time[100];
    memset(time, 0, 100);
    if (ssTime >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S")) {
//        cout << std::put_time(&t, "%c %Z") << "\n"
//             << std::mktime(&t) << "\n";
        sprintf(time, "%lld", mktime(&t));
        return string(time);
    } else {
        cout << "Parse failed\n";
        return "";
    }
}

pair<string, string> getStart_End(string a_date) {
    string start_time, end_time, str;
    stringstream ss(a_date);
    vector<string> results;
    while (getline(ss, str, '-')) {
        results.push_back(str);
    }
    int year = stoi(results[0]);
    int month = stoi(results[1]);
    string day = results[2];
    if (month + 4 > 12)
        end_time = getTimeinSeconds(to_string(year + 1) + "-0" + to_string(month - 8) + "-" + day + "T16:00:00");
    else {
        if (month + 4 < 10)
            end_time = getTimeinSeconds(to_string(year) + "-0" + to_string(month + 4) + "-" + day + "T16:00:00");
        else
            end_time = getTimeinSeconds(to_string(year) + "-" + to_string(month + 4) + "-" + day + "T16:00:00");
    }
    if (month - 4 <= 0)
        start_time = getTimeinSeconds(to_string(year - 1) + "-" + to_string(month + 8) + "-" + day + "T16:00:00");
    else
        start_time = getTimeinSeconds(to_string(year) + "-0" + to_string(month - 4) + "-" + day + "T16:00:00");
    return pair<string, string>(start_time, end_time);
}


bool read_surprise() {
    // -----------------Read Bloomberg Surprise Data--------------------
    ifstream surpirse_csv("announcements.csv");
    string line;
    if (surpirse_csv.is_open()) {
        getline(surpirse_csv, line);
        while (getline(surpirse_csv, line)) {
            string str;
            stringstream ss(line);
            vector<string> results;
            while (getline(ss, str, ',')) {
                results.push_back(str);
            }
            if (results[1] == "BF/B" || results[1] == "BRK/A")
                continue;
            Stock s(results[0], results[1], results[2], stod(results[3]));
            Stock_dic.insert(pair<string, Stock>(results[1], s));
        }
    } else {
        cerr << "Read csv error" << endl;
        return false;
    }
    Stock sp("S&P 500", "SPY", "2018-05-01", 0.0);
    Stock_dic.insert(pair<string, Stock>("SPY", sp));
    // -----------------Read Bloomberg Surprise Data--------------------
    return true;
}

bool fillin_price() {
    map<string, Stock>::iterator itr = Stock_dic.begin();

    struct MemoryStruct data;
    data.memory = NULL;
    data.size = 0;

    // file pointer to create file that store the data
    FILE *fp;

    // declaration of an object CURL
    CURL *handle;

    CURLcode result;

    // set up the program environment that libcurl needs
    curl_global_init(CURL_GLOBAL_ALL);

    // curl_easy_init() returns a CURL easy handle
    handle = curl_easy_init();

    // if everything's all right with the easy handle...
    if (handle) {
        int counter = 0;
        string sCookies, sCrumb;
        //----------------------------Get Crumb-----------------------
        if (sCookies.length() == 0 || sCrumb.length() == 0) {
            curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
            curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
            curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *) &data);
            // perform, then store the expected code in result
            result = curl_easy_perform(handle);
            if (result != CURLE_OK) {
                // if errors have occured, tell us what is wrong with result
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return false;
            }

            char cKey[] = "CrumbStore\":{\"crumb\":\"";
            char *ptr1 = strstr(data.memory, cKey);
            char *ptr2 = ptr1 + strlen(cKey);
            char *ptr3 = strstr(ptr2, "\"}");
            if (ptr3 != NULL)
                *ptr3 = NULL;
            sCrumb = ptr2;

            fp = fopen("cookies.txt", "r");
            char cCookies[100];
            if (fp) {
                while (fscanf(fp, "%s", cCookies) != EOF);
                fclose(fp);
            } else
                cerr << "cookies.txt does not exists" << endl;

            sCookies = cCookies;
            //读完了关于scrum的数据，free heap
            free(data.memory);
            data.memory = NULL;
            data.size = 0;
        }
        //----------------------------Get Crumb-----------------------
        cout << "Retrieving Price" << endl;
        cout << "*************************************************" << endl;
        while (true) {
            //break condition
            if (itr == Stock_dic.end())
                break;
            //----------------------------Get Price_lst----------------------
            string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
            string symbol = itr->first;
            string urlB = "?period1=";
            string urlC = "&period2=";
            string urlD = "&interval=1d&events=history&crumb=";
            pair<string, string> time = getStart_End(itr->second.get_date());
            string startTime, endTime;
            if (itr->first == "SPY") {
                startTime = getTimeinSeconds("2017-03-01T16:00:00");
                endTime = getTimeinSeconds("2019-04-01T16:00:00");
            } else {
                startTime = time.first;
                endTime = time.second;
            }
            string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
            const char *cURL = url.c_str();
            const char *cookies = sCookies.c_str();
            curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
            curl_easy_setopt(handle, CURLOPT_URL, cURL);
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *) &data);
            result = curl_easy_perform(handle);

            if (result != CURLE_OK) {
                // if errors have occurred, tell us what is wrong with result
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return false;
            }
            //----------------------------Get Price_lst----------------------


            stringstream sData;
            sData.str(data.memory);
            string sValue, sDate;
            double dValue = 0;
            string line;
            getline(sData, line);
            if (line.length() == 0) {
                itr++;
                free(data.memory);
                data.memory = NULL;
                data.size = 0;
                continue;
            }
            map<string, double> price_lst;
            string error_code = "";
            while (getline(sData, line)) {
                sDate = line.substr(0, line.find_first_of(','));
                line.erase(line.find_last_of(','));
                sValue = line.substr(line.find_last_of(',') + 1);
                dValue = strtod(sValue.c_str(), NULL);
                price_lst.insert(pair<string, double>(sDate, dValue));
                if (dValue == 0) {
                    error_code = itr->first;
                }
            }
            if (itr->first == "SPY") {
                itr->second.set_price_lst(price_lst);
                itr++;
                free(data.memory);
                data.memory = NULL;
                data.size = 0;
                continue;
            }
            //choose first 60 and later 60
            auto front = price_lst.find(itr->second.get_date());
            auto after = price_lst.find(itr->second.get_date());
            for (int i = 0; i < 60; i++) {
                front--;
                after++;
            }
            after++;
            price_lst.erase(price_lst.begin(), front);
            price_lst.erase(after, price_lst.end());
            itr->second.set_price_lst(price_lst);
            counter++;
            if (counter % 10 == 0)
                cout << "*";
            itr++;
            free(data.memory);
            data.memory = NULL;
            data.size = 0;
            if (error_code != "")
                Stock_dic.erase(error_code);
        }
        cout << endl;
    } else {
        fprintf(stderr, "Curl init failed!\n");
        return false;
    }


    // cleanup since you've used curl_easy_init
    curl_easy_cleanup(handle);

    // release resources acquired by curl_global_init()
    curl_global_cleanup();
    return true;
}

void fillin_AR() {
    map<string, double> sp_price = Stock_dic.find("SPY")->second.get_price_lst();
    for (auto stocki = Stock_dic.begin(); stocki != Stock_dic.end(); stocki++) {
        map<string, double> this_price_lst = stocki->second.get_price_lst();
        if (stocki->first == "SPY")
            continue;
        vector<double> AR;
        string start_date = this_price_lst.begin()->first;
        auto sp_itr = sp_price.find(start_date);
        double ThistodayPrice, ThistomorrowPrice, SPtodayPrice, SPtomorrowPrice, ThisReturn, SPReturn;
        for (auto i = this_price_lst.begin(); i != this_price_lst.end();) {
            ThistodayPrice = i->second;
            i++;
            if (i == this_price_lst.end())
                break;
            ThistomorrowPrice = i->second;
            ThisReturn = (ThistomorrowPrice - ThistodayPrice) / ThistodayPrice;
            SPtodayPrice = sp_itr->second;
            sp_itr++;
            SPtomorrowPrice = sp_itr->second;
            SPReturn = (SPtomorrowPrice - SPtodayPrice) / SPtodayPrice;
            AR.push_back(ThisReturn - SPReturn);
        }
        stocki->second.set_AR(AR);
    }
    cout << "AR Calculated" << endl;
}


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
    if (read_surprise())
        cout << "Read Surprise Finished" << endl;


    if (fillin_price())
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


    fillin_AR();

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