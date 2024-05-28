// Header mit Definitionen von Variablen
bool WarningOvervoltage = false;                                 // Boolescher Wert für Überspannung
bool WarningUndervoltage = false;                                // Boolescher Wert für Unterspannung

void checkcellVoltage(float Vcell[4])                            // Definition der Zellspannungsüberwachungsfunktion mit der Übergabe eines Arrays mit 4 Temperaturwerten 
{
  static unsigned long previousVcellMillis = 0;                  // Variable für Zeitstempel der letzten Spannungsmessung 
  const long VcellInterval = 200;                                // Konstantes Zeitintervall in ms in dem die Spannung gemessen werden soll (Spannungsmessung alle 200ms)
  unsigned long currentMillis = millis();                        // Variable currentMillis = aktuelle Zeit in ms 

  if (currentMillis - previousVcellMillis >= VcellInterval) {    // Überprüfen, ob seit letzter Messung min. 200ms vergangen sind. Falls nein, wird Funktion übersprungen.

    previousVcellMillis = currentMillis;                         // Setzen des Zeitstempels der neuen Messung
    WarningOvervoltage = false;                                  // Variable für den Rückgabewert WarningOvervoltage zurücksetzen
    WarningUndervoltage = false;                                 // Variable für den Rückgabewert WarningUndervoltage zurücksetzen

    for (int i = 0; i < 4; i++) {                                // Schleife mit i 1-4 für die jeweiligen Zellen
      if (Vcell[i] > 4.2) {                                      // Überprüfen, ob eine der Spannungen über 4,2 V liegt
        WarningOvervoltage = true;                               // wenn eine Zellspannung > 4,2 V: boolesche Variable WarningOvervoltage = true
        break;                                                   // wenn alle Spannungen < 4,2 V: boolesche Variable WarningOvervoltage = false
      }
    }
    for (int i = 0; i < 4; i++) {                                // Schleife mit i 1-4 für die jeweiligen Zellen
      if (Vcell[i] < 2.5) {                                      // Überprüfen, ob eine der Spannungen unter 2,5 V liegt
        WarningUndervoltage = true;                              // wenn eine Zellspannung < 2,5 V: boolesche Variable WarningUndervoltage = true
        break;                                                   // wenn alle Spannungen > 2,5 V: boolesche Variable WarningUndervoltage = false
      }
    }
  }
}