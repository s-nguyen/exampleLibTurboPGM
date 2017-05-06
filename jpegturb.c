/*
 * jpegturb.c
 *
 *  Created on: May 5, 2017
 *      Author: yorha
 */

#include <stdio.h>
#include <turbojpeg.h>
#include <stdint.h>

#define w 512
#define h 512
int main(){
	FILE* srcImage;
	FILE* outImage;
	srcImage = fopen("baboon.raw", "rb");
	fseek(srcImage, 0, SEEK_END);

	uint8_t imgBuf[ftell(srcImage)];
	rewind(srcImage);
	fseek(srcImage, 0, SEEK_SET);
	printf("Read Size: %d bytes\n", (int)fread(imgBuf, 1, sizeof(imgBuf), srcImage));
	fclose(srcImage);

	uint64_t compress_size;
	int s = tjBufSize(w, h, TJSAMP_GRAY );
	uint8_t jpegImage[s];
	uint8_t *jpegImagePtr = jpegImage;
	tjhandle jhandle = tjInitCompress();


	tjCompress2(jhandle, imgBuf, w, w, h, TJPF_GRAY, &jpegImagePtr, &compress_size, TJSAMP_GRAY, 50,
			TJFLAG_FASTDCT | TJFLAG_NOREALLOC);

	tjDestroy(jhandle);

	outImage = fopen("a.jpeg", "wb");
	printf("Write Size: %d bytes\n", (int)fwrite(jpegImage, 1, compress_size, outImage));
	fclose(outImage);

	printf("Done\n");
	return 0;
}
