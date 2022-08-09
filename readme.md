# prisma

`prisma` is software to control APA102 or SK9822 LEDs using an ESP32 over bluetooth.

## Configuration
Configuration is stored in a file named `/setup.h`. The options are as follows:

| Name                    | Description                                                                                             |
| ----------------------- | ------------------------------------------------------------------------------------------------------- |
| `BLUETOOTH_NAME`        | The bluetooth name of the device                                                                        |
| `BLUETOOTH_BUFFER_SIZE` | The size of the bluetooth buffer                                                                        |
| `STRIP_COUNT`           | The number of physical strips connected to the board                                                    |
| `STRIPS`                | An array of structures defining the pins and count of LEDs for each strip: `{dataPin, clockPin, count}` |
| `DEFAULT_MELDS`         | The meld settings on startup, array of bools (see documentation for the meld command for more info).    |

Before uploading `prisma` to a board, set your config in `/setup.h.example` and rename it to `/setup.h`.

## Commands

All commands begin with a hash `#` followed by 2 bytes (big endian unsigned int) storing the size of the entire message (including this metadata wrapper). These have been omitted in all documentation below for brevity, and all offsets stated are relative to the start of the message itself, not including the metadata. 

### Meld
The meld message is used to redefine how the LED strips are connected. For example:

| Byte | Description                                                                                                                                                                                                                                            |
| ---- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| 0    | Uppercase 'M'                                                                                                                                                                                                                                          |
| ...  | Definition of which end of each strip to connect to. Each bit corresponds to 1 strip, and states which end of the strip is the start. 1 represents the physical start of the strip and 0 the end. Padded with 0s to fill the rest of the current byte. |

For example,

`01001101 10100000`

Assuming this is a board with 3 strips connected, this command will set the lights up like so:
 - Starts at the physical start of the first strip
 - First strip connects to physical end of the second
 - Second strip connects to physical start of the third