all:
	g++ th.cpp -o th -I../jdb -L../jdb -ljdb -lpthread -ldl -lsqlite3
