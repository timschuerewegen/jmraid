#include "jmraid.h"

#include <stdlib.h>
#include <string.h>

#ifdef DEBUG_PRINT
#include <stdio.h>
extern void debug_print(const char* format, ...);
#else
#define debug_print(...)
#endif

static const uint32_t TABLE_CRC_FAST[256] =
{
	0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9,
	0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005,
	0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61,
	0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD,
	0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9,
	0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75,
	0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011,
	0x791D4014, 0x7DDC5DA3, 0x709F7B7A, 0x745E66CD,
	0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039,
	0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5,
	0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81,
	0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D,
	0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49,
	0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95,
	0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1,
	0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D,
	0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE,
	0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072,
	0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16,
	0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA,
	0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE,
	0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02,
	0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1, 0x53DC6066,
	0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA,
	0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E,
	0xBFA1B04B, 0xBB60ADFC, 0xB6238B25, 0xB2E29692,
	0x8AAD2B2F, 0x8E6C3698, 0x832F1041, 0x87EE0DF6,
	0x99A95DF3, 0x9D684044, 0x902B669D, 0x94EA7B2A,
	0xE0B41DE7, 0xE4750050, 0xE9362689, 0xEDF73B3E,
	0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2,
	0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686,
	0xD5B88683, 0xD1799B34, 0xDC3ABDED, 0xD8FBA05A,
	0x690CE0EE, 0x6DCDFD59, 0x608EDB80, 0x644FC637,
	0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB,
	0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F,
	0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53,
	0x251D3B9E, 0x21DC2629, 0x2C9F00F0, 0x285E1D47,
	0x36194D42, 0x32D850F5, 0x3F9B762C, 0x3B5A6B9B,
	0x0315D626, 0x07D4CB91, 0x0A97ED48, 0x0E56F0FF,
	0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623,
	0xF12F560E, 0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7,
	0xE22B20D2, 0xE6EA3D65, 0xEBA91BBC, 0xEF68060B,
	0xD727BBB6, 0xD3E6A601, 0xDEA580D8, 0xDA649D6F,
	0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604, 0xC960EBB3,
	0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7,
	0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B,
	0x9B3660C6, 0x9FF77D71, 0x92B45BA8, 0x9675461F,
	0x8832161A, 0x8CF30BAD, 0x81B02D74, 0x857130C3,
	0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640,
	0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C,
	0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8,
	0x68860BFD, 0x6C47164A, 0x61043093, 0x65C52D24,
	0x119B4BE9, 0x155A565E, 0x18197087, 0x1CD86D30,
	0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC,
	0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088,
	0x2497D08D, 0x2056CD3A, 0x2D15EBE3, 0x29D4F654,
	0xC5A92679, 0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0,
	0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB, 0xDBEE767C,
	0xE3A1CBC1, 0xE760D676, 0xEA23F0AF, 0xEEE2ED18,
	0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4,
	0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0,
	0x9ABC8BD5, 0x9E7D9662, 0x933EB0BB, 0x97FFAD0C,
	0xAFB010B1, 0xAB710D06, 0xA6322BDF, 0xA2F33668,
	0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4
};

static void write_u32_le(uint8_t *p, uint32_t d)
{
	p[0] = (uint8_t)(d >> 0);
	p[1] = (uint8_t)(d >> 8);
	p[2] = (uint8_t)(d >> 16);
	p[3] = (uint8_t)(d >> 24);
}

static uint16_t read_u16_le(const uint8_t *p)
{
	return (p[0] << 0) | (p[1] << 8);
}

static uint32_t read_u32_le(const uint8_t *p)
{
	return (p[0] << 0) | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}

static uint32_t calc_crc_fast(const uint8_t *data, uint32_t size)
{
	uint32_t crc;
	uint32_t i;

	crc = 0x52325032;
	for (i = 0; i < size; i += 4)
	{
		crc = TABLE_CRC_FAST[(data[i + 3] ^ (crc >> 24)) & 0xFF] ^ (crc << 8);
		crc = TABLE_CRC_FAST[(data[i + 2] ^ (crc >> 24)) & 0xFF] ^ (crc << 8);
		crc = TABLE_CRC_FAST[(data[i + 1] ^ (crc >> 24)) & 0xFF] ^ (crc << 8);
		crc = TABLE_CRC_FAST[(data[i + 0] ^ (crc >> 24)) & 0xFF] ^ (crc << 8);
	}

	return crc;
}

