# FridgeWatcher

ESP32 Project to monitor, log and secure the fride door that stays open sometimes. A Webserver will give an overview of the fride usage and will make a beep noise if the door stays open for too long.
The switch at the fride door will wake up the ESP32 that is put in deepsleep once the door cloeses.
The webserver is just avialable when the door is open because of deepsleep.
Thats why there is a branch "beepOnly" that will just beep if the door is open to long.

## Dependencies

This Project uses AsyncTCP and ESPasyncwebserver.

Copy the libraries folder in the Arduino folder to install the ESPasyncwebserver and AsnycTCP libraries.

For the file system on the ESP you will need the Sketch data uploader:

1) Go to the releases page and click the ESP32FS-1.0.zip file to download: [Here]](https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/)

2) Go to the Arduino IDE directory, and open the Tools folder.


3) Unzip the downloaded .zip folder to the Tools folder. You should have a similar folder structure:

    ```<home_dir>/Arduino-<version>/tools/ESP32FS/tool/esp32fs.jar```

    (If youre using a MAC go to the application and right klick to "Show package content" -> Contents/Java/tools/)

4) Finally, restart your Arduino IDE.

To check if the plugin was successfully installed, open your Arduino IDE. Select your ESP32 board, go to Tools and check that you have the option “ESP32 Sketch Data Upload“.

