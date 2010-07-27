/*
   base64.cpp and base64.h

   Copyright (C) 2004-2008 Ren� Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   Ren� Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/

/*
Changed by Andr� de Leiradella to work without std::string.
*/

#include "base64.h"

#include <string.h>
#include <ctype.h>

#include "debug.h"

static const unsigned char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


static inline bool is_base64(unsigned char c)
{
	return isalnum(c) || (c == '+') || (c == '/');
}

const char *Base64Encode(void *in, size_t in_len)
{
	unsigned char *ret = NEWARRAY(unsigned char, ((in_len + 3) * 4) / 3 + 1);
	int count = 0;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];
	unsigned char *bytes_to_encode = (unsigned char *)in;

	while (in_len--)
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i <4) ; i++)
			{
				ret[count++] = base64_chars[char_array_4[i]];
			}
			i = 0;
		}
	}

	if (i != 0)
	{
		for (j = i; j < 3; j++)
		{
			char_array_3[j] = '\0';
		}

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; j < i + 1; j++)
		{
			ret[count++] = base64_chars[char_array_4[j]];
		}

		while (i++ < 3)
		{
			ret[count++] = '=';
		}
	}

	ret[count] = '\0';
	return (const char *)ret;
}

void *Base64Decode(const char *in, size_t *out_len)
{
	size_t in_len = strlen(in);
	int i = 0;
	int j = 0;
	int pos = 0;
	unsigned char char_array_4[4];
	unsigned char char_array_3[3];
	unsigned char base64_reverse[256];
	unsigned char *buffer = NEWARRAY(unsigned char, in_len / 4 * 3);
	unsigned char *ret = buffer;

	if (buffer == NULL)
	{
		return NULL;
	}

	memset(base64_reverse, 0, sizeof(base64_reverse));
	for (const unsigned char *aux = base64_chars; *aux != '\0'; aux++)
	{
		base64_reverse[*aux] = (unsigned char)(aux - base64_chars);
	}

	while (in_len-- && (in[pos] != '=') && is_base64(in[pos]))
	{
		char_array_4[i++] = in[pos]; pos++;
		if (i == 4)
		{
			for (i = 0; i < 4; i++)
			{
				char_array_4[i] = base64_reverse[char_array_4[i]];
			}

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; i < 3; i++)
			{
				*ret++ = char_array_3[i];
			}
			i = 0;
		}
	}

	if (i != 0)
	{
		for (j = i; j < 4; j++)
		{
			char_array_4[j] = 0;
		}

		for (j = 0; j < 4; j++)
		{
			char_array_4[j] = base64_reverse[char_array_4[j]];
		}

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; j < i - 1; j++)
		{
			*ret++ = char_array_3[j];
		}
	}

	*out_len = (size_t)(ret - buffer);
	return (void *)buffer;
}
