void SafetyController() 
{
  static unsigned long previousMillis_Ibat = 0;                           // Erste Zuweisung Variable zum speichern des (letzten) Zeitstempel (für Einlesen Ibat)
  static unsigned long previousMillis_SC = 0;                             // Erste Zuweisung Variable zum speichern des (letzten) Zeitstempel (für Abfragen Warnungen SafetyController)
  unsigned long currentMillis = millis();                                 // Aktuelle Zeit abrufen
  const long Interval_SC = 50;                                            // Abtastrate 50 ms für Safety Controller
  const long Interval_Ibat = 200;                                         // Abtastrate 200 ms für kritische Stromwarnung

  if (currentMillis - previousMillis_Ibat >= Interval_Ibat) {             // nach Zeitintervall 200 ms wird Batteriestrom eingelesen (BMS 16)
    previousMillis_Ibat = currentMillis;                                  // Zeitstempel neu setzen
    float Ibat = getPackCurrent();                                        // Batteriestrom einlesen, als Ibat speichern
  }

  static int WarnOvervolt_count = 0;                                        // Definition der Zähler für permanente Warnungen und der Zähler wenn permanente Warnung nicht vorliegt
  static int NoWarnOvervolt_count = 0;
  static int WarnUndervolt_count = 0;
  static int NoWarnUndervolt_count = 0;
  static int WarnOvertemp_count = 0;
  static int NoWarnOvertemp_count = 0;
  static int WarnIbat_count = 0;
  static int NoWarnIbat_count = 0;

  if (currentMillis - previousMillis_SC >= Interval_SC){                          // nach Zeitintervall 50 ms werden Warnungen abgefragt
    previousMillis_SC = currentMillis;                                            // Zeitstempel neu setzen
    
    if (bitRead(regWarnings, 0) == true && WarnOvervolt_count < 12) {             // WarnOvervolt_count hochzählen wenn Byte regWarnings Bit 0 = true (Überspannung erkannt) und Zähler < 12
      WarnOvervolt_count++;
      NoWarnOvervolt_count = 0;
    } else if (bitRead(regWarnings, 0) == false && NoWarnOvervolt_count < 12){    // NoWarnOvervolt_count hochzählen wenn Byte regWarnings Bit 0 = false (keine Überspannung erkannt) und Zähler < 12
      NoWarnOvervolt_count++;
      WarnOvervolt_count = 0;
    }

    if (bitRead(regWarnings, 1) == true && WarnUndervolt_count < 12) {            // WarnUndervolt_count hochzählen wenn Byte regWarnings Bit 1 = true (Unterspannung erkannt) und Zähler < 12
      WarnUndervolt_count++;
      NoWarnUndervolt_count = 0;
    } else if (bitRead(regWarnings, 1) == false && NoWarnUndervolt_count < 12) {  // NoWarnUndervolt_count hochzählen wenn Byte regWarnings Bit 1 = false (keine Unterspannung erkannt) und Zähler < 12
      NoWarnUndervolt_count++;
      WarnUndervolt_count = 0;
    }

    if (bitRead(regWarnings, 2) == true && WarnOvertemp_count < 12) {             // WarnOvertemp_count hochzählen wenn Byte regWarnings Bit 2 = true (Übertemperatur erkannt) und Zähler < 12
      WarnOvertemp_count++;
      NoWarnOvertemp_count = 0;
    } else if (bitRead(regWarnings, 2) == false && NoWarnOvertemp_count < 12){    // NoWarnOvertemp_count hochzählen wenn Byte regWarnings Bit 2 = false (keine Übertemperatur erkannt) und Zähler < 12
      NoWarnOvertemp_count++;
      WarnOvertemp_count = 0;
    }
    
    if (Ibat > 400 && WarnIbat_count < 12) {                                      // WarnIbat_count hochzählen wenn Ibat > 400 A und Zähler < 12
      WarnIbat_count++;
      NoWarnIbat_count = 0;
    } else if (Ibat <= 400 && NoWarnIbat_count < 12) {                            // NoWarnIbat_count hochzählen wenn Ibat <= 400 A und Zähler < 12
      NoWarnIbat_count++;
      WarnIbat_count = 0;
    }

    if (WarnOvervolt_count > 11 || WarnUndervolt_count > 11 || WarnOvertemp_count > 11 || WarnIbat_count > 11 || (NoWarnOvervolt_count < 11 && NoWarnUndervolt_count < 11 &&  NoWarnOvertemp_count < 11 && NoWarnIbat_count < 11)) { // Abfrage ob eine der Warnungen länger als 600 ms (50*12) angehalten hat oder die NoWarnings noch nicht länger als 600 ms voriegen
      setBDU_Activation(true);    // Bit Schütze öffnen wenn eine Warnung >= 600 ms anliegt oder alle der NoWarnungen < 600 ms anliegen
    } else if ((NoWarnOvervolt_count > 11 && NoWarnUndervolt_count > 11 &&  NoWarnOvertemp_count > 11 && NoWarnIbat_count > 11) || WarnOvervolt_count < 11 || WarnUndervolt_count < 11 || WarnOvertemp_count < 11 || WarnIbat_count < 11); { // Abfrage ob keine der Warnungen länger als 600 ms (50*12) angehalten hat oder die Warnings noch nicht länger als 600 ms voriegen
      setBDU_Activation(false);   // Bit Schütze schließen wenn für >= 600 ms keine Warnung anliegt oder eine Warnung noch < 600 ms anliegt
    } 
  }
}