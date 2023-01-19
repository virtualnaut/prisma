#include "BluetoothSerial.h"
#include "strip.h"
#include "controller.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define HEADER_SIZE 3
#define LINEAR_VIRTUAL_STRIP_SIZE 6
#define MATRIX_VIRTUAL_STRIP_SIZE 12
#define MASK_SIZE 3

#define FILL_COLOUR_SIZE 9
#define FILL_BLEND_SIZE 13

#define MAX_MESSAGE_SIZE 512

BluetoothSerial bluetooth;

uint8_t bluetoothBuffer[BLUETOOTH_BUFFER_SIZE];
uint16_t contentSize = 0;

bool waitingForSegments = false;
unsigned int segmentCount = 0;
unsigned int segmentsReceived = 0;
unsigned int lastSegmentTime = 0;

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
        if (!waitingForSegments || (millis() - lastSegmentTime > BLUETOOTH_SPLIT_MESSAGE_WAIT))
        {
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

            segmentCount = (int)contentSize / MAX_MESSAGE_SIZE;

            if (segmentCount > 0)
            {
                bluetooth.readBytes(bluetoothBuffer, MAX_MESSAGE_SIZE);
                segmentsReceived = 1;
                lastSegmentTime = millis();
                waitingForSegments = true;
            }
            else
            {
                bluetooth.readBytes(bluetoothBuffer, contentSize);
                waitingForSegments = false;
                processBuffer();
            }
        }
        else
        {
            if (segmentsReceived == segmentCount)
            {
                bluetooth.readBytes(
                    bluetoothBuffer + segmentsReceived * MAX_MESSAGE_SIZE,
                    contentSize % MAX_MESSAGE_SIZE);
                waitingForSegments = false;
                processBuffer();
            }
            else
            {
                bluetooth.readBytes(
                    bluetoothBuffer + segmentsReceived * MAX_MESSAGE_SIZE,
                    MAX_MESSAGE_SIZE);
            }
            segmentsReceived++;
        }
    }
    delay(5);
}

