#pragma once
#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <random>

using namespace std;


// спрос на жильё в зависимости от сезона
double GetHouseDemand(int month) {
    month %= 12;
    if (month < 3) return 0.8;
    else if (month > 2 && month < 6) return 1.1;
    else if (month > 5 && month < 9) return 1.3;
    else return 1.5;
}

//коэфициент продаж в супермаркете
double GetSupermarketSales(int month) {
    month %= 12;
    if (month < 3) return 1.4;
    else if (month > 2 && month < 6) return 1.1;
    else if (month > 5 && month < 9) return 0.9;
    else return 1.2;
}

class Buildings {
protected:
    string type_;
    long long price_;
    int months_;
    int flats_;

public:
    long long earnings_ = 0;
    int progress_;
    int id_;
    long long meter_price_;
    int meters_in_flat;
    double base_demand;
    int advertisement_budget_ = 0;
    double sales_boost_ = 1.0;
    int flats_sold_ = 0;
    double accumulated_ad_effect_ = 0.0;

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

    virtual bool IsCompleted() const {
        return progress_ == 0;
    }

    virtual void UpdateConstructionProgress() {
        if (progress_ > 0) {
            progress_--;
        }
    }

    virtual void SetAdvertisementBudget(long long budget) {
        accumulated_ad_effect_ = sales_boost_ * 0.5;
        advertisement_budget_ = budget;
        double current_boost = 1.0 + (budget / 1000.0) * 0.005;
        sales_boost_ = current_boost + accumulated_ad_effect_;
    }

    virtual int SellFlats(int quantity) {
        int available = GetAvailableFlats();
        int to_sell = min(quantity, available);
        flats_sold_ += to_sell;

        long long income = to_sell * meters_in_flat * meter_price_;
        earnings_ += income;

        return to_sell;
    }

    virtual long long GetEarnings() {
        return earnings_;
    }

    virtual long long TakeEarnings() {
        long long earned = earnings_;
        earnings_ = 0;
        return earned;
    }
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
        progress_ = months_;
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
        progress_ = months_;
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
        progress_ = months_;
    }
};

class Supermarket {
protected:
    string name_;
    long long price_;
    int months_;

public:
    long long earnings_ = 0;
    int progress_;
    int id_;
    long long monthly_profit_ = 0;
    int advertisement_budget_ = 0;
    double profit_multiplier_ = 1.0;
    long long total_earnings_ = 0;

