#define PI 3.141592653589793238462643383279

/* double degToRad(double deg){return deg * PI / 180;}

double sin(double ) */

#include <cmath>
#include <iostream>
using namespace std;


double clean(double v) {
    return fabs((long double)v) < 1e-13 ? 0.0 : v; // long double because cos sin only work with float and long double not double... what the.. or IDE buggy
}

void discreteFourierTransformation(double* inreal, double* inimag, double* outreal, double* outimag, int size){
    for (int k = 0; k < size; k++) {
        double sumreal = 0;
        double sumimag = 0;
        for (int t = 0; t < size; t++) {
            long double angle = 2 * PI * t * k / size;  // long double because cos sin only work with float and long double not double... what the.. 
            sumreal += inreal[t] * cos(angle) + inimag[t] * sin(angle);
            sumimag += -inreal[t] * sin(angle) + inimag[t] * cos(angle);
        }
        outreal[k] = sumreal;
        outimag[k] = sumimag;
    }
}

void inverseDFT(double* inreal, double* inimag, double* outreal, double* outimag, int size){
    for (int k = 0; k < size; k++) {
        double sumreal = 0;
        double sumimag = 0;
        for (int t = 0; t < size; t++) {
            long double angle = 2 * PI * t * k / size;
            sumreal += inreal[t] * cos(angle) - inimag[t] * sin(angle);  
            sumimag += inreal[t] * sin(angle) + inimag[t] * cos(angle);  
        }
        outreal[k] = sumreal / size; 
        outimag[k] = sumimag / size;
    }
}

/* int main(){
    int n = 4;
    double inreal[]  = {1, 2, 3, 4};
    double inimag[]  = {0, 0, 0, 0};  // rein reelles Signal
    
    for (int k = 0; k < n; k++) {
        cout << "X_INPUT[" << k << "] = "
                << clean(inreal[k])
                << " + "
                << clean(inimag[k])
                << "i\n";
    }
    

    double outreal[4];
    double outimag[4];

    discreteFourierTransformation(inreal,
                                  inimag,
                                  outreal,
                                  outimag,
                                  n);
    

    for (int k = 0; k < n; k++) {
        cout << "X_TRANS[" << k << "] = "
                << clean(outreal[k])
                << " + "
                << clean(outimag[k])
                << "i\n";
    }
    double inversereal[4];
    double inverseimag[4];
    inverseDFT(outreal, outimag, inversereal, inverseimag, n);

    for (int k = 0; k < n; k++) {
        cout << "X_INVER[" << k << "] = "
                << clean(inversereal[k])
                << " + "
                << clean(inverseimag[k])
                << "i\n";
    }
} */