//code inspired by https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#define MAX 80
#define MAXCHAR 100000
#define PORT 8080 
#define SA struct sockaddr

int findIndex(char c, char arr[]) {
  for(int i = 0; i < sizeof(arr); i++) {
    printf("LOOKING FOR CHARACTER %c",c);
    printf("%c ", arr[i]);
    if(arr[i] == c) return i;
  }
  return -1;
}

// Function designed for chat between client and server. 
void func(int sockfd) 
{ 
	char buff[MAX]; 
	int n;
	char hangmanWord[20];
        char originalWord[20];
	char userEntry[MAX] = "";
	char incorrectGuesses[MAX] = "";
	int numIncorrect = 0;
        int numCorrect = 0;
	srand(time(0));
	int random = rand() % (15 + 1 - 0) + 0;
	
	FILE *fp;
        char str[MAXCHAR];
        char* filename = "hangman_words.txt";

        fp = fopen(filename, "r");
        if (fp == NULL){
         printf("Could not open file %s",filename);
         return 1;
         }

	int j = 0;
        while (fgets(str, MAXCHAR, fp) != NULL) {
        	if(j == random) {
			sprintf(hangmanWord, str); 	
		}
		printf("%s", str);
		j++;
	}
        fclose(fp);
	
	//strcat(hangmanWord, "\0");
	

	//sprintf(hangmanWord, "straight\0");
        sprintf(originalWord, hangmanWord);

	for(int i = 0; i < strlen(hangmanWord)-1; i++)
	{
		userEntry[ strlen(userEntry) ] = '_';
	}

	// infinite loop for chat
        
        sprintf(buff, "0%d%d%s%s", strlen(hangmanWord), numIncorrect, userEntry, incorrectGuesses);

	//sprintf(buff,userEntry);
	write(sockfd,buff, sizeof(buff));	
	//sprintf(buff, "Incorrect Guesses: %s\n", incorrectGuesses);
	//write(sockfd,buff,sizeof(buff));


        //printf("HANGMAN WORD IS %s", hangmanWord);
 	read(sockfd, buff, sizeof(buff));
        printf("HANGMAN WORD IS %s|", hangmanWord);

                // print buffer which contains the client contents
        printf("From client: %s \n", buff);
        //strncpy(hangmanWord, hangmanWord, strlen(hangmanWord)-2);

        //char* newLine = strchr(hangmanWord,13);
        //hangmanWord[ newLine - hangmanWord] = '*';

	for (;;) { 

		// read the message from client and copy it in buffer 
		read(sockfd, buff, sizeof(buff)); 
		// print buffer which contains the client contents 
		printf("client guess: %s \n", buff); 
	

		char* pPosition = strchr(hangmanWord, buff[0]);	
		
		if(pPosition)
		{


			//printf("NOT NULL INDEX IS %d IN WORD %s, and LENGTH IS %d\n", pPosition - hangmanWord,hangmanWord, strlen(hangmanWord));

			while(pPosition)
			{
				userEntry[pPosition-hangmanWord] = buff[0];
				hangmanWord[pPosition-hangmanWord] = '*';
				pPosition = strchr(hangmanWord, buff[0]);
                                numCorrect++;        
			}	
                        printf("TESTING0 userEntry: %s\n",userEntry); 

			//char* isWordComplete = strchr(userEntry, '_');
                        //printf("IS IT THIS LINE %d\n", isWordComplete-userEntry);
                        // int n = isWordComplete-hangmanWord;
                       // printf("n is %d\n",n);
                       //printf(strlen(isWordComplete)); 
                        printf("%s\n", userEntry);
			if(numCorrect == strlen(hangmanWord)-1)
                        {
                                //sprintf(buff,"TESTING\n");
                                int messageLength = 13 + strlen(hangmanWord);
				sprintf(buff,"%dThe word was %s", messageLength,userEntry);
                                write(sockfd, buff, sizeof(buff));
                                //sprintf(buff, "You Win!");
                                //write(sockfd, buff, sizeof(buff));

				sprintf(buff, "8You Win!");
                                write(sockfd, buff, sizeof(buff));
                                return;
			}

			/*for(int i = pPosition-hangmanWord; i < strlen(hangmanWord)-1; i++)
			{
				userEntry[pPosition-hangmanWord] = buff[0];
				//printf(hangmanWord[i]);

			}*/
		}else
		{
			incorrectGuesses[strlen(incorrectGuesses)] = buff[0];
			numIncorrect++;
			if(numIncorrect > 5)
			{
				int messageLength = 13 + strlen(hangmanWord);
				sprintf(buff, "%dThe word was %s", messageLength, originalWord);
				write(sockfd, buff, sizeof(buff));
		         	sprintf(buff, "You Lose!");
				write(sockfd, buff, sizeof(buff));
				return;

			}
		}	


		//THIS IS WHAT THE MESSAGE FORMAT SHOULD LOOK LIKE RELATIVELY 
		sprintf(buff, "0%d%d%s%s", strlen(hangmanWord)-1, numIncorrect, userEntry, incorrectGuesses);
		write(sockfd,buff,sizeof(buff));
	
		//sprintf(buff,userEntry);
		//write(sockfd, buff, sizeof(buff));

		//sprintf(buff, "Incorrect Guesses: %s", incorrectGuesses);
		//write(sockfd, buff, sizeof(buff));
		continue;
/*
		int sum = 10;
                int flag = 0;
                while(sum >= 10)
                {
                        sum = 0;
                        for(int i = 0; i < strlen(buff); i++)
                        {
                                if(buff[i] >= '0' && buff[i] <= '9')
                                { sum = sum + (int)(buff[i] - '0'); }
                                else if ( (buff[i] >= 32 && buff[i] <= 47) || (buff[i] >= 58 && buff[i] <= 127))
                                {
                                        sprintf(buff, "Sorry can't compute", "Sorry can't compute");
                                        //sendto(udpSocket, buff, strlen(buff)+1, 0, (struct sockaddr *)&serverStorage, addr_size);
                                        write(sockfd, buff, sizeof(buff));
					flag = 1;
                                        break;
                                }

                        }
                        if(flag == 1) break;
                        sprintf(buff, "%d",sum);
                        //sendto(udpSocket, buff, nBytes, 0, (struct sockaddr *)&serverStorage, addr_size);
			write(sockfd,buff, sizeof(buff));
                }
				
*/

	} 
} 

// Driver function 
int main() 
{ 
	int sockfd, connfd, len, master_socket, max_clients = 30, client_socket[30]; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	//initialise all client_socket[] to 0 so not checked
    	for (i = 0; i < max_clients; i++)
    	{
        	client_socket[i] = 0;
    	}

	//create a master socket
    	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    	{
        	perror("socket failed");
        	exit(EXIT_FAILURE);
    	}

	//set master socket to allow multiple connections
    	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
          sizeof(opt)) < 0 )
    	{
        	perror("setsockopt");
        	exit(EXIT_FAILURE);
    	}

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server acccept the client...\n"); 
		
		char buff[MAX];
		sprintf(buff, "Ready to start game? (y/n):", "Ready to start game? (y/n):");
                write(connfd,buff, sizeof(buff));



	// Function for chatting between client and server 
	func(connfd); 

	// After chatting close the socket 
	close(sockfd); 
} 

