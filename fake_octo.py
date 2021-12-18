""" Fake Filamon
    simulate the messaging protocol exchanged between octoprint-filamon and filamon box
"""

from __future__ import absolute_import

import time
import json
import threading
import logging
from logging.handlers import SysLogHandler

import filamon_connection as fc

JSON_DATA = {"printername": "bender_prime", "spool_id": 1423659708, "temp": 38.0, "humidity": .48, "weight": 788}
JSON_STRING = json.dumps(JSON_DATA)

class FakeOcto(threading.Thread):
    def __init__(self, port):
        self._logger = logging.getLogger("FilaMon")
        self._logger.setLevel(logging.DEBUG)
        self.port = port
        self.filacon = fc.FilamonConnection(self._logger, "/dev/ttyUSB0", "/dev/ttyUSB1", 115200)
        self.filacon.connect()
        threading.Thread.__init__(self, name="FakeOcto")
        self.daemon = True
        self.terminate = False
        self.retries = 3

    def stop(self):
        self.terminate = True
        self.join()

    def handle_client_msg(self, _type, body):
        if len(body):
            reply = body.decode('utf-8')
            print("Handle Client: received {} {} from filascale".format(_type, reply))
        else:
            print("Handle Client: received {} from filascale for body {}".format(_type, body))

    def exchange(self, msg):
        # print('Sending {}'.format(msg))
        self.filacon.send_msg(msg)

        for tries in range(self.retries):
            _type = None
            try:
                _type, body = self.filacon.recv_msg()
            except fc.NoData:
                time.sleep(0.1)
            except (fc.ShortMsg, fc.BadMsgType, fc.BadSize, fc.BadCRC) as err:
                print("fake octo: error {} trying to get msg".format(err))
                continue
            except fc.NoConnection:
                print('fake octo: No connection')
                raise
            else:
                # print('fake filamon: received type: %d body: [%s]'%( _type, body))
                self.handle_client_msg(_type, body)
                return
        raise fc.RetriesExhausted()

    def run(self):

        # msg = self.filacon.compose(fc.MT_CONFIG, b'{"scale": {"offset": 0, "gain": 1.00}}')
        # msg = self.filacon.compose(fc.MT_CONFIG, b'{"scale": {"offset": 95250, "gain": 1.00}}')
        msg = self.filacon.compose(fc.MT_CONFIG, b'{"scale": {"offset": 95250, "gain": 0.0001122}}')
        while not self.terminate:
            # Try to read a message
            try:
                self.exchange(msg)
            except (fc.RetriesExhausted, fc.NoConnection) as err:
                print("exhausted")
            else:
                time.sleep(2)
                break

        msg = self.filacon.compose(fc.MT_STATUS)
        while not self.terminate:
            # Try to read a message
            try:
                self.exchange(msg)
            except (fc.RetriesExhausted, fc.NoConnection) as err:
                print("exhausted")
            time.sleep(2)

if __name__ == '__main__':

    import sys
    port = "/dev/ttyUSB1"
    if len(sys.argv) > 1:
        port = sys.argv[1]

    fake = FakeOcto(port)
    fake.start()

    while True:
        ans = input("press q<enter> to quit")
        if ans == 'q':
            break

    fake.stop()
