ApiTest:main.o libnetworkapi.so include/*.h
	$(CC) main.o -L. -lnetworkapi -o ApiTest -pthread
main.o:include/*.h main.c
	$(CC) -c main.c -I ./include
so:wifi.o ppp.o ethernet.o
	$(CC) wifi.o ppp.o ethernet.o -fPIC -shared -o libnetworkapi.so
wifi.o:include/*.h
	$(CC) -fPIC -c wifi/wifi.c -I ./include -I ./wifi
ppp.o:include/*.h
	$(CC) -fPIC -c ppp/ppp.c -I ./include -I ./ppp	
ethernet.o:include/*.h
	$(CC) -fPIC -c ethernet/ethernet.c -I ./include -I ./ethernet	
clean:
	rm *.o *.so
