// #####################################################################################################################
// ######################################### MAGNETOETER COMPASS #######################################################
// ##################################################################################################################### 

#include <Wire.h>                         // Arduino I2C-BUS library

#define HMC5883L_Address             0x1E // 7bit address of HMC5883L
#define HMC5883L_Mode_Register       0x02 // The mode register
#define HMC5883L_Continuous_Mode     0x00 // Select first mode (0: continous, 1: single, 2: idle)
#define HMC5883L_Data_Output_Address 0x03 // Address 3 = X MSB register (continues till Y LSB)

unsigned long Timer_Serial  = 0;
unsigned long Timer_Display = 0;
// ##################################################################################################################### 
// ######################################### AXIS DATA #################################################################
// ##################################################################################################################### 
struct HMC5883LAxisData
{
    int X_Axis, Y_Axis, Z_Axis;
} compass;
// ##################################################################################################################### 
// ######################################### SETUP #####################################################################
// ##################################################################################################################### 
void setup()
{
    // Startup serial and i2c
    Serial.begin(9600);
    Wire.begin();
    // Send the correct register & mode to the compass
    writeHMC5883L(HMC5883L_Mode_Register, HMC5883L_Continuous_Mode);
    // Setup the lcd
    
}
// ##################################################################################################################### 
// ######################################### LOOP ######################################################################
// ##################################################################################################################### 
void loop()
{
    // Get the calculated angle
    float Angle = getAngle();
    // Send data depending on device and timer
    // Serial a bit slower than display
    if (millis() > Timer_Serial)
    {
        // Print it to the serial monitor
        Serial.print("X: ");
        Serial.print(compass.X_Axis);
        Serial.print(" | Y: ");
        Serial.print(compass.Y_Axis);
        Serial.print(" | Z: ");
        Serial.print(compass.Z_Axis);
        Serial.print(" | Angle: ");
        Serial.println(Angle);
        // Increase timer
        Timer_Serial = millis() + 500;
    }
    if (millis() > Timer_Display)
    {
        
        Timer_Display = millis() + 100;
    }
}
// ##################################################################################################################### 
// ######################################### GET ANGLE #################################################################
// ##################################################################################################################### 
float getAngle()
{
    // First of all get the raw data from the HMC5883L module
    readHMC5883L();
    /* 
    Calculate the angle:
        - angle (radiant) = atan(-y, x)
        - angle (degrees) = angle (radiant) * (180 / PI)
    */
    float Angle = atan2(-compass.Y_Axis, compass.X_Axis) * (180 / M_PI);
    /*
    Calculate the declination. The formula for this is:
        (DEGREE + (MINUTES / 60)) / (180 / PI);
    You can get your declination on http://magnetic-declination.com/
    Be careful, that you also set it positive or negative.
    The declination for Vienna in Austria is +3°57', and so I set my variables
    */
    byte  Declination_Degree = -7;
    byte  Declination_Minute = 53;
    float Declination = (Declination_Degree + (Declination_Minute / 60)) / (180 / M_PI);
    // Add the declination to the angle
    Angle += Declination;
    // Finally correct the angle, so it is shown from 0° to 360° (and not from -180° to +180°)
    if (Angle < 0)
        return Angle + 360;
    else
        return Angle;
}
// ##################################################################################################################### 
// ######################################### WRITE HMC5883L ############################################################
// ##################################################################################################################### 
void writeHMC5883L(int Parameter_Mode, int Parameter_Data)
{
    // Mode register and data have to be sent together !
    Wire.beginTransmission(HMC5883L_Address);
    Wire.write(Parameter_Mode);
    Wire.write(Parameter_Data);
    Wire.endTransmission();
}
void writeHMC5883L(int Parameter_Data)
{
    Wire.beginTransmission(HMC5883L_Address);
    Wire.write(Parameter_Data);
    Wire.endTransmission();
}
// ##################################################################################################################### 
// ######################################### READ HMC5883L #############################################################
// ##################################################################################################################### 
void readHMC5883L()
{
    // Begin to read the data to the correct address (X MSB register)
    writeHMC5883L(HMC5883L_Data_Output_Address);
    // Send request to read the axis data (there are 6 register to read)
    Wire.requestFrom(HMC5883L_Address, 6);
    /* 
    Finally start reading the data (each axis has two register)
    The register are set the following way: x_msb, x_lsb, z_msb, z_lsb, y_msb, y_lsb
    */
    if (6 <= Wire.available())
    {
        compass.X_Axis  = Wire.read() << 8 | Wire.read();
        compass.Z_Axis  = Wire.read() << 8 | Wire.read();
        compass.Y_Axis  = Wire.read() << 8 | Wire.read();
    }
}
// ##################################################################################################################### 
// ######################################### END OF CODE ###############################################################
// ##################################################################################################################### 
