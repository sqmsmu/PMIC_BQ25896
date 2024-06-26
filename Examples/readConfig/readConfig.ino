#include "PMIC_BQ25896.h"

PMIC_BQ25896 bq25896;

void setup(){
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  Serial.begin(115200);
  delay(2000);
  Serial.println("BQ25896 Power Management and Battery Charger Example");
  bq25896.begin();
  delay(500);
  if(!bq25896.isConnected()){
      Serial.println("BQ25896 not found! Check connection and power");
      while(1);
  }else{
      Serial.println("BQ25896 found successfully.");
  }
  Serial.println("Resetting BQ25896");
  bq25896.reset(); //reset all registers to default
  delay(1000);
  bq25896.setEN_ILIM(false); //disable hardware ilim pin
  //bq25896.setCONV_RATE(true); //set continous adc 1s read
  bq25896.setCONV_START(true); //read adc in one shot (default)
  bq25896.setWATCHDOG(0); //disable watchdog
  //bq25896.setBATFET_DIS(true); //Shipping Mode
  bq25896.setICHG(2000); //set charging current to 2000mA

}

void loop(){
    Serial.println("BQ25896 System Parameters");
    Serial.print("ILIM PIN : "); Serial.println(String(bq25896.getILIM_reg().en_ilim));
    Serial.print("IINLIM : "); Serial.println(String(bq25896.getIINLIM()) + "mA");
    Serial.print("VINDPM_OS : "); Serial.println(String(bq25896.getVINDPM_OS()) + "mV");
    Serial.print("SYS_MIN : "); Serial.println(String(bq25896.getSYS_MIN()) + "mV");
    Serial.print("ICHG : "); Serial.println(String(bq25896.getICHG()) + "mA");
    Serial.print("IPRE : "); Serial.println(String(bq25896.getIPRECHG()) + "mA");
    Serial.print("ITERM : "); Serial.println(String(bq25896.getITERM()) + "mA");
    Serial.print("VREG : "); Serial.println(String(bq25896.getVREG()) + "mV");
    Serial.print("BAT_COMP : "); Serial.println(String(bq25896.getBAT_COMP()) + "mΩ");
    Serial.print("VCLAMP : "); Serial.println(String(bq25896.getVCLAMP()) + "mV");
    Serial.print("BOOSTV : "); Serial.println(String(bq25896.getBOOSTV()) + "mV");
    Serial.print("BOOST_LIM : "); Serial.println(String(bq25896.getBOOST_LIM()) + "mA");
    Serial.print("VINDPM : "); Serial.println(String(bq25896.getVINDPM()) + "mV");
    Serial.print("BATV : "); Serial.println(String(bq25896.getBATV()) + "mV");
    Serial.print("SYSV : "); Serial.println(String(bq25896.getSYSV()) + "mV");
    Serial.print("TSPCT : "); Serial.println(String(bq25896.getTSPCT()) + "%");
    Serial.print("VBUSV : "); Serial.println(String(bq25896.getVBUSV()) + "mV");
    Serial.print("ICHGR : "); Serial.println(String(bq25896.getICHGR()) + "mA");
    
    Serial.print("Fault -> "); 
    Serial.print("NTC:" + String(bq25896.getFAULT_reg().ntc_fault));
    Serial.print(" ,BAT:" + String(bq25896.getFAULT_reg().bat_fault));
    Serial.print(" ,CHGR:" + String(bq25896.getFAULT_reg().chrg_fault));
    Serial.print(" ,BOOST:" + String(bq25896.getFAULT_reg().boost_fault));
    Serial.println(" ,WATCHDOG:" + String(bq25896.getFAULT_reg().watchdog_fault));

    Serial.print("Charging Status -> "); 
    Serial.print("CHG_EN:" + String(bq25896.getSYS_CTRL_reg().chg_config));
    Serial.print(" ,BATFET DIS:" + String(bq25896.getCTRL1_reg().batfet_dis));
    Serial.print(" ,BATLOAD_EN:" + String(bq25896.getSYS_CTRL_reg().bat_loaden));
    Serial.print(" ,PG STAT:" + String(bq25896.get_VBUS_STAT_reg().pg_stat));
    Serial.print(" ,VBUS STAT:" + String(bq25896.get_VBUS_STAT_reg().vbus_stat));
    Serial.print(" ,CHRG STAT:" + String(bq25896.get_VBUS_STAT_reg().chrg_stat));
    Serial.println(",VSYS STAT:" + String(bq25896.get_VBUS_STAT_reg().vsys_stat));

    bq25896.setCONV_START(true);
    delay(1000);
}