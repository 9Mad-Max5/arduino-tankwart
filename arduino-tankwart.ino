const int analogPin = A0;    // Analoger Pin für die Spannungsmessung
const int thresholdLow = 60;  // Schwelle für niedrige Spannung
const int thresholdHigh = 200; // Schwelle für hohe Spannung
const int outputPin = 13;     // Ausgangspin zum Schalten

const int lowerSensorPin = 2;     // Pin für den unteren Sensor (Keine Flüssigkeit)
const int upperSensorPin = 3;     // Pin für den oberen Sensor (Flüssigkeit im Tank)

const int maxOnTime = 12000;      // Maximale Einschaltdauer in Millisekunden (12 Sekunden)
const int cooldownTime = 60000;   // Cool-Down-Zeit in Millisekunden (60 Sekunden)

unsigned long lastActivationTime = 0;  // Zeitpunkt der letzten Aktivierung des Ausgangs

void setup() {
  pinMode(outputPin, OUTPUT);          // Output-Pin festlegen
  pinMode(lowerSensorPin, INPUT);      // Eingangs-Pin für den unteren Sensor festlegen
  pinMode(upperSensorPin, INPUT);      // Eingangs-Pin für den oberen Sensor festlegen
  Serial.begin(115200);          // Serielle Kommunikation starten
}

void loop() {
  int sensorValue = analogRead(analogPin);  // Spannungswert lesen
  int lowerSensorValue = digitalRead(lowerSensorPin); // Wert des unteren Sensors lesen
  int upperSensorValue = digitalRead(upperSensorPin); // Wert des oberen Sensors lesen
  Serial.write("Analog value: ");
  Serial.println(sensorValue);
  Serial.write("Leer Melder, leer = HIGH/True: "); // High wenn keine Flüssigkeit detektiert wird
  Serial.println(lowerSensorValue);
  Serial.write("Voll Melder, voll = LOW/False: "); // High wenn Flüssigkeit detektiert wird
  Serial.println(upperSensorValue);
  /*
  Die Logik wegen präsenz und Abwesenheit soll der Drahtbruchsicherheit dienen.
  Wir verkraften ist das es leer ist in diesem Szenario deswegen warten auf ein HIGH wobei dieser Fall eher unkritisch ist
  Da es schlimmer ist wenn es überläuft wie das es nicht pumpt nehmen wir bei Abriss == Voll
  */

  if ((sensorValue < thresholdLow && !isOutputActive()) || (lowerSensorValue == HIGH && !isOutputActive())) {
    activateOutput();
  } else if (sensorValue > thresholdHigh || upperSensorValue == LOW) {
    deactivateOutput();
  }

  delay(1000);  // Kurze Verzögerung zur Entprellung des Sensors
}

bool isOutputActive() {
  return (digitalRead(outputPin) == LOW);
}

void activateOutput() {
  unsigned long currentTime = millis();
  
  // Überprüfen, ob der Cool-Down-Zeitraum abgelaufen ist
  if (currentTime - lastActivationTime >= cooldownTime) {
    digitalWrite(outputPin, LOW);    // Ausgang einschalten
    lastActivationTime = currentTime; // Zeitpunkt der Aktivierung aktualisieren
    delay(maxOnTime);                  // Maximale Einschaltdauer abwarten
    digitalWrite(outputPin, HIGH);     // Ausgang ausschalten
  }
}

void deactivateOutput() {
  digitalWrite(outputPin, HIGH);   // Ausgang ausschalten
}

