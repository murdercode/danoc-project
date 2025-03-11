/**************************************************************************
 * DANOC Project - Environmental Monitoring Device
 * Uses Nicla Sense ME with BHY2 sensors and OLED display
 **************************************************************************/

// ===== LIBRARIES =====
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Nicla_System.h"
#include "Arduino_BHY2.h"

// ===== DISPLAY CONFIGURATION =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ===== LOGO CONFIGURATION =====
#define LOGO_HEIGHT 64
#define LOGO_WIDTH 64
// Logo bitmap data
static const unsigned char PROGMEM logo_bmp[] = {
    0x07, 0x83, 0xf0, 0x3f, 0x0f, 0x8c, 0x00, 0x30, 0x03, 0x83, 0xe0, 0x0f, 0x07, 0x8c, 0x00, 0x30,
    0x03, 0x83, 0xc0, 0x07, 0x07, 0x8c, 0x00, 0x30, 0x03, 0x03, 0x83, 0x07, 0x03, 0x8f, 0xc3, 0xf0,
    0x03, 0x03, 0x87, 0x87, 0x01, 0x8f, 0xc3, 0xf0, 0x11, 0x03, 0x87, 0xc3, 0x01, 0x8f, 0xc3, 0xf0,
    0x11, 0x23, 0x87, 0xc3, 0x00, 0x0f, 0xc3, 0xf0, 0x10, 0x23, 0x87, 0xc3, 0x08, 0x0f, 0xc3, 0xf0,
    0x18, 0x23, 0x87, 0x87, 0x08, 0x0f, 0xc3, 0xf0, 0x18, 0x23, 0x87, 0x87, 0x0c, 0x0f, 0xc3, 0xf0,
    0x18, 0x63, 0xc0, 0x07, 0x0e, 0x0f, 0xc3, 0xf0, 0x18, 0x63, 0xe0, 0x0f, 0x0e, 0x0f, 0xc3, 0xf0,
    0x1c, 0x63, 0xf0, 0x1f, 0x0f, 0x0f, 0xc3, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xc7, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x0f, 0xf9, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf0, 0x1f, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0xff, 0xfc, 0x3f, 0xff,
    0xff, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xcf, 0xff,
    0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0x8f, 0xff,
    0xff, 0xff, 0xff, 0x9f, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xcf, 0x00, 0x00, 0x3f, 0xff,
    0xff, 0xff, 0xff, 0xcf, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xc7, 0x00, 0x00, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xc7, 0x10, 0x84, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xe7, 0x31, 0x8e, 0x03, 0xff,
    0xff, 0xff, 0xff, 0xe6, 0x31, 0x8e, 0x03, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x31, 0x8e, 0x61, 0xff,
    0xff, 0xff, 0xff, 0xf0, 0x31, 0x8e, 0x61, 0xff, 0xff, 0xff, 0xff, 0xff, 0x31, 0x8e, 0x61, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x31, 0x8e, 0x61, 0xff, 0xff, 0xff, 0xff, 0xff, 0x31, 0x8e, 0x61, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x31, 0x8e, 0x61, 0xff, 0xff, 0xff, 0xff, 0xff, 0x31, 0x8e, 0x61, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x31, 0x8e, 0x71, 0xff, 0xff, 0xff, 0xff, 0xff, 0x31, 0x8e, 0x61, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x11, 0x84, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x80, 0x07, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x07, 0x07, 0x00, 0x60, 0x03, 0x00, 0xfe, 0x07, 0x07, 0x07, 0x00, 0x60, 0x03, 0x00, 0x3c, 0x03,
    0x07, 0x07, 0x00, 0x60, 0x03, 0x00, 0x38, 0x01, 0x03, 0x07, 0x1f, 0xfc, 0x3f, 0x1e, 0x30, 0xe0,
    0x02, 0x07, 0x0f, 0xfe, 0x3f, 0x1c, 0x30, 0xf0, 0x12, 0x07, 0x00, 0x7e, 0x3f, 0x00, 0x30, 0xf0,
    0x10, 0x47, 0x00, 0x7e, 0x3f, 0x00, 0xf1, 0xf0, 0x10, 0x47, 0x1f, 0xfe, 0x3f, 0x10, 0x70, 0xf0,
    0x10, 0x47, 0x1f, 0xfe, 0x3f, 0x18, 0x70, 0xf1, 0x18, 0x47, 0x00, 0x7e, 0x3f, 0x1c, 0x38, 0x01,
    0x18, 0xc7, 0x00, 0x3e, 0x3f, 0x1c, 0x1c, 0x03, 0x18, 0xc7, 0x00, 0x3e, 0x3f, 0x1e, 0x1e, 0x07};

