# CSC4640 Project 2 - RFIDuo: Multi-tool Pentesting and Ethical Hacking Device

## Informative Summary Poster
<p float="left">
 <img src="https://github.com/EzraRC/CSC4640-Project2/blob/main/Images/Final%20Poster.png" alt="Canva Poster" width="500"/>
</p>

## Project Description 

This educational project merges the Arduino Nano IoT 33 with the HiLetgo PN532 Module to create a compact device capable of receiving and transmitting RFID and NFC signals, akin to the Flipper Zero. The main function is to scan NFC cards, capturing their hexadecimal data, which can be stored on the Arduino Nano IoT 33 or transferred to external devices, including mobile apps. Additionally, the device incorporates WiFi sniffing for analyzing nearby networks and features Bad USB scripting capabilities for ethical hacking exercises. This project provides a hands-on experience in RFID, NFC, WiFi protocols, Arduino programming, and cybersecurity concepts.

 

## Specifications 

**RFID and NFC Capabilities:**
- Utilizes Arduino Nano IoT 33 and HiLetgo PN532 Module.
- Scans RFID and NFC signals, similar to Flipper Zero.
- Captures hexadecimal data from NFC cards.

**WiFi Sniffing:**
- Incorporates WiFi sniffing capabilities.
- Analyzes nearby WiFi signals for educational insights.
- Identifies nearby Access Points
- Lists the password security being used

**Bad USB Scripting:**
- Features Bad USB scripting capabilities.
- Enables emulation of a USB device for ethical hacking exercises.
- Runs a script to display the Star Wars movie command in CMDprompt to demonstrate exploitability
 

## Bill of Materials 

Below is a list of the items we have used for this project without the 3D Printed plastic case. Due to a classroom setting, we were granted the ability to use the Arduino Starter Kit coupled with resistors and wires; below we have listed the alternative with the Arduino Nano 33 alone as well if you already have the wires available.

### Hardware Requirements

| # | Hardware Item                                                                   | Price      | Link                                                                                                                                                     |
|---|------------------------------------------------------------------------|------------|----------------------------------------------------------------------------------------------------------------------------------------------------------|
| 1 | Arduino Nano 33 IoT                                                    | USD 25.50  | https://store-usa.arduino.cc/products/arduino-nano-33-iot?selectedStore=us                                                                                 |
| 2 | Raspberry Pi Pico W                                                       | USD 14.70  | https://a.co/d/3aaKyKk |                                                                                                                  |
| 3 | Adafruit FeatherWing OLED - 128x64                                     | USD 14.95  | https://www.adafruit.com/product/4650                                                                                                                    |
| 4 | Amazon Basics 4-Pack 9 Volt Alkaline Performance All-Purpose Batteries | USD 9.99   | https://a.co/d/bKedZnm                                                                                                                                   |

- Approximated Total Cost:  USD 65.14

These items can be purchased through any reputable sellers, however, the links provided are from the direct first-party sellers. 


### Software Requirements

| # | Software Item                 | Price                                 | Link                                                    |
|---|----------------------|---------------------------------------|---------------------------------------------------------|
| 1 | Arduino IDE          | USD 0.00                              | https://www.arduino.cc/en/software                      |
| 2 | Autodesk Fusion 360  | USD 0.00* (Free for College Students) | https://www.autodesk.com/campaigns/education/fusion-360 |
| 3 | UltiMaker Cura 5.4.0 | USD 0.00                              | https://www.ultimaker.com/software/ultimaker-cura/      |
| 4 | BadUSB Files         | USD 0.00                              | https://github.com/dbisu/pico-ducky/tree/main           |

#### Additional Requirements for Arduino IDE
Within Arduino IDE, there are a couple of Libraries that need to be installed to get the Adafruit Featherwing OLED add-ons to work with the Arduino. The following list is all the additional libraries that need to be installed within the Arduino IDE: 

- PN532
- PN532_SPI
- PN532_HSU
- PN532_I2C

Most of the software listed are by preference except for Arduino IDE which is necessary to be able to upload code to the Arduino Nano 33 IoT device. Autodesk Fusion 360 and UltiMaker Cura 5.4.0 were chosen because it was recommended by our professor as well as the 3D Printer available was only compatible with the UltiMaker Cura 5.4.0 software.

## Block Diagram of Device 

<p float="left">
 <img src="https://github.com/EzraRC/CSC4640-Project2/blob/main/Images/Diagrams/Block%20Diagram.png" alt="Block Diagram" width="600"/>
</p>

 

## Schematic of the Circuit 

The schematic below shows the HiLetGo PN523 gets its power from the 5V and GND ports of the Arduino Nano IoT 33. The SDA and SCL pins on the PN523 are connected to the SDA and SCL pin holes on the Arduino. Finally, the featherwing OLED display is connected using the same SDA, SCL, and GND ports but instead of 5V it uses 3V.

