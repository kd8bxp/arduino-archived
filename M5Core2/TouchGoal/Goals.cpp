#include "src/M5Core2/M5Core2.h"
#include "Goals.h"

extern Button A;
extern Button B;

Goal::Goal() { name = ""; success = false; }
bool  Goal::passed() { return success; }
const char* Goal::getName(){ return name.c_str(); }
bool  Goal::test() {
  M5.Lcd.fillRect(0, TEXT_TOP, 320, TEXT_HEIGHT, NAVY);
  M5.Lcd.drawCentreString(name, TEXT_CENTER, TEXT_TOP, TEXT_FONT);
  start_time = millis();
  while(start_time + TEST_DURRATION > millis()) {
    M5.update();
    delay(1);
    if(success) {
      return true;
    }
  }
  return false;
}


//  Tap the A button
//
TapAGoal::TapAGoal() { name = "Tap the A Button"; }
// Set success to true if all the conditions of the goal are met
void TapAGoal::event_handler(Event& evt) {
  if((0 == strcmp("E_TAP", evt.typeName())) && (0 == strcmp("A", evt.button->name()))) success = true;
}


//  Tap the B button
//
TapBGoal::TapBGoal() { name = "Tap the B Button"; }
void TapBGoal::event_handler(Event& evt) {
  if((0 == strcmp("E_TAP", evt.typeName())) && (0 == strcmp("B", evt.button->name()))) success = true;
}


//  Double Tap the A button
//
DoubleTapAGoal::DoubleTapAGoal() { name = "Double Tap the A Button"; }
// Set success to true if all the conditions of the goal are met
void DoubleTapAGoal::event_handler(Event& evt) {
  if((0 == strcmp("E_DBLTAP", evt.typeName())) && (0 == strcmp("A", evt.button->name()))) success = true;
}


//  Double Tap the B button
//
DoubleTapBGoal::DoubleTapBGoal() { name = "Double Tap the B Button"; }
void DoubleTapBGoal::event_handler(Event& evt) {
  if((0 == strcmp("E_DBLTAP", evt.typeName())) && (0 == strcmp("B", evt.button->name()))) success = true;
}


//  Tap the Background
//
TapBackgroundGoal::TapBackgroundGoal() { name = "Tap the Background"; }
// Set success to true if all the conditions of the goal are met
void TapBackgroundGoal::event_handler(Event& evt) {
  if((0 == strcmp("E_TAP", evt.typeName())) && (0 == strcmp("background", evt.button->name()))) success = true;
}


//  Double Tap the Background
//
DoubleTapBackgroundGoal::DoubleTapBackgroundGoal() { name = "Double Tap the Background"; }
void DoubleTapBackgroundGoal::event_handler(Event& evt) {
  if((0 == strcmp("E_DBLTAP", evt.typeName())) && (0 == strcmp("background", evt.button->name()))) success = true;
}


// Drag from A to B
//
DragFromAtoBGoal::DragFromAtoBGoal() { name = "Drag From A to B"; }
// The series of events I see is: E_TOUCH(A), E_MOVE(A)..., E_PRESSING(a), E_MOVE(A)..., E_RELEASE(A), E_DRAGGED(A)
// Button never reflects another object; get position and test location.
void DragFromAtoBGoal::event_handler(Event& evt) {
  if(0 == strcmp("E_DRAGGED", evt.typeName())) {
    if(A.contains(evt.from) && B.contains(evt.to)) success = true;
  }
}


// Drag from B to A
//
DragFromBtoAGoal::DragFromBtoAGoal() { name = "Drag From B to A"; }
void DragFromBtoAGoal::event_handler(Event& evt) {
  if(0 == strcmp("E_DRAGGED", evt.typeName())) {
    if(B.contains(evt.from) && A.contains(evt.to)) success = true;
  }
}


// Drag from A to Background
//
DragFromAtoBackgroundGoal::DragFromAtoBackgroundGoal() { name = "Drag From A to Background"; }
void DragFromAtoBackgroundGoal::event_handler(Event& evt) {
  if(0 == strcmp("E_DRAGGED", evt.typeName())) {
    if(A.contains(evt.from) && M5.background.contains(evt.to) && !A.contains(evt.to) && !B.contains(evt.to)) success = true;
  }
}


// Drag from B to Background
//
DragFromBtoBackgroundGoal::DragFromBtoBackgroundGoal() { name = "Drag From B to Background"; }
void DragFromBtoBackgroundGoal::event_handler(Event& evt) {
  if(0 == strcmp("E_DRAGGED", evt.typeName())) {
    if(B.contains(evt.from) && M5.background.contains(evt.to) && !A.contains(evt.to) && !B.contains(evt.to)) success = true;
  }
}


// Drag from Background to A
//
DragFromBackgroundtoAGoal::DragFromBackgroundtoAGoal() {
  name = "Drag From Background to A";
  can_succeed = true;
}
// You don't get an E_DRAGGED event if you start in the background, so return an error if one comes in.
void DragFromBackgroundtoAGoal::event_handler(Event& evt) {
  if(0 == strcmp("E_DRAGGED", evt.typeName())) can_succeed = false;
  if(0 == strcmp("E_RELEASE", evt.typeName())) {
    if(M5.background.contains(evt.from) && !A.contains(evt.from) && !B.contains(evt.from) && A.contains(evt.to)) success = can_succeed;
  }
}


// Drag from Background to B
//
DragFromBackgroundtoBGoal::DragFromBackgroundtoBGoal() {
  name = "Drag From Background to B";
  can_succeed = true;
}
// You don't get an E_DRAGGED event if you start in the background, so return an error if one comes in.
void DragFromBackgroundtoBGoal::event_handler(Event& evt) {
  if(0 == strcmp("E_DRAGGED", evt.typeName())) can_succeed = false;
  if(0 == strcmp("E_RELEASE", evt.typeName())) {
    if(M5.background.contains(evt.from) && !A.contains(evt.from) && !B.contains(evt.from) && B.contains(evt.to)) success = can_succeed;
  }
}


// Swipe up detection
//
SwipeUpGoal::SwipeUpGoal() { name = "Swipe Up"; }
void SwipeUpGoal::event_handler(Event& evt) {
  if((0 == strcmp("E_GESTURE", evt.typeName())) && (0 == strcmp("swipe up", evt.gesture->name()))) success = true;
}

// Swipe down detection
//
SwipeDownGoal::SwipeDownGoal() { name = "Swipe Down"; }
void SwipeDownGoal::event_handler(Event& evt) {
  if((0 == strcmp("E_GESTURE", evt.typeName())) && (0 == strcmp("swipe down", evt.gesture->name()))) success = true;
}

// Swipe left detection
//
SwipeLeftGoal::SwipeLeftGoal() { name = "Swipe Left"; }
void SwipeLeftGoal::event_handler(Event& evt) {
  if((0 == strcmp("E_GESTURE", evt.typeName())) && (0 == strcmp("swipe left", evt.gesture->name()))) success = true;
}

// Swipe right detection
//
SwipeRightGoal::SwipeRightGoal() { name = "Swipe Right"; }
void SwipeRightGoal::event_handler(Event& evt) {
  if((0 == strcmp("E_GESTURE", evt.typeName())) && (0 == strcmp("swipe right", evt.gesture->name()))) success = true;
}
