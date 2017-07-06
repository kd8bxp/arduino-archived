
int start = 3; //this is the starting location of robot
int target = 17; //this is the target location for robot
int row;
int col;
int rows[20] = {
  1,
  2,
  3,
  4,
  5,
  6,
  7,
  8,
  9,
  10,
  11,
  12,
  13,
  14,
  15,
  16,
  17,
  18,
  19,
  20
};

int cols[20] = {
  20,
  19,
  18,
  17,
  16,
  15,
  14,
  13,
  12,
  11,
  10,
  9,
  8,
  7,
  6,
  5,
  4,
  3,
  2,
  1
};

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.println("Attempt to find the location of a given number in array");
Serial.println("");
Serial.println("Target: ");
gofindnumber(target);
int targetrow = row;
int targetcol = col;
Serial.println("Start: ");
gofindnumber(start);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void gofindnumber(int findnumber) {
  for (int i=0; i<20; i++) {
    if (rows[i] == findnumber) {row = i;}
    if (cols[i] == findnumber) {col = i;}
  }
  Serial.print(findnumber);
  Serial.println(" is located at: ");
  Serial.print("Row: ");
  Serial.print(row);
  Serial.print(" Col: ");
  Serial.println(col);
}

