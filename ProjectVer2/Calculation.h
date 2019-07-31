#pragma once
#ifndef Calculation_h
#define Calculation_h
#include<vector>
#include"Stock.h"
#include<cstdlib>
#include<ctime>
using namespace std;

vector<Stock* > random_generator(vector<Stock *> group);

vector<double> AAR(vector<Stock *> selected);

vector<double> CAAR(vector<double> aar);

vector<double> averageCAAR(vector<Stock *> group);

vector<double> averageAAR(vector<Stock *> group);

#endif // !Calculation_h
