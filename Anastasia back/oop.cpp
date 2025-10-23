//#pragma once
//#include "headers.h"
//
//using namespace std;
//
//// спрос на жильё в зависимости от сезона
//double GetHouseDemand(int month) {
//    month %= 12;
//    //1-Jan 2-Feb 3-Mar 4-Apr 5-May 6-Jun 7-Jul 8-Aug 9-Sep 10-Okt 11-Nov 0-Dec
//
//    if (month < 3)
//    {
//        return 0.8;
//    }
//
//
//    else if (month > 2 and month < 6) {
//        return 1.1;
//    }
//    else if (month > 5 and month < 9) {
//        return 1.3;
//    }
//    else {
//        return 1.5;
//    }
//}
//
////коэфициент продаж в супермаркете
//double GetSupermarketSales(int month) {
//    month %= 12;
//    //1-Jan 2-Feb 3-Mar 4-Apr 5-May 6-Jun 7-Jul 8-Aug 9-Sep 10-Okt 11-Nov 0-Dec
//
//    if (month < 3)
//    {
//        return 1.4;
//    }
//    else if (month > 2 and month < 6) {
//        return 1.1;
//    }
//    else if (month > 5 and month < 9) {
//        return 0.9;
//    }
//    else {
//        return 1.2;
//    }
//}
//
//class Buildings {
//protected:
//    string type_;
//    long long price_;
//    int months_;
//    int flats_;
//
//public:
//    long long earnings_ = 0;
//    int progress_;
//    int id_;
//    long long meter_price_;
//    int meters_in_flat;
//    double base_demand;
//    int advertisement_budget_ = 0;
//    double sales_boost_ = 1.0;
//    int flats_sold_ = 0;
//    double accumulated_ad_effect_ = 0.0;
//
//    Buildings() {
//        type_ = "Will be later";
//        price_ = 0;
//        months_ = 0;
//        flats_ = 0;
//        id_ = rand() % 1000;
//        progress_ = months_;
//    };
//
//    virtual ~Buildings() = default;
//
//    virtual string GetType() {
//        return type_;
//    }
//
//    virtual long long GetPrice() {
//        return price_;
//    }
//
//    virtual int GetPeriod() {
//        return months_;
//    }
//
//    virtual int GetFlats() {
//        return flats_;
//    }
//
//    virtual int GetAvailableFlats() {
//        return flats_ - flats_sold_;
//    }
//
//    virtual bool IsCompleted() const {
//        return progress_ == 0;
//    }
//
//    virtual void UpdateConstructionProgress() {
//        if (progress_ > 0) {
//            progress_--;
//        }
//    }
//
//    virtual void SetAdvertisementBudget(long long budget) {
//        accumulated_ad_effect_ = sales_boost_ * 0.5;
//        advertisement_budget_ = budget;
//        double current_boost = 1.0 + (budget / 1000.0) * 0.005;
//        sales_boost_ = current_boost + accumulated_ad_effect_;
//    }
//
//    virtual int SellFlats(int quantity) {
//        int available = GetAvailableFlats();
//        int to_sell = min(quantity, available);
//        flats_sold_ += to_sell;
//
//        long long income = to_sell * meters_in_flat * meter_price_;
//        earnings_ += income;
//
//        return to_sell;
//    }
//
//    virtual long long GetEarnings() {
//        return earnings_;
//    }
//};
//
//class Panel : public Buildings {
//public:
//    Panel() {
//        type_ = "Panel";
//        price_ = 8000000;
//        months_ = 7;
//        flats_ = 100;
//        meter_price_ = 120000;
//        meters_in_flat = 65;
//        base_demand = 1.0;
//        progress_ = months_;
//    }
//};
//
//class Mono : public Buildings {
//public:
//    Mono() {
//        type_ = "Mono";
//        price_ = 12000000;
//        months_ = 10;
//        flats_ = 80;
//        meter_price_ = 160000;
//        meters_in_flat = 75;
//        base_demand = 1.3;
//        progress_ = months_;
//    }
//};
//
//class Brick : public Buildings {
//public:
//    Brick() {
//        type_ = "Brick";
//        price_ = 15000000;
//        months_ = 12;
//        flats_ = 60;
//        meter_price_ = 200000;
//        meters_in_flat = 85;
//        base_demand = 1.1;
//        progress_ = months_;
//    }
//};
//
//class Supermarket {
//protected:
//    string name_;
//    long long price_;
//    int months_;
//
//public:
//    long long earnings_ = 0;
//    int progress_;
//    int id_;
//    long long monthly_profit_ = 0;
//    int advertisement_budget_ = 0;
//    double profit_multiplier_ = 1.0;
//    long long total_earnings_ = 0;
//
//    Supermarket() {
//        name_ = "Supermarket";
//        price_ = 2500000;
//        months_ = 5;
//        id_ = rand() % 1000;
//        progress_ = months_;
//        monthly_profit_ = 400000;
//    }
//
//    virtual ~Supermarket() = default;
//
//    virtual string GetName() {
//        return name_;
//    }
//
//    virtual long long GetPrice() {
//        return price_;
//    }
//
//    virtual int GetPeriod() {
//        return months_;
//    }
//
//    virtual bool IsCompleted() const {
//        return progress_ == 0;
//    }
//
//    virtual void UpdateConstructionProgress() {
//        if (progress_ > 0) {
//            progress_--;
//        }
//    }
//
//    virtual void SetAdvertisementBudget(long long budget) {
//        advertisement_budget_ = budget;
//        profit_multiplier_ = 1.0 + (advertisement_budget_ / 500) * 0.03;
//    }
//
//    virtual long long SupermarketMonthlyProfit(int month, int total_houses_in_area) {
//        if (!IsCompleted()) return 0;
//        double profit = monthly_profit_;
//        double seasonal_koef = GetSupermarketSales(month);
//        double houses_bonus = 1.0 + (total_houses_in_area * 0.05);
//        double total_profit = profit * seasonal_koef * houses_bonus * profit_multiplier_;
//        earnings_ = static_cast<long long>(total_profit);
//        total_earnings_ += earnings_;
//        return earnings_;
//    }
//
//    virtual long long GetMonthlyProfit() {
//        return earnings_;
//    }
//
//    virtual long long GetTotalProfit() {
//        return total_earnings_;
//    }
//};
//
//class Realtor {
//public:
//    map<Buildings*, int> ProcessHousingSales(const vector<Buildings*>& all_buildings, int current_month, int total_supermarkets) {
//        map<Buildings*, int> sales_results;
//        vector<pair<Buildings*, double>> offers;
//
//        for (auto* building : all_buildings) {
//            if (building->IsCompleted() && building->GetAvailableFlats() > 0) {
//                double attractiveness = CalculateAttractiveness(building, current_month, total_supermarkets);
//                offers.push_back({ building, attractiveness });
//            }
//        }
//
//        sort(offers.begin(), offers.end(), [](const auto& a, const auto& b) {
//            return a.second > b.second;
//            });
//
//        double total_demand_value = CalculateTotalDemand(current_month, total_supermarkets);
//
//        // Распределяем спрос
//        double remaining_demand = total_demand_value;
//        for (auto& offer : offers) {
//            Buildings* building = offer.first;
//            double attractiveness = offer.second;
//
//            if (remaining_demand <= 0) break;
//
//            // Доля в общем спросе
//            double total_attractiveness = 0;
//            for (const auto& o : offers) {
//                total_attractiveness += o.second;
//            }
//
//            if (total_attractiveness > 0) {
//                double share = attractiveness / total_attractiveness;
//                int potential_sales = static_cast<int>(remaining_demand * share);
//                potential_sales = static_cast<int>(potential_sales * building->sales_boost_);
//                int actual_sales = min(potential_sales, building->GetAvailableFlats());
//
//                if (actual_sales > 0) {
//                    building->SellFlats(actual_sales);
//                    sales_results[building] = actual_sales;
//                    remaining_demand -= actual_sales;
//                }
//            }
//        }
//
//        return sales_results;
//    }
//
//    double Advertisement_bonus(long long budget, double accumulated_effect = 0) {
//        return 1.0 + (budget / 1000.0) * 0.005 + accumulated_effect * 0.5;
//    }
//
//    double Progress_bonus(Buildings* building) {
//        if (building->IsCompleted()) return 1.5;
//        double progress = 1.0 - (static_cast<double>(building->progress_) / building->GetPeriod());
//        return 1.0 + (progress * 0.5);
//    }
//
//    double CalculateAttractiveness(Buildings* building, int current_month, int total_supermarkets) {
//        double base = building->base_demand;
//        double season = GetHouseDemand(current_month);
//        double supermarket_bonus = 1.0 + (total_supermarkets * 0.03);
//        double ad_bonus = Advertisement_bonus(building->advertisement_budget_, building->accumulated_ad_effect_);
//        double progress_bonus = Progress_bonus(building);
//        double price_factor = 1.0 / (building->meter_price_ / 100000.0);
//        return base * season * supermarket_bonus * ad_bonus * progress_bonus * price_factor;
//    }
//
//    double CalculateTotalDemand(int current_month, int total_supermarkets) {
//        double base_demand = 50.0;
//        double seasonal_factor = GetHouseDemand(current_month);
//        double supermarket_bonus = 1.0 + (total_supermarkets * 0.03);
//        return base_demand * seasonal_factor * supermarket_bonus;
//    }
//};
