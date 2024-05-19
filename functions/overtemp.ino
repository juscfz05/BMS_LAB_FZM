// Globale Variablen für die Zelltemperaturen. Werden benötigt, um Variablenübergabe zwischen Funktionen zu ermöglichen.
float Tcell1;   // Definition der globalen Variable Tcell1 für die Zelltemperatur 1
float Tcell2;   // Definition der globalen Variable Tcell1 für die Zelltemperatur 2
float Tcell3;   // Definition der globalen Variable Tcell1 für die Zelltemperatur 3
float Tcell4;   // Definition der globalen Variable Tcell1 für die Zelltemperatur 4

// Definition der Temperaturauslesefunktion
void readTemperature() 
{
    Tcell1 = getCellTemp(1); // Auslesen und abspeichern Zelltemperatur 1 als Tcell1
    Tcell2 = getCellTemp(2); // Auslesen und abspeichern Zelltemperatur 2 als Tcell2
    Tcell3 = getCellTemp(3); // Auslesen und abspeichern Zelltemperatur 3 als Tcell3
    Tcell4 = getCellTemp(4); // Auslesen und abspeichern Zelltemperatur 4 als Tcell4
}

// Definition der Temperaturüberwachungsfunktion
void checkTemperature() 
{
  static unsigned long previousTempMillis = 0;    // Variable für Zeitstempel der letzten Temperaturmessung. Static, dass nur bei erster Initialisierung der Wert auf 0 gesetzt werden soll. 
  const long tempInterval = 3000;                 // Konstantes Zeitintervall in ms in dem die Temperatur gemessen werden soll (Temperaturmessung alle 3s)
  unsigned long currentMillis = millis();         // Variable currentMillis = aktuelle Zeit in ms 

  if (currentMillis - previousTempMillis >= tempInterval) {   // Überprüfen, ob seit letzter Messung min. 3s vergangen sind. Falls nein, wird Funktion übersprungen.

    previousTempMillis = currentMillis;                       // Setzen des Zeitstempels der neuen Messung

    readTemperature();              // Aufruf der Temperaturauslesefunktion
      
    if (Tcell1 > 60 || Tcell2 > 60 || Tcell3 > 60 || Tcell4 > 60) {   // Überprüfen, ob eine der Temperaturen über 60 Grad liegt
      setWarningOvertemp(true);    // Setzen der Übertemperaturwarnung, dass die VCU die Fahrleistung reduziert
    } else {
      setWarningOvertemp(false);    // Entfernen der Übertemperaturwarnung, dass die VCU die Leistung nicht mehr reduziert
    }
  }
}
