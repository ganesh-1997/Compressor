#include <stdlib.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define ADDR 0x3F
#define Rs 0
#define Rw 1
#define En 2
#define BACKLIGHT 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
LiquidCrystal_I2C lcd(ADDR, En, Rw, Rs, D4, D5, D6, D7);

#define SSID "GANESH_Network"
#define PASS "leeladeen"
#define IP "184.106.153.149"

String GET = "GET /update?key=P6NUL7CSV20DGH2D";
String FIELD1 = "&field1=";
String FIELD2 = "&field2=";
String FIELD3 = "&field3=";
String FIELD4 = "&field4=";
String FIELD5 = "&field5=";

#define BAUD_RATE 115200
#define DELAY_TIME 1000

#define PHIGH 2
#define PLOW 0.3

#define THIGH 40
#define TLOW 20

boolean pHigh, tHigh;

void setup() {
  Serial.begin(BAUD_RATE);

  Serial.println("AT");

  delay(5000);

  if (Serial.find("OK")) {
    if (connectWiFi() == false) {
      //Serial.println("Invalid Network Credentials");
    }
  } else {
    //Serial.println("Invalid Network Credentials");
  }

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(7, OUTPUT);

  pHigh = true;
  digitalWrite(7, 0);
  
  /* -- LCD Configurations --*/
  //lcd.clear();
  lcd.begin(16, 2);
  lcd.setBacklightPin(BACKLIGHT, POSITIVE);
  lcd.setBacklight(HIGH);
}

void loop() {
  /*-- Reading Voltage Inputs--*/
  float pVolts = (analogRead(A0) * 5.0 / 1023.0);
  float tVolts = (analogRead(A1) * 5.0 / 1023.0);
  /*-- Converting Voltage to Engineering Units--*/
  float pressure = (pVolts-1)*(10/4)-0.4;
  float temperature = (tVolts-1)*(100/4);
  if(pressure >= PHIGH) {
    pHigh = true;
  }
  if(temperature >= THIGH) {
    tHigh = true;
  }
  if(pHigh || tHigh) {
    digitalWrite(7,1);
  }
  if(pHigh && pressure < PLOW) {
    digitalWrite(7,0);
    pHigh = false;
  }
  /*--Updating Values to IP and LCD--*/
  updateDisplay(pressure, temperature);
  updateValues(String(pressure), String(temperature));
  delay(500);
}

boolean connectWiFi() {
    
  Serial.println("AT+CWMODE=1");
  delay(2000);

  String cmd = "AT+CWJAP=\"";
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";

  Serial.println(cmd);
  delay(5000);

  if (Serial.find("OK")) {
    return true;
  } else {
    return false;
  }
}

boolean updateValues(String pressure, String temperature) {
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";

  Serial.println(cmd);
  delay(1000);
  if (Serial.find("Error")) {
    return false;
  }

  cmd = GET;

  cmd += FIELD1;
  cmd += pressure;

  cmd += FIELD2;
  cmd += temperature;

  cmd += FIELD3;
  cmd += digitalRead(7);

  cmd += FIELD4;
  cmd += PHIGH;

  cmd += FIELD5;
  cmd += THIGH;

  cmd += "\r\n";

  Serial.print("AT+CIPSEND=");

  Serial.println(cmd.length());

  if (Serial.find(">")) {
    Serial.print(cmd);
  } else {
    Serial.println("AT+CIPCLOSE");
  }

  if (Serial.find("OK")) {
    return true;
  } else {
    return false;
  }
}

void updateDisplay(float pressure, float temperature) {
  lcd.home();
  lcd.setCursor (0, 0);
  /*-- State of Compressor --*/
  if (digitalRead(7) == LOW) {
    lcd.print(" Compressor ON  ");
  } else {
    lcd.print(" Compressor OFF ");
  }
  lcd.setCursor (0, 1);
  /*-- Display the Parameters --*/
  lcd.print(pressure, 2);
  lcd.print("Bar, ");
  lcd.print(temperature, 2);
  lcd.print("C  ");
}
