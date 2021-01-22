#include"main.h"
void MCMF::clear() {
	for (int i = 0; i < maxNode; i++) gph[i].clear();
}
MCMF::MCMF() {

}
MCMF::MCMF(char* fileName) {
	ExcelRead(fileName);
	MakeGraph();
}
MCMF::MCMF(const char* fileName) {
	ExcelRead(fileName);
	MakeGraph();
}
void MCMF::AddEdge(int s, int e, int cap, int cost) {
	gph[s].push_back({ e, cap, (int)gph[e].size(), cost });
	gph[e].push_back({ s, 0, (int)gph[s].size() - 1, -cost });
}

bool MCMF::SPFA(int src, int sink) {
	memset(dist, 0x3f, sizeof(dist));
	memset(inque, 0, sizeof(inque));
	queue<int> que;
	dist[src] = 0;
	inque[src] = 1;
	que.push(src);
	bool ok = 0;
	while (!que.empty()) {
		int x = que.front();
		que.pop();
		if (x == sink) ok = 1;
		inque[x] = 0;
		for (int i = 0; i < gph[x].size(); i++) {
			EDGE e = gph[x][i];
			if (e.cap > 0 && dist[e.pos] > dist[x] + e.cost) {
				dist[e.pos] = dist[x] + e.cost;
				pa[e.pos] = x;
				pe[e.pos] = i;
				if (!inque[e.pos]) {
					inque[e.pos] = 1;
					que.push(e.pos);
				}
			}
		}
	}
	return ok;
}

int MCMF::Match(int src, int sink) {
	int ret = 0;
	while (SPFA(src, sink)) {
		int cap = 1e9;
		for (int pos = sink; pos != src; pos = pa[pos])
			cap = min(cap, gph[pa[pos]][pe[pos]].cap);

		ret += dist[sink] * cap;
		for (int pos = sink; pos != src; pos = pa[pos]) {
			int rev = gph[pa[pos]][pe[pos]].rev;
			gph[pa[pos]][pe[pos]].cap -= cap;
			gph[pos][rev].cap += cap;
		}
	}
	return ret;
}

void MCMF::MakeGraph() {
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