#include <iostream>
#include <math.h>
#include <chrono>
#include <vector>
using namespace std;

typedef double number;



number vectorAccumulator(vector<number>* v){
    number sum=0.0;
    for (auto it = (*v).begin(); it != (*v).end(); it++)
         sum+=(*it);
    return sum;
}


number myFunction(number x)
{
    return 4 * (1.0 / (1 + pow(x, 2)));
}

template <typename T>
vector<T> linspace(number start, number end, number num)
{
    vector<T> linspaced;

    if (0 != num)
    {
        if (1 == num)
        {
            linspaced.push_back(static_cast<T>(start));
        }
        else
        {
            number delta = (end - start) / (num - 1);

            for (auto i = 0; i < (num - 1); ++i)
            {
                linspaced.push_back(static_cast<T>(start + delta * i));
            }
            // ensure that start and end are exactly the same as the input
            linspaced.push_back(static_cast<T>(end));
        }
    }
    return linspaced;
}

template <typename T>
T trapIntegration(number (*f)(number), number a, number b, number N)
{
    number dx = (number)(b - a) / N;
    vector<number> g1 = linspace<number>(a, b, N+1);

    vector<number> tempVector;
   std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    auto it = g1.begin();
    while(it <( g1.end() -1)){
        number value=(f(*it) + (f(*++it)));
        tempVector.push_back(static_cast<T>(value));
    }

    tempVector.resize(N-1);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

    
   // cout<< tempVector[0]  << endl;
    return vectorAccumulator(&tempVector) * (dx /2.0);
}


auto dummySinFunction (number x){
    return sin(static_cast<number>(x));
}

int main()
{

    // cout << "Hello world" << endl;

    // vector<number> g1 = linspace<number>(0, 1, 5);

    //  for (auto it = g1.begin(); it != g1.end(); it++)
    //      cout << *it << endl;

    //  cout<<"Average number"<<endl;
    using namespace std::chrono; 
    double pi = 2*asin(1.0);
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    cout<<"Doing calculations" <<endl;
    number g2 = trapIntegration<number>(*myFunction, 0, 1, 12000);
    cout<<"value " << g2 <<endl;
    printf(" %.9lf\n", g2);
    cout<<"DONEh " <<endl;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[ns]" << std::endl;
    return 0;
}
