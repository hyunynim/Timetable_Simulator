#include"main.h"

int Char2Int(char* src) {
	int res = 0;
	for (int i = 0; src[i]; ++i)
		res = res * 10 + src[i] - '0';
	return res;
}

string Wstr2Str(wstring src)
{
	USES_CONVERSION;
	return string(W2A(src.c_str()));
}

wstring Str2Wstr(string src) {
	USES_CONVERSION;
	return wstring(A2W(src.c_str()));
}