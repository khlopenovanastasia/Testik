#include "oop.cpp"
#include "funk.cpp"

int main()
{
    Supermarket first;
    first.progress_ = 1;
    first.UpdateConstructionProgress();
    for (int i = 0; i < supermarket_completed.size(); i++)
    {
        cout << supermarket_completed[i]->GetPrice();
    }
    //cout << first.GetName() << endl;
    cout << first.GetPrice() << endl;
    cout << first.GetPeriod() << endl;
    cout << first.earnings_ << endl;
    //cout << first.GetFlats() << endl;
}