<p float="left">
 <img src="https://github.com/EzraRC/CSC4640-Project2/blob/main/Images/Diagrams/Circuit%20Schematic.png" alt="Circuit Schematic" width="600"/>
</p>

 

## Source Code 

Below are GitHub links to where we have uploaded the source code for free use and manipulation. 

- [Source Code Folder](https://github.com/EzraRC/CSC4640-Project2/blob/main/src/)
- [Main Code File](https://github.com/EzraRC/CSC4640-Project2/blob/main/src/Project_2.ino)
- [Bad USB Script](https://github.com/EzraRC/CSC4640-Project2/blob/main/src/payload.dd)

 

## 3D Model of Device 

### Box
<p float="left">
 <img src="https://github.com/EzraRC/CSC4640-Project2/blob/main/Images/Screenshots/Box%201.jpg" alt="Box 1" width="500"/>
 <img src="https://github.com/EzraRC/CSC4640-Project2/blob/main/Images/Screenshots/Box%202.jpg" alt="Box 2" width="448"/>
</p>

### Lid
<p float="left">
 <img src="https://github.com/EzraRC/CSC4640-Project2/blob/main/Images/Screenshots/Lid%201.jpg" alt="Lid 1" width="500"/>
 <img src="https://github.com/EzraRC/CSC4640-Project2/blob/main/Images/Screenshots/Lid%202.jpg" alt="Lid 2" width="432"/>
</p>



## IRL Pictures

### Without Lid
<p float="left">
 <img src="https://github.com/EzraRC/CSC4640-Project2/blob/main/Images/IRL/Box%201.jpg" alt="Box 1" width="500"/>
 <img src="https://github.com/EzraRC/CSC4640-Project2/blob/main/Images/IRL/Box%202.jpg" alt="Box 2" width="280"/>
</p>


### Completed Box with Lid
<p float="left">
 <img src="https://github.com/EzraRC/CSC4640-Project2/blob/main/Images/IRL/Completed%20Box%201.jpg" alt="Completed Box 1" width="386"/>
 <img src="https://github.com/EzraRC/CSC4640-Project2/blob/main/Images/IRL/Completed%20Box%202.jpg" alt="Completed Box 2" width="500"/>
</p>

## Device Testing

Here, we demonstrate the device in action, showing the features that we have implemented.

### RFID / NFC Testing

Successful Scan
<p float="left">
 <img src="https://github.com/EzraRC/CSC4640-Project2/blob/main/Images/GIF/Success%20RFID%20Scan%20(1).gif" alt="Scan Success" width="386"/>
</p>

Failed Scan
<p float="left">
 <img src="https://github.com/EzraRC/CSC4640-Project2/blob/main/Images/GIF/Fail%20RFID%20Scan%20(1).gif" alt="Scan Fail" width="500"/>
</p>

### Wi-Fi Sniffer Testing
<p float="left">
 <img src="https://github.com/EzraRC/CSC4640-Project2/blob/main/Images/GIF/Networks-1.gif" alt="WiFi Sniffer" width="500"/>
</p>

### Bad USB Testing
<p float="left">
 <img src="https://github.com/EzraRC/CSC4640-Project2/blob/main/Images/GIF/Bad%20USB.gif" alt="Bad USB" width="500"/>
</p>


## Dimensions 

Below feature two tables which give the length, width, and height of the box and lid respectively in centimeters, as well as the weight of the completed device in grams. 


### Box Dimensions

| Measurement             | Value   |
|-------------------------|---------|
| Length (in centimeters) |  15.70cm |
| Width (in centimeters)  |  04.70cm |
| Height (in centimeters) |  04.00cm |
| Weight (in grams)       |  153.00g|

### Lid Dimensions

| Measurement             | Value   |
|-------------------------|---------|
| Length (in centimeters) |  15.70cm |
| Width (in centimeters)  |  04.70cm |
| Height (in centimeters) |  01.60cm |
| Weight (in grams)       |  35.00g|

### Completed Box Dimensions

| Measurement             | Value   |
|-------------------------|---------|
| Length (in centimeters) |   15.70cm |
| Width (in centimeters)  |   04.70cm |
| Height (in centimeters) |   05.00cm |
| Weight (in grams)       |   188.00g|


## Conclusion 

After dedicating approximately three weeks to the project, our team successfully developed a multifunctional device with RFID Read capabilities, a Bad USB feature, and WiFi sniffing functionalities. We have successfully created our own version of the renowned Flipper Zero device! However, we have identified several areas for improvement, outlined below:

- On-Off Switch: This feature would allow users to power on and off the device without the need to physically open its casing, streamlining the user experience.

- Optimized Wire Management: Recognizing the significance of efficient wire management, our team aims to implement better organization and structure for the internal wiring of the device.

- Increased RFID Read Success Rate: To elevate the performance of the RFID capabilities, we are committed to refining the system to achieve a higher frequency of successful RFID reads.
