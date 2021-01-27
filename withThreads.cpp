#include <iostream>
#include <math.h>
#include <chrono>
#include <vector>
#include <thread>
#include "mutex"
#include <future>
using namespace std;

typedef long double number;

mutex mtx;

int count = 0;

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

typedef vector<number>::iterator iter;

  void worker (iter begin, iter end, std::vector<number>::iterator itr, number (*f)(number)) {
      
    for(auto it = begin; it != end; ++it) {
      auto tempIt = it; 
      number value=(f(*tempIt) + (f(*++tempIt)));
      //lock_guard<mutex> lck(mtx);
      //count++;
      *itr = (static_cast<number>(value));
      itr++;
      
    }
  };


template <typename T>
T trapIntegration(number (*f)(number), number a, number b, number N)
{
    number dx = (number)(b - a) / N;
    vector<number> g1 = linspace<number>(a, b, N+1);

    vector<number> tempVector;

    vector<std::future<void>> futuresV(5);
    //vector<vector<number>> results(100);
    vector<number> results(N+1);

    const int grainsize = (N+1) / 5;
      std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    auto work_iter = std::begin(g1);
    auto res_it= std::begin(results);

    for(auto [it] = std::tuple<std::vector<std::future<void>>::iterator>{std::begin(futuresV)} ; it != std::end(futuresV)-1; ++it) {
        if(work_iter+grainsize < ( g1.end() -1)){
            //*it = thread(worker, work_iter, work_iter + grainsize, ref(*res_it), f);
            *it  = std::async(std::launch::async, worker, work_iter, work_iter + grainsize, res_it, f);
			work_iter += grainsize;	
            res_it+=grainsize;
		}
        else{
           // *it = std::thread(worker, work_iter, g1.end(), ref(*res_it), f);
           *it  = std::async(std::launch::async, worker, work_iter, g1.end(), res_it, f);
			work_iter += grainsize;
            res_it+=grainsize;
        }
    }

    if(work_iter < g1.end()){
        futuresV.back() = std::async(std::launch::async, worker, work_iter,g1.end()-1, res_it, f);
    }

     

    	for(auto &&i : futuresV) {
          //  cout<< "join" <<endl;
			i.get();
		}

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;



    cout<<"ENDED THREDING" << count <<endl;

    //tempVector.resize(N-1);

    number totalResult = 0;

  

    // for(auto &&v : results){
    //     v.resize(N-1);
    //     totalResult += vectorAccumulator(&v);
    // }
    cout<<vectorAccumulator(&results)<<endl;
    //return totalResult *(dx /2.0);
    return (totalResult + vectorAccumulator(&results)) * (dx /2.0);
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
    cout<<"BEGIN"<<endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    double pi = 2*asin(1.0);
    number g2 = trapIntegration<number>(*myFunction, 0, 1, 12000);
    cout<<"value " << g2 <<endl;
    cout<<"END"<<endl;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[ns]" << std::endl;
    return 0;
}
