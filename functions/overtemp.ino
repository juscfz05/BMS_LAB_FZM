bool WarningOvertemp = false;                     // Globale Definition der Übertemperaturwarnung als boolesche Variable

void checkTemperature(float Tcell[4])             // Definition der Temperaturüberwachungsfunktion mit der Übergabe eines Arrays mit 4 Temperaturwerten 
{
  static unsigned long previousTempMillis = 0;    // Variable für Zeitstempel der letzten Temperaturmessung. Static, dass nur bei erster Initialisierung der Wert auf 0 gesetzt werden soll. 
  const long tempInterval = 3000;                 // Konstantes Zeitintervall in ms, in dem die Temperatur gemessen werden soll (Temperaturmessung alle 3s)
  unsigned long currentMillis = millis();         // Variable currentMillis = aktuelle Zeit in ms 

  // Überprüfen, ob seit letzter Messung mindestens 3 Sekunden vergangen sind. Falls nein, wird die Funktion übersprungen.
  if (currentMillis - previousTempMillis >= tempInterval) {
    previousTempMillis = currentMillis;           // Setzen des Zeitstempels der neuen Messung

    WarningOvertemp = false;                      // Variable für den Rückgabewert zurücksetzen

    // Schleife zur Überprüfung aller Elemente des Arrays Tcell
    for (int i = 0; i < 4; i++) {
      if (Tcell[i] > 60) {
        WarningOvertemp = true;                   // Setzen der Warnung bei Übertemperatur
        break;                                    // Schleife abbrechen, wenn Übertemperatur erkannt wurde
      }
    }
  }
}