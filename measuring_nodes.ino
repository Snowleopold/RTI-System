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
const char* ssid1="node1";
const char* ssid2="node2";
const char* ssid3="node3";
const char* ssid4="node4";
const char* ssid5="node5";
const char* ssid6="node6";
const char* ssid7="node7";
const char* ssid8="node8";
const char* ssid9="node9";
const char* ssid10="node10";
const char* ssid11="node11";
const char* ssid12="node12";
const char* ssid13="node13";
const char* ssid14="node14";
const char* ssid15="node15";
const char* ssid16="node16";
const char* pass="53732394";
uint8_t base[]={0xB0,0xA7,0x32,0x2A,0x68,0x6C};
const char* emitterlist[]={ssid1,ssid2,ssid3,ssid4,ssid5,ssid6,ssid7,ssid8,ssid9,ssid10,ssid11,ssid12,ssid13,ssid14,ssid15,ssid16};
int size=total-1;
const char** ssidlist;
const char* route1[]={ssid2,ssid3,ssid4,ssid5,ssid6,ssid7,ssid8,ssid9,ssid10,ssid11,ssid12,ssid13,ssid14,ssid15,ssid16};
const char* route2[]={ssid1,ssid3,ssid4,ssid5,ssid6,ssid7,ssid8,ssid9,ssid10,ssid11,ssid12,ssid13,ssid14,ssid15,ssid16};
const char* route3[]={ssid1,ssid2,ssid4,ssid5,ssid6,ssid7,ssid8,ssid9,ssid10,ssid11,ssid12,ssid13,ssid14,ssid15,ssid16};
const char* route4[]={ssid1,ssid2,ssid3,ssid5,ssid6,ssid7,ssid8,ssid9,ssid10,ssid11,ssid12,ssid13,ssid14,ssid15,ssid16};
const char* route5[]={ssid1,ssid2,ssid3,ssid4,ssid6,ssid7,ssid8,ssid9,ssid10,ssid11,ssid12,ssid13,ssid14,ssid15,ssid16};
const char* route6[]={ssid1,ssid2,ssid3,ssid4,ssid5,ssid7,ssid8,ssid9,ssid10,ssid11,ssid12,ssid13,ssid14,ssid15,ssid16};
const char* route7[]={ssid1,ssid2,ssid3,ssid4,ssid5,ssid6,ssid8,ssid9,ssid10,ssid11,ssid12,ssid13,ssid14,ssid15,ssid16};
const char* route8[]={ssid1,ssid2,ssid3,ssid4,ssid5,ssid6,ssid7,ssid9,ssid10,ssid11,ssid12,ssid13,ssid14,ssid15,ssid16};
const char* route9[]={ssid1,ssid2,ssid3,ssid4,ssid5,ssid6,ssid7,ssid8,ssid10,ssid11,ssid12,ssid13,ssid14,ssid15,ssid16};
const char* route10[]={ssid1,ssid2,ssid3,ssid4,ssid5,ssid6,ssid7,ssid8,ssid9,ssid11,ssid12,ssid13,ssid14,ssid15,ssid16};
const char* route11[]={ssid1,ssid2,ssid3,ssid4,ssid5,ssid6,ssid7,ssid8,ssid9,ssid10,ssid12,ssid13,ssid14,ssid15,ssid16};
const char* route12[]={ssid1,ssid2,ssid3,ssid4,ssid5,ssid6,ssid7,ssid8,ssid9,ssid10,ssid11,ssid13,ssid14,ssid15,ssid16};
const char* route13[]={ssid1,ssid2,ssid3,ssid4,ssid5,ssid6,ssid7,ssid8,ssid9,ssid10,ssid11,ssid12,ssid14,ssid15,ssid16};
const char* route14[]={ssid1,ssid2,ssid3,ssid4,ssid5,ssid6,ssid7,ssid8,ssid9,ssid10,ssid11,ssid12,ssid13,ssid15,ssid16};
const char* route15[]={ssid1,ssid2,ssid3,ssid4,ssid5,ssid6,ssid7,ssid8,ssid9,ssid10,ssid11,ssid12,ssid13,ssid14,ssid16};
const char* route16[]={ssid1,ssid2,ssid3,ssid4,ssid5,ssid6,ssid7,ssid8,ssid9,ssid10,ssid11,ssid12,ssid13,ssid14,ssid15};
const char** routelist[]={route1,route2,route3,route4,route5,route6,route7,route8,route9,route10,route11,route12,route13,route14,route15,route16};

typedef struct packet{
  char info[32];
  short value[total-1];
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
  ssidlist=routelist[num-1];
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
          datapack.value[tim++]=WiFi.RSSI(i);
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
