#ifndef BITS_H
#define BITS_H

typedef enum {
    B0 = 1,
    B1 = 2,
    B2 = 4,
    B3 = 8,
    B4 = 16,
    B5 = 32,
    B6 = 64,
    B7 = 128,
    B8 = 256,
    B9 = 512,
    B10 = 1024,
    B11 = 2048,
    B12 = 4096,
    B13 = 8192,
    B14 = 1 << 14,
    B15 = 1 << 15,
    B16 = 1 << 16,
    B17 = 1 << 17,
    B18 = 1 << 18,
    B19 = 1 << 19,
    B20 = 1 << 20,
    B21 = 1 << 21,
    B22 = 1 << 22,
    B23 = 1 << 23,
    B24 = 1 << 24,
    B25 = 1 << 25,
    B26 = 1 << 26,
    B27 = 1 << 27,
    B28 = 1 << 28,
    B29 = 1 << 29,
    B30 =  1 << 30,
    B31 =  1 << 31
	} Bit;
//    BITS[] = {B0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15, 
//															B16, B17, B18, B19, B20, B21, B22, B23, B24, B25, B26, B27, B28, B29, B30, B31};


typedef enum
{
	false,
	true
} bool;

//															typedef struct {                                    /*!< (@ 0x48028000) PORT0 Structure                                        */
//  __IO uint32_t  OUT;                               /*!< (@ 0x48028000) Port 0 Output Register                                 */
//  __O  uint32_t  OMR;                               /*!< (@ 0x48028004) Port 0 Output Modification Register                    */
//  __I  uint32_t  RESERVED[2];
//  __IO uint32_t  IOCR0;                             /*!< (@ 0x48028010) Port 0 Input/Output Control Register 0                 */
//  __IO uint32_t  IOCR4;                             /*!< (@ 0x48028014) Port 0 Input/Output Control Register 4                 */
//  __IO uint32_t  IOCR8;                             /*!< (@ 0x48028018) Port 0 Input/Output Control Register 8                 */
//  __IO uint32_t  IOCR12;                            /*!< (@ 0x4802801C) Port 0 Input/Output Control Register 12                */
//  __I  uint32_t  RESERVED1;
//  __I  uint32_t  IN;                                /*!< (@ 0x48028024) Port 0 Input Register                                  */
//  __I  uint32_t  RESERVED2[6];
//  __IO uint32_t  PDR0;                              /*!< (@ 0x48028040) Port 0 Pad Driver Mode 0 Register                      */
//  __IO uint32_t  PDR1;                              /*!< (@ 0x48028044) Port 0 Pad Driver Mode 1 Register                      */
//  __I  uint32_t  RESERVED3[6];
//  __I  uint32_t  PDISC;                             /*!< (@ 0x48028060) Port 0 Pin Function Decision Control Register          */
//  __I  uint32_t  RESERVED4[3];
//  __IO uint32_t  PPS;                               /*!< (@ 0x48028070) Port 0 Pin Power Save Register                         */
//  __IO uint32_t  HWSEL;                             /*!< (@ 0x48028074) Port 0 Pin Hardware Select Register                    */
//} PORT0_Type;
//															
//typedef struct {                                    /*!< (@ 0x48028300) PORT3 Structure                                        */
//  __IO uint32_t  OUT;                               /*!< (@ 0x48028300) Port 3 Output Register                                 */
//  __O  uint32_t  OMR;                               /*!< (@ 0x48028304) Port 3 Output Modification Register                    */
//  __I  uint32_t  RESERVED[2];
//  __IO uint32_t  IOCR0;                             /*!< (@ 0x48028310) Port 3 Input/Output Control Register 0                 */
//  __IO uint32_t  IOCR4;                             /*!< (@ 0x48028314) Port 3 Input/Output Control Register 4                 */
//  __I  uint32_t  RESERVED1[3];
//  __I  uint32_t  IN;                                /*!< (@ 0x48028324) Port 3 Input Register                                  */
//  __I  uint32_t  RESERVED2[6];
//  __IO uint32_t  PDR0;                              /*!< (@ 0x48028340) Port 3 Pad Driver Mode 0 Register                      */
//  __I  uint32_t  RESERVED3[7];
//  __I  uint32_t  PDISC;                             /*!< (@ 0x48028360) Port 3 Pin Function Decision Control Register          */
//  __I  uint32_t  RESERVED4[3];
//  __IO uint32_t  PPS;                               /*!< (@ 0x48028370) Port 3 Pin Power Save Register                         */
//  __IO uint32_t  HWSEL;                             /*!< (@ 0x48028374) Port 3 Pin Hardware Select Register                    */
//} PORT3_Type;


///* ================================================================================ */
///* ================                      PORT4                     ================ */
///* ================================================================================ */


///**
//  * @brief Port 4 (PORT4)
//  */

