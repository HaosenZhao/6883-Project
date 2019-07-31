• Use liburl to retrieve historical price data from Yahoo Finance: A function retrieves the adjusted close prices for selected S & P 500 stocks and SPY into memory.
• Create a set of classes such as class for stock to handle EPS estimate and price information.
• Use member functions or independent functions for all calculation. Overload a few arithmetic operators for vector/matrix.
• The stocks and their corresponding price information for each group are stored in a STL map, with stock symbol as its keys.
• The AAR and CAAR for 3 groups are presented in a matrix. The row of the matrix isthe group#, matrix columns are for AAR and CAAR.
• Excel Driver or gnuplot to show the CAAR from all 3 groups in one graph.

What program can do
Retrieve historical price data for all selected stocks. Parse the retrieved data for dates and adjusted closing prices.
o Calculate AAR and CAAR for each group
o Populate the stock maps and AAR/CAAR matrix.
o Show an Excel graph with CAAR for all 3 groups.

Menu Options
o Retrieve historical price data for all stocks.
o Pull information for one stock from one group.
o Show AAR or CAAR for one group.
o Show the Excel graph with CAAR for all 3 groups. 
o Exit your program.
