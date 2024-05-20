// Globale Variablen für die Zellspannungen, damit diese an die Funktion checkcellVoltage übergeben werden können
float Vcell1, Vcell2, Vcell3, Vcell4;                         // Definition der globalen Variable VCellx für die Zellspannung x

// Funktion zum Auslesen der Zellspannungen
void callofcellVoltage() {
  Vcell1 = getCellVoltage(1); // Auslesen und Speichern der Zellspannung 1
  Vcell2 = getCellVoltage(2); // Auslesen und Speichern der Zellspannung 2
  Vcell3 = getCellVoltage(3); // Auslesen und Speichern der Zellspannung 3
  Vcell4 = getCellVoltage(4); // Auslesen und Speichern der Zellspannung 4
}

// Definition der Zellspannungsüberwachungsfunktion
void checkcellVoltage() 
{
  static unsigned long previousVcellMillis = 0;               // Variable für Zeitstempel der letzten Spannungsmessung 
  const long VcellInterval = 200;                             // Konstantes Zeitintervall in ms in dem die Spannung gemessen werden soll (Spannungsmessung alle 200ms)
  unsigned long currentMillis = millis();                     // Variable currentMillis = aktuelle Zeit in ms 

  if (currentMillis - previousVcellMillis >= VcellInterval) {   // Überprüfen, ob seit letzter Messung min. 200ms vergangen sind. Falls nein, wird Funktion übersprungen.

    previousVcellMillis = currentMillis;                      // Setzen des Zeitstempels der neuen Messung

    callofcellVoltage();                                      // Aufruf der Funktion, um Zellspannungen zu aktualisieren

    if (Vcell1 > 4.2 || Vcell2 > 4.2 || Vcell3 > 4.2 || Vcell4 > 4.2) {   // Überprüfen, ob eine der Spannungen über 4,2 V liegt
      setWarningOvervoltage(true);   // wenn eine Zellspannung > 4,2 V, wird eine Überspannungswarnung für die VCU auf wahr gesetzt
    } else {
      setWarningOvervoltage(false);  // wenn alle Spannungen <= 4,2 V, wird die Überspannungswarnung für die VCU auf falsch festgelegt 
    }
    if (Vcell1 < 2.5 || Vcell2 < 2.5 || Vcell3 < 2.5 || Vcell4 < 2.5) {   // Überprüfen, ob eine der Spannungen unter 2,5 V liegt
      setWarningUndervoltage(true);   // wenn eine Zellspannung < 2,5 V, wird eine Unterspannungswarnung für die VCU auf wahr gesetzt
    } else {
      setWarningUndervoltage(false);  // wenn alle Spannungen >= 2,5 V, wird die Unterspannungswarnung für die VCU auf falsch festgelegt
    }
    
  }
}
