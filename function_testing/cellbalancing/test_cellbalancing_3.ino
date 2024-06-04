// Globale Variablen
int cell1counter = 0;
int cell2counter = 0;
int cell3counter = 0;
int cell4counter = 0;
int setbalActive = 0;// Variable für das Aktivieren des Cell-Balancing
float VCell[4] = {4.0, 4.0, 4.0, 4.0};

void checkcellbalancing(float VCell[4]);

void setup() {
  // Initialisiere den seriellen Monitor
  Serial.begin(9600);
}

void loop() {
  // Testwerte für die Zellen

  for (int i = 0; i < 1000; i++) {
    if (i < 3) {
      VCell[0] = 4.5;
      VCell[1] = 4.0;
      VCell[2] = 4.0;
      VCell[3] = 4.0;
    } else if (i >= 20 && i < 25)
    {
      VCell[0] = 4.5;
      VCell[1] = 4.0;
      VCell[2] = 4.0;
      VCell[3] = 4.0;
      Serial.println(i);
    }
    else {
      VCell[0] = 4.0;
      VCell[1] = 4.0;
      VCell[2] = 4.0;
      VCell[3] = 4.0;
    }


    // Balancing-Funktion
    checkcellbalancing(VCell);

    // Ausgabe der Testwerte und des Balancing-Status auf dem seriellen Monitor
    /*Serial.print("VCell[0]: ");
      Serial.print(VCell[0]);
      Serial.print(" V");
      Serial.print(" | VCell[1]: ");
      Serial.print(VCell[1]);
      Serial.print(" V");
      Serial.print(" | VCell[2]: ");
      Serial.print(VCell[2]);
      Serial.print(" V");
      Serial.print(" | VCell[3]: ");
      Serial.print(VCell[3]);
      Serial.println(" V");
    */
    Serial.print("setbalActive: ");
    Serial.println(setbalActive);

    // Kurze Verzögerung für bessere Lesbarkeit
    delay(200);
  }
}

void checkcellbalancing(float VCell[4])
{
  static unsigned long previousCellBalCalcMillis = 0; // Variable für Zeitstempel der letzten Cell-Balancing-Berechnung
  static unsigned long previousCellBalMillis = 0;     // Variable für Zeitstempel des letzten Cell-Balancing
  //static bool setbalActive = false;                      // Variable fürs Aktivieren des Cell-Balancing
  const long cellbalcalcInterval = 200;               // konstantes Zeitintervall (200 ms) in dem die Cell-Balancing-Berechnung durchgeführt wird
  const long cellbalInterval = 400;                   // konstantes Zeitintervall (400 ms) in dem das Cell-Balancing durchgeführt wird
  unsigned long currentMillis = millis();             // aktuelle Zeit in ms

  if (currentMillis - previousCellBalCalcMillis >= cellbalcalcInterval) {  // Wird nur ausgeführt, wenn seit letzter Messung 200 ms vergangen sind

    previousCellBalCalcMillis = currentMillis;        // Setzen des Zeitstempels der neuen Messung

    float Mittelwert = (VCell[0] + VCell[1] + VCell[2] + VCell[3]) / 4;                        // Berechnung Mittelwert der 4 Zellspannungen

    float Standardabweichung = sqrt((pow(VCell[0] - Mittelwert, 2) + pow(VCell[1] - Mittelwert, 2) // Berechnung Standardabweichung der 4 Zellspannungen
                                     + pow(VCell[2] - Mittelwert, 2) + pow(VCell[3] - Mittelwert, 2)) / 3);

    // Wenn eine Zellspannung zu groß oder zu klein ist, wird der jeweilige cellcounter hochgezählt
    if (abs(VCell[0] - Mittelwert) > Standardabweichung) {
      cell1counter = cell1counter + 1;
    } else {
      cell1counter = 0;
    }

    if (abs(VCell[1] - Mittelwert) > Standardabweichung) {
      cell2counter = cell2counter + 1;
    } else {
      cell2counter = 0;
    }

    if (abs(VCell[2] - Mittelwert) > Standardabweichung) {
      cell3counter = cell3counter + 1;
    } else {
      cell3counter = 0;
    }

    if (abs(VCell[3] - Mittelwert) > Standardabweichung) {
      cell4counter = cell4counter + 1;
    } else {
      cell4counter = 0;
    }

  }

  if (currentMillis - previousCellBalMillis >= cellbalInterval) {  // Wird nur ausgeführt, wenn seit letzter Messung 400 ms vergangen sind

    previousCellBalMillis = currentMillis;        // Setzen des Zeitstempels der neuen Messung

    // Wenn eine Zellspannung einer Zelle 4 mal hintereinander zu sehr abweicht wird das Cell Balancing der jeweiligen Zelle aktiviert
    if (cell1counter >= 4) {
      cell1counter = 0;
      setbalActive = 1; // Cell 1 balancieren
    } else if (cell2counter >= 4) {
      cell2counter = 0;
      setbalActive = 2; // Cell 2 balancieren
    } else if (cell3counter >= 4) {
      cell3counter = 0;
      setbalActive = 3; // Cell 3 balancieren
    } else if (cell4counter >= 4) {
      cell4counter = 0;
      setbalActive = 4; // Cell 4 balancieren
    } else {
      setbalActive = 0; // Keine Zelle balancieren
    }
  } else {
    setbalActive = 0; // Keine Zelle balancieren <- nur notwendig wenn Anforderung so gestellt
  }
}
