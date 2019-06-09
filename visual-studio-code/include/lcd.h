/***************************************************
  This is an Arduino Library for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480
 
  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#ifndef LCD_H
#define LCD_H

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif
#include "graphics.h"

#if defined(__SAM3X8E__)
  #include <include/pio.h>
  #define PROGMEM
  #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
  #define pgm_read_word(addr) (*(const unsigned short *)(addr))
  typedef unsigned char prog_uchar;
#elif defined(__AVR__)
  #include <avr/pgmspace.h>
#elif defined(ESP8266)
  #include <pgmspace.h>
#endif


#define ILI9340_TFTWIDTH  240
#define ILI9340_TFTHEIGHT 320

#define ILI9340_NOP     0x00
#define ILI9340_SWRESET 0x01
#define ILI9340_RDDID   0x04
#define ILI9340_RDDST   0x09

#define ILI9340_SLPIN   0x10
#define ILI9340_SLPOUT  0x11
#define ILI9340_PTLON   0x12
#define ILI9340_NORON   0x13

#define ILI9340_RDMODE  0x0A
#define ILI9340_RDMADCTL  0x0B
#define ILI9340_RDPIXFMT  0x0C
#define ILI9340_RDIMGFMT  0x0A
#define ILI9340_RDSELFDIAG  0x0F

#define ILI9340_INVOFF  0x20
#define ILI9340_INVON   0x21
#define ILI9340_GAMMASET 0x26
#define ILI9340_DISPOFF 0x28
#define ILI9340_DISPON  0x29

#define ILI9340_CASET   0x2A
#define ILI9340_PASET   0x2B
#define ILI9340_RAMWR   0x2C
#define ILI9340_RAMRD   0x2E

#define ILI9340_PTLAR   0x30
#define ILI9340_MADCTL  0x36


#define ILI9340_MADCTL_MY  0x80
#define ILI9340_MADCTL_MX  0x40
#define ILI9340_MADCTL_MV  0x20
#define ILI9340_MADCTL_ML  0x10
#define ILI9340_MADCTL_RGB 0x00
#define ILI9340_MADCTL_BGR 0x08
#define ILI9340_MADCTL_MH  0x04

#define ILI9340_PIXFMT  0x3A

#define ILI9340_FRMCTR1 0xB1
#define ILI9340_FRMCTR2 0xB2
#define ILI9340_FRMCTR3 0xB3
#define ILI9340_INVCTR  0xB4
#define ILI9340_DFUNCTR 0xB6

#define ILI9340_PWCTR1  0xC0
#define ILI9340_PWCTR2  0xC1
#define ILI9340_PWCTR3  0xC2
#define ILI9340_PWCTR4  0xC3
#define ILI9340_PWCTR5  0xC4
#define ILI9340_VMCTR1  0xC5
#define ILI9340_VMCTR2  0xC7

#define ILI9340_RDID1   0xDA
#define ILI9340_RDID2   0xDB
#define ILI9340_RDID3   0xDC
#define ILI9340_RDID4   0xDD

#define ILI9340_GMCTRP1 0xE0
#define ILI9340_GMCTRN1 0xE1
/*
#define ILI9340_PWCTR6  0xFC

*/

// Color definitions
#define  ILI9340_BLACK   0x0000
#define ILI9340_BLUE    0x001F
#define ILI9340_RED     0xF800
#define ILI9340_GREEN   0x07E0
#define ILI9340_CYAN    0x07FF
#define ILI9340_MAGENTA 0xF81F
#define ILI9340_YELLOW  0xFFE0  
#define ILI9340_WHITE   0xFFFF

// some RGB color definitions
enum Color{
  Black          = 0x0000,      /*   0,   0,   0 */
  Navy           = 0x000F,      /*   0,   0, 128 */
  DarkGreen      = 0x03E0,      /*   0, 128,   0 */
  DarkCyan       = 0x03EF,      /*   0, 128, 128 */
  Maroon         = 0x7800,      /* 128,   0,   0 */
  Purple         = 0x780F,      /* 128,   0, 128 */
  Olive          = 0x7BE0,      /* 128, 128,   0 */
  LightGrey      = 0xC618,      /* 192, 192, 192 */
  DarkGrey       = 0x7BEF,      /* 128, 128, 128 */
  Blue           = 0x001F,      /*   0,   0, 255 */
  Green          = 0x07E0,      /*   0, 255,   0 */
  Cyan           = 0x07FF,      /*   0, 255, 255 */
  Red            = 0xF800,      /* 255,   0,   0 */
  Magenta        = 0xF81F,      /* 255,   0, 255 */
  Yellow         = 0xFFE0,      /* 255, 255,   0 */
  White          = 0xFFFF,      /* 255, 255, 255 */
  Orange         = 0xFD20,      /* 255, 165,   0 */
  GreenYellow    = 0xAFE5,      /* 173, 255,  47 */
  Pink           = 0xF81F
};

class LCD : public Graphics {

 public:
  LCD(uint8_t CS, uint8_t RS, uint8_t MOSI, uint8_t SCLK,
       uint8_t RST, uint8_t MISO);
  LCD(uint8_t CS, uint8_t RS, uint8_t RST);

  void     begin(void),
           setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1),
           pushColor(uint16_t color),
           fillScreen(uint16_t color),
           drawPixel(int16_t x, int16_t y, uint16_t color),
           drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
           drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
           fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
             uint16_t color),
           setRotation(uint8_t r),
           invertDisplay(boolean i);
  uint16_t Color565(uint8_t r, uint8_t g, uint8_t b);

  /* These are not for current use, 8-bit protocol only! */
  uint8_t  readdata(void),
           readcommand8(uint8_t);
  /*
  uint16_t readcommand16(uint8_t);
  uint32_t readcommand32(uint8_t);
  void     dummyclock(void);
  */  

  void     spiwrite(uint8_t),
    writecommand(uint8_t c),
    writedata(uint8_t d),
    commandList(uint8_t *addr);
  uint8_t  spiread(void);

 private:
  uint8_t  tabcolor;



  boolean  hwSPI;
#ifdef __AVR__  
  volatile uint8_t *mosiport, *clkport, *dcport, *rsport, *csport;
  uint8_t  _cs, _dc, _rst, _mosi, _miso, _sclk,
           mosipinmask, clkpinmask, cspinmask, dcpinmask;
#endif //  #ifdef __AVR__
#if defined(__SAM3X8E__)
  Pio *mosiport, *clkport, *dcport, *rsport, *csport;
  uint32_t  _cs, _dc, _rst, _mosi, _miso, _sclk,
            mosipinmask, clkpinmask, cspinmask, dcpinmask;
#endif //  #if defined(__SAM3X8E__)
#if defined(__arm__) && defined(CORE_TEENSY)
  volatile uint8_t *mosiport, *clkport, *dcport, *rsport, *csport;
  uint8_t  _cs, _dc, _rst, _mosi, _miso, _sclk,
           mosipinmask, clkpinmask, cspinmask, dcpinmask;
#endif
};

#endif

