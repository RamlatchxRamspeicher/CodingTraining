#include "fourier.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <cmath>
#include <cstdio>
#include <chrono>

unsigned char* _loadImg(const char imgPath[], int* width, int* height, int* channels){
    unsigned char* img = stbi_load(imgPath, width, height, channels, 0);
    if (!img) { printf("Fehler beim Laden\n"); throw("");}
    printf("Bild: %dx%d, %d Kanal/Kanaele\n", &width, &height, &channels);
    return img;
}
int _toRealImag(const char imgPath[], double*** paddedreal, double *** paddedimag, double*** outReal, double*** outImag, int* width, int* height, int* channels, int* paddedWidth, int* paddedHeight){
    unsigned char* img;
    try{img = _loadImg(imgPath,width,height,channels);}catch(...){return 1;}
    
    auto nextPow2 = [](int n) {
        int p = 1;
        while (p < n) p *= 2;
        return p;
    };

    *paddedWidth  = nextPow2(*width);
    *paddedHeight = nextPow2(*height);

    // Neue Arrays mit Nullen
    double*** realPadded = new double**[*channels];
    double*** imagPadded = new double**[*channels];
    double*** outReal = new double**[*channels];
    double*** outImag = new double**[*channels];
    for (int c = 0; c < *channels; c++) {
        realPadded[c] = new double*[*paddedHeight];
        imagPadded[c] = new double*[*paddedHeight];
        outReal[c] = new double*[*paddedHeight];
        outImag[c] = new double*[*paddedHeight];
        for (int y = 0; y < *paddedHeight; y++) {
            realPadded[c][y] = new double[*paddedWidth]{};  // {} = mit Nullen
            imagPadded[c][y] = new double[*paddedWidth]{};
            outReal[c][y] = new double[*width]{};
            outImag[c][y] = new double[*width]{};
            if (y < *height) {
                for (int x = 0; x < *width; x++) {
                    realPadded[c][y][x] = img[(y * *width + x) * *channels + c] / 255.0;
                }
            }
        }
    }
    stbi_image_free(img);
}
int _saveImg(const char imgPath[], double*** outReal, double*** outImag, int width, int height, int channels){
    unsigned char* toImage = new unsigned char[width * height * channels];
    for (int c = 0; c < channels; c++) {

        double maxMag = 0;
        double** mag = new double*[height];
        for (int y = 0; y < height; y++) {
            mag[y] = new double[width]{};
            for (int x = 0; x < width; x++) {
                mag[y][x] = log(1 + sqrt(
                    outReal[c][y][x] * outReal[c][y][x] +
                    outImag[c][y][x] * outImag[c][y][x]
                ));
                if (mag[y][x] > maxMag) maxMag = mag[y][x];
            }
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                toImage[(y * width + x) * channels + c] =
                    (unsigned char)(mag[y][x] / maxMag * 255);
            }
            delete[] mag[y];
        }
        delete[] mag;
    }
    stbi_write_png(imgPath, width, height, channels, toImage, width * channels);
    delete[] toImage;
    printf("Bild gespeichert: %s\n",imgPath);
    for (int c = 0; c < channels; c++) {
        for (int y = 0; y < height; y++) {
            delete[] outReal[c][y]; delete[] outImag[c][y];
        }
        delete[] outReal[c];  delete[] outImag[c];
    }
    delete[] outReal;  delete[] outImag;
}