//typedef struct {                                    /*!< (@ 0x48028400) PORT4 Structure                                        */
//  __IO uint32_t  OUT;                               /*!< (@ 0x48028400) Port 4 Output Register                                 */
//  __O  uint32_t  OMR;                               /*!< (@ 0x48028404) Port 4 Output Modification Register                    */
//  __I  uint32_t  RESERVED[2];
//  __IO uint32_t  IOCR0;                             /*!< (@ 0x48028410) Port 4 Input/Output Control Register 0                 */
//  __I  uint32_t  RESERVED1[4];
//  __I  uint32_t  IN;                                /*!< (@ 0x48028424) Port 4 Input Register                                  */
//  __I  uint32_t  RESERVED2[6];
//  __IO uint32_t  PDR0;                              /*!< (@ 0x48028440) Port 4 Pad Driver Mode 0 Register                      */
//  __I  uint32_t  RESERVED3[7];
//  __I  uint32_t  PDISC;                             /*!< (@ 0x48028460) Port 4 Pin Function Decision Control Register          */
//  __I  uint32_t  RESERVED4[3];
//  __IO uint32_t  PPS;                               /*!< (@ 0x48028470) Port 4 Pin Power Save Register                         */
//  __IO uint32_t  HWSEL;                             /*!< (@ 0x48028474) Port 4 Pin Hardware Select Register                    */
//} PORT4_Type;


///* ================================================================================ */
///* ================                      PORT5                     ================ */
///* ================================================================================ */


///**
//  * @brief Port 5 (PORT5)
//  */

//typedef struct {                                    /*!< (@ 0x48028500) PORT5 Structure                                        */
//  __IO uint32_t  OUT;                               /*!< (@ 0x48028500) Port 5 Output Register                                 */
//  __O  uint32_t  OMR;                               /*!< (@ 0x48028504) Port 5 Output Modification Register                    */
//  __I  uint32_t  RESERVED[2];
//  __IO uint32_t  IOCR0;                             /*!< (@ 0x48028510) Port 5 Input/Output Control Register 0                 */
//  __IO uint32_t  IOCR4;                             /*!< (@ 0x48028514) Port 5 Input/Output Control Register 4                 */
//  __I  uint32_t  RESERVED1[3];
//  __I  uint32_t  IN;                                /*!< (@ 0x48028524) Port 5 Input Register                                  */
//  __I  uint32_t  RESERVED2[6];
//  __IO uint32_t  PDR0;                              /*!< (@ 0x48028540) Port 5 Pad Driver Mode 0 Register                      */
//  __I  uint32_t  RESERVED3[7];
//  __I  uint32_t  PDISC;                             /*!< (@ 0x48028560) Port 5 Pin Function Decision Control Register          */
//  __I  uint32_t  RESERVED4[3];
//  __IO uint32_t  PPS;                               /*!< (@ 0x48028570) Port 5 Pin Power Save Register                         */
//  __IO uint32_t  HWSEL;                             /*!< (@ 0x48028574) Port 5 Pin Hardware Select Register                    */
//} PORT5_Type;

///* ================================================================================ */
///* ================                     PORT14                     ================ */
///* ================================================================================ */

//														typedef struct {                                    /*!< (@ 0x48028000) PORT0 Structure                                        */
//  __IO uint32_t  OUT;                               /*!< (@ 0x48028000) Port 0 Output Register                                 */
//  __O  uint32_t  OMR;                               /*!< (@ 0x48028004) Port 0 Output Modification Register                    */
//  __I  uint32_t  RESERVED[2];
//  __IO uint32_t  IOCR0;                             /*!< (@ 0x48028010) Port 0 Input/Output Control Register 0                 */
//  __IO uint32_t  IOCR4;                             /*!< (@ 0x48028014) Port 0 Input/Output Control Register 4                 */
//  __IO uint32_t  IOCR8;                             /*!< (@ 0x48028018) Port 0 Input/Output Control Register 8                 */
//  __IO uint32_t  IOCR12;                            /*!< (@ 0x4802801C) Port 0 Input/Output Control Register 12                */
//  __I  uint32_t  RESERVED1;
//  __I  uint32_t  IN;                                /*!< (@ 0x48028024) Port 0 Input Register                                  */
//  __I  uint32_t  RESERVED2[6];
//  __IO uint32_t  PDR0;                              /*!< (@ 0x48028040) Port 0 Pad Driver Mode 0 Register                      */
//  __IO uint32_t  PDR1;                              /*!< (@ 0x48028044) Port 0 Pad Driver Mode 1 Register                      */
//  __I  uint32_t  RESERVED3[6];
//  __I  uint32_t  PDISC;                             /*!< (@ 0x48028060) Port 0 Pin Function Decision Control Register          */
//  __I  uint32_t  RESERVED4[3];
//  __IO uint32_t  PPS;                               /*!< (@ 0x48028070) Port 0 Pin Power Save Register                         */
//  __IO uint32_t  HWSEL;                             /*!< (@ 0x48028074) Port 0 Pin Hardware Select Register                    */
//} PORT0_Type;
///**
//  * @brief Port 14 (PORT14)
//  */

