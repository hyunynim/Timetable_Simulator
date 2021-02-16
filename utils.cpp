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

void RandomPreference(int sum) {
	preference.clear();
	preference.resize(profList.size() + 1);
	int origin = sum;
	for (int i = 1; i< preference.size(); ++i){
		sum = origin;
		for (int j = 0; j < dayCount * 2; ++j) {
			auto randomGen = ud(0, sum);

			int cur = randomGen(gen);

			sum -= cur;
			preference[i].push_back(cur);
		}
		random_shuffle(preference[i].begin(), preference[i].end());
	}
}