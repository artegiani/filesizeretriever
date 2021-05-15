// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <malloc.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
   
    char *fnf = "File not Found";
    FILE * fp;
    char buff[1024]={0};
	int i=0;
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    //to keep the socket up and listening
    while(1){
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    	char * welcome = "Welcome to Remote File Size Retriever, Please Insert File Path:\n";
	char * insert_length = "\n Insert File Path Character Length\n";
	char * come_back= "\nPlease, Feel Free to Come Back Again!\n";
	
	send(new_socket, welcome, strlen(welcome),0);
	//resetting the buffers each loop to avoid segmentation fault
  	char appoggio[1024]={0};
       char buffer[1024] = {0};
	char alength[1024]={0};       
       //receive input from client
       if( recv(new_socket, buffer, 1024, 0)){
	printf("%s\n", buffer);
	//deleting the extra character
	for(int j=0;j<strlen(buffer)-1;j++){
	appoggio[j]=buffer[j];
	}
	char buffer2[1024] = {0};
	 send(new_socket,insert_length,strlen(insert_length),0);
         int length;
	  recv(new_socket, buffer2, strlen(buffer),0);
	
	 for(int j=0;j<strlen(buffer2)-1;j++){
        	alength[j]=buffer2[j];
       	 }

	length =atoi(alength);
	printf("%d",length);
 	

	if (length>1024){
	length =1024;
       	} 
	//fopen with control over the existence of the file
	if(fp = fopen(appoggio, "r")){
		char * ff = "\nFile Found!\n";
		send(new_socket,ff,strlen(ff),0);
		for(int j=0;j<strlen(appoggio);j++){
			buff[j]=appoggio[j];
		}
		fread(&buff[strlen(appoggio)],sizeof(char),sizeof(buff),fp);
		char appoggio2[1024]={0};
		for(int j =0; j<length;j++){
			appoggio2[j]=buff[j];
		}
			send(new_socket,appoggio2,strlen(appoggio2),0);
			
			send(new_socket,"\nNumero Caratteri: ",strlen("\nNumero Caratteri: "),0);
			fseek(fp, 0L, SEEK_END);
  
    // calculating the size of the file
    			int res = ftell(fp);
			char * sender = {0};
			itoa (res, sender);
			printf("%c", sender);
			send(new_socket, &sender, strlen(&sender),0);
        fclose(fp);
	}
	else{
	send(new_socket, fnf, strlen(fnf),0);	
	}
       }
	send(new_socket, come_back, strlen(come_back),0);
	close(new_socket);
    }
		
		return 0;
}



char * itoa (int n, char *s){
	int i,sign;
	if((sign = n)<0)
		n=-n;
	i=0;
	do{
		s[i++] = n % 10 + '0';
	} while ((n/=10)>0);
	if (sign < 0)
		s[i++]= '-';
	s[i]='\0';
	reverse(s);

	return(s);
}


