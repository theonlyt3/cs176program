// code inspired by https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

void prepend(char* s, const char* t)
{
    size_t len = strlen(t);
    size_t i;

    memmove(s + len, s, strlen(s) + 1);

    for (i = 0; i < len; ++i)
    {
        s[i] = t[i];
    }
}

void func(int sockfd) 
{ 
	char buff[MAX]; 
	int n; 
	int nBytes;
	char response[MAX];
 	
	read(sockfd,buff,sizeof(buff));
        if(strlen(buff) > 0)
        {
              printf("%s\n", buff);
        }
	fgets(buff, 1024, stdin);
	
	if( buff[0] == 'n')
	{
		return;
	}else if ( buff[0] == 'y' )
	{
		//printf("Success");
		sprintf(buff, "0","0"); 
		write(sockfd, buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));

	}

	for (;;) { 

                if(buff[0] == '0')
                {
                  int lengthOfWord = buff[1] - '0';
                  int numIncorrect = buff[2] - '0';


                  //printf("%s, length of word is %d",buff,lengthOfWord);
                  
                printf(">>> "); 
                for(int i = 3; i < lengthOfWord+3; i++) {
                        printf("%c ", buff[i]);
                } 
                printf("\n");
                //read(sockfd, buff, sizeof(buff));
                printf(">>> Incorrect Guesses: ");
                for(int i = 3 + lengthOfWord; i < strlen(buff); i++)
                {
                    printf("%c,",buff[i]);
                }
                  printf("\n");



		printf("Letter to guess: ");
		fgets(buff, 1024, stdin);
		char holder[MAX];
		sprintf(holder,buff);
		//printf("Holder is %s, sizeof(buff) is %d, strlen(buff) is %d",holder,sizeof(buff),strlen(buff));
		//sprintf(buff, strlen(buff) - '0'); // CAUSES SEGMENTATION FAULT
		//strcat(holder,buff);
		

		//nBytes = strlen(buffer) + 1;

		write(sockfd, buff, sizeof(buff));
		read(sockfd,buff,sizeof(buff));
		
                }else
                {
                  for(int i = 0; i < strlen(buff); i++)
                  {
                    if(buff[i] >= '0' && buff[i] <= '9')
                      continue;

                    printf("%c",buff[i]);

                  }
                  printf("\n");
                    //printf("buff:%s\n",buff);
                  read(sockfd,buff,sizeof(buff));
                  for(int i = 0; i < strlen(buff); i++) {
                    if(buff[i] >= '0' && buff[i] <= '9')
                      continue;
                    printf("%c",buff[i]);

                  }
		 
                  printf("\n");

                  printf("Game Over!\n");
                  break;
                }
	} 
} 

int main() 
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 

	// socket create and varification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(PORT); 

	// connect the client socket to server socket 
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server..\n"); 

	// function for chat 
	func(sockfd); 

	// close the socket 
	close(sockfd); 
} 

