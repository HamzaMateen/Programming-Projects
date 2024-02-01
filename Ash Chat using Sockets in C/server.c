#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include "util/util.h"
/*
 Warm-up Exercise
• Write two programs in C: hello_client and
hello_server
• The server listens for, and accepts, a single TCP connection; it reads all
the data it can from that connection, and prints it to the screen; then it
closes the connection
• The client connects to the server, sends the string “Hello, world!”, then
closes the connection.

Address to accept any incoming messages.
#define	INADDR_ANY		((in_addr_t) 0x00000000)
*/
#include "./lib/libbcrypt/bcrypt.h"

// TODO (Active): Implement Message Relay System

// handle connection with the client
// for now, only 9 simulataneous connections can be made!
// TODO: threads synchronization
typedef struct {
  int originator_fd;
  int peer_fd;
} conn_info;

void *client_handler(void *arg) {
  conn_info ci = *(conn_info *)arg;

  int *conn_fds = (int *)arg;
  char buffer[BUFFER_SIZE]; // this is the sign of scalability I had
                            // intended for but couldn't fathom. I love
                            // writing comments

  // session duration (5 mins for now);
  struct timeval session_duration;

  session_duration.tv_sec = 5 * MINUTE;
  session_duration.tv_usec = 0;

  int max_fd = ci.originator_fd > ci.peer_fd ? ci.originator_fd : ci.peer_fd;

  int bw, br;
  char msg_buffer[BUFFER_SIZE];

  fd_set read_set;
  // reading all the data from this connection
  while (1) {
    FD_ZERO(&read_set);
    FD_SET(ci.originator_fd, &read_set);
    FD_SET(ci.peer_fd, &read_set);

    int ready = select(max_fd + 1, &read_set, NULL, NULL, &session_duration);
    checkError(ready, "THREAD Select error");

    if (ready == 0) {
      printf("No conversation. Disconnecting ...\n");
      close(ci.originator_fd); // only close the originator of connection.
      break;
    }

    if (FD_ISSET(ci.originator_fd, &read_set)) {
      printf("ORIGINATOR\n");

      int br = read(ci.originator_fd, buffer, BUFFER_SIZE);
      checkError(br, "conn_fd read error");

      int bw = write(ci.peer_fd, buffer, br);
      checkError(bw, "conn_fd write error");
    }

    if (FD_ISSET(ci.peer_fd, &read_set)) {
      printf("PEER\n");

      int br = read(ci.peer_fd, buffer, BUFFER_SIZE);
      checkError(br, "conn_fd read error");

      int bw = write(ci.originator_fd, buffer, br);
      checkError(bw, "conn_fd write error");
    }
  }

  // close the connection, release memory
  close(ci.originator_fd);
  printf("connection with %d closed, disconnecting ...\n", ci.originator_fd);

  free(arg);
  return NULL;
}

// USER MANAGEMENT
// data
typedef struct {
  char *username;
  char *password;

  int user_fd;
} User;

// node (data container)
struct user_node {
  User user;
  struct user_node *next;
};

// list (collection of nodes)
struct user_node *head = NULL;
int user_count = 0;

// User Management Functions
struct user_node *create_user_node() {
  struct user_node *new_user =
      (struct user_node *)(malloc(sizeof(struct user_node)));

  new_user->user.username = NULL;
  new_user->user.password = NULL;
  new_user->user.user_fd = -1;

  return new_user;
}

void add_user(char *username, char *password, int fd) {
  struct user_node *new_user = create_user_node();
  if (new_user == NULL) {
    perror("Node allocation error");
    exit(EXIT_FAILURE);
  }

  char *d_username = (char *)malloc(sizeof(char) * strlen(username) + 1);
  if (d_username == NULL) {
    perror("Malloc username error");
    exit(EXIT_FAILURE);
  }
  strcpy(d_username, username);

  char *d_password = (char *)malloc(sizeof(char) * strlen(password) + 1);
  if (d_username == NULL) {
    perror("Malloc username error");
    exit(EXIT_FAILURE);
  }
  strcpy(d_password, password);

  new_user->user.user_fd = fd;
  new_user->user.username = d_username;
  new_user->user.password = d_password;

  new_user->next = head;
  head = new_user;
}

struct user_node *search_user(char *username) {
  // search the present list
  if (head == NULL)
    return NULL;

  struct user_node *temp_head = head;