static void scramble(const uint8_t *data_in, uint8_t *data_out, uint32_t size)
{
	uint32_t v3;
	uint32_t crc;
	uint32_t data;
	int i;
	uint8_t data_bit[32];
	uint8_t new_crc_bit[32];
	uint8_t crc_bit[32];

	crc = 0x52325032;
	for (v3 = 0; v3 < size >> 2; v3++)
	{
		for (i = 0; i < 32; i++)
		{
			crc_bit[i] = (uint8_t)((crc >> i) & 1);
		}

		data = ((uint32_t *)data_in)[v3];
		for (i = 0; i < 32; i++)
		{
			data_bit[i] = (uint8_t)((data >> i) & 1);
		}

		data_bit[31] = data_bit[31] ^ crc_bit[27];
		data_bit[30] = data_bit[30] ^ crc_bit[30];
		data_bit[29] = data_bit[29] ^ crc_bit[23];
		data_bit[28] = data_bit[28] ^ crc_bit[18];
		data_bit[27] = data_bit[27] ^ crc_bit[8];
		data_bit[26] = data_bit[26] ^ crc_bit[25];
		data_bit[25] = data_bit[25] ^ crc_bit[3];
		data_bit[24] = data_bit[24] ^ crc_bit[29];
		data_bit[23] = data_bit[23] ^ crc_bit[9];
		data_bit[22] = data_bit[22] ^ crc_bit[17];
		data_bit[21] = data_bit[21] ^ crc_bit[1];
		data_bit[20] = data_bit[20] ^ crc_bit[22];
		data_bit[19] = data_bit[19] ^ crc_bit[10];
		data_bit[18] = data_bit[18] ^ crc_bit[20];
		data_bit[17] = data_bit[17] ^ crc_bit[5];
		data_bit[16] = data_bit[16] ^ crc_bit[12];
		data_bit[15] = data_bit[15] ^ crc_bit[28];
		data_bit[14] = data_bit[14] ^ crc_bit[14];
		data_bit[13] = data_bit[13] ^ crc_bit[2];
		data_bit[12] = data_bit[12] ^ crc_bit[24];
		data_bit[11] = data_bit[11] ^ crc_bit[15];
		data_bit[10] = data_bit[10] ^ crc_bit[6];
		data_bit[9] = data_bit[9] ^ crc_bit[26];
		data_bit[8] = data_bit[8] ^ crc_bit[4];
		data_bit[7] = data_bit[7] ^ crc_bit[19];
		data_bit[6] = data_bit[6] ^ crc_bit[0];
		data_bit[5] = data_bit[5] ^ crc_bit[16];
		data_bit[4] = data_bit[4] ^ crc_bit[7];
		data_bit[3] = data_bit[3] ^ crc_bit[21];
		data_bit[2] = data_bit[2] ^ crc_bit[13];
		data_bit[1] = data_bit[1] ^ crc_bit[31];
		data_bit[0] = data_bit[0] ^ crc_bit[11];

		data = 0;
		for (i = 31; i >= 0; i--)
		{
			data = (data << 1) | data_bit[i];
		}
		((uint32_t *)data_out)[v3] = data;

		new_crc_bit[31] = crc_bit[5] ^ crc_bit[8] ^ crc_bit[9] ^ crc_bit[11] ^ crc_bit[15] ^ crc_bit[23] ^ crc_bit[24] ^ crc_bit[25] ^ crc_bit[27] ^ crc_bit[28] ^ crc_bit[29] ^ crc_bit[30] ^ crc_bit[31];
		new_crc_bit[30] = crc_bit[4] ^ crc_bit[7] ^ crc_bit[8] ^ crc_bit[10] ^ crc_bit[14] ^ crc_bit[22] ^ crc_bit[23] ^ crc_bit[24] ^ crc_bit[26] ^ crc_bit[27] ^ crc_bit[28] ^ crc_bit[29] ^ crc_bit[30];
		new_crc_bit[29] = crc_bit[3] ^ crc_bit[6] ^ crc_bit[7] ^ crc_bit[9] ^ crc_bit[13] ^ crc_bit[21] ^ crc_bit[22] ^ crc_bit[23] ^ crc_bit[25] ^ crc_bit[26] ^ crc_bit[27] ^ crc_bit[29] ^ crc_bit[31] ^ crc_bit[28];
		new_crc_bit[28] = crc_bit[2] ^ crc_bit[5] ^ crc_bit[6] ^ crc_bit[8] ^ crc_bit[12] ^ crc_bit[20] ^ crc_bit[21] ^ crc_bit[22] ^ crc_bit[24] ^ crc_bit[25] ^ crc_bit[27] ^ crc_bit[28] ^ crc_bit[30] ^ crc_bit[26];
		new_crc_bit[27] = crc_bit[1] ^ crc_bit[4] ^ crc_bit[5] ^ crc_bit[7] ^ crc_bit[11] ^ crc_bit[19] ^ crc_bit[20] ^ crc_bit[21] ^ crc_bit[23] ^ crc_bit[24] ^ crc_bit[25] ^ crc_bit[26] ^ crc_bit[27] ^ crc_bit[29];
		new_crc_bit[26] = crc_bit[0] ^ crc_bit[3] ^ crc_bit[4] ^ crc_bit[6] ^ crc_bit[10] ^ crc_bit[18] ^ crc_bit[19] ^ crc_bit[20] ^ crc_bit[22] ^ crc_bit[23] ^ crc_bit[24] ^ crc_bit[26] ^ crc_bit[28] ^ crc_bit[31] ^ crc_bit[25];
		new_crc_bit[25] = crc_bit[2] ^ crc_bit[3] ^ crc_bit[8] ^ crc_bit[11] ^ crc_bit[15] ^ crc_bit[17] ^ crc_bit[18] ^ crc_bit[19] ^ crc_bit[21] ^ crc_bit[22] ^ crc_bit[29] ^ crc_bit[31] ^ crc_bit[28];
		new_crc_bit[24] = crc_bit[1] ^ crc_bit[2] ^ crc_bit[7] ^ crc_bit[10] ^ crc_bit[14] ^ crc_bit[16] ^ crc_bit[17] ^ crc_bit[18] ^ crc_bit[20] ^ crc_bit[21] ^ crc_bit[27] ^ crc_bit[28] ^ crc_bit[30];
		new_crc_bit[23] = crc_bit[0] ^ crc_bit[1] ^ crc_bit[6] ^ crc_bit[9] ^ crc_bit[13] ^ crc_bit[15] ^ crc_bit[16] ^ crc_bit[17] ^ crc_bit[19] ^ crc_bit[26] ^ crc_bit[27] ^ crc_bit[29] ^ crc_bit[31] ^ crc_bit[20];
		new_crc_bit[22] = crc_bit[0] ^ crc_bit[9] ^ crc_bit[11] ^ crc_bit[12] ^ crc_bit[14] ^ crc_bit[16] ^ crc_bit[18] ^ crc_bit[19] ^ crc_bit[24] ^ crc_bit[26] ^ crc_bit[27] ^ crc_bit[29] ^ crc_bit[31] ^ crc_bit[23];
		new_crc_bit[21] = crc_bit[5] ^ crc_bit[9] ^ crc_bit[10] ^ crc_bit[13] ^ crc_bit[17] ^ crc_bit[18] ^ crc_bit[22] ^ crc_bit[24] ^ crc_bit[26] ^ crc_bit[27] ^ crc_bit[29] ^ crc_bit[31];
		new_crc_bit[20] = crc_bit[4] ^ crc_bit[8] ^ crc_bit[9] ^ crc_bit[12] ^ crc_bit[16] ^ crc_bit[17] ^ crc_bit[21] ^ crc_bit[23] ^ crc_bit[25] ^ crc_bit[26] ^ crc_bit[28] ^ crc_bit[30];
		new_crc_bit[19] = crc_bit[3] ^ crc_bit[7] ^ crc_bit[8] ^ crc_bit[11] ^ crc_bit[15] ^ crc_bit[16] ^ crc_bit[20] ^ crc_bit[22] ^ crc_bit[24] ^ crc_bit[25] ^ crc_bit[27] ^ crc_bit[29];
		new_crc_bit[18] = crc_bit[2] ^ crc_bit[6] ^ crc_bit[7] ^ crc_bit[10] ^ crc_bit[14] ^ crc_bit[15] ^ crc_bit[19] ^ crc_bit[21] ^ crc_bit[23] ^ crc_bit[24] ^ crc_bit[26] ^ crc_bit[28] ^ crc_bit[31];
		new_crc_bit[17] = crc_bit[1] ^ crc_bit[5] ^ crc_bit[6] ^ crc_bit[9] ^ crc_bit[13] ^ crc_bit[14] ^ crc_bit[18] ^ crc_bit[20] ^ crc_bit[22] ^ crc_bit[23] ^ crc_bit[27] ^ crc_bit[30] ^ crc_bit[31] ^ crc_bit[25];
		new_crc_bit[16] = crc_bit[0] ^ crc_bit[4] ^ crc_bit[5] ^ crc_bit[8] ^ crc_bit[12] ^ crc_bit[13] ^ crc_bit[17] ^ crc_bit[19] ^ crc_bit[21] ^ crc_bit[22] ^ crc_bit[24] ^ crc_bit[26] ^ crc_bit[29] ^ crc_bit[30];
		new_crc_bit[15] = crc_bit[3] ^ crc_bit[4] ^ crc_bit[5] ^ crc_bit[7] ^ crc_bit[8] ^ crc_bit[9] ^ crc_bit[12] ^ crc_bit[15] ^ crc_bit[16] ^ crc_bit[18] ^ crc_bit[20] ^ crc_bit[21] ^ crc_bit[24] ^ crc_bit[27] ^ crc_bit[30];
		new_crc_bit[14] = crc_bit[2] ^ crc_bit[3] ^ crc_bit[4] ^ crc_bit[6] ^ crc_bit[7] ^ crc_bit[8] ^ crc_bit[11] ^ crc_bit[14] ^ crc_bit[15] ^ crc_bit[17] ^ crc_bit[19] ^ crc_bit[20] ^ crc_bit[23] ^ crc_bit[26] ^ crc_bit[29];
		new_crc_bit[13] = crc_bit[1] ^ crc_bit[2] ^ crc_bit[3] ^ crc_bit[5] ^ crc_bit[6] ^ crc_bit[7] ^ crc_bit[10] ^ crc_bit[13] ^ crc_bit[14] ^ crc_bit[16] ^ crc_bit[18] ^ crc_bit[19] ^ crc_bit[22] ^ crc_bit[25] ^ crc_bit[28] ^ crc_bit[31];
		new_crc_bit[12] = crc_bit[0] ^ crc_bit[1] ^ crc_bit[2] ^ crc_bit[4] ^ crc_bit[5] ^ crc_bit[6] ^ crc_bit[9] ^ crc_bit[12] ^ crc_bit[13] ^ crc_bit[15] ^ crc_bit[17] ^ crc_bit[18] ^ crc_bit[21] ^ crc_bit[24] ^ crc_bit[27] ^ crc_bit[30] ^ crc_bit[31];
		new_crc_bit[11] = crc_bit[0] ^ crc_bit[1] ^ crc_bit[3] ^ crc_bit[4] ^ crc_bit[9] ^ crc_bit[12] ^ crc_bit[14] ^ crc_bit[15] ^ crc_bit[16] ^ crc_bit[17] ^ crc_bit[20] ^ crc_bit[24] ^ crc_bit[25] ^ crc_bit[26] ^ crc_bit[27] ^ crc_bit[28] ^ crc_bit[31];
		new_crc_bit[10] = crc_bit[0] ^ crc_bit[2] ^ crc_bit[3] ^ crc_bit[5] ^ crc_bit[9] ^ crc_bit[13] ^ crc_bit[14] ^ crc_bit[16] ^ crc_bit[19] ^ crc_bit[26] ^ crc_bit[29] ^ crc_bit[31] ^ crc_bit[28];
		new_crc_bit[9] = crc_bit[1] ^ crc_bit[2] ^ crc_bit[4] ^ crc_bit[5] ^ crc_bit[9] ^ crc_bit[11] ^ crc_bit[12] ^ crc_bit[13] ^ crc_bit[18] ^ crc_bit[23] ^ crc_bit[24] ^ crc_bit[29];
		new_crc_bit[8] = crc_bit[0] ^ crc_bit[1] ^ crc_bit[3] ^ crc_bit[4] ^ crc_bit[8] ^ crc_bit[10] ^ crc_bit[11] ^ crc_bit[12] ^ crc_bit[17] ^ crc_bit[22] ^ crc_bit[23] ^ crc_bit[28] ^ crc_bit[31];
		new_crc_bit[7] = crc_bit[0] ^ crc_bit[2] ^ crc_bit[3] ^ crc_bit[5] ^ crc_bit[7] ^ crc_bit[8] ^ crc_bit[10] ^ crc_bit[15] ^ crc_bit[16] ^ crc_bit[21] ^ crc_bit[22] ^ crc_bit[23] ^ crc_bit[24] ^ crc_bit[28] ^ crc_bit[29] ^ crc_bit[25];
		new_crc_bit[6] = crc_bit[1] ^ crc_bit[2] ^ crc_bit[4] ^ crc_bit[5] ^ crc_bit[6] ^ crc_bit[7] ^ crc_bit[8] ^ crc_bit[11] ^ crc_bit[14] ^ crc_bit[20] ^ crc_bit[21] ^ crc_bit[22] ^ crc_bit[25] ^ crc_bit[29] ^ crc_bit[30];
		new_crc_bit[5] = crc_bit[0] ^ crc_bit[1] ^ crc_bit[3] ^ crc_bit[4] ^ crc_bit[5] ^ crc_bit[6] ^ crc_bit[7] ^ crc_bit[10] ^ crc_bit[13] ^ crc_bit[19] ^ crc_bit[20] ^ crc_bit[21] ^ crc_bit[24] ^ crc_bit[28] ^ crc_bit[29];
		new_crc_bit[4] = crc_bit[0] ^ crc_bit[2] ^ crc_bit[3] ^ crc_bit[4] ^ crc_bit[6] ^ crc_bit[8] ^ crc_bit[11] ^ crc_bit[12] ^ crc_bit[15] ^ crc_bit[18] ^ crc_bit[19] ^ crc_bit[20] ^ crc_bit[24] ^ crc_bit[25] ^ crc_bit[29] ^ crc_bit[30] ^ crc_bit[31];
		new_crc_bit[3] = crc_bit[1] ^ crc_bit[2] ^ crc_bit[3] ^ crc_bit[7] ^ crc_bit[8] ^ crc_bit[9] ^ crc_bit[10] ^ crc_bit[14] ^ crc_bit[15] ^ crc_bit[17] ^ crc_bit[18] ^ crc_bit[19] ^ crc_bit[25] ^ crc_bit[27] ^ crc_bit[31];
		new_crc_bit[2] = crc_bit[0] ^ crc_bit[1] ^ crc_bit[2] ^ crc_bit[6] ^ crc_bit[7] ^ crc_bit[8] ^ crc_bit[9] ^ crc_bit[13] ^ crc_bit[14] ^ crc_bit[16] ^ crc_bit[17] ^ crc_bit[18] ^ crc_bit[24] ^ crc_bit[26] ^ crc_bit[30] ^ crc_bit[31];
		new_crc_bit[1] = crc_bit[0] ^ crc_bit[1] ^ crc_bit[6] ^ crc_bit[7] ^ crc_bit[9] ^ crc_bit[11] ^ crc_bit[12] ^ crc_bit[13] ^ crc_bit[16] ^ crc_bit[17] ^ crc_bit[24] ^ crc_bit[27] ^ crc_bit[28];
		new_crc_bit[0] = crc_bit[0] ^ crc_bit[6] ^ crc_bit[9] ^ crc_bit[10] ^ crc_bit[12] ^ crc_bit[16] ^ crc_bit[24] ^ crc_bit[25] ^ crc_bit[26] ^ crc_bit[28] ^ crc_bit[29] ^ crc_bit[30] ^ crc_bit[31];

		crc = 0;
		for (i = 31; i >= 0; i--)
		{
			crc = (crc << 1) | new_crc_bit[i];
		}
	}
}

