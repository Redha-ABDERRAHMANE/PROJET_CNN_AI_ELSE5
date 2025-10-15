CC=gcc

IDIR = /usr/include/hdf5/serial/
CFLAGS = -I$(IDIR) -O3
LIBS = -lhdf5_serial -lm

lenet_cnn_float: lenet_cnn_float.o utils.o utils.o lenet_cnn_method_def.o
	$(CC) -o lenet_cnn_float lenet_cnn_float.o  utils.o  lenet_cnn_method_def.o $(LIBS)

lenet_cnn_float.o: lenet_cnn_float.c 
	$(CC) -c lenet_cnn_float.c $(CFLAGS)

lenet_cnn_method_def.o: lenet_cnn_method_def.c 
	$(CC) -c lenet_cnn_method_def.c $(CFLAGS)

#pool.o: pool.c 
#	$(CC) -c pool.c $(CFLAGS)

#conv.o: conv.c 
#	$(CC) -c conv.c $(CFLAGS)

utils.o: utils.c 
	$(CC) -c utils.c $(CFLAGS)
	
clean: 
	rm -r lenet_cnn_float.o utils.o lenet_cnn_float lenet_cnn_method_def.o
