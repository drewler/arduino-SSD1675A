#include <SPI.h>

#define MOSI_PIN 6
#define MISO_PIN 7
#define CLK_PIN 8
#define RST_PIN 9

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  
}

void SPI4W_WRITECOM(unsigned char INIT_COM){
  SPI.transfer(INIT_COM);
}

void SPI4W_WRITEDATA(unsigned char INIT_DATA){
  SPI.transfer(INIT_DATA);
}

void set_xy_window(unsigned char xs, unsigned char xe, unsigned int ys, unsigned int ye){
  SPI4W_WRITECOM(0x44);    // set RAM x address start/end, in page 36
  SPI4W_WRITEDATA(xs);    // RAM x address start at 00h;
  SPI4W_WRITEDATA(xe);    // RAM x address end at 0fh(12+1)*8->104 
  SPI4W_WRITECOM(0x45);   // set RAM y address start/end, in page 37
  SPI4W_WRITEDATA(ys);    // RAM y address start at 0;
  SPI4W_WRITEDATA(ys>>8);   
  SPI4W_WRITEDATA(ye);    // RAM y address end at 
  SPI4W_WRITEDATA(ye>>8);   // RAM y address end at 
}

void set_xy_counter(unsigned char x, unsigned char y){
  SPI4W_WRITECOM(0x4E);    // set RAM x address count 
  SPI4W_WRITEDATA(x);
  SPI4W_WRITECOM(0x4F);   // set RAM y address count  
  SPI4W_WRITEDATA(y);
  SPI4W_WRITEDATA(y>>8);
}

void Display_update(void){
  SPI4W_WRITECOM(0x22);
  SPI4W_WRITEDATA(0xC7);    //Load LUT from MCU(0x32), Display update
  SPI4W_WRITECOM(0x20);
//  DELAY_mS(1);
//  READBUSY();
}

void dis_img(unsigned char num) {
  unsigned int row, col;
  unsigned int pcnt;
  set_xy_window(0, 15, 0, 295);
  set_xy_counter(0, 0);
  SPI4W_WRITECOM(0x24);

  pcnt = 0;
  for(col = 0; col < 296; col++){
    for(row = 0; row < 16; row++){
      Serial.print('A');
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
