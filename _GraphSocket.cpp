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
#define INITIALIZE -2
#define ORDERBIT -4

typedef char(*STATE)[WIDTH_SIZE];

void SetSocket();
//State_node* SetState_fromServer(char state[HEIGHT_SIZE][WIDTH_SIZE], bool host);
//void SetState_fromSErver(char state[HEIGHT_SIZE][WIDTH_SIZE], bool host);
void ReturnBoard(char* parsedData, char (&board)[HEIGHT_SIZE][WIDTH_SIZE]);
int ReturnHost(char* parsedData);
int CheckData(char* msg);
bool isOrderBit(int _bit);

int main(int argc, char **argv) {

  SetSocket();
}

void SetSocket() {
  int host = INITIALIZE;
  int bytesread;
  int fd_max; //maximum file descriptor number
  int _socket; //waiting socket
  int order = INITIALIZE;

  fd_set master; //master file descriptor
  fd_set temps; //temp file destriptor

  char sendMSG[BUFSIZ]; //buffer for client data
  char receiveMSG[BUFSIZ];
  char tempsBUF[BUFSIZ];
  char certifiData[BUFSIZ];
  char board[HEIGHT_SIZE][WIDTH_SIZE];

  char *token;
  char *IP = "203.246.112.146";
  char *Port = "6120";

  struct hostent *_host;
  struct sockaddr_in  _server; //server's address

  // NEED TO CREATE A SOCKET FOR TCP SERVER
  if((_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
    std::cout << "Generate Socket Error" << std::endl;
    exit(1);
  }

  //return DNS to IP
  if ((_host = gethostbyname(IP)) == 0) {
    std::cout << Port << ": Unknown Host" << std::endl;
    exit(1);
  }

  //Initialize Server Setting
  memset((void *)&_server, 0, sizeof(_server));
  _server.sin_family = AF_INET;
  _server.sin_port = htons((u_short)atoi(Port));
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

  std::cout << "Graph Module Generated" << std::endl;

  while (true) {
    temps = master;
    int selector;

    if((selector = select(fd_max + 1, &temps, NULL, NULL, NULL)) < 0) {
      std::cout << "Select Error" << std::endl;
      exit(1);
    }

    if (FD_ISSET(_socket, &temps)) {
      if((bytesread = read(_socket, receiveMSG, sizeof (receiveMSG)))) {
        std::cout << "Data Received Number: " << count++ << std::endl;

        receiveMSG[bytesread] = '\0';
        //memcpy((void*) tempsBUF, (void *) receiveMSG, strlen(receiveMSG));
        memcpy((void*) certifiData, (void *) receiveMSG, strlen(receiveMSG));
        std::cout << receiveMSG << std::endl;

        //CHECK RECEIVED DATA IS ORDER DATA OR BOARD DATA
        int checkBit = CheckData(certifiData);

        //IF CHECKBIT == -4 -> ORDER BIT
        if(isOrderBit(checkBit)) {
          std::cout << "Order Bit" << checkBit << std::endl;
          // CODE ABOUT SELECTING AI ORDER
          // MSG FORM LIKE "ORDER|0"
          int order = 1;
          sprintf(sendMSG, "ORDER|%d", order);
          write(_socket, sendMSG, strlen(sendMSG));
        }
        // IF CHECKBIT != -4 -> BOARD DATA
        else {
          host = checkBit;
          std::cout << "Host: " << host << std::endl;
          ReturnBoard(receiveMSG, board);

          std::cout << "MAIN BOARD" << std::endl;
          for(int i = 1; i < HEIGHT_SIZE; i++) {
            for(int j = 1; j < WIDTH_SIZE; j++) {
              std::cout << board[i][j] << " ";
            }bcd
            std::cout << std::endl;
          }
          // FIND IN GRAPH WITH DATA(host, board)
          //SetState_fromServer(board, host);
          // FILL THE CODE BELOW and return data to sendMSG
          // MSG FORM LIKE "BOARD|78 79"
          sprintf(sendMSG, "BORAD|Data From Graph Module > BOARD");
          write(_socket, sendMSG, strlen(sendMSG));
        }

        memset((void*)sendMSG, NULL, sizeof(sendMSG));
        memset((void*)receiveMSG, NULL, sizeof(receiveMSG));
        memset((void*)board, NULL, sizeof(char) * HEIGHT_SIZE * WIDTH_SIZE);
      }
      if(bytesread == -1) {
        std::cout << "Server Disconnected" << std::endl;
        close(_socket);
        exit(1);
      }
    }
  }
}
int CheckData(char* _received) {
  char *token;
  token = strtok(_received, "|");
  if(strcmp(token, "order") == 0) {
    return ORDERBIT;
  }
  else {
    return atoi(token);
  }
}
bool isOrderBit(int _bit) {
  if(_bit == ORDERBIT)
    return true;
  else
    return false;
}
void ReturnBoard(char* parsedData, char (&board)[HEIGHT_SIZE][WIDTH_SIZE]) {
  char *token;
  token = strtok(parsedData, "|");

  for(int i = 0; i < HEIGHT_SIZE; i++) {
    for(int j = 0; j < WIDTH_SIZE; j++) {
      token = strtok(NULL, ",");
      board[i][j] = token[0] ;
    }
  }
}
int ReturnHost(char* parsedData) {
  char *host;
  host = strtok(parsedData, "|");
  return atoi(host);
}
