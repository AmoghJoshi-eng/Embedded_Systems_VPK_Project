Embedded Systems (ECL403) - Pulse Sensing and Recording Platform with PCB, GUI and Database

Group members:
BT21ECE022 - Amogh Joshi
BT21ECE034 - Bhumik Patil
BT21ECE038 - Mukundan Vijay
BT21ECE042 - Shrikrishna Sawangikar



Instructions to be followed to run the Prototype:

1. Upload the Program written in Arduino IDE in ESP32-C3. Ensure that the ESP32-C3 and PC on which waveforms are displayed must be connected using a common network.
2. Ensure while uploading the program provide the 'SSID' and 'Password' of the device which is providing network access to PC and ESP32-C3.
3. Open the Webpage, created using Websocket Protocol and click on 'Start' to get the combined waveforms of Vatta, Pitta and Kapha.
4. To analyse each pulse separately, click on 'Show All Plots' to perform detailed analysis of each pulse. 




********Overview of Zip folder********
1) Folder Arduino_code - contains .ino file, which was uploaded in the program memory of ESP32 C3.
2) Folder GUI_files - contains .html file for GUI which was uploaded along with .html file for Webpage displaying the Plots.
3) Folder PCB files -  
	Subfolder Gerber_Multiplexer - contains design of PCB from easyeda for Multiplexer.
	Subfolder Gerber_Sensors - contains design of PCB from easyeda for MAX30102 Sensors.
     Subfolder Gerber_LTC - contains design of PCB from easyeda for LTC4317 Address Translator
4) Folder Video_description - contains .mp4 file for a brief description of the prototype