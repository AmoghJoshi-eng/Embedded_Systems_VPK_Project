# Embedded_Systems_VPK_Project
Pulse Measurement setup is a micro-controller based pulse measuring system which can be used to monitor pulse of human and analyse their health.
Pulse Measurement wearable setup based on Ayurvedic NADI PARIKSHAN is a micro-controller based pulse measuring system which can be used to monitor pulse of human. Setup designed has the added advantage of being wearable and the waveforms at each point are monitored in real-time on Serial Plotter as well as displayed on a GUI based webpage for intuitive analysis

# Hardware Components Used
1) MAX30102 Pulse Oximeter and Heart Rate Sensor Module and IC
<img src="https://github.com/AmoghJoshi-eng/Embedded_Systems_VPK_Project/blob/main/PCB_Photos/MAX30102_Sensor.jpg?raw=true" alt="Sensor_Module" width="200">


This is sensor module which is used to sense the pulse which works on the principle known as Photoplethysmography (PPG). The IC named as MAX30102EFDT+ consists of two LED's (Red LED and IR LED),a photodiode and a signal processing circuitry. To analyse the working of module read the following text. The LED (consider IR RED) emits light towards human skin.A certain amount of light gets absorbed by the blood flowing through the radial artery. The amount of light getting absorbed depends on the Oxygen content in the blood. The higher the oxygenated blood more amount of light gets absorbed. The remaining light gets reflected which is sensed by the photodiode. Depending on different absorption,a waveform gets plotted after certain processing after receiving signal through photodiode which represents the human heart pulse.

2) XIAO ESP32-C3
<img src="https://github.com/AmoghJoshi-eng/Embedded_Systems_VPK_Project/blob/main/PCB_Photos/Xiao_ESP32C3.jpg?raw=true" alt="Xiao_ESP32C3" width="200">


ESP32-C3 is Seeed Studio's miniature development board designed for Internet of Things (IoT) projects. It consisits of RISC-V 32-bit processor with FPU supporting WiFi and Bluetooth Low Energy (BLE) connectivity. It acting like the brain of the setup. It collects data from the three sensors and sends the data of each sensor to the Web based GUI.It provides battery charging pins which help to make the product battery powered and rechargeable.

3) TCA9456A MUltiplexing IC
<img src=""

