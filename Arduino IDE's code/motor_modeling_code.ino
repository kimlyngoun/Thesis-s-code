
int ledPin = 4;
int ledPin_1 = 13;

int time_now = 0;
int time_last = 0;
int count_target = 0;
int count = 0;


//########################################## SETUP ######################################################
void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 0);
  time_now = time_last = millis();
}

//########################################## Task ######################################################
void loop() {
  if (Serial.available()) {
    int time_cmd = Serial.parseInt();
    if (time_cmd > 0) {
      count_target = time_cmd;
      Serial.print("Turn on motor for: ");
      Serial.print(time_cmd);
      Serial.println(" sec");
    }
    else if (time_cmd == -1) {
      count_target = 0;
      Serial.println("Force stop");
    }
  }

  time_now = millis();
  if (count_target > 0) {
    if (time_now - time_last >= 1000) {
      time_last = time_now;
      count++;
      if (count >= count_target) {
        Serial.println("Turn off motor");
        count_target = 0;
        digitalWrite(ledPin, 0);
      }
      else {
        digitalWrite(ledPin, 1);
        // Serial.print("time: ");
        // Serial.print(count);
        // Serial.println("s");
      }
    }
  }
  else {
    time_last = time_now;
    count = 0;
    digitalWrite(ledPin, 0);
  }
}
