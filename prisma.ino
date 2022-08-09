#include "BluetoothSerial.h"
#include "strip.h"
#include "controller.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define HEADER_SIZE 3

BluetoothSerial bluetooth;

char bluetoothBuffer[BLUETOOTH_BUFFER_SIZE];

Controller lights = Controller(STRIPS, STRIP_COUNT);

void setup()
{
    Serial.begin(115200);

    bluetooth.begin(BLUETOOTH_NAME);

    lights.setPixel(100, {255, 0, 0});
    lights.setMelds(DEFAULT_MELDS);
    lights.draw();
}

void loop()
{
    if (bluetooth.available())
    {
        // bluetooth.readBytesUntil(0, bluetoothBuffer, 256);

        // Ignore everything up to the first hash character.
        bluetooth.readBytesUntil('#', bluetoothBuffer, BLUETOOTH_BUFFER_SIZE);

        if (!bluetooth.available())
        {
            // There was no hash character in the message.
            return;
        }

        // Read the size.
        char sizeBuffer[2];
        bluetooth.readBytes(sizeBuffer, 2);
        uint16_t messageSize = (sizeBuffer[0] << 8) + sizeBuffer[1];

        // Read the actual message.
        bluetooth.readBytes(bluetoothBuffer, messageSize - HEADER_SIZE);

        // Decide what to do based on the first byte.
        switch (bluetoothBuffer[0])
        {
        case 'M':
            handleMeld();
            break;
        case '_':
            lights.setAll({0, 0, 0});
            lights.setPixel(bluetoothBuffer[1], {255, 0, 0});
            lights.draw();
            break;
        default:
            Serial.println("Invalid message");
        }
    }
    delay(5);
}

void handleMeld()
{
    bool melds[STRIP_COUNT];

    for (unsigned int strip = 0; strip < STRIP_COUNT; strip++)
    {
        melds[strip] = ((bluetoothBuffer[(strip / 8) + 1]) >> (7 - (strip % 8))) & 1;
    }
    lights.setMelds(melds);
}
