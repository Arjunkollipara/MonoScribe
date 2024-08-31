# MonoScribe
MonoScribe is a text-to-display device designed to empower deaf individuals by converting text input into real-time graphical output. It uses a digital screen to present text clearly, enhancing communication in social, professional, and educational settings, fostering inclusivity and connection.

## Project Overview

MonoScribe is an integrated wearable technology system designed to enhance communication and navigation for elderly individuals. It features smart glasses equipped with a camera and display, a smart walking stick with sensors, and an IoT-based data processing unit to provide real-time feedback and alerts.

## Features

- **Wearable Glasses**: Equipped with a camera for capturing the user's point of view and a display for visual alerts.
- **Smart Walking Stick**: Includes IR and ultrasound sensors to detect obstacles and provide haptic feedback.
- **ESP32 Module**: Handles data processing and communication between the glasses, stick, and local server.
- **Real-Time Alerts**: Provides visual and haptic feedback to inform users about obstacles and potential hazards.

## Components

1. **Camera**: Captures the user's point of view and helps in identifying obstacles.
2. **Display**: Shows real-time alerts and information to the user.
3. **Battery Pack**: Portable power source for the glasses and ESP32 module.
4. **ESP32**: Manages data from the camera and sensors, and communicates with the local server.
5. **Smart Stick**: Equipped with IR and ultrasound sensors for obstacle detection and haptic feedback.
6. **Local Server**: Receives data from the ESP32 and provides additional alerts if needed.

## Technology Stack

- **Hardware**:
  - ESP32
  - Camera
  - OLED Display
  - IR Sensors
  - Ultrasound Sensors
  - Battery Pack

- **Software**:
  - Arduino IDE (for ESP32 programming)
  - Python (for local server and data processing)
  - Custom algorithms for data processing and feedback control

## Installation

### Hardware Setup

1. **Assemble the Smart Glasses**:
   - Attach the camera and display to the frame.
   - Connect to the battery pack and ESP32 module.

2. **Assemble the Smart Walking Stick**:
   - Install IR and ultrasound sensors.
   - Connect to the haptic feedback system.

3. **Connect and Configure**:
   - Ensure all components are properly wired and powered.
   - Flash the ESP32 with the necessary firmware.

### Software Setup

1. **Clone the Repository**:
   ```sh
   git clone https://github.com/yourusername/monoscribe.git
