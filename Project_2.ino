/**************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <WiFiNINA.h>    // variable for reading the pushbutton status

// If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK  (13)
#define PN532_MOSI (11)
#define PN532_SS   (10)
#define PN532_MISO (12)

// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield

const int DELAY_BETWEEN_CARDS = 500;
long timeLastCardRead = 0;
boolean readerDisabled = false;
int irqCurr;
int irqPrev;

// This example uses the IRQ line, which is available when in I2C mode.
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

// Define the pin numbers for the buttons
const int buttonPin1 = 2;
const int buttonPin2 = 3;
const int buttonPin3 = 4;
const int buttonPin4 = 5;

void(* resetFunc) (void) = 0;

void setup() {
  // Initialize the Serial communication
  Serial.begin(9600);
  // Set up the button pins as inputs
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
}

void loop() {
  // Check if button 1 is pressed
  if (digitalRead(buttonPin1) == LOW) {
    Serial.println("NFC Read Mode");
    delay(200); // Debounce delay
    Serial.begin(115200);
    while (!Serial) delay(10); // for Leonardo/Micro/Zero

    Serial.println("Hello!");

    nfc.begin();

    uint32_t versiondata = nfc.getFirmwareVersion();
    if (! versiondata) {
      Serial.print("Didn't find PN53x board");
      while (1); // halt
    }
    // Got ok data, print it out!
    Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);
    Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);
    Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
    nfcRead();
  }

  // Check if button 2 is pressed
  if (digitalRead(buttonPin2) == LOW) {
    Serial.println("NFC Write Mode");
    delay(200); // Debounce delay
  }

  // Check if button 3 is pressed
  if (digitalRead(buttonPin3) == LOW) {
    Serial.println("Wifi Sniffing");
    delay(200);
    Serial.begin(9600);
    // Initialize and check for the presence of the WiFi module
    if (WiFi.status() == WL_NO_MODULE) {
      Serial.println("WiFi module not found!");
      while (true); // Halt if no WiFi module is detected
    }
    Serial.println("Starting network scan...");
    scanAndListNetworks();
    delay(60000);
  }

  // Check if button 4 is pressed
  if (digitalRead(buttonPin4) == LOW) {
    Serial.println("RESET");
    resetFunc();
  }
}

// NFC READER SETUP NFC READER SETUP NFC READER SETUP NFC READER SETUP NFC READER SETUP NFC READER SETUP NFC READER SETUP
void nfcRead() {
  if (readerDisabled) {
    if (millis() - timeLastCardRead > DELAY_BETWEEN_CARDS) {
      readerDisabled = false;
      startListeningToNFC();
    }
  } else {
    irqCurr = digitalRead(PN532_IRQ);

    // When the IRQ is pulled low - the reader has got something for us.
    if (irqCurr == LOW && irqPrev == HIGH) {
       Serial.println("Got NFC IRQ");
       handleCardDetected();
    }

    irqPrev = irqCurr;
  }
}

void startListeningToNFC() {
  // Reset our IRQ indicators
  irqPrev = irqCurr = HIGH;

  Serial.println("Waiting for an ISO14443A Card ...");
  nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
}

void handleCardDetected() {
    uint8_t success = false;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

    // read the NFC tag's info
    success = nfc.readDetectedPassiveTargetID(uid, &uidLength);
    Serial.println(success ? "Read successful" : "Read failed (not a card?)");
    Serial.println("");

    if (success) {
      // Display some basic information about the card
      Serial.println("Found an ISO14443A card");
      Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
      Serial.print("  UID Value: ");
      nfc.PrintHex(uid, uidLength);

      if (uidLength == 4)
      {
        // We probably have a Mifare Classic card ...
        uint32_t cardid = uid[0];
        cardid <<= 8;
        cardid |= uid[1];
        cardid <<= 8;
        cardid |= uid[2];
        cardid <<= 8;
        cardid |= uid[3];
        Serial.print("Seems to be a Mifare Classic card #");
        Serial.println(cardid);
      }
      Serial.println("");

      timeLastCardRead = millis();
    }

    // The reader will be enabled again after DELAY_BETWEEN_CARDS ms will pass.
    readerDisabled = true;
}

// WIFI SNIFFEFR WIFI SNIFFEFR WIFI SNIFFEFR WIFI SNIFFEFR WIFI SNIFFEFR WIFI SNIFFEFR WIFI SNIFFEFR WIFI SNIFFEFR WIFI SNIFFEFR WIFI SNIFFEFR
void scanAndListNetworks() {
  // Scan for available WiFi networks
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    Serial.println("WiFi scan failed");
    return;
  }
  // Print the number of networks found
  Serial.print("Found ");
  Serial.print(numSsid);
  Serial.println(" networks:");
  Serial.println("----------------------------------------------------------------");
  // Iterate through all found networks
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    // Print details of each network
    printNetworkDetails(thisNet);
    // Attempt to connect to unsecured networks
    if (WiFi.encryptionType(thisNet) == ENC_TYPE_NONE) {
      attemptConnection(thisNet);
    }
    Serial.println("----------------------------------------------------------------");
  }
}
void printNetworkDetails(int networkIndex) {
  Serial.print("Network ");
  Serial.print(networkIndex + 1);
  Serial.println(":");
  Serial.print("  SSID: ");
  Serial.println(WiFi.SSID(networkIndex));
  Serial.print("  Signal: ");
  Serial.print(WiFi.RSSI(networkIndex));
  Serial.println(" dBm");
  Serial.print("  Encryption: ");
  printEncryptionType(WiFi.encryptionType(networkIndex));
  Serial.print("  BSSID: ");
  byte bssid[6];
  WiFi.BSSID(networkIndex, bssid);
  printMacAddress(bssid);
  Serial.print("  Channel: ");
  Serial.println(WiFi.channel(networkIndex));
}
void attemptConnection(int networkIndex) {
  Serial.print("  Connecting to: ");
  Serial.println(WiFi.SSID(networkIndex));
  WiFi.begin(WiFi.SSID(networkIndex));
  // Wait for connection attempt for up to 10 seconds
  long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  // Check the result of the connection attempt
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("  Successfully connected to: ");
    Serial.println(WiFi.SSID(networkIndex));
    displayNetworkInfo();
    WiFi.disconnect();
    Serial.print("  Disconnected from: ");
    Serial.println(WiFi.SSID(networkIndex));
  } else {
    Serial.println("  Connection failed.");
  }
}
void displayNetworkInfo() {
  // Display detailed network information
  Serial.print("      MAC Address: ");
  byte mac[6];
  WiFi.macAddress(mac);
  printMacAddress(mac);
  Serial.print("      Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("      Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("      Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
}
void printMacAddress(byte mac[]) {
  for (int i = 0; i < 6; i++) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i < 5) {
      Serial.print(":");
    }
  }
  Serial.println();
}
void printEncryptionType(int thisType) {
  // Print human-readable encryption type
  switch (thisType) {
    case ENC_TYPE_WEP: Serial.println("WEP"); break;
    case ENC_TYPE_TKIP: Serial.println("WPA"); break;
    case ENC_TYPE_CCMP: Serial.println("WPA2"); break;
    case ENC_TYPE_NONE: Serial.println("None"); break;
    case ENC_TYPE_AUTO: Serial.println("Auto"); break;
  }
}

//==============================================================================================================================================================