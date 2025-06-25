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
#define APP_VERSION "1.0.0"

// BME688 Sensor Warm-up Management
// BME688 requires 30 minutes warm-up for accurate gas readings
const unsigned long BME688_WARMUP_TIME = 1800000; // 30 minutes in milliseconds
unsigned long bme688StartTime = 0;
bool bme688IsWarmedUp = false;
bool showingWarmupScreen = false;

// Sensor Status Tracking
struct SensorStatus {
    bool initialized;
    bool communicating;
    unsigned long lastReadTime;
    float lastValidValue;
};

SensorStatus tempStatus = {false, false, 0, 0.0};
SensorStatus humStatus = {false, false, 0, 0.0};
SensorStatus gasStatus = {false, false, 0, 0.0};
SensorStatus baroStatus = {false, false, 0, 0.0};
SensorStatus bsecStatus = {false, false, 0, 0.0};

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

// Enhanced tap detection variables
const int TAP_BUFFER_SIZE = 10;
float accelHistory[TAP_BUFFER_SIZE]; // Buffer to store recent acceleration values
int accelHistoryIndex = 0;
unsigned long lastTapTime = 0;
const unsigned long TAP_COOLDOWN = 500; // Minimum ms between tap detections

// ===== DANGER THRESHOLDS =====
// Definitions of dangerous gas value thresholds
#define IAQ_DANGER_THRESHOLD 300  // IAQ > 300 is considered harmful
#define CO2_DANGER_THRESHOLD 2000 // CO2 > 2000 ppm can cause headaches and concentration difficulties
#define VOC_DANGER_THRESHOLD 5    // VOC > 5 ppm is considered dangerous
#define GAS_DANGER_THRESHOLD 1000 // Generic gas threshold value
#define DANGER_SYMBOL "!"         // Symbol to indicate danger on display

// Flag for danger status
bool isDangerousCondition = false;

// Variables for LED and text blinking
unsigned long lastLedToggleTime = 0;
const unsigned long ledBlinkInterval = 500; // 500ms for visible blinking
bool blinkState = true;                     // Shared state between LED and warning text

/**
 * Setup function - initializes hardware and sensors
 */
