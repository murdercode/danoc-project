/**************************************************************************

 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Nicla_System.h"

#include "Arduino_BHY2.h"

Sensor temperature(SENSOR_ID_TEMP);
Sensor humidity(SENSOR_ID_HUM);
Sensor gas(SENSOR_ID_GAS);
Sensor barometer(SENSOR_ID_BARO);
Sensor stepCounter(SENSOR_ID_STC);

SensorXYZ accel(SENSOR_ID_ACC);

SensorBSEC bsec(SENSOR_ID_BSEC); // 4-in-1 sensor.

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL) / SDA 8 (A0) | SCL 7 (A1)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES 10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT 64
#define LOGO_WIDTH 64
// Made with https://pkolt.github.io/bitmap_editor/
static const unsigned char PROGMEM logo_bmp[] =
    {0x07, 0x83, 0xf0, 0x3f, 0x0f, 0x8c, 0x00, 0x30, 0x03, 0x83, 0xe0, 0x0f, 0x07, 0x8c, 0x00, 0x30,
     0x03, 0x83, 0xc0, 0x07, 0x07, 0x8c, 0x00, 0x30, 0x03, 0x03, 0x83, 0x07, 0x03, 0x8f, 0xc3, 0xf0,
     0x03, 0x03, 0x87, 0x87, 0x01, 0x8f, 0xc3, 0xf0, 0x11, 0x03, 0x87, 0xc3, 0x01, 0x8f, 0xc3, 0xf0,
     0x11, 0x23, 0x87, 0xc3, 0x00, 0x0f, 0xc3, 0xf0, 0x10, 0x23, 0x87, 0xc3, 0x08, 0x0f, 0xc3, 0xf0,
     0x18, 0x23, 0x87, 0x87, 0x08, 0x0f, 0xc3, 0xf0, 0x18, 0x23, 0x87, 0x87, 0x0c, 0x0f, 0xc3, 0xf0,
     0x18, 0x63, 0xc0, 0x07, 0x0e, 0x0f, 0xc3, 0xf0, 0x18, 0x63, 0xe0, 0x0f, 0x0e, 0x0f, 0xc3, 0xf0,
     0x1c, 0x63, 0xf0, 0x1f, 0x0f, 0x0f, 0xc3, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
     0xff, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xff,
     0xff, 0xff, 0xff, 0xc7, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x0f, 0xf9, 0xff, 0xff, 0xff,
     0xff, 0xff, 0xf0, 0x1f, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0xff, 0xfc, 0x3f, 0xff, 0xff,
     0xff, 0xff, 0x8f, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0x8f, 0xff, 0xff,
     0xff, 0xff, 0x9f, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xcf, 0xff, 0xff,
     0xff, 0xff, 0x9f, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xff, 0xff, 0x9f, 0x00, 0x00, 0x1f, 0xff, 0xff,
     0xff, 0xff, 0xcf, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xcf, 0x00, 0x00, 0x7f, 0xff, 0xff,
     0xff, 0xff, 0xc7, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xc7, 0x10, 0x84, 0x0f, 0xff, 0xff,
     0xff, 0xff, 0xe7, 0x31, 0x8e, 0x03, 0xff, 0xff, 0xff, 0xff, 0xe6, 0x31, 0x8e, 0x03, 0xff, 0xff,
     0xff, 0xff, 0xe0, 0x31, 0x8e, 0x61, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x31, 0x8e, 0x61, 0xff, 0xff,
     0xff, 0xff, 0xff, 0x31, 0x8e, 0x61, 0xff, 0xff, 0xff, 0xff, 0xff, 0x31, 0x8e, 0x61, 0xff, 0xff,
     0xff, 0xff, 0xff, 0x31, 0x8e, 0x61, 0xff, 0xff, 0xff, 0xff, 0xff, 0x31, 0x8e, 0x61, 0xff, 0xff,
     0xff, 0xff, 0xff, 0x31, 0x8e, 0x61, 0xff, 0xff, 0xff, 0xff, 0xff, 0x31, 0x8e, 0x71, 0xff, 0xff,
     0xff, 0xff, 0xff, 0x31, 0x8e, 0x61, 0xff, 0xff, 0xff, 0xff, 0xff, 0x11, 0x84, 0x03, 0xff, 0xff,
     0xff, 0xff, 0xff, 0x01, 0x80, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xff, 0xff,
     0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xff, 0xff,
     0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff,
     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
     0x07, 0x07, 0x00, 0x60, 0x03, 0x00, 0xfe, 0x07, 0x07, 0x07, 0x00, 0x60, 0x03, 0x00, 0x3c, 0x03,
     0x07, 0x07, 0x00, 0x60, 0x03, 0x00, 0x38, 0x01, 0x03, 0x07, 0x1f, 0xfc, 0x3f, 0x1e, 0x30, 0xe0,
     0x02, 0x07, 0x0f, 0xfe, 0x3f, 0x1c, 0x30, 0xf0, 0x12, 0x07, 0x00, 0x7e, 0x3f, 0x00, 0x30, 0xf0,
     0x10, 0x47, 0x00, 0x7e, 0x3f, 0x00, 0xf1, 0xf0, 0x10, 0x47, 0x1f, 0xfe, 0x3f, 0x10, 0x70, 0xf0,
     0x10, 0x47, 0x1f, 0xfe, 0x3f, 0x18, 0x70, 0xf1, 0x18, 0x47, 0x00, 0x7e, 0x3f, 0x1c, 0x38, 0x01,
     0x18, 0xc7, 0x00, 0x3e, 0x3f, 0x1c, 0x1c, 0x03, 0x18, 0xc7, 0x00, 0x3e, 0x3f, 0x1e, 0x1e, 0x07};

unsigned long previousMillis = 0; // variabile per salvare l'ultimo momento in cui la pagina è stata cambiata
int page = 1;                     // variabile per tenere traccia della pagina corrente

void setup()
{

   Serial.begin(115200);
   BHY2.begin();

   bsec.begin();

   temperature.begin();
   humidity.begin();
   gas.begin();
   barometer.begin();
   stepCounter.begin();

   accel.begin();

   nicla::begin();

   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
   if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
   {
      Serial.println(F("SSD1306 allocation failed"));
      for (;;)
         ; // Don't proceed, loop forever
   }

   // Show initial display buffer contents on the screen --
   // the library initializes this with an Adafruit splash screen.
   // display.display();
   // delay(2000); // Pause for 2 seconds

   // Clear the buffer
   display.clearDisplay();

   // Draw a single pixel in white
   display.drawPixel(10, 10, SSD1306_WHITE);

   // Show the display buffer on the screen. You MUST call display() after
   // drawing commands to make them visible on screen!
   display.display();

   testdrawbitmap();

   delay(2000);

   testscrolltext();

   // printSensorsValues();
   // delay(2000);
   // display.display() is NOT necessary after every single drawing command,
   // unless that's what you want...rather, you can batch up a bunch of
   // drawing operations and then update the screen all at once by calling
   // display.display(). These examples demonstrate both approaches...

   // testdrawline(); // Draw many lines

   // testdrawrect(); // Draw rectangles (outlines)

   // testfillrect(); // Draw rectangles (filled)

   // testdrawcircle(); // Draw circles (outlines)

   // testfillcircle(); // Draw circles (filled)

   // testdrawroundrect(); // Draw rounded rectangles (outlines)

   // testfillroundrect(); // Draw rounded rectangles (filled)

   // testdrawtriangle(); // Draw triangles (outlines)

   // testfilltriangle(); // Draw triangles (filled)

   // testdrawchar(); // Draw characters of the default font

   // testdrawstyles(); // Draw 'stylized' characters

   // testscrolltext(); // Draw scrolling text

   // testdrawbitmap(); // Draw a small bitmap image

   // // Invert and restore display, pausing in-between
   // display.invertDisplay(true);
   // delay(1000);
   // display.invertDisplay(false);
   // delay(1000);

   // testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
}

bool wasTapDetected = false;
float previousAccel = 0.0;
float tapThreshold = 300;

void loop()
{

   // Tap Detection
   float currentAccel = accel.x() + accel.y() + accel.z();

   // Se l'accelerazione è aumentata rapidamente rispetto alla lettura precedente,
   // allora è stato rilevato un tap
   if (currentAccel - previousAccel > tapThreshold)
   {
      Serial.println("Tap detected!");

      // Cambia la pagina quando viene rilevato un tap
      if (page == 1)
      {
         printPage2();
         page = 2;
      }
      else
      {
         printPage1();
         page = 1;
      }
   }

   previousAccel = currentAccel; // Aggiorna l'accelerazione precedente

   delay(300); // Piccolo ritardo per limitare l'output al serial monitor

   BHY2.update();
}

float calculateAltitude(float pressure, float seaLevelPressure = 1013.25)
{
   return 44330.0 * (1.0 - pow(pressure / seaLevelPressure, 0.1903));
}

// Print sensors values on the screen
void printPage1()
{
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(SSD1306_WHITE);
   display.setCursor(0, 0);

   // Stampa il nome del programma nelle prime due righe
   display.print("DANOC v0.2");
   // Simula indicatore batteria
   display.println(" - 1/2");

   display.println("-----------------");

   // Stampa i valori dei sensori in una tabella
   display.print("Temp: ");
   display.print(temperature.value());
   display.println("C");

   display.print("Umid: ");
   display.print(humidity.value());
   display.println("%");

   display.print("Gas: ");
   display.print(gas.value());
   display.println("ppm");

   float alt = calculateAltitude(barometer.value());
   display.print("Alt: ");
   display.print(alt);
   display.println("m");

   display.print("Baro: ");
   display.print(barometer.value());
   display.println("hPa");

   // Steps
   display.print("P:");
   display.print(stepCounter.value());

   // Birre
   display.print(" - Birre:");
   display.print(stepCounter.value() / 100);

   display.display();
}

void printPage2()
{
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(SSD1306_WHITE);
   display.setCursor(0, 0);

   // Stampa il nome del programma nelle prime due righe
   display.print("DANOC v0.2");
   // Simula indicatore batteria
   display.println(" - 2/2");

   display.println("-----------------");

   // Stampa i valori dei sensori in una tabella
   // display.print(bsec.toString());
   Serial.println(bsec.toString());

   display.print("Gas: ");
   display.println(gas.value());

   display.print("IAQ: ");
   display.println(bsec.iaq());

   display.print("CO2: ");
   display.println(bsec.co2_eq());

   display.print("VOC: ");
   display.println(bsec.b_voc_eq());

   display.display();
}

void testdrawline()
{
   int16_t i;

   display.clearDisplay(); // Clear display buffer

   for (i = 0; i < display.width(); i += 4)
   {
      display.drawLine(0, 0, i, display.height() - 1, SSD1306_WHITE);
      display.display(); // Update screen with each newly-drawn line
      delay(1);
   }
   for (i = 0; i < display.height(); i += 4)
   {
      display.drawLine(0, 0, display.width() - 1, i, SSD1306_WHITE);
      display.display();
      delay(1);
   }
   delay(250);

   display.clearDisplay();

   for (i = 0; i < display.width(); i += 4)
   {
      display.drawLine(0, display.height() - 1, i, 0, SSD1306_WHITE);
      display.display();
      delay(1);
   }
   for (i = display.height() - 1; i >= 0; i -= 4)
   {
      display.drawLine(0, display.height() - 1, display.width() - 1, i, SSD1306_WHITE);
      display.display();
      delay(1);
   }
   delay(250);

   display.clearDisplay();

   for (i = display.width() - 1; i >= 0; i -= 4)
   {
      display.drawLine(display.width() - 1, display.height() - 1, i, 0, SSD1306_WHITE);
      display.display();
      delay(1);
   }
   for (i = display.height() - 1; i >= 0; i -= 4)
   {
      display.drawLine(display.width() - 1, display.height() - 1, 0, i, SSD1306_WHITE);
      display.display();
      delay(1);
   }
   delay(250);

   display.clearDisplay();

   for (i = 0; i < display.height(); i += 4)
   {
      display.drawLine(display.width() - 1, 0, 0, i, SSD1306_WHITE);
      display.display();
      delay(1);
   }
   for (i = 0; i < display.width(); i += 4)
   {
      display.drawLine(display.width() - 1, 0, i, display.height() - 1, SSD1306_WHITE);
      display.display();
      delay(1);
   }

   delay(2000); // Pause for 2 seconds
}

void testdrawrect(void)
{
   display.clearDisplay();

   for (int16_t i = 0; i < display.height() / 2; i += 2)
   {
      display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, SSD1306_WHITE);
      display.display(); // Update screen with each newly-drawn rectangle
      delay(1);
   }

   delay(2000);
}

void testfillrect(void)
{
   display.clearDisplay();

   for (int16_t i = 0; i < display.height() / 2; i += 3)
   {
      // The INVERSE color is used so rectangles alternate white/black
      display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, SSD1306_INVERSE);
      display.display(); // Update screen with each newly-drawn rectangle
      delay(1);
   }

   delay(2000);
}

void testdrawcircle(void)
{
   display.clearDisplay();

   for (int16_t i = 0; i < max(display.width(), display.height()) / 2; i += 2)
   {
      display.drawCircle(display.width() / 2, display.height() / 2, i, SSD1306_WHITE);
      display.display();
      delay(1);
   }

   delay(2000);
}

void testfillcircle(void)
{
   display.clearDisplay();

   for (int16_t i = max(display.width(), display.height()) / 2; i > 0; i -= 3)
   {
      // The INVERSE color is used so circles alternate white/black
      display.fillCircle(display.width() / 2, display.height() / 2, i, SSD1306_INVERSE);
      display.display(); // Update screen with each newly-drawn circle
      delay(1);
   }

   delay(2000);
}

void testdrawroundrect(void)
{
   display.clearDisplay();

   for (int16_t i = 0; i < display.height() / 2 - 2; i += 2)
   {
      display.drawRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i,
                            display.height() / 4, SSD1306_WHITE);
      display.display();
      delay(1);
   }

   delay(2000);
}

void testfillroundrect(void)
{
   display.clearDisplay();

   for (int16_t i = 0; i < display.height() / 2 - 2; i += 2)
   {
      // The INVERSE color is used so round-rects alternate white/black
      display.fillRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i,
                            display.height() / 4, SSD1306_INVERSE);
      display.display();
      delay(1);
   }

   delay(2000);
}

void testdrawtriangle(void)
{
   display.clearDisplay();

   for (int16_t i = 0; i < max(display.width(), display.height()) / 2; i += 5)
   {
      display.drawTriangle(
          display.width() / 2, display.height() / 2 - i,
          display.width() / 2 - i, display.height() / 2 + i,
          display.width() / 2 + i, display.height() / 2 + i, SSD1306_WHITE);
      display.display();
      delay(1);
   }

   delay(2000);
}

void testfilltriangle(void)
{
   display.clearDisplay();

   for (int16_t i = max(display.width(), display.height()) / 2; i > 0; i -= 5)
   {
      // The INVERSE color is used so triangles alternate white/black
      display.fillTriangle(
          display.width() / 2, display.height() / 2 - i,
          display.width() / 2 - i, display.height() / 2 + i,
          display.width() / 2 + i, display.height() / 2 + i, SSD1306_INVERSE);
      display.display();
      delay(1);
   }

   delay(2000);
}

void testdrawchar(void)
{
   display.clearDisplay();

   display.setTextSize(1);              // Normal 1:1 pixel scale
   display.setTextColor(SSD1306_WHITE); // Draw white text
   display.setCursor(0, 0);             // Start at top-left corner
   display.cp437(true);                 // Use full 256 char 'Code Page 437' font

   // Not all the characters will fit on the display. This is normal.
   // Library will draw what it can and the rest will be clipped.
   for (int16_t i = 0; i < 256; i++)
   {
      if (i == '\n')
         display.write(' ');
      else
         display.write(i);
   }

   display.display();
   delay(2000);
}

void testdrawstyles(void)
{
   display.clearDisplay();

   display.setTextSize(1);              // Normal 1:1 pixel scale
   display.setTextColor(SSD1306_WHITE); // Draw white text
   display.setCursor(0, 0);             // Start at top-left corner
   display.println(F("Hello, world!"));

   display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
   display.println(3.141592);

   display.setTextSize(2); // Draw 2X-scale text
   display.setTextColor(SSD1306_WHITE);
   display.print(F("0x"));
   display.println(0xDEADBEEF, HEX);

   display.display();
   delay(2000);
}

void testscrolltext(void)
{
   display.clearDisplay();

   display.setTextSize(2); // Draw 2X-scale text
   display.setTextColor(SSD1306_WHITE);
   display.setCursor(10, 0);
   display.println(F("DANOC sta armando per voi..."));
   display.display(); // Show initial text
   delay(100);

   // Scroll in various directions, pausing in-between:
   display.startscrollright(0x00, 0x0F);
   delay(2000);
   display.stopscroll();
   delay(1000);
   display.startscrollleft(0x00, 0x0F);
   delay(2000);
   display.stopscroll();
   delay(1000);
   display.startscrolldiagright(0x00, 0x07);
   delay(2000);
   display.startscrolldiagleft(0x00, 0x07);
   delay(2000);
   display.stopscroll();
   delay(1000);
}

void testdrawbitmap(void)
{
   display.clearDisplay();

   display.drawBitmap(
       (display.width() - LOGO_WIDTH) / 2,
       (display.height() - LOGO_HEIGHT) / 2,
       logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
   display.display();
   delay(1000);
}

#define XPOS 0 // Indexes into the 'icons' array in function below
#define YPOS 1
#define DELTAY 2

void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h)
{
   int8_t f, icons[NUMFLAKES][3];

   // Initialize 'snowflake' positions
   for (f = 0; f < NUMFLAKES; f++)
   {
      icons[f][XPOS] = random(1 - LOGO_WIDTH, display.width());
      icons[f][YPOS] = -LOGO_HEIGHT;
      icons[f][DELTAY] = random(1, 6);
      Serial.print(F("x: "));
      Serial.print(icons[f][XPOS], DEC);
      Serial.print(F(" y: "));
      Serial.print(icons[f][YPOS], DEC);
      Serial.print(F(" dy: "));
      Serial.println(icons[f][DELTAY], DEC);
   }

   for (;;)
   {                          // Loop forever...
      display.clearDisplay(); // Clear the display buffer

      // Draw each snowflake:
      for (f = 0; f < NUMFLAKES; f++)
      {
         display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
      }

      display.display(); // Show the display buffer on the screen
      delay(200);        // Pause for 1/10 second

      // Then update coordinates of each flake...
      for (f = 0; f < NUMFLAKES; f++)
      {
         icons[f][YPOS] += icons[f][DELTAY];
         // If snowflake is off the bottom of the screen...
         if (icons[f][YPOS] >= display.height())
         {
            // Reinitialize to a random position, just off the top
            icons[f][XPOS] = random(1 - LOGO_WIDTH, display.width());
            icons[f][YPOS] = -LOGO_HEIGHT;
            icons[f][DELTAY] = random(1, 6);
         }
      }
   }
}
