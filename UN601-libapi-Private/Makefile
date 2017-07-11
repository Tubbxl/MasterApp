CC=/opt/fsl/sysroots/arm_linux_5.3/bin/arm-poky-linux-gnueabi-gcc  -march=armv7ve -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=/opt/fsl/sysroots/cortexa7hf-neon-poky-linux-gnueabi
so:system.o icc.o mcr.o picc.o pci.o signature.o scan.o printer.o msgqueue.o serial.o externalserial.o
	$(CC) system.o icc.o mcr.o picc.o pci.o signature.o scan.o printer.o msgqueue.o serial.o externalserial.o -fPIC -shared -o libapi.so
system.o:include/*.h
	$(CC) -fPIC -c system/system.c -I ./include
icc.o:include/*.h
	$(CC) -fPIC -c icc/icc.c -I ./include
mcr.o:include/*.h
	$(CC) -fPIC -c mcr/mcr.c -I ./include
picc.o:include/*.h
	$(CC) -fPIC -c picc/picc.c -I ./include
pci.o:include/*.h
	$(CC) -fPIC -c pci/pci.c -I ./include
scan.o:include/*.h
	$(CC) -fPIC -c scan/scan.c -I ./include
signature.o:include/*.h
	$(CC) -fPIC -c signature/signature.c -I ./include
printer.o:
	$(CC) -fPIC -c printer/printer.c -I ./include
msgqueue.o:
	$(CC) -fPIC -c msg/msgqueue.c -I ./include
serial.o:include/*.h
	$(CC) -fPIC -c serial/serial.c -I ./include
externalserial.o:include/*.h
	$(CC) -fPIC -c externalserial/externalserial.c -I ./include
clean:
	rm *.o *.so
