#include <stdexcept>
#include <omp.h>
#include <cmath>
#define PI 3.141592653589793238462643383279
//pow2
void _FFT_IFFT(double* inreal, double* inimag, double* outreal, double* outimag, int size, bool inverse){
    int check = size;
    if(check == 1){
        outreal[0] = inreal[0];
        outimag[0] = inimag[0];
        return;
    }
    while(check%2==0){check/=2;}
    if(check!=1) throw std::runtime_error("size not 2^x");

    double* inEvenReal = new double[size/2]{};
    double* inEvenImag = new double[size/2]{};
    double* inOddReal = new double[size/2]{};
    double* inOddImag = new double[size/2]{};
    double* outEvenReal = new double[size/2]{};
    double* outEvenImag = new double[size/2]{};
    double* outOddReal = new double[size/2]{};
    double* outOddImag = new double[size/2]{};

    for(int n=0; n<size/2; n++){
        inEvenReal[n] = inreal[2*n];
        inEvenImag[n] = inimag[2*n];
        inOddReal[n]   = inreal[2*n+1];
        inOddImag[n]   = inimag[2*n+1];
    }
    _FFT_IFFT(inEvenReal,inEvenImag,outEvenReal,outEvenImag,size/2,inverse);
    delete[] inEvenReal; delete[] inEvenImag; 
    _FFT_IFFT(inOddReal,inOddImag,outOddReal,outOddImag,size/2,inverse);
    delete[] inOddReal; delete[] inOddImag;
    for(int k=0; k<size/2; k++){
        long double angle = -2 * PI * k/size;
        if(inverse) angle *= -1;
        double wReal = cos(angle);
        double wImag = sin(angle);

        double tReal = wReal * outOddReal[k] - wImag * outOddImag[k];
        double tImag = wReal * outOddImag[k] + wImag * outOddReal[k];

        outreal[k]        = outEvenReal[k] + tReal;
        outimag[k]        = outEvenImag[k] + tImag;
        outreal[k+size/2] = outEvenReal[k] - tReal;
        outimag[k+size/2] = outEvenImag[k] - tImag;
        if(inverse){
            outreal[k] /=2;
            outimag[k] /=2;
            outreal[k+size/2] /=2;
            outimag[k+size/2] /=2;
        }
    }
    delete[] outEvenReal; delete[] outEvenImag; delete[] outOddReal; delete[] outOddImag;
}

void FFT(double* inreal, double* inimag, double* outreal, double* outimag, int size){
    _FFT_IFFT(inreal,inimag,outreal,outimag,size,false);
}
void iFFT(double* inreal, double* inimag, double* outreal, double* outimag, int size){
    _FFT_IFFT(inreal,inimag,outreal,outimag,size,true);
}

