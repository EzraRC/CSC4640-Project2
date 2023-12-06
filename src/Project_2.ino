#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <WiFiNINA.h>

// Define pins for SPI communication
#define PN532_SCK  (13)
#define PN532_MOSI (11)
#define PN532_SS   (10)
#define PN532_MISO (12)

// Define pins for I2C mode for NFC (IRQ and reset)
#define PN532_IRQ   (2)
#define PN532_RESET (3)

// Constants and global variables for NFC reading
const int DELAY_BETWEEN_CARDS = 500;
long timeLastCardRead = 0;
boolean readerDisabled = false;
int irqCurr;
int irqPrev;

// NFC reader instance (I2C mode)
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

// Button pin assignments
const int buttonPin1 = 4;
const int buttonPin2 = 5;
const int buttonPin3 = 6;

// Function pointer for MCU reset
void (* resetFunc) (void) = 0;

void setup() {
  Serial.begin(9600); // Initialize serial communication
  // Setup button pins as inputs with internal pull-up resistors
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
}

void loop() {
  // Handle button presses to trigger different modes

  // Button 1: NFC Read Mode
  if (digitalRead(buttonPin1) == LOW) {
    Serial.println("NFC Read Mode");
    delay(200); // Debounce delay
    Serial.begin(115200);
    while (!Serial) delay(10); // Wait for serial port

    nfc.begin();

    // Get firmware version of the NFC reader
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
      Serial.print("Didn't find PN53x board");
      while (1); // Halt if no NFC board found
    }
    // Display firmware version information
    Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
    Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
    Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

    nfcRead(); // Begin reading NFC tags
  }

  // Button 2: WiFi Sniffing Mode
  if (digitalRead(buttonPin3) == LOW) {
    Serial.println("Wifi Sniffing");
    delay(200);
    Serial.begin(9600);

    // Check WiFi module status and initialize
    if (WiFi.status() == WL_NO_MODULE) {
      Serial.println("WiFi module not found!");
      while (true); // Halt if no WiFi module detected
    }
    Serial.println("Starting network scan...");
    scanAndListNetworks(); // Start scanning for WiFi networks
    delay(60000); // Delay after scanning
  }

  // Button 3: Reset Functionality
  if (digitalRead(buttonPin4) == LOW) {
    Serial.println("Reset");
    resetFunc(); // Reset the microcontroller
  }
}





// NFC READ CAPABILITIES //

// NFC Reading Function: Handles the NFC reading logic
void nfcRead() {
  if (readerDisabled) {
    // Re-enable reader after specified delay
    if (millis() - timeLastCardRead > DELAY_BETWEEN_CARDS) {
      readerDisabled = false;
      startListeningToNFC();
    }
  } else {
    irqCurr = digitalRead(PN532_IRQ);

    // Detect card presence based on IRQ status
    if (irqCurr == LOW && irqPrev == HIGH) {
      Serial.println("Got NFC IRQ");
      handleCardDetected(); // Handle detected card
    }

    irqPrev = irqCurr;
  }
}

// Start listening for NFC cards
void startListeningToNFC() {
  irqPrev = irqCurr = HIGH;
  Serial.println("Waiting for an ISO14443A Card ...");
  nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
}

// Handle detected NFC card
void handleCardDetected() {
    uint8_t success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Buffer for UID
    uint8_t uidLength; // Length of the UID

    // Read NFC tag's info
    success = nfc.readDetectedPassiveTargetID(uid, &uidLength);
    Serial.println(success ? "Read successful" : "Read failed (not a card?)");
    Serial.println("");

    if (success) {
      // Display information about the card
      Serial.println("Found an ISO14443A card");
      Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
      Serial.print("  UID Value: ");
      nfc.PrintHex(uid, uidLength);

      // Check if card is Mifare Classic
      if (uidLength == 4) {
        // Construct card ID from UID
        uint32_t cardid = uid[0];
        cardid <<= 8; cardid |= uid[1];
        cardid <<= 8; cardid |= uid[2];
        cardid <<= 8; cardid |= uid[3];
        Serial.print("Seems to be a Mifare Classic card #");
        Serial.println(cardid);
      }
      Serial.println("");

      timeLastCardRead = millis();
    }

    // Disable reader temporarily after reading a card
    readerDisabled = true;
}






