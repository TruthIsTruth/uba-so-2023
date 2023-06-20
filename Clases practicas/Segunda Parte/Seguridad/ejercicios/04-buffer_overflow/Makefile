.PHONY: all clean

all: login login-no-dep

login: login.c
	$(CC) -O0 -ggdb $< -o $@
	sudo chown root $@
	sudo chmod u+s $@

login-no-dep: login.c
	$(CC) -O0 -z execstack -ggdb $< -o $@
	sudo chown root $@
	sudo chmod u+s $@

clean:
	rm -f login login-no-dep