static uint32_t calc_handshake_checksum(const void *data, uint32_t size)
{
	uint32_t crc = 0;
	uint32_t *data32 = (uint32_t *)data;
	uint32_t i;
	for (i = 0; i < size / 8; i++)
	{
		crc = crc + data32[i * 2];
	}
	return 0 - crc;
}

static void swap_bytes(uint8_t *data, uint32_t size)
{
	while (size > 1)
	{
		uint8_t temp;
		temp = data[0];
		data[0] = data[1];
		data[1] = temp;
		data += 2;
		size -= 2;
	}
}

void parse_jmraid_chip_info(const uint8_t *src, struct jmraid_chip_info *dst)
{
	const uint8_t *p = src;

	debug_print("parse_jmraid_chip_info\n");

	memset(dst, 0, sizeof(struct jmraid_chip_info));

	dst->firmware_version[0] = p[0];
	dst->firmware_version[1] = p[1];
	dst->firmware_version[2] = p[2];
	dst->firmware_version[3] = p[3];
	memcpy(dst->product_name, p + 0x14, 0x20);
	memcpy(dst->manufacturer, p + 0x34, 0x20);
	dst->serial_number = read_u32_le(p + 0xA0);
}

void parse_jmraid_sata_info(const uint8_t *src, struct jmraid_sata_info *dst)
{
	const uint8_t *p = src;
	int i;

	debug_print("parse_jmraid_sata_info\n");

	memset(dst, 0, sizeof(struct jmraid_sata_info));

	p += 0x04;
	for (i = 0; i < 5; i++)
	{
		struct jmraid_sata_info_item *item = &dst->item[i];
		memcpy(item->model_name, p + 0x00, 0x28);
		swap_bytes(item->model_name, 0x28);
		memcpy(item->serial_number, p + 0x28, 0x14);
		swap_bytes(item->serial_number, 0x14);
		item->capacity = ((uint64_t)read_u32_le(p + 0x3C)) * (32 * 1024 * 1024);
		item->port_type = p[0x48];
		item->port_speed = p[0x4A];
		item->page_0_state = p[0x41];
		item->page_0_raid_index = p[0x42];
		item->page_0_raid_member_index = p[0x43];
		item->port = p[0x49];
		p += 0x50;
	}
}

