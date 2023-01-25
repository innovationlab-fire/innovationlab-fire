For more detailed information about this project visit our website: https://innovationlab-fire.netlify.app

**User Manual:**

1) To reconstruct the prototype, you need the materials listed in the following:

- Arduino R3 incl. USB-cabel
- MQ-2 Gas Sensor
- SPI Reader Micro Memory SD TF Card Memory Card Shield Module
- SIM800L GPRS GSM Module incl. antenna
- SD-Card
- Lithium Polymer Battery 3.7 V 2000 mAh
- Breadboard with 830 or 400 pins
- Sim-Card
- A Case for the hardware	(files for 3d-print available in repository)	

In addition, for the installation you need jumper wires, resistors (330 Ohm) and a soldering device. All in all, you should expect spending approximately 60 to 70 euros for the purchase of all materials for one device (as of January 2023).

2) After collecting all needed materials, you have to download the Arduino IDE. You can do that directly on the Arduino website. After successfully downloading the IDE you should also download the required library “Adafruit BusIO” by Adafruit via the integrated library manager on your Arduino IDE. Then, you can download the project code from our Github Repository.

3) After downloading the code, start your Arduino IDE and click on „File“ in the top left corner of the Arduino IDE and click on „Open…“. Then, you need to select the previously downloaded code and it will be opened in the IDE. Now, you only have to make a small alteration to the code, in order to send the SMS to the right phone number regarding your respective use case (we have made a comment like “XXXXX” to indicate where you have to change the number). After that, the code is ready to use.

4) Before starting with the hardware, create an empty csv file called "test.csv" on your SD card. This will be used later in order to save the measured gas values including the respective timestamps.

5) Regarding the hardware, you need to wire up the Arduino uno and the modules like shown in the hardware diagram below.

![Circuit-diagram-SEDI](https://user-images.githubusercontent.com/123488476/214719107-5ce79ffe-ec6f-48b8-acec-61978ea5e815.jpg)

You also need to solder the sim module correctly, connect it to the separate LiPo battery and clip on the green antenna. After you have done this, see if the sim module is working correctly if it flashes every 3 seconds. If it only flashes at 1 second intervals, the sim module is still searching for the available network. As a starting aid, you can find a video concerning the wiring and soldering on our Youtube chanel: https://www.youtube.com/channel/UCS5Bbf_3bhYVp7_8QDcPbxg. Please note that the Arduino requires a constant power supply for smooth operation (5V), which can be connected via the power supply unit. Please bear in mind that we only describe the start-up via a computer, which, however, is not a realistic option for the desired application in a forest. Therefore, please inquire about alternative power supply options.

6) Once the hardware is wired and soldered correctly, you can connect the Arduino with the USB cable to the computer. Click on the „Upload“ button in the toolbar of the IDE and the code will be uploaded to the Arduino. The code will then start running and you can open the serial monitor to track the process.

7) After the code was once uploaded to the Arduino through the IDE, the Arduino will always start running the code once it has a power supply. Therefore, you can choose alternative power supply methods other than plugging it into the computer as described above.

8) In case of high gas values in the air, the device sends an SMS to the phone number you have specified. In addition, the measured values are stored on the SD card and you can visualize them by inserting the SD card into your computer via Excel according to your wishes.

**Licenses**

Our project is published under the GNU GENERAL PUBLIC LICENSE version 3 license. The full source code of the project is available on our GitHub page.
