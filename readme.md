# prisma

`prisma` is software to control APA102 or SK9822 LEDs using an ESP32 over bluetooth.

## Configuration
Configuration is stored in a file named `/setup.h`. The options are as follows:

Before uploading `prisma` to a board, set your config in `/setup.h.example` and rename it to `/setup.h`.

| Name                           | Description                                                                                             |
| ------------------------------ | ------------------------------------------------------------------------------------------------------- |
| `BLUETOOTH_NAME`               | The bluetooth name of the device                                                                        |
| `BLUETOOTH_BUFFER_SIZE`        | The size of the bluetooth buffer                                                                        |
| `BLUETOOTH_SPLIT_MESSAGE_WAIT` | Time to wait between receiving segments of the same command                                             |
| `STRIP_COUNT`                  | The number of physical strips connected to the board                                                    |
| `LED_CHIP`                     | The type of the LEDs (use a member of `LEDChip` enum)                                                   |
| `STRIPS`                       | An array of structures defining the pins and count of LEDs for each strip: `{dataPin, clockPin, count}` |
| `DEFAULT_MELDS`                | The meld settings on startup, array of bools (see documentation for the meld command for more info)     |


### LED Type Specific Configuration
For some LED types, specific config options are needed.
| Name                       | Type    | Description                           |
| -------------------------- | ------- | ------------------------------------- |
| `NEO_PIXEL_COLOUR_FEATURE` | WS2812B | Defines the colour order for the LEDs |
| `NEO_PIXEL_METHOD`         | WS2812B | Defines how the pixels are updated    |

## Commands

All commands begin with a hash `#` followed by 2 bytes (big endian unsigned int) storing the size of the entire message (including this header). These have been omitted in all documentation below for brevity, and all offsets stated are relative to the start of the message itself, not including the header.

The maximum size of a bluetooth message is 512 B. If a command takes up more than this size, it should be split into multiple messages/segments. The `prisma` device will read the size of the command from the header and wait for the rest of the command. The amount of time `prisma` waits for subseqent messages is configurable via `BLUETOOTH_SPLIT_MESSAGE_WAIT`.

### Meld
The meld command is used to redefine how the LED strips are connected. For example:

| Byte             | Description                                                                                                                                                                                                                                            |
| ---------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| 0                | Uppercase 'M'                                                                                                                                                                                                                                          |
| 1..`STRIP_COUNT` | Definition of the ordering of the strips. There will be 1 byte for each strip, and each byte will contain the original index of the strip from `/setup.h`.                                                                                             |
| ...              | Definition of which end of each strip to connect to. Each bit corresponds to 1 strip, and states which end of the strip is the start. 1 represents the physical start of the strip and 0 the end. Padded with 0s to fill the rest of the current byte. |


<details>
<summary>Meld Example</summary>

---

`01001101 || 00000000 || 00000001 || 00000010 || 10100000`

Assuming this is a board with 3 strips connected, this command will set the lights up like so:
    - The ordering is unchanged, then
    - Starts at the physical start of the first strip
    - First strip connects to physical end of the second
    - Second strip connects to physical start of the third

---

</details>

<details>
<summary>Meld Order Example</summary>

---

`01001101 || 00000000 || 00000010 || 000000001 || 10100000`

In this example, the order of the second and third strips have been swapped, so that the first strip connects to the second and the second to the third. So, in full:
    - Start at the physical start of the first strip
    - First strip connects to the physical end of the third
    - Third strip connects to physical start of the second.

---

</details>

### Virtual Strips
The virtual strips command is used to split the melded strip into a number of virtual strips that can be addressed separately.

| Byte | Description                                       |
| ---- | ------------------------------------------------- |
| 0    | Uppercase 'V'                                     |
| ...  | A number of virtual strip definitions (see below) |

#### Virtual Strip Definition
| Offset | Description                                                                                                        |
| ------ | ------------------------------------------------------------------------------------------------------------------ |
| 0      | Whether pixels are referenced using a pixel offset or a fraction of the length. 0 for pixel offset, else fraction. |
| 1,2    | The start pixel of the strip (2-byte, BE, unsigned integer)                                                        |
| 3,4    | The end pixel of the strip (2-byte, BE, unsigned integer)                                                          |

<details>
<summary>Example</summary>

---

`01010110 || 00000000 || 00000000 00000000 || 00000000 00110010 || 00000001 || 01100100 00000000 || 00000001 00101100`

