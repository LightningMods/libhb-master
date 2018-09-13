
#include "util.h"

std::string common::Util::formatString(const char* format, ...) {
	unsigned int bufSize = 1024;
	char *buf = (char*)malloc(bufSize);
	va_list ap;
	va_start(ap, format);
	do {
		int n = vsnprintf(buf, bufSize, format, ap);
		va_end(ap);
		if ((n >= 0) && ((n + 1)<bufSize)) {
			std::string ret = buf;
			free(buf);
			return ret;
		}
		bufSize *= 2;
		buf = (char*)realloc(buf, bufSize);
	} while (1);
}

std::wstring common::Util::convertUtf8toUcs2(const std::string& str) {
	std::wstring ret;
	const char *src = str.c_str();
	if (src == NULL) { return ret; }
	wchar_t	dstCode = 0;
	char	strByte = 0;
	char	convertNum = 0;

	int	i = 0;
	while (1) {
		if (strByte == 1) {
			ret += dstCode;
			dstCode = 0;
			strByte = 0;
			convertNum++;
		}
		else if (strByte > 1) {
			dstCode <<= 6;
			dstCode += (src[i] & 0x3F);
			strByte--;
			i++;
			continue;
		}

		if (src[i] == 0) {
			break;
		}

		if ((src[i] & 0x80) != 0x80) {
			strByte = 1;
			dstCode = src[i] & 0x7F;
		}
		else if (((src[i] & 0xE0) == 0xC0) &&
			((src[i + 1] & 0xC0) == 0x80)) {
			strByte = 2;
			dstCode = src[i] & 0x1F;
		}
		else if (((src[i] & 0xF0) == 0xE0) &&
			((src[i + 1] & 0xC0) == 0x80) &&
			((src[i + 2] & 0xC0) == 0x80)) {
			strByte = 3;
			dstCode = src[i] & 0x0F;
		}
		else {
			break;
		}

		i++;
	}
	return ret;
}