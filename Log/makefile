CC := g++
TARGET := test
OBJS = test.o log.o logstream.o logbuffer.o asynclog.o logfile.o \
		file.o thread.o threadId.o


$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS) -lpthread

test.o : test.cc log.h thread.h
	$(CC) -c test.cc -lpthread

log.o : log.cc log.h logstream.h logbuffer.h asynclog.h threadId.h 
	$(CC) -c log.cc -lpthread

logstream.o : logstream.cc logstream.h logbuffer.h
	$(CC) -c logstream.cc 

logbuffer.o : logbuffer.cc logbuffer.h
	$(CC) -c logbuffer.cc 

asynclog.o : asynclog.cc asynclog.h logbuffer.h noncopy.h mutex.h condition.h thread.h logfile.h
	$(CC) -c asynclog.cc  -lpthread


thread.o : thread.cc thread.h noncopy.h mutex.h condition.h threadId.h countDownLatch.h
	$(CC) -c thread.cc -lpthread

threadId.o : threadId.cc threadId.h
	$(CC) -c threadId.cc 

logfile.o : logfile.cc logfile.h noncopy.h file.h 
	$(CC) -c logfile.cc 

file.o : file.cc file.h noncopy.h
	$(CC) -c file.cc 


clean:
	rm *.o 
