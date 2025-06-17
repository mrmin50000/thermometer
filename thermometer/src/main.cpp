#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 2 
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

int anodPins[] = {A1, A2, A3, A4}; 
int segmentsPins[] = {5, 6, 7, 8, 9, 10, 11, 12}; 



void setup() {
  for (int i = 0; i < 4; i++) pinMode(anodPins[i], OUTPUT);
  for (int i = 0; i < 8; i++) pinMode(segmentsPins[i], OUTPUT);
  Serial.begin(9600);
  dht.begin();
}

//{A, B, C, D, E, F, G,DP}
int seg[12][8] = {
  {1, 1, 1, 1, 1, 1, 0, 0}, 
  {0, 1, 1, 0, 0, 0, 0, 0}, 
  {1, 1, 0, 1, 1, 0, 1, 0}, 
  {1, 1, 1, 1, 0, 0, 1, 0}, 
  {0, 1, 1, 0, 0, 1, 1, 0}, 
  {1, 0, 1, 1, 0, 1, 1, 0}, 
  {1, 0, 1, 1, 1, 1, 1, 0}, 
  {1, 1, 1, 0, 0, 0, 0, 0}, 
  {1, 1, 1, 1, 1, 1, 1, 0}, 
  {1, 1, 1, 1, 0, 1, 1, 0}, 
  {0, 0, 0, 0, 0, 0, 1, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}  
};

int err[4][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {1, 0, 0, 1, 1, 1, 1, 0}, 
  {0, 0, 0, 0, 1, 0, 1, 0}, 
  {0, 0, 0, 0, 1, 0, 1, 0}  
};

static int arr[4][8]; 
float value = 0.0;

int counter = -1;

void getDigits (float value) {
Serial.println("");


if ((value >= 10000) ||
    (value <= -1000)) {

for (int i = 0; i < 4; i++) { 
    for (int k = 0; k < 8; k++) {
    arr[i][k] = err[i][k];
    }
}
return; 
}

int digits = 4; 
if (value < 0) digits = 3; 


int intPart = (int)abs(value);
int intLength = ((String)intPart).length(); 

char valueChars[8];
dtostrf(value, 5, 4, valueChars); 


String valueStr = valueChars;

int fracIndex = valueStr.indexOf(".");
String fracPartStr = valueStr.substring(fracIndex + 1, valueStr.length());
int fracDigits = digits - intLength;
fracPartStr = fracPartStr.substring(0, fracDigits);

int fracInt = fracPartStr.toInt();

String output = (value < 0) ? "-" : "";
output += (String)intPart;

String outputFrac = ((digits - intLength <= 0) || (fracInt == 0)) ? "" : ((String)"." + fracPartStr);

output += (String)outputFrac;

String spaces = "     ";
digits = 4;
if (~output.indexOf(".")) digits += 1;
if (output.length() < digits) output = spaces.substring(0, digits - output.length()) + output;


int dig = -1;
for (int i = 0; i < output.length(); i++) {
String _char = output.substring(i, i + 1);

if (_char != ".") dig += 1;

int actualdigit = 11;
if ((_char == "-")) {
    actualdigit = 10;
}
else if (_char == " " || _char == ".") {
}
else {
    actualdigit = _char.toInt();
}

if (_char == ".") {
    arr[dig][7] = 1;
}
else  {
    for (int n = 0; n <= 7; n++) {
    arr[dig][n] = seg[actualdigit][n];
    }
}
}
}

void displayMessage(int dig[4][8]) {
for (int i = 0; i < 4; i++) {
for (int k = 0; k < 8; k++) {
    digitalWrite(segmentsPins[k], ((dig[i][k] == 1) ? LOW : HIGH));
}
digitalWrite(anodPins[i], HIGH);
delay(1);
digitalWrite(anodPins[i], LOW);
}
}

int k = 0;

void loop() {
  counter += 1;
  if ((counter % 2000) == 0) {
    counter = 0;
    if(k == 0){
        value = dht.readTemperature();
        k = 1;
    }
    else{
        value = dht.readHumidity();
        k = 0;
    }
    getDigits(value);
  }
  displayMessage(arr);
}



