#include"main.h"
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

		if (prac[0] == 'Y') {
			++lecPerProf[prof];
			++totalLecCount;
		}
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

	xls.New(2);

	BasicExcelWorksheet* sheet = xls.GetWorksheet(0);
	sheet->Rename("Timetable");
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

	sheet = xls.GetWorksheet(1);
	sheet->Rename("Preference");
	
	//Base form for timetable
	cFmt.set_format_string(XLS_FORMAT_TEXT);
	cFmt.set_font(ExcelFont().set_weight(FW_BOLD));
	cFmt.set_alignment(EXCEL_HALIGN_CENTRED);
	sheet->MergeCells(0, 1, 1, 10);
	cell = sheet->Cell(0, 1);
	cell->Set(Str2Wstr(string("선호도")).c_str());
	cell->SetFormat(cFmt);
	for (int i = 0; i < dayCount; ++i) {
		sheet->MergeCells(1, i * 2 + 1, 1, 2);
		cell = sheet->Cell(1, i * 2 + 1);
		cell->Set(Str2Wstr(dayName[i]).c_str());
		cell->SetFormat(cFmt);

		cell = sheet->Cell(2, i * 2 + 1);
		cell->Set(Str2Wstr("오전").c_str());
		cell->SetFormat(cFmt);

		cell = sheet->Cell(2, i * 2 + 2);
		cell->Set(Str2Wstr("오후").c_str());
		cell->SetFormat(cFmt);
	}

	for (int i = 0; i < profList.size(); ++i) {
		cell = sheet->Cell(i + 3, 0);
		cell->Set(Str2Wstr(profList[i]).c_str());
		cell->SetFormat(cFmt);
	}

	//Data
	cFmt.set_font(ExcelFont().set_weight(FW_NORMAL));
	for(int i = 1; i < preference.size(); ++i){
		for (int j = 0; j < preference[i].size(); ++j) {
			cell = sheet->Cell(i + 2, j + 1);
			cell->Set(preference[i][j]);
			cell->SetFormat(cFmt);
		}
	}
	xls.SaveAs(fileName);
	return 1;
}