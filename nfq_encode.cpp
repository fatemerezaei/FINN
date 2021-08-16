#include <iostream>
#include <cstdlib>
#include <ctime>

#include <stdint.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <netinet/tcp.h>
#include <math.h>
#include <stdlib.h>
#define PORT 3790//3780 
#define PORT_NFQ 3760//3770
#include <time.h>
#include <string>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include<sys/time.h>
#include <netinet/in.h>
#include <linux/types.h>
using namespace std;
#include <time.h>
#include <unistd.h>
#include <experimental/filesystem>
#include <chrono>
#include <ctime> 
namespace fs = std::experimental::filesystem;
extern "C" {
#include <linux/netfilter.h>
#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nfnetlink_queue.h>
#include <libnetfilter_queue/libnetfilter_queue.h>
}typedef unsigned char BYTE;
/*
if this code did not work, I should consider calling the function insertFinger in cb too.
*/ 


std::vector<double> readFromFile(const std::string address);
int insertFinger();
static int cb(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
        struct nfq_data *nfa, void *data);

static u_int32_t prinft_pkt(struct nfq_data *tb);
using std::cout;
std::vector<std::vector<double>> all_finger;
std::vector<double> fingerprint;
std::string path = "/home/fatemeh/MyProjects/Fingerprint/2020-05-05/ipd_fing_enc10/";

long int previousTime = 0;
int count = 0;
int flow_number = 0;
int chunk = 0;
int sock = socket(AF_INET, SOCK_STREAM, 0);
char buffer[1024] = {0};
int new_socket=0;
int main(void) {
 
    struct nfq_handle *h;
    struct nfq_q_handle *qh;
    struct nfnl_handle *nh;

    int fd;
    int rv;

    char buf[4096] __attribute__ ((aligned));

   // reading first 10 fing of first flow( flow_number = 0)
   fingerprint = readFromFile(path + std::to_string(flow_number) + "/"+ std::to_string(0) + ".txt");
   all_finger.push_back(fingerprint);

 //connect to the encoder
 struct sockaddr_in serv_addr; 
     


    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
    inet_pton(AF_INET, "0.0.0.0", &serv_addr.sin_addr);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    

    /////server///////////
    int valread; 
    struct sockaddr_in address; 
    int opt = 1; 


    int addrlen = sizeof(address);
    // Creating socket file descriptor 
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
       

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)); 
   
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT_NFQ ); 
    bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address));
   
    listen(server_fd, 3);
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    
     //connect to the encoder
  
   fingerprint = all_finger[flow_number];
   printf("Number of fingerprint arrays: %ld  \n", all_finger.size());
    
    struct timeval tr;
    gettimeofday(&tr, NULL);
    previousTime = tr.tv_sec * 1000 + tr.tv_usec / 1000; 
  
    h = nfq_open();
    //create q number 0
    qh = nfq_create_queue(h, 0, &cb, NULL);
 
    if (!qh) {
        fprintf(stderr, "error during nfq_create_queue()\n");

        //exit(1);
    }
 
    printf("setting copy_packet mode\n");
    if (nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff) < 0) {
        fprintf(stderr, "can't set packet_copy mode\n");
        //exit(1);
    }
 
    fd = nfq_fd(h);
    count += 1;
 
    while ((rv = recv(fd, buf, sizeof(buf), 0)) && rv >= 0) {
        nfq_handle_packet(h, buf, rv);
    }
 
    printf("unbinding from queue 0\n");
    nfq_destroy_queue(qh);
 
#ifdef INSANE
    /* normally, applications SHOULD NOT issue this command, since
     * it detaches other programs/sockets from AF_INET, too ! */
    printf("unbinding from AF_INET\n");
    nfq_unbind_pf(h, AF_INET);
#endif
 
    printf("closing library handle\n");
    nfq_close(h);
 
 
    return 0;
 
}
static u_int32_t prinft_pkt(struct nfq_data *tb) {
    int id = 0;
    struct nfqnl_msg_packet_hdr *ph;
    struct nfqnl_msg_packet_hw *hwph;
    u_int32_t mark, ifi;
    int ret;
    unsigned char *data;
 
    ph = nfq_get_msg_packet_hdr(tb);

    if (ph) {
        id = ntohl(ph->packet_id);
    }
 
    hwph = nfq_get_packet_hw(tb);
    ret = nfq_get_payload(tb, &data);

    fputc('\n', stdout);
 
    return id;
}

static int cb(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
        struct nfq_data *nfa, void *data) {
    
    u_int32_t id = prinft_pkt(nfa);
    insertFinger();
    // this should remain 0. if changed to 1, it does not work
    return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
}

int insertFinger() {

    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    long double currentTime = ms;

    int timeBetweenTwoFlows = 20000;
    if (ms - previousTime  > timeBetweenTwoFlows){
      
      count = 0;
      flow_number += 1;
      fingerprint = readFromFile(path + std::to_string(flow_number) + "/"+ std::to_string(0) + ".txt");
      all_finger.push_back(fingerprint);
      printf("New flow %d started! \n", flow_number);
    } 
    else if (count % 10 == 0){
        chunk +=1;
    	fingerprint = readFromFile(path + std::to_string(flow_number) + "/"+ std::to_string(chunk) + ".txt");
        all_finger.push_back(fingerprint);
        count = 0;
    
    }
      
    double ipd = ms - currentTime;
    double disPrevPkt = currentTime - previousTime;
    char ipd_send[50]={0};
    snprintf(ipd_send, 50, "%f  %d", disPrevPkt, count);
    //ask the encoder:
    send(sock , ipd_send , strlen(ipd_send) , 0 ); 
    printf("client hello sent \n"); 
    int valread = read( new_socket , buffer, 1024); 
	           printf("server hello read %s   %d\n",buffer, count); 
    
    //finish asking
    
    
    struct timeval tpr;
    gettimeofday(&tpr, NULL);
    currentTime = tpr.tv_sec * 1000.0 + tpr.tv_usec / 1000.0;
    
    previousTime =  currentTime; //Is it OK, or I should compute time again?

    count += 1;
   return 1;
}

std::vector<double> readFromFile(const std::string address ) {
    std::ifstream in(address);
    double num;
    std::vector<double> result;
    std::vector<double> resultStr;

    while (in >> num) {
        result.push_back(num);
    }

    return result;
}
