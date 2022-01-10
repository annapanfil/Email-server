void base(const char* ip_addr, const int port, void* (*client_f)(void*)){
  /*create server socket, accept clients and run client_f on each*/

  int server_socket, new_socket;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  //Configure server address
  struct sockaddr_in serverAddr = {.sin_family = AF_INET, .sin_port = htons(port), .sin_addr.s_addr = inet_addr(ip_addr)};
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  //Create the socket
  server_socket = socket(PF_INET, SOCK_STREAM, 0);
  if (server_socket == -1){
    printf("Error creating socket");
    exit (EXIT_FAILURE);
  }

  //Bind the address struct to the socket
  int res = bind(server_socket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
  if (res == -1){
    printf("Cannot bind [Errno: %d]\n", errno);
    exit(EXIT_FAILURE);
  }

  //Listen on the socket
  if(listen(server_socket, 50) == 0)
    printf("Listening\n");
  else
    printf("Error\n");

  pthread_t thread_id;

  while(1)
  {
      //Accept call creates a new socket for the incoming connection
      addr_size = sizeof serverStorage;
      new_socket = accept(server_socket, (struct sockaddr *) &serverStorage, &addr_size);

      if( pthread_create(&thread_id, NULL, client_f, &new_socket) != 0 )
         printf("Failed to create thread\n");

      pthread_detach(thread_id);
  }
  return 0;
}

void login(){

}