    Supermarket() {
        name_ = "Supermarket";
        price_ = 2500000;
        months_ = 5;
        id_ = rand() % 1000;
        progress_ = months_;
        monthly_profit_ = 150000;
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

    virtual bool IsCompleted() const {
        return progress_ == 0;
    }

    virtual void UpdateConstructionProgress() {
        if (progress_ > 0) {
            progress_--;
        }
    }

    virtual void SetAdvertisementBudget(long long budget) {
        advertisement_budget_ = budget;
        profit_multiplier_ = 1.0 + (advertisement_budget_ / 500) * 0.03;
    }

    virtual long long SupermarketMonthlyProfit(int month, int total_houses_in_area) {
        if (!IsCompleted()) return 0;
        double profit = monthly_profit_;
        double seasonal_koef = GetSupermarketSales(month);
        double houses_bonus = 1.0 + (total_houses_in_area * 0.02);
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

    virtual long long TakeEarnings() {
        long long earned = earnings_;
        earnings_ = 0;
        return earned;
    }
};

class Realtor {
private:
    long long global_advertisement_budget_ = 0;
    double global_ad_multiplier_ = 1.0;

public:
    void SetGlobalAdvertisementBudget(long long budget) {
        global_advertisement_budget_ = budget;
        global_ad_multiplier_ = 1.0 + (global_advertisement_budget_ / 1000.0) * 0.01;
    }

    long long GetGlobalAdvertisementBudget() const {
        return global_advertisement_budget_;
    }

    double GetGlobalAdMultiplier() const {
        return global_ad_multiplier_;
    }

    map<Buildings*, int> ProcessHousingSales(const vector<Buildings*>& all_buildings, int current_month, int total_supermarkets) {
        map<Buildings*, int> sales_results;
        vector<pair<Buildings*, double>> offers;

        for (auto* building : all_buildings) {
            if (building->IsCompleted() && building->GetAvailableFlats() > 0) {
                double attractiveness = CalculateAttractiveness(building, current_month, total_supermarkets);
                offers.push_back({ building, attractiveness });
            }
        }

        sort(offers.begin(), offers.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
            });

        double total_demand_value = CalculateTotalDemand(current_month, total_supermarkets);

        double remaining_demand = total_demand_value;
        for (auto& offer : offers) {
            Buildings* building = offer.first;
            double attractiveness = offer.second;

            if (remaining_demand <= 0) break;

            double total_attractiveness = 0;
            for (const auto& o : offers) {
                total_attractiveness += o.second;
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

    double Advertisement_bonus(long long budget, double accumulated_effect = 0) {
        return 1.0 + (budget / 1000.0) * 0.003;
    }

    double Progress_bonus(Buildings* building) {
        if (building->IsCompleted()) return 1.3;
        double progress = 1.0 - (static_cast<double>(building->progress_) / building->GetPeriod());
        return 1.0 + (progress * 0.3);
    }

    double CalculateAttractiveness(Buildings* building, int current_month, int total_supermarkets) {
        double base = building->base_demand;
        double season = GetHouseDemand(current_month);
        double supermarket_bonus = 1.0 + (total_supermarkets * 0.02);
        double ad_bonus = Advertisement_bonus(building->advertisement_budget_, building->accumulated_ad_effect_);
        double progress_bonus = Progress_bonus(building);
        double price_factor = 1.0 / (building->meter_price_ / 120000.0);
        return base * season * supermarket_bonus * ad_bonus * progress_bonus * price_factor;
    }

    double CalculateTotalDemand(int current_month, int total_supermarkets) {
        double base_demand = 30.0;
        double seasonal_factor = GetHouseDemand(current_month);
        double supermarket_bonus = 1.0 + (total_supermarkets * 0.02);
        double total_demand = base_demand * seasonal_factor * supermarket_bonus * global_ad_multiplier_;
        return total_demand;
    }
};


class RealPlayer;
class AggressiveBot;
class RandomBot;

class Player {
protected:
    string name_;
    long long capital_;
    vector<Buildings*> buildings_;
    vector<Supermarket*> supermarkets_;
    int player_id_;
    static int player_count_;
    long long previous_capital_;
    long long previous_assets_;
    long long monthly_income_ = 0;

public:
    Player(const string& name, long long initial_capital = 37000000)
        : name_(name), capital_(initial_capital) {
        player_id_ = player_count_++;
        previous_capital_ = capital_;
        previous_assets_ = CalculateTotalAssets();
    }

    virtual ~Player() {
        for (auto building : buildings_) {
            delete building;
        }
        for (auto supermarket : supermarkets_) {
            delete supermarket;
        }
    }
    virtual bool IsRealPlayer() const { return false; }
    virtual void MakeDecision(int current_month, int total_months) {
    }

    string GetName() { return name_; }
    long long GetCapital() { return capital_; }
    int GetId() { return player_id_; }
    vector<Buildings*>& GetBuildings() { return buildings_; }
    vector<Supermarket*>& GetSupermarkets() { return supermarkets_; }
    long long GetMonthlyIncome() const { return monthly_income_; }

    bool InvestInPanelHouse(int count = 1) {
        long long total_cost = 8000000 * count;
        if (capital_ >= total_cost) {
            capital_ -= total_cost;
            for (int i = 0; i < count; i++) {
                buildings_.push_back(new Panel());
            }
            return true;
        }
        return false;
    }

    bool InvestInMonoHouse(int count = 1) {
        long long total_cost = 12000000 * count;
        if (capital_ >= total_cost) {
            capital_ -= total_cost;
            for (int i = 0; i < count; i++) {
                buildings_.push_back(new Mono());
            }
            return true;
        }
        return false;
    }

    bool InvestInBrickHouse(int count = 1) {
        long long total_cost = 15000000 * count;
        if (capital_ >= total_cost) {
            capital_ -= total_cost;
            for (int i = 0; i < count; i++) {
                buildings_.push_back(new Brick());
            }
            return true;
        }
        return false;
    }

    bool InvestInSupermarket(int count = 1) {
        long long total_cost = 2500000 * count;
        if (capital_ >= total_cost) {
            capital_ -= total_cost;
            for (int i = 0; i < count; i++) {
                supermarkets_.push_back(new Supermarket());
            }
            return true;
        }
        return false;
    }

    void SetBuildingAdvertisementBudget(Buildings* building, long long budget) {
        if (capital_ >= budget) {
            capital_ -= budget;
            building->SetAdvertisementBudget(budget);
        }
    }

    void SetSupermarketAdvertisementBudget(Supermarket* supermarket, long long budget) {
        if (capital_ >= budget) {
            capital_ -= budget;
            supermarket->SetAdvertisementBudget(budget);
        }
    }

    void UpdateMonthly() {
        for (auto building : buildings_) {
            building->UpdateConstructionProgress();
        }
        for (auto supermarket : supermarkets_) {
            supermarket->UpdateConstructionProgress();
        }
    }

    void CollectIncome() {
        monthly_income_ = 0;

        for (auto* building : buildings_) {
            long long building_earnings = building->TakeEarnings();
            monthly_income_ += building_earnings;
            capital_ += building_earnings;
        }

        for (auto* supermarket : supermarkets_) {
            long long market_earnings = supermarket->TakeEarnings();
            monthly_income_ += market_earnings;
            capital_ += market_earnings;
        }
    }

    long long CalculateTotalAssets() const {
        long long total_assets = capital_;

        for (const auto& building : buildings_) {
            total_assets += building->GetPrice();
        }
        for (const auto& supermarket : supermarkets_) {
            total_assets += supermarket->GetPrice();
        }

        for (const auto& building : buildings_) {
            if (building->IsCompleted()) {
                int unsold_flats = building->GetAvailableFlats();
                long long construction_cost_per_flat = building->GetPrice() / building->GetFlats();
                total_assets += unsold_flats * construction_cost_per_flat;
            }
        }

        for (const auto& supermarket : supermarkets_) {
            if (supermarket->IsCompleted()) {
                total_assets += supermarket->GetPrice() * 1.6;
            }
        }

        return total_assets;
    }

    void SaveState() {
        previous_capital_ = capital_;
        previous_assets_ = CalculateTotalAssets();
        monthly_income_ = 0;
    }

    long long GetCapitalChange() const {
        return capital_ - previous_capital_;
    }

    long long GetAssetsChange() const {
        return CalculateTotalAssets() - previous_assets_;
    }

};

int Player::player_count_ = 0;

class RealPlayer : public Player {
public:
    RealPlayer(const string& name, long long initial_capital = 37000000)
        : Player(name, initial_capital) {}
    bool IsRealPlayer() const override { return true; }
    bool SetChoice(int choice, int count = 1) {
        if (choice == 1) {
            return InvestInPanelHouse(count);
        }
        else if (choice == 2) {
            return InvestInMonoHouse(count);
        }
        else if (choice == 3) {
            return InvestInBrickHouse(count);
        }
        else if (choice == 4) {
            return InvestInSupermarket(count);
        }
        return false;
    }
};

class AggressiveBot : public Player {
public:
    AggressiveBot(const string& name, long long initial_capital = 37000000)
        : Player(name, initial_capital) {}
    bool IsRealPlayer() const override { return false; }
    void MakeDecision(int current_month, int total_months) override {
        if (current_month <= total_months / 2) {
            int max_brick = static_cast<int>(GetCapital() / 15000000);
            if (max_brick > 0) {
                InvestInBrickHouse(max_brick);
                return;
            }

            int max_mono = static_cast<int>(GetCapital() / 12000000);
            if (max_mono > 0) {
                InvestInMonoHouse(max_mono);
                return;
            }

            int max_panel = static_cast<int>(GetCapital() / 8000000);
            if (max_panel > 0) {
                InvestInPanelHouse(max_panel);
                return;
            }

            int max_market = static_cast<int>(GetCapital() / 2500000);
            if (max_market > 0) {
                InvestInSupermarket(max_market);
                return;
            }
        }
        else {
            int max_market = static_cast<int>(GetCapital() / 2500000);
            if (max_market > 0) {
                InvestInSupermarket(max_market);
                return;
            }

            int max_panel = static_cast<int>(GetCapital() / 8000000);
            if (max_panel > 0) {
                InvestInPanelHouse(max_panel);
                return;
            }

            int max_mono = static_cast<int>(GetCapital() / 12000000);
            if (max_mono > 0) {
                InvestInMonoHouse(max_mono);
                return;
            }
        }
    }
};

class RandomBot : public Player {
public:
    RandomBot(const string& name, long long initial_capital = 37000000)
        : Player(name, initial_capital) {}
    bool IsRealPlayer() const override { return false; }
    void MakeDecision(int current_month, int total_months) override {
        vector<pair<string, long long>> available_actions;
        int max_panel = static_cast<int>(GetCapital() / 8000000);
        int max_mono = static_cast<int>(GetCapital() / 12000000);
        int max_brick = static_cast<int>(GetCapital() / 15000000);
        int max_market = static_cast<int>(GetCapital() / 2500000);

        if (max_panel > 0) {
            available_actions.push_back({ "panel", 8000000 });
        }
        if (max_mono > 0) {
            available_actions.push_back({ "mono", 12000000 });
        }
        if (max_brick > 0) {
            available_actions.push_back({ "brick", 15000000 });
        }
        if (max_market > 0) {
            available_actions.push_back({ "supermarket", 2500000 });
        }
        available_actions.push_back({ "skip", 0 });

        if (available_actions.size() > 1) {
            int rn = rand() % available_actions.size();
            string action = available_actions[rn].first;
            long long price = available_actions[rn].second;

            if (action == "panel") {
                int count = 1 + rand() % max_panel; 
                InvestInPanelHouse(count);
            }
            else if (action == "mono") {
                int count = 1 + rand() % max_mono;
                InvestInMonoHouse(count);
            }
            else if (action == "brick") {
                int count = 1 + rand() % max_brick;
                InvestInBrickHouse(count);
            }
            else if (action == "supermarket") {
                int count = 1 + rand() % max_market;
                InvestInSupermarket(count);
            }
        }
    }
};



class SetupWindow {
private:
    int num_players = 3;
    int total_months = 24;
    vector<int> player_types;
    vector<string> player_names;

public:
    SetupWindow() {
        player_types.resize(8, 0);
        player_names.resize(8);
        for (int i = 0; i < 8; i++) {
            player_names[i] = "Player " + to_string(i + 1);
        }
    }

    bool Render() {
        bool start_game = false;

        ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

        ImGui::Begin("Game Setup", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        ImGui::Text("GAME SETUP");
        ImGui::Separator();

        ImGui::Text("Number of players:");
        ImGui::SliderInt("##Players", &num_players, 1, 8);
        ImGui::SameLine();
        ImGui::Text("%d", num_players);

        ImGui::Text("Game duration (months):");
        ImGui::SliderInt("##Months", &total_months, 12, 60);
        ImGui::SameLine();
        ImGui::Text("%d", total_months);

        ImGui::Separator();
        ImGui::Text("Player Configuration:");

        if (ImGui::BeginTable("PlayersTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Player");
            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Type");
            ImGui::TableHeadersRow();

            for (int i = 0; i < num_players; i++) {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Player %d", i + 1);

                ImGui::TableSetColumnIndex(1);
                string label = "##Name" + to_string(i);
                ImGui::SetNextItemWidth(150);
                ImGui::InputText(label.c_str(), &player_names[i]);

                ImGui::TableSetColumnIndex(2);
                string combo_label = "##Type" + to_string(i);
                const char* types[] = { "Human", "Aggressive Bot", "Random Bot" };
                ImGui::SetNextItemWidth(150);
                ImGui::Combo(combo_label.c_str(), &player_types[i], types, IM_ARRAYSIZE(types));
            }
            ImGui::EndTable();
        }

        ImGui::Separator();

        if (ImGui::Button("START GAME", ImVec2(200, 60))) {
            start_game = true;
        }

        ImGui::End();

        return start_game;
    }

    int GetNumPlayers() const { return num_players; }
    int GetTotalMonths() const { return total_months; }
    int GetPlayerType(int index) const { return player_types[index]; }
    string GetPlayerName(int index) const { return player_names[index]; }
};

class MonthlyResultsWindow {
private:
    bool show_results = false;
    int current_month = 0;

public:
    void Show(int month) {
        show_results = true;
        current_month = month;
    }

    bool Render(const vector<Player*>& players) {
        if (!show_results) return false;

        bool continue_game = false;

        ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(300, 150), ImGuiCond_Always);

        ImGui::Begin("Monthly Results", &show_results, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        ImGui::Text("MONTH %d RESULTS", current_month);
        ImGui::Separator();

        if (ImGui::BeginTable("ResultsTable", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Player");
            ImGui::TableSetupColumn("Type");
            ImGui::TableSetupColumn("Monthly Income");
            ImGui::TableSetupColumn("Capital Change");
            ImGui::TableSetupColumn("Assets Change");
            ImGui::TableSetupColumn("Total Assets");
            ImGui::TableHeadersRow();

            for (int i = 0; i < players.size(); i++) {
                auto* player = players[i];
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", player->GetName().c_str());

                ImGui::TableSetColumnIndex(1);
                if (player->IsRealPlayer()) {
                    ImGui::Text("Human");
                }
                else if (dynamic_cast<AggressiveBot*>(player)) {
                    ImGui::Text("Aggressive Bot");
                }
                else {
                    ImGui::Text("Random Bot");
                }

                ImGui::TableSetColumnIndex(2);
                long long monthly_income = player->GetMonthlyIncome();
                if (monthly_income > 0) {
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "+%.2fM", monthly_income / 1000000.0);
                }
                else {
                    ImGui::Text("0.00M");
                }

                ImGui::TableSetColumnIndex(3);
                long long capital_change = player->GetCapitalChange();
                if (capital_change > 0) {
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "+%.2fM", capital_change / 1000000.0);
                }
                else if (capital_change < 0) {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.2fM", capital_change / 1000000.0);
                }
                else {
                    ImGui::Text("0.00M");
                }

                ImGui::TableSetColumnIndex(4);
                long long assets_change = player->GetAssetsChange();
                if (assets_change > 0) {
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "+%.2fM", assets_change / 1000000.0);
                }
                else if (assets_change < 0) {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.2fM", assets_change / 1000000.0);
                }
                else {
                    ImGui::Text("0.00M");
                }

                ImGui::TableSetColumnIndex(5);
                ImGui::Text("%.2fM", player->CalculateTotalAssets() / 1000000.0);
            }
            ImGui::EndTable();
        }

        ImGui::Separator();
        ImGui::Text("Summary:");
        ImGui::Text("Buildings constructed this month: %d", CountNewBuildings(players));
        ImGui::Text("Supermarkets constructed this month: %d", CountNewSupermarkets(players));
        ImGui::Text("Apartments sold this month: %d", CountSoldFlats(players));

        ImGui::Separator();
        if (ImGui::Button("CONTINUE TO NEXT MONTH", ImVec2(300, 60))) {
            continue_game = true;
            show_results = false;
        }

        ImGui::End();

        return continue_game;
    }

private:
    int CountNewBuildings(const vector<Player*>& players) {
        int count = 0;
        for (auto* player : players) {
            for (auto* building : player->GetBuildings()) {
                if (building->GetPeriod() - building->progress_ == 1) { 
                    count++;
                }
            }
        }
        return count;
    }

    int CountNewSupermarkets(const vector<Player*>& players) {
        int count = 0;
        for (auto* player : players) {
            for (auto* market : player->GetSupermarkets()) {
                if (market->GetPeriod() - market->progress_ == 1) {
                    count++;
                }
            }
        }
        return count;
    }

    int CountSoldFlats(const vector<Player*>& players) {
        int count = 0;
        for (auto* player : players) {
            for (auto* building : player->GetBuildings()) {
                count += building->flats_sold_;
            }
        }
        return count;
    }
};

class GameOverWindow {
private:
    bool show_game_over = false;

public:
    void Show() {
        show_game_over = true;
    }

    bool Render(const vector<Player*>& players) {
        if (!show_game_over) return false;

        bool restart_game = false;

        ImGui::SetNextWindowSize(ImVec2(900, 700), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(250, 100), ImGuiCond_Always);

        ImGui::Begin("GAME OVER - FINAL RESULTS", &show_game_over,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "INVESTMENT GAME FINISHED!");
        ImGui::Separator();

        vector<Player*> sorted_players = players;
        sort(sorted_players.begin(), sorted_players.end(),
            [](Player* a, Player* b) {
                return a->CalculateTotalAssets() > b->CalculateTotalAssets();
            });

        ImGui::Text("FINAL RANKINGS:");
        ImGui::Separator();

        if (ImGui::BeginTable("FinalResults", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Rank");
            ImGui::TableSetupColumn("Player");
            ImGui::TableSetupColumn("Type");
            ImGui::TableSetupColumn("Final Capital");
            ImGui::TableSetupColumn("Total Assets");
            ImGui::TableSetupColumn("Buildings/Markets");
            ImGui::TableHeadersRow();

            for (int i = 0; i < sorted_players.size(); i++) {
                auto* player = sorted_players[i];
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                if (i == 0) {
                    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "1st");
                }
                else if (i == 1) {
                    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "2nd");
                }
                else if (i == 2) {
                    ImGui::TextColored(ImVec4(0.8f, 0.5f, 0.2f, 1.0f), "3rd");
                }
                else {
                    ImGui::Text("%dth", i + 1);
                }

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", player->GetName().c_str());


                ImGui::TableSetColumnIndex(2);
                if (player->IsRealPlayer()) {
                    ImGui::Text("Human");
                }
                else if (dynamic_cast<AggressiveBot*>(player)) {
                    ImGui::Text("Aggressive Bot");
                }
                else {
                    ImGui::Text("Random Bot");
                }

                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%.2fM", player->GetCapital() / 1000000.0);

                ImGui::TableSetColumnIndex(4);
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%.2fM", player->CalculateTotalAssets() / 1000000.0);

                ImGui::TableSetColumnIndex(5);
                ImGui::Text("%d / %d", player->GetBuildings().size(), player->GetSupermarkets().size());
            }
            ImGui::EndTable();
        }

        ImGui::Separator();

        if (!sorted_players.empty()) {
            Player* winner = sorted_players[0];
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "WINNER: %s", winner->GetName().c_str());
            ImGui::Text("Total Assets: %.2fM", winner->CalculateTotalAssets() / 1000000.0);

            if (winner->IsRealPlayer()) {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Congratulations! You won!");
            }
            else {
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "Bot victory! Better luck next time!");
            }
        }

        ImGui::Separator();

        ImGui::Text("GAME SUMMARY:");
        ImGui::Text("Total Buildings: %d", CountTotalBuildings(players));
        ImGui::Text("Total Supermarkets: %d", CountTotalSupermarkets(players));
        ImGui::Text("Total Apartments Sold: %d", CountTotalSoldFlats(players));

        ImGui::Separator();

        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 150);
        if (ImGui::Button("PLAY AGAIN", ImVec2(300, 80))) {
            restart_game = true;
            show_game_over = false;
        }

        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 150);
        if (ImGui::Button("EXIT TO MAIN MENU", ImVec2(300, 60))) {
            show_game_over = false;
        }

        ImGui::End();

        return restart_game;
    }

