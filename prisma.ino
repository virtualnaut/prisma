#include "strip.h"
#include "BluetoothSerial.h"
#include "setup.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial bluetooth;

char bluetoothBuffer[256];

const uint8_t leftDataPin = 23;
const uint8_t leftClockPin = 22;
const uint8_t leftLength = 118;

const uint8_t rightDataPin = 14;
const uint8_t rightClockPin = 12;
const uint8_t rightLength = 117;

Strip strip = Strip(leftLength, leftDataPin, leftClockPin);

void setup()
{
    Serial.begin(115200);
    bluetooth.begin(BLUETOOTH_NAME);
}

void loop()
{
    if (bluetooth.available())
    {
        bluetooth.readBytesUntil(';', bluetoothBuffer, 256);
        strip.setAll({0, 0, 0});
        strip.setPixelColour(bluetoothBuffer[0], {255, 0, 0});
        strip.draw();
    }
    delay(20);
}
