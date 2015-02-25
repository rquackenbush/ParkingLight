#include <EEPROM.h>

/* --- Configuration --- */
uint16_t calibratedValue = 20;
uint16_t hysteresis = 5;

const uint8_t NumberOfParkingSpaces = 1;
const uint8_t numberOfLights = 3;
const uint8_t lightPinAssignments[numberOfLights] = { 2, 3, 4 };
/* --------------------- */

const uint8_t AnalogPin = 0;
int8_t currentLightIndex = numberOfLights;
uint16_t thresholds[numberOfLights];

enum ParkingState {
  PARKING_STATE_IDLE,
  PARKING_STATE_MOVING
};

void SetLight(uint8_t lightIndex, uint8_t isOn) {
  
  uint8_t value = isOn ? LOW : HIGH;
  
  digitalWrite(lightPinAssignments[lightIndex], value);    
}

void Calibrate() {
  
}

void SetThresholds(uint16_t calibratedValue) {
  
  for(int lightIndex = 0; lightIndex < numberOfLights; lightIndex++) {
    
    thresholds[lightIndex] = calibratedValue;
    
    calibratedValue += 20;
    
  }
}

void LightDemo() {
  
  for(int lightIndex = 0; lightIndex < numberOfLights; lightIndex++) {
    
    delay(300);
    
    SetLight(lightIndex, false);
    
  }
}

void setup()
{
  SetThresholds(calibratedValue);
  
  for(int lightIndex = 0; lightIndex < numberOfLights; lightIndex++) {
    pinMode(lightPinAssignments[lightIndex], OUTPUT);
  }
     
  LightDemo();
  
  //Serial.begin(9600);
}

int8_t GetLightIndex(uint16_t value) {
  
  uint16_t threshold;
  
  for(uint8_t lightIndex = 0; lightIndex < numberOfLights; lightIndex++) {
    
    threshold = thresholds[lightIndex];
    
    if (lightIndex == currentLightIndex) {
      threshold += hysteresis;
    }
    else if (lightIndex == (currentLightIndex + 1)) {
      threshold -= hysteresis;
    }
    
    if (value <= threshold) {
      currentLightIndex = lightIndex;
      return currentLightIndex;
    }
  }
  
  currentLightIndex = numberOfLights;
  return currentLightIndex;
}

void LightProcess(){
  
    char temp[20];
    int value = analogRead(AnalogPin);
    
    uint8_t lightBand = GetLightIndex(value);
  
    for(int lightIndex = 0; lightIndex < numberOfLights; lightIndex++) {
      
          //sprintf(temp, "Treshold[%u] = %u", lightIndex, thresholds[lightIndex]);
          //Serial.println(temp);
    
          SetLight(lightIndex, lightBand == lightIndex);
        
    }
    
    //sprintf(temp,"Value / Band: (%u,%d)", value, lightBand);
    //Serial.println(temp);
}

void loop()
{   
    LightProcess();
//delay(250);

    /* Show this over the  */
    //Serial.println(val);
}
