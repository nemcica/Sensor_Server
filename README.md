# Distributed Sensor Server (C++ / Boost.Asio)

A simple distributed sensor system built in C++ using **Boost.Asio**, featuring a multithreaded TCP server and multithreaded sensor clients.  
The client simulates several sensors sending periodic readings, and the server aggregates incoming data in real time.

---
<img width="1272" height="425" alt="server-client" src="https://github.com/user-attachments/assets/74af6bab-d4b0-4ee7-8815-880336d81c72" />


## 🚀 Features
- Multithreaded TCP **server**
- Multithreaded TCP **client** with multiple sensor threads  
- Custom text protocol (e.g. `SENSOR 1 VALUE 25.4`)
- Thread-safe shared buffer using `std::mutex`
- Aggregator thread calculating:
  - **min**
  - **max**
  - **average**
  - **count**

---

## 🔧 Build Instructions

### Requirements
- **C++17** or newer  
- **Boost** (Boost.Asio)  
- Visual Studio **or** CMake

### Visual Studio
1. Open the `.sln` file  
2. Build `server_main`  
3. Build `client_main`  

---

## ▶️ How to Run

### Start the server:
server_main.exe
client_main.exe
