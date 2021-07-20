/**
 * xml/MibEnum.h is part of Brewken, and is copyright the following authors 2021:
 *   • Matt Young <mfsy@yahoo.com>
 *
 * Brewken is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Brewken is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef XML_MIBENUM_H
#define XML_MIBENUM_H
#pragma once


namespace CharacterSets {
   /**
    * This enum is a standard way of identifying character sets, per the MIME name and "MIBenum" in
    * https://www.iana.org/assignments/character-sets/character-sets.xml
    *
    * To make things work in C++, we have removed '(' and ')' from the IANA names, and replaced '.', '-' and ':' with
    * '_'.
    *
    * MIB stands for Management Information Base, which you can read all about in
    * https://datatracker.ietf.org/doc/html/rfc1213 and https://datatracker.ietf.org/doc/html/rfc1158.  For our purposes,
    * it suffices to know that, for most character sets, there is a well-defined integer identifier (the MIBenum) that we
    * can use to specify "encode in this character set" to Qt.  It seems more satisfactory to use this number than to
    * specify the character set (via https://doc.qt.io/qt-5/qtextcodec.html#codecForMib) than to specify a string name
    * which results in a search for a "best match" (per https://doc.qt.io/qt-5/qtextcodec.html#codecForName-1).
    */
   enum MibEnum {
      US_ASCII = 3,
      ISO_8859_1_1987 = 4,
      ISO_8859_2_1987 = 5,
      ISO_8859_3_1988 = 6,
      ISO_8859_4_1988 = 7,
      ISO_8859_5_1988 = 8,
      ISO_8859_6_1987 = 9,
      ISO_8859_7_1987 = 10,
      ISO_8859_8_1988 = 11,
      ISO_8859_9_1989 = 12,
      ISO_8859_10 = 13,
      ISO_6937_2_add = 14,
      JIS_X0201 = 15,
      JIS_Encoding = 16,
      Shift_JIS = 17,
      Extended_UNIX_Code_Packed_Format_for_Japanese = 18,
      Extended_UNIX_Code_Fixed_Width_for_Japanese = 19,
      BS_4730 = 20,
      SEN_850200_C = 21,
      IT = 22,
      ES = 23,
      DIN_66003 = 24,
      NS_4551_1 = 25,
      NF_Z_62_010 = 26,
      ISO_10646_UTF_1 = 27,
      ISO_646_basic_1983 = 28,
      INVARIANT = 29,
      ISO_646_irv_1983 = 30,
      NATS_SEFI = 31,
      NATS_SEFI_ADD = 32,
      NATS_DANO = 33,
      NATS_DANO_ADD = 34,
      SEN_850200_B = 35,
      KS_C_5601_1987 = 36,
      ISO_2022_KR = 37,
      EUC_KR = 38,
      ISO_2022_JP = 39,
      ISO_2022_JP_2 = 40,
      JIS_C6220_1969_jp = 41,
      JIS_C6220_1969_ro = 42,
      PT = 43,
      greek7_old = 44,
      latin_greek = 45,
      NF_Z_62_010_1973 = 46,
      Latin_greek_1 = 47,
      ISO_5427 = 48,
      JIS_C6226_1978 = 49,
      BS_viewdata = 50,
      INIS = 51,
      INIS_8 = 52,
      INIS_cyrillic = 53,
      ISO_5427_1981 = 54,
      ISO_5428_1980 = 55,
      GB_1988_80 = 56,
      GB_2312_80 = 57,
      NS_4551_2 = 58,
      videotex_suppl = 59,
      PT2 = 60,
      ES2 = 61,
      MSZ_7795_3 = 62,
      JIS_C6226_1983 = 63,
      greek7 = 64,
      ASMO_449 = 65,
      iso_ir_90 = 66,
      JIS_C6229_1984_a = 67,
      JIS_C6229_1984_b = 68,
      JIS_C6229_1984_b_add = 69,
      JIS_C6229_1984_hand = 70,
      JIS_C6229_1984_hand_add = 71,
      JIS_C6229_1984_kana = 72,
      ISO_2033_1983 = 73,
      ANSI_X3_110_1983 = 74,
      T_61_7bit = 75,
      T_61_8bit = 76,
      ECMA_cyrillic = 77,
      CSA_Z243_4_1985_1 = 78,
      CSA_Z243_4_1985_2 = 79,
      CSA_Z243_4_1985_gr = 80,
      ISO_8859_6_E = 81,
      ISO_8859_6_I = 82,
      T_101_G2 = 83,
      ISO_8859_8_E = 84,
      ISO_8859_8_I = 85,
      CSN_369103 = 86,
      JUS_I_B1_002 = 87,
      IEC_P27_1 = 88,
      JUS_I_B1_003_serb = 89,
      JUS_I_B1_003_mac = 90,
      greek_ccitt = 91,
      NC_NC00_10_81 = 92,
      ISO_6937_2_25 = 93,
      GOST_19768_74 = 94,
      ISO_8859_supp = 95,
      ISO_10367_box = 96,
      latin_lap = 97,
      JIS_X0212_1990 = 98,
      DS_2089 = 99,
      us_dk = 100,
      dk_us = 101,
      KSC5636 = 102,
      UNICODE_1_1_UTF_7 = 103,
      ISO_2022_CN = 104,
      ISO_2022_CN_EXT = 105,
      UTF_8 = 106,
      ISO_8859_13 = 109,
      ISO_8859_14 = 110,
      ISO_8859_15 = 111,
      ISO_8859_16 = 112,
      GBK = 113,
      GB18030 = 114,
      OSD_EBCDIC_DF04_15 = 115,
      OSD_EBCDIC_DF03_IRV = 116,
      OSD_EBCDIC_DF04_1 = 117,
      ISO_11548_1 = 118,
      KZ_1048 = 119,
      ISO_10646_UCS_2 = 1000,
      ISO_10646_UCS_4 = 1001,
      ISO_10646_UCS_Basic = 1002,
      ISO_10646_Unicode_Latin1 = 1003,
      ISO_10646_J_1 = 1004,
      ISO_Unicode_IBM_1261 = 1005,
      ISO_Unicode_IBM_1268 = 1006,
      ISO_Unicode_IBM_1276 = 1007,
      ISO_Unicode_IBM_1264 = 1008,
      ISO_Unicode_IBM_1265 = 1009,
      UNICODE_1_1 = 1010,
      SCSU = 1011,
      UTF_7 = 1012,
      UTF_16BE = 1013,
      UTF_16LE = 1014,
      UTF_16 = 1015,
      CESU_8 = 1016,
      UTF_32 = 1017,
      UTF_32BE = 1018,
      UTF_32LE = 1019,
      BOCU_1 = 1020,
      UTF_7_IMAP = 1021,
      ISO_8859_1_Windows_3_0_Latin_1 = 2000,
      ISO_8859_1_Windows_3_1_Latin_1 = 2001,
      ISO_8859_2_Windows_Latin_2 = 2002,
      ISO_8859_9_Windows_Latin_5 = 2003,
      hp_roman8 = 2004,
      Adobe_Standard_Encoding = 2005,
      Ventura_US = 2006,
      Ventura_International = 2007,
      DEC_MCS = 2008,
      IBM850 = 2009,
      IBM852 = 2010,
      IBM437 = 2011,
      PC8_Danish_Norwegian = 2012,
      IBM862 = 2013,
      PC8_Turkish = 2014,
      IBM_Symbols = 2015,
      IBM_Thai = 2016,
      HP_Legal = 2017,
      HP_Pi_font = 2018,
      HP_Math8 = 2019,
      Adobe_Symbol_Encoding = 2020,
      HP_DeskTop = 2021,
      Ventura_Math = 2022,
      Microsoft_Publishing = 2023,
      Windows_31J = 2024,
      GB2312 = 2025,
      Big5 = 2026,
      macintosh = 2027,
      IBM037 = 2028,
      IBM038 = 2029,
      IBM273 = 2030,
      IBM274 = 2031,
      IBM275 = 2032,
      IBM277 = 2033,
      IBM278 = 2034,
      IBM280 = 2035,
      IBM281 = 2036,
      IBM284 = 2037,
      IBM285 = 2038,
      IBM290 = 2039,
      IBM297 = 2040,
      IBM420 = 2041,
      IBM423 = 2042,
      IBM424 = 2043,
      IBM500 = 2044,
      IBM851 = 2045,
      IBM855 = 2046,
      IBM857 = 2047,
      IBM860 = 2048,
      IBM861 = 2049,
      IBM863 = 2050,
      IBM864 = 2051,
      IBM865 = 2052,
      IBM868 = 2053,
      IBM869 = 2054,
      IBM870 = 2055,
      IBM871 = 2056,
      IBM880 = 2057,
      IBM891 = 2058,
      IBM903 = 2059,
      IBM904 = 2060,
      IBM905 = 2061,
      IBM918 = 2062,
      IBM1026 = 2063,
      EBCDIC_AT_DE = 2064,
      EBCDIC_AT_DE_A = 2065,
      EBCDIC_CA_FR = 2066,
      EBCDIC_DK_NO = 2067,
      EBCDIC_DK_NO_A = 2068,
      EBCDIC_FI_SE = 2069,
      EBCDIC_FI_SE_A = 2070,
      EBCDIC_FR = 2071,
      EBCDIC_IT = 2072,
      EBCDIC_PT = 2073,
      EBCDIC_ES = 2074,
      EBCDIC_ES_A = 2075,
      EBCDIC_ES_S = 2076,
      EBCDIC_UK = 2077,
      EBCDIC_US = 2078,
      UNKNOWN_8BIT = 2079,
      MNEMONIC = 2080,
      MNEM = 2081,
      VISCII = 2082,
      VIQR = 2083,
      KOI8_R = 2084,
      HZ_GB_2312 = 2085,
      IBM866 = 2086,
      IBM775 = 2087,
      KOI8_U = 2088,
      IBM00858 = 2089,
      IBM00924 = 2090,
      IBM01140 = 2091,
      IBM01141 = 2092,
      IBM01142 = 2093,
      IBM01143 = 2094,
      IBM01144 = 2095,
      IBM01145 = 2096,
      IBM01146 = 2097,
      IBM01147 = 2098,
      IBM01148 = 2099,
      IBM01149 = 2100,
      Big5_HKSCS = 2101,
      IBM1047 = 2102,
      PTCP154 = 2103,
      Amiga_1251 = 2104,
      KOI7_switched = 2105,
      BRF = 2106,
      TSCII = 2107,
      CP51932 = 2108,
      windows_874 = 2109,
      windows_1250 = 2250,
      windows_1251 = 2251,
      windows_1252 = 2252,
      windows_1253 = 2253,
      windows_1254 = 2254,
      windows_1255 = 2255,
      windows_1256 = 2256,
      windows_1257 = 2257,
      windows_1258 = 2258,
      TIS_620 = 2259,
      CP50220 = 2260
   };
}

#endif
