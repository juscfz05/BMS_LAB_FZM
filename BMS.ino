// Include necessary libraries
#include <SPI.h>              // SPI communication library
#include <Adafruit_ILI9341.h> // Display library
#include <TouchScreen.h>      // Display touch functionality

# include"F:\BMS\functions\cellbalancing.ino"
# include"F:\BMS\functions\cellvoltage.ino"
# include"F:\BMS\functions\overtemp.ino"
# include"F:\BMS\functions\SafetyController.ino"

extern volatile uint32_t pwmDuration;

void setup() 
{
  setupBSW();
  Serial.begin(9600);
}

void loop() 
{ 
  unsigned long start_loop = millis();
  unsigned long end_loop;
  //unsigned long start_warning;
  //unsigned long time_BDU_open;
  //unsigned long time_BDU_close;
  //unsigned long start_no_warning;

  // variables
  float VCell[4];
  float TCell[4];
  float Ibat;  

  //setDriveMode(2);           // 1-Cycle Test 2-Slow Driver 3-Fast Driver 4-Power Mode
  receiveAndParseCommands();   // Empfängt Befehle über den Serial Monitor und führt diese aus

  // get cell voltages
  VCell[0] = getCellVoltage(1);
  VCell[1] = getCellVoltage(2);
  VCell[2] = getCellVoltage(3);
  VCell[3] = getCellVoltage(4);
  checkcellVoltage(VCell);  // call function checkcellVoltage

  // get cell tempratures
  TCell[0] = getCellTemp(1);
  TCell[1] = getCellTemp(2);
  TCell[2] = getCellTemp(3);
  TCell[3] = getCellTemp(4);
  checkTemperature(TCell);  // call function checkTemperature

  //if(WarningOvervoltage == true || WarningUndervoltage == true ||WarningOvertemp == true){ // print Warnings
    //start_warning = millis();
    //Serial.print("Warning Overvoltage: ");
    //Serial.println(WarningOvervoltage);
    //Serial.print("Warning Undervoltage: ");
    //Serial.println(WarningUndervoltage);
    //Serial.print("Warning Overtemp.: ");
    //Serial.println(WarningOvertemp);
  //}  else if (WarningOvervoltage == false & WarningUndervoltage == false & WarningOvertemp == false){start_no_warning = millis();}
  
  
  // get batterypack current
  Ibat = getPackCurrent();

  SafetyController(WarningOvervoltage, WarningUndervoltage, WarningOvertemp, Ibat); // call function SafetyController

  //Serial.print("BDU: "); // Print BDU trigger
  //Serial.println(BDUtrigger);

  if (BDUtrigger == true){    
    setBDU_Activation(false); // disconect BDU
    //time_BDU_open = millis();
    //Serial.print("Zeit zwischen Warning und öffnen der Schütze: ");
    //Serial.println(time_BDU_open-start_warning);
  }
  else if(BDUtrigger == false){
    setBDU_Activation(true);  // reconnect BDU
    //time_BDU_close = millis();
    //Serial.print("Zeit zwischen Warning und schließen der Schützen: ");
    //Serial.println(time_BDU_close-start_no_warning);
  }

  checkcellbalancing(VCell);  // call function checkcellbalancing

  if (setbalActive != 0){
      setBalancing(setbalActive);   // activate cellbalancing
  }

  showMeasurementValues(VCell, TCell, Ibat, WarningOvertemp, WarningOvervoltage, WarningUndervoltage);   // Stellt Messwerte numerisch dar
  drawMeasurementCurves(1, VCell, TCell, Ibat); // Messkurven - Parameter defines Minutes for full scale of X-Axis
  end_loop = millis();

  Serial.print("Looptime: ");
  Serial.println(end_loop - start_loop);
  Serial.print("\n\n");
}