void processBuffer()
{
    // Decide what to do based on the first byte.
    switch (bluetoothBuffer[0])
    {
    case 'M':
        handleMeld();
        break;
    case 'V':
        handleLinearVirtualStrips();
        break;
    case 'S':
        handleMatrixStrips();
        break;
    case 'B':
        handleMask();
        break;
    case 'F':
        handleFill();
        break;
    case 'X':
        handleMatrix();
        break;
    case 'R':
        handleRegion();
        break;
    case '.':
        lights.clearAll();
        // lights.setMask(0, (bluetoothBuffer[1] << 8) + bluetoothBuffer[2]);
        // lights.setMask(1, (bluetoothBuffer[1] << 8) + bluetoothBuffer[2]);
        // lights.setMask(2, (bluetoothBuffer[1] << 8) + bluetoothBuffer[2]);
        // lights.setMask(3, (bluetoothBuffer[1] << 8) + bluetoothBuffer[2]);
        // lights.setMask(4, (bluetoothBuffer[1] << 8) + bluetoothBuffer[2]);
        // lights.setMask(5, (bluetoothBuffer[1] << 8) + bluetoothBuffer[2]);
        // lights.draw();
        break;
    case ',':
        lights.getMatrix()->setPixel(bluetoothBuffer[1], bluetoothBuffer[2], parseRGBA(bluetoothBuffer, 3));
        lights.draw();
        break;

    default:
        Serial.println("Invalid message");
    }
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

void handleLinearVirtualStrips()
{
    static LinearVirtualStripMessage virtualStrips[MAX_VIRTUAL_STRIPS];
    unsigned int stripCount = (contentSize - 1) / LINEAR_VIRTUAL_STRIP_SIZE;

    if ((contentSize - 1) % LINEAR_VIRTUAL_STRIP_SIZE)
    {
        // Invalid message!
        Serial.println("Invalid virtual strip message");
        return;
    }

    for (unsigned int strip = 0; strip < stripCount; strip++)
    {
        virtualStrips[strip] = {
            (bool)bluetoothBuffer[(strip * LINEAR_VIRTUAL_STRIP_SIZE + 1)],
            (MaskMode)bluetoothBuffer[(strip * LINEAR_VIRTUAL_STRIP_SIZE + 2)],
            parseUInt16(bluetoothBuffer, (strip * LINEAR_VIRTUAL_STRIP_SIZE) + 3),
            parseUInt16(bluetoothBuffer, (strip * LINEAR_VIRTUAL_STRIP_SIZE) + 5)};
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
    }

    for (unsigned int strip = 0; strip < stripCount; strip++)
    {
        lights.getVirtualStrip(strip)->setAll({255, 255, 255, 0.5});
    }
}

void handleMatrixStrips()
{
    static MatrixVirtualStripMessage virtualStrips[MAX_VIRTUAL_STRIPS];
    unsigned int stripCount = (contentSize - 1) / MATRIX_VIRTUAL_STRIP_SIZE;

    if ((contentSize - 1) % MATRIX_VIRTUAL_STRIP_SIZE)
    {
        // Invalid message!
        Serial.println("Invalid virtual strip message");
        return;
    }

    for (unsigned int strip = 0; strip < stripCount; strip++)
    {
        unsigned int cursor = strip * MATRIX_VIRTUAL_STRIP_SIZE;
        virtualStrips[strip] = {
            (bool)bluetoothBuffer[(cursor + 1)],
            (MaskMode)bluetoothBuffer[(cursor + 2)],
            (bool)bluetoothBuffer[(cursor + 3)],
            (bool)bluetoothBuffer[(cursor + 4)],
            parseUInt16(bluetoothBuffer, (cursor) + 5),
            parseUInt16(bluetoothBuffer, (cursor) + 7),
            parseUInt16(bluetoothBuffer, (cursor) + 9),
            parseUInt16(bluetoothBuffer, (cursor) + 11)};
    }

    lights.getMatrix()->setVirtualStrips(virtualStrips, stripCount);
}

void handleMask()
{
    unsigned int maskCount = (contentSize - 1) / MASK_SIZE;

    if ((contentSize - 1) % MASK_SIZE)
    {
        // Invalid message!
        Serial.println("Invalid strip mask message");
        return;
    }

    lights.clearAll();

    for (unsigned int mask = 0; mask < maskCount; mask++)
    {
        lights.setMask(
            bluetoothBuffer[(mask * MASK_SIZE) + 1],
            parseUInt16(bluetoothBuffer, (mask * MASK_SIZE) + 2));
        // (uint16_t)((bluetoothBuffer[(mask * MASK_SIZE) + 2] << 8) + (bluetoothBuffer[(mask * MASK_SIZE) + 3])));
    }
    lights.draw();
}

void handleFill()
{
    unsigned int cursor = 3;

    unsigned int strip = bluetoothBuffer[1];
    bool redraw = bluetoothBuffer[2];

    while (cursor < contentSize)
    {
        switch (bluetoothBuffer[cursor])
        {
        case 'c':
            cursor = handleFillColour(strip, cursor);
            break;
        case 'b':
            cursor = handleFillBlend(strip, cursor);
            break;
        default:
            Serial.println("Invalid strip fill definition");
            return;
        }
    }

    if (redraw)
    {
        lights.clearAll();
        lights.draw();
    }
}

void handleMatrix()
{
    lights.initialiseMatrix(bluetoothBuffer[1], bluetoothBuffer[2]);
    Matrix *matrix = lights.getMatrix();

    matrix->setHorizontalFlip(bluetoothBuffer[3]);
    matrix->setVerticalFlip(bluetoothBuffer[4]);

    Serial.println("Device has entered matrix mode");
}

void handleRegion()
{
    unsigned int count = bluetoothBuffer[3] * bluetoothBuffer[4];
    bool clear = bluetoothBuffer[5];
    ColourRGBA colours[count];

    unsigned int cursor = 6;
    unsigned int pixel = 0;

    while (cursor < contentSize)
    {
        colours[pixel] = parseRGBA(bluetoothBuffer, cursor);
        cursor += 4;
        pixel++;
    }

    if (clear)
    {
        lights.clearAll();
    }

    lights.getMatrix()
        ->setRegion(bluetoothBuffer[1], bluetoothBuffer[2], bluetoothBuffer[3], bluetoothBuffer[4], colours);
    lights.draw();
}

unsigned int handleFillColour(unsigned int strip, unsigned int cursor)
{
    VirtualStrip *virtualStrip = lights.getVirtualStrip(strip);

    if (virtualStrip != nullptr)
    {
        virtualStrip->applyColourRange(
            parseUInt16(bluetoothBuffer, cursor + 1),
            parseUInt16(bluetoothBuffer, cursor + 3),
            parseRGBA(bluetoothBuffer, cursor + 5));
    }
    else
    {
        // Skip the message if the virtual strip index is invalid.
        Serial.println("Invalid strip index when setting fill");
    }
    return cursor + FILL_COLOUR_SIZE;
}

unsigned int handleFillBlend(unsigned int strip, unsigned int cursor)
{
    VirtualStrip *virtualStrip = lights.getVirtualStrip(strip);

    if (virtualStrip != nullptr)
    {
        virtualStrip->applyBlendRange(
            parseUInt16(bluetoothBuffer, cursor + 1),
            parseUInt16(bluetoothBuffer, cursor + 3),
            parseRGBA(bluetoothBuffer, cursor + 5),
            parseRGBA(bluetoothBuffer, cursor + 9));
    }
    else
    {
        // Skip the message if the virtual strip index is invalid.
        Serial.println("Invalid strip index when setting fill");
    }

    return cursor + FILL_BLEND_SIZE;
}

uint16_t parseUInt16(uint8_t *buffer, unsigned int offset)
{
    return (buffer[offset] << 8) + (buffer[offset + 1]);
}

ColourRGBA parseRGBA(uint8_t *buffer, unsigned int offset)
{
    return {
        buffer[offset],
        buffer[offset + 1],
        buffer[offset + 2],
        (float)buffer[offset + 3] / 255};
}