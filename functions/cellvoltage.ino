// Header mit Definitionen von Variablen

bool WarningOvervoltage = false; // Boolescher Wert für Überspannung
bool WarningUndervoltage = false; // Boolescher Wert für Unterspannung
unsigned long previousVcellMillis = 0; // Variable für Zeitstempel der letzten Spannungsmessung 
const long VcellInterval = 200;       // Konstantes Zeitintervall in ms in dem die Spannung gemessen werden soll (Spannungsmessung alle 200ms)


// Definition der Zellspannungsüberwachungsfunktion

void checkcellVoltage() 
{
  unsigned long currentMillis = millis();                     // Variable currentMillis = aktuelle Zeit in ms 

  if (currentMillis - previousVcellMillis >= VcellInterval) {   // Überprüfen, ob seit letzter Messung min. 200ms vergangen sind. Falls nein, wird Funktion übersprungen.

    previousVcellMillis = currentMillis;                       // Setzen des Zeitstempels der neuen Messung

    float Vcell1 = getCellVoltage(1); // Auslesen und abspeichern Zellspannung 1 als Vcell1
    float Vcell2 = getCellVoltage(2); // Auslesen und abspeichern Zellspannung 2 als Vcell2
    float Vcell3 = getCellVoltage(3); // Auslesen und abspeichern Zellspannung 3 als Vcell3
    float Vcell4 = getCellVoltage(4); // Auslesen und abspeichern Zellspannung 4 als Vcell4

    if (Vcell1 > 4.2 || Vcell2 > 4.2 || Vcell3 > 4.2 || Vcell4 > 4.2) {   // Überprüfen, ob eine der Spannungen über 4,2 V liegt
      WarningOvervoltage = true;   // wenn eine Zellspannung > 4,2 V: boolesche Variable WarningOvervoltage = true
    } else {
      WarningOvervoltage = false;  // wenn alle Spannungen <= 4,2 V: boolesche Variable WarningOvervoltage = false
    }
    if (Vcell1 < 2.5 || Vcell2 < 2.5 || Vcell3 < 2.5 || Vcell4 < 2.5) {   // Überprüfen, ob eine der Spannungen unter 2,5 V liegt
      WarningUndervoltage = true;   // wenn eine Zellspannung < 2,5 V: boolesche Variable WarningUndervoltage = true
    } else {
      WarningUndervoltage = false;  // wenn alle Spannungen >= 2,5 V: boolesche Variable WarningUndervoltage = false
    }
      
  }
}