void parse_jmraid_sata_port_info(const uint8_t *src, struct jmraid_sata_port_info *dst)
{
	const uint8_t *p = src;

	debug_print("parse_jmraid_sata_port_info\n");

	memset(dst, 0, sizeof(struct jmraid_sata_port_info));

	p += 0x04;
	memcpy(dst->model_name, p + 0x00, 0x28);
	swap_bytes(dst->model_name, 0x28);
	memcpy(dst->serial_number, p + 0x28, 0x14);
	swap_bytes(dst->serial_number, 0x14);
	memcpy(dst->firmware_version, p + 0x40, 0x08);
	swap_bytes(dst->firmware_version, 0x08);
	dst->capacity = ((uint64_t)read_u32_le(p + 0x3C)) * (32 * 1024 * 1024);
	dst->port_type = p[0x60];
	dst->port = p[0x5A];
	dst->capacity_used = ((uint64_t)read_u32_le(p + 0xCC)) * (32 * 1024 * 1024);
	dst->page_0_state = p[0xBD];
	dst->page_0_raid_index = p[0xBE];
	dst->page_0_raid_member_index = p[0xBF];
}

void parse_jmraid_raid_port_info(const uint8_t *src, struct jmraid_raid_port_info *dst)
{
	const uint8_t *p = src;
	int i;

	debug_print("parse_jmraid_raid_port_info\n");

	memset(dst, 0, sizeof(struct jmraid_raid_port_info));

	p += 0x04;
	dst->port_state = p[0x40];
	memcpy(dst->model_name, p + 0x00, 0x28);
	swap_bytes(dst->model_name, 0x28);
	memcpy(dst->serial_number, p + 0x28, 0x14);
	swap_bytes(dst->serial_number, 0x14);
	dst->level = p[0x50];
	dst->capacity = ((uint64_t)read_u32_le(p + 0x3C)) * (32 * 1024 * 1024);
	dst->state = p[0x42];
	dst->member_count = p[0x51];
	dst->rebuild_priority = read_u16_le(p + 0x60);
	dst->standby_timer = read_u16_le(p + 0x62) * 10;
	memcpy(dst->password, p + 0x78, 0x08);
	dst->rebuild_progress = ((uint64_t)read_u32_le(p + 0x5C)) * (32 * 1024 * 1024);

	p += 0xA0;
	for (i = 0; i < 5; i++)
	{
		struct jmraid_raid_port_info_member *member = &dst->member[i];
		member->ready = p[0x00];
		member->lba48_support = p[0x04];
		member->sata_page = p[0x06];
		member->sata_port = p[0x07];
		member->sata_base = read_u32_le(p + 0x08);
		member->sata_size = ((uint64_t)read_u32_le(p + 0x0C)) * (32 * 1024 * 1024);
		p += 0x20;
	}
}

