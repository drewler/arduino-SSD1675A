#include <SPI.h>
#include "img_file.h"

#define CS_PIN 7
#define DI_PIN 8    // Data/Instruction pin
#define BUSY_PIN 9
#define RST_PIN 10
#define MOSI_PIN 11
#define MISO_PIN 12
#define CLK_PIN 13

const unsigned char init_data[] = {
0x22,  0x11, 0x10, 0x00, 0x10, 0x00, 0x00, 0x11, 0x88, 0x80, 0x80, 0x80, 0x00, 0x00, 0x6A, 0x9B,
0x9B, 0x9B, 0x9B, 0x00, 0x00, 0x6A, 0x9B, 0x9B, 0x9B, 0x9B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x04, 0x18, 0x04, 0x16, 0x01, 0x0A, 0x0A, 0x0A, 0x0A, 0x02, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x08, 0x3C, 0x07, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

void setup() {
  pinMode(CS_PIN, OUTPUT);
  pinMode(DI_PIN, OUTPUT);
  pinMode(BUSY_PIN, INPUT);
  pinMode(RST_PIN, OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  RESET();
  SPI4W_WRITECOM(0x12);      //SWRESET
  READBUSY();
  INIT_SSD1675();
  dis_img(1);
}

void INIT_SSD1675(){
  SPI4W_WRITECOM(0x74);
  SPI4W_WRITEDATA(0x54);
  SPI4W_WRITECOM(0x75);
  SPI4W_WRITEDATA(0x3b);
  SPI4W_WRITECOM(0x01);   // Set MUX as 296
  SPI4W_WRITEDATA(0x27);
  SPI4W_WRITEDATA(0x01);
  SPI4W_WRITEDATA(0x00);
  SPI4W_WRITECOM(0x3A);   // Set 100Hz
  //SPI4W_WRITEDATA(0x25);         // Set 100Hz
  SPI4W_WRITEDATA(0x35);         // Set 130Hz
  //SPI4W_WRITEDATA(0x07);         // Set 150Hz
  SPI4W_WRITECOM(0x3B);   // Set 100Hz
  //SPI4W_WRITEDATA(0x06);         // Set 100Hz
  SPI4W_WRITEDATA(0x04);         // Set 130Hz
  SPI4W_WRITECOM(0x11);   // data enter mode
  SPI4W_WRITEDATA(0x03);
  SPI4W_WRITECOM(0x44);   // set RAM x address start/end, in page 36
  SPI4W_WRITEDATA(0x00);    // RAM x address start at 00h;
  SPI4W_WRITEDATA(0x0f);    // RAM x address end at 0fh(15+1)*8->128 
  SPI4W_WRITECOM(0x45);   // set RAM y address start/end, in page 37

  SPI4W_WRITEDATA(0x00);    // RAM y address start at 127h;
  SPI4W_WRITEDATA(0x00);      
  SPI4W_WRITEDATA(0x27);    // RAM y address end at 00h;
  SPI4W_WRITEDATA(0x01);    
  SPI4W_WRITECOM(0x04);   // set VSH,VSL value
  SPI4W_WRITEDATA(0x41);    //      2D9  15v
  // SPI4W_WRITEDATA(0xc6);   //      2D13   7.8v 
  //SPI4W_WRITEDATA(0xc1);    //      2D13   7.5v 
  //  SPI4W_WRITEDATA(0x96);    //      2D9   3.8v 
  //  SPI4W_WRITEDATA(0xbf);   //      2D13   7.3v 
  //  SPI4W_WRITEDATA(0xb8);    //      2D13   6.6v 
  //SPI4W_WRITEDATA(0xb4);    //      2D13   6.2v 
  //SPI4W_WRITEDATA(0xb2);    //      2D13   6v 
  // SPI4W_WRITEDATA(0xad);    //      2D9   5v 
  SPI4W_WRITEDATA(0xa8);   //      2D9   5v 
  // SPI4W_WRITEDATA(0xA4);    //      2D9   4.6v 
  // SPI4W_WRITEDATA(0xA2);    //      2D9   4.4v 
  // SPI4W_WRITEDATA(0xa0);    //      2D9   4.2v 
  // SPI4W_WRITEDATA(0x9C);    //      2D9   3.8v 
  //  SPI4W_WRITEDATA(0x96);    //      2D9   3.8v 
  SPI4W_WRITEDATA(0x32);    //      2D9  -15v
  SPI4W_WRITECOM(0x2C);           // vcom
  // SPI4W_WRITEDATA(0x78);           //-3V
  // SPI4W_WRITEDATA(0x6f);           //-2.6V
  //  SPI4W_WRITEDATA(0x6c);           //-2.6V
  SPI4W_WRITEDATA(0x68);           //-2.6V
  // SPI4W_WRITEDATA(0x5F);           //-2.4V
  // SPI4W_WRITEDATA(0x58);           //-2.4V
  SPI4W_WRITECOM(0x3C);   // board
  SPI4W_WRITEDATA(0x33);    //GS1-->GS1
  WRITE_LUT();
}

void WRITE_LUT(){
  unsigned char i;

  SPI4W_WRITECOM(0x32);
  for(i=0;i<70;i++){    // write LUT register with 29bytes instead of 30bytes 2D13
    SPI4W_WRITEDATA(init_data[i]);
  }
}

void RESET(){
  digitalWrite(RST_PIN, LOW);
  delay(1);
  digitalWrite(RST_PIN, HIGH);
  delay(1);
}

void SPI4W_WRITECOM(unsigned char INIT_COM){
  digitalWrite(CS_PIN, LOW);
  digitalWrite(DI_PIN, LOW);
  SPI.beginTransaction (SPISettings (1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(INIT_COM);
  SPI.endTransaction();
  digitalWrite(CS_PIN, HIGH);
}

void SPI4W_WRITEDATA(unsigned char INIT_DATA){
  digitalWrite(CS_PIN, LOW);
  digitalWrite(DI_PIN, HIGH);
  SPI.beginTransaction (SPISettings (1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(INIT_DATA);
  SPI.endTransaction();
  digitalWrite(CS_PIN, HIGH);
}

void READBUSY(){
  int busy_value = HIGH;

  while(1){
    busy_value = digitalRead(BUSY_PIN);
    if(busy_value == LOW){
      break;
    }
  }
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

void display_update(void){
  SPI4W_WRITECOM(0x22);
  SPI4W_WRITEDATA(0xC7);    //Load LUT from MCU(0x32), Display update
  SPI4W_WRITECOM(0x20);
//  DELAY_mS(1);
  READBUSY();
}

void dis_img(unsigned char num) {
  unsigned int row, col;
  unsigned int pixel_count;

  const unsigned char *img_bw = img1_bw;
  const unsigned char *img_red = img1_red;

  // Load B/W pixels into RAM
  set_xy_window(0, 15, 0, 295);
  set_xy_counter(0, 0);
  SPI4W_WRITECOM(0x24); // Write RAM (B/W)

  pixel_count = 0;
  for(col = 0; col < 296; col++){
    for(row = 0; row < 16; row++){
      SPI4W_WRITEDATA(img_bw[pixel_count]);
      pixel_count++;
    }
  }

  // Load red pixels into RAM
  set_xy_window(0, 15, 0, 295);
  set_xy_counter(0, 0);
  SPI4W_WRITECOM(0x26); // Write RAM (red)
  
  pixel_count = 0;
  for(col = 0; col < 296; col++){
    for(row = 0; row < 16; row++){
      SPI4W_WRITEDATA(img_red[pixel_count]);
    }
  }

  display_update();
}

void loop() {
  // put your main code here, to run repeatedly:

}
