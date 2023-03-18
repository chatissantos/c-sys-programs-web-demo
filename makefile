CC	= gcc
OSS_TARGET = oss
WORKER_TARGET = worker
SERVER_TARGET = server
OSS_DEMO_TARGET = ossDemo
CLIENT_TARGET = client

all: $(CLIENT_TARGET) $(OSS_TARGET) $(WORKER_TARGET) $(SERVER_TARGET) $(OSS_DEMO_TARGET)

$(OSS_TARGET): src/oss.c src/helpers/*.c
	$(CC) -o $@ src/oss.c src/helpers/*.c -I.

$(WORKER_TARGET): src/worker.c
	$(CC) -o $@ src/worker.c -I.

$(SERVER_TARGET): server.c
	$(CC) -o $@ server.c -I.
$(OSS_DEMO_TARGET): ossDemo.c
	$(CC) -o $@ ossDemo.c -I.
$(CLIENT_TARGET): client.c
	$(CC) -o $@ client.c -I.

clean:
	/bin/rm -f $(OSS_TARGET) $(WORKER_TARGET) $(SERVER_TARGET) $(OSS_DEMO_TARGET) $(CLIENT_TARGET)