  while (temp_head != NULL) {
    printf("temp: %ld, username: %ld\n", strlen(temp_head->user.username),
           strlen(username));
    if (0 == strcmp(temp_head->user.username, username)) {
      printf("Here!\n");
      return temp_head;
    }

    temp_head = temp_head->next;
  }

  return NULL;
}
// TODO: can add user deletion later

// handle client setup
void handle_first_msg(char *msg, conn_info *connection) {
  char *saveptr;
  char *action = strtok_r(msg, " ", &saveptr);
  char *username = strtok_r(NULL, " ", &saveptr);
  char *password = NULL; // Password is optional, depending on the action

  // convert new line or space into \0.
  printf("len: %ld\n", strlen(username));
  username[strlen(username)] = '\0';
  printf("len: %ld\n", strlen(username));

  if (action == NULL) {
    printf("Invalid command format.\n");
    return;
  }

  if (strcmp(action, "CONNECT") == 0) {
    if (username == NULL) {
      printf("Username is required for CONNECT.\n");
    } else {
      printf("Connect request for user: %s\n", username);

      // connect to the user
      struct user_node *user = search_user(username);
      if (user == NULL) {
        printf("User does not exist, hang on!\n");
      } else {
        // if it does, then create a conenction between these two
        connection->peer_fd = user->user.user_fd;
      }
    }
  } else if (strcmp(action, "REGISTER") == 0 || strcmp(action, "LOGIN") == 0) {
    password = strtok_r(NULL, " ", &saveptr);

    if (username == NULL || password == NULL) {
      printf("%s requires both username and password.\n", action);
    } else {
      if (strcmp(action, "REGISTER") == 0) {
        printf("Register request for user: %s with password: %s\n", username,
               password);
        // Handle register logic here
        char salt[BCRYPT_HASHSIZE];
        if (bcrypt_gensalt(WORK_FACTOR, salt) != 0) {
          perror("Salt generation error");
          exit(EXIT_FAILURE);
        };

        char hash[BCRYPT_HASHSIZE];
        if (bcrypt_hashpw(password, salt, hash) != 0) {
          perror("Hash generation error");
          exit(EXIT_FAILURE);
        }

        add_user(username, hash, connection->originator_fd);
        printf("User add succuess\n");

      } else { // LOGIN
        printf("Login request for user: %s with password: %s\n", username,
               password);
        // Handle login logic here
      }
    }
  } else {
    printf("Unknown action: %s\n", action);
    // Handle unknown action
  }
}

int main(int argc, char *argv[]) {

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(PORT); // htons = host (machine) to network, short

  // since this is the server, we would bind to any interface
  address.sin_addr.s_addr = INADDR_ANY; // accept any messages

  // zero out the sin_zero chars to pad this structure to size of sockaddr
  // structure.
  memset(address.sin_zero, '\0', sizeof(address.sin_zero));
  int server_sfd =
      socket(AF_INET, SOCK_STREAM, 0); // use default specific protocol

  if (bind(server_sfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind error");
    exit(EXIT_FAILURE);
  }

  // start listening for requests now
  if (listen(server_sfd, BACKLOG) < 0) {
    perror("Listen error");
    exit(EXIT_FAILURE);
  }

  // if success then print it to the console
  printf("Server listening on port %d\n", PORT);

  // will accept one connection and then end
  int conn_fd;
  char buffer[BUFFER_SIZE];
  int bw, br;

  while (1) { // can be converted to for loop w.r.t the client_index
    if ((conn_fd = accept(server_sfd, NULL, 0)) < 0) {
      perror("Accept error");
      exit(EXIT_FAILURE);
    }

    printf("Client with connfd %d connected!\n", conn_fd);

    // threads to manage connections and avoid blocking!
    char init_msg[BUFFER_SIZE];
    br = read(conn_fd, init_msg, BUFFER_SIZE);
    checkError(br, "Initial msg read error");

    conn_info *connection = malloc(sizeof(conn_info));
    if (connection == NULL) {
      perror("mallloc error");
      continue; // to next connection, don't crash server
    }

    // send msg to the handler to get fd of targeted user
    connection->originator_fd = conn_fd;
    handle_first_msg(init_msg, connection);

    // launch thread to handle this connection
    pthread_t thread_id;
    int ret_val =
        pthread_create(&thread_id, NULL, client_handler, (void *)connection);
    if (ret_val < 0) {
      perror("Could not create thread");
      free(connection);
      close(conn_fd);
      continue;
    }
  }

  return EXIT_SUCCESS;
}
