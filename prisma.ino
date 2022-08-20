#include "BluetoothSerial.h"
#include "strip.h"
#include "controller.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define HEADER_SIZE 3
#define VIRTUAL_STRIP_SIZE 5

BluetoothSerial bluetooth;

char bluetoothBuffer[BLUETOOTH_BUFFER_SIZE];
uint16_t contentSize = 0;

Controller lights = Controller(STRIPS, STRIP_COUNT);

void setup()
{
    Serial.begin(115200);

    bluetooth.begin(BLUETOOTH_NAME);

    lights.setMelds(DEFAULT_MELDS);
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

        contentSize = ((sizeBuffer[0] << 8) + sizeBuffer[1]) - HEADER_SIZE;

        // Read the actual message.
        bluetooth.readBytes(bluetoothBuffer, contentSize);

        // Decide what to do based on the first byte.
        switch (bluetoothBuffer[0])
        {
        case 'M':
            handleMeld();
            break;
        case 'V':
            handleVirtualStrips();
            break;
        case '.':
            lights.clearAll();
            lights.setMask(0, (bluetoothBuffer[1] << 8) + bluetoothBuffer[2]);
            lights.setMask(1, (bluetoothBuffer[1] << 8) + bluetoothBuffer[2]);
            lights.setMask(2, (bluetoothBuffer[1] << 8) + bluetoothBuffer[2]);
            lights.setMask(3, (bluetoothBuffer[1] << 8) + bluetoothBuffer[2]);
            lights.setMask(4, (bluetoothBuffer[1] << 8) + bluetoothBuffer[2]);
            lights.setMask(5, (bluetoothBuffer[1] << 8) + bluetoothBuffer[2]);
            lights.draw();
            break;
        default:
            Serial.println("Invalid message");
        }
    }
    delay(5);
}

/**
 * Handle a meld message
 */
void handleMeld()
{
    bool melds[STRIP_COUNT];
    char order[STRIP_COUNT];

    for (unsigned int strip = 0; strip < STRIP_COUNT; strip++)
    {
        order[strip] = bluetoothBuffer[strip + 1];
        melds[strip] = ((bluetoothBuffer[(strip / 8) + STRIP_COUNT + 1]) >> (7 - (strip % 8))) & 1;
    }

    lights.setMelds(melds);
    lights.setOrder(order);
}

void handleVirtualStrips()
{
    VirtualStripMessage virtualStrips[MAX_VIRTUAL_STRIPS];
    unsigned int stripCount = (contentSize - 1) / VIRTUAL_STRIP_SIZE;

    if ((contentSize - 1) % VIRTUAL_STRIP_SIZE)
    {
        // Invalid message!
        Serial.println("Invalid virtual strip message");
        return;
    }

    for (unsigned int strip = 0; strip < stripCount; strip++)
    {
        virtualStrips[strip] = {
            (bool)bluetoothBuffer[(strip * VIRTUAL_STRIP_SIZE + 1)],
            (unsigned int)((bluetoothBuffer[(strip * VIRTUAL_STRIP_SIZE) + 2]) << 8) + (unsigned int)(bluetoothBuffer[(strip * VIRTUAL_STRIP_SIZE) + 3]),
            (unsigned int)((bluetoothBuffer[(strip * VIRTUAL_STRIP_SIZE) + 4]) << 8) + (unsigned int)(bluetoothBuffer[(strip * VIRTUAL_STRIP_SIZE) + 5]),
        };
    }

    VirtualStripStatus error = lights.setVirtualStrips(virtualStrips, stripCount);

    switch (error)
    {
    case VirtualStripStatus::OutOfBounds:
        Serial.println("One or more virtual strips go out of bounds");
        lights.setAll({255, 0, 0});
        lights.draw();
        delay(500);
        lights.setAll({0, 0, 0});
        lights.draw();
        break;
    default:
        Serial.println("Successfully set the virtual strips");
    }

    for (unsigned int strip = 0; strip < stripCount; strip++)
    {
        lights.getVirtualStrip(strip)->setAll({255, 255, 255, 0.5});
    }
}