void parse_jmraid_disk_smart_info(const uint8_t *src1, const uint8_t *src2, struct jmraid_disk_smart_info *dst)
{
	debug_print("parse_jmraid_disk_smart_info\n");

	memset(dst, 0, sizeof(struct jmraid_disk_smart_info));

	if (src1)
	{
		const uint8_t *p = src1;
		int i;

		p += 0x14;
		p += 0x02;
		for (i = 0; i < 30; i++)
		{
			// 00h | 1 | Attribute ID Number (01h to FFh)
			// 01h | 2 | Status Flags 2
			// 03h | 1 | Attribute Value (valid values from 01h to FDh)
			// 04h | 8 | Vendor specific
			if (p[0] != 0)
			{
				struct jmraid_disk_smart_info_attribute *attribute = &dst->attribute[i];
				attribute->id = p[0];
				attribute->flags = read_u16_le(p + 1);
				attribute->current_value = p[3];
				attribute->worst_value = p[4];
				attribute->raw_value = read_u32_le(p + 5) | ((uint64_t)read_u16_le(p + 9) << 32);
			}
			p += 0x0C;
		}
	}

	if (src2)
	{
		const uint8_t *p = src2;
		int i;

		p += 0x14;
		p += 0x02;
		for (i = 0; i < 30; i++)
		{
			if (p[0] != 0)
			{
				struct jmraid_disk_smart_info_attribute *attribute = &dst->attribute[i];
				attribute->threshold = p[1];
			}
			p += 0x0C;
		}
	}
}

void jmraid_init(struct jmraid *jmraid)
{
	debug_print("jmraid_init\n");
	memset(jmraid, 0, sizeof(struct jmraid));
	disk_init(&jmraid->disk);
	jmraid->unused_sector = (uint64_t)-1;
}

bool jmraid_disk_open(struct jmraid *jmraid, const char *disk_name)
{
	debug_print("jmraid_disk_open | %s\n", disk_name);

	if (jmraid->is_disk_open)
	{
		debug_print("disk already open\n");
		return false;
	}

	if (!disk_open(&jmraid->disk, disk_name, "rw"))
	{
		debug_print("disk_open failed\n");
		return false;
	}
	jmraid->is_disk_open = true;

	return true;
}

bool jmraid_disk_close(struct jmraid *jmraid)
{
	debug_print("jmraid_disk_close\n");

	if (!jmraid->is_disk_open)
	{
		debug_print("disk not open\n");
		return false;
	}

	if (!disk_close(&jmraid->disk))
	{
		debug_print("disk_close failed\n");
		return false;
	}
	jmraid->is_disk_open = false;

	return true;
}

bool jmraid_disk_read_sector(struct jmraid *jmraid, uint64_t sector, uint8_t *data)
{
	debug_print("jmraid_disk_read_sector | %llu\n", sector);

	if (!jmraid->is_disk_open)
	{
		debug_print("disk not open\n");
		return false;
	}

	if (!disk_read_sector(&jmraid->disk, sector, data))
	{
		debug_print("disk_read_sector failed\n");
		return false;
	}

	return true;
}

bool jmraid_disk_write_sector(struct jmraid *jmraid, uint64_t sector, const uint8_t *data)
{
	debug_print("jmraid_disk_write_sector | %llu\n", sector);

	if (!jmraid->is_disk_open)
	{
		debug_print("disk not open\n");
		return false;
	}

	if (!disk_write_sector(&jmraid->disk, sector, data))
	{
		debug_print("disk_write_sector failed\n");
		return false;
	}

	return true;
}

void jmraid_set_unused_sector(struct jmraid *jmraid, uint64_t unused_sector)
{
	debug_print("jmraid_set_unused_sector | %llu\n", unused_sector);
	jmraid->unused_sector = unused_sector;
}

void jmraid_set_vendor_id(struct jmraid *jmraid, uint32_t vendor_id)
{
	debug_print("jmraid_set_vendor_id | %08X\n", vendor_id);
	jmraid->vendor_id = vendor_id;
}

