// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <netinet/tcp.h>
#include <math.h>
#include <stdlib.h>
#define PORT 3780 
#define PORT_NFQ 3770
#include <time.h>

int main(int argc, char const *argv[]) 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 

    char *hello = "We apply two primary approaches to our applications. First, the statistical approach inwhich we manually extract the traffic features to obtains sensitive information. Second,we use DNN-based methods to find intrinsic features of the flow automatically. One ofthe advantages of the DNN-based technique is that it eliminates the need for an expert toselect the features because it automatically extracts the features through training. Also,deep learning is useful when the pattern of old classes change, or new types of trafficemergeWe apply two primary approaches to our applications. First, the statistical approach inwhich we manually extract the traffic features to obtains sensitive information. Second,we use DNN-based methods to find intrinsic features of the flow automatically. One ofthe advantages of the DNN-based technique is that it eliminates the need for an expert toselect the features because it automatically extracts the features through training. Also,deep learning is useful when the pattern of old classes change, or new types of trafficemergeWe apply two primary approaches to our applications. First, the statistical approach inwhich we manually extract the traffic features to obtains sensitive information. Second,we use DNN-based methods to find intrinsic features of the flow automatically. One ofthe advantages of the DN";
    char buffer[1024] = {0}; 
double arr[] = {1,2,3};
int flags =1;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   //setsockopt(sock, SOL_TCP, TCP_NODELAY, (void *)&flags, sizeof(flags));

    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "0.0.0.0", &serv_addr.sin_addr) <=0 ) // ip address of digital ocean 165.22.216.182
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    

    /////server///////////
    int server_fd, new_socket, valread2; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer2[1024] = {0}; 

       
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
    address.sin_port = htons( PORT_NFQ ); 
       
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
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 

     ///////////Server/////////////////
    	for (int i = 0; i<5; i++){
        send(sock , hello , strlen(hello) , 0 ); 
           printf("client hello sent \n"); 
	valread2 = read( new_socket , buffer2, 1024); 
	           printf("server hello read \n"); 
       //send(new_socket , hello , strlen(hello) , 0 ); 
   }
    return 0; 
} 

/*

 //for (int i = 0; i<440; i++){
    

    printf("Hello message sent  %d\n",i); 
 // valread = read( sock , buffer, 1024); 
    //float t = floor(100000000*0.001*arr[i])/100000000; 
    //printf("%s %d    %f\n",buffer ,i,t); 
     usleep(1000*1000);//input is in microseconds
   // }

*/
