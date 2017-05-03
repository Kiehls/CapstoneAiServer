#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

#define WIDTH_SIZE 10
#define HEIGHT_SIZE 11

void setSocket(char *argv1, char *argv2);
char** returnBoard(char* parsedData);

int main(int argc, char **argv) {
  // NEED TO ADD SOME VARIABLES
  // if (argc != 3) {
  //   std::cout << "Usage : " << argv[0] << "<Host> <Port>" << std::endl;
  //   exit(1);
  // }
  char *IP = "203.246.112.146";
  char *Port = "6120";
  setSocket(IP, Port);
}

void setSocket(char *argv1, char *argv2) {
  int host = -2;
  int bytesread;
  int fd_max; //maximum file descriptor number
  int _socket; //waiting socket

  fd_set master; //master file descriptor
  fd_set temps; //temp file destriptor

  char sendMSG[BUFSIZ]; //buffer for client data
  char receiveMSG[BUFSIZ];
  char board[WIDTH_SIZE][HEIGHT_SIZE];
  char *token;

  struct hostent *_host;
  struct sockaddr_in  _server; //server's address

  // NEED TO CREATE A SOCKET FOR TCP SERVER
  if((_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
    std::cout << "Generate Socket Error" << std::endl;
    exit(1);
  }

  //return DNS to IP
  if ((_host = gethostbyname(argv1)) == 0) {
    std::cout << argv2 << ": Unknown Host" << std::endl;
    exit(1);
  }

  //Initialize Server Setting
  memset((void *)&_server, 0, sizeof(_server));
  _server.sin_family = AF_INET;
  _server.sin_port = htons((u_short)atoi(argv2));
  memcpy((void*) &_server.sin_addr, _host->h_addr, _host->h_length);

  //Server Connection
  if (connect(_socket, (struct sockaddr *)&_server, sizeof (_server)) < 0) {
    (void) close(_socket);
    std::cout << "Connection Error" << std::endl;
    exit(1);
  }

  // BIT INITIALIZE
  FD_ZERO(&master);
  FD_ZERO(&temps);

  //ADD THE LISTENR TO THE READS SET
  FD_SET(_socket, &master); // =set 1 index 3

  //KEEP TRACK OF THE BIGGEST FILE DESCRIPTOR
  fd_max = _socket;

  memset((void*)sendMSG, 0, sizeof(sendMSG));
  memset((void*)receiveMSG, 0, sizeof(receiveMSG));

  int count = 1;

  std::cout << "Graph Module Generated" << std::endl << std::endl;

  while (true) {
    std::cout << "WAITING FOR RECEIVING DATA" << std::endl;
    sprintf(sendMSG, "Data From Graph Module");

    temps = master;
    int selector;

    if((selector = select(fd_max + 1, &temps, NULL, NULL, NULL)) < 0) {
      std::cout << "Select Error" << std::endl;
      exit(1);
    }

    if (FD_ISSET(_socket, &temps)) {
      if((bytesread = read(_socket, receiveMSG, sizeof (receiveMSG)))) {
        receiveMSG[bytesread] = '\0';
        std::cout << receiveMSG << std::endl;
        std::cout << "Data Received Number: " << count++ << std::endl;

        write(_socket, sendMSG, strlen(sendMSG));

        memset((void*)sendMSG, NULL, sizeof(sendMSG));
        memset((void*)receiveMSG, NULL, sizeof(receiveMSG));
      }
      if(bytesread == -1) {
        std::cout << "Server Disconnected" << std::endl;
        close(_socket);
        exit(1);
      }
    }
  }
}
