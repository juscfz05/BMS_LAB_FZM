void SafetyController() 
{
    static unsigned long previousMillis_Ibat = 0;                           // Erste Zuweisung Variable zum speichern des (letzten) Zeitstempel (für Einlesen Ibat)
    static unsigned long previousMillis_SC = 0;                             // Erste Zuweisung Variable zum speichern des (letzten) Zeitstempel (für Abfragen Warnungen SafetyController)
    unsigned long currentMillis = millis();                                 // Aktuelle Zeit abrufen
    const long Interval_SC = 50;                                            // Abtastrate 50 ms für Safety Controller
    const long Interval_Ibat = 200;                                         // Abtastrate 200 ms für kritische Stromwarnung

    if (currentMillis - previousMillis_Ibat >= Interval_Ibat) {             // nach Zeitintervall 200 ms wird Batteriestrom eingelesen (BMS 16)
      previousMillis_Ibat = currentMillis; 
      float Ibat = getPackCurrent();                                        // Batteriestrom einlesen, als Ibat speichern
    }

  static int WarnOvervolt_count = 0;                                        // Definition der Zähler für permanente Warnungen und der Zähler wenn Warnung nicht vorliegt
  static int NoWarnOvervolt_count = 0;
  static int WarnUndervolt_count = 0;
  static int NoWarnUndervolt_count = 0;
  static int WarnOvertemp_count = 0;
  static int NoWarnOvertemp_count = 0;
  static int WarnIbat_count = 0;
  static int NoWarnIbat_count = 0;

  if (currentMillis - previousMillis_SC >= Interval_SC){                    // nach Zeitintervall 50 ms werden Warnungen abgefragt
    previousMillis_SC = currentMillis;                                      // Zeitstempel neu setzen
    
    if (bitRead(regWarnings, 0) == true) {                                  // Zählen wenn Byte regWarnings Bit 0 = true (Überspannung erkannt), sonst Zähler für keine Warning hochzählen
      WarnOvervolt_count++;
    } else {
      NoWarnOvervolt_count++;
    }

    if (bitRead(regWarnings, 1) == true) {                                  // Zählen wenn Byte regWarnings Bit 1 = true (Unterspannung erkannt), sonst Zähler für keine Warning hochzählen
      WarnUndervolt_count++;
    } else {
      NoWarnUndervolt_count++;
    }

    if (bitRead(regWarnings, 2) == true) {                                  // Zählen wenn Byte regWarnings Bit 2 = true (Übertemperatur erkannt), sonst Zähler für keine Warning hochzählen
      WarnOvertemp_count++;
    } else {
      NoWarnOvertemp_count++;
    }
    
    if (Ibat > 400) {                                                       // Zählen wenn Ibat > 400 A, sonst Zähler für keine Warning hochzählen
      WarnIbat_count++;
    } else {
      NoWarnIbat_count++;
    }

    if (WarnOvervolt_count > 11 || WarnUndervolt_count > 11 || WarnOvertemp_count > 11 || WarnIbat_count > 11) { // Abfrage ob eine der Warnungen länger als 600 ms (50*12) angehalten hat (BMS 17 und 18)
      setBDU_Activation(true);    // Schütze öffnen wenn eine Warnung >= 600 ms anliegt
    } else (NoWarnOvervolt_count > 11 && NoWarnUndervolt_count >11 &&  NoWarnOvertemp_count > 11 && NoWarnIbat_count > 11); {
      setBDU_Activation(false);   // Schütze schließen wenn für >= 600 ms keine Warnung anliegt
    } 
  }
}