
bool WarningUndervoltage = false;
bool WarningOvervoltage = false;           
bool WarningOvertemp = false;
float Ibat = 350;

unsigned long StrtUVWarnOn = 0;
void SafetyController(bool WarningOvervoltage, bool WarningUndervoltage, bool WarningOvertemp, float Ibat);
bool BDUtrigger = false;

  
void setup() {
  Serial.begin(9600);

}

void loop() {
  if (millis() >= 3000 && WarningUndervoltage == false) {
    WarningUndervoltage = true;
    StrtUVWarnOn = millis();
    //Serial.println((String)"Start time of undervoltage warning activated test: "+StrtUVWarnOn);
  }

  SafetyController(WarningOvervoltage, WarningUndervoltage, WarningOvertemp, Ibat);

  if (BDUtrigger == true) {
    static unsigned long EndUVWarnOn = millis();
    //Serial.println((String)"End time of undervoltage warning activated test: "+EndUVWarnOn);
    unsigned long DiffUVWarnOn = EndUVWarnOn - StrtUVWarnOn; 
    Serial.println((String)"Difference time of undervoltage warning activated test: "+DiffUVWarnOn);
    delay(100000);
  }
}


void SafetyController(bool WarningOvervoltage, bool WarningUndervoltage, bool WarningOvertemp, float Ibat) 
{ 
  static unsigned long previousMillis_Ibat = 0;                           // Erste Zuweisung Variable zum speichern des (letzten) Zeitstempel
  unsigned long currentMillis = millis();                                 // Aktuelle Zeit abrufen
  const long Interval_SC = 50;                                            // Abtastrate 50 ms für Safety Controller
  const long Interval_Ibat = 200;                                         // Abtastrate 200 ms für kritische Stromwarnung
  float Ibat_200ms;                                                       // Ibat der alle 200ms eingelesen wird

  if (currentMillis - previousMillis_Ibat >= Interval_Ibat) {             // nach Zeitintervall 200 ms wird Batteriestrom eingelesen
    previousMillis_Ibat = currentMillis;                                  // Zeitstempel neu setzen
    Ibat_200ms = Ibat;                                                    // Batteriestrom alle 200ms einlesen, als Ibat_200ms  speichern
  }

  static int WarnOvervolt_count = 0;                                      // Definition der Zähler für permanente Warnungen und der Zähler wenn permanente Warnung nicht vorliegt
  static int NoWarnOvervolt_count = 0;
  static int WarnUndervolt_count = 0;
  static int NoWarnUndervolt_count = 0;
  static int WarnOvertemp_count = 0;
  static int NoWarnOvertemp_count = 0;
  static int WarnIbat_count = 0;
  static int NoWarnIbat_count = 0;

  static unsigned long previousMillis_SC = 0;                                // Erste Zuweisung Variable zum speichern des (letzten) Zeitstempel (für Abfragen Warnungen SafetyController)
  if (currentMillis - previousMillis_SC >= Interval_SC){                     // nach Zeitintervall 50 ms werden Warnungen abgefragt
    previousMillis_SC = currentMillis;                                       // Zeitstempel neu setzen

    if (WarningOvervoltage == true && WarnOvervolt_count < 13) {             // WarnOvervolt_count hochzählen wenn Byte regWarnings Bit 0 = true (Überspannung erkannt) und Zähler < 13
      WarnOvervolt_count++;
      NoWarnOvervolt_count = 0;
    } else if (WarningOvervoltage == false && NoWarnOvervolt_count < 14){    // NoWarnOvervolt_count hochzählen wenn Byte regWarnings Bit 0 = false (keine Überspannung erkannt) und Zähler < 13
      NoWarnOvervolt_count++;
      WarnOvervolt_count = 0;
    }

    if (WarningUndervoltage == true && WarnUndervolt_count < 13) {            // WarnUndervolt_count hochzählen wenn Byte regWarnings Bit 1 = true (Unterspannung erkannt) und Zähler < 13
      WarnUndervolt_count++;
      NoWarnUndervolt_count = 0;
    } else if (WarningUndervoltage == false && NoWarnUndervolt_count < 14) {  // NoWarnUndervolt_count hochzählen wenn Byte regWarnings Bit 1 = false (keine Unterspannung erkannt) und Zähler < 13
      NoWarnUndervolt_count++;
      WarnUndervolt_count = 0;
    }

    if (WarningOvertemp == true && WarnOvertemp_count < 13) {                 // WarnOvertemp_count hochzählen wenn Byte regWarnings Bit 2 = true (Übertemperatur erkannt) und Zähler < 13
      WarnOvertemp_count++;
      NoWarnOvertemp_count = 0;
    } else if (WarningOvertemp == false && NoWarnOvertemp_count < 14){        // NoWarnOvertemp_count hochzählen wenn Byte regWarnings Bit 2 = false (keine Übertemperatur erkannt) und Zähler < 13
      NoWarnOvertemp_count++;
      WarnOvertemp_count = 0;
    }
    
    if ((Ibat_200ms > 400 || Ibat_200ms < -400) && WarnIbat_count < 13) {                                  // WarnIbat_count hochzählen wenn Ibat > 400 A und Zähler < 13
      WarnIbat_count++;
      NoWarnIbat_count = 0;
    } else if ((Ibat_200ms <= 400 || Ibat_200ms >= -400) && NoWarnIbat_count < 14) {                        // NoWarnIbat_count hochzählen wenn Ibat <= 400 A und Zähler < 13
      NoWarnIbat_count++;
      WarnIbat_count = 0;
    }

    if (WarnOvervolt_count == 13 || WarnUndervolt_count == 13 || WarnOvertemp_count == 13 || WarnIbat_count == 13) { // Abfrage ob eine der Warnungen für 600 ms (50*12) vorliegt
      BDUtrigger = true;    // Bit Schütze öffnen wenn eine Warnung >= 600 ms anliegt
    } else if (BDUtrigger == true && (NoWarnOvervolt_count >= 13 && NoWarnUndervolt_count >= 13 &&  NoWarnOvertemp_count >= 13 && NoWarnIbat_count >= 13)) { // Abfrage ob keine der Warnungen länger als 600 ms (50*12) vorliegt und Schütze offen sind
      BDUtrigger = false;   // Bit Schütze schließen wenn für >= 600 ms keine Warnung anliegt
    } 
  }
}
