const int analogPin = A0;    // Analoger Pin für die Spannungsmessung
const int thresholdLow = 60;  // Schwelle für niedrige Spannung
const int thresholdHigh = 200; // Schwelle für hohe Spannung
const int outputPin = 13;     // Ausgangspin zum Schalten

void setup() {
  pinMode(outputPin, OUTPUT);  // Output-Pin festlegen
  Serial.begin(115200);          // Serielle Kommunikation starten
}

void loop() {
  int sensorValue = analogRead(analogPin);  // Spannungswert lesen
  Serial.write("Got value: ");
  Serial.println(sensorValue);
  

  if (sensorValue < thresholdLow) {
    digitalWrite(outputPin, LOW);   // Ausgang ausschalten
  } else if (sensorValue > thresholdHigh) {
    digitalWrite(outputPin, HIGH);  // Ausgang einschalten
  }

  delay(1000);  // 5 Sekunden warten
}
