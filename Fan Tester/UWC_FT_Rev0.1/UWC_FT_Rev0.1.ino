#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

#define LOGO_WIDTH 32
#define LOGO_HEIGHT 32
static const unsigned char PROGMEM uwc_logo[] = {
  0x00, 0x46, 0x00, 0x00, 0x00, 0xc7, 0x00, 0x00, 0x01, 0xc7, 0x80, 0x00, 0x01, 0xc7, 0x80, 0x00, 
	0x01, 0xc7, 0xc0, 0x00, 0x03, 0xc7, 0xc0, 0x00, 0x0b, 0xc7, 0xd0, 0x00, 0x1b, 0xc7, 0xd8, 0x00, 
	0x3b, 0xe7, 0xdc, 0x00, 0x7b, 0xd7, 0xde, 0x00, 0x7b, 0xcf, 0xde, 0x00, 0x3b, 0xc7, 0xdc, 0x00, 
	0x1b, 0xc7, 0xd8, 0x00, 0x0b, 0xc7, 0xd0, 0x00, 0x03, 0xc7, 0xc0, 0x00, 0x03, 0xc7, 0xc0, 0x00, 
	0x01, 0xe7, 0x80, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 
	0x00, 0x3c, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x09, 0x52, 0xe0, 0x00, 0x09, 0x5d, 0xa0, 0x00, 
	0x09, 0x7d, 0x00, 0x00, 0x0f, 0x6d, 0xa0, 0x00, 0x06, 0x20, 0xc0, 0x00
  // Your bitmap data here
};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int sensorPin=2; // Analog pin for RPM sensor
unsigned long prevTime = 0;
unsigned long elapsedTime;
volatile int rpm;
volatile int rpmCount;

void rpmInterrupt() {
  rpmCount++;
}

void setup() {
  Serial.begin(9600);

  pinMode(sensorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(sensorPin), rpmInterrupt, FALLING);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
 display.drawBitmap(50, 15, uwc_logo, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
  display.setCursor(5, 50);
 display.println("Powered By RF & EE");
 
 //display.clearDisplay();
  display.display();
  delay(5000);
}

void loop() {
  
  unsigned long currentTime = millis();
  elapsedTime = currentTime - prevTime;

  if (elapsedTime >= 3000) {
    detachInterrupt(digitalPinToInterrupt(sensorPin));
   //rpm = 60 * 1000 / (elapsedTime * rpmCount / 2);
   // rpm = ((60*1000)*rpmCount)/(elapsedTime*2);
    rpm=((rpmCount*60*1000)/(elapsedTime*2));
    rpmCount = 0;
    prevTime = currentTime;
    attachInterrupt(digitalPinToInterrupt(sensorPin), rpmInterrupt, FALLING);

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(25, 0);
    display.println("UWC");

    // Display the logo
    display.drawBitmap(100, 0, uwc_logo, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);

    display.setTextSize(1.5);
    display.setCursor(0, 20);
    display.println("Fan Speed Tester");
    display.println("");

    display.setTextSize(2.5);
     display.setCursor(5, 38);
    display.print("RPM:");
    display.drawRect(0, 30, SCREEN_WIDTH - 8, 30, SSD1306_WHITE); // Draw border around RPM print area
    display.print(rpm);

    // Calculate the position to center the RPM value
    int textWidth = display.getCursorX();
    int xPos = (SCREEN_WIDTH - textWidth) / 2;
    display.setCursor(xPos, 10);

    display.display();
  }
  analogWrite(7,10);
}
