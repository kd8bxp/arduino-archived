int globalIntVar = 9;
int localIntVar = -9;
 
void setup() {
 
  Serial.begin(115200);
  delay(1000);
 
  xTaskCreate(
                    globalIntTask,             /* Task function. */
                    "globalIntTask",           /* String with name of task. */
                    10000,                     /* Stack size in words. */
                    (void*)&globalIntVar,      /* Parameter passed as input of the task */
                    1,                         /* Priority of the task. */
                    NULL);                     /* Task handle. */
 
  
 
  xTaskCreate(
                    localIntTask,              /* Task function. */
                    "localIntTask",            /* String with name of task. */
                    10000,                     /* Stack size in words. */
                    (void*)&localIntVar,       /* Parameter passed as input of the task */
                    1,                         /* Priority of the task. */
                    NULL);                     /* Task handle. */
 
}
 
void loop() {
  delay(1000);
  
}
 
void globalIntTask( void * parameter ){
 while(1) {
    Serial.print("globalIntTask: ");
    Serial.println(*((int*)parameter));            
    localIntVar --;
    //vTaskDelete( NULL );
 vTaskDelay(1000);
 }
}
 
void localIntTask( void * parameter ){
 while(1) {
    Serial.print("localIntTask: ");
    Serial.println(*((int*)parameter));            
 globalIntVar ++;
    //vTaskDelete( NULL );
 vTaskDelay(1000);
 }
}
