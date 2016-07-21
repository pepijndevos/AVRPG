#include "SPI.h"
#include "ILI9341_t3.h"
#include "sprites.h"

// For the Adafruit shield, these are the default.
#define TFT_DC  9
#define TFT_CS 10

#define TS 32 // tile size

#define TRANSP 0x5452
#define GRASS sprite_1
#define SAND sprite_9
#define SANDS sprite_10

#define BITL sprite_75
#define BOTL sprite_72
#define BOL sprite_80

#define FLWR sprite_169
#define PLNT sprite_173

#define CH1U1 char_0
#define CH1U2 char_1
#define CH1U3 char_2
#define CH1D1 char_3
#define CH1D2 char_4
#define CH1D3 char_5
#define CH1L1 char_6
#define CH1L2 char_7
#define CH1L3 char_8
#define CH1R1 char_9
#define CH1R2 char_10
#define CH1R3 char_11


// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

const uint16_t* layer0[7][10] = {
  SAND , SAND , SANDS, SAND , SAND , SAND , GRASS, GRASS, GRASS, GRASS,
  SAND , SANDS, SAND , SAND , SAND , GRASS, GRASS, GRASS, GRASS, GRASS,
  SANDS, SAND , SAND , SAND , GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,
  SAND , SAND , SAND , GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,
  SAND , SAND , GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,
  SAND , SAND , GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,
  SAND , SAND , GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS
};

const uint16_t* layer1[7][10] = {
  NULL , NULL , NULL , NULL , BOTL , BITL , NULL,  NULL,  NULL,  NULL, 
  NULL , NULL , NULL , BOTL , BITL , NULL,  PLNT,  NULL,  FLWR,  NULL, 
  NULL , NULL , BOTL , BITL , NULL,  NULL,  NULL,  NULL,  NULL,  NULL, 
  NULL , BOTL , BITL , NULL,  NULL,  NULL,  FLWR,  NULL,  NULL,  NULL, 
  NULL , BOL  , NULL,  NULL,  PLNT,  NULL,  NULL,  NULL,  NULL,  PLNT, 
  NULL , BOL  , NULL,  NULL,  NULL,  PLNT,  NULL,  NULL,  FLWR,  NULL, 
  NULL , BOL  , NULL , PLNT,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL
};

int chx=2*TS;
int chy=6*TS;
uint16_t chbg[TS*TS];
const uint16_t* chsp = CH1D1;

void setup() {
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.setRotation(3);
  tft.println("Waiting for Arduino Serial Monitor...");

  Serial.begin(9600);
  //while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println("ILI9341 Test!"); 

  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 

  for (int x=0; x<10; x++) {
    for (int y=0; y<7; y++) {
      if(layer1[y][x] == NULL) {
        tft.writeRect(x*TS, y*TS, TS, TS, layer0[y][x]);
      } else {
        writeOn(x*TS, y*TS, TS, TS, layer0[y][x], layer1[y][x]);
      }
    }
  }
  tft.readRect(chx, chy, TS, TS, chbg);
  //delay(5000);
}

#define DELAY 1000
void loop(void) {
  int i;
  for(i=0; i<8; i++) {
    chsp = CH1R1;
    delay(100);
    moveSmall(4, -1);
    chsp = CH1R2;
    delay(100);
    moveSmall(4, -1);
    chsp = CH1R3;
    delay(100);
    moveSmall(4, -1);
    chsp = CH1R2;
    delay(100);
    moveSmall(4, -1);
  }
  for(i=0; i<8; i++) {
    chsp = CH1U1;
    delay(100);
    moveSmall(1, -4);
    chsp = CH1U2;
    delay(100);
    moveSmall(1, -4);
    chsp = CH1U3;
    delay(100);
    moveSmall(1, -4);
    chsp = CH1U2;
    delay(100);
    moveSmall(1, -4);
  }
  for(i=0; i<8; i++) {
    chsp = CH1L1;
    delay(100);
    moveSmall(-4, 1);
    chsp = CH1L2;
    delay(100);
    moveSmall(-4, 1);
    chsp = CH1L3;
    delay(100);
    moveSmall(-4, 1);
    chsp = CH1L2;
    delay(100);
    moveSmall(-4, 1);
  }
  for(i=0; i<8; i++) {
    chsp = CH1D1;
    delay(100);
    moveSmall(-1, 4);
    chsp = CH1D2;
    delay(100);
    moveSmall(-1, 4);
    chsp = CH1D3;
    delay(100);
    moveSmall(-1, 4);
    chsp = CH1D2;
    delay(100);
    moveSmall(-1, 4);
  }
  
}

void pixcpytr ( uint16_t * destination, const uint16_t * source, size_t num ) {
  for(unsigned int i=0; i<num; i++) {
    if (source[i] != TRANSP) {
      destination[i]=source[i];
    }
  }
}

void writeTransparent(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *pcolors) {
  uint16_t buf[w*h];
  tft.readRect(x, y, w, h, buf);
  pixcpytr(buf, pcolors, w*h);
  tft.writeRect(x, y, w, h, buf);
}

void writeOn(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *bg, const uint16_t *fg) {
  uint16_t buf[w*h];
  memcpy(buf, bg, w*h*sizeof(uint16_t));
  for(int i=0; i<w*h; i++) {
    if (fg[i] != TRANSP) {
      buf[i]=fg[i];
    }
  }
  tft.writeRect(x, y, w, h, buf);
}

// copy a portion of a sprite to another sprite
void subSprite(int16_t sx, int16_t sy, int16_t w, int16_t h, const uint16_t *src, int16_t dx, int16_t dy, uint16_t *dest) {
  for(int y = 0; y < h; y++) {
    pixcpytr(&dest[TS*(y+dy)+dx], &src[TS*(y+sy)+sx], w);
  }
}

void moveSmall(int dx, int dy) {
  uint16_t buf[TS*TS];
  int w = TS-abs(dx);
  int h = TS-abs(dy);
  int srx = dx > 0 ? dx : 0;
  int sry = dy > 0 ? dy : 0;
  int dsx = dx < 0 ? -dx : 0;
  int dsy = dy < 0 ? -dy : 0;
  // Write old bg with sprite in new pos
  memcpy(buf, chbg, TS*TS*sizeof(uint16_t));
  subSprite(dsx, dsy, w, h, chsp, srx, sry, buf);
  tft.writeRect(chx, chy, TS, TS, buf);

  chy+=dy;
  chx+=dx;
  
  tft.readRect(chx, chy, TS, TS, buf);
  subSprite(srx, sry, w, h, chbg, dsx, dsy, buf);
  memcpy(chbg, buf, TS*TS*sizeof(uint16_t));
  writeOn(chx, chy, TS, TS, chbg, chsp);
}

void moveDown(unsigned int dy) {
  tft.writeRect(chx, chy, TS, dy, chbg);
  subSprite(0, dy, TS, TS-dy,  chbg, 0, 0, chbg);
  tft.readRect(chx, chy+TS, TS, dy, &chbg[TS*(TS-dy)]);
  chy+=dy;
}

void moveUp(unsigned int dy) {
  tft.writeRect(chx, chy+TS-dy, TS, dy, &chbg[TS*(TS-dy)]);
  subSprite(0, 0, TS, TS-dy,  chbg, 0, dy, chbg);
  tft.readRect(chx, chy-dy, TS, dy, chbg);
  chy-=dy;
}





