#include"main.h"

void PrintLog();
template<typename T> void PrintLog(T log);
template<typename T, typename... Args> void PrintLog(T log, Args... args);

char globalMsg[1010];
map<pair<int, int>, LECTURE> decomp;
map<string, int> prof2Idx;
map<string, int> lecPerProf;

vector<string> profList;
vector<vector<int>> preference;
int labCount;
int dayNodeNumber = 200;

const int source = 0;
const int sink = 1000;
const int dayCount = 5;
const int profNodeBegin = 100;
const int dayNodeBegin = 200;
const int labNodeBegin = 300;
vector<string> dayName = { "월", "화", "수", "목", "금" };
vector<string> timeName = { "오전", "오후" };
vector<vector<int>> timetable(dayCount * 2);

MCMF mcmf;

void MakeGraph() {
	int count = 0;
	for (int i = 0; i < profList.size(); ++i) {
		mcmf.AddEdge(source, profNodeBegin + i, lecPerProf[profList[i]], 0);
		for (int j = 0; j < dayCount; ++j) {
			for (int k = 0; k < 2; ++k) {
				int profIdx = prof2Idx[profList[i]];
				int dayIdx = dayNodeBegin + k * 5 + j;

				mcmf.AddEdge(profNodeBegin + i, dayIdx, 1, -preference[profIdx][dayIdx - dayNodeBegin]);
			}
		}
	}

	count = 0;
	for (int i = 0; i < dayCount; ++i)
		for (int j = 0; j < 2; ++j)
			for (int k = 0; k < labCount; ++k)
				mcmf.AddEdge(dayNodeBegin + j * 5 + i, labNodeBegin + count++, 1, 0);

	for (int i = 0; i < labCount * dayCount * 2; ++i)
		mcmf.AddEdge(labNodeBegin + i, sink, 1, 0);
}
int main() {
	printf("강의실 수: ");
	scanf("%d", &labCount);

	ExcelRead("lec.xls");
	MakeGraph();

	int ans = mcmf.Match(source, sink);	//source, sink

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