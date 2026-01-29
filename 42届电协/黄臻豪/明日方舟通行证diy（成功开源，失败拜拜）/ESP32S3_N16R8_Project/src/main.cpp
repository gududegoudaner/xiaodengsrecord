#include <Arduino.h>
#include <FastLED.h>

// --- Configuration ---
#define LED_PIN         48
#define NUM_LEDS        8
#define HALL_PIN        1 
#define LED_TYPE        WS2812B
#define COLOR_ORDER     GRB

// Colors
#define COLOR_IDLE      CRGB::PowderBlue // 淡蓝色
#define COLOR_TRIGGERED CRGB::Orange
#define COLOR_SUCCESS   CRGB::Green
#define COLOR_WARNING   CRGB::Red

CRGB leds[NUM_LEDS];

// --- State Machine ---
enum AppState {
    STATE_IDLE,      // 呼吸态
    STATE_TRIGGERED, // 触发态 (旋转)
    STATE_SUCCESS,   // 成功态 (闪烁)
    STATE_WARNING    // 失败/警告 (快闪)
};

AppState currentState = STATE_IDLE;
AppState nextState = STATE_IDLE;

// Forward declarations
void changeState(AppState newState);

// --- Timing Variables ---
unsigned long lastUpdate = 0;
unsigned long stateStartTime = 0;

// Idle Breathing
int brightness = 10;
int fadeAmount = 1;
int minBrightness = 2;
int maxBrightness = 25; // 极低亮度, max 25 for "10" feel or slightly higher

// Triggered Rotating
int rotateIndex = 0;
unsigned long lastRotateTime = 0;
int rotateInterval = 100; // milliseconds, decreases for acceleration

// Success Flash
int flashCount = 0;
bool flashState = false;
unsigned long lastFlashTime = 0;

// Warning Flash
unsigned long lastWarnTime = 0;
bool warnState = false;

// Simulation / Logic flags
bool isSensorTriggered = false;

void setup() {
    Serial.begin(115200);
    delay(2000); // Safety delay
    
    Serial.println("System Starting...");

    // Setup LEDs
    // ESP32-S3 specific: might need the RMT driver or I2S driver for FastLED
    // FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    // Note: On some ESP32-S3 boards, pin 48 is a standard GPIO. 
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(50); // Global brightness cap

    // Setup Hall Sensor
    pinMode(HALL_PIN, INPUT_PULLUP); // Assuming active LOW switch
    
    // Initial State
    currentState = STATE_IDLE;
    Serial.println("State: IDLE");
}

void handleIdle() {
    // Breathing effect
    // Sine wave or linear fade
    EVERY_N_MILLISECONDS(30) {
        brightness += fadeAmount;
        if (brightness <= minBrightness || brightness >= maxBrightness) {
            fadeAmount = -fadeAmount;
        }
        // Clamp
        if (brightness < minBrightness) brightness = minBrightness;
        if (brightness > maxBrightness) brightness = maxBrightness;

        FastLED.setBrightness(brightness);
        fill_solid(leds, NUM_LEDS, COLOR_IDLE);
    }
}

void handleTriggered() {
    // Rotating Orange Marquee
    // Accelerate rotation? 
    // Logic: Reduce rotateInterval over time
    
    unsigned long duration = millis() - stateStartTime;
    
    // Acceleration logic: start slow (150ms), go fast (30ms) over 3 seconds
    int currentInterval = map(constrain(duration, 0, 3000), 0, 3000, 150, 40);

    if (millis() - lastRotateTime > currentInterval) {
        lastRotateTime = millis();
        rotateIndex++;
        if (rotateIndex >= NUM_LEDS) rotateIndex = 0;
        
        // Draw: Fade all slightly, set new pixel orange
        fadeToBlackBy(leds, NUM_LEDS, 100); // Trail effect
        leds[rotateIndex] = COLOR_TRIGGERED;
        
        // Ensure brightness is up for this state
        FastLED.setBrightness(150); 
    }
}