private:
    int CountTotalBuildings(const vector<Player*>& players) {
        int count = 0;
        for (auto* player : players) {
            count += player->GetBuildings().size();
        }
        return count;
    }

    int CountTotalSupermarkets(const vector<Player*>& players) {
        int count = 0;
        for (auto* player : players) {
            count += player->GetSupermarkets().size();
        }
        return count;
    }

    int CountTotalSoldFlats(const vector<Player*>& players) {
        int count = 0;
        for (auto* player : players) {
            for (auto* building : player->GetBuildings()) {
                count += building->flats_sold_;
            }
        }
        return count;
    }
};

class GameUI {
private:
    Realtor realtor;
    vector<Player*> players;
    int current_month = 0;
    int total_months = 24;
    bool game_running = false;
    bool game_started = false;
    bool show_monthly_results = false;
    bool show_game_over = false;
    int selected_player = 0;
    int selected_building_type = 0;
    Buildings* selected_building = nullptr;
    Supermarket* selected_market = nullptr;
    MonthlyResultsWindow results_window;
    GameOverWindow game_over_window;

    int panel_count = 1;
    int mono_count = 1;
    int brick_count = 1;
    int supermarket_count = 1;

    int global_ad_budget = 0;
    int building_ad_budget = 0;
    int supermarket_ad_budget = 0;

