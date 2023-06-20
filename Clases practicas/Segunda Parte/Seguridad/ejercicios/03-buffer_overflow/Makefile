login: login.c
	$(CC) -ggdb $< -o $@
	sudo chown root $@
	sudo chmod u+s $@

clean:
	rm -f login