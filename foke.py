#!/usr/bin/env python3

import sys


from fake_octo import FakeOcto
from filamon_connection import NoConnection

port = "/dev/ttyUSB0"
if len(sys.argv) > 1:
    port = sys.argv[1]

try:
    fake = FakeOcto(port)
except NoConnection:
    print("Error attempting to connect to the filament monitor: no connection")
except Exception as err:
    print("Error {} attempting to connect to the filament monitor: no connection".format(err))
    raise
else:
    fake.start()
    while True:
        ans = input("press q<enter> to quit")
        if ans == 'q':
            break
    fake.stop()

