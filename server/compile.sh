gcc server_outgoing.c -lpthread -Wall -o server_out.o;
gcc server_incoming.c -lpthread -Wall -o server_in.o;
# gcc tests/test_client.c -lpthread -Wall -o client.o;

gcc tests/test.c -lpthread -Wall -o test.o;
