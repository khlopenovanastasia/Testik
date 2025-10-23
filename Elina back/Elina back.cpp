#include <iostream>
#include <random>
#include "C:\Users\elina\OneDrive\Документы\GitHub\Testik\Anastasia back\Anastasia back.cpp"

class Player {
protected:
    string name_;
    long long capital_;
    vector<Buildings*> buildings_;
    vector<Supermarket*> supermarkets_;
    int player_id_;
    static int player_count_;

public:
    Player(const string& name, long long initial_capital = 37000000)
        : name_(name), capital_(initial_capital) {
        player_id_ = player_count_++;
    }
    virtual ~Player() {
        for (auto building : buildings_) {
            delete building;
        }
        for (auto supermarket : supermarkets_) {
            delete supermarket;
        }
    }
    virtual void MakeDecision(int current_month, int total_months) = 0;
    string GetName() { return name_; }
    long long GetCapital() { return capital_; }
    int GetId() { return player_id_; }
    vector<Buildings*>& GetBuildings() { return buildings_; }
    vector<Supermarket*>& GetSupermarkets() { return supermarkets_; }

    bool InvestInPanelHouse() {
        if (capital_ >= 8000000) {
            capital_ -= 8000000;
            buildings_.push_back(new Panel());
            return true;
        }
        return false;
    }

    bool InvestInMonoHouse() {
        if (capital_ >= 12000000) {
            capital_ -= 12000000;
            buildings_.push_back(new Mono());
            return true;
        }
        return false;
    }

    bool InvestInBrickHouse() {
        if (capital_ >= 15000000) {
            capital_ -= 15000000;
            buildings_.push_back(new Brick());
            return true;
        }
        return false;
    }

    bool InvestInSupermarket() {
        if (capital_ >= 2500000) {
            capital_ -= 2500000;
            supermarkets_.push_back(new Supermarket());
            return true;
        }
        return false;
    }

    void UpdateMonthly() {
        for (auto building : buildings_) {
            building->UpdateConstructionProgress();
        }
        for (auto supermarket : supermarkets_) {
            supermarket->UpdateConstructionProgress();
        }
    }

    long long CalculateTotalAssets() {
        long long total_assets = capital_;
        for (const auto& building : buildings_) {
            total_assets += building->GetPrice();
        }
        for (const auto& supermarket : supermarkets_) {
            total_assets += supermarket->GetPrice();
        }
        return total_assets;
    }
};

int Player::player_count_ = 0;

class RealPlayer : public Player {
public:
    RealPlayer(const string& name, long long initial_capital = 37000000)
        : Player(name, initial_capital) {}
    bool SetChoice(int choice) {
        if (choice == 1) {
            if (InvestInPanelHouse()) {
                return true;
            }
            else {
                return false;
            }
        }
        else if (choice == 2) {
            if (InvestInMonoHouse()) {
                return true;
            }
            else {
                return false;
            }
        }
        else if (choice == 3) {
            if (InvestInBrickHouse()) {
                return true;
            }
            else {
                return false;
            }
        }
        else if (choice == 4) {
            if (InvestInSupermarket()) {
                return true;
            }
            else {
                return false;
            }
        }
    }


};

class AggressiveBot : public Player {
public:
    AggressiveBot(const string& name, long long initial_capital = 37000000)
        : Player(name, initial_capital) {}

    void MakeDecision(int current_month, int total_months) override {
        if (current_month <= total_months / 2) {
            if (InvestInBrickHouse()) {
                return;
            }
            if (InvestInMonoHouse()) {
                return;
            }
            if (InvestInPanelHouse()) {
                return;
            }
            if (InvestInSupermarket()) {
                return;
            }
        }
        else {
            if (InvestInSupermarket()) {
                return;
            }
            if (InvestInPanelHouse()) {
                return;
            }
        }
    }
};

class RandomBot : public Player {
public:
    RandomBot(const string& name, long long initial_capital = 37000000)
        : Player(name, initial_capital) {
    }

    void MakeDecision(int current_month, int total_months) override {
        vector<string> available_actions;
        if (capital_ >= 8000000) available_actions.push_back("panel");
        if (capital_ >= 12000000) available_actions.push_back("mono");
        if (capital_ >= 15000000) available_actions.push_back("brick");
        if (capital_ >= 2500000) available_actions.push_back("supermarket");
        available_actions.push_back("skip");
        int rn = 0;
        if (available_actions.size() != 1) {
            rn = rand() % available_actions.size();
        }
        string action = available_actions[rn];
        if (action == "panel") {
            InvestInPanelHouse();
        }
        else if (action == "mono") {
            InvestInMonoHouse();
        }
        else if (action == "brick") {
            InvestInBrickHouse();
        }
        else if (action == "supermarket") {
            InvestInSupermarket();
        }
    }
};