    const float WINDOW_WIDTH = 1400.0f;
    const float WINDOW_HEIGHT = 900.0f;
    const float LEFT_PANEL_WIDTH = 400.0f;
    const float RIGHT_PANEL_WIDTH = 450.0f;

    vector<Buildings*> GetAllBuildingsFromPlayers() {
        vector<Buildings*> all_buildings;
        for (auto* player : players) {
            auto& player_buildings = player->GetBuildings();
            all_buildings.insert(all_buildings.end(), player_buildings.begin(), player_buildings.end());
        }
        return all_buildings;
    }

    int GetTotalCompletedSupermarkets() {
        int total = 0;
        for (auto* player : players) {
            for (auto* supermarket : player->GetSupermarkets()) {
                if (supermarket->IsCompleted()) total++;
            }
        }
        return total;
    }

    int GetTotalHousesInArea() {
        int total = 0;
        for (auto* player : players) {
            for (auto* building : player->GetBuildings()) {
                if (building->IsCompleted()) total++;
            }
        }
        return total;
    }

    string GetSeasonName(int month) {
        int m = month % 12;
        if (m < 3) return "Winter";
        if (m < 6) return "Spring";
        if (m < 9) return "Summer";
        return "Autumn";
    }

    RealPlayer* GetCurrentRealPlayer() {
        if (players.empty()) return nullptr;
        auto* player = players[selected_player];
        return dynamic_cast<RealPlayer*>(player);
    }

