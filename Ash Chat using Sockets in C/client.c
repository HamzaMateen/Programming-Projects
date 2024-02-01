#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#include "util/util.h"

int main(int argc, char *argv[]) {

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(PORT);
  inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

  int client_sfd = socket(AF_INET, SOCK_STREAM, 0);
  checkError(client_sfd, "Socket error");

  // connect to server now
  int ret_val =
      connect(client_sfd, (struct sockaddr *)&address, sizeof(address));
  checkError(ret_val, "Connect error");

  // write data to the server now, I said right now

  // TCP is FULL-duplex so we have to monitor the client_sfd as well as terminal
  // input in order to get the text input from either side
  fd_set read_set;

  // session duration (5 mins for now);
  struct timeval session_duration;

  session_duration.tv_sec = 5 * MINUTE;
  session_duration.tv_usec = 0;

  int max_fd =
      STDIN_FILENO > client_sfd
          ? STDIN_FILENO
          : client_sfd; // for rare cases during redirections [irrelevant]

  int bw, br;
  char msg_buffer[BUFFER_SIZE];

  // assuming there are less than 1024 characters TODO
  while (1) { // infinite user connection, the user might disconnect via
    FD_ZERO(&read_set);
    FD_SET(STDIN_FILENO, &read_set);
    FD_SET(client_sfd, &read_set);

    int ready = select(max_fd + 1, &read_set, NULL, NULL, &session_duration);
    checkError(ready, "Select error");

    if (ready == 0) {
      printf("No conversation. Disconnecting ...\n");
      close(client_sfd);
      break;
    }

    if (FD_ISSET(STDIN_FILENO, &read_set)) {
      // msg from us
      // check if its a good bye
      br = read(STDIN_FILENO, msg_buffer, BUFFER_SIZE);
      checkError(br, "STDIN_FILENO read error");

      if (br == 0) {
        // server has closed the connection for some reason
        fprintf(stderr, "Server has closed the connection");
        close(client_sfd);
        break;
      }

      if (strncmp(msg_buffer, "Bye!", 4) == 0) {
        printf("\n\nDisconnecting ...\n");
        close(client_sfd);
        break;
      }

      // read the message from the client, which could be credentials as well
      // and send it over to the server
      // TODO: we will specify client id to send messages over
      // to a user
      bw = write(client_sfd, msg_buffer, br);
      checkError(bw, "client_sfd write error");
    }

    if (FD_ISSET(client_sfd, &read_set)) {
      // msg from peer TODO
      br = read(client_sfd, msg_buffer, BUFFER_SIZE);
      checkError(br, "client_sfd read error");

      // print to the console
      bw = write(STDOUT_FILENO, msg_buffer, br);
      checkError(bw, "STDOUT write error");
    }
  }
  exit(EXIT_SUCCESS);
}
