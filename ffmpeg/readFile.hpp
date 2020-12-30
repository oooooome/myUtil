#include <iostream>
#include <string>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/imgutils.h"
}


void readYUVasBuf(std::string path, int width, int height, uint8_t* y, uint8_t* u, uint8_t* v)
{
	FILE* in;

	int len = width * height * 3 / 2;
	uint8_t* buff = (uint8_t*)malloc(len);
	y = (uint8_t*)malloc(width * height);
	u = (uint8_t*)malloc(width * height / 4);
	v = (uint8_t*)malloc(width * height / 4);


	in = fopen(path.c_str(), "rb");
	if (in == nullptr) {
		return;
	}


	fread(buff, sizeof(uint8_t), len, in);

	memcpy(y, buff, width * height);
	memcpy(u, buff + width * height, width * height / 4);
	memcpy(v, buff + width * height * 5 / 4, width * height / 4);

	free(buff);
	fclose(in);
}

void readYUVasAVFrame(std::string path, int width, int height, AVFrame* frame)
{
	FILE* in;

	int len = width * height * 3 / 2;
	uint8_t* buff = (uint8_t*)malloc(len);
	frame = av_frame_alloc();
	frame->width = width;
	frame->height = height;
	frame->format = AV_PIX_FMT_YUV420P;
	int ret = av_frame_get_buffer(frame, 32);
	if (ret < 0)
	{
		return;
	}


	in = fopen(path.c_str(), "rb");
	if (in == nullptr) {
		return;
	}


	av_image_copy_plane(frame->data[0], frame->linesize[0], (const uint8_t*)buff, width, width, height);
	av_image_copy_plane(frame->data[1], frame->linesize[1], (const uint8_t*)buff + width * height, width / 2, width / 2, height);
	av_image_copy_plane(frame->data[2], frame->linesize[2], (const uint8_t*)buff + width * height, width / 2, width / 2, height);

	fread(buff, sizeof(uint8_t), len, in);



	fclose(in);
}