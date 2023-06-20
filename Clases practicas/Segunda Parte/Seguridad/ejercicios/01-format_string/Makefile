ping: ping.c
	$(CC) -O0 -ggdb $< -o $@
	sudo chown root $@
	sudo chmod u+s $@

clean:
	rm -f ping