const int sensorPin = A0;
const float baselineTemp = 20.0;

void setup() {
  Serial.begin(9600);
  for (int pinNumber = 2; pinNumber < 5; pinNumber++) {
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }
}

void loop() {
  int sensorVal = analogRead(sensorPin);
  float voltage = sensorVal * 5.0 / 1024.0;
  float temperature = (voltage - 0.5) * 100;

  Serial.print("Sensor Value: ");
  Serial.print(sensorVal);
  Serial.print(", Volts: ");
  Serial.print(voltage);
  Serial.print(", degrees C: ");
  Serial.println(temperature);

  updateLEDs(temperature);
  delay(5);
}

void updateLEDs(float temperature) {
  int ledState = temperature >= baselineTemp ? HIGH : LOW;
  digitalWrite(2, ledState);

  ledState = temperature >= baselineTemp + 2 && temperature < baselineTemp + 6 ? HIGH : LOW;
  digitalWrite(3, ledState);

  ledState = temperature >= baselineTemp + 6 ? HIGH : LOW;
  digitalWrite(4, ledState);
}
