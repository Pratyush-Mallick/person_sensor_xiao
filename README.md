# Description
The project demostrates the use of person sensor to turn the coventional Air Cooler into a Smart one.
The anchor of the project is the Person Sensor, which does the heavy lifting for computing the coordinates
of location of the person in frame. Useful Sensors is emerging startup by Pete Warden and they have come up with a sensor that is able to person person
detection on the sensor itself. The algorithm runs on sensor itself and generatesrates meta data such a bounding boxes
and number of faces in the frame and can be accessed through the I2C connection.

This repository contains code and build steps for Smart Air Cooler based on Person Sensor.

# Hardware Components:
* Person Sensor 
* Xiao Ble Sense
* Xiao Exapansion Board
* Servo Motor X 2
* Relay module
* Crompton Air Buddy

# Hardware Connection:
Person Sensor <---> I2C (D4, D5) | Xiao Ble Sense
Servo Vertical <----> A7 | Xiao Ble Sense
Servo Horizontal <----> A0 | Xiao Ble Sense
Relay <---> D10 | Xiao Ble Sense

# Libraries
* Person Sensor I2C drivers
* U8g2 monochrome graphics library
* Seeed Studio Motor Drivers.

# Build Steps
The repository is straight forward to clone and build it using Arduino or PlatformIO IDE.
The user just need to add the following code section in the intenal I2C library:
In the requestFrom() function from the I2C class add rxBuffer.clear(); code 
in the begining of the function.