// ===== SENSOR DEFINITIONS =====
Sensor temperature(SENSOR_ID_TEMP);
Sensor humidity(SENSOR_ID_HUM);
Sensor gas(SENSOR_ID_GAS);
Sensor barometer(SENSOR_ID_BARO);
Sensor stepCounter(SENSOR_ID_STC);
SensorXYZ accel(SENSOR_ID_ACC);
SensorBSEC bsec(SENSOR_ID_BSEC);

// ===== GLOBAL VARIABLES =====
// Version info
#define APP_VERSION "0.7"

// Display state tracking
int currentPage = 1;
unsigned long lastDisplayRefreshTime = 0;
const unsigned long displayRefreshInterval = 2000; // 2 seconds refresh

// Power management
unsigned long lastActivityTime = 0;
const unsigned long idleTimeout = 30000; // 30 seconds idle timeout
bool isInIdleMode = false;

// Tap detection
float previousAcceleration = 0.0;
const float tapThreshold = 300.0;
unsigned long lastSensorResetTime = 0;
const unsigned long sensorResetInterval = 1800000; // 30 minutes in milliseconds

// ===== SOGLIE DI PERICOLO =====
// Definizioni delle soglie per i valori pericolosi dei gas
#define IAQ_DANGER_THRESHOLD 300  // IAQ > 300 è considerato dannoso
#define CO2_DANGER_THRESHOLD 2000 // CO2 > 2000 ppm può causare mal di testa e difficoltà di concentrazione
#define VOC_DANGER_THRESHOLD 5    // VOC > 5 ppm è considerato pericoloso
#define GAS_DANGER_THRESHOLD 1000 // Valore generico di soglia per gas
#define DANGER_SYMBOL "!"         // Simbolo per indicare pericolo sul display

// Flag per lo stato di pericolo
bool isDangerousCondition = false;

// Variabili per il lampeggio del LED e del testo
unsigned long lastLedToggleTime = 0;
const unsigned long ledBlinkInterval = 500; // 500ms per un lampeggio visibile
bool blinkState = true;                     // Stato condiviso tra LED e testo di avviso

/**
 * Setup function - initializes hardware and sensors
 */
void setup()
{
   // Initialize serial communication
   Serial.begin(115200);

   // Initialize sensors
   BHY2.begin();
   bsec.begin();
   temperature.begin();
   humidity.begin();
   gas.begin();
   barometer.begin();
   stepCounter.begin();
   accel.begin();

   // Initialize Nicla board
   nicla::begin();

   // Initialize display
   if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
   {
      Serial.println(F("SSD1306 display initialization failed"));
      for (;;)
         ; // Don't proceed if display fails
   }

   // Show logo/splash screen
   showSplashScreen();

   // Initialize activity tracking
   lastActivityTime = millis();

   // Test different LED control methods (try all possibilities)
   // Metodo 1: Standard Arduino LED control
   pinMode(LED_BUILTIN, OUTPUT);
   digitalWrite(LED_BUILTIN, HIGH); // Start with LED off

   // Metodo 2: Nicla RGB LED control
   nicla::begin();
   nicla::leds.begin();
}

/**
 * Main loop - handles sensor readings, tap detection, and display updates
 */
