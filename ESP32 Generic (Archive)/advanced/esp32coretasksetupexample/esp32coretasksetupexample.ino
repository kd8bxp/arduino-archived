static int taskCore = 0;
 
void coreTask( void * pvParameters ){
 
    String taskMessage = "Task running on core ";
    taskMessage = taskMessage + xPortGetCoreID();
 
    while(true){
        Serial.println(taskMessage);
        delay(1000);
    }
 
}
 
void setup() {
 
  Serial.begin(9600);
  delay(1000);
 
  Serial.print("Starting to create task on core ");
  Serial.println(taskCore);
 
  xTaskCreatePinnedToCore(
                    coreTask,   /* Function to implement the task */
                    "coreTask", /* Name of the task */
                    10000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    0,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    taskCore);  /* Core where the task should run */
 
  Serial.println("Task created...");
}
 
void loop() {
 
    Serial.print("Starting main loop...");
    String taskMessage = "Task running on core ";
    taskMessage = taskMessage + xPortGetCoreID();
   while (true) {
    Serial.println(taskMessage);
    delay(1000);
   }
 
}
