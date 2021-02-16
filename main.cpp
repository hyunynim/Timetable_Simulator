#include"main.h"

void PrintLog();
template<typename T> void PrintLog(T log);
template<typename T, typename... Args> void PrintLog(T log, Args... args);

random_device rd;
mt19937 gen(rd());

char globalMsg[1010];
map<pair<int, int>, LECTURE> decomp;
map<string, int> prof2Idx;
map<string, int> lecPerProf;

vector<string> profList;
vector<vector<int>> preference;
int dayNodeNumber = 200;
int totalLecCount;

const int labCount = 4;
const int source = 0;
const int sink = 1000;
const int dayCount = 5;
const int profNodeBegin = 100;
const int dayNodeBegin = 200;
const int labNodeBegin = 300;
vector<string> dayName = { "월", "화", "수", "목", "금" };
vector<string> timeName = { "오전", "오후" };
vector<vector<int>> timetable(dayCount * 2);

MCMF mcmf("lec.xls", 1);

int main() {
	//printf("강의실 수: ");
	//scanf("%d", &labCount);

	int ans = mcmf.Match(source, sink);	//source, sink
	int count = 0;

	for (int i = 0; i < mcmf.gph[sink].size(); ++i) {
		auto cur = mcmf.gph[sink][i];
		if (cur.cap) ++count;
	}

	if (count != totalLecCount) {
		printf("배정 불가\n%d %d", ans, totalLecCount);
		return 0;
	}
	for (int i = 0; i < profList.size(); ++i) {
		int cur = profNodeBegin + i;
		int lecCount = lecPerProf[profList[i]];
		PrintLog(profList[i]);
		for (auto node : mcmf.gph[cur]) {
			int pos = node.pos;
			int cap = node.cap;
			int dayIndex = pos - dayNodeBegin;
			if (cap == 0 && dayIndex >= 0) {
				PrintLog(pos, "(", dayName[dayIndex % 5], timeName[dayIndex / 5], ")");
				timetable[dayIndex].push_back(i);
			}
		}
	}
	ExcelWrite("res.xls");
	puts("res.xls로 저장");
}

void PrintLog() {}

template<typename T>
void PrintLog(T log) {
#ifdef DEBUGMODE
	cout << log << '\n';
#endif
}

template<typename T, typename... Args>
void PrintLog(T log, Args... args) {
#ifdef DEBUGMODE
	cout << log << " ";
	PrintLog(args...);
#endif
}