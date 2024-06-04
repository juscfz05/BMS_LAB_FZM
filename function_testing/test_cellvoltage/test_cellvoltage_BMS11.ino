// Header mit Definitionen von Variablen
bool WarningOvervoltage = false;                                                  // Boolescher Wert für Überspannung
bool WarningUndervoltage = false;                                                 // Boolescher Wert für Unterspannung

void checkcellVoltage(float Vcell[4])                                             // Definition der Zellspannungsüberwachungsfunktion mit der Übergabe eines Arrays mit 4 Zellspannungen 
{
  static unsigned long previousVcellMillis = 0;                                   // Variable für Zeitstempel der letzten Spannungsmessung 
  const long VcellInterval = 200;                                                 // Konstantes Zeitintervall in ms in dem die Spannung gemessen werden soll (Spannungsmessung alle 200ms)
  unsigned long currentMillis = millis();                                         // Variable currentMillis = aktuelle Zeit in ms 

  if (currentMillis - previousVcellMillis >= VcellInterval) {                     // Überprüfen, ob seit letzter Messung min. 200ms vergangen sind. Falls nein, wird Funktion übersprungen.

    previousVcellMillis = currentMillis;                                          // Setzen des Zeitstempels der neuen Messung

    if (Vcell[0] > 4.2 || Vcell[1] > 4.2 || Vcell[2] > 4.2 || Vcell[3] > 4.2) {   // Überprüfen, ob eine der Spannungen über 4,2 V liegt
      WarningOvervoltage = true;                                                  // wenn eine Zellspannung > 4,2 V: boolesche Variable WarningOvervoltage = true
    } else {
      WarningOvervoltage = false;                                                 // wenn alle Zellspannungen < 4,2 V: boolesche Variable WarningOvervoltage = false
    }   
    if (Vcell[0] < 2.5 || Vcell[1] < 2.5 || Vcell[2] < 2.5 || Vcell[3] < 2.5) {   // Überprüfen, ob eine der Spannungen unter 2,5 V liegt
      WarningUndervoltage = true;                                                 // wenn eine Zellspannung < 2,5 V: boolesche Variable WarningUndervoltage = true
    } else {
      WarningUndervoltage = false;                                                // wenn alle Zellspannungen > 2,5 V: boolesche Variable WarningUndervoltage = false
    }   
  }
}


void setup() {
  Serial.begin(9600);  // Initialisieren der seriellen Kommunikation
  // Weitere Initialisierungen, falls notwendig
}

void loop() {
  // Beispielhafte Zellspannungen (diese würden normalerweise von Sensoren gelesen)
  float Vcell[4] = {4.2, 4.3, 2.5, 2.6};  // Beispielwerte, sollten in einer echten Anwendung von Sensoren gelesen werden

  checkcellVoltage(Vcell);
  Serial.print("Warning Overvoltage: ");
  Serial.println(WarningOvervoltage);
  Serial.print("Warning Undervoltage: ");
  Serial.println(WarningUndervoltage);
  
  delay(1000);  // Kleine Verzögerung zur Stabilisierung des Loops
}