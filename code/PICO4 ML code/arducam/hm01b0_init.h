#ifndef HM01B0_INIT_H
#define HM01B0_INIT_H
#include "arducam.h"
#include <stdint.h>
struct senosr_reg hm01b0_324x244[] = {
    {0x0103, 0x0},
    {0x0100,0x00},  
    {0x1003,0x8f}, // 0x08
    {0x1007,0x8f}, // 0x08
    {0x3044,0x0A},     
    {0x3045,0x00},    
    {0x3047,0x0A},    
    {0x3050,0xC0},    
    {0x3051,0x42}, 
    {0x3052,0x50},
    {0x3053,0x00},
    {0x3054,0x03}, 
    {0x3055,0xF7},
    {0x3056,0xF8},
    {0x3057,0x29},
    {0x3058,0x1F},
    {0x3059,0x1E},
    {0x3064,0x00},
    {0x3065,0x04},
    {0x1000,0x43},
    {0x1001,0x40},
    {0x1002,0x32}, 
    {0x0350,0x7F},
    {0x1006,0x01},
    {0x1008,0x00},
    {0x1009,0xA0},
    {0x100A,0x60},
    {0x100B,0x90},
    {0x100C,0x40},
    {0x3022,0x01},
    {0x1012,0x01},
    {0x2000,0x07},
    {0x2003,0x00}, 
    {0x2004,0x1C},
    {0x2007,0x00}, 
    {0x2008,0x58}, 
    {0x200B,0x00}, 
    {0x200C,0x7A}, 
    {0x200F,0x00},
    {0x2010,0xB8},
    {0x2013,0x00},
    {0x2014,0x58},
    {0x2017,0x00},
    {0x2018,0x9B},
    {0x2100,0x01},
    {0x2101,0x5F},
    {0x2102,0x0A},
    {0x2103,0x03},
    {0x2104,0x05},
    {0x2105,0x02},
    {0x2106,0x14},
    {0x2107,0x02},
    {0x2108,0x03},
    {0x2109,0x03},
    {0x210A,0x00},
    {0x210B,0x80},
    {0x210C,0x40},
    {0x210D,0x20},
    {0x210E,0x03},
    {0x210F,0x00},
    {0x2110,0x85},
    {0x2111,0x00},
    {0x2112,0xA0},
    {0x2150,0x03},
    {0x0340,0x01},
    {0x0341,0x7A},
    {0x0342,0x01},
    {0x0343,0x77},
    {0x3010,0x00},  //bit[0] 1 enable QVGA
    {0x0383,0x01},
    {0x0387,0x01},
    {0x0390,0x00},
    {0x3011,0x70},
    {0x3059,0x22},
    {0x3060,0x30},
    {0x0101,0x01}, 
    {0x0104,0x01},
    //{0x0390,0x03},  //1/4 binning
    //{0x0383,0x03},
    //{0x0387,0x03},
    //{0x1012,0x03},
    {0x0100,0x01},
    {0xFFFF,0xFF},
};
#endif 