    int GetMaxPanelCount(Player* player) {
        return static_cast<int>(player->GetCapital() / 8000000);
    }

    int GetMaxMonoCount(Player* player) {
        return static_cast<int>(player->GetCapital() / 12000000);
    }

    int GetMaxBrickCount(Player* player) {
        return static_cast<int>(player->GetCapital() / 15000000);
    }

    int GetMaxSupermarketCount(Player* player) {
        return static_cast<int>(player->GetCapital() / 2500000);
    }

public:
    void InitializePlayers(const SetupWindow& setup) {
        
        for (auto* player : players) {
            delete player;
        }
        players.clear();


        int num_players = setup.GetNumPlayers();
        total_months = setup.GetTotalMonths();

        for (int i = 0; i < num_players; i++) {
            string name = setup.GetPlayerName(i);
            int type = setup.GetPlayerType(i);

            Player* player = nullptr;
            switch (type) {
            case 0: 
                player = new RealPlayer(name);
                break;
            case 1: 
                player = new AggressiveBot(name);
                break;
            case 2: 
                player = new RandomBot(name);
                break;
            }

            if (player) {
                players.push_back(player);
            }
        }

        game_started = true;
        game_running = true;
        current_month = 0;
        show_game_over = false;


        panel_count = 1;
        mono_count = 1;
        brick_count = 1;
        supermarket_count = 1;


        global_ad_budget = 0;
        building_ad_budget = 0;
        supermarket_ad_budget = 0;


        for (auto* player : players) {
            player->SaveState();
        }
    }

