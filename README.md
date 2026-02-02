# Automatic Parking System – Arduino Project

## 📌 Project Overview
This project implements a **simplified automatic parking system** using an **Arduino Uno**.
The system controls the **entrance and exit barriers** of a parking area, detects vehicles using
**ultrasonic sensors**, and displays the number of **available parking slots** on an LCD screen.

---

## 🎯 Objectives
- Detect vehicles at the entrance and exit of the parking
- Automatically open and close barriers using servo motors
- Keep track of the number of cars inside the parking
- Display the number of free parking slots on an LCD
- Provide visual feedback using LEDs

---

## 🧰 Hardware Components
- Arduino Uno
- 2 × Ultrasonic sensors (HC-SR04)
- 2 × Servo motors
- LCD 16x2 with I2C interface
- LEDs (red, green, yellow)
- Resistors
- Breadboard
- Jumper wires

---

## ⚙️ System Description
The system uses **two ultrasonic sensors**:
- one for the entrance
- one for the exit

Each barrier is controlled using a **finite state machine** with four states.

Servo motors move gradually to ensure smooth and safe operation.
Sensor readings are filtered to avoid false detections caused by noise.

The LCD displays the number of **free parking slots** in real time, while LEDs indicate
whether entry is allowed or denied.
