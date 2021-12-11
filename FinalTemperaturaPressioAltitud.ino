#include <Wire.h>
#include <Adafruit_BMP085.h>
#define seaLevelPressure_hPa 1020.25
#include <SD.h> //Load SD library

int chipSelect = 9; //chip select pin for the MicroSD Card Adapter
File file; // file object that is used to read and write data
Adafruit_BMP085 bmp;
  
void setup() {
  Serial.begin(9600);
  pinMode(6, OUTPUT);
  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
  pinMode(chipSelect, OUTPUT); // chip select pin must be set to OUTPUT mode
  if (!SD.begin(chipSelect)) { // Initialize SD card
    Serial.println("Could not initialize SD card."); // if return value is false, something went wrong.
  }
  
  if (SD.exists("file.txt")) { // if "file.txt" exists, fill will be deleted
    Serial.println("File exists.");
    if (SD.remove("file.txt") == true) {
      Serial.println("Successfully removed file.");
    } else {
      Serial.println("Could not removed file.");
    }
  }
}
  
void loop() {
    
   file = SD.open("file.txt", FILE_WRITE); // open "file.txt" to write data
  if (file) {
    
    file.print(bmp.readTemperature()); // write number to file
    file.print(";");
    file.print(bmp.readPressure());
    file.print(";");
    file.print(bmp.readAltitude());
    file.print(";");
    file.println(bmp.readSealevelPressure());
    file.close(); // close file

    //Turn on LED if everything works
    digitalWrite(6, HIGH);
    
   
    Serial.print("Temperature: "); // debug output: show written number in serial monitor
    Serial.println(bmp.readTemperature());
  } else {
    digitalWrite(6, LOW);
    Serial.println("Could not open file (writing).");
  }
  
  file = SD.open("file.txt", FILE_READ); // open "file.txt" to read data
  if (file) {
    Serial.println("- – Reading start – -");
    char character;
    while ((character = file.read()) != -1) { // this while loop reads data stored in "file.txt" and prints it to serial monitor
      Serial.print(character);
    }
    file.close();
    Serial.println("- – Reading end – -");
  } else {
    Serial.println("Could not open file (reading).");
  }
  delay(2000); // wait for 5000ms
}
