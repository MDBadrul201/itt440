
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SIZE 1024

void write_file(int sockfd, struct sockaddr_in addr)
{

  char* filename = "helloworld1.txt";
  int i;
  char buffer[SIZE];
  socklen_t addr_size;

  // TO CREATE A FILE.
  FILE* fp = fp = fopen(filename, "w");

  // RECEIVE THE DATA AND ALSO WRITE THE DATA INTO THE FILE
  while (1)
  {
    addr_size = sizeof(addr);
    i = recvfrom(sockfd, buffer, SIZE, 0, (struct sockaddr*)&addr, &addr_size);

    if (strcmp(buffer, "END") == 0)
    {
      break;
    }

    printf("Receiving data from client: %s", buffer);
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }

  fclose(fp);
}

int main()
{

  // TO DEFINE THE IP AND THE PORT
  char* ip = "192.168.0.135";
  const int port = 20;

  // DEFINE THE VARIABLES
  int server_sockfd;
  struct sockaddr_in server_addr, client_addr;
  char buffer[SIZE];
  int e;

  // UDP SOCKET CREATED
  server_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (server_sockfd < 0)
  {
    perror("Socket Error");
    exit(1);
  }
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = bind(server_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (e < 0)
  {
    perror("Binding Error");
    exit(1);
  }

  printf("Starting UDP file transfer\n");
  write_file(server_sockfd, client_addr);

  printf("Data transfer completed.\n");
  printf("Closing connection.\n");
  close(server_sockfd);

  return 0;
}
