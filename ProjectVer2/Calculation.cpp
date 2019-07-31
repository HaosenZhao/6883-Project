#include "Stock.h"
#include "StockDic.h"
#include "Matrix.h"
#include "Calculation.h"
using namespace std;

//Generate a set of 30 different random stocks  of each group
vector<Stock* > random_generator(vector<Stock *> group) {
	vector<int> a;
	int size = group.size();
	for (int i = 0; i < size; i++)
		a.push_back(i);
	random_shuffle(a.begin(), a.end());
    vector<int> top_30(a.begin(), a.begin() + 30);
	/*vector<int> top_30(30);
	int temp;
	for (int i = 0; i < 30; i++) {
		temp = rand() % (a.size()-i);
		top_30[i] = a[temp];
		a[temp] = a[a.size() - 1 - i];
	}*/
	
	//cout << top_30 << endl << endl;
	vector<Stock *> selected;
	for (int i = 0; i < top_30.size(); i++) {
		selected.push_back(group[top_30[i]]);
	}
	return selected;
}

//Calculate AAR using AR of selected stocks
vector<double> AAR(vector<Stock *> selected) {
	vector<double> aar(120);
	for (int t = 0; t < 120; t++) {
		aar[t] = 0.0;
	}
	for (int i = 0; i < selected.size(); i++){
		aar = aar + selected[i]->get_AR();
	}
    aar =  (1/(double)selected.size()) * aar;
	return aar;
}

//Calculate CAAR using AAR
vector<double> CAAR(vector<double> aar) {
	vector<double> caar(120);
	caar[0] = aar[0];
	for (int t = 1; t < 120; t++) {
		caar[t] = caar[t - 1] + aar[t];
	}
	return caar;
}

//Calculate average CAAR of 30 samples
vector<double> averageCAAR(vector<Stock *> group) {
	vector<double> AverCAAR(120);
	for (int t = 0; t < 120; t++) {
		AverCAAR[t] = 0.0;
	}
	for (int i = 0; i < 30; i++) {
		vector<Stock *> selected = random_generator(group);
		vector<double> aar = AAR(selected);
		vector<double> caar = CAAR(aar);
		AverCAAR = AverCAAR + caar;
	}
	AverCAAR = (1.0/30.0) * AverCAAR;
	return AverCAAR;
}

//Calculate average AAR of 30 samples
vector<double> averageAAR(vector<Stock *> group) {
	vector<double> AverAAR(120);
	for (int t = 0; t < 120; t++) {
		AverAAR[t] = 0.0;
	}
	for (int i = 0; i < 30; i++) {
		vector<Stock *> selected = random_generator(group);
		vector<double> aar = AAR(selected);
		AverAAR = AverAAR + aar;
	}
	AverAAR = (1.0 / 30.0) * AverAAR;
	return AverAAR;
}
