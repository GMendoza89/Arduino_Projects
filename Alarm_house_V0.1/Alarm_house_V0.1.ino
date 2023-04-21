// importa libreria Keypad
#include <Keypad.h>
// define numero de filas
const uint8_t ROWS = 4;
// define numero de columnas
const uint8_t COLS = 4;
// define la distribucion de teclas
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
// pines correspondientes a las filas
uint8_t colPins[COLS] = { 16, 4, 2, 15 };
// pines correspondientes a las columnas
uint8_t rowPins[ROWS] = { 19, 18, 5, 17 };
// crea objeto con los prametros creados previamente
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


//Password's variable
char passArmed[7] = "00000";
char passDisarm[7] = "99999";
char ingressPass[7];
byte idx = 0;

//Output pins
unsigned int pinBuzzer = 14;
unsigned int pinLedGreen = 13;
unsigned int pinLedRed = 12;
// Sensor's Pins
unsigned int pinSensorPIR = 21;
unsigned int pinSensorMag = 32;

//Status Variable
bool alarmOn;
bool alarmArmed;
bool toogle;

//sensor's variables
unsigned int magneticSensorValue;
bool pirSensorValue;
//Time variables
unsigned long int presentTime;

//Key pad input variable
char key;
void setup() {
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinLedGreen, OUTPUT);
  pinMode(pinLedRed, OUTPUT);
  Serial.begin(115200);
  digitalWrite(pinBuzzer, LOW);
  pinMode(pinSensorPIR, INPUT);
  pinMode(pinSensorMag, INPUT);
  alarmOn = false;
  alarmArmed = false;
  toogle = false;
  presentTime = millis();
}

void loop() {
    magneticSensorValue = analogRead(pinSensorMag);
    pirSensorValue = digitalRead(pinSensorPIR);
    key = keypad.getKey();
    
    if(key){
        ingressPass[idx] = key;
        idx++;
        if(idx==5){
            if(strcmp(passArmed,ingressPass) == 0){
                Serial.println("The security system is armed");
                alarmArmed = true;
            }else if(strcmp(passDisarm ,ingressPass)==0){
                Serial.println("The security system is disarmed");
                alarmArmed = false;
            }else{
              Serial.println("Error, wrong password");
            }
            idx = 0;  
        }
    }
    //
    if(alarmArmed){
        if(magneticSensorValue != 4095){
            alarmOn = true;
            digitalWrite(pinLedGreen,LOW);
            digitalWrite(pinLedRed,HIGH);
        }else{
            alarmOn = false;
            digitalWrite(pinLedGreen,HIGH);
            digitalWrite(pinLedRed,LOW);
        }
        if(pirSensorValue){
            alarmOn = true;
            digitalWrite(pinLedGreen,LOW);
            digitalWrite(pinLedRed,HIGH);
        }else{
            alarmOn = false;
            digitalWrite(pinLedGreen,HIGH);
            digitalWrite(pinLedRed,LOW);
        }
    }
    if(alarmOn && alarmArmed){
        digitalWrite(pinBuzzer,HIGH);
        if(millis() - presentTime > 1000){
          toogle = !toogle;          
          digitalWrite(pinLedGreen,toogle);
          digitalWrite(pinLedRed,toogle);
          presentTime = millis();
        }


    }
    
}

  
