#include <ft2build.h>
#include FT_FREETYPE_H
#include <Windows.h>

static const DWORD MAX_SIZE = 65536;
static int font_size = 16;
static int ary_size = 17 * 16;
static int single_size = 32;

int generate_font_char(FT_Face face, char * tmpfont)
{
	int tidx = 0;

	DWORD len = ary_size;
	tmpfont[0] = '\0';

	int lx, ly, counter;

	for (ly = 0; ly <= (font_size) * 26 / 32 - face->glyph->bitmap_top; ++ly)
	{
		for (lx = 0; lx < font_size; ++lx)
		{
			tmpfont[tidx] = '_';
			tidx += 1;
		}
		tmpfont[tidx] = '\n';
		tidx += 1;
	}

	int iRow = 0, iCol = 0;
	for (iRow = 0; iRow < face->glyph->bitmap.rows; iRow++)
	{
		for (lx = 0; lx < face->glyph->bitmap_left; ++lx)
		{
			tmpfont[tidx] = '_';
			tidx += 1;
		}

		for (iCol = 0; iCol < face->glyph->bitmap.width; iCol++)
		{
			if ((face->glyph->bitmap.buffer[iRow * face->glyph->bitmap.pitch + iCol / 8] & (0x80 >> (iCol % 8))) == 0)
			{
				tmpfont[tidx] = '_';
				tidx += 1;
			}
			else
			{
				tmpfont[tidx] = '0';
				tidx += 1;
			}
		}
		for (counter = 0; counter < font_size - (lx + iCol); ++counter)
		{
			tmpfont[tidx] = '_';
			tidx += 1;
		}
		tmpfont[tidx] = '\n';
		tidx += 1;
		ly++;
	}

	for (; ly < font_size; ++ly)
	{
		for (lx = 0; lx < font_size; ++lx)
		{
			tmpfont[tidx] = '_';
			tidx += 1;
		}
		tmpfont[tidx] = '\n';
		tidx += 1;
	}

	tmpfont[tidx] = '\0';
	return 0;
}

int main(void)
{
	font_size = 12;
	ary_size = font_size * font_size + font_size;

	char * fontname = "nsimsun.ttf";
	char * tmpfont = new char[ary_size];

	DWORD size;

	single_size = font_size * font_size / 8;
	size = MAX_SIZE * single_size;

	BYTE * data = (BYTE *)malloc(size);
	memset(data, 0, size);

    FT_Library library;
    FT_Face face;
    FT_UInt char_index;
    int error;

    error = FT_Init_FreeType(&library);
    if (error)
    {
        printf("can not init free type library!\n");
        goto __end;
    }

    error = FT_New_Face(library, fontname, 0, &face);
    if (error)
    {
        printf("create new face falied!\n");
        goto __end;
    }

    error = FT_Set_Pixel_Sizes(face, 0, font_size);
    if (error)
    {
        printf("set font size error!\n");
        goto __end;
    }
	
	for (DWORD i = 0; i < MAX_SIZE; ++i)
	{
		char_index = FT_Get_Char_Index(face, i);
		if (!char_index) continue;

		error = FT_Load_Glyph(face, char_index, FT_LOAD_DEFAULT);
		if (error) continue;

		if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
		{
			error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);
			if (error) continue;
		}

		if (!generate_font_char(face, tmpfont))
		{
			int j = 0, t = 0;
			BYTE fc = 0;
			for (int k = 0; k < ary_size; ++k)
			{
				if (tmpfont[k] == '0')
					fc |= (1 << j);
				else if (tmpfont[k] == '\n')
					continue;

				j++;
				if (j == 8)
				{
					data[i * single_size + t] = fc;
					t++;
					
					j = 0;
					fc = 0;
				}
			}
		}
	}

	FILE * fp = 0;
	fp = fopen("Unicode12","wb");
    if (fp == NULL)
    {
        perror("Cann't open the file\n");
        goto __end;
    }
    
	fwrite(data, size, 1, fp);
    fclose(fp);

__end:
    printf("\n");
	system("pause");
    return 0;
}