#include "headers.h"

using namespace std;

class Buildings {
    public:
        Buildings() {
            type_ = "Will be later";
            price_ = 0;
            months_ = 0;
            flats_ = 0;
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
        Supermarket() {
            name_ = "Will be later";
            price_ = 2500000;
            months_ = 5;
        }

        virtual string GetName() {
            return name_;
        }

        virtual long long GetPrice() {
            return price_;
        }

        virtual int GetPeriod() {
            return months_;
        }

        virtual ~Supermarket() = default;

    protected:
        string name_;
        long long price_;
        int months_;
};

int main()
{
    Supermarket first;
    cout << first.GetName() << endl;
    cout << first.GetPrice() << endl;
    cout << first.GetPeriod() << endl;
    //cout << first.GetFlats() << endl;
}