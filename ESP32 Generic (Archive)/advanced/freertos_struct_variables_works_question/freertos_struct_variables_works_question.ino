struct Data_t {
  int globalIntVar;
  int localIntVar; } GenericData_t;

 //typedef struct Data_t GenericData_t;
 
void setup() {
 
  Serial.begin(115200);
  delay(1000);
 
  xTaskCreate(
                    globalIntTask,             /* Task function. */
                    "globalIntTask",           /* String with name of task. */
                    10000,                     /* Stack size in words. */
                    (void*)&GenericData_t,      /* Parameter passed as input of the task */
                    1,                         /* Priority of the task. */
                    NULL);                     /* Task handle. */
 
  
 
  xTaskCreate(
                    localIntTask,              /* Task function. */
                    "localIntTask",            /* String with name of task. */
                    10000,                     /* Stack size in words. */
                    (void*)&GenericData_t,       /* Parameter passed as input of the task */
                    1,                         /* Priority of the task. */
                    NULL);                     /* Task handle. */
 
}
 
void loop() {
  delay(10000);
  GenericData_t.globalIntVar = 5000;
  GenericData_t.localIntVar = 0;
}
 
void globalIntTask( void * xStruct ){
 while(1) {
  //GenericData_t * data = (GenericData_t *) xStruct;
    Serial.print("globalIntTask: ");
    Serial.println(GenericData_t.globalIntVar);            
    GenericData_t.localIntVar --;
    //vTaskDelete( NULL );
 vTaskDelay(1000);
 }
}
 
void localIntTask( void * xStruct ){
 while(1) {
  //GenericData_t * data = (GenericData_t *) xStruct;
    Serial.print("localIntTask: ");
    Serial.println(GenericData_t.localIntVar);            
 GenericData_t.globalIntVar ++;
    //vTaskDelete( NULL );
 vTaskDelay(1000);
 }
}
