#!/usr/bin/sudo python3
import os
import sys

FORBIDDEN=[";", "/", "(", ")", ">", "<", "&", "|"]

if len(sys.argv) <= 1:
	print("Use: ping IP")
	exit()

hostname = sys.argv[1]
for c in FORBIDDEN:
	if c in hostname:
		print("Wrong hostname!!")
		exit()

os.system("ping -c 1 " + hostname)
