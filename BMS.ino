// Include necessary libraries
#include <SPI.h>              // SPI communication library
#include <Adafruit_ILI9341.h> // Display library
#include <TouchScreen.h>      // Display touch functionality

extern volatile uint32_t pwmDuration;
float VCell_meas[4];
float TCell_meas[4];
unsigned long time;
unsigned long prev_time = 0;

void setup() 
{
  setupBSW();
  //Serial.begin(9600);
}

void loop() 
{ 
  unsigned long start_loop = millis();
  // variables
  float Ibat;

  //setDriveMode(2);           // 1-Cycle Test 2-Slow Driver 3-Fast Driver 4-Power Mode
  receiveAndParseCommands();   // Empfängt Befehle über den Serial Monitor und führt diese aus

  // get cell voltages
  VCell_meas[0] = getCellVoltage(1);
  VCell_meas[1] = getCellVoltage(2);
  VCell_meas[2] = getCellVoltage(3);
  VCell_meas[3] = getCellVoltage(4);
  checkcellVoltage(VCell_meas);  // call function checkcellVoltage

  time = millis();
  // get cell tempratures
  if (time - prev_time >= 3000){
    prev_time = time;
    TCell_meas[0] = getCellTemp(1);
    TCell_meas[1] = getCellTemp(2);
    TCell_meas[2] = getCellTemp(3);
    TCell_meas[3] = getCellTemp(4);
  }
  checkTemperature(TCell_meas);  // call function checkTemperature
  
  // get batterypack current
  Ibat = getPackCurrent();
  SafetyController(WarningOvervoltage, WarningUndervoltage, WarningOvertemp, Ibat); // call function SafetyController

  //Serial.print("BDU trigger: "); // Print BDU trigger
  //Serial.println(BDUtrigger);

  //if(BDUtrigger == true & (WarningOvervoltage == false || WarningUndervoltage == false ||WarningOvertemp == false)){ // print Warnings
    //start_warning = millis();      
    //Serial.print("Warning Overvoltage: ");
    //.println(WarningOvervoltage);
    //Serial.print("Warning Undervoltage: ");
    //Serial.println(WarningUndervoltage);
    //Serial.print("Warning Overtemp.: ");
    //Serial.println(WarningOvertemp);
  //}  //else if (WarningOvervoltage == false & WarningUndervoltage == false & WarningOvertemp == false){start_no_warning = millis();}
  

  if (BDUtrigger == true){    
    setBDU_Activation(false); // open BDU
  }
  else if(BDUtrigger == false){
    setBDU_Activation(true);  // close BDU
  }

  checkcellbalancing(VCell_meas);  // call function checkcellbalancing

  if (setbalActive != 0){
      setBalancing(setbalActive);   // activate cellbalancing
  }

  showMeasurementValues(VCell_meas, TCell_meas, Ibat, WarningOvertemp, WarningOvervoltage, WarningUndervoltage);   // Stellt Messwerte numerisch dar
  drawMeasurementCurves(1, VCell_meas, TCell_meas, Ibat); // Messkurven - Parameter defines Minutes for full scale of X-Axis

  //Serial.print("Looptime: ");
  //Serial.println(millis()-start_loop);
  //Serial.print("\n\n");
}
