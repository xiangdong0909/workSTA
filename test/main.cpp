#include <fstream>
#include <iostream>

#include "sta.hpp"

using namespace STA;
using namespace std;

Tclock readSdc() {
  std::ifstream ifs;
  ifs.open("/home/xiangdong/workself/workSTA/sdc/adder.fakesdc", std::ios::in);
  if (!ifs.is_open()) {
    cout << "fail" << endl;
    return 0;
  }
  std::string str;
  double t;
  ifs >> str >> t;
  return t;
}

void readLib(std::vector<Cell*>& cells) {
  std::ifstream ifs;
  ifs.open("/home/xiangdong/workself/workSTA/lib/tsmc65.fakelib", std::ios::in);
  if (!ifs.is_open()) {
    return;
  }
  int n;
  ifs >> n;
  while (n--) {
    std::string str;
    int m;
    ifs >> str >> m;
    std::vector<PinEdge*> pinedges;
    while (m--) {
      std::string in_pin, out_pin, dis;
      ifs >> in_pin >> out_pin >> dis;
      Delay d = INF;
      if (dis != "INF") d = stod(dis);
      PinEdge pe = PinEdge{in_pin, out_pin, d};
      pinedges.push_back(&pe);
    }
    Cell cell = Cell(str, pinedges);
    cells.push_back(&cell);
  }
}

void findCellPin(const std::string& str, std::string& cell, std::string& pin) {
  int pos = str.find('/');
  if (pos == -1) {
    cell = str;
    pin = "";
  } else {
    cell = str.substr(0, pos);
    pin = str.substr(pos + 1);
  }
}

void readNetlists(std::vector<Vertex*>& vertexs, std::vector<Edge*>& edges) {
  std::ifstream ifs;
  ifs.open("/home/xiangdong/workself/workSTA/verilog/adder.fakev", std::ios::in);
  if (!ifs.is_open()) {
    return;
  }
  int n;
  ifs >> n;
  while (n--) {
    std::string name, type;
    ifs >> name >> type;
    Vertex v = Vertex(name, type);
    vertexs.push_back(&v);
  }
  int m;
  ifs >> m;
  while (m--) {
    std::string from, to;
    ifs >> from >> to;
    std::string from_cell, from_pin, to_cell, to_pin;
    findCellPin(from, from_cell, from_pin);
    findCellPin(to, to_cell, to_pin);
    Edge edge = Edge(from_cell, to_cell, from_pin, to_pin);
    edges.push_back(&edge);
  }
}

int main() {
  Tclock t = readSdc();
  std::vector<Cell*> cells;
  readLib(cells);
  std::vector<Vertex*> vertexs;
  std::vector<Edge*> edges;
  readNetlists(vertexs, edges);
  Graph graph = Graph(t, cells, vertexs, edges);
  graph.calcSlack();
  cout << "t: " << graph.t() << endl;
  cout << "starts:" << endl;
  for (Vertex* v : graph.starts()) cout << v->name() << " ";
  cout << endl;
  cout << "ends:" << endl;
  for (Vertex* v : graph.ends()) cout << v->name() << " ";
  cout << endl;
  return 0;
}