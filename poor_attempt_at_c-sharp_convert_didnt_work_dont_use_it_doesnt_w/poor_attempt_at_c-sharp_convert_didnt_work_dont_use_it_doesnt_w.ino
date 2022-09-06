#include <math.h> 
#include <vector>

double PhoneA_x = 0;
double PhoneA_y = 0;
double PhoneB_x = 6;
double PhoneB_y = 3;
double PhoneC_x = 3;
double PhoneC_y = 6;


double getPosition(double phoneADistance, double phoneBDistance, double phoneCDistance)
    {
        //double answer = new double[] { 0, 0 };
       // double PhoneADist = meterToFeet(phoneADistance);
       // double PhoneBDist = meterToFeet(phoneBDistance);
       // double PhoneCDist = meterToFeet(phoneCDistance);
       // Vector<double> P1 = new DenseVector(new[] { PhoneA_x, PhoneA_y });
       // Vector<double> P2 = new DenseVector(new[] { PhoneB_x, PhoneB_y });
       // Vector<double> P3 = new DenseVector(new[] { PhoneC_x, PhoneC_y });
        //Translate values for the three points
        int B3 = PhoneA_x;
        int C3 = PhoneA_y;
        int D3 = phoneADistance;
        int B4 = PhoneB_x;
        int C4 = PhoneB_y;
        int D4 = phoneBDistance;
        int B5 = PhoneC_x;
        int C5 = PhoneC_y;
        int D5 = phoneCDistance;
        //Translate P1 to Origin
        int B8 = B3 - B3;
        int C8 = C3 - C3;
        int D8 = D3;
        int B9 = B4 - B3;
        int C9 = C4 - C3;
        int D9 = D4;
        int B10a = B5 - B3;
        int C10 = C5 - C3;
        int D10 = D5;
        //Find Calculation Values
        int B13 = atan2(C9, B9); ;
        int B14 = atan2(C10, B10a);
        int B15 = sqrt(pow(B4 - B3, 2) + pow(C4 - C3, 2));
        int B16 = sqrt(pow(B5 - B3, 2) + pow(C5 - C3, 2));
        //Polar Coordinates for the Rotated System
        //int B20 = 0;
        //int C20 = 0;
        int D20 = D3;
        int B21 = B15;
        //int C21 = 0;
        int D21 = D4;
        int B22 = B16;
        int C22 = B14 - B13;
        int D22 = D5;
        //Rectangular Coordinates for the Rotated System
        //int B26 = 0;
        //int C26 = 0;
        int D26 = D3;
        int B27 = B21;
        //int C27 = 0;
        int D27 = D4;
        int B28 = B22 * cos(C22);
        int C28 = B22 * sin(C22);
        int D28 = D5;
        //Coordinates of Roated Solution
        int B31 = (pow(D3, 2) - pow(D4, 2) + pow(B27, 2)) / (B27 * 2);
        int B32 = sqrt(pow(D3, 2) - pow(B31, 2));
        int D32 = -B32;
        //Convert to Polar
        int B35 = sqrt(pow(B31, 2) + pow(B32, 2));
        int B36 = atan2(B32, B31);
        int D36 = atan2(D32, B31);
        //Unrotate
        int B39 = B35;
        int B40 = B36 + B13;
        int D40 = D36 + B13;
        //Rectangular Coordinates
        int B43 = B39 * cos(B40);
        int D43 = B39 * cos(D40);
        int B44 = B39 * sin(B40);
        int D44 = B39 * sin(D40);
        //Untranslate
        int B47 = B43 + B3;
        int D47 = D43 + B3;
        int B48 = B44 + C3;
        int D48 = D44 + C3;
        int x = B47;
        int y = B48;
        //Return Answer
        //if (!Double.IsNaN(x) || !Double.IsNaN(y))
        //{
            //answer = { x, y };
            Serial.println(x + " " + y);
        //}
//        return; //answer;
    }

    
void setup() {
   Serial.begin(115200);
  delay(2000);
  Serial.println(getPosition(.4,.4,.4));
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
