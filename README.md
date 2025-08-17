# Smart Cane for the Blind (Arduino)

An Arduino-based smart walking stick designed to assist visually impaired users by detecting obstacles and providing audio/haptic feedback.  

## Features
- **Obstacle detection:** Uses ultrasonic sensors to detect objects and alert the user with a buzzer (similar to car reverse sensors).  
- **Voice feedback:** Pressing a button triggers a voice module that announces the current time.  
- **Bluetooth remote:** A built-in Bluetooth controller allows remote activation and control.  

## Technologies & Components
- Arduino UNO  
- Ultrasonic sensor (HC-SR04)  
- Buzzer  
- Voice module / speaker  
- Bluetooth module  
- Push buttons  
- Power supply (battery-based)  

## How it works
1. The ultrasonic sensor measures the distance to nearby objects.  
2. When an obstacle is detected within a threshold, the buzzer beeps with increasing frequency as the user approaches.  
3. A button press activates the voice module to announce the current time.  
4. A Bluetooth-based remote provides additional control.  

## Future Improvements
- Add vibration motor for haptic feedback.  
- Extend battery life with power optimization.  
- Add GPS module for location assistance.  

## Author
Ofir Perez  
