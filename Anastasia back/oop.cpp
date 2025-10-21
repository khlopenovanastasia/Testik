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
    }

};

class Mono : public Buildings {
public:
    Mono() {
        type_ = "Mono";
        price_ = 12000000;
        months_ = 10;
        flats_ = 80;
    }
};

class Brick : public Buildings {
public:
    Brick() {
        type_ = "Brick";
        price_ = 15000000;
        months_ = 12;
        flats_ = 60;
    }
};

class Supermarket {
public:
    long long earnings_ = 0;
    int progress_ = months_;
    int id_;

    Supermarket() {
        name_ = "Will be later";
        price_ = 2500000;
        months_ = 5;
        id_ = rand() % 1000;
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

protected:
    string name_;
    long long price_;
    int months_;
};

class NewProject {
public:


};