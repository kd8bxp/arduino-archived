#ifndef PIDTUNER_
#define PIDTUNER_

#include <Arduino.h>
#include "src/M5Stack/M5Stack.h"
#include "src/PID/PID_v1.h"
#include "datatypes.h"

class PIDTuner
{
public:
	//PIDTuner::PIDTuner(PID& pid, PID_DATA& pid_data);
	PIDTuner(PID& pid, PID_DATA& pid_data);
	//void PIDTuner::handle_buttons();
  void handle_buttons();

private:
	PID& pid_;
	PID_DATA& pid_data_;
};

#endif // !PIDTUNER_
