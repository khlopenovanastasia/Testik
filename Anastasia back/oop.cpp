#include "headers.h"

using namespace std;

class Buildings;
class Supermarket;

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

    Buildings() {
        type_ = "Will be later";
        price_ = 0;
        months_ = 0;
        flats_ = 0;
        id_ = rand() % 1000;
    };
    virtual ~Buildings() = default;

    virtual string GetType() {
        return "Will be later";
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

    virtual void UpdateConstructionProgress() {
        progress_--;
        if (progress_ == 0)
        {
            buildings_completed.push_back(this);
        }
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
    double Advertisement_bonus(long long budget) {
        // 1 + (бюджет на рекламу * 0,005) + (уже имеющийс€ бонус * 0,5)
    }

    double Progress_bonus(Buildings* building) {
        double progress = 1 - (building->progress_ / building->GetPeriod());
        return 1 + (progress * 0.5);
    }

    //расчЄт привлекательности предложений
    double Bonus(long long budget, Buildings* building) {
        building->base_demand* Advertisement_bonus(budget)* Progress_bonus(building);
    }

protected:
    

};