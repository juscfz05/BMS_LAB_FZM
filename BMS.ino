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
  unsigned long Spg_start;
  unsigned long Spg_end;

  unsigned long tmp_start;
  unsigned long tmp_end;

  unsigned long Safe_start;
  unsigned long Safe_end;

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

  // get cell tempratures
  TCell[0] = getCellTemp(1);
  TCell[1] = getCellTemp(2);
  TCell[2] = getCellTemp(3);
  TCell[3] = getCellTemp(4);

  // get batterypack current
  Ibat = getPackCurrent();

  checkcellVoltage(VCell);  // call function checkcellVoltage
  checkTemperature(TCell);  // call function checkTemperature

  if(WarningOvervoltage == true || WarningUndervoltage == true ||WarningOvertemp == true){
    Serial.print("Warning over: ");
    Serial.println(WarningOvervoltage);
    Serial.print("Warning under: ");
    Serial.println(WarningUndervoltage);
    Serial.print("Warning temp: ");
    Serial.println(WarningOvertemp);
  }
  SafetyController(WarningOvervoltage, WarningUndervoltage, WarningOvertemp, Ibat); // call function SafetyController
  Serial.print("BDU: ");
  Serial.println(BDUtrigger);

  if (BDUtrigger == true){
    setBDU_Activation(false); // disconect BDU
  }
  else if(BDUtrigger == false){
    setBDU_Activation(true);  // reconnect BDU
  }

  checkcellbalancing(VCell);  // call function checkcellbalancing
  if (setbalActive != 0){
      setBalancing(setbalActive);   // activate cellbalancing
  }

  showMeasurementValues(VCell, TCell, Ibat, WarningOvertemp, WarningOvervoltage, WarningUndervoltage);   // Stellt Messwerte numerisch dar
  drawMeasurementCurves(10, VCell, TCell, Ibat); // Messkurven - Parameter defines Minutes for full scale of X-Axis
}
