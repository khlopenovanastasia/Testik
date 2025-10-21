#include "headers.h"

// спрос на жильё
double GetHouseDemand(int month) {
	month %= 12;
	//1-Jan 2-Feb 3-Mar 4-Apr 5-May 6-Jun 7-Jul 8-Aug 9-Sep 10-Okt 11-Nov 0-Dec

	if (month < 3)
	{
		return 0.8;
	}
	else if (month > 2 and month < 6) {
		return 1.1;
	}
	else if (month > 5 and month < 9) {
		return 1.3;
	}
	else {
		return 1.5;
	}
}

//коэфициент продаж в суппермаркете
double GetSupermarketSales(int month) {
	month %= 12;
	//1-Jan 2-Feb 3-Mar 4-Apr 5-May 6-Jun 7-Jul 8-Aug 9-Sep 10-Okt 11-Nov 0-Dec

	if (month < 3)
	{
		return 1.4;
	}
	else if (month > 2 and month < 6) {
		return 1.1;
	}
	else if (month > 5 and month < 9) {
		return 0.9;
	}
	else {
		return 1.2;
	}
}
