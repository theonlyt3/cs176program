//code inspired by https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
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
        int opt = 1; 
	int i, addrlen, new_socket, sockfd, connfd, len, master_socket, max_clients = 3, client_socket[3]; 
	struct sockaddr_in servaddr, cli; 

        fd_set readfds;
        int max_sd, sd, activity;

        char *message = "ECHO Daemon v1.0 \r\n";


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
    	if( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
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

        addrlen = sizeof(servaddr);

        while(1)   
    {  
        //clear the socket set  
        FD_ZERO(&readfds);   
     
        //add master socket to set  
        FD_SET(sockfd, &readfds);   
        max_sd = sockfd;   
             
        //add child sockets to set  
        for ( i = 0 ; i < max_clients ; i++)   
        {   
            //socket descriptor  
            sd = client_socket[i];   
                 
            //if valid socket descriptor then add to read list  
            if(sd > 0)   
                FD_SET( sd , &readfds);   
                 
            //highest file descriptor number, need it for the select function  
            if(sd > max_sd)   
                max_sd = sd;   
        }   
     
        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely  
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR))   
        {   
            printf("select error");   
        }   
             
        //If something happened on the master socket ,  
        //then its an incoming connection  
        if (FD_ISSET(sockfd, &readfds))   
        {  
            if ((new_socket = accept(sockfd,  
                    (struct sockaddr *)&servaddr, (socklen_t*)&addrlen))<0)   
            {
                perror("accept");   
                exit(EXIT_FAILURE);   
            }   
            //inform user of socket number - used in send and receive commands  
            //printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(servaddr.sin_addr) , ntohs (servaddr.sin_port));   
            //send new connection greeting message  
            
            char buff[MAX];
          sprintf(buff, "Ready to start game? (y/n):", "Ready to start game? (y/n):");
            write(new_socket,buff, sizeof(buff));
            
            func(new_socket);
            
            /*if( send(new_socket, message, strlen(message), 0) != strlen(message) )   
            {   
                perror("send");   
            }*/   
                 
            //puts("Welcome message sent successfully");   
            //add new socket to array of sockets  
            for (i = 0; i < max_clients; i++)   
            {   

                //if position is empty  
                if( client_socket[i] == 0 )   
                {   
                    client_socket[i] = new_socket;   
                    printf("Adding to list of sockets as %d\n" , i);   
                    break;   
                }   
            }
            if( i == 2)
              break;   
        } 

    }

}
