#include <math.h> 
#include <vector>

// found at https://copyprogramming.com/howto/trilateration-formula-programming
// Part 9 Trilateration (2D) algorithm implementation


struct point 
{
    float x,y;
};

float norm (point p) // get the norm of a vector
{
    return pow(pow(p.x,2)+pow(p.y,2),.5);
}

point trilateration(point point1, point point2, point point3, double r1, double r2, double r3) {
    point resultPose;
    //unit vector in a direction from point1 to point 2
    double p2p1Distance = pow(pow(point2.x-point1.x,2) + pow(point2.y-   point1.y,2),0.5);
    point ex = {(point2.x-point1.x)/p2p1Distance, (point2.y-point1.y)/p2p1Distance};
    point aux = {point3.x-point1.x,point3.y-point1.y};
    //signed magnitude of the x component
    double i = ex.x * aux.x + ex.y * aux.y;
    //the unit vector in the y direction. 
    point aux2 = { point3.x-point1.x-i*ex.x, point3.y-point1.y-i*ex.y};
    point ey = { aux2.x / norm (aux2), aux2.y / norm (aux2) };
    //the signed magnitude of the y component
    double j = ey.x * aux.x + ey.y * aux.y;
    //coordinates
    double x = (pow(r1,2) - pow(r2,2) + pow(p2p1Distance,2))/ (2 * p2p1Distance);
    double y = (pow(r1,2) - pow(r3,2) + pow(i,2) + pow(j,2))/(2*j) - i*x/j;
    //result coordinates
    double finalX = point1.x+ x*ex.x + y*ey.x;
    double finalY = point1.y+ x*ex.y + y*ey.y;
    resultPose.x = finalX;
    resultPose.y = finalY;
    return resultPose;
}

    point finalPose;
    point p1 = {0.0, -1.0}; //{4.0,4.0};
    point p2 = {7.0, 3.0}; //{9.0,7.0};
    point p3 = {3.0, 7.0}; //{9.0,1.0};
    double r1,r2,r3;
     

void setup() {
//r is the distance, appears that it needs to multiplyed by 10 using my map

    r1 = .4; //4;
    r2 = .4; //3;
    r3 = .4; //3.25;
  
  finalPose = trilateration(p1,p2,p3,r1,r2,r3); //returns x,y position
  /*
   * Appears these need to be rounded and maybe use abs. 
   * corridant 3,3 comes out 2.5, 2.5 thinking if it's .5 or above need to
   * round up, .4 and below need to round down.
   * corridant 0,0 returned a -0.22 and -0.22 haven't tested the rest of locations on zero
   * haven't tested the rest of the 3s
   * Otherwise this appears to work quite well.
   * 
   */
  Serial.begin(115200);
  delay(2000);
  //using round() here seems to correct this problems listed above
  Serial.print("X: "); Serial.println((finalPose.x));
  Serial.print("Y: "); Serial.println((finalPose.y));
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

/*
 * Other Trilateration sites and information
 * https://www.gps.gov/multimedia/tutorials/trilateration/#:~:text=Distance%20%3D%20Rate%20%C3%97%20Time,your%20location%20on%20the%20planet.
 * https://handwiki.org/wiki/Trilateration
 * https://handwiki.org/wiki/Euclidean_distance
 * https://math.stackexchange.com/questions/884807/find-x-location-using-3-known-x-y-location-using-trilateration
 * https://www.101computing.net/cell-phone-trilateration-algorithm/
 * https://inst.eecs.berkeley.edu/~ee127/sp21/livebook/l_lineqs_apps_trilateration.html
 * 
 * Some of the forumals are really confusing, but I think I understand what
 * is going on with the copyprogramming.com site and information
 */
