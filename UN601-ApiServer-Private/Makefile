ApiServer:main.o system.o serial.o icc.o mcr.o picc.o pci.o scan.o signature.o printer.o com_print.o mb_gbk2uni.o GetFont.o loadapp.o apppermsfile.o include/*.h
	$(CC) main.o system.o serial.o icc.o mcr.o picc.o pci.o scan.o signature.o printer.o com_print.o mb_gbk2uni.o GetFont.o loadapp.o apppermsfile.o -o ApiServer -lpthread
main.o:include/*.h main.c 
	$(CC) -c main.c -I ./include
system.o:include/*.h 
	$(CC) -fPIC -c system/system.c -I ./include
serial.o:include/*.h
	$(CC) -fPIC -c serial/serial.c -I ./include
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
	$(CC) -fPIC -c printer/printer.c -I ./include -I ./printer
com_print.o:
	$(CC) -fPIC -c printer/com_print.c -I ./include -I ./printer
mb_gbk2uni.o:
	$(CC) -fPIC -c printer/mb_gbk2uni.c -I ./include -I ./printer
GetFont.o:
	$(CC) -fPIC -c printer/GetFont.c -I ./include -I ./printer
loadapp.o:include/*.h
	$(CC) -fPIC -c load/loadapp.c -I ./include
apppermsfile.o:include/*.h
	$(CC) -fPIC -c app/apppermsfile.c -I ./include
clean:
	rm *.o
	rm ApiServer
