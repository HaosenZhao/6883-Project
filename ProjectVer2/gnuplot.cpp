#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include"gnuplot.h"
using namespace std;

//Please input data in the sequence of miss, meet, beat
int plotResults(vector<double> yData1, vector<double> yData2, vector<double> yData3) {
	int dataSize = 120;
	FILE *gnuplotPipe, *tempDataFile;
	//String of legends
	const char *tempDataFileName1 = "Miss CAAR";
	const char *tempDataFileName2 = "Meet CAAR";
	const char *tempDataFileName3 = "Beat CAAR";
	double x, y, x2, y2, x3, y3;
	int i;
	//Check if the 3 data are of the same length
	if (sizeof(yData1) != sizeof(yData2) || sizeof(yData2) != sizeof(yData3)) {
		cout << "Error Input" << endl;
		return 1;
	}
	//gnuplot
	gnuplotPipe = popen("gnuplot -persist", "w");
	if (gnuplotPipe) {
		fprintf(gnuplotPipe, "plot \"%s\" with lines,\"%s\" with lines, \"%s\" with lines\n", tempDataFileName1, tempDataFileName2, tempDataFileName3);
		fflush(gnuplotPipe);
		tempDataFile = fopen(tempDataFileName1, "w");
		for (i = 0; i < dataSize; i++) {
			x = i - 60;
			y = yData1[i];
			fprintf(tempDataFile, "%lf %lf\n", x, y);
		}
		fclose(tempDataFile);
		tempDataFile = fopen(tempDataFileName2, "w");
		for (i = 0; i < dataSize; i++) {
			x2 = i - 60;
			y2 = yData2[i];
			fprintf(tempDataFile, "%lf %lf\n", x2, y2);
		}
		fclose(tempDataFile);
		tempDataFile = fopen(tempDataFileName3, "w");
		for (i = 0; i < dataSize; i++) {
			x3 = i - 60;
			y3 = yData3[i];
			fprintf(tempDataFile, "%lf %lf\n", x3, y3);
		}
		fclose(tempDataFile);
		printf("press enter to continue...\n");
		getchar();
//		remove(tempDataFileName1);
//		remove(tempDataFileName2);
//		remove(tempDataFileName3);
		fprintf(gnuplotPipe, "exit \n");
	}
	else {
		printf("gnuplot not found...");
	}
	return 0;
}