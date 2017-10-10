#include "Word100.h";

Word100 Word100(10);

int sentence[] = {one, two, three, four, five, six, seven, eight, nine, ten};
int sentence2[] = {alert, alert, intruder, _in, zone, two};

void setup() {
  // put your setup code here, to run once:
Word100.begin();

}

#define arr_len( x ) ( sizeof ( x ) / sizeof (*x) )

void loop() {

  //say the first sentence (Count to Ten)
  Serial.print(arr_len(sentence));
  // put your main code here, to run repeatedly:
for (int i = 0; i < arr_len(sentence); i++) {
  Word100.say(sentence[i]);
}

delay(1000);

//say the second sentence (Alert Alert intruder in zone two)
 Serial.println(arr_len(sentence2));
  // put your main code here, to run repeatedly:
for (int i = 0; i < arr_len(sentence2); i++) {
  Word100.say(sentence2[i]);
}

delay(1000);

//say all the words. using the words as counters
for (int i = colour; i < zone; i++) {
  Word100.say(i);
}

delay(1000);

//we can just call say(xxx)

Word100.say(red);
Word100.say(alert);
delay(1000);

while(1);
}
