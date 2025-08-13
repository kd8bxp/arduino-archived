#include "SelectionMode.h"



void Selector::changeSelection()
{
	if (!selection_lock)
	{
		selection_mode = (SelectionMode)((selection_mode + 1) % 4);
		selection_lock = true;
	}
}

SelectionMode Selector::getSelection()
{
	return selection_mode;
}

Selector::Selector()
{
}

String Selector::getSelectionString()
{
	switch (selection_mode)
	{
	case Kp:
		return "Kp";
	case Ki:
		return "Ki";
	case Kd:
		return "Kd";
	case Setpoint:
		return "Setpoint";
	}
}

void Selector::unlockSelection()
{
	selection_lock = false;
}

