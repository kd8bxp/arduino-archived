#include "PIDTuner.h"


PIDTuner::PIDTuner(PID& pid, PID_DATA& pid_data) :
	pid_(pid),
	pid_data_(pid_data)
{
	
}

void PIDTuner::handle_buttons()
{
	
	if (M5.BtnA.pressedFor(3000))
	{
		//selector.changeSelection();
   changeSelection();
		last_locked_time = millis();
	}

	if (M5.BtnA.isReleased())
	{
		if (millis() - last_locked_time > 1500)
		{
			//selector.unlockSelection();
     unlockSelection();
		}
	}

	if (M5.BtnB.wasPressed())
	{
		//switch (selector.getSelection())
    switch (getSelection())
		{
		case Kp:
			kp_ += 0.5;
			pid.SetTunings(kp_, ki_, kd_);
			break;
		case Ki:
			ki_ += 0.05;
			pid.SetTunings(kp_, ki_, kd_);
			break;
		case Kd:
			kd_ += 0.05;
			pid.SetTunings(kp_, ki_, kd_);
			break;
		case Setpoint:
			setpoint += 0.1;
			break;
		default:
			break;
		}
	}

	if (M5.BtnC.wasPressed())
	{
		//switch (selector.getSelection())
    switch (getSelection())
		{
		case Kp:
			kp_ -= 0.5;
			pid.SetTunings(kp_, ki_, kd_);
			break;
		case Ki:
			ki_ -= 0.05;
			pid.SetTunings(kp_, ki_, kd_);
			break;
		case Kd:
			kd_ -= 0.05;
			pid.SetTunings(kp_, ki_, kd_);
			break;
		case Setpoint:
			setpoint -= 0.1;
			break;
		default:
			break;
		}
	}

}