bool jmraid_find_unused_sector(struct jmraid *jmraid, uint32_t num, uint64_t *sector)
{
	uint32_t sector_max;
	uint8_t mbr[SECTOR_SIZE];
	uint8_t gpt_header[SECTOR_SIZE];
	uint8_t gpt_entry[SECTOR_SIZE];

	debug_print("jmraid_find_unused_sector\n");

	sector_max = 0x27;
	if (!jmraid_disk_read_sector(jmraid, 0, mbr))
	{
		debug_print("jmraid_disk_read_sector failed\n");
		return false;
	}
	if ((mbr[0x1BE] != 0x00) || (mbr[0x1C2] != 0xEE) || (mbr[0x1C6] != 0x01))
	{
		if ((mbr[0x1FE] != 0x55) || (mbr[0x1FF] != 0xAA))
		{
			if ((mbr[0x000] == 0x45) && (mbr[0x001] == 0x52) && (mbr[0x002] == 0x02) && (mbr[0x003] == 0x00))
			{
				sector_max = 0x3F;
			}
			else
			{
				sector_max = 0x27;
			}
		}
		else
		{
			if (mbr[0x1C6] <= 1)
			{
				sector_max = 0x3E;
			}
			else
			{
				sector_max = mbr[0x1C6] - 1;
			}
			if (sector_max > 0x3E)
			{
				sector_max = 0x3E;
			}
		}
	}
	else
	{
		if (!jmraid_disk_read_sector(jmraid, 1, gpt_header))
		{
			debug_print("jmraid_disk_read_sector failed\n");
			return false;
		}
		if (memcmp(gpt_header + 0, "EFI PART", 8) == 0)
		{
			if (!jmraid_disk_read_sector(jmraid, 2, gpt_entry))
			{
				debug_print("jmraid_disk_read_sector failed\n");
				return false;
			}
			if (gpt_entry[0x20] > 1)
			{
				sector_max = gpt_entry[0x20] - 1;
			}
			else
			{
				sector_max = 0x27;
			}
		}
		else
		{
			sector_max = 0x27;
		}
	}

	sector_max = sector_max - num;
	if (sector_max <= 0x0A)
	{
		sector_max = 0x27 - num;
	}

	*sector = sector_max;

	return true;
}

bool jmraid_backup_unused_sector_data(struct jmraid *jmraid)
{
	debug_print("jmraid_backup_unused_sector_data\n");

	if (jmraid->is_unused_sector_data_valid)
	{
		debug_print("unused sector data not valid\n");
		return false;
	}

	if (!jmraid_disk_read_sector(jmraid, jmraid->unused_sector, jmraid->unused_sector_data))
	{
		debug_print("jmraid_disk_read_sector failed\n");
		return false;
	}

	jmraid->is_unused_sector_data_valid = true;

	return true;
}

bool jmraid_restore_unused_sector_data(struct jmraid *jmraid)
{
	debug_print("jmraid_restore_unused_sector_data\n");

	if (!jmraid->is_unused_sector_data_valid)
	{
		debug_print("unused sector data already valid\n");
		return false;
	}

	if (!jmraid_disk_write_sector(jmraid, jmraid->unused_sector, jmraid->unused_sector_data))
	{
		debug_print("jmraid_disk_write_sector failed\n");
		return false;
	}

	jmraid->is_unused_sector_data_valid = false;

	return true;
}

bool jmraid_prepare_unused_sector(struct jmraid *jmraid)
{
	const uint32_t magic[4] = { 0x3C75A80B, 0x0388E337, 0x689705F3, 0xE00C523A };
	int i;

	debug_print("jmraid_prepare_unused_sector\n");

	for (i = 0; i < 4; i++)
	{
		if (!jmraid_send_handshake(jmraid, magic[i]))
		{
			debug_print("jmraid_send_handshake failed\n");
			return false;
		}
	}

	return true;
}

bool jmraid_open(struct jmraid *jmraid, const char *disk_name, uint32_t vendor_id)
{
	uint64_t unused_sector;

	debug_print("jmraid_open | %s | %08X\n", disk_name, vendor_id);

	if (!jmraid_disk_open(jmraid, disk_name))
	{
		debug_print("jmraid_disk_open failed\n");
		jmraid_close(jmraid);
		return false;
	}

	if (!jmraid_find_unused_sector(jmraid, 0, &unused_sector))
	{
		debug_print("jmraid_find_unused_sector failed\n");
		jmraid_close(jmraid);
		return false;
	}

	jmraid_set_unused_sector(jmraid, unused_sector);

	if (!jmraid_backup_unused_sector_data(jmraid))
	{
		debug_print("jmraid_backup_unused_sector_data failed\n");
		jmraid_close(jmraid);
		return false;
	}

	if (!jmraid_prepare_unused_sector(jmraid))
	{
		debug_print("jmraid_prepare_unused_sector failed\n");
		jmraid_close(jmraid);
		return false;
	}

	jmraid->vendor_id = vendor_id;

	return true;
}

bool jmraid_close(struct jmraid *jmraid)
{
	debug_print("jmraid_close\n");

	if (jmraid->is_unused_sector_data_valid)
	{
		if (!jmraid_restore_unused_sector_data(jmraid))
		{
			debug_print("jmraid_restore_unused_sector_data failed\n");
		}
	}

	if (jmraid->is_disk_open)
	{
		if (!jmraid_disk_close(jmraid))
		{
			debug_print("jmraid_disk_close failed\n");
		}
	}

	return true;
}

bool jmraid_detect_vendor_id(struct jmraid *jmraid, uint32_t *vendor_id)
{
	struct jmraid_chip_info chip_info;
	uint32_t orig_vendor_id;

	debug_print("jmraid_detect_vendor_id\n");

	orig_vendor_id = jmraid->vendor_id;

	jmraid_set_vendor_id(jmraid, 0x197B0562);
	if (!jmraid_get_chip_info(jmraid, &chip_info))
	{
		jmraid_set_vendor_id(jmraid, 0x197B0322);
		if (!jmraid_get_chip_info(jmraid, &chip_info))
		{
			jmraid_set_vendor_id(jmraid, orig_vendor_id);
			debug_print("failed to detect vendor id\n");
			return false;
		}
	}

	*vendor_id = jmraid->vendor_id;

	jmraid_set_vendor_id(jmraid, orig_vendor_id);

	return true;
}