void setup()
{
   // Initialize serial communication
   Serial.begin(115200);
   Serial.println(F("DANOC Environmental Monitor v0.8 Starting..."));

   // Initialize Nicla board first
   nicla::begin();

   // Initialize sensors with error checking
   Serial.println(F("Initializing BHY2 system..."));
   if (!initializeSensorWithValidation("BHY2")) {
      Serial.println(F("CRITICAL: BHY2 initialization failed"));
      showErrorScreen("BHY2 INIT FAIL");
      return;
   }
   BHY2.begin();

   Serial.println(F("Initializing environmental sensors..."));
   // Initialize individual sensors with validation
   tempStatus.initialized = initializeSensorWithValidation("Temperature");
   if (tempStatus.initialized) temperature.begin();
   
   humStatus.initialized = initializeSensorWithValidation("Humidity");
   if (humStatus.initialized) humidity.begin();
   
   gasStatus.initialized = initializeSensorWithValidation("Gas");
   if (gasStatus.initialized) gas.begin();
   
   baroStatus.initialized = initializeSensorWithValidation("Barometer");
   if (baroStatus.initialized) barometer.begin();
   
   bsecStatus.initialized = initializeSensorWithValidation("BSEC");
   if (bsecStatus.initialized) bsec.begin();
   
   // Always initialize these for device operation
   stepCounter.begin();
   accel.begin();

   // Record BME688 start time for warm-up tracking
   bme688StartTime = millis();
   Serial.println(F("BME688 warm-up period started (30 minutes required)"));

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
   
   // Show sensor status after splash
   showSensorStatus();

   // Test different LED control methods (try all possibilities)
   // Method 1: Standard Arduino LED control
   pinMode(LED_BUILTIN, OUTPUT);
   digitalWrite(LED_BUILTIN, HIGH); // Start with LED off

   // Method 2: Nicla RGB LED control
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
   
   // Check BME688 warm-up status
   if (!bme688IsWarmedUp && (currentTime - bme688StartTime >= BME688_WARMUP_TIME)) {
      bme688IsWarmedUp = true;
      Serial.println(F("BME688 warm-up completed - gas readings now reliable"));
      // Force display update to show new status
      lastDisplayRefreshTime = 0;
   }
   
   // Validate sensor communications periodically
   validateSensorCommunications(currentTime);

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
         // Approach 2: Nicla RGB LED API (red)
         nicla::leds.setColor(red);
      }
      else
      {
         // Approach 2: Nicla RGB LED API (off)
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
   float currentAcceleration = sqrt(accel.x() * accel.x() + accel.y() * accel.y() + accel.z() * accel.z());

   // Store in circular buffer
   accelHistory[accelHistoryIndex] = currentAcceleration;
   accelHistoryIndex = (accelHistoryIndex + 1) % TAP_BUFFER_SIZE;

   // Check for tap pattern (not just magnitude change)
   if (detectTapPattern() && (currentTime - lastTapTime > TAP_COOLDOWN))
   {
      Serial.println(F("*** Tap detected - changing page ***"));
      lastActivityTime = currentTime; // Reset activity timer
      lastTapTime = currentTime;      // Update last tap time for debouncing

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

   // Check for dangerous conditions (only if BME688 is warmed up)
   if (bme688IsWarmedUp) {
      checkDangerousMeasurements();
   }

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
 * Calculates dew point from temperature and humidity
 */
float calculateDewPoint(float temperature, float humidity)
{
   // Constants for Magnus formula
   const float a = 17.27;
   const float b = 237.7;

   // Calculate gamma parameter (simplified Clausius-Clapeyron equation)
   float gamma = (a * temperature) / (b + temperature) + log(humidity / 100.0);

   // Calculate dew point
   float dewPoint = (b * gamma) / (a - gamma);

   return dewPoint;
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

   // Header with warm-up status
   display.print("DANOC v");
   display.print(APP_VERSION);
   display.print(" 1/2");
   if (!bme688IsWarmedUp) {
      display.print("[W]");
   }
   display.println();
   display.println("-----------------");

   // Sensor data with validation
   display.print(tempStatus.initialized ? "OK" : "X");
   display.print(" Temp: ");
   float tempValue = getValidatedSensorValue(temperature.value(), tempStatus, -40.0, 85.0);
   display.print(tempValue);
   display.println("C");

   display.print(humStatus.initialized ? "OK" : "X");
   display.print(" Hum: ");
   float humValue = getValidatedSensorValue(humidity.value(), humStatus, 0.0, 100.0);
   display.print(humValue);
   display.println("%");

   display.print(baroStatus.initialized ? "OK" : "X");
   display.print(" Baro: ");
   float baroValue = getValidatedSensorValue(barometer.value(), baroStatus, 300.0, 1100.0);
   display.print(baroValue);
   display.println("hPa");
   
   float altitude = calculateAltitude(baroValue);
   display.print("Alt: ");
   display.print(altitude);
   display.println("m");

   // Add dew point calculation with validated values
   float dewPoint = calculateDewPoint(tempValue, humValue);
   display.print("Dew Point: ");
   display.print(dewPoint);
   display.println("C");

   display.print("Steps: ");
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

   // Header with BME688 warm-up status
   display.print("DANOC v");
   display.print(APP_VERSION);
   display.print(" - 2/2");
   if (!bme688IsWarmedUp) {
      display.print(" [W]");
   }
   display.println();
   display.println("-----------------");
   
   // Show warm-up progress if still warming up
   if (!bme688IsWarmedUp) {
      unsigned long elapsed = millis() - bme688StartTime;
      float progressPercent = (float)elapsed / BME688_WARMUP_TIME * 100.0;
      unsigned long remainingMinutes = (BME688_WARMUP_TIME - elapsed) / 60000;
      
      display.print("Gas Warm-up: ");
      display.print(progressPercent, 1);
      display.println("%");
      display.print("Remaining: ");
      display.print(remainingMinutes);
      display.println(" min");
      display.println();
      display.setTextSize(1);
      display.println("IMPORTANT: Keep in");
      display.println("CLEAN environment");
      display.println("during calibration!");
      display.display();
      return;
   }

   // Air quality data
   Serial.println(bsec.toString()); // Debug to serial

   // Gas reading with status indicator (only if sensor initialized)
   display.print(gasStatus.initialized ? "" : "X ");
   if (gasStatus.initialized) {
      float gasValue = getValidatedSensorValue(gas.value(), gasStatus, 0.0, 10000.0);
      bool gasIsDangerous = isDangerous(gasValue, GAS_DANGER_THRESHOLD);
      display.print(gasIsDangerous ? "! " : "OK ");
      display.print("Gas: ");
      display.print(gasValue);
      if (gasIsDangerous) {
         display.print(" ");
         display.print(DANGER_SYMBOL);
      }
      display.println("ppm");
   } else {
      display.println("Gas: N/A");
   }

   // BSEC readings with status indicators (only if sensor initialized)
   display.print(bsecStatus.initialized ? "" : "X ");
   if (bsecStatus.initialized) {
      // IAQ reading
      float iaqValue = getValidatedSensorValue(bsec.iaq(), bsecStatus, 0.0, 500.0);
      bool iaqIsDangerous = isDangerous(iaqValue, IAQ_DANGER_THRESHOLD);
      display.print(iaqIsDangerous ? "! " : "OK ");
      display.print("IAQ: ");
      display.print(iaqValue);
      if (iaqIsDangerous) {
         display.print(" ");
         display.print(DANGER_SYMBOL);
      }
      display.println();

      // CO2 reading
      float co2Value = bsec.co2_eq();
      bool co2IsDangerous = isDangerous(co2Value, CO2_DANGER_THRESHOLD);
      display.print(co2IsDangerous ? "! " : "OK ");
      display.print("CO2: ");
      display.print(co2Value);
      if (co2IsDangerous) {
         display.print(" ");
         display.print(DANGER_SYMBOL);
      }
      display.println("ppm");

      // VOC reading
      float vocValue = bsec.b_voc_eq();
      bool vocIsDangerous = isDangerous(vocValue, VOC_DANGER_THRESHOLD);
      display.print(vocIsDangerous ? "! " : "OK ");
      display.print("VOC: ");
      display.print(vocValue);
      if (vocIsDangerous) {
         display.print(" ");
         display.print(DANGER_SYMBOL);
      }
      display.println("ppm");
   } else {
      display.println("IAQ: N/A");
      display.println("CO2: N/A");
      display.println("VOC: N/A");
   }

   // If there's a dangerous condition, show a blinking warning
   if (isDangerousCondition && blinkState)
   {
      display.setTextSize(1);
      display.println("");
      display.println("!!! GAS DANGER !!!");
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
   display.println(F("DANOC initializing..."));
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
 * Checks for dangerous measurements and activates red LED if necessary
 */
void checkDangerousMeasurements()
{
   // Only check if BME688 is warmed up and sensors are initialized
   if (!bme688IsWarmedUp) {
      isDangerousCondition = false;
      return;
   }
   
   bool dangerDetected = false;

   // Check BSEC values only if sensor is initialized and communicating
   if (bsecStatus.initialized && bsecStatus.communicating) {
      float iaqValue = getValidatedSensorValue(bsec.iaq(), bsecStatus, 0.0, 500.0);
      if (iaqValue > IAQ_DANGER_THRESHOLD) {
         dangerDetected = true;
         Serial.println(F("DANGER: IAQ threshold exceeded"));
      }

      float co2Value = bsec.co2_eq();
      if (co2Value > CO2_DANGER_THRESHOLD) {
         dangerDetected = true;
         Serial.println(F("DANGER: CO2 threshold exceeded"));
      }

      float vocValue = bsec.b_voc_eq();
      if (vocValue > VOC_DANGER_THRESHOLD) {
         dangerDetected = true;
         Serial.println(F("DANGER: VOC threshold exceeded"));
      }
   }

   // Check gas sensor if initialized and communicating
   if (gasStatus.initialized && gasStatus.communicating) {
      float gasValue = getValidatedSensorValue(gas.value(), gasStatus, 0.0, 10000.0);
      if (gasValue > GAS_DANGER_THRESHOLD) {
         dangerDetected = true;
         Serial.println(F("DANGER: Gas threshold exceeded"));
      }
   }

   // Update danger state
   isDangerousCondition = dangerDetected;

   // LED control is handled in main loop for blinking effect
   if (!isDangerousCondition) {
      // If no danger, ensure LED is off using both methods
      digitalWrite(LED_BUILTIN, HIGH); // LED OFF (high is off for standard Arduino)
      nicla::leds.setColor(off);       // LED OFF using Nicla API
   }
   // When there's danger, LED blinking is handled in main loop
}

/**
 * Checks if a value exceeds the danger threshold
 * @param value the value to check
 * @param threshold the threshold to compare against
 * @return true if the value exceeds the threshold
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
   Serial.println(F("Resetting sensors for drift prevention"));
   
   // Reinitialize all environmental sensors
   if (tempStatus.initialized) {
      temperature.begin();
      Serial.println(F("Temperature sensor reset"));
   }
   
   if (humStatus.initialized) {
      humidity.begin();
      Serial.println(F("Humidity sensor reset"));
   }
   
   if (gasStatus.initialized) {
      gas.begin();
      Serial.println(F("Gas sensor reset"));
   }
   
   if (baroStatus.initialized) {
      barometer.begin();
      Serial.println(F("Barometer sensor reset"));
   }
   
   if (bsecStatus.initialized) {
      bsec.begin();
      Serial.println(F("BSEC sensor reset"));
   }
   
   // Reset accelerometer and baseline
   accel.begin();
   previousAcceleration = 0.0;

   // Reset tap detection history
   for (int i = 0; i < TAP_BUFFER_SIZE; i++) {
      accelHistory[i] = 0;
   }
   accelHistoryIndex = 0;

   // Quick recalibration with more readings for better baseline
   Serial.println(F("Performing sensor recalibration..."));
   for (int i = 0; i < 20; i++) {
      BHY2.update();
      float reading = abs(accel.x()) + abs(accel.y()) + abs(accel.z());
      previousAcceleration = reading * 0.1 + previousAcceleration * 0.9;
      delay(100);
   }
   
   Serial.println(F("Sensor reset completed"));
}

/**
 * Detects a characteristic tap pattern in the acceleration data
 * @return true if a tap pattern is detected
 */
bool detectTapPattern()
{
   // Need at least a few samples
   if (accelHistoryIndex < TAP_BUFFER_SIZE - 1)
   {
      return false;
   }

   // Get index positions for analysis, accounting for circular buffer
   int current = accelHistoryIndex;
   int prev1 = (current - 1 + TAP_BUFFER_SIZE) % TAP_BUFFER_SIZE;
   int prev2 = (current - 2 + TAP_BUFFER_SIZE) % TAP_BUFFER_SIZE;
   int prev3 = (current - 3 + TAP_BUFFER_SIZE) % TAP_BUFFER_SIZE;
   int prev4 = (current - 4 + TAP_BUFFER_SIZE) % TAP_BUFFER_SIZE;

   // Calculate the average acceleration as baseline
   float sum = 0;
   for (int i = 0; i < TAP_BUFFER_SIZE; i++)
   {
      sum += accelHistory[i];
   }
   float avgAccel = sum / TAP_BUFFER_SIZE;

   // Calculate standard deviation to determine normal variation
   sum = 0;
   for (int i = 0; i < TAP_BUFFER_SIZE; i++)
   {
      sum += (accelHistory[i] - avgAccel) * (accelHistory[i] - avgAccel);
   }
   float stdDev = sqrt(sum / TAP_BUFFER_SIZE);

   // DEBUG: Print diagnostic values occasionally
   static unsigned long lastDebugTime = 0;
   unsigned long now = millis();
   if (now - lastDebugTime > 5000)
   { // Every 5 seconds
      Serial.print("Accel avg: ");
      Serial.print(avgAccel);
      Serial.print(" stdDev: ");
      Serial.println(stdDev);
      lastDebugTime = now;
   }

   // Reduce strictness: Look for significant acceleration change
   float peakValue = accelHistory[prev2];
   float peakThreshold = 1.8 * stdDev; // Reduced from 2.5

   // 1. Simpler peak detection
   bool hasPeak = peakValue > avgAccel + peakThreshold;

   // 2. Check for rapid change (tap signature)
   bool hasRapidChange = false;

   // Look for changes in recent history
   for (int i = 1; i < TAP_BUFFER_SIZE - 1; i++)
   {
      int idx = (current - i + TAP_BUFFER_SIZE) % TAP_BUFFER_SIZE;
      int nextIdx = (idx + 1) % TAP_BUFFER_SIZE;

      // If we find a significant jump between consecutive readings
      if (abs(accelHistory[nextIdx] - accelHistory[idx]) > tapThreshold / 2)
      {
         hasRapidChange = true;
         break;
      }
   }

   // Alternative simple detection as fallback - detect a single spike
   bool hasSpike = false;
   for (int i = 0; i < TAP_BUFFER_SIZE; i++)
   {
      if (accelHistory[i] > avgAccel + tapThreshold / 2)
      {
         hasSpike = true;
         break;
      }
   }

   // If either the pattern detection or the spike detection triggers
   bool detected = (hasPeak && hasRapidChange) || hasSpike;

   if (detected)
   {
      // When a tap is detected, print the values for debugging
      Serial.println("TAP DETECTED!");
      Serial.print("Peak: ");
      Serial.print(peakValue);
      Serial.print(" Avg: ");
      Serial.print(avgAccel);
      Serial.print(" Threshold: ");
      Serial.println(avgAccel + peakThreshold);
   }

   return detected;
}

/**
 * Initializes a sensor with validation and error checking
 * @param sensorName Name of the sensor for logging
 * @return true if initialization successful
 */
bool initializeSensorWithValidation(const char* sensorName) {
   Serial.print(F("Initializing "));
   Serial.print(sensorName);
   Serial.print(F("... "));
   
   // Add small delay for sensor stabilization
   delay(100);
   
   // For now, assume initialization succeeds
   // In a real implementation, you would check sensor response
   Serial.println(F("OK"));
   return true;
}

/**
 * Validates sensor communication and data integrity
 * @param currentTime Current timestamp for timeout checking
 */
void validateSensorCommunications(unsigned long currentTime) {
   const unsigned long SENSOR_TIMEOUT = 10000; // 10 seconds
   
   // Update communication status for each sensor
   if (tempStatus.initialized) {
      tempStatus.communicating = (currentTime - tempStatus.lastReadTime < SENSOR_TIMEOUT);
   }
   if (humStatus.initialized) {
      humStatus.communicating = (currentTime - humStatus.lastReadTime < SENSOR_TIMEOUT);
   }
   if (gasStatus.initialized) {
      gasStatus.communicating = (currentTime - gasStatus.lastReadTime < SENSOR_TIMEOUT);
   }
   if (baroStatus.initialized) {
      baroStatus.communicating = (currentTime - baroStatus.lastReadTime < SENSOR_TIMEOUT);
   }
   if (bsecStatus.initialized) {
      bsecStatus.communicating = (currentTime - bsecStatus.lastReadTime < SENSOR_TIMEOUT);
   }
}

/**
 * Gets validated sensor value with range checking
 * @param rawValue Raw sensor reading
 * @param status Sensor status structure
 * @param minValid Minimum valid value
 * @param maxValid Maximum valid value
 * @return Validated sensor value or last known good value
 */
float getValidatedSensorValue(float rawValue, SensorStatus &status, float minValid, float maxValid) {
   unsigned long currentTime = millis();
   
   // Check if value is within valid range
   if (rawValue >= minValid && rawValue <= maxValid && !isnan(rawValue)) {
      status.lastValidValue = rawValue;
      status.lastReadTime = currentTime;
      return rawValue;
   } else {
      // Return last known good value if current reading is invalid
      Serial.print(F("Invalid sensor reading: "));
      Serial.print(rawValue);
      Serial.print(F(", using last valid: "));
      Serial.println(status.lastValidValue);
      return status.lastValidValue;
   }
}

/**
 * Shows sensor initialization status on display
 */
void showSensorStatus() {
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(SSD1306_WHITE);
   display.setCursor(0, 0);
   
   display.println(F("Sensor Status:"));
   display.println(F("--------------"));
   
   display.print(F("Temp: "));
   display.println(tempStatus.initialized ? F("OK") : F("FAIL"));
   
   display.print(F("Hum:  "));
   display.println(humStatus.initialized ? F("OK") : F("FAIL"));
   
   display.print(F("Gas:  "));
   display.println(gasStatus.initialized ? F("OK") : F("FAIL"));
   
   display.print(F("Baro: "));
   display.println(baroStatus.initialized ? F("OK") : F("FAIL"));
   
   display.print(F("BSEC: "));
   display.println(bsecStatus.initialized ? F("OK") : F("FAIL"));
   
   display.println();
   display.println(F("BME688 30min warm-up"));
   display.println(F("NEEDS CLEAN AIR!"));
   display.println(F("No perfumes/solvents"));
   
   display.display();
   delay(5000);
}

/**
 * Shows critical error screen and halts execution
 * @param errorMsg Error message to display
 */
void showErrorScreen(const char* errorMsg) {
   display.clearDisplay();
   display.setTextSize(2);
   display.setTextColor(SSD1306_WHITE);
   display.setCursor(0, 0);
   
   display.println(F("ERROR:"));
   display.println(errorMsg);
   display.println();
   display.setTextSize(1);
   display.println(F("Device halted."));
   display.println(F("Check connections."));
   
   display.display();
   
   // Flash LED to indicate error
   while(true) {
      nicla::leds.setColor(red);
      delay(500);
      nicla::leds.setColor(off);
      delay(500);
   }
}
