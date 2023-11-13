#include <WiFi.h>
#include <esp_now.h>

const int TIMEOUT_LIMIT=5000;
int done=0;
int nodenum;
const int total=16;
int count=0;
int expect;
int t1,t2;
const char* boosted="boost";
const char* tran="transfer";
const char* win1="Station ";
const char* win2=" boosted!";
const char* checkmsg="check";
uint8_t emitter1[]={0xA8,0x42,0xE3,0xCA,0x89,0xAC};
uint8_t emitter2[]={0xB0,0xA7,0x32,0xF3,0x96,0x68};
uint8_t emitter3[]={0xB0,0xA7,0x32,0x29,0xF0,0x50};
uint8_t emitter4[]={0xA8,0x42,0xE3,0xCA,0x23,0xBC};
uint8_t emitter5[]={0xA8,0x42,0xE3,0xCB,0x78,0x14};
uint8_t emitter6[]={0xA8,0x42,0xE3,0xCB,0x99,0xF4};
uint8_t emitter7[]={0xB0,0xA7,0x32,0xF2,0xEE,0x14};
uint8_t emitter8[]={0xA8,0x42,0xE3,0xCB,0x2F,0x00};
uint8_t emitter9[]={0xB0,0xA7,0x32,0x29,0xF1,0xD8};
uint8_t emitter10[]={0xA8,0x42,0xE3,0xCB,0x35,0x9C};
uint8_t emitter11[]={0xA8,0x42,0xE3,0xCA,0x79,0x98};
uint8_t emitter12[]={0xA8,0x42,0xE3,0xCA,0x9F,0xF8};
uint8_t emitter13[]={0xB0,0xA7,0x32,0xF2,0x9D,0xC8};
uint8_t emitter14[]={0xA8,0x42,0xE3,0xCA,0x75,0x10};
uint8_t emitter15[]={0xA8,0x42,0xE3,0xCB,0x77,0xC4};
uint8_t emitter16[]={0xB0,0xA7,0x32,0x2A,0xD2,0x70};
uint8_t* emitterlist[]={emitter1,emitter2,emitter3,emitter4,emitter5,emitter6,emitter7,emitter8,emitter9,emitter10,emitter11,emitter12,emitter13,emitter14,emitter15,emitter16};
uint8_t values[total*(total-1)];

bool maccompare(const uint8_t* mac1,const uint8_t* mac2,int len){
  for(int i=0;i<len;i++){
    if(mac1[i]!=mac2[i]) return false;
  }
  return true;
}


typedef struct packet{
  char info[32];
  uint8_t value[total-1];
} packet;

packet recvpack,sendpack,checkpack;
esp_now_peer_info_t peerinfo;

void ondatarecv(const uint8_t* mac,const uint8_t* incomingdata,int len){
  memcpy(&recvpack,incomingdata,sizeof(recvpack));
  if(strcmp(boosted,recvpack.info)==0){
    count++;
    if(count==total){
      count=0;
      esp_now_send(0,(uint8_t*) &sendpack,sizeof(sendpack));
      delay(500);
      esp_now_send(emitter1,(uint8_t*) &checkpack,sizeof(checkpack));
      t1=millis();
      expect=1;
      Serial.write('s');
    }
  }
  else if(strcmp(tran,recvpack.info)==0){
    for(int i=1;i<=total;i++){
      if(maccompare(emitterlist[i-1],mac,6)){
        if(i==expect){
          nodenum=i;
        }
        else{
          return;
        }
        break;
      }
    }
    for(int i=0;i<(total-1);i++){
      values[done++]=recvpack.value[i];
    }
    if(nodenum==total){
      done=0;
      for(int i=0;i<((total)*(total-1));i++){
        Serial.write(values[i]);
      }
      esp_now_send(0,(uint8_t*) &sendpack,sizeof(sendpack));
      delay(500);
      esp_now_send(emitter1,(uint8_t*) &checkpack,sizeof(checkpack));
      t1=millis();
      expect=1;
      Serial.write('s');
    }
    else{
      esp_now_send(emitterlist[nodenum],(uint8_t*) &checkpack,sizeof(checkpack));
      t1=millis();
      expect=nodenum+1;
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_recv_cb(ondatarecv);
  strcpy(sendpack.info,tran);
  strcpy(checkpack.info,checkmsg);
  peerinfo.channel=0;
  peerinfo.encrypt=false;
  for(int i=0;i<total;i++){
    memcpy(peerinfo.peer_addr,emitterlist[i],6);
    esp_now_add_peer(&peerinfo);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  t2=millis();
  if(t2-t1>=TIMEOUT_LIMIT){
    expect=1;
    done=0;
    esp_now_send(0,(uint8_t*) &sendpack,sizeof(sendpack));
    delay(500);
    esp_now_send(emitter1,(uint8_t*) &checkpack,sizeof(checkpack));
    t1=millis();
  }
}
