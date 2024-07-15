#include "Adafruit_ADS1X15.h"
#include "DFRobot_ESP_EC.h"
#include "EEPROM.h"

int pumpA = 14;
int pumpB = 19;
int button = 5;
const int ecPin = A0;
float EC_O = 0.0;
float EC_O_SUM = 0.0;
float EC_O_AVG = 0.0;
float EC_target = 1.6;

// Define threshold values for EC sensor
const float ecThresholdLow = 1.5;
const float ecThresholdHigh = 2;

unsigned long lastMeasurementTime = 0;
unsigned long lastMixingTime = 0;
const unsigned long measurementInterval = 1000;  // 1 second
const unsigned long mixingTimeInterval = 180000;   // 3 minutes

enum State {
  IDLE,
  MEASURE_EC,
  CALCULATE_NUTRIENT,
  ADD_A,
  MIX_A,
  ADD_B,
  MIX_B,
};

State state = IDLE;

DFRobot_ESP_EC ec;
Adafruit_ADS1115 ads;

int sample_counter = 0;

int T;
//float T;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  EEPROM.begin(32);
  pinMode(pumpA, OUTPUT);
  pinMode(pumpB, OUTPUT);
  pinMode(ecPin, INPUT);
  pinMode(button, INPUT_PULLUP);
  // digitalWrite(pumpA, HIGH);
  // digitalWrite(pumpB, HIGH);
  digitalWrite(pumpA, LOW);
  digitalWrite(pumpB, LOW);

  ec.begin();
  ads.setGain(GAIN_ONE);
  ads.begin();
}

void loop() {
  unsigned long currentTime = millis();

  switch (state) {
    case IDLE:
      if (digitalRead(button) == 0) {
        state = MEASURE_EC;
        sample_counter = 0;
        EC_O_SUM = 0;
        lastMeasurementTime = currentTime;
        Serial.println("Button pressed, stating EC measurement.");
      }
      break;

    case MEASURE_EC:
      if (currentTime - lastMeasurementTime >= measurementInterval) {
        sample_counter++;
        lastMeasurementTime = currentTime;
        EC_O = ec_measurement();

        Serial.print("Current EC value: ");
        Serial.println(EC_O, 4);

        EC_O_SUM = EC_O_SUM + EC_O;

        if (sample_counter >= 10) {
          EC_O_AVG = EC_O_SUM / 10;
          Serial.print("Average EC value: ");
          Serial.println(EC_O_AVG, 4);
          state = CALCULATE_NUTRIENT;
        }
      }
      break;

    case CALCULATE_NUTRIENT:
      Serial.println("Calculating required nutrients...");
      if (EC_O_AVG < ecThresholdLow) {
        float Vn = calculate_required_nutrient(EC_target, EC_O);
        T = calculate_motor_time(Vn) * 1000;  // millisecond
        // T= calculate_motor_time(Vn);
        Serial.print("The value of Vn is: ");
        Serial.print(Vn, 4);
        Serial.println(" ml");
        Serial.print("Time to pump nutrient is: ");
        Serial.print(T);
        Serial.println(" ms");

        lastMixingTime = currentTime;
        state = ADD_A;
      } else {
        Serial.println("EC level is sufficien, going back to IDLE.");
        state = IDLE;
      }
      break;

    case ADD_A:
      if (currentTime - lastMixingTime < T) {
        turn_on_motor_A();
      } else {
        turn_off_motor_A();
        lastMixingTime = currentTime;
        state = MIX_A;
      }
      break;

    case MIX_A:
      if (currentTime - lastMixingTime >= mixingTimeInterval) {
        lastMixingTime = currentTime;
        state = ADD_B;
      }
      break;

    case ADD_B:

      if (currentTime - lastMixingTime < T) {
        turn_on_motor_B();
      } else {
        turn_off_motor_B();
        state = MIX_B;
      }
      break;

    case MIX_B:

      if (currentTime - lastMixingTime >= mixingTimeInterval) {
        state = IDLE;
      }
      break;
  }
}

float ec_measurement() {
  float voltage = 0.0;
  int16_t adc0;

  // Read from ADC0
  adc0 = ads.readADC_SingleEnded(0);
  voltage = adc0 /10;
  // voltage = adc0 * 0.1875 / 1000;        // Convert ADC reading to voltage (assuming ADS1115 gain = 1)
  float EC_O = ec.readEC(voltage, 25);  // Use 25 degrees Celsius as the reference temperature
 
  return EC_O;
}

float calculate_required_nutrient(float EC_target, float EC_O) {
  float Vn = (EC_target - EC_O) / 0.0038;
  return Vn;
}

float calculate_motor_time(float Vn) {
  float T = Vn / 1.295;
  return T;
}

void turn_on_motor_A() {
  // Serial.println("Motor A turned on.");
  digitalWrite(pumpA, HIGH);
  //digitalWrite(pumpA, LOW);
}

void turn_off_motor_A() {
  digitalWrite(pumpA, LOW);
  //digitalWrite(pumpA, HIGH);
  // Serial.println("Motor A turned off.");
}

void turn_on_motor_B() {
  // Serial.println("Motor B turned on.");
  digitalWrite(pumpB, HIGH);
  //digitalWrite(pumpB, LOW);
}

void turn_off_motor_B() {
  digitalWrite(pumpB, LOW);
  //digitalWrite(pumpB, HIGH );
  // Serial.println("Motor B turned off.");
}
