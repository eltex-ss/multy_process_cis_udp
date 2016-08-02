#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MESSAGE_LENGTH      50
#define PORT_MESSAGE_LENGTH 10

int main(void)
{
  int sock;
  struct sockaddr_in server_address;
  socklen_t server_length;
  char message[MESSAGE_LENGTH];
  char my_message[MESSAGE_LENGTH];
  char port_message[PORT_MESSAGE_LENGTH];
  int port;

  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    perror("socket error");
    exit(1);
  }
  server_address.sin_family = AF_INET;
  server_address.sin_port = 10000;
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

  sprintf(my_message, "Hello!");
  server_length = sizeof(struct sockaddr_in);
  if (sendto(sock, my_message, MESSAGE_LENGTH, 0,
             (struct sockaddr *) &server_address, server_length) == -1) {
    perror("sendto error");
    close(sock);
    exit(1);
  }

  if (recvfrom(sock, port_message, PORT_MESSAGE_LENGTH, 0,
               (struct sockaddr *) &server_address, &server_length) == -1) {
    perror("recvfrom error");
    close(sock);
    exit(1);
  }

  memcpy(&port, port_message, sizeof(int));
  server_address.sin_port = port;
  if (recvfrom(sock, message, PORT_MESSAGE_LENGTH, 0,
               (struct sockaddr *) &server_address, &server_length) == -1) {
    perror("recvfrom error");
    close(sock);
    exit(1);
  }
  printf("%s\n", message);
  if (sendto(sock, my_message, MESSAGE_LENGTH, 0,
             (struct sockaddr *) &server_address, server_length) == -1) {
    perror("sendto error");
    close(sock);
    exit(1);
  }

  close(sock);

  return 0;
}