void loop()
{
   // Update sensors
   BHY2.update();

   // Get current time
   unsigned long currentTime = millis();

   // Reset sensors periodically to prevent drift
   if (currentTime - lastSensorResetTime >= sensorResetInterval)
   {
      resetSensors();
      lastSensorResetTime = currentTime;
   }

   // ----- BLINK EFFECT FOR LED AND WARNING TEXT -----
   if (isDangerousCondition && (currentTime - lastLedToggleTime >= ledBlinkInterval))
   {
      blinkState = !blinkState; // Toggle blink state for both LED and text
      lastLedToggleTime = currentTime;

      // Try multiple LED control approaches to ensure one works
      if (blinkState)
      {
         // Approccio 2: Nicla RGB LED API (rosso)
         nicla::leds.setColor(red);
      }
      else
      {
         // Approccio 2: Nicla RGB LED API (spento)
         nicla::leds.setColor(off);
      }

      // Update display immediately to show text blinking effect if in active mode
      if (!isInIdleMode)
      {
         updateCurrentPage();
      }
   }

   // ----- TAP DETECTION -----
   // Calculate acceleration magnitude - using absolute values makes detection more robust
   float currentAcceleration = abs(accel.x()) + abs(accel.y()) + abs(accel.z());

   // Detect tap based on absolute magnitude and change
   if (currentAcceleration > 300 && (currentAcceleration - previousAcceleration > tapThreshold))
   {
      Serial.println(F("Tap detected"));
      lastActivityTime = currentTime; // Reset activity timer

      if (isInIdleMode)
      {
         // Wake device from idle mode
         exitIdleMode();
      }
      else
      {
         // Change page
         togglePage();
      }

      // Reset display refresh timer after tap action
      lastDisplayRefreshTime = currentTime;
   }

   previousAcceleration = currentAcceleration * 0.6 + previousAcceleration * 0.4; // Apply smoothing

   // ----- DISPLAY UPDATES -----
   // Update display when not in idle mode and refresh interval has passed
   if (!isInIdleMode && (currentTime - lastDisplayRefreshTime >= displayRefreshInterval))
   {
      updateCurrentPage();
      lastDisplayRefreshTime = currentTime;
   }

   // ----- POWER MANAGEMENT -----
   // Enter idle mode after timeout period
   if (!isInIdleMode && (currentTime - lastActivityTime > idleTimeout))
   {
      enterIdleMode();
   }

   // Check for dangerous conditions
   checkDangerousMeasurements();

   delay(100); // Stabilize readings
}

/**
 * Toggles between page 1 and page 2
 */
void togglePage()
{
   if (currentPage == 1)
   {
      currentPage = 2;
   }
   else
   {
      currentPage = 1;
   }
   updateCurrentPage();
}

/**
 * Updates display with current page content
 */
void updateCurrentPage()
{
   if (currentPage == 1)
   {
      displayPage1();
   }
   else
   {
      displayPage2();
   }
}

/**
 * Calculates altitude from barometric pressure
 */
float calculateAltitude(float pressure, float seaLevelPressure = 1013.25)
{
   return 44330.0 * (1.0 - pow(pressure / seaLevelPressure, 0.1903));
}

/**
 * Displays environmental data (page 1)
 */
void displayPage1()
{
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(SSD1306_WHITE);
   display.setCursor(0, 0);

   // Header
   display.print("DANOC v");
   display.print(APP_VERSION);
   display.println(" - 1/2");
   display.println("-----------------");

   // Sensor data
   display.print("Temp: ");
   display.print(temperature.value());
   display.println("C");

   display.print("Umid: ");
   display.print(humidity.value());
   display.println("%");

   float altitude = calculateAltitude(barometer.value());
   display.print("Alt: ");
   display.print(altitude);
   display.println("m");

   display.print("Baro: ");
   display.print(barometer.value());
   display.println("hPa");

   display.print("Passi: ");
   display.println(stepCounter.value());

   display.display();
}

/**
 * Displays air quality data (page 2)
 */
void displayPage2()
{
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(SSD1306_WHITE);
   display.setCursor(0, 0);

   // Header
   display.print("DANOC v");
   display.print(APP_VERSION);
   display.println(" - 2/2");
   display.println("-----------------");

   // Air quality data
   Serial.println(bsec.toString()); // Debug to serial

   display.print("Gas: ");
   display.print(gas.value());
   if (isDangerous(gas.value(), GAS_DANGER_THRESHOLD))
   {
      display.print(" ");
      display.print(DANGER_SYMBOL);
   }
   display.println("ppm");

   display.print("IAQ: ");
   display.print(bsec.iaq());
   if (isDangerous(bsec.iaq(), IAQ_DANGER_THRESHOLD))
   {
      display.print(" ");
      display.print(DANGER_SYMBOL);
   }
   display.println();

   display.print("CO2: ");
   display.print(bsec.co2_eq());
   if (isDangerous(bsec.co2_eq(), CO2_DANGER_THRESHOLD))
   {
      display.print(" ");
      display.print(DANGER_SYMBOL);
   }
   display.println("ppm");

   display.print("VOC: ");
   display.print(bsec.b_voc_eq());
   if (isDangerous(bsec.b_voc_eq(), VOC_DANGER_THRESHOLD))
   {
      display.print(" ");
      display.print(DANGER_SYMBOL);
   }
   display.println("ppm");

   // Se c'è una condizione pericolosa, mostra un avviso lampeggiante
   if (isDangerousCondition && blinkState)
   {
      display.setTextSize(1);
      display.println("");
      display.println("PERICOLO! GAS!");
   }

   display.display();
}

