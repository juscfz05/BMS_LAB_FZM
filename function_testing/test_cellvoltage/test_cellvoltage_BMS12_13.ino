bool WarningOvervoltage = false;
bool WarningUndervoltage = false;

void checkcellVoltage(float Vcell[4]) {
  static unsigned long previousVcellMillis = 0;
  const long VcellInterval = 200;
  unsigned long currentMillis = millis();

  if (currentMillis - previousVcellMillis >= VcellInterval) {
    previousVcellMillis = currentMillis;

    if (Vcell[0] > 4.2 || Vcell[1] > 4.2 || Vcell[2] > 4.2 || Vcell[3] > 4.2) {
      WarningOvervoltage = true;
    } else {
      WarningOvervoltage = false;
    }
    if (Vcell[0] < 2.5 || Vcell[1] < 2.5 || Vcell[2] < 2.5 || Vcell[3] < 2.5) {
      WarningUndervoltage = true;
    } else {
      WarningUndervoltage = false;
    }
  }
}

void setup() {
  Serial.begin(9600); // Initialisiert die serielle Kommunikation mit einer Baudrate von 9600
}

void loop() {
  float Vcell[4] = {3.5, 3.7, 4.1, 2.8}; // Beispielwerte für die Zellspannungen
  static unsigned long previousPrintMillis = 0;
  unsigned long currentMillis = millis();

  // Überprüfung der Zellspannungen
  checkcellVoltage(Vcell);

  // Berechnung der Zeitdifferenz zwischen zwei Ausgaben
  unsigned long printTimeDifference = currentMillis - previousPrintMillis;

  Serial.print("Zeitdifferenz: ");
  Serial.print(printTimeDifference);
  Serial.println(" ms");

  previousPrintMillis = currentMillis;


  delay(199); // Kurze Verzögerung, um die Ausgabe lesbar zu machen
}