//typedef struct {                                    /*!< (@ 0x48028E00) PORT14 Structure                                       */
//  __IO uint32_t  OUT;                               /*!< (@ 0x48028E00) Port 14 Output Register                                */
//  __O  uint32_t  OMR;                               /*!< (@ 0x48028E04) Port 14 Output Modification Register                   */
//  __I  uint32_t  RESERVED[2];
//  __IO uint32_t  IOCR0;                             /*!< (@ 0x48028E10) Port 14 Input/Output Control Register 0                */
//  __IO uint32_t  IOCR4;                             /*!< (@ 0x48028E14) Port 14 Input/Output Control Register 4                */
//  __IO uint32_t  IOCR8;                             /*!< (@ 0x48028E18) Port 14 Input/Output Control Register 8                */
//  __IO uint32_t  IOCR12;                            /*!< (@ 0x48028E1C) Port 14 Input/Output Control Register 12               */
//  __I  uint32_t  RESERVED1;
//  __I  uint32_t  IN;                                /*!< (@ 0x48028E24) Port 14 Input Register                                 */
//  __I  uint32_t  RESERVED2[14];
//  __IO uint32_t  PDISC;                             /*!< (@ 0x48028E60) Port 14 Pin Function Decision Control Register         */
//  __I  uint32_t  RESERVED3[3];
//  __IO uint32_t  PPS;                               /*!< (@ 0x48028E70) Port 14 Pin Power Save Register                        */
//  __IO uint32_t  HWSEL;                             /*!< (@ 0x48028E74) Port 14 Pin Hardware Select Register                   */
//} PORT14_Type;


///* ================================================================================ */
///* ================                     PORT15                     ================ */
///* ================================================================================ */

//															typedef struct {                                    /*!< (@ 0x48028000) PORT0 Structure                                        */
//  __IO uint32_t  OUT;                               /*!< (@ 0x48028000) Port 0 Output Register                                 */
//  __O  uint32_t  OMR;                               /*!< (@ 0x48028004) Port 0 Output Modification Register                    */
//  __I  uint32_t  RESERVED[2];
//  __IO uint32_t  IOCR0;                             /*!< (@ 0x48028010) Port 0 Input/Output Control Register 0                 */
//  __IO uint32_t  IOCR4;                             /*!< (@ 0x48028014) Port 0 Input/Output Control Register 4                 */
//  __IO uint32_t  IOCR8;                             /*!< (@ 0x48028018) Port 0 Input/Output Control Register 8                 */
//  __IO uint32_t  IOCR12;                            /*!< (@ 0x4802801C) Port 0 Input/Output Control Register 12                */
//  __I  uint32_t  RESERVED1;
//  __I  uint32_t  IN;                                /*!< (@ 0x48028024) Port 0 Input Register                                  */
//  __I  uint32_t  RESERVED2[6];
//  __IO uint32_t  PDR0;                              /*!< (@ 0x48028040) Port 0 Pad Driver Mode 0 Register                      */
//  __IO uint32_t  PDR1;                              /*!< (@ 0x48028044) Port 0 Pad Driver Mode 1 Register                      */
//  __I  uint32_t  RESERVED3[6];
//  __I  uint32_t  PDISC;                             /*!< (@ 0x48028060) Port 0 Pin Function Decision Control Register          */
//  __I  uint32_t  RESERVED4[3];
//  __IO uint32_t  PPS;                               /*!< (@ 0x48028070) Port 0 Pin Power Save Register                         */
//  __IO uint32_t  HWSEL;                             /*!< (@ 0x48028074) Port 0 Pin Hardware Select Register                    */
//} PORT0_Type;

///**
//  * @brief Port 15 (PORT15)
//  */

//typedef struct {                                    /*!< (@ 0x48028F00) PORT15 Structure                                       */
//  __IO uint32_t  OUT;                               /*!< (@ 0x48028F00) Port 15 Output Register                                */
//  __O  uint32_t  OMR;                               /*!< (@ 0x48028F04) Port 15 Output Modification Register                   */
//  __I  uint32_t  RESERVED[2];
//  __IO uint32_t  IOCR0;                             /*!< (@ 0x48028F10) Port 15 Input/Output Control Register 0                */
//  __IO uint32_t  IOCR4;                             /*!< (@ 0x48028F14) Port 15 Input/Output Control Register 4                */
//  __IO uint32_t  IOCR8;                             /*!< (@ 0x48028F18) Port 15 Input/Output Control Register 8                */
//  __I  uint32_t  RESERVED1[2];
//  __I  uint32_t  IN;                                /*!< (@ 0x48028F24) Port 15 Input Register                                 */
//  __I  uint32_t  RESERVED2[14];
//  __IO uint32_t  PDISC;                             /*!< (@ 0x48028F60) Port 15 Pin Function Decision Control Register         */
//  __I  uint32_t  RESERVED3[3];
//  __IO uint32_t  PPS;                               /*!< (@ 0x48028F70) Port 15 Pin Power Save Register                        */
//  __IO uint32_t  HWSEL;                             /*!< (@ 0x48028F74) Port 15 Pin Hardware Select Register                   */
//} PORT15_Type;


//	

#endif

