#include "inplace/bubblesort.cpp"
#include "mergesort.cpp"
#include "inplace/quicksort.cpp"
#include "inplace/selectionsort.cpp"
#include "inplace/insertionsort.cpp"
#include <random>
#include <iostream>
using namespace std;

int main(){
    static const int size = 100;
    double lower_bound = 0;
    double upper_bound = size*12;
    double toSort[size]{};

    uniform_real_distribution<double> unif(lower_bound,
                                           upper_bound);

    default_random_engine re;

    // Getting a random double value
    cout << "[";
    for(int i = 0; i<size; i++){
        toSort[i]=unif(re);
        cout << toSort[i];
        if(i<size-1) cout << " ,";
    }
    cout << "]" << endl;
    
    insertionsort(toSort, size);
    cout << "[";
    for(int i = 0; i<size; i++){
        cout << toSort[i];
        if(i<size-1) cout << " ,";
    }
    cout << "]" << endl;    
}