//Bluestein FFT-> faltung wie in studium gelernt
// normale DFT      X[k] = Σ x[n] * e^{-i 2π nk/N}
// bluestein nutzt n*k = -(n-k)²/2 + n²/2 + k²/2 daraus folgt Faltung
//                  X[k] = e^{-iπk²/N} * Σ (x[n] * e^{-iπn²/N}) * e^{iπ(n-k)²/N}
// Faltung ist groesse egal -> allgemein nutzbar 
void _faltung(double* aReal, double* aImag, double* bReal, double* bImag, double* outReal, double* outImag, int size){
    double* frequenzAReal = new double[size]{};
    double* frequenzAImag = new double[size]{};
    double* frequenzBReal = new double[size]{};
    double* frequenzBImag = new double[size]{};

    FFT(aReal,aImag,frequenzAReal,frequenzAImag,size);
    FFT(bReal,bImag,frequenzBReal,frequenzBImag,size);

    double* faltungsProduktReal = new double[size]{};
    double* faltungsProduktImag = new double[size]{};

    for(int i=0; i<size; i++){
        faltungsProduktReal[i] = frequenzAReal[i] * frequenzBReal[i] - frequenzAImag[i] * frequenzBImag[i];
        faltungsProduktImag[i] = frequenzAReal[i] * frequenzBImag[i] + frequenzAImag[i] * frequenzBReal[i];
    }
    delete[] frequenzAReal; delete[] frequenzAImag;
    delete[] frequenzBReal; delete[] frequenzBImag;

    iFFT(faltungsProduktReal, faltungsProduktImag, outReal, outImag, size);
    delete[] faltungsProduktReal; delete[] faltungsProduktImag;
}
// a[n] = x[n] * e^{-iπn²/N}
// b[j] = e^{iπj²/N} j= n-k
// b ist convolution kernel 
void _bluesteinFFT(double* inreal, double* inimag, double* outreal, double* outimag, int size, bool inverse){
    int paddedSize = 1;
    while(paddedSize < 2*size-1) paddedSize *=2;
    double* aReal = new double[paddedSize]{};
    double* aImag = new double[paddedSize]{};

    for(int i =0; i<size; i++){
        long double angle = -PI *i*i/size;
        if(inverse) angle *=-1;
        double wReal = cos(angle);
        double wImag = sin(angle);
        aReal[i]= inreal[i] * wReal - inimag[i] * wImag;
        aImag[i]= inreal[i] * wImag + inimag[i] * wReal;
    }

    double* bReal = new double[paddedSize]{};
    double* bImag = new double[paddedSize]{};
    for(int i =0; i<size; i++){
        long double angle = PI *i*i/size;
        if(inverse) angle *=-1;
        double wReal = cos(angle);
        double wImag = sin(angle);
        bReal[i] = wReal;
        bImag[i] = wImag;
        if (i > 0) {
            //n-k kann negativ werden wenn k > n -> -2 selber kernel wie 2. Weiterhin 0 fuer [N,...,m-N] 
            bReal[paddedSize-i] = wReal;
            bImag[paddedSize-i] = wImag;
        }
    }

    double* convReal = new double[paddedSize]{};
    double* convImag = new double[paddedSize]{};
    _faltung(aReal,aImag,bReal,bImag,convReal,convImag,paddedSize);
    delete[] aReal; delete[] aImag;
    delete[] bReal; delete[] bImag;

    for(int k = 0; k<size; k++){
        long double angle = -PI * k * k / size;
        if(inverse) angle *=-1;
        double wReal = cos(angle);
        double wImag = sin(angle);
        outreal[k] = convReal[k] * wReal - convImag[k] * wImag;
        outimag[k] = convReal[k] * wImag + convImag[k] * wReal;
        if(inverse){
            outreal[k]/=size;
            outimag[k]/=size;
        }
    }
    delete[] convReal; delete[] convImag;    
}

void bluesteinFFT(double* inreal, double* inimag, double* outreal, double* outimag, int size){
    _bluesteinFFT(inreal, inimag, outreal, outimag, size, false);
}
void bluesteiniFFT(double* inreal, double* inimag, double* outreal, double* outimag, int size){
    _bluesteinFFT(inreal, inimag, outreal, outimag, size, true);
}
void _swap(double& a, double& b){
    double tmp = a;
    a = b;
    b=tmp;
}
//inplace with bitreversal
void _inplaceFFT(double* real, double* imag, int size, bool inverse){

    int bits = (int)log2(size);
    for (int i = 0; i<size; i++){
        int j = 0;
        int x = i;
        for(int b = 0; b< bits; b++){
            j = (j << 1) | (x & 1);
            x>>=1;
        }
        if(i<j) {
            _swap(real[i], real[j]);
            _swap(imag[i], imag[j]);
        }
    }

    for (int len = 2; len <= size; len *= 2) {
        double angle = 2.0 * PI / len;
        if (!inverse) angle *= -1;

        double wReal = cos(angle);
        double wImag = sin(angle);

        for (int i = 0; i < size; i += len) {
            // aktueller Drehfaktor, startet bei 1+0i
            double drehfaktorReal = 1.0;
            double drehfaktorImag = 0.0;
            for (int k = 0; k < len/2; k++) {
                int u = i + k;           // obere Hälfte
                int v = i + k + len/2;
                // t = cur * data[v]
                double tReal = drehfaktorReal * real[v] - drehfaktorImag * imag[v];
                double tImag = drehfaktorReal * imag[v] + drehfaktorImag * real[v];

                // Butterfly
                real[v] = real[u] - tReal;
                imag[v] = imag[u] - tImag;
                real[u] = real[u] + tReal;
                imag[u] = imag[u] + tImag;

                // Drehfaktor weiterrotieren
                double nextRe = drehfaktorReal * wReal - drehfaktorImag * wImag;
                double nextIm = drehfaktorReal * wImag + drehfaktorImag * wReal;
                drehfaktorReal = nextRe;
                drehfaktorImag = nextIm;
            }
        }
    }
// 3. Division durch n bei inverser FFT
    if (inverse) {
        for (int i = 0; i < size; i++) {
            real[i] /= size;
            imag[i] /= size;
        }
    }
}

