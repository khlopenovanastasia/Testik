//#pragma once
//#include "headers.h"
////#include "C:\Users\elina\OneDrive\ƒокументы\GitHub\Testik\Anastasia back\oop.cpp"
//
//map<Buildings*, int> IdBuildings;
//
//Buildings* CreateNewBuilding(int chose) { // в парметрах тип дома 1 - панель, 2 - монол, 3 - кирпичный
//	//также в параметрах можно указывать id игрока
//	Buildings* NewBuilding = nullptr;
//
//	switch (chose)
//	{
//	case 1:
//		NewBuilding = new Panel();
//		IdBuildings[NewBuilding] = NewBuilding->id_;
//		break;
//	case 2:
//		NewBuilding = new Mono;
//		IdBuildings[NewBuilding] = NewBuilding->id_;
//		break;
//	case 3:
//		NewBuilding = new Brick;
//		IdBuildings[NewBuilding] = NewBuilding->id_;
//		break;
//
//	default:
//		break;
//	}
//	return NewBuilding;
//}
//
//Supermarket* CreateNewSupermarket() {
//	Supermarket* NewSupermarket = new Supermarket();
//	return NewSupermarket;
//}
//
////функци€ дл€ расчета прибыли всех супермаркетов, мапа с указателем на магазин и его прибыли 
//map<Supermarket*, long long> CalculateAllSupermarketProfits(vector<Supermarket*>& supermarkets, int current_month, int total_houses) {
//	map<Supermarket*, long long> profits;
//	for (auto* supermarket : supermarkets) {
//		if (supermarket->progress_ == 0) { //построенные супермаркеты
//			long long profit = supermarket->SupermarketMonthlyProfit(current_month, total_houses);
//			profits[supermarket] = profit;
//		}
//	}
//	return profits;
//}
//
////функци€ дл€ обновлени€ прогресса во всех супермаркетах
//void UpdateAllSupermarkets(vector<Supermarket*>& supermarkets) {
//	for (auto* supermarket : supermarkets) {
//		supermarket->UpdateConstructionProgress();
//	}
//}
//
//int CountTotalHouses(vector<Buildings*>& all_buildings) {
//	int total = 0;
//	for (auto* building : all_buildings) {
//		if (building->progress_ == 0) { //построенные дома
//			total++;
//		}
//	}
//	return total;
//}
//
//int CountTotalSupermarkets(vector<Supermarket*>& all_supermarkets) {
//	int total = 0;
//	for (auto* supermarket : all_supermarkets) {
//		if (supermarket->progress_ == 0) { //построенные супермаркеты
//			total++;
//		}
//	}
//	return total;
//}