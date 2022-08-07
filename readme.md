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

Before uploading `prisma` to a board, set your config in `/setup.h.example` and rename it to `/setup.h`.
