#include "headers.h"

using namespace std;

vector<Buildings*>buildings_completed;
vector<Supermarket*>supermarket_completed;

class Buildings {
public:
    long long earnings_ = 0;
    int progress_ = months_;
    int id_;
    long long meter_price_;
    int meters_in_flat;
    double base_demand;
    int advertisement_budget_ = 0; // бюджет на рекламу
    double sales_boost_ = 1.0; // множитель продаж от рекламы
    int flats_sold_ = 0;  // проданные квартиры
    double accumulated_ad_effect_ = 0.0;  // накопленный эффект рекламы

    Buildings() {
        type_ = "Will be later";
        price_ = 0;
        months_ = 0;
        flats_ = 0;
        id_ = rand() % 1000;
        progress_ = months_;
    };
    virtual ~Buildings() = default;

    virtual string GetType() {
        return type_;
    }

    virtual long long GetPrice() {
        return price_;
    }

    virtual int GetPeriod() {
        return months_;
    }

    virtual int GetFlats() {
        return flats_;
    }

    virtual int GetAvailableFlats() {
        return flats_ - flats_sold_;
    }

    virtual void UpdateConstructionProgress() {
        progress_--;
        if (progress_ == 0)
        {
            buildings_completed.push_back(this);
        }
    }

    virtual void SetAdvertisementBudget(long long budget) {
        //сохран€ем 50% эффекта от предыдущей рекламы
        accumulated_ad_effect_ = sales_boost_ * 0.5;
        advertisement_budget_ = budget;

        //кажда€ 1000 у.е. увеличивает продажи на 0.5%
        double current_boost = 1.0 + (budget / 1000.0) * 0.005;
        sales_boost_ = current_boost + accumulated_ad_effect_;
    }

    virtual int SellFlats(int quantity) {
        int available = GetAvailableFlats();
        int to_sell = min(quantity, available);
        flats_sold_ += to_sell;

        //доход от продажи
        long long income = to_sell * meters_in_flat * meter_price_;
        earnings_ += income;

        return to_sell;
    }

    //доход от продаж
    virtual long long GetEarnings() {
        return earnings_;
    }


protected:
    string type_;
    long long price_;
    int months_;
    int flats_;
};

class Panel : public Buildings {
public:
    Panel() {
        type_ = "Panel";
        price_ = 8000000;
        months_ = 7;
        flats_ = 100;
        meter_price_ = 120000;
        meters_in_flat = 65;
        base_demand = 1.0;
    }


};

class Mono : public Buildings {
public:
    Mono() {
        type_ = "Mono";
        price_ = 12000000;
        months_ = 10;
        flats_ = 80;
        meter_price_ = 160000;
        meters_in_flat = 75;
        base_demand = 1.3;
    }
};

class Brick : public Buildings {
public:
    Brick() {
        type_ = "Brick";
        price_ = 15000000;
        months_ = 12;
        flats_ = 60;
        meter_price_ = 200000;
        meters_in_flat = 85;
        base_demand = 1.1;
    }
};

class Supermarket {
public:
    long long earnings_ = 0;
    int progress_ = months_;
    int id_;

    long long monthly_profit_ = 0;
    int advertisement_budget_ = 0; // бюджет на рекламу
    double profit_multiplier_ = 1.0; // множитель прибыли от рекламы
    long long total_earnings_ = 0;

    Supermarket() {
        name_ = "Will be later";
        price_ = 2500000;
        months_ = 5;
        id_ = rand() % 1000;
        progress_ = months_;
        monthly_profit_ = 400000; //400,000 у.е. в мес€ц
    }

    virtual ~Supermarket() = default;

    virtual string GetName() {
        return name_;
    }

    virtual long long GetPrice() {
        return price_;
    }

    virtual int GetPeriod() {
        return months_;
    }

    virtual void UpdateConstructionProgress() {
        progress_--;
        if (progress_ == 0)
        {
            supermarket_completed.push_back(this);
        }
    }

    //распределение бюджета, узнаем бонус 
    virtual void SetAdvertisementBudget(long long budget) {
        advertisement_budget_ = budget;
        //  аждые 500 у.е. увеличивают прибыль на 3%
        profit_multiplier_ = 1.0 + (advertisement_budget_ / 500) * 0.03;
    }

