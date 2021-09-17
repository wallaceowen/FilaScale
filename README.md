# FilaScale

This firmware drives an ESP32 with a qvga touchscreen to service the sensors and user interface needs of a spool monitor attached to octoprint over a serial connection.

The code exchanges binary 'envelopes' (sync_byte, msg_type, length, body, crc) with the body carrying json content (some queries have no body, most replies carry json)

The firmware reads connected sensors for temperature, humidity and weight of the spool container, attends to the user via the touch interface, and replies to requests from an octoprint plugin called FilaMon (see OctoPrint-Filamon)

The json reply for the status request looks like: { "spool_id": 1423659708, "temp": 38.0, "humidity": .48, "weight": 788 }
where:
   spool_id is an RFID tag attached to the spool by the receiving department
   temp is the temp of the filament in the spool container
   humidity is the humidity of the filament in the spool container
   weight is the weight of the spool in the spool container (including the spool).

## Setup

Download the code and burn it into an ESP32

    https://github.com/wallaceowen/FilaScale/archive/master.zip

