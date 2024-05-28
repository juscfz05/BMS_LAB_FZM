bool WarningOvertemp = false;                     // Globale Definition der Übertemperaturwarnung als boolesche Variable

void checkTemperature(float Tcell[4])              // Definition der Temperaturüberwachungsfunktion mit der Übergabe eines Arrays mit 4 Temperaturwerten 
{
  static unsigned long previousTempMillis = 0;    // Variable für Zeitstempel der letzten Temperaturmessung. Static, dass nur bei erster Initialisierung der Wert auf 0 gesetzt werden soll. 
  const long tempInterval = 3000;                 // Konstantes Zeitintervall in ms, in dem die Temperatur gemessen werden soll (Temperaturmessung alle 3s)
  unsigned long currentMillis = millis();         // Variable currentMillis = aktuelle Zeit in ms 
  
  if (currentMillis - previousTempMillis >= tempInterval) {     // Überprüfen, ob seit letzter Messung mindestens 3 Sekunden vergangen sind. Falls nein, wird die Funktion übersprungen.
    previousTempMillis = currentMillis;                         // Setzen des Zeitstempels der neuen Messung

    if (Tcell[0] > 60.0 || Tcell[1] > 60.0 || Tcell[2] > 60.0 || Tcell[3] > 60.0) {   // Überprüfen, ob eine der Temperaturen über 60 Grad liegt
        WarningOvertemp = true;       // wenn eine Zelltemperatur >60 Grad: boolesche Variable WarningOvertemp = true
    } else {
        WarningOvertemp = false;      // wenn alle Temperaturen <=60 Grad: boolesche Variable WarningOvertemp = false
    }   
  }
}