bool jmraid_send_handshake(struct jmraid *jmraid, uint32_t magic)
{
	uint8_t data[SECTOR_SIZE];
	uint32_t i;

	debug_print("jmraid_send_handshake | %08X\n", magic);

	for (i = 0; i < SECTOR_SIZE; i++)
	{
		data[i] = (uint8_t)(i & 0xFF);
	}

	write_u32_le(data + 0x000, 0x197B0325);
	write_u32_le(data + 0x004, magic);
	write_u32_le(data + 0x008, 0);
	write_u32_le(data + 0x00C, 0);
	write_u32_le(data + 0x1F8, calc_handshake_checksum(data, 0x1F8));
	write_u32_le(data + 0x1FC, calc_crc_fast(data, 0x1FC));

	if (!disk_write_sector(&jmraid->disk, jmraid->unused_sector, data))
	{
		debug_print("disk_write_sector failed\n");
		return false;
	}

	return true;
}

bool jmraid_invoke_command(struct jmraid *jmraid, const uint8_t *data_in, uint32_t size_in, uint8_t *data_out, uint32_t size_out)
{
	uint8_t sector_data[SECTOR_SIZE];

	debug_print("jmraid_invoke_command | %02X %02X\n", data_in[0], data_in[1]);

	memset(sector_data, 0, SECTOR_SIZE);
	write_u32_le(sector_data + 0x00, jmraid->vendor_id);
	write_u32_le(sector_data + 0x04, jmraid->seq_id);
	sector_data[0x09] = data_in[0];
	sector_data[0x0A] = data_in[1];
	sector_data[0x0B] = 0xFF;
	memcpy(sector_data + 0x0C, data_in + 2, size_in - 2);
	write_u32_le(sector_data + SECTOR_SIZE - 4, calc_crc_fast(sector_data, SECTOR_SIZE - 4));

	scramble(sector_data, sector_data, SECTOR_SIZE);

	if (!disk_write_sector(&jmraid->disk, jmraid->unused_sector, sector_data))
	{
		debug_print("disk_write_sector failed\n");
		return false;
	}

	if (!disk_read_sector(&jmraid->disk, jmraid->unused_sector, sector_data))
	{
		debug_print("disk_read_sector failed\n");
		return false;
	}

	scramble(sector_data, sector_data, SECTOR_SIZE);

	if (read_u32_le(sector_data + SECTOR_SIZE - 4) != calc_crc_fast(sector_data, SECTOR_SIZE - 4))
	{
		debug_print("invoke command response error -1\n");
		return false;
	}

	if (read_u32_le(sector_data + 0x04) != jmraid->seq_id)
	{
		debug_print("invoke command response error -2\n");
		return false;
	}

	if ((sector_data[0x09] != data_in[0x00]) || (sector_data[0x0A] != data_in[0x01]))
	{
		debug_print("invoke command response command error -3\n");
		return false;
	}

	if (sector_data[0x0B] != 0)
	{
		debug_print("invoke command response command error %d\n", sector_data[0x0B]);
		return false;
	}

	size_out = min(size_out, SECTOR_SIZE - 0x10);
	memcpy(data_out, sector_data + 0x0C, size_out);

	return true;
}

bool jmraid_invoke_command_get_chip_info(struct jmraid *jmraid, uint8_t *data_out, uint32_t size_out)
{
	uint8_t data_in[2];

	debug_print("jmraid_invoke_command_get_chip_info\n");

	data_in[0] = 0x01;
	data_in[1] = 0x01;

	if (!jmraid_invoke_command(jmraid, data_in, sizeof(data_in), data_out, size_out))
	{
		debug_print("jmraid_invoke_command failed\n");
		return false;
	}

	return true;
}

bool jmraid_invoke_command_get_sata_info(struct jmraid *jmraid, uint8_t *data_out, uint32_t size_out)
{
	uint8_t data_in[2];

	debug_print("jmraid_invoke_command_get_sata_info\n");

	data_in[0] = 0x02;
	data_in[1] = 0x01;

	if (!jmraid_invoke_command(jmraid, data_in, sizeof(data_in), data_out, size_out))
	{
		debug_print("jmraid_invoke_command failed\n");
		return false;
	}

	return true;
}

bool jmraid_invoke_command_get_sata_port_info(struct jmraid *jmraid, uint8_t sata_port, uint8_t *data_out, uint32_t size_out)
{
	uint8_t data_in[3];

	debug_print("jmraid_invoke_command_get_sata_port_info\n");

	data_in[0] = 0x02;
	data_in[1] = 0x02;
	data_in[2] = sata_port;

	if (!jmraid_invoke_command(jmraid, data_in, sizeof(data_in), data_out, size_out))
	{
		debug_print("jmraid_invoke_command failed\n");
		return false;
	}

	return true;
}

bool jmraid_invoke_command_get_raid_port_info(struct jmraid *jmraid, uint8_t raid_port, uint8_t *data_out, uint32_t size_out)
{
	uint8_t data_in[3];

	debug_print("jmraid_invoke_command_get_raid_port_info\n");

	data_in[0] = 0x03;
	data_in[1] = 0x02;
	data_in[2] = raid_port;

	if (!jmraid_invoke_command(jmraid, data_in, sizeof(data_in), data_out, size_out))
	{
		debug_print("jmraid_invoke_command failed\n");
		return false;
	}

	return true;
}

bool jmraid_invoke_command_ata_passthrough(struct jmraid *jmraid, uint8_t sata_port, uint8_t ata_read_addr, uint8_t ata_read_size, const uint8_t *ata_data, uint8_t *data_out, uint32_t size_out)
{
	uint8_t data_in[22];

	debug_print("jmraid_invoke_command_ata_passthrough\n");

	data_in[0] = 0x02;
	data_in[1] = 0x03;
	data_in[2] = sata_port;
	data_in[3] = 0x02; // ?
	data_in[4] = ata_read_addr;
	data_in[5] = ata_read_size;
	memcpy(data_in + 6, ata_data, 16);

	if (!jmraid_invoke_command(jmraid, data_in, sizeof(data_in), data_out, size_out))
	{
		debug_print("jmraid_invoke_command failed\n");
		return false;
	}

	return true;
}

