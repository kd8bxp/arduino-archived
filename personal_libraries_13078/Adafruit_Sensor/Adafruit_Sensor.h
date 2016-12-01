/*
* Copyright (C) 2008 The Android Open Source Project
*
* Licensed under the Apache License, Version 2.0 (the &quot;License&quot;);
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software&lt; /span&gt;
* distributed under the License is distributed on an &quot;AS IS&quot; BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/* Update by K. Townsend (Adafruit Industries) for lighter typedefs, and
 * extended sensor support to include color, voltage and current */
 
#ifndef _ADAFRUIT_SENSOR_H
#define _ADAFRUIT_SENSOR_H

#if ARDUINO &gt;= 100
 #include &quot;Arduino.h&quot;
 #include &quot;Print.h&quot;
#else
 #include &quot;WProgram.h&quot;
#endif

/* Intentionally modeled after sensors.h in the Android API:
 * https://github.com/android/platform_hardware_libhardware/blob/master/include/hardware/sensors.h */

/* Constants */
#define SENSORS_GRAVITY_EARTH             (9.80665F)              /**&lt; Earth's gravity in m/s^2 */
#define SENSORS_GRAVITY_MOON              (1.6F)                  /**&lt; The moon's gravity in m/s^2 */
#define SENSORS_GRAVITY_SUN               (275.0F)                /**&lt; The sun's gravity in m/s^2 */
#define SENSORS_GRAVITY_STANDARD          (SENSORS_GRAVITY_EARTH)
#define SENSORS_MAGFIELD_EARTH_MAX        (60.0F)                 /**&lt; Maximum magnetic field on Earth's surface */
#define SENSORS_MAGFIELD_EARTH_MIN        (30.0F)                 /**&lt; Minimum magnetic field on Earth's surface */
#define SENSORS_PRESSURE_SEALEVELHPA      (1013.25F)              /**&lt; Average sea level pressure is 1013.25 hPa */
#define SENSORS_DPS_TO_RADS               (0.017453293F)          /**&lt; Degrees/s to rad/s multiplier */
#define SENSORS_GAUSS_TO_MICROTESLA       (100)                   /**&lt; Gauss to micro-Tesla multiplier */

/** Sensor types */
typedef enum
{
  SENSOR_TYPE_ACCELEROMETER         = (1),   /**&lt; Gravity + linear acceleration */
  SENSOR_TYPE_MAGNETIC_FIELD        = (2),
  SENSOR_TYPE_ORIENTATION           = (3),
  SENSOR_TYPE_GYROSCOPE             = (4),
  SENSOR_TYPE_LIGHT                 = (5),
  SENSOR_TYPE_PRESSURE              = (6),
  SENSOR_TYPE_PROXIMITY             = (8),
  SENSOR_TYPE_GRAVITY               = (9),
  SENSOR_TYPE_LINEAR_ACCELERATION   = (10),  /**&lt; Acceleration not including gravity */
  SENSOR_TYPE_ROTATION_VECTOR       = (11),
  SENSOR_TYPE_RELATIVE_HUMIDITY     = (12),
  SENSOR_TYPE_AMBIENT_TEMPERATURE   = (13),
  SENSOR_TYPE_VOLTAGE               = (15),
  SENSOR_TYPE_CURRENT               = (16),
  SENSOR_TYPE_COLOR                 = (17)
} sensors_type_t;

/** struct sensors_vec_s is used to return a vector in a common format. */
typedef struct {
    union {
        float v[3];
        struct {
            float x;
            float y;
            float z;
        };
        /* Orientation sensors */
        struct {
            float roll;    /**&lt; Rotation around the longitudinal axis (the plane body, 'X axis'). Roll is positive and increasing when moving downward. -90°&lt;=roll&lt;=90° */
            float pitch;   /**&lt; Rotation around the lateral axis (the wing span, 'Y axis'). Pitch is positive and increasing when moving upwards. -180°&lt;=pitch&lt;=180°) */
            float heading; /**&lt; Angle between the longitudinal axis (the plane body) and magnetic north, measured clockwise when viewing from the top of the device. 0-359° */
        };
    };
    int8_t status;
    uint8_t reserved[3];
} sensors_vec_t;

/** struct sensors_color_s is used to return color data in a common format. */
typedef struct {
    union {
        float c[3];
        /* RGB color space */
        struct {
            float r;       /**&lt; Red component */
            float g;       /**&lt; Green component */
            float b;       /**&lt; Blue component */
        };
    };
    uint32_t rgba;         /**&lt; 24-bit RGBA value */
} sensors_color_t;

/* Sensor event (36 bytes) */
/** struct sensor_event_s is used to provide a single sensor event in a common format. */
typedef struct
{
    int32_t version;                          /**&lt; must be sizeof(struct sensors_event_t) */
    int32_t sensor_id;                        /**&lt; unique sensor identifier */
    int32_t type;                             /**&lt; sensor type */
    int32_t reserved0;                        /**&lt; reserved */
    int32_t timestamp;                        /**&lt; time is in milliseconds */
    union
    {
        float           data[4];
        sensors_vec_t   acceleration;         /**&lt; acceleration values are in meter per second per second (m/s^2) */
        sensors_vec_t   magnetic;             /**&lt; magnetic vector values are in micro-Tesla (uT) */
        sensors_vec_t   orientation;          /**&lt; orientation values are in degrees */
        sensors_vec_t   gyro;                 /**&lt; gyroscope values are in rad/s */
        float           temperature;          /**&lt; temperature is in degrees centigrade (Celsius) */
        float           distance;             /**&lt; distance in centimeters */
        float           light;                /**&lt; light in SI lux units */
        float           pressure;             /**&lt; pressure in hectopascal (hPa) */
        float           relative_humidity;    /**&lt; relative humidity in percent */
        float           current;              /**&lt; current in milliamps (mA) */
        float           voltage;              /**&lt; voltage in volts (V) */
        sensors_color_t color;                /**&lt; color in RGB component values */
    };
} sensors_event_t;

/* Sensor details (40 bytes) */
/** struct sensor_s is used to describe basic information about a specific sensor. */
typedef struct
{
    char     name[12];                        /**&lt; sensor name */
    int32_t  version;                         /**&lt; version of the hardware + driver */
    int32_t  sensor_id;                       /**&lt; unique sensor identifier */
    int32_t  type;                            /**&lt; this sensor's type (ex. SENSOR_TYPE_LIGHT) */
    float    max_value;                       /**&lt; maximum value of this sensor's value in SI units */
    float    min_value;                       /**&lt; minimum value of this sensor's value in SI units */
    float    resolution;                      /**&lt; smallest difference between two values reported by this sensor */
    int32_t  min_delay;                       /**&lt; min delay in microseconds between events. zero = not a constant rate */
} sensor_t;

class Adafruit_Sensor {
 public:
  // Constructor(s)
  void constructor();

  // These must be defined by the subclass
  virtual void enableAutoRange(bool enabled) {};
  virtual void getEvent(sensors_event_t*);
  virtual void getSensor(sensor_t*);
  
 private:
  bool _autoRange;
};

#endif
