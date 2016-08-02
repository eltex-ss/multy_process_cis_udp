#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MESSAGE_LENGTH      50
#define PORT_MESSAGE_LENGTH 10

int CreateSocket(void)
{
  int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock == -1) {
    perror("socket error");
    exit(1);
  }
  return sock;
}

struct sockaddr_in InitializeAddress(int port_num)
{
  struct sockaddr_in address;
  
  address.sin_family = AF_INET;
  address.sin_port = port_num;
  address.sin_addr.s_addr = INADDR_ANY;
  return address;
}

void BindSocket(int sock, struct sockaddr_in *address)
{
  if (bind(sock, (struct sockaddr *) address,
           sizeof(struct sockaddr_in)) == -1) {
    perror("bind error");
    close(sock);
    exit(1);
  }
}

int main(void)
{
  int sock;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
  socklen_t client_length;
  char message[MESSAGE_LENGTH];
  char my_message[MESSAGE_LENGTH];
  char port_message[PORT_MESSAGE_LENGTH];
  int port_num = 10000;

  sock = CreateSocket();
  server_address = InitializeAddress(port_num);
  BindSocket(sock, &server_address);

  client_length = sizeof(struct sockaddr_in);
  sprintf(my_message, "Hi!");
  while (1) {
    if (recvfrom(sock, message, MESSAGE_LENGTH, 0,
                 (struct sockaddr *) &client_address, &client_length) == -1) {
      perror("recv error");
      close(sock);
      exit(1);
    }
    ++port_num;
    memcpy(port_message, &port_num, sizeof(int));
    if (sendto(sock, port_message, PORT_MESSAGE_LENGTH, 0,
              (struct sockaddr *) &client_address, client_length) == -1) {
      perror("send error");
      close(sock);
      exit(1);
    }
    if (fork() == 0) {
      int fork_socket;
      struct sockaddr_in fork_address;

      fork_socket = CreateSocket();
      fork_address = InitializeAddress(port_num);
      BindSocket(fork_socket, &fork_address);

      printf("New port: %d\n", port_num);
      if (sendto(fork_socket, my_message, MESSAGE_LENGTH, 0,
                 (struct sockaddr *) &client_address, client_length) == -1) {
        perror("sendto error");
        close(fork_socket);
        exit(1);
      }
      if (recvfrom(fork_socket, message, MESSAGE_LENGTH, 0,
                   (struct sockaddr *) &client_address, &client_length) == -1) {
        perror("recvfrom error");
        close(fork_socket);
        exit(1);
      }
      printf("%s\n", message);
      close(fork_socket);
      exit(0);
    }
    else {

    }
  }

  close(sock);

  return 0;
}