// WIFI CAPABILITIES //

// WiFi Network Scanning Function: Scans and lists available networks
void scanAndListNetworks() {
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    Serial.println("WiFi scan failed");
    return;
  }

  // Print number of networks found
  Serial.print("Found ");
  Serial.print(numSsid);
  Serial.println(" networks:");
  Serial.println("----------------------------------------------------------------");

  // Iterate through available networks
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    printNetworkDetails(thisNet);
    // Attempt to connect to unsecured networks
    if (WiFi.encryptionType(thisNet) == ENC_TYPE_NONE) {
      attemptConnection(thisNet);
    }
    Serial.println("----------------------------------------------------------------");
  }
}

// Function to print details of a network given its index
void printNetworkDetails(int networkIndex) {
    Serial.print("Network ");
    Serial.print(networkIndex + 1);
    Serial.println(":");
    Serial.print("  SSID: ");
    Serial.println(WiFi.SSID(networkIndex)); // Display the network SSID
    Serial.print("  Signal: ");
    Serial.print(WiFi.RSSI(networkIndex));
    Serial.println(" dBm"); // Display the signal strength
    Serial.print("  Encryption: ");
    printEncryptionType(WiFi.encryptionType(networkIndex)); // Display encryption type
    Serial.print("  BSSID: ");
    byte bssid[6];
    WiFi.BSSID(networkIndex, bssid);
    printMacAddress(bssid); // Display the BSSID
    Serial.print("  Channel: ");
    Serial.println(WiFi.channel(networkIndex)); // Display the channel
}

// Function to attempt connection to a network given its index
void attemptConnection(int networkIndex) {
    Serial.print("  Connecting to: ");
    Serial.println(WiFi.SSID(networkIndex)); // Attempting connection
    WiFi.begin(WiFi.SSID(networkIndex));
    // Wait for up to 10 seconds for connection
    long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
        delay(500);
        Serial.print("."); // Connection attempt indication
    }
    Serial.println();
    // Check and display the result of the connection attempt
    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("  Successfully connected to: ");
        Serial.println(WiFi.SSID(networkIndex));
        displayNetworkInfo(); // Display network information on successful connection
        WiFi.disconnect();
        Serial.print("  Disconnected from: ");
        Serial.println(WiFi.SSID(networkIndex)); // Inform about disconnection
    } else {
        Serial.println("  Connection failed."); // Connection failed message
    }
}

// Function to display detailed information about the connected network
void displayNetworkInfo() {
    Serial.print("      MAC Address: ");
    byte mac[6];
    WiFi.macAddress(mac);
    printMacAddress(mac); // Display MAC address
    Serial.print("      Local IP: ");
    Serial.println(WiFi.localIP()); // Display local IP address
    Serial.print("      Subnet Mask: ");
    Serial.println(WiFi.subnetMask()); // Display subnet mask
    Serial.print("      Gateway IP: ");
    Serial.println(WiFi.gatewayIP()); // Display gateway IP
}

// Function to print a MAC address in a readable format
void printMacAddress(byte mac[]) {
    for (int i = 0; i < 6; i++) {
        if (mac[i] < 16) {
            Serial.print("0");
        }
        Serial.print(mac[i], HEX);
        if (i < 5) {
            Serial.print(":"); // Separate MAC address octets with colon
        }
    }
    Serial.println();
}

// Function to print human-readable encryption type of a network
void printEncryptionType(int thisType) {
    switch (thisType) {
        case ENC_TYPE_WEP: Serial.println("WEP"); break;
        case ENC_TYPE_TKIP: Serial.println("WPA"); break;
        case ENC_TYPE_CCMP: Serial.println("WPA2"); break;
        case ENC_TYPE_NONE: Serial.println("None"); break; // No encryption
        case ENC_TYPE_AUTO: Serial.println("Auto"); break; // Auto-detect encryption type
    }
}
