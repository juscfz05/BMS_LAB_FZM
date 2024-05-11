// Header mit Definitionen von Variablen

bool overVoltageDetected = false; // Boolescher Wert für Überspannung
bool underVoltageDetected = false; // Boolescher Wert für Unterspannung
unsigned long previousTempMillis = 0; // Variable für Zeitstempel der letzten Spannungsmessung 
const long tempInterval = 200;       // Konstantes Zeitintervall in ms in dem die Spannung gemessen werden soll (Spannungsmessung alle 200ms)


// Definition der Zellspannungsüberwachungsfunktion

void checkcellVoltage() 
{
  unsigned long currentMillis = millis();                     // Variable currentMillis = aktuelle Zeit in ms 

  if (currentMillis - previousTempMillis >= tempInterval) {   // Überprüfen, ob seit letzter Messung min. 200ms vergangen sind. Falls nein, wird Funktion übersprungen.

    previousTempMillis = currentMillis;                       // Setzen des Zeitstempels der neuen Messung

    float cell1Volt = getCellVoltage(1); // Auslesen und abspeichern Zellspannung 1 als cell1Volt
    float cell2Volt = getCellVoltage(2); // Auslesen und abspeichern Zellspannung 2 als cell2Volt
    float cell3Volt = getCellVoltage(3); // Auslesen und abspeichern Zellspannung 3 als cell3Volt
    float cell4Volt = getCellVoltage(4); // Auslesen und abspeichern Zellspannung 4 als cell4Volt

    if (cell1Volt > 4.2 || cell2Volt > 4.2 || cell3Volt > 4.2 || cell4Volt > 4.2) {   // Überprüfen, ob eine der Spannungen über 4,2 V liegt
      overVoltageDetected = true;   // wenn eine Zellspannung > 4,2 V: boolesche Variable overVoltageDetected = true
    } else {
      overVoltageDetected = false;  // wenn alle Spannungen <= 4,2 V: boolesche Variable overVoltageDetected = false
    }
    if (cell1Volt < 2.5 || cell2Volt < 2.5 || cell3Volt < 2.5 || cell4Volt < 2.5) {   // Überprüfen, ob eine der Spannungen unter 2,5 V liegt
      underVoltageDetected = true;   // wenn eine Zellspannung < 2,5 V: boolesche Variable underVoltageDetected = true
    } else {
      underVoltageDetected = false;  // wenn alle Spannungen >= 2,5 V: boolesche Variable underVoltageDetected = false
    }
   
  }
}
