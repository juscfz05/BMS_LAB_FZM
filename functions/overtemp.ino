// Header mit Definitionen von Variablen

bool WarningOvertemp = false; // Boolescher Wert für Übertemperatur
unsigned long previousTempMillis = 0; // Variable für Zeitstempel der letzten Temperaturmessung 
const long tempInterval = 3000;       // Konstantes Zeitintervall in ms in dem die Temperatur gemessen werden soll (Temperaturmessung alle 3s)


// Definition der Temperaturüberwachungsfunktion

void checkTemperature() 
{
  unsigned long currentMillis = millis();                     // Variable currentMillis = aktuelle Zeit in ms 

  if (currentMillis - previousTempMillis >= tempInterval) {   // Überprüfen, ob seit letzter Messung min. 3s vergangen sind. Falls nein, wird Funktion übersprungen.

    previousTempMillis = currentMillis;                       // Setzen des Zeitstempels der neuen Messung

    float Tcell1 = getCellTemp(1); // Auslesen und abspeichern Zelltemperatur 1 als cell1Temp
    float Tcell2 = getCellTemp(2); // Auslesen und abspeichern Zelltemperatur 2 als cell2Temp
    float Tcell3 = getCellTemp(3); // Auslesen und abspeichern Zelltemperatur 3 als cell3Temp
    float Tcell4 = getCellTemp(4); // Auslesen und abspeichern Zelltemperatur 4 als cell4Temp
      
    if (Tcell1 > 60 || Tcell2 > 60 || Tcell3 > 60 || Tcell4 > 60) {   // Überprüfen, ob eine der Temperaturen über 60 Grad liegt
      WarningOvertemp = true;   // wenn eine Zelltemperatur >60 Grad: boolesche Variable overTemperatureDetected = true
    } else {
      WarningOvertemp = false;  // wenn alle Temperaturen <=60 Grad: boolesche Variable overTemperatureDetected = false
    }
  }
}