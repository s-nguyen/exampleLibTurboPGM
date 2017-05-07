/*
 * jpegturb.c
 *
 *  Created on: May 5, 2017
 *      Author: yorha
 */
#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <turbojpeg.h>
#include <stdint.h>
#include <time.h>

#define w 800
#define h 800
int main() {
	FILE* srcImage;
	FILE* outImage;
	srcImage = fopen("/home/yorha/workspace/exampleturb/white.raw", "rb");
	fseek(srcImage, 0, SEEK_END);

	uint8_t imgBuf[ftell(srcImage)];
	rewind(srcImage);
	fseek(srcImage, 0, SEEK_SET);
	printf("Read Size: %d bytes\n",
			(int) fread(imgBuf, 1, sizeof(imgBuf), srcImage));
	fclose(srcImage);

	uint64_t compress_size;
	int s = tjBufSize(w, h, TJSAMP_GRAY);
	uint8_t jpegImage[s];
	uint8_t *jpegImagePtr = jpegImage;
	tjhandle jhandle = tjInitCompress();

	struct timespec start, stop;
	clock_gettime(CLOCK_MONOTONIC, &start);
	tjCompress2(jhandle, imgBuf, w, w, h, TJPF_GRAY, &jpegImagePtr,
			&compress_size, TJSAMP_GRAY, 100,
			TJFLAG_FASTDCT | TJFLAG_NOREALLOC);
	clock_gettime(CLOCK_MONOTONIC, &stop);
	printf("Comprerssed %ld Bytes. Time taken: %ld.%09ld\n", compress_size,
			(stop.tv_sec - start.tv_sec), (stop.tv_nsec - start.tv_nsec));

	tjDestroy(jhandle);

	outImage = fopen("a.jpeg", "wb");
	printf("Write Size: %d bytes\n",
			(int) fwrite(jpegImage, 1, compress_size, outImage));
	fclose(outImage);

	printf("Done\n");
	return 0;
}
