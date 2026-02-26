
// Project 31 - Temperature History Monitor

//define the pins that the LEDs are connected to:
//#include <TFT.h>            // Arduino TFT LCD library
#include <SPI.h>            // SPI bus library
#include <Adafruit_GFX.h>   // core graphics library
#include <Adafruit_ST7789.h> // hardware-specific library for ST7789

#if defined (ARDUINO_FEATHER_ESP32)   // feather huzzah32
  #define TFT_CS    14
  #define TFT_RST   15
  #define TFT_DC    32

#elif defined(ESP8266)
  #define TFT_CS    4
  #define TFT_RST   16
  #define TFT_DC    5

#else
    #define TFT_CS    10
    #define TFT_RST   9   // or set to -1 and connect to Arduino reset pin
    #define TFT_DC    8
#endif

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

float p = 3.1415926;




void setup(void) 
{
  Serial.begin(9600);
  tft.init(240, 320);                 // activate LCD
  tft.fillScreen(ST77XX_BLACK);    // set display to black

  uint16_t time = millis();
  time = millis() - time;

  testlines(ST77XX_YELLOW);
  delay(500);

  Serial.println(time, DEC);
  delay(500);

  tft.drawPixel(tft.width()/2, tft.height()/2, ST77XX_GREEN);
  tft.fillScreen(ST77XX_BLACK);    // clear screen to black
  delay(500);
}





int tcurrent = 0;
int tempArray[120];

char currentString[3];



void getTemp()        // function to get read temperature from TMP36
{
float sum = 0;
float voltage = 0;
float celsius;
float sensor = 0;

// read the temperature sensor and convert the result to degrees C
sensor = analogRead(5);
voltage = (sensor * 5000) / 1024;
//voltage = voltage - 500;
celsius = voltage / 10;

tcurrent = int(celsius);

Serial.println(celsius);

// insert the new temperature at the start of the array of past temperatures
for (int a = 119; a>= 0; --a)
{
  tempArray [a] = tempArray [a - 1];
}
  tempArray[0] = tcurrent;
}



void drawScreen() // generate TFT LCD display effects
{
  int q;
  // display current temperature
  

  tft.setCursor(0, 10);
  tft.setTextSize(2);
  tft.print("Current: ");
  for (int16_t x=tft.width()-1; x > 6; x-=6) 
  {
    tft.fillRect(105, 0 , x/5, x/10, ST77XX_BLACK);
  }
  String tempString = String(tcurrent);

  tempString.toCharArray(currentString, 3);
  tft.setCursor(115, 0);
  tft.print(currentString);

// draw scale for graph
 tft.setCursor(0, 30);
 tft.setTextColor(ST77XX_WHITE);
 tft.setTextSize(2);

tft.println("50");
tft.println("45");

tft.println("40");
tft.println("35");

tft.println("30");
tft.println("25");

tft.println("20");
tft.println("15");

tft.println("10");
tft.println("5");

tft.println("0");


// plot temperature data points
for (int a = 25 ; a < 145 ; a++)
{
  // convert the temperature value to a suitable y axis position on the LCD
  q = (200 - (tempArray [a - 25] * 3.325));
  tft.drawPixel(a, q, ST77XX_GREEN);
  Serial.print(q);
  Serial.println(a);
  delay(500);
}
}



/*void setup(void) 
{
  Serial.begin(9600);
  uint16_t time = millis();
  //TFTscreen.begin();                 // activate LCD
  tft.fillScreen(ST77XX_BLACK);    // set display to black
  
  time = millis() - time;
  tft.init(240, 320);

  testlines(ST77XX_YELLOW);
  delay(500);

  Serial.println(time, DEC);
  delay(500);

  tft.drawPixel(tft.width()/2, tft.height()/2, ST77XX_GREEN);
  delay(500);
}*/




void loop() 
{
  
  for (int a = 0 ; a < 20 ; a++)    // wait 20 minutes until the next reading
  {
    getTemp();
    drawScreen();
    delay(1000);                   // wait 1 minute
  }
}



void testlines(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) 
  {
    tft.drawLine(0, 0, x, tft.height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, 0, tft.width()-1, y, color);
    delay(0);
  }}