/**
 * Enters low-power idle mode
 */
void enterIdleMode()
{
   Serial.println(F("Entering idle mode"));
   display.clearDisplay();
   display.display();
   isInIdleMode = true;
}

/**
 * Exits idle mode and restores display
 */
void exitIdleMode()
{
   Serial.println(F("Exiting idle mode"));
   isInIdleMode = false;
   updateCurrentPage();
}

/**
 * Shows splash screen with logo
 */
void showSplashScreen()
{
   display.clearDisplay();

   // Show logo centered on screen
   display.drawBitmap(
       (display.width() - LOGO_WIDTH) / 2,
       (display.height() - LOGO_HEIGHT) / 2,
       logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
   display.display();
   delay(1000);

   // Show scrolling welcome text
   display.clearDisplay();
   display.setTextSize(2);
   display.setTextColor(SSD1306_WHITE);
   display.setCursor(10, 0);
   display.println(F("DANOC sta armando per voi..."));
   display.display();

   // Scroll animation
   display.startscrollright(0x00, 0x0F);
   delay(2000);
   display.stopscroll();
   delay(500);
   display.startscrollleft(0x00, 0x0F);
   delay(2000);
   display.stopscroll();

   // Clear display before showing first page
   display.clearDisplay();
   display.display();
}

/**
 * Verifica se ci sono misurazioni pericolose e attiva il LED rosso se necessario
 */
void checkDangerousMeasurements()
{
   // Controlla tutti i valori rilevanti
   bool dangerDetected = false;

   // Controlla IAQ
   if (bsec.iaq() > IAQ_DANGER_THRESHOLD)
   {
      dangerDetected = true;
   }

   // Controlla CO2 equivalente
   if (bsec.co2_eq() > CO2_DANGER_THRESHOLD)
   {
      dangerDetected = true;
   }

   // Controlla VOC equivalente
   if (bsec.b_voc_eq() > VOC_DANGER_THRESHOLD)
   {
      dangerDetected = true;
   }

   // Controlla valore generico del gas
   if (gas.value() > GAS_DANGER_THRESHOLD)
   {
      dangerDetected = true;
   }

   // Aggiorna lo stato di pericolo
   isDangerousCondition = dangerDetected;

   // Il controllo del LED è gestito nel loop principale per permettere il lampeggio
   if (!isDangerousCondition)
   {
      // Se non c'è pericolo, assicurati che LED sia spento usando entrambi i metodi
      digitalWrite(LED_BUILTIN, HIGH); // LED OFF (high is off for standard Arduino)
      nicla::leds.setColor(off);       // LED OFF usando l'API Nicla
   }
   // Quando c'è pericolo, il lampeggio del LED è gestito nel loop principale
}

/**
 * Controlla se un valore è oltre la soglia di pericolo
 * @param value il valore da controllare
 * @param threshold la soglia da confrontare
 * @return true se il valore è oltre la soglia
 */
bool isDangerous(float value, float threshold)
{
   return value > threshold;
}

/**
 * Resets sensors to prevent drift and ensure reliable operation
 */
void resetSensors()
{
   Serial.println(F("Resetting sensors"));
   accel.begin();              // Reinitialize accelerometer
   previousAcceleration = 0.0; // Reset baseline

   // Quick recalibration
   for (int i = 0; i < 10; i++)
   {
      BHY2.update();
      float reading = abs(accel.x()) + abs(accel.y()) + abs(accel.z());
      previousAcceleration = reading * 0.2 + previousAcceleration * 0.8;
      delay(50);
   }
}
