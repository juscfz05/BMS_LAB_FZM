bool WarningOvertemp = false;                     // Globale Definition der Übertemperaturwarnung als boolesche Variable
int counter = 0;
float Tcell[4] = {57, 54, 56, 58};
unsigned long currentMillisStart = 0; // Variable für Startzeitpunkt

void checkTemperature(float Tcell[]);

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 // Testwerte für die Zellen
  unsigned long currentMillis2 = millis();

  if (currentMillis2 < 2900) {
    Tcell[0] = 60;
    Tcell[1] = 60;
    Tcell[2] = 60;
    Tcell[3] = 60;
  } else {
    Tcell[0] = 61;
    Tcell[1] = 60;
    Tcell[2] = 60;
    Tcell[3] = 60;

    if (counter == 0) {
      currentMillisStart = millis();
      counter = 1;
    }
  }


  //Overtemp-Funktion
  checkTemperature(Tcell);

  if (WarningOvertemp == true > 0 && counter == 1) {
    unsigned long currentMillisStop = millis();
    unsigned long Diff = currentMillisStop - currentMillisStart;
    counter = 2;
    Serial.print("Time till not Overtemp: ");
    Serial.println(Diff);
  } else if (WarningOvertemp == false && counter == 2){
    Serial.println("Reset Cycle ");
    counter = 0;
    } 




} 





// Definition der Temperaturüberwachungsfunktion

void checkTemperature(float Tcell[4])             // Definition der Temperaturüberwachungsfunktion mit der Übergabe eines Arrays mit 4 Temperaturwerten 
{
  static unsigned long previousTempMillis = 0;    // Variable für Zeitstempel der letzten Temperaturmessung. Static, damit der Wert nur bei erster Initialisierung auf 0 gesetzt wird. 
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

