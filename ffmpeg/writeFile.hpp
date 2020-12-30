#include <fstream>
#include <iostream>
#include <string>

extern "C" {
#include "libavcodec/avcodec.h"
}

void writePCM(uint8_t* buffer, int capturedDataSize)
{
	std::ofstream outFile1;
	std::string fileName1 = "save.pcm";
	outFile1.open(fileName1, std::ios::binary | std::ios::out | std::ios::app);
	if (!outFile1.is_open()) {
		std::cout << "file open error" << std::endl;
	}
	else {
		outFile1.write((char*)buffer, capturedDataSize);
		outFile1.close();
	}
}

void writeYUV420(uint8_t* data, int width, int height)
{
	FILE* f;
	uint8_t* buf = data;
	int w = width;
	int h = height;

	f = fopen("save.yuv", "ab+");
	for (int i = 0; i < height; i++) {
		fwrite(buf + i * w, 1, w, f);
	}

	w = w / 2;
	h = h / 2;
	buf += w * h;
	for (int i = 0; i < height; i++) {
		fwrite(buf + i * w, 1, w, f);
	}

	buf += w * h / 4;
	for (int i = 0; i < height; i++) {
		fwrite(buf + i * w, 1, w, f);
	}
	fclose(f);
}

void writeYUV420(AVFrame* yuvFrame)
{
	FILE* f;
	int width = yuvFrame->width;
	int height = yuvFrame->height;
	f = fopen("save.yuv", "wb");
	if (f == nullptr) {
		printf("Unable to open file\n");
		return;
	}

	int y, writeError;

	/*Writing Y plane data to file.*/
	for (y = 0; y < height; y++) {
		writeError = fwrite(yuvFrame->data[0] + y * yuvFrame->linesize[0], 1, width, f);
		if (writeError != width) {
			printf("Unable to write Y plane data!\n");
			return;
		}
	}

	/*Dividing by 2.*/
	height /= 2;
	width /= 2;

	/*Writing U plane data to file.*/
	for (y = 0; y < height; y++) {
		writeError = fwrite(yuvFrame->data[1] + y * yuvFrame->linesize[1], 1, width, f);
		if (writeError != width) {
			printf("Unable to write U plane data!\n");
			return;
		}
	}

	/*Writing V plane data to file.*/
	for (y = 0; y < height; y++) {
		writeError = fwrite(yuvFrame->data[2] + y * yuvFrame->linesize[2], 1, width, f);
		if (writeError != width) {
			printf("Unable to write V plane data!\n");
			return;
		}
	}
}

void writeH264(uint8_t* data, int size)
{
	FILE* fpSave;
	char filename[100] = { 0 };
	sprintf(filename, "%s", "out.h264");
	if ((fpSave = fopen(filename, "ab")) == NULL)  // h264保存的文件名
	{
		printf("Unable to open %s...\n", filename);
		return;
	}
	fwrite(data, 1, size, fpSave);  //写数据到文件中
	fclose(fpSave);
}