    //сколько заработал за мес€ц
    virtual long long SupermarketMonthlyProfit(int month, int total_houses_in_area) {
        if (progress_ > 0) return 0;
        double profit = monthly_profit_;
        double seasonal_koef = GetSupermarketSales(month);
        //бонус от близлежащих домов (+5% за каждый дом)
        double houses_bonus = 1.0 + (total_houses_in_area * 0.05);
        double total_profit = profit * seasonal_koef * houses_bonus * profit_multiplier_;
        earnings_ = static_cast<long long>(total_profit);
        total_earnings_ += earnings_;
        return earnings_;
    }

    virtual long long GetMonthlyProfit() {
        return earnings_;
    }

    virtual long long GetTotalProfit() {
        return total_earnings_;
    }




protected:
    string name_;
    long long price_;
    int months_;
};

class Realtor {
public:
    map<Buildings*, int> ProcessHousingSales(vector<Buildings*>& all_buildings, double total_demand, int current_month, int total_supermarkets) {
        map<Buildings*, int> sales_results;
        vector<pair<Buildings*, double>>offers; //здани€ и их привлекательность
        for (auto* building : all_buildings) {
            if (building->progress_ == 0 and building->GetAvailableFlats() > 0) {
                double attractiveness = CalculateAttractiveness(building, current_month, total_supermarkets);
                offers.push_back({ building, attractiveness });
            }
        }
        sort(offers.begin(), offers.end(), [](const auto& a, const auto& b) { //сортируем по привлекательности
            return a.second > b.second;
            });
        //расчет общего спроса
        double total_demand_value = CalculateTotalDemand(current_month, total_supermarkets);
        //распредел€ем спрос
        double remaining_demand = total_demand_value;
        for (auto& [building, attractiveness] : offers) {
            if (remaining_demand <= 0) break;

            // ƒол€ в общем спросе
            double total_attractiveness = 0;
            for (const auto& offer : offers) {
                total_attractiveness += offer.second;
            }

            if (total_attractiveness > 0) {
                double share = attractiveness / total_attractiveness;
                int potential_sales = static_cast<int>(remaining_demand * share);
                potential_sales = static_cast<int>(potential_sales * building->sales_boost_);
                int actual_sales = min(potential_sales, building->GetAvailableFlats());

                if (actual_sales > 0) {
                    building->SellFlats(actual_sales);
                    sales_results[building] = actual_sales;
                    remaining_demand -= actual_sales;
                }
            }
        }

        return sales_results;
    }


    //----------------------ƒЋя »“ќ√ќ¬ќ√ќ -------------------------------
    double Advertisement_bonus(long long budget, double accumulated_effect = 0) {
        // 1 + (бюджет на рекламу * 0,005) + (уже имеющийс€ бонус * 0,5)
        return 1.0 + (budget / 1000.0) * 0.005 + accumulated_effect * 0.5;
    }

    double Progress_bonus(Buildings* building) {
        if (building->progress_ <= 0) return 1.5; //построенный дом дает +50%
        double progress = 1.0 - (static_cast<double>(building->progress_) / building->GetPeriod());
        return 1.0 + (progress * 0.5); //до +50% при завершении строительства
    }

    //расчЄт привлекательности предложений
    double CalculateAttractiveness(Buildings* building, int current_month, int total_supermarkets) {
        double base = building->base_demand;
        double season = GetHouseDemand(current_month);
        double supermarket_bonus = 1.0 + (total_supermarkets * 0.03);
        double ad_bonus = Advertisement_bonus(building->advertisement_budget_, building->accumulated_ad_effect_);
        double progress_bonus = Progress_bonus(building);
        double price_factor = 1.0 / (building->meter_price_ / 100000.0);
        return base * season * supermarket_bonus * ad_bonus * progress_bonus * price_factor;
    }

    //расчет общего спроса на жильЄ
    double CalculateTotalDemand(int current_month, int total_supermarkets) {
        double base_demand = 50.0; //базовый спрос в квартирах
        double seasonal_factor = GetHouseDemand(current_month);
        double supermarket_bonus = 1.0 + (total_supermarkets * 0.03);

        return base_demand * seasonal_factor * supermarket_bonus;
    }

};