void handleSuccess() {
    // Flash Green 2 times
    // ON - OFF - ON - OFF -> Idle
    // Duration per state: 200ms
    const int flashDuration = 200;
    
    if (millis() - lastFlashTime > flashDuration) {
        lastFlashTime = millis();
        flashState = !flashState;
        
        if (flashState) {
            FastLED.setBrightness(200);
            fill_solid(leds, NUM_LEDS, COLOR_SUCCESS);
            flashCount++;
        } else {
            fill_solid(leds, NUM_LEDS, CRGB::Black);
            if (flashCount >= 2) {
                // Done flashing
                changeState(STATE_IDLE);
            }
        }
    }
}

void handleWarning() {
    // Partial Red Fast Flash
    // E.g. every other LED flashes red
    const int warnDuration = 100;
    
    if (millis() - lastWarnTime > warnDuration) {
        lastWarnTime = millis();
        warnState = !warnState;
        
        FastLED.setBrightness(200);
        if (warnState) {
            for(int i=0; i<NUM_LEDS; i++) {
                if(i % 2 == 0) leds[i] = COLOR_WARNING;
                else leds[i] = CRGB::Black;
            }
        } else {
            fill_solid(leds, NUM_LEDS, CRGB::Black);
        }
    }
}

void changeState(AppState newState) {
    currentState = newState;
    stateStartTime = millis();
    
    // Reset specific state variables
    if (newState == STATE_TRIGGERED) {
        rotateIndex = 0;
        lastRotateTime = millis();
    } else if (newState == STATE_SUCCESS) {
        flashCount = 0;
        flashState = false; 
        lastFlashTime = millis();
        fill_solid(leds, NUM_LEDS, CRGB::Black); // Start clean
    } else if (newState == STATE_WARNING) {
        warnState = false;
        lastWarnTime = millis();
    } else if (newState == STATE_IDLE) {
        brightness = minBrightness;
        fadeAmount = 1;
    }
    
    switch(newState) {
        case STATE_IDLE: Serial.println("State: IDLE (Breathing)"); break;
        case STATE_TRIGGERED: Serial.println("State: TRIGGERED (Rotating)"); break;
        case STATE_SUCCESS: Serial.println("State: SUCCESS (Flash)"); break;
        case STATE_WARNING: Serial.println("State: WARNING (Red Flash)"); break;
    }
}

void loop() {
    // 1. Input Handling
    // Read Hall Sensor
    bool sensorVal = digitalRead(HALL_PIN); 
    // Assuming active LOW (LOW = detected magnet)
    
    // State Transitions
    if (currentState == STATE_IDLE) {
        if (sensorVal == LOW) { // Triggered
            changeState(STATE_TRIGGERED);
        }
        // Handle Serial commands for testing
        if (Serial.available()) {
            char c = Serial.read();
            if (c == 'w') changeState(STATE_WARNING); // Test warning
        }
    }
    else if (currentState == STATE_TRIGGERED) {
        // Simulation: After 3 seconds of "exchange", success
        if (millis() - stateStartTime > 3000) {
            changeState(STATE_SUCCESS);
        }
        
        // Allow Serial override for Warning
        if (Serial.available()) {
            char c = Serial.read();
            if (c == 'f') changeState(STATE_WARNING);
        }
    }
    else if (currentState == STATE_WARNING) {
        // Hold warning for 2 seconds then Idle
         if (millis() - stateStartTime > 2000) {
            changeState(STATE_IDLE);
        }
    }
    // STATE_SUCCESS handles its own transition to IDLE in handleSuccess()

    // 2. State Logic Execution
    switch (currentState) {
        case STATE_IDLE:      handleIdle();      break;
        case STATE_TRIGGERED: handleTriggered(); break;
        case STATE_SUCCESS:   handleSuccess();   break;
        case STATE_WARNING:   handleWarning();   break;
    }

    // 3. Update LEDs
    FastLED.show();
    
    // Small delay to yield
    delay(10); 
}