In decimal,

`'V' 0 0 50 1 300 51`

This strip has 300 pixels, divided into 2 strips, where one strip starts at 0 and ends at 50, and the other starts at 300 and ends at 51.
The first strip is accessed using the absolute absolute pixel index and the second is accessed using a fraction.

---

</details>

### Strip Mask
The strip mask command sets the mask value for a number of virtual strip. The mask represents how much of the virtual strip should be visible.

| Byte | Description                                    |
| ---- | ---------------------------------------------- |
| 0    | Uppercase 'B'                                  |
| ...  | A number of strip mask definitions (see below) |

#### Strip Mask Definition

| Byte | Description                                             |
| ---- | ------------------------------------------------------- |
| 0    | Index of the strip to set the mask for                  |
| 1,2  | Value to set the mask to (2-byte, BE, unsigned integer) |

<details>
<summary>Example</summary>

---

`01000010 || 00000000 || 00100000 00111100 || 00000001 || 00000000 00000100`

`'B' 0 8252 1 4`

This message sets the mask for two strips. The first is set to a value of 8252 (assuming this strip has `isFractional` set, this is equivalent to 0.126...). The second strip has a mask of 4.

---

</details>

### Strip Fill
The strip fill command is used to define the colour of a range of pixels. This command has a number of sub commands that can be used to specify what method to use to fill the strip

The generic form of the strip fill command is:

| Byte | Description                                                       |
| ---- | ----------------------------------------------------------------- |
| 0    | Uppercase 'F'                                                     |
| 1    | Index of the virtual strip we are changing the colour of          |
| 2    | Subcommand character (see below)                                  |
| ...  | Subcommand arguments (specific to which subcommand is being used) |


#### Single Colour Subcommand
The single colour subcommand sets a range of pixels within a virtual strip to a single colour.

The character for this subcommand is a lowercase `c`.

The arguments are as follows (offsets are relative):
| Byte    | Description                          |
| ------- | ------------------------------------ |
| 0       | Start pixel within the virtual strip |
| 1       | End pixel within the virtual strip   |
| 2,3,4,5 | Red, green, blue and alpha channels  |

<details>
<summary>Example</summary>

---

`01000110 || 00000001 || 01100011 || 00000000 || 00110010 || 11111111 || 00000000 || 11111111 || 10000000`

`'F' 1 'c' 0 50 255 0 255 128 `

On virtual strip 1, for pixels 0 to 50 (inclusive), set the colour to RGBA(255, 0, 255, 0.5).

---

</details>

#### Blend Subcommand
The blend subcommand takes two colours and creates a linear gradient between the start and end pixels.

The character for this subcommand is a lowercase `b`.

The arguments are as follows (offsets are relative):
| Byte    | Description                                             |
| ------- | ------------------------------------------------------- |
| 0       | Start pixel within the virtual strip                    |
| 1       | End pixel within the virtual strip                      |
| 2,3,4,5 | Red, green, blue and alpha channels for the start pixel |
| 6,7,8,9 | Red, green, blue and alpha channels  for the end pixel  |


<details>
<summary>Example</summary>

---

`01000110 || 00000000 || 01100010 || 00110011 || 01100100 || 11111111 || 00000000 || 11111111 || 10000000 || 11111111 || 00000000 || 00000000 || 00000000`

`'F' 0 'b' 51 100 255 0 255 128 255 0 0 0`

On virtual strip 1, for pixels 51 to 100, set the start colour to RGBA(255, 0, 255, 0.5) and blend to the end pixel with a colour of RGBA(255, 0, 0, 0).

---

</details>

### Matrix Setup
The matrix setup command defines the device as a matrix and allows specification of the properties of the matrix.

| Byte | Description                             |
| ---- | --------------------------------------- |
| 0    | Uppercase 'X'                           |
| 1    | Width of the matrix                     |
| 2    | Height of the matrix                    |
| 3    | Whether to flip the matrix horizontally |
| 4    | Whether to flip the matrix vertically   |

### Matrix Region
Update a region of the display with new data.
| Byte | Description                                |
| ---- | ------------------------------------------ |
| 0    | Uppercase 'R'                              |
| 1    | X-coordinate of the top left of the region |
| 2    | Y-coordinate of the top left of the region |
| 3    | Width of the region                        |
| 4    | Height of the region                       |
| ...  | List of 4-byte RGBA colours (1 per pixel)  |
