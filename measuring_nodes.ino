#include <WiFi.h>
#include <esp_now.h>

const int total=16;
bool found=false;
bool check=false;
int n;
int num=7;
bool receive=false;
const char* boosted="boost";
const char* tran="transfer";
const char* checkmsg="check";
const char* pass="53732394";
uint8_t base[]={0xB0,0xA7,0x32,0x2A,0x68,0x6C};
const char* emitterlist[]={"node1", "node2", "node3", "node4", "node5", "node6", "node7", "node8", "node9", "node10", "node11", "node12", "node13", "node14", "node15", "node16"};
int size=total-1;
const char* ssidlist[total - 1];

typedef struct packet{
  char info[32];
  uint8_t value[total-1];
} packet;

packet datapack,sendpack,dummy;
esp_now_peer_info_t peerinfo;

void ondatarecv(const uint8_t* mac,const uint8_t* incomingdata,int len){
  memcpy(&dummy,incomingdata,sizeof(dummy));
  if(strcmp(dummy.info,checkmsg)==0) check=true;
  else{
    receive=true;
  }
}

void setup() {
  WiFi.mode(WIFI_AP_STA);
  Serial.begin(115200);
  esp_now_init();
  esp_now_register_recv_cb(ondatarecv);
  memcpy(peerinfo.peer_addr,base,6);
  peerinfo.channel=0;
  peerinfo.encrypt=false;
  esp_now_add_peer(&peerinfo);
  for(int i=0,j=0;i<total;i++){
    if(i==num-1) continue;
    else ssidlist[j++]=emitterlist[i];
  }
  strcpy(sendpack.info,boosted);
  strcpy(datapack.info,tran);
  esp_now_send(base,(uint8_t*) &sendpack,sizeof(sendpack));
  WiFi.softAP(emitterlist[num-1],pass,1);
}

void loop() {
  if(receive){
    n=WiFi.scanNetworks(false,false,false,500,1);
    int tim=0;
    while(tim<size){
      for(int i=0;i<n;i++){
        if(strcmp(WiFi.SSID(i).c_str(),ssidlist[tim])==0){
          datapack.value[tim++]=(-WiFi.RSSI(i));
          found=true;
          break;
        }
      }
      if(found) found=false;
      else n=WiFi.scanNetworks(false,false,false,500,1);
    }
    tim=0;
    while(!check){
      delay(100);
    };
    WiFi.enableAP(false);
    esp_now_send(base,(uint8_t*) &datapack,sizeof(datapack));
    receive=false;
    check=false;
    WiFi.enableAP(true);
  }
}
