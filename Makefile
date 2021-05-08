server:server.c
	gcc server.c -I/usr/include/mysql/ -L/usr/lib64/mysql -lmysqlclient -o $@

clean:
	rm server

