# Smart Irrigation System (Arduino-Based)

This project is a fully integrated smart irrigation system built using Arduino. It is designed to monitor soil conditions and the surrounding environment, then make automatic decisions to ensure optimal plant watering while displaying data in a clear and interactive way.

The system relies on a soil moisture sensor to measure moisture levels and a temperature sensor to track environmental conditions. When the soil moisture drops below a defined threshold, the water pump is automatically activated to irrigate the plant, and it stops once the desired level is reached.

A transparent water tank is included, equipped with a water level sensor to visually represent the available water. The system also features a smart buzzer with different alert patterns:

- Fast beeping when temperature is high
- Intermittent beeping when water level is low


An LCD or OLED display is used to show real-time data such as:

- Soil moisture percentage
- Temperature
- Water Level
- Pump Status

To enhance user interaction, LED indicators are included to reflect system status:
-Green: System operating normally
-Blue: Pump is running
-Red: Error state (high temperature or low water level)

The system is connected to a mobile phone via a Bluetooth module, sending periodic updates every few seconds. Users can monitor system status and control the pump through a simple mobile application.

The entire project is housed in a well-organized enclosure (acrylic or wood), with clean wiring and labeled components, giving it a polished, product-like appearance.


Components
- Electronic Components
- Arduino UNO
- Soil Moisture Sensor (Capacitive recommended)
- Temperature Sensor (DHT11 or DHT22)
- Water Level Sensor
- Relay Module
- Water Pump
- Buzzer
- Bluetooth Module (HC-05 or HC-06)
- LCD 16x2 or OLED Display
- LEDs (Green, Red, Blue)
- Resistors (for LEDs)
- Push Buttons (for manual control / reset
- power supply
