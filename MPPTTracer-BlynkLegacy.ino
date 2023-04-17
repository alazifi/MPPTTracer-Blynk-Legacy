#ifdef ESP8266
#include <BlynkSimpleEsp8266.h>
#elif defined(ESP32)
#include <BlynkSimpleEsp32.h>
#else 
#error "Tidak ada modul WiFi"
#endif

#include "Rizz-OTABlynkMPPT-lib.h"
#include <ModbusMaster.h>
#include "pin-blynk.h"

credentials Credentials;
CaptiveRequestHandler RizzPortal;

//Variabel Blynk
char token_blynk[33];
bool terhubung_internet = 0;
const int Erasing_button = 0;

//Nama WiFi ESP Mode Thetering (AP)
char* esp_ssid = APSSID;
char* esp_pass = APPASS;

const int defaultBaudRate = 115200;
int timerTask1, timerTask2, timerTask3, timerTask4;

ModbusMaster node;
SimpleTimer timer;

void setup()

{
  RizzPortal.begin();
  delay(1000);
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  Serial.begin(defaultBaudRate);
  pinMode(Erasing_button, INPUT);

  // Modbus slave ID 1
  node.begin(1, Serial);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.println(t);
    delay(1000);
  }

  // Tekan dan tahanuntuk hapus kredensial
  if (digitalRead(Erasing_button) == LOW)
  {
    Credentials.Erase_eeprom();
  }

  String auth_string = Credentials.EEPROM_Config();
  auth_string.toCharArray(token_blynk, 33);

  if (Credentials.credentials_get())
  {
    Blynk.config(token_blynk);
    terhubung_internet = 1;
  }
  else
  {
    Credentials.setupAP(esp_ssid, esp_pass);
    terhubung_internet = 0;
  }

  if (terhubung_internet)
  {  
  timerTask1 = timer.setInterval(1000L, executeCurrentRegistryFunction);
  timerTask2 = timer.setInterval(1000L, nextRegistryNumber);
  timerTask3 = timer.setInterval(1000L, uploadToBlynk);
  }
}

  // upload values
  void uploadToBlynk() {
    Blynk.virtualWrite(vPIN_PV_POWER1,                   pvpower1);
    Blynk.virtualWrite(vPIN_PV_CURRENT1,                 pvcurrent1);
    Blynk.virtualWrite(vPIN_PV_VOLTAGE1,                 pvvoltage1);
    Blynk.virtualWrite(vPIN_LOAD_CURRENT1,               lcurrent1);
    Blynk.virtualWrite(vPIN_LOAD_POWER1,                 lpower1);
    Blynk.virtualWrite(vPIN_BATT_TEMP1,                  btemp1);
    Blynk.virtualWrite(vPIN_BATT_VOLTAGE1,               bvoltage1);
    Blynk.virtualWrite(vPIN_BATT_REMAIN1,                bremaining1);
    Blynk.virtualWrite(vPIN_CONTROLLER_TEMP1,            ctemp1);
    Blynk.virtualWrite(vPIN_BATTERY_CHARGE_CURRENT1,     battChargeCurrent1);
    Blynk.virtualWrite(vPIN_BATTERY_CHARGE_POWER1,       battChargePower1);
    Blynk.virtualWrite(vPIN_BATTERY_OVERALL_CURRENT1,    battOverallCurrent1);
    Blynk.virtualWrite(vPIN_LOAD_ENABLED1,               state);
    Blynk.virtualWrite(vPIN_DAILY_ENERGY_GNERATED1 ,     GEtoday1);
    Blynk.virtualWrite(vPIN_GENERATED_ENERGY_MONTH1,     GEmonth1);
    Blynk.virtualWrite(vPIN_TOTAL_ENERGY_GENERATED1,     GEtotal1);
//    Blynk.virtualWrite(V70, bat11);
//    Blynk.virtualWrite(V74, bat21);
  }
  
  void executeCurrentRegistryFunction() {
    Registries[currentRegistryNumber]();
  }
  
uint8_t setOutputLoadPower(uint8_t state)
{
  Serial1.print("Writing coil 0x0002 value to: ");
  Serial1.println(state);

  delay(10);
  // Set coil at address 0x0002 (Force the load on/off)
  node.writeSingleCoil(0x0001, 1);
  result = node.writeSingleCoil(0x0002, state);  

  if (result == node.ku8MBSuccess)
  {
    node.getResponseBuffer(0x00);
    Serial1.println("Success.");
  }
  return result;
}

// callback to on/off button state changes from the Blynk app
  BLYNK_WRITE(vPIN_LOAD_ENABLED1) {
    uint8_t newState = (uint8_t)param.asInt();

    if (debug == 1) {
      Serial.print("Setting load state output coil to value: ");
      Serial.println(newState);
    }

    result = setOutputLoadPower(newState);
    //readOutputLoadState();
    result = checkLoadCoilState();

        if (debug == 1) {
     if (result == node.ku8MBSuccess) {
        Serial.println("Write & Read suceeded.");
      } else {
        Serial.println("Write & Read failed.");
      }

      Serial.print("Output Load state value: ");
      Serial.println(state);
      Serial.println();
      Serial.println("Uploading results to Blynk.");
    }
    uploadToBlynk();
  }

