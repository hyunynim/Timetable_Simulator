#pragma once
#ifndef __MCMF_H__
#define __MCMF_H__
#endif

const int maxNode = 1010;
class MCMF {
	struct EDGE { int pos, cap, rev, cost; };
	int dist[maxNode], pa[maxNode], pe[maxNode];
	bool inque[maxNode];
	void MakeGraph();

public:
	MCMF();
	MCMF(char* fileName);
	MCMF(const char* fileName);
	MCMF(char* fileName, bool flag);
	MCMF(const char* fileName, bool flag);

	vector<EDGE> gph[maxNode];
	void clear();
	void AddEdge(int s, int e, int cap, int cost);	//x: cap, c: cost
	bool SPFA(int src, int sink);
	int Match(int src, int sink);
};
