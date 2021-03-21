//
// icon_tool - pack multiple PNG images into an ICO file
//
// Copyright (c) 2021 Optidash-AI
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//    http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int get_png_info(unsigned char *pIn, int iSize, int *width, int *height, int *bpp)
{
    int w, h, i, j, bits=0;
    if (pIn[0] != 0x89 || pIn[1] != 0x50 || pIn[2] != 0x4e || pIn[3] != 0x47) {
        // PNG signature not present - not a PNG file
        return 1;
    }
    // Parse enough of the PNG file to get to the image header (IHDR) to get
    // the image size and bit depth
    w = (pIn[16] << 24) | (pIn[17] << 16) | (pIn[18] << 8) | pIn[19];
    h = (pIn[20] << 24) | (pIn[21] << 16) | (pIn[22] << 8) | pIn[23];
    i = pIn[24]; // bits per pixel
    j = pIn[25]; // pixel type
    switch (j) {
        case 0: // grayscale
        case 3: // palette image
            bits = i;
            break;
        case 2: // RGB triple
            bits = i * 3;
            break;
        case 4: // grayscale + alpha channel
            bits = i * 2;
            break;
        case 6: // RGB + alpha
            bits = i * 4;
            break;
    }
    *width = w; *height = h; *bpp = bits;
    return 0;
} /* get_png_info() */
//
// Main program entry point
//
int main(int argc, char *argv[])
{
    int i, j, iSize, iFileCount;
    int width, height, bpp;
    int iOffset;
    FILE *ihandle, *ohandle;
    unsigned char *pIn;
    unsigned char ucTemp[512];
    
    if (argc < 3 || argc > 16)
    {
        printf("icon_tool Copyright (c) 2021 Optidash-AI\n");
        printf("Combines multiple PNG images into a single ICO file\n");
        printf("Source images must be <= 256 pixels in each dimension\n\n");
        printf("Usage: icon_tool <outfile> <infile_1> <infile_2> <infile_N>\n");
        printf("example:\n\n");
        printf("icon_tool out.ico favicon1.png favicon2.png favicon3.png\n");
        return 0; // no filenames passed
    }
    iFileCount = argc - 2;
    ohandle = fopen(argv[1], "w+b");
    if (ohandle == NULL) {
        fprintf(stderr, "Unable to open output file: %s\n", argv[1]);
        return -1; // bad filename passed
    }
    // write an empty header; we'll fill it in later
    fwrite(ucTemp, 1, 6 + (iFileCount * 16), ohandle);
    ucTemp[0] = 0; // ICONDIR structure starts with 0,0
    ucTemp[1] = 0;
    ucTemp[2] = 1; // ICON file (2 = cursor file)
    ucTemp[3] = 0; // 2-byte int
    ucTemp[4] = (unsigned char)iFileCount;
    ucTemp[5] = 0; // 2-byte int
    iOffset = 6 + (iFileCount * 16); // starting offset of first file data
    for (i=2; i<argc; i++) { // collect all of the input files
        ihandle = fopen(argv[i],"rb");
        if (ihandle == NULL)
        {
            fprintf(stderr, "Unable to open input file: %s\n", argv[i]);
            return -1; // bad filename passed
        }
        fseek(ihandle, 0L, SEEK_END); // get the file size
        iSize = (int)ftell(ihandle);
        fseek(ihandle, 0, SEEK_SET);
        pIn = malloc(iSize);
        if (pIn == NULL) {
            fprintf(stderr, "Memory allocation error: size = %d\n", iSize);
            return -1; // bad filename passed
        }
        fread(pIn, 1, iSize, ihandle);
        if (get_png_info(pIn, iSize, &width, &height, &bpp)) {
            fprintf(stderr, "Input files must be PNG; exiting...\n");
            return -1; // bad filename passed
        }
        if (width > 256 || height > 256) {
            fprintf(stderr, "image files cannot be larger than 256x256; exiting...\n");
            return -1; // bad filename passed
        }
        // Fill in ICONDIRENTRY for this image
        j = 6 + (i-2) * 16;
        ucTemp[j] = (unsigned char)width;
        ucTemp[j+1] = (unsigned char)height;
        if (bpp < 8)
            ucTemp[j+2] = (1 << bpp); // number of colors
        else if (bpp == 8)
            ucTemp[j+2] = 255;
        else
            ucTemp[j+2] = 0; // non-palette image
        ucTemp[j+3] = 0; // reserved
        ucTemp[j+4] = 1; // color planes
        ucTemp[j+5] = 0; // 2-byte int
        ucTemp[j+6] = bpp;
        ucTemp[j+7] = 0; // 2-byte int
        *(uint32_t *)&ucTemp[j+8] = iSize; // image file size
        *(uint32_t *)&ucTemp[j+12] = iOffset; // offset to this image
        iOffset += iSize;
        // Write this image to the output file
        fwrite(pIn, 1, iSize, ohandle);
        free(pIn);
        fclose(ihandle);
    }
    // Seek to the beginning and update the ICONDIR and ICONDIRENTRY structures
    fseek(ohandle, 0, SEEK_SET);
    fwrite(ucTemp, 1, 6 + (iFileCount * 16), ohandle);
    fclose(ohandle);
    return 0;
} /* main() */
