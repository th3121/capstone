#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

int main(int argc, char **argv)
{
    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    int err;
    char addr[19] = { 0 };
    char name[248] = { 0 };
    
    dev_id = hci_get_route(NULL);
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }

    len  = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 ) perror("hci_inquiry");
    
    
while(1){
    for (i = 0; i < num_rsp; i++) {
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name),
            name, 0) < 0)
        strcpy(name, "[unknown]");
        //printf("%s  %s\n", addr, name);
    }
    
    if(err==30)//이곳에 카메라 동작 트리거
    {
      printf("카메라동작");
    }
    else if(strcmp(addr,"74:E5:43:C3:B8:77")==0)//MAC 주소 확인하여 같다면 프린트.
    {
      printf("%s is driving\n",name);
    }
    else if(strcmp(addr,"74:E5:43:C3:B8:77")!=0) //주소값이 다르다면 확인되지 않은 기기
    {
      printf("undefined device\n"); 
      err++;
    }
    else if(strcmp(name,"[unknown]")==0)//주변기기가 탐색되지 않았을경우
    {
    err++;
    }
    
    
    }
    
    
    

    free( ii );
    close( sock );
    return 0;
}
