#include <cmath>
#define PI 3.141592653589793238462643383279

struct quaternion{
    double w; //real x0*1
    double i; //     x1*i
    double j; //     x2*j
    double k; //     x3*k
    quaternion& operator+(const quaternion& other){
        this->w += other.w;
        this->i += other.i;
        this->j += other.j;
        this->k += other.k;
        return *this;
    }
    quaternion& operator*(const quaternion& other){
        this->w = (this->w*other.w - this->i*other.i - this->j*other.j - this->k*other.k);
        this->i = (this->w*other.i + this->i*other.w + this->j*other.k - this->k*other.j);
        this->j = (this->w*other.j - this->i*other.k + this->j*other.w + this->k*other.i);
        this->k = (this->w*other.k + this->i*other.j - this->j*other.i + this->k*other.w);
        return *this;
    }
    quaternion inverse(){
        double divider = (w*w+i*i+j*j+k*k);
        return {(w/divider),
                (-i/divider),
                (-j/divider),
                (-k/divider)};
    }
    quaternion(double angle, eulerangle axis){
        w=cos(angle/2);
        i=sin(angle/2)*cos(axis.roll);
        j=sin(angle/2)*cos(axis.pitch);
        k=sin(angle/2)*cos(axis.jaw);
    }
    quaternion(double w, double i, double j, double k): w(w), i(i), j(j),k(k){}
    quaternion() = default;
};

struct RotationMatrix {
    double x[3]{};
    double y[3]{};
    double z[3]{};

    RotationMatrix(quaternion q){
        double w2,i2,j2,k2,wi,wj,wk,ij,ik,jk;
        w2 = q.w*q.w; i2 = q.i*q.i; j2 = q.j*q.j; k2 = q.k*q.k;
        wi = q.w*q.i; wj = q.w*q.j; wk = q.w*q.k; ij = q.i*q.j; ik = q.i*q.k; jk = q.j*q.k;
        x[0] = w2 + i2 - j2 - k2;
        x[1] = -2*wk + 2*ij;
        x[2] = 2*wj + 2*ik;

        y[0] = 2*wk + 2*ij;
        y[1] = w2 - i2 + j2 - k2;
        y[2] = -2*wi + 2*jk;

        z[0] = -2*wj + 2*ik;
        z[1] = 2*wi + 2*jk;
        z[2] = w2 - i2 - j2 + k2;
    }
};

struct eulerangle{
    double roll;
    double pitch;
    double jaw;
};