    void RenderUI() {
        if (!game_started) return;


        if (show_game_over) {
            if (game_over_window.Render(players)) {

                game_started = false;
                game_running = false;
                show_game_over = false;
            }
            return;
        }


        if (show_monthly_results) {
            if (results_window.Render(players)) {
                show_monthly_results = false;
                game_running = true;
            }
            return;
        }

        ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

        ImGui::Begin("Investment Game", nullptr,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

        ImGui::SetWindowFontScale(1.2f);

        RenderTopPanel();

        ImGui::BeginChild("MainContent", ImVec2(0, 0), true);

        ImGui::Columns(2, "MainColumns", false);
        ImGui::SetColumnWidth(0, LEFT_PANEL_WIDTH);

        RenderLeftColumn();

        ImGui::NextColumn();

        RenderRightColumn();

        ImGui::Columns(1);
        ImGui::EndChild();

        ImGui::End();
    }

    void RenderSetupWindow(SetupWindow& setup) {
        if (setup.Render()) {
            InitializePlayers(setup);
        }
    }

    bool IsGameStarted() const { return game_started; }

private:
    void RenderTopPanel() {
        ImGui::BeginChild("TopPanel", ImVec2(0, 80), true);

        ImGui::Columns(3, "TopColumns", false);

        ImGui::Text("Month: %d/%d", current_month, total_months);
        ImGui::Text("Season: %s", GetSeasonName(current_month).c_str());

        ImGui::NextColumn();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 70);
        if (ImGui::Button("Next Month", ImVec2(120, 40)) && game_running) {
            ProcessNextMonth();
        }

        ImGui::NextColumn();

        ImGui::Text("Buildings: %d", GetAllBuildingsFromPlayers().size());
        ImGui::Text("Supermarkets: %d", GetTotalCompletedSupermarkets());

        ImGui::Columns(1);
        ImGui::EndChild();
    }

    void RenderLeftColumn() {
        ImGui::BeginChild("PlayersPanel", ImVec2(0, 300), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
        ImGui::Text("PLAYERS");
        ImGui::Separator();

        for (int i = 0; i < players.size(); ++i) {
            auto* player = players[i];
            ImGui::PushID(i);

            bool is_selected = (selected_player == i);
            bool is_real_player = player->IsRealPlayer();

            ImVec4 button_color = is_real_player ?
                (is_selected ? ImVec4(0.2f, 0.6f, 0.2f, 0.8f) : ImVec4(0.3f, 0.3f, 0.3f, 0.6f)) :
                (is_selected ? ImVec4(0.6f, 0.2f, 0.2f, 0.8f) : ImVec4(0.4f, 0.2f, 0.2f, 0.6f));

            ImGui::PushStyleColor(ImGuiCol_Button, button_color);

            string button_text = player->GetName();
            if (!is_real_player) {
                button_text += " (BOT)";
            }

            if (ImGui::Button(button_text.c_str(), ImVec2(350, 50))) {
                selected_player = i;
            }

            ImGui::PopStyleColor();

            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("Capital: %.2fM", player->GetCapital() / 1000000.0);
                ImGui::Text("Assets: %.2fM", player->CalculateTotalAssets() / 1000000.0);
                ImGui::Text("Buildings: %d", player->GetBuildings().size());
                ImGui::Text("Supermarkets: %d", player->GetSupermarkets().size());
                if (!is_real_player) {
                    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "BOT - View Only");
                }
                ImGui::EndTooltip();
            }

            ImGui::PopID();
        }
        ImGui::EndChild();

        ImGui::BeginChild("ConstructionPanel", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
        ImGui::Text("CONSTRUCTION");
        ImGui::Separator();

        if (!players.empty()) {
            auto* current_player = players[selected_player];
            bool is_real_player = current_player->IsRealPlayer();

            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Available: %.2fM", current_player->GetCapital() / 1000000.0);
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.2f, 1.0f), "Total Assets: %.2fM", current_player->CalculateTotalAssets() / 1000000.0);

            if (!is_real_player) {
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "BOT PLAYER - VIEW ONLY");
                ImGui::Text("You can view bot's properties but cannot build for them");

