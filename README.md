# SerialBluetoothATCommands
Serial Bluetooth AT Commands For HC-05 / HC-06

## Basic AT Command for HC-06

* Change name
* Change baud rate
* Change pin
 
## Code for Arduino
```
#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 2); // RX, TX

String command = ""; // Stores response of the HC-06 Bluetooth device


void setup() {
  // Open serial communications:
  Serial.begin(115200);
  Serial.println("Type AT commands!");
  
  // The HC-06 defaults to 9600 according to the datasheet.
  mySerial.begin(9600);
  mySerial.print("AT");
}

void loop() {
  // Read device output if available.
  if (mySerial.available()) {
    while(mySerial.available()) { // While there is more to be read, keep reading.
      command += (char)mySerial.read();
    }
    
    Serial.print(command);
    command = ""; // No repeats
  }
  
  // Read user input if available.
  if (Serial.available()){
    delay(10); // The delay is necessary to get this working!
    mySerial.write(Serial.read());
  } 
}
```
