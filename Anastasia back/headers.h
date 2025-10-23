// headers.h
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <random>

using namespace std;

// Предварительные объявления
class Buildings;
class Panel;
class Mono;
class Brick;
class Supermarket;
class Realtor;
class Player;
class RealPlayer;
class AggressiveBot;
class RandomBot;

// Функции из oop.cpp
double GetHouseDemand(int month);
double GetSupermarketSales(int month);

// Функции из funk.cpp
Buildings* CreateNewBuilding(int chose);
Supermarket* CreateNewSupermarket();
map<Supermarket*, long long> CalculateAllSupermarketProfits(vector<Supermarket*>& supermarkets, int current_month, int total_houses);
void UpdateAllSupermarkets(vector<Supermarket*>& supermarkets);
int CountTotalHouses(vector<Buildings*>& all_buildings);
int CountTotalSupermarkets(vector<Supermarket*>& all_supermarkets);