void _interativeFFT2D(double** inreal, double** inimag, double** outreal, double** outimag, int width, int height, bool inverse){
    // Ergebnis-Arrays mit Eingabe befüllen (in-place Algorithmus)
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            outreal[y][x] = inreal[y][x];
            outimag[y][x] = inimag[y][x];
        }

    // Zeilen parallel
    #pragma omp parallel for
    for (int y = 0; y < height; y++)
        _inplaceFFT(outreal[y], outimag[y], width, inverse);

    // Spalten parallel
    #pragma omp parallel for
    for (int x = 0; x < width; x++) {
        double* colReal = new double[height];
        double* colImag = new double[height];

        for (int y = 0; y < height; y++) {
            colReal[y] = outreal[y][x];
            colImag[y] = outimag[y][x];
        }

        _inplaceFFT(colReal, colImag, height, inverse);

        for (int y = 0; y < height; y++) {
            outreal[y][x] = colReal[y];
            outimag[y][x] = colImag[y];
        }

        delete[] colReal;
        delete[] colImag;
    }
}

void iterFFT2D(double** inreal, double** inimag, double** outreal, double** outimag, int width, int height){
    _interativeFFT2D(inreal, inimag, outreal, outimag, width, height, false);
}
void iteriFFT2D(double** inreal, double** inimag, double** outreal, double** outimag, int width, int height){
    _interativeFFT2D(inreal, inimag, outreal, outimag, width, height, true);
}

void iterFFT2D_multichannel(
    double*** inreal, double*** inimag,
    double*** outreal, double*** outimag,
    int width, int height, int channels)
{
    for (int c = 0; c < channels; c++) {
        iterFFT2D(inreal[c], inimag[c], outreal[c], outimag[c], width, height);
    }
}

void iteriFFT2D_multichannel(
    double*** inreal, double*** inimag,
    double*** outreal, double*** outimag,
    int width, int height, int channels)
{
    for (int c = 0; c < channels; c++) {
        iteriFFT2D(inreal[c], inimag[c], outreal[c], outimag[c], width, height);
    }
}

void _FFT2D(double** inreal, double** inimag, double** outreal, double** outimag, int width, int height, bool inverse){
    double** tmpReal = new double*[height];
    double** tmpImag = new double*[height];
    for (int i = 0; i < height; i++) {
        tmpReal[i] = new double[width]{};
        tmpImag[i] = new double[width]{};
    }
    //zeilen
    //#pragma omp parallel for
    for (int y = 0; y < height; y++) {
        _bluesteinFFT(inreal[y], inimag[y], tmpReal[y], tmpImag[y], width, inverse);
    }
    //spalten
    //#pragma omp parallel for
    for (int x = 0; x < width; x++) {
        double* colReal = new double[height];
        double* colImag = new double[height];
        double* colOutReal = new double[height];
        double* colOutImag = new double[height];

        for (int y = 0; y < height; y++) {
            colReal[y] = tmpReal[y][x];
            colImag[y] = tmpImag[y][x];
        }
        _bluesteinFFT(colReal, colImag, colOutReal, colOutImag, height, inverse);
        delete[] colReal; delete[] colImag;
        
        for (int y = 0; y < height; y++) {
            outreal[y][x] = colOutReal[y];
            outimag[y][x] = colOutImag[y];
        }
        delete[] colOutReal; delete[] colOutImag;

    }
    //aufräumen
    for (int i = 0; i < height; i++) {
        delete[] tmpReal[i];
        delete[] tmpImag[i];
    }
    delete[] tmpReal; delete[] tmpImag;
}
void FFT2D(double** inreal, double** inimag, double** outreal, double** outimag, int width, int height){
    _FFT2D(inreal, inimag, outreal, outimag, width, height, false);
}
void iFFT2D(double** inreal, double** inimag, double** outreal, double** outimag, int width, int height){
    _FFT2D(inreal, inimag, outreal, outimag, width, height, true);
}

void FFT2D_multichannel(
    double*** inreal, double*** inimag,
    double*** outreal, double*** outimag,
    int width, int height, int channels)
{
    for (int c = 0; c < channels; c++) {
        FFT2D(inreal[c], inimag[c], outreal[c], outimag[c], width, height);
    }
}

