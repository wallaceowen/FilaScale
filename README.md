# FilaScale

This firmware drives an ESP32 with a qvga touchscreen to service the sensors and user interface needs of a spool monitor attached to octoprint over a serial connection.

The code exchanges JSON data over binary 'envelopes' (sync byte, msg type, length, body, crc). Some queries have no body.

The firmware reads connected sensors for temperature, humidity and weight of the spool container, attends to the user via the touch interface, and replies to requests from an octoprint plugin called FilaMon (see OctoPrint-Filamon)

The json reply for the status request looks like: { "spool_id": 1423659708, "temp": 38.0, "humidity": .48, "weight": 788 }
where:
   spool_id is an RFID tag attached to the spool by the receiving department
   temp is the temp of the filament in the spool container
   humidity is the humidity of the filament in the spool container
   weight is the weight of the spool in the spool container (including the spool).

## Setup and Installation

Download the code and burn it into a FilaScale board (or your own board, or a breadboard, with TFT and sensors connected)


## The Hardware
The KiCAD board design for FilaScale is at https://github.com/wallaceowen/FilaScaleBoard/tree/master

## Usage

FilaScale listens for packets on the primary serial interface, maintaining a user interface in the background.
Packets over the serial interface deliver commands.  FilaScale responds with replies.

## Comunnications Protocol

Communications with Filascale is asynchronous.
Commands are acted on in the order of reception.
The message body is a proper JSON string.
This body is transported in a packet with a SYNC byte, a message type byte, a 16-bit length field (in little-endian byte order) set to the length of the body.
The body follows.
A 16-bit CRC, computed over the entire message, is appended.

Currently only one message is implemented: STATUS.
When sent by a connected octoprint the message has a zero length body. The reply contains the state of the sensors.  It has the following form:
  {"spool_id": 0, "temp": 21.15, "humidity": 41.10, "weight": 238.75}


## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
[MIT](https://choosealicense.com/licenses/mit/)
