/*
 * Sample of using unidimensional Kalman filter
 *
 * Copyright (c) 2018 Damian Wrobel <dwrobel@ertelnet.rybnik.pl>
 *
 * THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY EXPRESSED
 * OR IMPLIED. ANY USE IS AT YOUR OWN RISK.
 *
 * Permission is hereby granted to use or copy this program
 * for any purpose, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */
#include <NewPing.h>
#include <TrivialKalmanFilter.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define DT_COVARIANCE_RK .01 // Estimation of the noise covariances (process)
#define DT_COVARIANCE_QK .04   // Estimation of the noise covariances (observation)

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


#include <TrivialKalmanFilter.h>


TrivialKalmanFilter<float> filter(DT_COVARIANCE_RK, DT_COVARIANCE_QK);

void setup(void) {
  Serial.begin(9600);

}

void loop(void) {
  int ping = sonar.ping_cm();
    Serial.print("Reading: ");
      Serial.print(ping);
      Serial.print(" Predicted: ");
      Serial.println((filter.update(ping)));
       delay(500);
}
