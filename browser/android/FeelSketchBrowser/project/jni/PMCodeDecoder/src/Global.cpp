/**
***                  "Feel Sketch" PMCode Encoder & Decoder.
***    Copyright (C) 2009, Content Idea of ASIA Co.,Ltd. (oss.pmcode@ci-a.com)
***
***    This program is free software: you can redistribute it and/or modify
***    it under the terms of the GNU General Public License as published by
***    the Free Software Foundation, either version 3 of the License, or
***    (at your option) any later version.
***
***    This program is distributed in the hope that it will be useful,
***    but WITHOUT ANY WARRANTY; without even the implied warranty of
***    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
***    GNU General Public License for more details.
***
***    You should have received a copy of the GNU General Public License
***    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "define.h"
#include "Global.h"
#include "mathematics.h"
#include <ctype.h>

long CalcPitch (short BitCount,long Width) {

	long lPitch;
	if (BitCount < BIT_COUNT_8) {
		int n = BIT_COUNT_8 / BitCount;
		lPitch = (Width + n - 1) / n;
	} else {
		lPitch = BitCount / 8;
		if (Width % 4 == 0) {
			return Width * lPitch;
		} else {
			return Width * lPitch + (4 - (Width * lPitch) % 4);
		}
	}
	return (lPitch + 3) &~3;
}



char	g_CurrentPath [MAX_PATH];
int		g_MaskSetting [MAX_LAYERSIZE];
long	g_ColorCodeSettingDefinition [MAX_LAYERSIZE];
long	g_ColorCodeSetting3 [3];
long	g_ColorCodeSetting4to9 [9];
long	g_ColorCodeSetting10to24 [MAX_LAYERSIZE];
long	g_ColorCodeTable [COLORTABLE_SIZE + 1];
int		g_ColorCodeTableSize;
char	g_IniFilePath [MAX_PATH];
int		g_SelectMenuCode;
int		g_ColorCodeSetting;
int		g_ThreeValueThreshold;
int		g_ColorCodeThreshold;
int		g_CorrectSlopeThreshold;
int		g_CorrectColorThreshold;
BYTE	g_byModuleData[MAX_MODULESIZE][MAX_MODULESIZE];
long	g_PMCodeColor [COLORTABLE_SIZE + 1];
int		g_iLevelingRate;
int		g_iCheckFileOutput;

QR_VERSIONINFO QR_VersonInfo[41] = {{0}, 
										 { 1, // Ver.1
										    23,   16,   13,   10,    6,
										   0,   0,   0,   0,   0,   0,   0,
										   1,  23,  16,
										   1,  23,  13,
										   1,  23,  10,
										   1,  23,   6,
										   0,   0,   0,
										   0,   0,   0,
										   0,   0,   0,
										   0,   0,   0},
										 { 2, // Ver.2
										    41,   31,   25,   19,   13,
										   1,  18,   0,   0,   0,   0,   0,
										   1,  41,  31,
										   1,  41,  25,
										   1,  41,  19,
										   1,  41,  13,
										   0,   0,   0,
										   0,   0,   0,
										   0,   0,   0,
										   0,   0,   0},
										 { 3, // Ver.3
										    61,   46,   35,   25,   17,
										   1,  22,   0,   0,   0,   0,   0,
										   1,  61,  46,
										   1,  61,  35,
										   1,  32,  14,
										   1,  32,  10,
										   0,   0,   0,
										   0,   0,   0,
										   1,  29,  11,
										   1,  29,   7},
										 { 4, // Ver.4
										   91,   71,   55,   39,   27,
										   1,  26,   0,   0,   0,   0,   0,
										   1,  91,  71,
										   1,  47,  29,
										   1,  47,  21,
										   3,  23,   7,
										   0,   0,   0,
										   1,  44,  26,
										   1,  44,  18,
										   1,  22,   6},
										 { 5, // Ver.5
										   125,   99,   77,   53,   37,
										   1,  30,   0,   0,   0,   0,   0,
										   1, 125,  99,
										   1,  64,  40,
										   3,  31,  13,
										   3,  31,   9,
										   0,   0,   0,
										   1,  61,  37,
										   1,  32,  14,
										   1,  32,  10},
										 { 6, // Ver.6
										   167,  131, 103,   71,   55,
										   1,  34,   0,   0,   0,   0,   0,
										   1,  83,  65,
										   3,  42,  26,
										   3,  42,  18,
										   3,  42,  14,
										   1,  84,  66,
										   1,  41,  25,
										   1,  41,  17,
										   1,  41,  13},
										 { 7, // Ver.7
										   191,  151,  119,  83,   61,
										   2,  22,  38,   0,   0,   0,   0,
										   1,  95,  75,
										   1,  50,  32,
										   5,  32,  14,
										   4,  38,  12,
										   1,  96,  76,
										   3,  47,  29,
										   1,  31,  13,
										   1,  39,  13},
										 { 8, // Ver.8
										   237,  189,  149,  105,   81,
										   2,  24,  42,   0,   0,   0,   0,
										   1, 118,  94,
										   3,  59,  37,
										   5,  39,  17,
										   5,  39,  13,
										   1, 119,  95,
										   1,  60,  38,
										   1,  32,  20,
										   1,  42,  16},
										 { 9, // Ver.9
										   287,  227,  177,  127,  95,
										   2,  26,  46,   0,   0,   0,   0,
										   1, 143, 113,
										   3,  57,  35,
										   7,  36,  16,
										   7,  36,  12,
										   1, 144, 114,
										   2,  58,  36,
										   1,  35,  15,
										   1,  35,  11},
										 {10, // Ver.10
										   341,  269,  211,  149,  117,
										   2,  28,  50,   0,   0,   0,   0,
										   3,  85,  67,
										   4,  68,  42,
										   5,  43,  19,
										   5,  43,  15,
										   1,  86,  68,
										   1,  69,  43,
										   3,  42,  18,
										   3,  42,  14},
										 {11, // Ver.11
										   399,  319,  249,  175,  135,
										   2,  30,  54,   0,   0,   0,   0,
										   3, 100,  80,
										   1,  79,  49,
										   7,  50,  22,
										   8,  36,  12,
										   1,  99,  79,
										   4,  80,  50,
										   1,  49,  21,
										   3,  37,  13},
										 {12, // Ver.12
										   461,  365,  285,  201,  153,
										   2,  32,  58,   0,   0,   0,   0,
										   3, 115,  91,
										   5,  58,  36,
										   9,  46,  20,
										  10,  42,  14,
										   1, 116,  92,
										   3,  57,  35,
										   1,  47,  21,
										   1,  41,  13},
										 {13, // Ver.13
										   527,  423,  329,  239,  175,
										   2,  34,  62,   0,   0,   0,   0,
										   3, 132, 106,
										   5,  59,  37,
										  11,  44,  20,
										  15,  33,  11,
										   1, 131, 105,
										   4,  58,  36,
										   1,  43,  19,
										   1,  32,  10},
										 {14, // Ver.14
										   576,  456,  360,  256,  192,
										   3,  26,  46,  66,   0,   0,   0,
										   4, 144, 114,
										   9,  64,  40,
										  16,  36,  16,
										  16,  36,  12,
										   0,   0,   0,
										   0,   0,   0,
										   0,   0,   0,
										   0,   0,   0},
										 {15, // Ver.15
										   650,  518,  410,  290,  218,
										   3,  26,  48,  70,   0,   0,   0,
										   2, 109,  87,
										  10,  65,  41,
										  10,  54,  24,
										  16,  36,  12,
										   4, 108,  86,
										   0,   0,   0,
										   2,  55,  25,
										   2,  37,  13},
										 {16, // Ver.16
										   728,  584,  448,  320,  248,
										   3,  26,  50,  74,   0,   0,   0,
										   2, 122,  98,
										   8,  73,  45,
										  14,  43,  19,
										   4,  44,  14,
										   4, 121,  97,
										   2,  72,  44,
										   3,  42,  18,
										  12,  46,  16},
										 {17, // Ver.17
										   810,  642,  502,  362,  278,
										   3,  30,  54,  78,   0,   0,   0,
										   6, 135, 107,
										   7,  74,  46,
										   6,  50,  22,
										   7,  42,  14,
										   0,   0,   0,
										   4,  73,  45,
										  10,  51,  23,
										  12,  43,  15},
										 {18, // Ver.18
										   896,  716,  558,  392,  308,
										   3,  30,  56,  82,   0,   0,   0,
										   2, 150, 120,
										  12,  69,  43,
										  14,  50,  22,
										   7,  42,  14,
										   4, 149, 119,
										   1,  68,  42,
										   4,  49,  21,
										  14,  43,  15},
										 {19, // Ver.19
										   986,  790,  622,  440,  336,
										   3,  30,  58,  86,   0,   0,   0,
										   6, 141, 113,
										   8,  70,  44,
										  20,  47,  21,
										  14,  39,  13,
										   1, 140, 112,
										   6,  71,  45,
										   1,  46,  20,
										  11,  40,  14},
										 {20, // Ver.20
										  1080,  856,  664,  480,  380,
										   3,  34,  62,  90,   0,   0,   0,
										   8, 135, 107,
										   8,  67,  41,
										  20,  54,  24,
										  20,  43,  15,
										   0,   0,   0,
										   8,  68,  42,
										   0,   0,   0,
										   5,  44,  16},
										 {21, // Ver.21
										  1151,  927,  709,  507,  401,
										   4,  28,  50,  72,  94,   0,   0,
										   7, 144, 116,
										  12,  68,  42,
										  22,  50,  22,
										  24,  46,  16,
										   1, 143, 115,
										   5,  67,  41,
										   1,  51,  23,
										   1,  47,  17},
										 {22, // Ver.22
										  1253, 1001,  777,  563,  437,
										   4,  26,  50,  74,  98,   0,   0,
										   7, 139, 111,
										   5,  73,  45,
										  12,  54,  24,
										  29,  37,  13,
										   2, 140, 112,
										  12,  74,  46,
										  11,  55,  25,
										   5,  36,  12},
										 {23, // Ver.23
										  1359, 1089,  855,  609,  459,
										   4,  30,  54,  78, 102,   0,   0,
										   9, 151, 121,
										   9,  75,  47,
										  16,  54,  24,
										  21,  45,  15,
										   0,   0,   0,
										   9,  76,  48,
										   9,  55,  25,
										   9,  46,  16},
										 {24, // Ver.24
										  1469, 1169,  909,  659,  509,
										   4,  28,  54,  80, 106,   0,   0,
										   9, 147, 117,
										  11,  73,  45,
										  16,  54,  24,
										  29,  46,  16,
										   1, 146, 116,
										   9,  74,  46,
										  11,  55,  25,
										   3,  45,  15},
										 {25, // Ver.25
										  1583, 1271, 995,  713,  533,
										   4,  32,  58,  84, 110,   0,   0,
										  11, 132, 106,
										  13,  75,  47,
										  12,  54,  24,
										  27,  45,  15,
										   1, 131, 105,
										   8,  76,  48,
										  17,  55,  25,
										   8,  46,  16},
										 {26, // Ver.26
										  1701, 1365, 1057,  749,  591,
										   4,  30,  58,  86, 114,   0,   0,
										   3, 141, 113,
										  22,  74,  46,
										  33,  50,  22,
										  36,  46,  16,
										   9, 142, 114,
										   1,  73,  45,
										   1,  51,  23,
										   1,  45,  15},
										 {27, // Ver.27
										  1823, 1463, 1123,  803,  623,
										   4,  34,  62,  90, 118,   0,   0,
										  11, 152, 122,
										  23,  73,  45,
										  13,  53,  23,
										  17,  45,  15,
										   1, 151, 121,
										   2,  72,  44,
										  21,  54,  24,
										  23,  46,  16},
										 {28, // Ver.28
										  1916, 1526, 1188,  866,  656,
										   5,  26,  50,  74,  98, 122,   0,
										   8, 147, 117,
										   8,  73,  45,
										   9,  54,  24,
										  16,  45,  15,
										   5, 148, 118,
										  18,  74,  46,
										  26,  55,  25,
										  26,  46,  16},
										 {29, // Ver.29
										  2046, 1626, 1262,  906,  696,
										   5,  30,  54,  78, 102, 126,   0,
										  12, 146, 116,
										  26,  73,  45,
										   6,  53,  23,
										  24,  45,  15,
										   2, 147, 117,
										   2,  74,  46,
										  32,  54,  24,
										  21,  46,  16},
										 {30, // Ver.30
										  2180, 1730, 1368,  980,  740,
										   5,  26,  52,  78, 104, 130,   0,
										  10, 145, 115,
										  24,  75,  47,
										  20,  54,  24,
										  28,  45,  15,
										   5, 146, 116,
										   5,  76,  48,
										  20,  55,  25,
										  20,  46,  16},
										 {31, // Ver.31
										  2318, 1838, 1450, 1028,  788,
										   5,  30,  56,  82, 108, 134,   0,
										  14, 145, 115,
										   7,  74,  46,
										   4,  53,  23,
										  28,  45,  15,
										   2, 144, 114,
										  24,  75,  47,
										  39,  54,  24,
										  23,  46,  16},
										 {32, // Ver.32
										  2460, 1950, 1536, 1110,  840,
										   5,  34,  60,  86, 112, 138,   0,
										   5, 144, 114,
										  15,  74,  46,
										  15,  54,  24,
										  24,  45,  15,
										  12, 145, 115,
										  18,  75,  47,
										  30,  55,  25,
										  30,  46,  16},
										 {33, // Ver.33
										  2606, 2066, 1626, 1166,  896,
										   5,  30,  58,  86, 114, 142,   0,
										   4, 144, 114,
										  19,  74,  46,
										  34,  54,  24,
										  16,  45,  15,
										  14, 145, 115,
										  16,  75,  47,
										  14,  55,  25,
										  41,  46,  16},
										 {34, // Ver.34
										  2756, 2186, 1720, 1226,  956,
										   5,  34,  62,  90, 118, 146,   0,
										  18, 145, 115,
										  19,  74,  46,
										  49,  54,  24,
										  56,  46,  16,
										   1, 146, 116,
										  18,  75,  47,
										   2,  55,  25,
										   4,  45,  15},
										 {35, // Ver.35
										  2871, 2301, 1807, 1281,  981,
										   6,  30,  54,  78, 102, 126, 150,
										  17, 151, 121,
										  17,  75,  47,
										  44,  54,  24,
										  27,  45,  15,
										   2, 152, 122,
										  21,  76,  48,
										   9,  55,  25,
										  36,  46,  16},
										 {36, // Ver.36
										  3029, 2429, 1909, 1349, 1049,
										   6,  24,  50,  76, 102, 128, 154,
										  11, 151, 121,
										  11,  75,  47,
										  51,  54,  24,
										   7,  45,  15,
										   9, 152, 122,
										  29,  76,  48,
										   5,  55,  25,
										  59,  46,  16},
										 {37, // Ver.37
										  3191, 2561, 1987, 1421, 1091,
										   6,  28,  54,  80, 106, 132, 158,
										  20, 152, 122,
										  34,  74,  46,
										  54,  54,  24,
										  29,  45,  15,
										   1, 151, 121,
										   9,  75,  47,
										   5,  55,  25,
										  41,  46,  16},
										 {38, // Ver.38
										  3357, 2697, 2097, 1497, 1137,
										   6,  32,  58,  84, 110, 136, 162,
										   9, 152, 122,
										  18,  74,  46,
										  53,  54,  24,
										  47,  45,  15,
										  13, 153, 123,
										  27,  75,  47,
										   9,  55,  25,
										  27,  46,  16},
										 {39, // Ver.39
										  3527, 2807, 2211, 1577, 1217,
										   6,  26,  54,  82, 110, 138, 166,
										  23, 147, 117,
										  45,  75,  47,
										  48,  54,  24,
										  15,  45,  15,
										   1, 146, 116,
										   2,  76,  48,
										  17,  55,  25,
										  62,  46,  16},
										 {40, // Ver.40
										  3701, 2951, 2329, 1661, 1271,
										   6,  30,  58,  86, 114, 142, 170,
										  24, 148, 118,
										  23,  75,  47,
										  39,  54,  24,
										  25,  45,  15,
										   1, 149, 119,
										  26,  76,  48,
										  29,  55,  25,
										  56,  46,  16}
										};

char g_szLayerSetting [22][12] = { '\x00', '\x00', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\x00', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\xFF', '\xFF', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\x00', '\x00', '\xFF', '\x00', '\x00', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\x00', '\xFF', '\x00', '\x00', '\x00', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\xFF', '\x00', '\x00', '\x00', '\x00', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\xFF', '\xFF', '\xFF', '\x00', '\x00', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\x00', '\x00', '\xFF', '\x00', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\x00', '\xFF', '\x00', '\x00', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\xFF', '\x00', '\x00', '\x00', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\xFF', '\xFF', '\xFF', '\x00', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\x00', '\x00', '\xFF', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\x00', '\xFF', '\x00', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\xFF', '\x00', '\x00', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\xFF', '\xFF', '\xFF', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\x00', '\x00', '\xFF', '\xFF', '\xFF', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\x00', '\xFF', '\x00', '\xFF', '\xFF', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\xFF', '\x00', '\x00', '\xFF', '\xFF', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
								, '\x00', '\x00', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\xFF', '\x00', '\x00', '\x00'
								, '\x00', '\xFF', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\xFF', '\x00', '\x00', '\x00'
								 };


#define ASCINT_ERROR        -1L
#define ASCINT_ON            1
#define ASCINT_OFF            0

LONG AscHexToLong(PSTR pszData, int ilength)
{
    LONG    lData = 0L;
    BYTE    cbPick;
    int     iErr = ASCINT_ON;

	for (int i = 0; i < ilength; i ++) {
        cbPick = (BYTE)*pszData;
        if((cbPick >= '0') && (cbPick <= '9'))
            cbPick &= 0x0f;
        else{
            if(isalpha(cbPick)){
                cbPick = ((cbPick & 0x0f) - 1) + 10;
                if(cbPick > 0x0f)
                    iErr = ASCINT_ERROR;
            }
        }
        if((iErr == ASCINT_ON) && (*pszData++ != '\0')){
            lData = lData * 16L + (LONG)cbPick;
        }
 	}
    if(iErr == ASCINT_ERROR){
        lData = ASCINT_ERROR;
    }
    return lData;
}

void b1th2( unsigned char dat, char *p )
{
	static	char	hextb[] = "0123456789ABCDEF";

	*p     = hextb[ (dat >> 4) ];
	*(p+1) = hextb[ (dat & 0x0f) ];
}

unsigned char h2tb1( char *p )
{
	unsigned char	d1,d2;

	d1 = *p;
	d2 = *(p+1);
	if( d1 >= 'A' ) d1 += 9;
	if( d2 >= 'A' ) d2 += 9;
	return( ( d1 << 4 ) | ( d2 & 0x0f ) );
}


void MakeColorTable (int iColor, int iLayer) {
	
	long	lBaseColor [MAX_LAYERSIZE];
	long	lColorCode;
	int		iColorCount = 0;
	int		iColorCompound = 0;
	int		iBits;
	int		idiff = 0;

	for (int i = 0; i < MAX_LAYERSIZE; i ++) {
		lBaseColor [i] = 0;
	}
	for (int i = 0; i < COLORTABLE_SIZE + 1; i ++) {
		g_ColorCodeTable [i] = 0;
	}

	for (int i = 0; i < iLayer; i ++) {
		if (g_ColorCodeSetting == 0) {
			lColorCode = g_ColorCodeSettingDefinition [i];
		} else {
			if (iLayer == 3) {
				lColorCode = g_ColorCodeSetting3 [i];
			} else if (iLayer > 9) {
				lColorCode = g_ColorCodeSetting10to24 [i];
			} else {
				lColorCode = g_ColorCodeSetting4to9 [i];
			}
		}
		lBaseColor [i]			= lColorCode;
		g_ColorCodeTable [i]	= lColorCode;

	}
	iColorCompound = pow ((double)2, iColor) - 1;
	iColorCount = iColor;
	for (int i = 1; i <= iColorCompound; i ++){
		if ((0x01 << idiff) == i) {
			idiff ++;
		} else {
			for (int j = 0; j < iColor; j ++) {
				iBits = ((i >> j) & 0x01);
				if (iBits == 0x01) {
					g_ColorCodeTable [iColorCount] += lBaseColor [j];
				}
			}
			iColorCount ++;
		}
	}
	g_ColorCodeTable [iColorCount]		= 0;
	g_ColorCodeTable [iColorCount + 1]	= 0xFFFFFF;
	g_ColorCodeTable [iColorCount + 2]	= 0x7F7F7F;
	g_ColorCodeTableSize = iColorCount + 3;
}