// reads Load Enable Override coil
  uint8_t checkLoadCoilState() {
    if (debug == 1) {
      Serial.print("Reading coil 0x0006... ");
    }
    delay(10);
    result = node.readCoils(0x0002, 1);

        if (debug == 1) {
    Serial.print("Result: ");
    Serial.println(result);
        }
        
    if (result == node.ku8MBSuccess) {
      state = node.getResponseBuffer(0x00) > 0;

      if (debug == 1) {
        Serial.print(" Value: ");
        Serial.println(state);
      }
    } else {
      if (debug == 1) {
        Serial.println("Failed to read coil 0x0002!");
      }
    }
   
    return result;
 }


// -----------------------------------------------------------------

//Energi Harian
 void AddressRegistry_330C() {
  result = node.readInputRegisters(0x330C, 2);
 
  if (result == node.ku8MBSuccess) {
  GEtoday1 = ((node.getResponseBuffer(1) << 16) | node.getResponseBuffer(0)) / 100.0f;
//   CEtoday1 = ((((long)node.getResponseBuffer(0x05) << 16 | node.getResponseBuffer(0x00)) / 100.0f));
    } else {
    rs485DataReceived = false;
  }
}

//Energi Bulanan
 void AddressRegistry_330E() {
  result = node.readInputRegisters(0x330E, 2);
  if (result == node.ku8MBSuccess)
  {
    GEmonth1 = ((long)node.getResponseBuffer(0x07) << 16 | node.getResponseBuffer(0x00)) / 100.0f;
  } else {
    rs485DataReceived = false;
  }
}

//Energi Total 
 void AddressRegistry_3312() {
  result = node.readInputRegisters(0x3312, 2);
  if (result == node.ku8MBSuccess)
  {
    GEtotal1 = ((long)node.getResponseBuffer(0x07) << 16 | node.getResponseBuffer(0x00)) / 100.0f;
  } else {
    rs485DataReceived = false;
  }
}

 void AddressRegistry_3100() {
  result = node.readInputRegisters(0x3100, 6);
  if (result == node.ku8MBSuccess) {
      pvvoltage1 = node.getResponseBuffer(0x00) / 100.0f;
      pvcurrent1 = node.getResponseBuffer(0x01) / 100.0f;
      pvpower1 = (node.getResponseBuffer(0x02) | node.getResponseBuffer(0x03) << 16) / 100.0f;
      bvoltage1 = node.getResponseBuffer(0x04) / 100.0f;
      battChargeCurrent1 = node.getResponseBuffer(0x05) / 100.0f;

    }
  }

  void AddressRegistry_3106(){
    result = node.readInputRegisters(0x3106, 2);

    if (result == node.ku8MBSuccess) {
      battChargePower1 = (node.getResponseBuffer(0x00) | node.getResponseBuffer(0x01) << 16)  / 100.0f;
      if (debug == 1) {
        Serial.print("Battery Charge Power: ");
        Serial.println(battChargePower1);
      }
    }
  }

  void AddressRegistry_310D(){
    result = node.readInputRegisters(0x310D, 3);

      if (result == node.ku8MBSuccess) {
      lcurrent1 = node.getResponseBuffer(0x00) / 100.0f;
      lpower1 = (node.getResponseBuffer(0x01) | node.getResponseBuffer(0x02) << 16) / 100.0f;
    } else {
      rs485DataReceived = false;
    }    
  }
 

  void AddressRegistry_311A() {
    result = node.readInputRegisters(0x311A, 2);
   
    if (result == node.ku8MBSuccess) {    
      bremaining1 = node.getResponseBuffer(0x00) / 1.0f;
    } else {
      rs485DataReceived = false;
    }
  }

void AddressRegistry_331D() {
    result = node.readInputRegisters(0x331D, 2);
  
    if (result == node.ku8MBSuccess) {    
      btemp1 = node.getResponseBuffer(0x01) / 100.0f + 15;
  }
 }

void AddressRegistry_3111() {
    result = node.readInputRegisters(0x3111, 2);
  
    if (result == node.ku8MBSuccess) {    
      ctemp1 = node.getResponseBuffer(0x01) / 100.0f;
  }
 }

   void AddressRegistry_331B() {
    result = node.readInputRegisters(0x331B, 2);

    if (result == node.ku8MBSuccess) {
      battOverallCurrent1 = (node.getResponseBuffer(0x00) | node.getResponseBuffer(0x01) << 16) / 100.0f;
    }
  }


void loop()
{
  Credentials.server_loops();
  RizzPortal.run();
  if (terhubung_internet)
  {
    Blynk.run();
    timer.run();
  }
}
