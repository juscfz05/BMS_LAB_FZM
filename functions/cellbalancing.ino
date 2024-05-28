void checkcellbalancing(float VCell[4])
{
  static unsigned long previousCellBalCalcMillis = 0; // Variable für Zeitstempel der letzten Cell-Balancing-Berechnung
  static unsigned long previousCellBalMillis = 0;     // Variable für Zeitstempel des letzten Cell-Balancing
  static bool setbalActive = false;                      // Variable fürs Aktivieren des Cell-Balancing
  const long cellbalcalcInterval = 200;               // konstantes Zeitintervall (200 ms) in dem die Cell-Balancing-Berechnung durchgeführt wird
  const long cellbalInterval = 400;                   // konstantes Zeitintervall (400 ms) in dem das Cell-Balancing durchgeführt wird
  unsigned long currentMillis = millis();             // aktuelle Zeit in ms

  if (currentMillis - previousCellBalCalcMillis >= cellbalcalcInterval) {  // Wird nur ausgeführt, wenn seit letzter Messung 200 ms vergangen sind

    previousCellBalCalcMillis = currentMillis;        // Setzen des Zeitstempels der neuen Messung

    float Mittelwert = (VCell[0] + VCell[1] + VCell[2] + VCell[3])/4;                          // Berechnung Mittelwert der 4 Zellspannungen

    float Standardabweichung = sqrt((pow(VCell[0]-Mittelwert,2)+pow(VCell[1]-Mittelwert,2) // Berechnung Standardabweichung der 4 Zellspannungen
    +pow(VCell[2]-Mittelwert,2)+pow(VCell[3]-Mittelwert,2))/3);

    // Wenn 4 aufeinanderfolgende Messungen den definierten Wertebereich über- oder unterschreiten wird Cell-Balancing aktiviert
    bool balActive1 = (abs(VCell[0] - Mittelwert) > Standardabweichung || abs(VCell[0] - Mittelwert) < Standardabweichung); 
    bool balActive2 = (abs(VCell[1] - Mittelwert) > Standardabweichung || abs(VCell[1] - Mittelwert) < Standardabweichung);
    bool balActive3 = (abs(VCell[2] - Mittelwert) > Standardabweichung || abs(VCell[2] - Mittelwert) < Standardabweichung);
    bool balActive4 = (abs(VCell[3] - Mittelwert) > Standardabweichung || abs(VCell[3] - Mittelwert) < Standardabweichung);

    setbalActive = balActive1 && balActive2 && balActive3 && balActive4;
  }

  if (currentMillis - previousCellBalMillis >= cellbalInterval) {  // Wird nur ausgeführt, wenn seit letzter Messung 400 ms vergangen sind
    
    previousCellBalMillis = currentMillis;        // Setzen des Zeitstempels der neuen Messung

    if (setbalActive == true){
      balActive = true;
    }
  }
}