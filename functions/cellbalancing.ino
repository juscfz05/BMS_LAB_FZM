void checkcellbalancing()
{
  static unsigned long previousCellBalMillis = 0; // Variable für Zeitstempel des letzten Cell-Balancing
  static bool balActive = false;                  // Variable fürs Aktivieren des Cell-Balancing
  const long cellbalInterval = 200;               // konstantes Zeitintervall in ms in dem das Cell-Balancing durchgeführt werden soll (alle 200ms)
  unsigned long currentMillis = millis();         // aktuelle Zeit in ms

  if (currentMillis - previousCellBalMillis >= cellbalInterval) {   // Wird nur ausgeführt, wenn seit letzter Messung min. 200 ms vergangen sind

    previousCellBalMillis = currentMillis;      // Setzen des Zeitstempels der neuen Messung

    if (balActive == true){
      setBalancing(1);
      setBalancing(2);
      setBalancing(3);
      setBalancing(4);
    }


    float VCell1 = getCellVoltage(1); // Zellspannung aus Messung Nr. 1
    float VCell2 = getCellVoltage(2); // Zellspannung aus Messung Nr. 2
    float VCell3 = getCellVoltage(3); // Zellspannung aus Messung Nr. 3
    float VCell4 = getCellVoltage(4); // Zellspannung aus Messung Nr. 4

    float Mittelwert = (VCell1 + VCell2 + VCell3 + VCell4)/4;                          // Berechnung Mittelwert der 4 Zellspannungen

    float Standardabweichung = sqrt((pow(VCell1-Mittelwert,2)+pow(VCell2-Mittelwert,2) // Berechnung Standardabweichung der 4 Zellspannungen
    +pow(VCell3-Mittelwert,2)+pow(VCell4-Mittelwert,2))/3);

    // Wenn 4 aufeinanderfolgende Messungen den definierten Wertebereich über- oder unterschreiten wird Cell-Balancing aktiviert
    bool balActive1 = (abs(VCell1 - Mittelwert) > Standardabweichung || abs(VCell1 - Mittelwert) < Standardabweichung); 
    bool balActive2 = (abs(VCell2 - Mittelwert) > Standardabweichung || abs(VCell2 - Mittelwert) < Standardabweichung);
    bool balActive3 = (abs(VCell3 - Mittelwert) > Standardabweichung || abs(VCell3 - Mittelwert) < Standardabweichung);
    bool balActive4 = (abs(VCell4 - Mittelwert) > Standardabweichung || abs(VCell4 - Mittelwert) < Standardabweichung);

    balActive = balActive1 && balActive2 && balActive3 && balActive4;
  }
}