void iFFT2D_multichannel(
    double*** inreal, double*** inimag,
    double*** outreal, double*** outimag,
    int width, int height, int channels)
{
    for (int c = 0; c < channels; c++) {
        iFFT2D(inreal[c], inimag[c], outreal[c], outimag[c], width, height);
    }
}

/* void FFT(double* inreal, double* inimag, double* outreal, double* outimag, int size){
    int check = size;
    if(check == 1){
        outreal[0] = inreal[0];
        outimag[0] = inimag[0];
        return;
    }
    while(check%2==0){check/=2;}
    if(check!=1) throw std::runtime_error("size not 2^x");

    double* inEvenReal = new double[size/2]{};
    double* inEvenImag = new double[size/2]{};
    double* inOddReal = new double[size/2]{};
    double* inOddImag = new double[size/2]{};
    double* outEvenReal = new double[size/2]{};
    double* outEvenImag = new double[size/2]{};
    double* outOddReal = new double[size/2]{};
    double* outOddImag = new double[size/2]{};

    for(int n=0; n<size/2; n++){
        inEvenReal[n] = inreal[2*n];
        inEvenImag[n] = inimag[2*n];
        inOddReal[n]   = inreal[2*n+1];
        inOddImag[n]   = inimag[2*n+1];
    }
    FFT(inEvenReal,inEvenImag,outEvenReal,outEvenImag,size/2);
    delete[] inEvenReal; delete[] inEvenImag; 
    FFT(inOddReal,inOddImag,outOddReal,outOddImag,size/2);
    delete[] inOddReal; delete[] inOddImag;
    for(int k=0; k<size/2; k++){
        long double angle = -2 * PI * k/size;
        double wReal = cos(angle);
        double wImag = sin(angle);

        double tReal = wReal * outOddReal[k] - wImag * outOddImag[k];
        double tImag = wReal * outOddImag[k] + wImag * outOddReal[k];

        outreal[k]        = outEvenReal[k] + tReal;
        outimag[k]        = outEvenImag[k] + tImag;
        outreal[k+size/2] = outEvenReal[k] - tReal;
        outimag[k+size/2] = outEvenImag[k] - tImag;
    }
    delete[] outEvenReal; delete[] outEvenImag; delete[] outOddReal; delete[] outOddImag;
}

void iFFT(double* inreal, double* inimag, double* outreal, double* outimag, int size){
    int check = size;
    if(check == 1){
        outreal[0] = inreal[0];
        outimag[0] = inimag[0];
        return;
    }
    while(check%2==0){check/=2;}
    if(check!=1) throw std::runtime_error("size not 2^x");

    double* inEvenReal = new double[size/2]{};
    double* inEvenImag = new double[size/2]{};
    double* inOddReal = new double[size/2]{};
    double* inOddImag = new double[size/2]{};
    double* outEvenReal = new double[size/2]{};
    double* outEvenImag = new double[size/2]{};
    double* outOddReal = new double[size/2]{};
    double* outOddImag = new double[size/2]{};

    for(int n=0; n<size/2; n++){
        inEvenReal[n] = inreal[2*n];
        inEvenImag[n] = inimag[2*n];
        inOddReal[n]   = inreal[2*n+1];
        inOddImag[n]   = inimag[2*n+1];
    }
    FFT(inEvenReal,inEvenImag,outEvenReal,outEvenImag,size/2);
    delete[] inEvenReal; delete[] inEvenImag; 
    FFT(inOddReal,inOddImag,outOddReal,outOddImag,size/2);
    delete[] inOddReal; delete[] inOddImag;
    for(int k=0; k<size/2; k++){
        long double angle = 2 * PI * k/size;
        double wReal = cos(angle);
        double wImag = sin(angle);

        double tReal = wReal * outOddReal[k] - wImag * outOddImag[k];
        double tImag = wReal * outOddImag[k] + wImag * outOddReal[k];

        outreal[k]        = (outEvenReal[k] + tReal)/2;
        outimag[k]        = (outEvenImag[k] + tImag)/2;
        outreal[k+size/2] = (outEvenReal[k] - tReal)/2;
        outimag[k+size/2] = (outEvenImag[k] - tImag)/2;
    }
    delete[] outEvenReal; delete[] outEvenImag; delete[] outOddReal; delete[] outOddImag;
} */