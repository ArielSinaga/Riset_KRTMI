#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define Mark -999999

SemaphoreHandle_t xBinarySemaphore; 
TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;

bool isNumeric(String str) {
  if (str.length() == 0) return false;
  for (unsigned int i = 0; i < str.length(); i++) {
    if (!isDigit(str.charAt(i)) && !(i == 0 && (str.charAt(i) == '-' || str.charAt(i) == '+'))) {
      return false;
    }
  }
  return true;
}

void Task1_TX(void *pvParameters) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    Serial.println("Task1: Memberikan Semaphore.");
    xSemaphoreGive(xBinarySemaphore); 
    vTaskDelay(pdMS_TO_TICKS(1000)); 
  }
}

void Task2_TX(void *pvParameters) {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE) {
      Serial.println("Task2: Semaphore Berhasil Diambil! (Mulai tugas kontrol)");
    }
  }
}

void setup() {
  Serial.begin(115200);  
  delay(1000);
  Serial.println("Setup running on core " + String(xPortGetCoreID()));

  xBinarySemaphore = xSemaphoreCreateBinary();

  if (xBinarySemaphore != NULL) {
    xTaskCreatePinnedToCore(
        Task1_TX, "Task1", 4096, NULL, 5, &Task1_Handle, 0);
    xTaskCreatePinnedToCore(
        Task2_TX, "Task2", 4096, NULL, 5, &Task2_Handle, 1); 
  } else {
    Serial.println("Gagal membuat semaphore.");
  }
}

void loop() {
  if (Serial.available() > 0) {  
    String data = Serial.readStringUntil('\n'); 
    Serial.print("Diterima: ");
    Serial.println(data);
    data.trim();
    
    if (isNumeric(data)) {
      int value = data.toInt();
      if(value >= 0 && value <= 180){
        Serial.print("servo_deg : ");
        Serial.println(value);
      }
      else Serial.println("Angka invalid (di luar 0-180)");
    } 
    else{
      Serial.println("Bukan Angka");
    }
  }
  vTaskDelay(pdMS_TO_TICKS(100)); 
}