bool jmraid_get_chip_info(struct jmraid *jmraid, struct jmraid_chip_info *info)
{
	uint8_t data_out[SECTOR_SIZE];

	debug_print("jmraid_get_chip_info\n");

	if (!jmraid_invoke_command_get_chip_info(jmraid, data_out, sizeof(data_out)))
	{
		debug_print("jmraid_invoke_command_get_chip_info failed\n");
		return false;
	}

	parse_jmraid_chip_info(data_out, info);

	return true;
}

bool jmraid_get_sata_info(struct jmraid *jmraid, struct jmraid_sata_info *info)
{
	uint8_t data_out[SECTOR_SIZE];

	debug_print("jmraid_get_sata_info\n");

	if (!jmraid_invoke_command_get_sata_info(jmraid, data_out, sizeof(data_out)))
	{
		debug_print("jmraid_invoke_command_get_sata_info failed\n");
		return false;
	}

	parse_jmraid_sata_info(data_out, info);

	return true;
}

bool jmraid_get_sata_port_info(struct jmraid *jmraid, uint8_t index, struct jmraid_sata_port_info *info)
{
	uint8_t data_out[SECTOR_SIZE];

	debug_print("jmraid_get_sata_port_info\n");

	if (!jmraid_invoke_command_get_sata_port_info(jmraid, index, data_out, sizeof(data_out)))
	{
		debug_print("jmraid_invoke_command_get_sata_port_info failed\n");
		return false;
	}

	parse_jmraid_sata_port_info(data_out, info);

	return true;
}

bool jmraid_get_raid_port_info(struct jmraid *jmraid, uint8_t index, struct jmraid_raid_port_info *info)
{
	uint8_t data_out[SECTOR_SIZE];

	debug_print("jmraid_get_raid_port_info\n");

	if (!jmraid_invoke_command_get_raid_port_info(jmraid, index, data_out, sizeof(data_out)))
	{
		debug_print("jmraid_invoke_command_get_raid_port_info failed\n");
		return false;
	}

	parse_jmraid_raid_port_info(data_out, info);

	return true;
}

bool jmraid_get_disk_smart_info(struct jmraid *jmraid, uint8_t sata_port, struct jmraid_disk_smart_info *info)
{
	uint8_t data_in[16];
	uint8_t data_out_1[SECTOR_SIZE];
	uint8_t data_out_2[SECTOR_SIZE];

	debug_print("jmraid_get_disk_smart_info\n");

	memset(data_in, 0, sizeof(data_in));
	data_in[2] = 0xD0;
	data_in[8] = 0x4F;
	data_in[10] = 0xC2;
	data_in[12] = 0xA0;
	data_in[14] = 0xB0;

	if (!jmraid_invoke_command_ata_passthrough(jmraid, sata_port, 0x00, 0xE0, data_in, data_out_1, sizeof(data_out_2)))
	{
		debug_print("jmraid_invoke_command_ata_passthrough failed\n");
		return false;
	}

	memset(data_in, 0, sizeof(data_in));
	data_in[2] = 0xD1;
	data_in[8] = 0x4F;
	data_in[10] = 0xC2;
	data_in[12] = 0xA0;
	data_in[14] = 0xB0;

	if (!jmraid_invoke_command_ata_passthrough(jmraid, sata_port, 0x00, 0xE0, data_in, data_out_2, sizeof(data_out_2)))
	{
		debug_print("jmraid_invoke_command_ata_passthrough failed\n");
		return false;
	}

	parse_jmraid_disk_smart_info(data_out_1, data_out_2, info);

	return true;
}

bool jmraid_ata_identify_device(struct jmraid *jmraid, uint8_t sata_port, uint8_t *data_out)
{
	uint8_t data_in[16];
	uint8_t temp_data_out[SECTOR_SIZE];

	debug_print("jmraid_ata_identify_device\n");

	memset(data_in, 0, sizeof(data_in));
	data_in[14] = 0xEC;

	if (!jmraid_invoke_command_ata_passthrough(jmraid, sata_port, 0x00, 0x80, data_in, temp_data_out, sizeof(temp_data_out)))
	{
		debug_print("jmraid_invoke_command_ata_passthrough failed\n");
		return false;
	}

	memcpy(data_out, temp_data_out + 0x14, 0x100);

	if (!jmraid_invoke_command_ata_passthrough(jmraid, sata_port, 0x80, 0x80, data_in, temp_data_out, sizeof(temp_data_out)))
	{
		debug_print("jmraid_invoke_command_ata_passthrough failed\n");
		return false;
	}

	memcpy(data_out + 0x100, temp_data_out + 0x14, 0x100);

	return true;
}

bool jmraid_ata_smart_read_data(struct jmraid *jmraid, uint8_t sata_port, uint8_t *data_out)
{
	uint8_t data_in[16];
	uint8_t temp_data_out[SECTOR_SIZE];

	debug_print("jmraid_ata_smart_read_data\n");

	memset(data_in, 0, sizeof(data_in));
	data_in[2] = 0xD0;
	data_in[8] = 0x4F;
	data_in[10] = 0xC2;
	data_in[12] = 0xA0;
	data_in[14] = 0xB0;

	if (!jmraid_invoke_command_ata_passthrough(jmraid, sata_port, 0x00, 0x80, data_in, temp_data_out, sizeof(temp_data_out)))
	{
		debug_print("jmraid_invoke_command_ata_passthrough failed\n");
		return false;
	}

	memcpy(data_out, temp_data_out + 0x14, 0x100);

	if (!jmraid_invoke_command_ata_passthrough(jmraid, sata_port, 0x80, 0x80, data_in, temp_data_out, sizeof(temp_data_out)))
	{
		debug_print("jmraid_invoke_command_ata_passthrough failed\n");
		return false;
	}

	memcpy(data_out + 0x100, temp_data_out + 0x14, 0x100);

	return true;
}
