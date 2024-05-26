void showMeasurementValues()
{
  static uint32_t lastCall = 0; // Variable für Zeit des letzten Funktionsaufrufs
  int i; // Variable für Nummer der Batteriezellen

  // do not call more often than once per second
  if (millis()-lastCall < 200) // Überprüfen ob letzter Aufruf früher als 200ms
    return; // Falls Aufruf früher als 200ms wird die Funktion beendet

  lastCall = millis(); // Abspeichern der Zeit des letzten Aufrufs

  // write Cell Voltages on screen
  for (i = 1; i <= 4; i++) // Schleife mit i 1-4 für die jeweiligen Zellen
  {
    writeText(i*68-52, 4, 2, "V"+String(i), ILI9341_WHITE); // Anzeige der Zellennummerierung für jeweilige Zelle
    writeText(i*68-66, 22, 2, String(getCellVoltage(i),2), colCell(i)); // Anzeige der Zellspannungen für jeweilige Zelle
    writeText(i*68-66, 40, 2, strLen(String(getCellTemp(i),1),4), colTemp(i)); // Anzeige der Zelltemperatur für jeweilige Zelle
  }
  
  // write pack current on screen
  writeText(280,  4, 2, "I/A",ILI9341_LIGHTGREY); // Anzeige Strom in Ampere
  writeText(270, 22, 2, strLen(String(getPackCurrent(),0),4),ILI9341_BLUE); // Anzeigen der derzeitigen Stromstärke

  // Timer
  writeText(282, 42, 2, "t/s",ILI9341_LIGHTGREY); // Anzeige Zeit in Sekunden
  writeText(258, 60, 2, strLen(String((lastCall/1000)-5),5),ILI9341_LIGHTGREY); // Anzeige der Sekunden seit Beginn der Programms

  // Warnungen
  if (regWarnings & 1) // Prüfen ob Überspannung vorliegt
    writeText(40, 58, 1, "OverVolt",ILI9341_RED); // Anzeige einer Warnung für Überspannung im Display
  else
    writeText(40, 58, 1, "OverVolt",ILI9341_BLACK); // Löschen der Warnung für Überspannung falls diese nicht vorhanden ist

  if (regWarnings & 2) // Prüfen ob Unterspannung vorliegt
    writeText(105, 58, 1, "UnderVolt",ILI9341_RED); // Anzeige einer Warnung für Unterspannung im Display
  else
    writeText(105, 58, 1, "UnderVolt",ILI9341_BLACK); // Löschen der Warnung für Unterspannung falls diese nicht vorhanden ist
  
  if (regWarnings & 4) // Prüfen ob Übertemperatur in einer Zelle vorliegt
    writeText(180, 58, 1, "OverTemp",ILI9341_RED); // Anzeige einer Warnung für Übertemperatur im Display
  else
    writeText(180, 58, 1, "OverTemp",ILI9341_BLACK); // Löschen der Warnung für Übertemperatur falls diese nicht vorhanden ist
}

void drawMeasurementCurves(uint16_t fullScreenDuration)
{
  static int tx  = 0; static float lastI=0; static uint32_t lastCall = 0;
  int i; float I;
  int mX = 319; //tS = 10;
  int yV = 183; float dV = 25.0; int mVo = yV-4.2*dV; // coordinates for Voltage-Axes
  int mVu = yV-2.5*dV; // lower voltage level
  int yT = 239; float dT = 0.70; int mT = yT- 60*dT; // coordinates for Temperature-Axes
  int yI = 159; float dI = 0.07; int sI = dI*400; // coordinates for Current-Axes

  int tS = (fullScreenDuration * 60.0) / mX; // scaling of the time axis
  
  if (tS < 1) tS = 1;
  
  // increase and reset + clear screen @ overflow
  tx++; if (tx/tS >= 319) {fillScreen(ILI9341_BLACK); tx = 0; }

  // draw Axes
  drawLine(0,mVu,294,mVu,ILI9341_DARKGREY); writeText(0, mVu-9, 1, "2.5V",colCell(3)); // draw upper Cell Voltage limit
  drawLine(0,mVo, mX,mVo,ILI9341_DARKGREY); writeText(0, mVo-9, 1, "4.2V",colCell(3)); // draw lower Cell Voltage limit

  drawLine( 0,yI,mX,yI,rgb565(0,0,150)); writeText(319-12, yI-9, 1, "0A",ILI9341_BLUE); // draw 0A Line
  drawLine( 0,yI-sI,mX,yI-sI,rgb565(0,0,100)); writeText(319-24, yI-9-sI, 1, "400A",ILI9341_BLUE); // draw Upper Current limit
  drawLine(20,yI+sI,mX,yI+sI,rgb565(0,0,100)); writeText(319-30, yI-9+sI, 1, "-400A",ILI9341_BLUE); // draw Lower Current limit

  drawLine(0,yT,mX,yT,ILI9341_DARKGREY); writeText(0, yT-9, 1,  "0C",colTemp(3)); // draw lower Temperature limit
  drawLine(0,mT,mX,mT,ILI9341_DARKGREY); writeText(0, mT-9, 1, "60C",colTemp(3)); // draw upper Temperature limit

  // draw Cell Voltages and Temperatures
  for (i = 1; i <= 4; i++)
  {
    if ((getCellVoltage(i) > 4.2) || (getCellVoltage(i) < 2.5)) // check if Cell Voltage limit is crossed
      drawPixel(tx/tS, yV-dV*max(min(getCellVoltage(i),4.5),2.2), ILI9341_RED); // draw critical Cell Voltages red
    else
      drawPixel(tx/tS, yV-dV*max(min(getCellVoltage(i),4.5),2.2), colCell(i)); // draw Cell Voltages within the limits

    if (getCellTemp(i) > 60) // check if Cell Temperature is crossed
      drawPixel(tx/tS, yT-dT*getCellTemp(i), ILI9341_RED); // draw critical Cell Temperatures red
    else
      drawPixel(tx/tS, yT-dT*getCellTemp(i), colTemp(i)); // draw Cell Temperatures within the limits
  }
  // draw Current
  //drawPixel(tx/tS, yI-dI*getPackCurrent(), ILI9341_BLUE);
  I = getPackCurrent();
  drawLine((tx-1)/tS,yI-dI*lastI,tx/tS,yI-dI*I,ILI9341_BLUE);
  lastI = I;

  // balancing pwm is active
  //if (pwmCell[0]+pwmCell[1]+pwmCell[2]+pwmCell[3] > -4) drawPixel(tx/tS, 238, ILI9341_YELLOW); // 158
  
  // wait for "dur" ms so that time for full screen matches fullScreenDuration in mins
  float dur = 60.0*fullScreenDuration/320.0/tS*1000; // duration in ms per call
  while (millis()-lastCall < dur)
    delay(1);
  lastCall = millis();
}

void showOCVcurve()
{
  float x, U;
  int yV = 183; float dV = 25.0; int mVo = yV-4.2*dV; // coordinates for Voltage-Axes

  for (x = 0; x < 1.3; x += 0.0015)
  {
    U = scaleVcell(x);
    if (U <= 4.2)
      drawPixel(round(x*100)+189,yV-dV*U, 0xFF00);
    else
      drawPixel(round(x*100)+189,yV-dV*U, ILI9341_RED);
  }
}
