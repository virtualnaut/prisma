#include "BluetoothSerial.h"
#include "setup.h"
#include "strip.h"
#include "controller.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial bluetooth;

char bluetoothBuffer[BLUETOOH_BUFFER_SIZE];

Controller lights = Controller(STRIPS, STRIP_COUNT);

void setup()
{
    Serial.begin(115200);

    bluetooth.begin(BLUETOOTH_NAME);

    lights.setPixel(100, {255, 0, 0});
    lights.draw();
}

void loop()
{
    if (bluetooth.available())
    {
        bluetooth.readBytesUntil(';', bluetoothBuffer, 256);
        lights.setAll({0, 0, 0});
        lights.setPixel(bluetoothBuffer[0], {255, 0, 0});
        lights.draw();
    }
    delay(20);
}
