#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>
#include "cStarterGUI.h"

class cRequest
{

public:
    std::string id;
    int start;
    int end;
    int value;

    bool isOverlap(const cRequest &other) const;

    void display()
    {
        std::cout << id << "\t" << start << "\t" << end << "\t" << value << "\n";
    }
};

class cSolution
{
public:
    std::vector<cRequest> myRequest;
    std::vector<cRequest> myAccepted;

    void readfile(const std::string &fname);
    void sort();
    void displayRequests();
    void displaySolution();
    void solve();
};

bool cRequest::isOverlap(const cRequest &other) const
{
    // allow coming and going on same day
    // bool ret = ( start < other.start && end > other.start ||
    //     start > other.start && start < other.end );

    // disallow coming and going on same day
    bool ret = (start <= other.start && end >= other.start ||
                start >= other.start && start <= other.end);

    std::cout << start << " " << end << " " << other.start << " " << other.end << " " << ret << "\n";
    return ret;
}

void cSolution::readfile(const std::string &fname)
{
    std::ifstream ifs(fname);
    if (!ifs.is_open())
        throw std::runtime_error(
            "Cannot open input");
    cRequest R;
    while (ifs.good())
    {
        ifs >> R.id >> R.start >> R.end >> R.value;
        myRequest.push_back(R);
    }
}
void cSolution::sort()
{
    std::sort(
        myRequest.begin(), myRequest.end(),
        [](const cRequest &a, const cRequest &b)
        {
            return a.start < b.start;
        });
}
void cSolution::displayRequests()
{
    for (auto &R : myRequest)
        R.display();
}
void cSolution::displaySolution()
{
    int V = 0;
    for (auto &R : myAccepted) {
        R.display();
        V += R.value;
    }
    std::cout << "Total Value " << V << "\n";
}
void cSolution::solve()
{
    sort();
    cRequest Rbest;
    int booked = 0;
    for (auto &R : myRequest)
    {
        if( R.start <= booked )
        continue;

        Rbest = R;
        for (auto &R2 : myRequest)
        {
            if (R.id == R2.id)
                continue;
            if (R.isOverlap(R2))
            {
                if( R2.value > Rbest.value )
                    Rbest = R2;
            }
        }
        myAccepted.push_back(Rbest);
        booked = Rbest.end;
    }
}
class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Starter",
              {50, 50, 1000, 500}),
          lb(wex::maker::make<wex::label>(fm))
    {
        lb.move(50, 50, 100, 30);
        lb.text("Hello World");

        show();
        run();
    }

private:
    wex::label &lb;
};

main()
{
    // cGUI theGUI;

    cSolution S;
    S.readfile("rent.txt");

    S.displayRequests();

    S.solve();

    S.displaySolution();

    return 0;
}
