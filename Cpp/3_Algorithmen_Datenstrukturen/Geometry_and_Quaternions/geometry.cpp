#include "quaternions.cpp"



struct position{
    double x;
    double y;
    double z;
    position operator+(position other){
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
};

struct odometry{
    position pos;
    eulerangle angle;
    odometry(double x, double y, double z, double R, double P, double Y): pos({x,y,z}),angle({R,P,Y}){}
    odometry(position pos, double R, double P, double Y): pos(pos),angle({R,P,Y}){}
    odometry(double x, double y, double z, eulerangle angle): pos({x,y,z}),angle(angle){}
    odometry(position pos, eulerangle angle): pos(pos),angle(angle){};
    odometry(double x, double y, double z, quaternion q): pos({x,y,z}){angle=toEuler(q);}
    odometry(position pos, quaternion q): pos(pos){angle=toEuler(q);}
    odometry(odomQ other): pos(other.pos){angle=toEuler(other.angle);}
    odometry() = default;
};

struct odomQ
{
    position pos;
    quaternion angle;
    odomQ(double x, double y, double z, double w, double i, double j, double k): pos({x,y,z}),angle({w,i,j,k}){}
    odomQ(position pos, double w, double i, double j, double k): pos(pos),angle({w,i,j,k}){}
    odomQ(double x, double y, double z, quaternion angle): pos({x,y,z}),angle(angle){}
    odomQ(position pos, quaternion angle): pos(pos),angle(angle){}
    odomQ(double x, double y, double z, eulerangle e): pos({x,y,z}){angle=toQuaternion(e);}
    odomQ(position pos, eulerangle e): pos(pos){angle=toQuaternion(e);}
    odomQ(odometry other): pos(other.pos){angle=toQuaternion(other.angle);}
};

using namespace std;
eulerangle toEuler(quaternion q){
    return {
        atan2(2* (q.w*q.i+q.j*q.k), 1-2*(q.i*q.i+q.j*q.j)),
        -PI/2 + 2*atan2(sqrt(1+2*(q.w*q.j-q.i*q.k)),sqrt(1-2*(q.w*q.j-q.i*q.k))),
        atan2(2* (q.w*q.k+q.i*q.j), 1-2*(q.j*q.j+q.k*q.k))
    };
}
quaternion toQuaternion(eulerangle angle){
    return {
        cos(angle.roll/2)*cos(angle.pitch/2)*cos(angle.jaw/2) + sin(angle.roll/2)*sin(angle.pitch/2)*sin(angle.jaw/2),
        sin(angle.roll/2)*cos(angle.pitch/2)*cos(angle.jaw/2) - cos(angle.roll/2)*sin(angle.pitch/2)*sin(angle.jaw/2),
        cos(angle.roll/2)*sin(angle.pitch/2)*cos(angle.jaw/2) + sin(angle.roll/2)*cos(angle.pitch/2)*sin(angle.jaw/2),
        cos(angle.roll/2)*cos(angle.pitch/2)*sin(angle.jaw/2) - sin(angle.roll/2)*sin(angle.pitch/2)*cos(angle.jaw/2)
    };
}

void rotate3DPoint(position& pos, quaternion q){
    RotationMatrix r{q};
    
    pos.x *=  r.x[0] + r.x[1] + r.x[2];
    pos.y *=  r.y[0] + r.y[1] + r.y[2];
    pos.z *=  r.z[0] + r.z[1] + r.z[2];
}

void rotate3DPoint(position& pos, eulerangle e){
    RotationMatrix r{toQuaternion(e)};
    
    pos.x *=  r.x[0] + r.x[1] + r.x[2];
    pos.y *=  r.y[0] + r.y[1] + r.y[2];
    pos.z *=  r.z[0] + r.z[1] + r.z[2];
}

void rotate3DPoint(position& pos, double angle, eulerangle axis){
    RotationMatrix r{{angle, axis}};
    
    pos.x *=  r.x[0] + r.x[1] + r.x[2];
    pos.y *=  r.y[0] + r.y[1] + r.y[2];
    pos.z *=  r.z[0] + r.z[1] + r.z[2];
}

struct cuboid{
    odometry odom;
    position corners[8]{};
    cuboid() = default;
    cuboid(odometry odom): odom(odom){}
};

//0: origin/front bottom left; 1 0-> right, 2 0-> back, 3 2-> right; 4 0-> top, 5 1-> top, 6 2-> top, 7 3->top;
struct rectangle: public cuboid{
    double length, height, depth;
    rectangle(double length, double height, double depth, odometry odom): length(length), height(height), depth(depth), cuboid(odom){
        corners[0] = odom.pos;
        corners[1] = position{length,      0,      0};
        corners[2] = position{     0,  depth,      0};
        corners[3] = position{length,  depth,      0};
        corners[4] = position{     0,      0, height};
        corners[5] = position{length,      0, height};
        corners[6] = position{     0,  depth, height};
        corners[7] = position{length,  depth, height};
        for(auto c : corners){rotate3DPoint(c, odom.angle);c=c+odom.pos;}
    }
};


struct cube: public rectangle{
    cube(double length, odometry odom): rectangle(length, length, length, odom){}
};


struct ellipse{
    odometry odom;
    double xLen, yLen;
    int resolution;
    position* points;
    ellipse(double xLen, double yLen, int resolution, odometry odom): xLen(xLen), yLen(yLen), resolution(resolution), odom(odom){
        points = new position[resolution]{};
        double angle = 360/resolution;
        for(int i=0; i<resolution; i++){
            points[i] = {(xLen/2)*cos(angle*i),(yLen/2)*sin(angle*i),0};
            rotate3DPoint(points[i],odom.angle);
            points[i] = points[i] + odom.pos;
        }
    }
};

// origin? center/ x+r z+r?
struct circle{
    odometry odom;
    double radius;
    int resolution;
    position* points;
    circle(double radius, int resolution, odometry odom): radius(radius), resolution(resolution), odom(odom){
        points = new position[resolution]{};
        double angle = 360/resolution;
        for(int i=0; i<resolution; i++){
            points[i] = {cos(angle*i),sin(angle*i),0};
            rotate3DPoint(points[i],odom.angle);
            points[i] = points[i] + odom.pos;
        }
    }
};

struct cylinder{
    odometry odom;
    double radius;
    double height;
    int resolution;
    position* points;
    cylinder(double radius, double height, int resolution, odometry odom): radius(radius), height(height), resolution(resolution), odom(odom){
        points = new position[2*resolution]{};
        double angle = 360/resolution;
        for(int i=0; i<resolution; i++){
            points[i] = position{cos(angle*i),sin(angle*i),0};
            points[i+resolution] = position{cos(angle*i),sin(angle*i),height};
            rotate3DPoint(points[i],odom.angle);
            rotate3DPoint(points[i+resolution],odom.angle);
            points[i] = points[i] + odom.pos;
            points[i+resolution] = points[i+resolution] + odom.pos;
        }
    }
};

struct elypsoid{
    odometry odom;
    double xLen, yLen, zLen;
    int resolution;
    position* points;
    elypsoid(double xLen, double yLen, double zLen, int resolution, odometry odom): xLen(xLen), yLen(yLen), zLen(zLen), resolution(resolution), odom(odom){
        points = new position[resolution*resolution]{};
        double phi, theta;
        for(int i=0; i<resolution; i++){
            phi = -PI/2 + PI*i/(resolution-1);
            for(int j=0; j<resolution; j++){
                theta = 2*PI*j/resolution;
                points[i*resolution+j]={
                    (xLen/2)*cos(phi)*cos(theta),
                    (yLen/2)*cos(phi)*sin(theta),
                    (zLen/2)*sin(phi)
                };
                rotate3DPoint(points[i*resolution+j],odom.angle);
                points[i*resolution+j]=odom.pos+points[i*resolution+j];
            }
        }
    }
};

struct Spheroid: public elypsoid{
    Spheroid(double rad, double zLen, int resolution, odometry odom): elypsoid(rad*2, rad*2, zLen, resolution, odom){}
};

struct Sphere: public elypsoid{
    Sphere(double rad, int resolution, odometry odom): elypsoid(rad*2, rad*2, rad*2, resolution, odom){}
};

