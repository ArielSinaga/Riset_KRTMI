#include <Arduino.h>
#include <cstdint>
#include <cmath>

const int button = 10;
const int potensio = 9;
const int joystick_c = 8;
const int joystick_d = 7; 
const int led = 32;

const float pi = 3.14159;

struct DataKustom {
    int8_t joystickX_ADCvalue = 10;
};

// Deklarasi variabel dari struktur tersebut
struct DataKustom mydata;

void setup() {
  // put your setup code here, to run once:
  pinMode(button, INPUT_PULLUP);
  pinMode(potensio, INPUT);
  pinMode(joystick_c, INPUT);
  pinMode(joystick_d, INPUT);

  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.begin(115200);

  int potensio_value = analogRead(potensio);
  int button_value = digitalRead(button);
  int joystickvalueX = analogRead(joystick_c);
  int joystickvalueY = analogRead(joystick_d);

  int joystickvalueX_min;
  int joystickvalueX_max;

  if (potensio_value <= 200) {
    potensio_value = 201;
  }

  if (joystickvalueX >= -50 && joystickvalueX <= 50 || joystickvalueX >= -510 && joystickvalueX <= -500 || joystickvalueX <= 510 && joystickvalueX >= 500) {
    joystickvalueX_min = -51;
    joystickvalueX_max = 51;
  };

  int actualjoystickvalueY = map(joystickvalueY, 0, 1023, -100, 100);

  uint8_t potensio_ADCvalue = map(potensio_value, 201, 4095, 0, 255);
  uint8_t joystickY_ADCvalue = map(actualjoystickvalueY, -100, 100, 0, 255);

  int min_valuex = map(joystickvalueX, -500, joystickvalueX_min, 0, 127);

  int max_valuex = map(joystickvalueX, joystickvalueX_max, 510, 128, 255);

  mydata.joystickX_ADCvalue = constrain(joystickvalueX, min_valuex, max_valuex);

  Serial.write(potensio_ADCvalue);

  if (button_value == 1){
    analogWrite(led, potensio_ADCvalue);
  }
  else if (button_value == 0){
    digitalWrite(led, LOW);
  }

  int valuejoystick = (1.3 * pow(joystickvalueY, 2)  + 2.1 * joystickvalueX - 1.8 * (joystickvalueY * sqrt(joystickvalueX))) / exp(joystickvalueX * joystickvalueY / 75 * pi);

  Serial.print(valuejoystick);
}