                if (dynamic_cast<AggressiveBot*>(current_player)) {
                    ImGui::Text("Strategy: Aggressive - builds maximum expensive buildings");
                }
                else if (dynamic_cast<RandomBot*>(current_player)) {
                    ImGui::Text("Strategy: Random - builds random number of buildings");
                }
            }
            else {

                ImGui::Text("Multiple Construction:");
                ImGui::Separator();

      
                int max_panels = GetMaxPanelCount(current_player);
                if (max_panels > 0) {
                    ImGui::Text("Panel Houses (8M each):");
                    ImGui::SliderInt("##PanelCount", &panel_count, 1, max_panels);
                    ImGui::SameLine();
                    ImGui::Text("%d", panel_count);
                    ImGui::SameLine();
                    if (ImGui::Button("Build##Panel", ImVec2(80, 30))) {
                        current_player->InvestInPanelHouse(panel_count);
                        panel_count = 1; 
                    }
                    ImGui::Text("Total cost: %.2fM", (panel_count * 8000000) / 1000000.0);
                }

                
                int max_monos = GetMaxMonoCount(current_player);
                if (max_monos > 0) {
                    ImGui::Text("Monolithic Houses (12M each):");
                    ImGui::SliderInt("##MonoCount", &mono_count, 1, max_monos);
                    ImGui::SameLine();
                    ImGui::Text("%d", mono_count);
                    ImGui::SameLine();
                    if (ImGui::Button("Build##Mono", ImVec2(80, 30))) {
                        current_player->InvestInMonoHouse(mono_count);
                        mono_count = 1;
                    }
                    ImGui::Text("Total cost: %.2fM", (mono_count * 12000000) / 1000000.0);
                }

            
                int max_bricks = GetMaxBrickCount(current_player);
                if (max_bricks > 0) {
                    ImGui::Text("Brick Houses (15M each):");
                    ImGui::SliderInt("##BrickCount", &brick_count, 1, max_bricks);
                    ImGui::SameLine();
                    ImGui::Text("%d", brick_count);
                    ImGui::SameLine();
                    if (ImGui::Button("Build##Brick", ImVec2(80, 30))) {
                        current_player->InvestInBrickHouse(brick_count);
                        brick_count = 1;
                    }
                    ImGui::Text("Total cost: %.2fM", (brick_count * 15000000) / 1000000.0);
                }

                int max_markets = GetMaxSupermarketCount(current_player);
                if (max_markets > 0) {
                    ImGui::Text("Supermarkets (2.5M each):");
                    ImGui::SliderInt("##MarketCount", &supermarket_count, 1, max_markets);
                    ImGui::SameLine();
                    ImGui::Text("%d", supermarket_count);
                    ImGui::SameLine();
                    if (ImGui::Button("Build##Market", ImVec2(80, 30))) {
                        current_player->InvestInSupermarket(supermarket_count);
                        supermarket_count = 1;
                    }
                    ImGui::Text("Total cost: %.2fM", (supermarket_count * 2500000) / 1000000.0);
                }

                if (max_panels == 0 && max_monos == 0 && max_bricks == 0 && max_markets == 0) {
                    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "Not enough money to build anything");
                }
            }
        }
        ImGui::EndChild();
    }

    void RenderRightColumn() {
        ImGui::BeginChild("MarketPanel", ImVec2(0, 300), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
        ImGui::Text("MARKET ANALYSIS");
        ImGui::Separator();

        auto all_buildings = GetAllBuildingsFromPlayers();
        int total_supermarkets = GetTotalCompletedSupermarkets();
        double current_demand = realtor.CalculateTotalDemand(current_month, total_supermarkets);

        ImGui::Text("Housing Demand: %.1f apartments", current_demand);
        ImGui::Text("Active Supermarkets: %d", total_supermarkets);
        ImGui::Text("Total Buildings: %d", all_buildings.size());


        ImGui::Separator();
        ImGui::Text("ADVERTISEMENT");

        double ad_multiplier = realtor.GetGlobalAdMultiplier();
        ImGui::Text("Global Ad Multiplier: +%.1f%%", (ad_multiplier - 1.0) * 100.0);
        ImGui::Text("Total Ad Budget: %.1fK", realtor.GetGlobalAdvertisementBudget() / 1000.0);

        if (!players.empty() && players[selected_player]->IsRealPlayer()) {
            auto* current_player = players[selected_player];
            ImGui::SliderInt("Global Ad Budget (K)##GlobalAd", &global_ad_budget, 0, static_cast<int>(current_player->GetCapital() / 1000));
            ImGui::SameLine();
            if (ImGui::Button("Buy##GlobalAd", ImVec2(60, 20))) {
                long long budget = global_ad_budget * 1000LL;
                if (current_player->GetCapital() >= budget) {
                    long long new_capital = current_player->GetCapital() - budget;
                    realtor.SetGlobalAdvertisementBudget(realtor.GetGlobalAdvertisementBudget() + budget);
                    global_ad_budget = 0;
                }
            }
            ImGui::Text("Effect: +%.1f%% demand", (global_ad_budget * 0.01));
        }

        ImGui::Separator();
        ImGui::Text("Seasonal Trends:");
        RenderSeasonalChart();

        ImGui::EndChild();

        ImGui::BeginChild("ObjectsPanel", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
        ImGui::Text("OBJECTS MANAGEMENT");
        ImGui::Separator();

        if (!players.empty()) {
            auto* current_player = players[selected_player];
            bool is_real_player = current_player->IsRealPlayer();

            
            if (!is_real_player) {
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "Viewing: %s (BOT)", current_player->GetName().c_str());
                ImGui::Text("Buildings and supermarkets are read-only for bots");
                ImGui::Separator();
            }

            if (ImGui::TreeNodeEx("Buildings", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto& buildings = current_player->GetBuildings();
                if (buildings.empty()) {
                    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "No buildings constructed");
                }
                else {
                    for (auto* building : buildings) {
                        ImGui::PushID(building);

                        bool is_selected = (selected_building == building);
                        string status = building->IsCompleted() ?
                            "DONE " + to_string(building->flats_sold_) + "/" + to_string(building->GetFlats()) + " sold" :
                            "BUILD " + to_string(building->GetPeriod() - building->progress_) + "/" + to_string(building->GetPeriod()) + " months";

                        
                        if (is_real_player) {
                            if (ImGui::Selectable(building->GetType().c_str(), is_selected)) {
                                selected_building = building;
                                selected_market = nullptr;
                            }
                        }
                        else {
                            ImGui::Text("%s", building->GetType().c_str());
                        }

                        ImGui::SameLine();
                        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.2f, 1.0f), "%s", status.c_str());

                        if (is_real_player && is_selected && building->IsCompleted()) {
                            ImGui::Text("  Advertisement: %.1fK (+%.1f%%)",
                                building->advertisement_budget_ / 1000.0,
                                (building->sales_boost_ - 1.0) * 100.0);

                            ImGui::SliderInt("  Ad Budget (K)##BuildingAd", &building_ad_budget, 0,
                                static_cast<int>(current_player->GetCapital() / 1000));
                            ImGui::SameLine();
                            if (ImGui::Button("Buy##BuildingAd", ImVec2(60, 20))) {
                                long long budget = building_ad_budget * 1000LL;
                                current_player->SetBuildingAdvertisementBudget(building, budget);
                                building_ad_budget = 0;
                            }
                        }

                        ImGui::PopID();
                    }
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNodeEx("Supermarkets", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto& supermarkets = current_player->GetSupermarkets();
                if (supermarkets.empty()) {
                    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "No supermarkets constructed");
                }
                else {
                    for (auto* market : supermarkets) {
                        ImGui::PushID(market);

                        bool is_selected = (selected_market == market);
                        string status = market->IsCompleted() ?
                            "PROFIT +" + to_string(market->GetMonthlyProfit() / 1000) + "K/month" :
                            "BUILD " + to_string(market->GetPeriod() - market->progress_) + "/" + to_string(market->GetPeriod()) + " months";

                        
                        if (is_real_player) {
                            if (ImGui::Selectable("Supermarket", is_selected)) {
                                selected_market = market;
                                selected_building = nullptr;
                            }
                        }
                        else {
                        
                            ImGui::Text("Supermarket");
                        }

                        ImGui::SameLine();
                        ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "%s", status.c_str());

                        if (is_real_player && is_selected && market->IsCompleted()) {
                            ImGui::Text("  Advertisement: %.1fK (+%.1f%%)",
                                market->advertisement_budget_ / 1000.0,
                                (market->profit_multiplier_ - 1.0) * 100.0);

                            ImGui::SliderInt("  Ad Budget (K)##MarketAd", &supermarket_ad_budget, 0,
                                static_cast<int>(current_player->GetCapital() / 1000));
                            ImGui::SameLine();
                            if (ImGui::Button("Buy##MarketAd", ImVec2(60, 20))) {
                                long long budget = supermarket_ad_budget * 1000LL;
                                current_player->SetSupermarketAdvertisementBudget(market, budget);
                                supermarket_ad_budget = 0;
                            }
                        }

                        ImGui::PopID();
                    }
                }
                ImGui::TreePop();
            }

            if (!is_real_player) {
                ImGui::Separator();
                ImGui::Text("Bot Statistics:");
                ImGui::Text("Total Investments: %.2fM",
                    (current_player->GetBuildings().size() * 10000000LL +
                        current_player->GetSupermarkets().size() * 2500000LL) / 1000000.0);
                ImGui::Text("Construction in progress: %d",
                    CountBuildingsInProgress(current_player) + CountMarketsInProgress(current_player));
            }
        }
        ImGui::EndChild();
    }

    void RenderSeasonalChart() {
        ImGui::Text("Housing Demand:");
        for (int m = 0; m < 12; m++) {
            float demand = static_cast<float>(GetHouseDemand(m));
            ImGui::SameLine();
            ImGui::VSliderFloat(("##h" + std::to_string(m)).c_str(),
                ImVec2(20, 60), &demand, 0.5f, 1.5f, "");
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Month %d: %.1f", m + 1, demand);
            }
        }
    }

    int CountBuildingsInProgress(Player* player) {
        int count = 0;
        for (auto* building : player->GetBuildings()) {
            if (!building->IsCompleted()) count++;
        }
        return count;
    }

    int CountMarketsInProgress(Player* player) {
        int count = 0;
        for (auto* market : player->GetSupermarkets()) {
            if (!market->IsCompleted()) count++;
        }
        return count;
    }

    void ProcessNextMonth() {
        if (!game_running || current_month >= total_months) return;

        current_month++;

        for (auto* player : players) {
            player->SaveState();
        }

        for (auto* player : players) {
            player->UpdateMonthly();
        }

        for (auto* player : players) {
            if (!player->IsRealPlayer()) {
                player->MakeDecision(current_month, total_months);
            }
        }

        auto all_buildings = GetAllBuildingsFromPlayers();
        int total_supermarkets = GetTotalCompletedSupermarkets();
        realtor.ProcessHousingSales(all_buildings, current_month, total_supermarkets);

        int total_houses = GetTotalHousesInArea();
        for (auto* player : players) {
            for (auto* market : player->GetSupermarkets()) {
                market->SupermarketMonthlyProfit(current_month, total_houses);
            }
        }

        for (auto* player : players) {
            player->CollectIncome();
        }

        if (current_month >= total_months) {
            game_running = false;
            show_game_over = true;
            game_over_window.Show();
        }
        else {
            game_running = false;
            show_monthly_results = true;
            results_window.Show(current_month);
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1400, 900), "Investment Game - Real Estate Tycoon");
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(1.3f);
    ImGui::GetIO().FontGlobalScale = 1.3f;

    GameUI game_ui;
    SetupWindow setup_window;

    sf::Clock delta_clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }

        ImGui::SFML::Update(window, delta_clock.restart());

        if (!game_ui.IsGameStarted()) {
            game_ui.RenderSetupWindow(setup_window);
        }
        else {
            game_ui.RenderUI();
        }

        window.clear(sf::Color(35, 35, 40));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}