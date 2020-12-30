#include <string>


unsigned char clip_value(unsigned char x, unsigned char min_val, unsigned char  max_val) {
	if (x > max_val) {
		return max_val;
	}
	else if (x < min_val) {
		return min_val;
	}
	else {
		return x;
	}
}


void rgb24_to_yuv420(unsigned char* RgbBuf, int w, int h, unsigned char* yuvBuf)
{
	unsigned char* ptrY, * ptrU, * ptrV, * ptrRGB;
	memset(yuvBuf, 0, w * h * 3 / 2);
	ptrY = yuvBuf;
	ptrU = yuvBuf + w * h;
	ptrV = ptrU + (w * h * 1 / 4);
	unsigned char y, u, v, r, g, b;
	for (int j = 0; j < h; j++) {
		ptrRGB = RgbBuf + w * j * 3;
		for (int i = 0; i < w; i++) {

			r = *(ptrRGB++);
			g = *(ptrRGB++);
			b = *(ptrRGB++);
			y = (unsigned char)((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
			u = (unsigned char)((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
			v = (unsigned char)((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;
			*(ptrY++) = clip_value(y, 0, 255);
			if (j % 2 == 0 && i % 2 == 0) {
				*(ptrU++) = clip_value(u, 0, 255);
			}
			else {
				if (i % 2 == 0) {
					*(ptrV++) = clip_value(v, 0, 255);
				}
			}
		}
	}
}

void yuv420_to_rgb(char* yuv, int w, int h, char* rgb)
{
	unsigned char* pBufy = new unsigned char[w * h];
	unsigned char* pBufu = new unsigned char[w * h / 4];
	unsigned char* pBufv = new unsigned char[w * h / 4];

	memcpy(pBufy, yuv, w * h);
	memcpy(pBufu, yuv + w * h, w * h / 4);
	memcpy(pBufv, yuv + w * h * 5 / 4, w * h / 4);

	for (int i = 0; i < w * h / 4; i++)
	{
		rgb[i * 3 + 2] = pBufy[i] + 1.772 * (pBufu[i] - 128);  //B = Y +1.779*(U-128)
		rgb[i * 3 + 1] = pBufy[i] - 0.34413 * (pBufu[i] - 128) - 0.71414 * (pBufv[i] - 128);//G = Y-0.3455*(U-128)-0.7169*(V-128)
		rgb[i * 3 + 0] = pBufy[i] + 1.402 * (pBufv[i] - 128);//R = Y+1.4075*(V-128)
	}
	free(pBufy);
	free(pBufu);
	free(pBufv);
}