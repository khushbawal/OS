#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>
#include <semaphore.h>

/** Port number used by my server */
#define PORT_NUMBER "26384"

/** Maximum command line length */
#define N 256
char game[100][100];
int r, c;
sem_t mutex;

/** Print out an error message and exit. */
static void fail(char const *message)
{
  fprintf(stderr, "%s\n", message);
  exit(EXIT_FAILURE);
}

/** handle a client connection, close it when we're done. */
void *handleClient(void *sock)
{
  int *sock1 = (int *)sock;
  // Here's a nice trick, wrap a C standard IO FILE around the
  // socket, so we can communicate the same way we would read/write
  // a file.
  FILE *fp = fdopen(*sock1, "a+");

  // Prompt the user for a command.
  fprintf(fp, "cmd> ");

  // Temporary values for parsing commands.
  char cmd[N];
  bool flag = true;

  while (fscanf(fp, "%s", cmd) == 1 && strncmp(cmd, "quit", 4) != 0)
  {
    // Just echo the command back to the client for now.
    if (!strcmp(cmd, "pick"))
    {
      int r1, c1, ar, ac, j = 0;
      int nwords = 0;
      char ch, clientchar;
      char str[N - 6], rstr[3], cstr[3], word[3];
      int i = 0;
      // Limit 250 for buffer.
      do
      {
        if (i > N - 6)
        {
          break;
        }
        fscanf(fp, "%c", str + i);
        // printf("character :%c\n",str[i]);
      } while (str[i++] != '\n');
      if (i == N - 6)
      {
        fprintf(fp, "%s\n", "Invalid command0");
        fprintf(fp, "cmd>");
        continue;
      }
      else
      {
        str[i] = '\0';
      }
      // fscanf(fp, "%[^\n]%*c", str);
      // printf("string:%s\n", str);
      for (int i = 0; str[i] != '\0'; i++)
      {
        if (nwords == 0 && str[i] == ' ')
        {
          nwords++;
          // printf("nwords1:%d\n", nwords);
          if (str[i + 1] == ' ')
            break;
        }
        else if (nwords == 1)
        {
          // printf("character:%c\n", str[i]);
          if (str[i] == ' ')
          {
            nwords++;
            // printf("nwords2:%d\n", nwords);
            rstr[j++] = '\0';
            j = 0;
          }
          else if (isdigit(str[i]))
          {
            rstr[j++] = str[i];
          }
          else
          {
            break;
          }
        }
        else if (nwords == 2)
        {
          if (str[i] == ' ')
          {
            nwords++;
            // printf("nwords3:%d\n", nwords);
            cstr[j++] = '\0';
            if (str[i + 1] == ' ')
              break;
            j = 0;
          }
          else if (isdigit(str[i]))
          {
            cstr[j++] = str[i];
          }
          else
          {
            break;
          }
        }
        else if (nwords == 3)
        {
          // printf("character:%c\n", str[i]);
          word[j++] = str[i];
          nwords++;
          // printf("nwords4:%d\n", nwords);
          word[j] = '\0';
          break;
        }
        else
        {
          break;
        }
      }
      // printf("nwords5:%d\n", nwords);
      if (nwords != 4)
      {
        fprintf(fp, "%s\n", "Invalid command1");
      }
      else
      {
        r1 = atoi(rstr);
        c1 = atoi(cstr);
        ch = word[0];
        // printf("r1:%d c1:%d ch:%c \n", r1, c1, ch);
        if (r1 >= r || c1 >= c)
        {
          fprintf(fp, "%s\n", "Invalid command2");
        }
        else if (isalpha(game[r1][c1]))
        {
          fprintf(fp, "%s\n", "Invalid command3");
        }
        else if (isalpha(ch))
        {
          if (flag == true)
          {
            sem_wait(&mutex);
            game[r1][c1] = ch;
            sem_post(&mutex);
            flag = false;
            ar = r1;
            ac = c1;
            clientchar = ch;
          }
          else if (ch != clientchar)
          {
            fprintf(fp, "%s\n", "Invalid command4");
          }
          else if ((r1 == ar - 1 && c1 == ac) || (r1 == ar && c1 == ac - 1) || (r1 == ar && c1 == ac + 1) || (r1 == ar + 1 && c1 == ac))
          {
            sem_wait(&mutex);
            game[r1][c1] = ch;
            sem_post(&mutex);
            flag = false;
            ar = r1;
            ac = c1;
          }
          else
          {
            fprintf(fp, "%s\n", "Invalid command5");
          }
        }
        else
        {
          fprintf(fp, "%s\n", "Invalid command6");
        }
      }
    }
    else if (!strcmp(cmd, "board"))
    {
      for (int k = 1; k < c + 3; k++)
      {
        if (k == 1 || k == c + 2)
          fprintf(fp, "%c", '+');
        else
          fprintf(fp, "%c", '-');
      }
      fprintf(fp, "%c", '\n');
      for (int i = 0; i < r; i++)
      {
        fprintf(fp, "%c", '|');
        for (int j = 0; j < c; j++)
        {
          fprintf(fp, "%c", game[i][j]);
        }
        fprintf(fp, "%c", '|');
        fprintf(fp, "%c", '\n');
      }
      for (int k = 1; k < c + 3; k++)
      {
        if (k == 1 || k == c + 2)
          fprintf(fp, "%c", '+');
        else
          fprintf(fp, "%c", '-');
      }
      fprintf(fp, "%c", '\n');
    }
    else
    {
      fprintf(fp, "Invalid Command\n");
    }

    // Prompt the user for the next command.
    fprintf(fp, "cmd> ");
  }

  // Close the connection with this client.
  fclose(fp);
  return NULL;
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    fail("less no.of arguments\n");
    exit(1);
  }
  int i, j;
  sem_init(&mutex, 0, 1);
  r = atoi(argv[1]);
  c = atoi(argv[2]);
  sem_wait(&mutex);
  for (i = 0; i < r; i++)
  {
    for (j = 0; j < c; j++)
    {
      game[i][j] = ' ';
    }
  }
  sem_post(&mutex);

  // Prepare a description of server address criteria.
  pthread_t thread;
  struct addrinfo addrCriteria;
  memset(&addrCriteria, 0, sizeof(addrCriteria));
  addrCriteria.ai_family = AF_INET;
  addrCriteria.ai_flags = AI_PASSIVE;
  addrCriteria.ai_socktype = SOCK_STREAM;
  addrCriteria.ai_protocol = IPPROTO_TCP;

  // Lookup a list of matching addresses
  struct addrinfo *servAddr;
  if (getaddrinfo(NULL, PORT_NUMBER, &addrCriteria, &servAddr))
    fail("Can't get address info");

  // Try to just use the first one.
  if (servAddr == NULL)
    fail("Can't get address");

  // Create a TCP socket
  int servSock = socket(servAddr->ai_family, servAddr->ai_socktype,
                        servAddr->ai_protocol);
  if (servSock < 0)
    fail("Can't create socket");

  // Bind to the local address
  if (bind(servSock, servAddr->ai_addr, servAddr->ai_addrlen) != 0)
    fail("Can't bind socket");

  // Tell the socket to listen for incoming connections.
  if (listen(servSock, 5) != 0)
    fail("Can't listen on socket");

  // Free address list allocated by getaddrinfo()
  freeaddrinfo(servAddr);

  // Fields for accepting a client connection.
  struct sockaddr_storage clntAddr; // Client address
  socklen_t clntAddrLen = sizeof(clntAddr);

  while (true)
  {
    // Accept a client connection.
    int sock = accept(servSock, (struct sockaddr *)&clntAddr, &clntAddrLen);
    pthread_create(&thread, NULL, handleClient, &sock);
  }

  // Stop accepting client connections (never reached).
  close(servSock);

  return 0;
}