int _from_to_Spectrum(const char imgPath[], bool to){
    int width, height, channels, paddedwidth, paddedheight;
    double*** real;
    double*** imag;
    double*** outReal;
    double*** outImag;
    int err =0;
    if(_toRealImag(imgPath, real, imag, outReal, outImag, &width, &height, &channels, &paddedwidth, &paddedheight)!=0) return 1;
    
    if(to)   iterFFT2D_multichannel(real, imag, outReal, outImag, paddedwidth, paddedheight, channels);
    else    iteriFFT2D_multichannel(real, imag, outReal, outImag, paddedwidth, paddedheight, channels);

    for (int c = 0; c < channels; c++) {
        for (int y = 0; y < height; y++) {
            delete[] real[c][y]; delete[] imag[c][y];
        }
        delete[] real[c];  delete[] imag[c];
    }
    delete[] real;  delete[] imag;

    const char* outPath = (to) ? "spectrum.png" : "recovered.png";
    if(_saveImg(outPath,outReal,outImag,width,height,channels)!=0) return 1;
    return 0;
}
int toSpectrum(const char imgPath[]){
    return _from_to_Spectrum(imgPath,true);
}
int fromSpectrum(const char imgPath[]){
    return _from_to_Spectrum(imgPath,false);
}
int getSpectrum2(const char imgPath[]){
    int width, height, channels;
    unsigned char* img = stbi_load(imgPath, &width, &height, &channels, 0);
    if (!img) { printf("Fehler beim Laden\n"); return 1; }
    printf("Bild: %dx%d, %d Kanal/Kanaele\n", width, height, channels);
    double*** real = new double**[channels];
    double*** imag = new double**[channels];
    for (int c = 0; c < channels; c++) {
        real[c] = new double*[height];
        imag[c] = new double*[height];
        for (int y = 0; y < height; y++) {
            real[c][y] = new double[width]{};
            imag[c][y] = new double[width]{};
            for (int x = 0; x < width; x++) {
                real[c][y][x] = img[(y * width + x) * channels + c] / 255.0;
            }
        }
    }
    stbi_image_free(img);

    double*** outReal = new double**[channels];
    double*** outImag = new double**[channels];
    for (int c = 0; c < channels; c++) {
        outReal[c] = new double*[height];
        outImag[c] = new double*[height];
        for (int y = 0; y < height; y++) {
            outReal[c][y] = new double[width]{};
            outImag[c][y] = new double[width]{};
        }
    }

    iterFFT2D_multichannel(real, imag, outReal, outImag, width, height, channels);

    unsigned char* spectrum = new unsigned char[width * height * channels];
    for (int c = 0; c < channels; c++) {

        double maxMag = 0;
        double** mag = new double*[height];
        for (int y = 0; y < height; y++) {
            mag[y] = new double[width]{};
            for (int x = 0; x < width; x++) {
                mag[y][x] = log(1 + sqrt(
                    outReal[c][y][x] * outReal[c][y][x] +
                    outImag[c][y][x] * outImag[c][y][x]
                ));
                if (mag[y][x] > maxMag) maxMag = mag[y][x];
            }
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                spectrum[(y * width + x) * channels + c] =
                    (unsigned char)(mag[y][x] / maxMag * 255);
            }
            delete[] mag[y];
        }
        delete[] mag;
    }
    stbi_write_png("spectrum.png", width, height, channels, spectrum, width * channels);
    delete[] spectrum;
    printf("Spektrum gespeichert: spectrum.png\n");
    for (int c = 0; c < channels; c++) {
        for (int y = 0; y < height; y++) {
            delete[] real[c][y];    delete[] imag[c][y];
            delete[] outReal[c][y]; delete[] outImag[c][y];
        }
        delete[] real[c];     delete[] imag[c];
        delete[] outReal[c];  delete[] outImag[c];
    }
    delete[] real;     delete[] imag;
    delete[] outReal;  delete[] outImag;
    printf("aufgeraumt\n");
    return 0;
}
int fromSpectrum2(const char imgPath[]){
    int width, height, channels;
    unsigned char* img = stbi_load(imgPath, &width, &height, &channels, 0);
    if (!img) { printf("Fehler beim Laden\n"); return 1; }
    printf("Bild: %dx%d, %d Kanal/Kanaele\n", width, height, channels);
    double*** backReal = new double**[channels];
    double*** backImag = new double**[channels];
    for (int c = 0; c < channels; c++) {
        backReal[c] = new double*[height];
        backImag[c] = new double*[height];
        for (int y = 0; y < height; y++) {
            backReal[c][y] = new double[width]{};
            backImag[c][y] = new double[width]{};
            for (int x = 0; x < width; x++) {
                backReal[c][y][x] = img[(y * width + x) * channels + c] / 255.0;
            }
        }
    }
    stbi_image_free(img);

    double*** outReal = new double**[channels];
    double*** outImag = new double**[channels];
    for (int c = 0; c < channels; c++) {
        outReal[c] = new double*[height];
        outImag[c] = new double*[height];
        for (int y = 0; y < height; y++) {
            outReal[c][y] = new double[width]{};
            outImag[c][y] = new double[width]{};
        }
    }
    iteriFFT2D_multichannel(backReal, backReal, outReal, outImag, width, height, channels);

    unsigned char* recovered = new unsigned char[width * height * channels];
    for (int c = 0; c < channels; c++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                double v = backReal[c][y][x] * 255.0;
                if (v < 0) v = 0;
                if (v > 255) v = 255;
                recovered[(y * width + x) * channels + c] = (unsigned char)round(v);
            }
        }
    }
    stbi_write_png("recovered.png", width, height, channels, recovered, width * channels);
    delete[] recovered;
    printf("Ruecktransformiert gespeichert: recovered.png\n");
    for (int c = 0; c < channels; c++) {
        for (int y = 0; y < height; y++) {
            delete[] backReal[c][y];    delete[] backImag[c][y];
            delete[] outReal[c][y]; delete[] outImag[c][y];
        }
        delete[] backReal[c];     delete[] backImag[c];
        delete[] outReal[c];  delete[] outImag[c];
    }
    delete[] backReal;     delete[] backImag;
    delete[] outReal;  delete[] outImag;
    printf("aufgeraumt\n");
    return 0;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    // 1. Bild laden
    int width, height, channels;
    unsigned char* img = stbi_load("testimg.jpg", &width, &height, &channels, 0);
    if (!img) { printf("Fehler beim Laden\n"); return 1; }
    printf("Bild: %dx%d, %d Kanal/Kanaele\n", width, height, channels);

    // 2. In double-Arrays umwandeln (planar: [kanal][y][x])
    double*** real = new double**[channels];
    double*** imag = new double**[channels];
    for (int c = 0; c < channels; c++) {
        real[c] = new double*[height];
        imag[c] = new double*[height];
        for (int y = 0; y < height; y++) {
            real[c][y] = new double[width]{};
            imag[c][y] = new double[width]{};
            for (int x = 0; x < width; x++) {
                real[c][y][x] = img[(y * width + x) * channels + c] / 255.0;
            }
        }
    }
    stbi_image_free(img);
    auto nextPow2 = [](int n) {
        int p = 1;
        while (p < n) p *= 2;
        return p;
    };

    int paddedWidth  = nextPow2(width);
    int paddedHeight = nextPow2(height);

    // Neue Arrays mit Nullen
    double*** realPadded = new double**[channels];
    double*** imagPadded = new double**[channels];
    for (int c = 0; c < channels; c++) {
        realPadded[c] = new double*[paddedHeight];
        imagPadded[c] = new double*[paddedHeight];
        for (int y = 0; y < paddedHeight; y++) {
            realPadded[c][y] = new double[paddedWidth]{};  // {} = mit Nullen
            imagPadded[c][y] = new double[paddedWidth]{};
            if (y < height) {
                for (int x = 0; x < width; x++) {
                    realPadded[c][y][x] = real[c][y][x];  // original kopieren
                }
            }
        }
    }
    // 3. FFT pro Kanal
    double*** outReal = new double**[channels];
    double*** outImag = new double**[channels];
    for (int c = 0; c < channels; c++) {
        outReal[c] = new double*[paddedHeight];
        outImag[c] = new double*[paddedHeight];
        for (int y = 0; y < paddedHeight; y++) {
            outReal[c][y] = new double[paddedWidth]{};
            outImag[c][y] = new double[paddedWidth]{};
        }
    }
    iterFFT2D_multichannel(realPadded, imagPadded, outReal, outImag, paddedWidth, paddedHeight, channels);

    // 4. Spektrum speichern (Magnitude, logarithmisch, pro Kanal)
    unsigned char* spectrum = new unsigned char[width * height * channels];
    for (int c = 0; c < channels; c++) {
        double maxMag = 0;
        double** mag = new double*[height];
        for (int y = 0; y < height; y++) {
            mag[y] = new double[width]{};
            for (int x = 0; x < width; x++) {
                mag[y][x] = log(1 + sqrt(
                    outReal[c][y][x] * outReal[c][y][x] +
                    outImag[c][y][x] * outImag[c][y][x]
                ));
                if (mag[y][x] > maxMag) maxMag = mag[y][x];
            }
        }
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++)
                spectrum[(y * width + x) * channels + c] =
                    (unsigned char)(mag[y][x] / maxMag * 255);
            delete[] mag[y];
        }
        delete[] mag;
    }
    stbi_write_png("spectrum.png", width, height, channels, spectrum, width * channels);
    delete[] spectrum;
    printf("Spektrum gespeichert: spectrum.png\n");

    // 5. iFFT (auf gepaddeten Arrays)
    double*** backReal = new double**[channels];
    double*** backImag = new double**[channels];
    for (int c = 0; c < channels; c++) {
        backReal[c] = new double*[paddedHeight];
        backImag[c] = new double*[paddedHeight];
        for (int y = 0; y < paddedHeight; y++) {
            backReal[c][y] = new double[paddedWidth]{};
            backImag[c][y] = new double[paddedWidth]{};
        }
    }
    iteriFFT2D_multichannel(outReal, outImag, backReal, backImag, paddedWidth, paddedHeight, channels);

    // 6. Rücktransformiertes Bild — nur originale Dimensionen
    unsigned char* recovered = new unsigned char[width * height * channels];
    for (int c = 0; c < channels; c++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                double v = backReal[c][y][x] * 255.0;
                if (v < 0) v = 0;
                if (v > 255) v = 255;
                recovered[(y * width + x) * channels + c] = (unsigned char)round(v);
            }
        }
    }
    stbi_write_png("recovered.png", width, height, channels, recovered, width * channels);
    delete[] recovered;
    printf("Ruecktransformiert gespeichert: recovered.png\n");

    // 7. Vergleich — nur originale Dimensionen
    for (int c = 0; c < channels; c++) {
        double maxErr = 0, avgErr = 0;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                double err = fabs(real[c][y][x] - backReal[c][y][x]);
                if (err > maxErr) maxErr = err;
                avgErr += err;
            }
        }
        avgErr /= width * height;
        printf("Kanal %d — Max Fehler: %.2e, Avg Fehler: %.2e\n", c, maxErr, avgErr);
    }

    // 8. Aufräumen
    for (int c = 0; c < channels; c++) {
        for (int y = 0; y < height; y++) {
            delete[] real[c][y]; delete[] imag[c][y];
        }
        delete[] real[c]; delete[] imag[c];

        for (int y = 0; y < paddedHeight; y++) {
            delete[] realPadded[c][y]; delete[] imagPadded[c][y];
            delete[] outReal[c][y];    delete[] outImag[c][y];
            delete[] backReal[c][y];   delete[] backImag[c][y];
        }
        delete[] realPadded[c]; delete[] imagPadded[c];
        delete[] outReal[c];    delete[] outImag[c];
        delete[] backReal[c];   delete[] backImag[c];
    }
    delete[] real;      delete[] imag;
    delete[] realPadded; delete[] imagPadded;
    delete[] outReal;   delete[] outImag;
    delete[] backReal;  delete[] backImag;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Execution time: " << elapsed.count() << " ms" << std::endl;
    return 0;
}