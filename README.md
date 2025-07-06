# 🔥 Fire and Smoke Detection System using AI and IOT

This project is an AI-powered real-time fire and smoke detection system. It combines object detection using the YOLO model with FastAPI for API management, and Firebase for real-time sensor data monitoring and control. The system is designed for quick response to fire hazards in indoor or outdoor environments.

---

## 🚀 Features

- **YOLO-based Fire and Smoke Detection** (YOLOv8 or later)
-  **Live Camera Streaming** (IP or USB camera)
-  **Firebase Integration** to monitor temperature/gas sensors
-  **Auto Trigger Detection** based on sensor thresholds
- **JSON Logging** of detection results (with timestamps, type, and bounding boxes)
- **REST API with FastAPI** for setting camera URL and controlling the system

---

## 🛠 Tools and Technologies

- **YOLOv8** – Object detection model
- **FastAPI** – High-performance web framework
- **OpenCV** – Video processing and camera stream handling
- **Firebase Realtime Database** – Sensor data monitoring and threshold triggers
- **Threading** – Parallel execution for live camera feed and model inference
- **Uvicorn** – ASGI server for running FastAPI
