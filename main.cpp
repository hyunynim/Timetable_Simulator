#include"main.h"

void PrintLog();
template<typename T> void PrintLog(T log);
template<typename T, typename... Args> void PrintLog(T log, Args... args);

struct LECTURE {
	string name;
	string prof;
};


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
vector<vector<int>> timetable;

MCMF mcmf;

bool ExcelRead(const char* fileName) {
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL) return 0;
	fclose(fp);
	BasicExcel xls(fileName);
	BasicExcelWorksheet* sheet1 = xls.GetWorksheet("Lecture");
	BasicExcelWorksheet* sheet2 = xls.GetWorksheet("Preference");
	int no, lecCode, classCode;
	string lecName, prof, prac;
	wstring wstrTmp;
	for (int i = 1; i < sheet1->GetTotalRows(); ++i) {
		for (int j = 0; j < sheet1->GetTotalCols(); ++j) {
			//no, lecCode, lecName, classCode, prof, prac
			BasicExcelCell* cell = sheet1->Cell(i, j);
			switch (j) {
			case 0:
				no = (int)cell->GetDouble();
				break;
			case 1:
				lecCode = (int)cell->GetDouble();
				break;
			case 2:
				if (cell->Type() == 4)
					lecName = Wstr2Str(cell->GetWString());
				else
					lecName = cell->GetString();
				break;
			case 3:
				classCode = cell->GetDouble();
				break;
			case 4:
				if (cell->Type() == 4)
					prof = Wstr2Str(cell->GetWString());
				else
					prof = cell->GetString();
				break;
			case 5:
				prac = cell->GetString();
				break;
			}
		}
		decomp[{lecCode, classCode}] = { lecName, prof };

		profList.push_back(prof);

		if (prac[0] == 'Y') 
			++lecPerProf[prof];
		
	}
	sort(profList.begin(), profList.end());
	profList.erase(unique(profList.begin(), profList.end()), profList.end());

	preference.resize(profList.size() + 1);

	int idx = 1;
	for (auto i : profList)
		prof2Idx[i] = idx++;

	for (int i = 3; i < sheet2->GetTotalRows(); ++i) {
		int profIndex;
		for (int j = 0; j < sheet2->GetTotalCols() - 1; ++j) {
			BasicExcelCell* cell = sheet2->Cell(i, j);
			if (j == 0) {
				if (cell->Type() == 4)
					prof = Wstr2Str(cell->GetWString());
				else
					prof = cell->GetString();
				profIndex = prof2Idx[prof];
			}
			else {
				int pref = (int)cell->GetDouble();
				preference[profIndex].push_back(pref);
			}
		}
	}
	return 1;
}
bool ExcelWrite(const char* fileName) {
	BasicExcel xls;

	//Used to separate the sheet with day of the week
	//int sheetNum = dayCount;
	//xls.New(sheetNum);

	//Code for one sheet
	xls.New(1);

	BasicExcelWorksheet* sheet = xls.GetWorksheet(0);

	XLSFormatManager fmtMgr(xls);
	CellFormat cFmt(fmtMgr);
	BasicExcelCell* cell;

	//Base form for timetable
	cFmt.set_format_string(XLS_FORMAT_TEXT);
	cFmt.set_font(ExcelFont().set_weight(FW_BOLD));
	cFmt.set_alignment(EXCEL_HALIGN_CENTRED);
	for (int i = 0; i < dayCount; ++i) {
		sheet->MergeCells(0, i * 4 + 1, 1, 4);
		cell = sheet->Cell(0, i * 4 + 1);
		cell->Set(Str2Wstr(dayName[i]).c_str());
		cell->SetFormat(cFmt);
	}

	for (int i = 1; i <= 9; ++i) {
		sprintf(globalMsg, "%d교시", i);
		cell = sheet->Cell(i, 0);
		cell->Set(Str2Wstr(string(globalMsg)).c_str());
		cell->SetFormat(cFmt);
	}

	//Data
	cFmt.set_font(ExcelFont().set_weight(FW_NORMAL));
	for (int i = 0; i < dayCount; ++i) {
		for (int k = 0; k < 4; ++k) {
			for (int j = 0; j < timetable[i].size(); ++j) {
				cell = sheet->Cell(1 + k, 1 + j + i * 4);
				cell->Set(Str2Wstr(profList[timetable[i][j]]).c_str());
				cell->SetFormat(cFmt);
			}
			for (int j = 0; j < timetable[i + dayCount].size(); ++j) {
				cell = sheet->Cell(1 + k + dayCount, 1 + j + i * 4);
				cell->Set(Str2Wstr(profList[timetable[i + dayCount][j]]).c_str());
				cell->SetFormat(cFmt);
			}
		}
	}

	xls.SaveAs(fileName);
	return 1;
}
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

	timetable.resize(dayCount * 2);
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