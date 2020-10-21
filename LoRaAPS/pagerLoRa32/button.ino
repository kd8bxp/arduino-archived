void setupButtons() {
   button.setClickHandler(click);
  button.setLongClickHandler(longClick);
  button.setDoubleClickHandler(doubleClick);
  button.setTripleClickHandler(tripleClick);
}


void click(Button2& btn) {
  displayChange++;
  switch(displayChange) {
    case 1:
    displayCall();
    break;
    case 2:
    displayMsg(from, holdMsg);
    break;
    case 3:
    displayPath();
    break;
    case 4:
    oled.clear();
    break;
    default:
    displayCall();
    break;
  }
  if (displayChange>4) {displayChange=1;}
}

void longClick(Button2& btn) {
    //Nothing here yet
}

void doubleClick(Button2& btn) {
    //Nothing here yet
}

void tripleClick(Button2& btn) {
    //Nothing here yet
}
