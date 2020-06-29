/*
 * SPI_FLASH.h
 *
 *  Created on: 2020Äê5ÔÂ22ÈÕ
 *      Author: WangXiang
 */

#ifndef SPI_FLASH_SPI_FLASH_H_
#define SPI_FLASH_SPI_FLASH_H_
#include "UserConfig.h"
#include <stdarg.h>



#define Page1 0x0
#define Page2 0x200
#define Page3 0x400
#define Page4 0x600
#define Page5 0x800
#define Page6 0xa00
#define Page7 0xc00
#define Page8 0xe00
#define Page9 0x1000
#define Page10 0x1200
#define Page11 0x1400
#define Page12 0x1600
#define Page13 0x1800
#define Page14 0x1a00
#define Page15 0x1c00
#define Page16 0x1e00
#define Page17 0x2000
#define Page18 0x2200
#define Page19 0x2400
#define Page20 0x2600
#define Page21 0x2800
#define Page22 0x2a00
#define Page23 0x2c00
#define Page24 0x2e00
#define Page25 0x3000
#define Page26 0x3200
#define Page27 0x3400
#define Page28 0x3600
#define Page29 0x3800
#define Page30 0x3a00
#define Page31 0x3c00
#define Page32 0x3e00
#define Page33 0x4000
#define Page34 0x4200
#define Page35 0x4400
#define Page36 0x4600
#define Page37 0x4800
#define Page38 0x4a00
#define Page39 0x4c00
#define Page40 0x4e00
#define Page41 0x5000
#define Page42 0x5200
#define Page43 0x5400
#define Page44 0x5600
#define Page45 0x5800
#define Page46 0x5a00
#define Page47 0x5c00
#define Page48 0x5e00
#define Page49 0x6000
#define Page50 0x6200
#define Page51 0x6400
#define Page52 0x6600
#define Page53 0x6800
#define Page54 0x6a00
#define Page55 0x6c00
#define Page56 0x6e00
#define Page57 0x7000
#define Page58 0x7200
#define Page59 0x7400
#define Page60 0x7600
#define Page61 0x7800
#define Page62 0x7a00
#define Page63 0x7c00
#define Page64 0x7e00
#define Page65 0x8000
#define Page66 0x8200
#define Page67 0x8400
#define Page68 0x8600
#define Page69 0x8800
#define Page70 0x8a00
#define Page71 0x8c00
#define Page72 0x8e00
#define Page73 0x9000
#define Page74 0x9200
#define Page75 0x9400
#define Page76 0x9600
#define Page77 0x9800
#define Page78 0x9a00
#define Page79 0x9c00
#define Page80 0x9e00
#define Page81 0xa000
#define Page82 0xa200
#define Page83 0xa400
#define Page84 0xa600
#define Page85 0xa800
#define Page86 0xaa00
#define Page87 0xac00
#define Page88 0xae00
#define Page89 0xb000
#define Page90 0xb200
#define Page91 0xb400
#define Page92 0xb600
#define Page93 0xb800
#define Page94 0xba00
#define Page95 0xbc00
#define Page96 0xbe00
#define Page97 0xc000
#define Page98 0xc200
#define Page99 0xc400
#define Page100 0xc600
#define Page101 0xc800
#define Page102 0xca00
#define Page103 0xcc00
#define Page104 0xce00
#define Page105 0xd000
#define Page106 0xd200
#define Page107 0xd400
#define Page108 0xd600
#define Page109 0xd800
#define Page110 0xda00
#define Page111 0xdc00
#define Page112 0xde00
#define Page113 0xe000
#define Page114 0xe200
#define Page115 0xe400
#define Page116 0xe600
#define Page117 0xe800
#define Page118 0xea00
#define Page119 0xec00
#define Page120 0xee00
#define Page121 0xf000
#define Page122 0xf200
#define Page123 0xf400
#define Page124 0xf600
#define Page125 0xf800
#define Page126 0xfa00
#define Page127 0xfc00
#define Page128 0xfe00
#define Page129 0x10000
#define Page130 0x10200
#define Page131 0x10400
#define Page132 0x10600
#define Page133 0x10800
#define Page134 0x10a00
#define Page135 0x10c00
#define Page136 0x10e00
#define Page137 0x11000
#define Page138 0x11200
#define Page139 0x11400
#define Page140 0x11600
#define Page141 0x11800
#define Page142 0x11a00
#define Page143 0x11c00
#define Page144 0x11e00
#define Page145 0x12000
#define Page146 0x12200
#define Page147 0x12400
#define Page148 0x12600
#define Page149 0x12800
#define Page150 0x12a00
#define Page151 0x12c00
#define Page152 0x12e00
#define Page153 0x13000
#define Page154 0x13200
#define Page155 0x13400
#define Page156 0x13600
#define Page157 0x13800
#define Page158 0x13a00
#define Page159 0x13c00
#define Page160 0x13e00
#define Page161 0x14000
#define Page162 0x14200
#define Page163 0x14400
#define Page164 0x14600
#define Page165 0x14800
#define Page166 0x14a00
#define Page167 0x14c00
#define Page168 0x14e00
#define Page169 0x15000
#define Page170 0x15200
#define Page171 0x15400
#define Page172 0x15600
#define Page173 0x15800
#define Page174 0x15a00
#define Page175 0x15c00
#define Page176 0x15e00
#define Page177 0x16000
#define Page178 0x16200
#define Page179 0x16400
#define Page180 0x16600
#define Page181 0x16800
#define Page182 0x16a00
#define Page183 0x16c00
#define Page184 0x16e00
#define Page185 0x17000
#define Page186 0x17200
#define Page187 0x17400
#define Page188 0x17600
#define Page189 0x17800
#define Page190 0x17a00
#define Page191 0x17c00
#define Page192 0x17e00
#define Page193 0x18000
#define Page194 0x18200
#define Page195 0x18400
#define Page196 0x18600
#define Page197 0x18800
#define Page198 0x18a00
#define Page199 0x18c00
#define Page200 0x18e00
#define Page201 0x19000
#define Page202 0x19200
#define Page203 0x19400
#define Page204 0x19600
#define Page205 0x19800
#define Page206 0x19a00
#define Page207 0x19c00
#define Page208 0x19e00
#define Page209 0x1a000
#define Page210 0x1a200
#define Page211 0x1a400
#define Page212 0x1a600
#define Page213 0x1a800
#define Page214 0x1aa00
#define Page215 0x1ac00
#define Page216 0x1ae00
#define Page217 0x1b000
#define Page218 0x1b200
#define Page219 0x1b400
#define Page220 0x1b600
#define Page221 0x1b800
#define Page222 0x1ba00
#define Page223 0x1bc00
#define Page224 0x1be00
#define Page225 0x1c000
#define Page226 0x1c200
#define Page227 0x1c400
#define Page228 0x1c600
#define Page229 0x1c800
#define Page230 0x1ca00
#define Page231 0x1cc00
#define Page232 0x1ce00
#define Page233 0x1d000
#define Page234 0x1d200
#define Page235 0x1d400
#define Page236 0x1d600
#define Page237 0x1d800
#define Page238 0x1da00
#define Page239 0x1dc00
#define Page240 0x1de00
#define Page241 0x1e000
#define Page242 0x1e200
#define Page243 0x1e400
#define Page244 0x1e600
#define Page245 0x1e800
#define Page246 0x1ea00
#define Page247 0x1ec00
#define Page248 0x1ee00
#define Page249 0x1f000
#define Page250 0x1f200
#define Page251 0x1f400
#define Page252 0x1f600
#define Page253 0x1f800
#define Page254 0x1fa00
#define Page255 0x1fc00
#define Page256 0x1fe00



#define CS_HIGH digitalWriteA(GPIO_Pin_4,HIGH)
#define CS_LOW digitalWriteA(GPIO_Pin_4,LOW)

extern int SPI_printf(uint32_t page,const char *fmt, ...);

void SPI_INIT();
void SPI_write(u8 TxData);
void SPI_writeStr(uint32_t page,char *str);



void WREN();
void ERDI();



void ts_itoa(char **buf, unsigned int d, int base);
int ts_formatstring(char *buf, const char *fmt, va_list va);
int ts_formatlength(const char *fmt, va_list va);

#endif /* SPI_FLASH_SPI_FLASH_H_ */
