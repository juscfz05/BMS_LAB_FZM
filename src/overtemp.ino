// Header mit Definitionen von Variablen

bool overTemperatureDetected = false; // Boolescher Wert für Übertemperatur
unsigned long previousTempMillis = 0; // Variable für Zeitstempel der letzten Temperaturmessung 
const long tempInterval = 3000;       // Konstantes Zeitintervall in ms in dem die Temperatur gemessen werden soll (Temperaturmessung alle 3s)


// Definition der Temperaturüberwachungsfunktion

void checkTemperature() 
{
  unsigned long currentMillis = millis();                     // Variable currentMillis = aktuelle Zeit in ms 

  if (currentMillis - previousTempMillis >= tempInterval) {   // Überprüfen, ob seit letzter Messung min. 3s vergangen sind. Falls nein, wird Funktion übersprungen.

    previousTempMillis = currentMillis;                       // Setzen des Zeitstempels der neuen Messung

    float cell1Temp = getCellTemp(1); // Auslesen und abspeichern Zelltemperatur 1 als cell1Temp
    float cell2Temp = getCellTemp(2); // Auslesen und abspeichern Zelltemperatur 2 als cell2Temp
    float cell3Temp = getCellTemp(3); // Auslesen und abspeichern Zelltemperatur 3 als cell3Temp
    float cell4Temp = getCellTemp(4); // Auslesen und abspeichern Zelltemperatur 4 als cell4Temp
      
    if (cell1Temp > 60 || cell2Temp > 60 || cell3Temp > 60 || cell4Temp > 60) {   // Überprüfen, ob eine der Temperaturen über 60 Grad liegt
      overTemperatureDetected = true;   // wenn eine Zelltemperatur >60 Grad: boolesche Variable overTemperatureDetected = true
    } else {
      overTemperatureDetected = false;  // wenn alle Temperaturen <=60 Grad: boolesche Variable overTemperatureDetected = false
    }
  }
}