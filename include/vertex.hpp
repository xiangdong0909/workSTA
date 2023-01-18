#include <string>
#include <unordered_map>
#include <vector>

namespace xiangdongSTA {

typedef std::string Cellname;
typedef std::string Vertexname;
typedef std::string Pinname;
typedef double Delay;
typedef double Arrival;
typedef double Require;
typedef double Slack;
typedef double Tclock;

const int INF = 1E9;

struct PinEdge {
  Pinname pfrom, pto;
  Delay pdelay;
};

class Cell {
 public:
  Cell(const Cellname& c, const std::vector<PinEdge*>& e)
      : name_(c), pin_edges_(e) {}
  Cellname name() const { return name_; }
  std::vector<PinEdge*> pin_edges() const { return pin_edges_; }
  Delay getDelay(Pinname p) const {
    for(PinEdge* pedge: pin_edges_) {
      if(pedge->pfrom == p) return pedge->pdelay;
    }
    return 0;
  }

 protected:
  Cellname name_;
  std::vector<PinEdge*> pin_edges_;
};

std::unordered_map<Cellname, Cell*> cell;

struct NextVertexPin {
  Vertexname vname;
  Pinname pname;
};

struct PreVertexPin {
  Vertexname vname;
  Pinname pname;
};

class Vertex {
 public:
  Vertex(const Vertexname& v, const Cellname& t) : name_(v), type_(t) {
    arrival_ = INF;
    require_ = INF;
    slack_ = INF;
  }
  Vertexname name() const { return name_; }
  Cellname type() const { return type_; }
  Arrival arrival() const { return arrival_; }
  void setarrival(const Arrival& a) { arrival_ = a; }
  Require require() const { return require_; }
  void setrequire(const Require& r) { require_ = r; }
  Slack slack() const { return slack_; }
  void setslack(const Slack& s) { slack_ = s; }
  std::vector<NextVertexPin*> nexts() const { return nexts_; }
  void addnexts(NextVertexPin* vp) { nexts_.push_back(vp); }
  std::vector<PreVertexPin*> pres() const { return pres_; }
  void addpres(PreVertexPin* vp) { pres_.push_back(vp); }

 protected:
  Vertexname name_;
  Cellname type_;
  Arrival arrival_;
  Require require_;
  Slack slack_;
  std::vector<NextVertexPin*> nexts_;
  std::vector<PreVertexPin*> pres_;
};

std::unordered_map<Vertexname, Vertex*> vertex;

class Edge {
 public:
  Edge(const Vertex& vf, const Vertex& vt, const Pinname& pf, const Pinname& pt)
      : vfrom_(vf), vto_(vt), pfrom_(pf), pto_(pt) {}
  Vertex vfrom() const { return vfrom_; }
  Vertex vto() const { return vto_; }
  Pinname pfrom() const { return pfrom_; }
  Pinname pto() const { return pto_; }

 protected:
  Vertex vfrom_, vto_;
  Pinname pfrom_, pto_;
};

class Graph {
 public:
  Graph(const Tclock& t, const std::vector<Vertex*>& v,
        const std::vector<Edge*>& e)
      : t_(t), vertexs_(v), edges_(e) {
    init();
  }
  void init() {
    for (Vertex* v : vertexs_) {
      if (!v->pres().size()) starts_.push_back(v);
      if (!v->nexts().size()) ends_.push_back(v);
    }
  }
  Delay pinLen(Vertex* v, Pinname p) {
    if(!p.size()) return 0;
    Cell* vcell = cell[v->type()];
    return vcell->getDelay(p);
  }
  void dfs_next(Vertex* v) {
    if(v->require() != INF) return ;
    if(!v->nexts().size()) {
      v->setrequire(t_);
      return ;
    }
    Require requireMin = INF;
    for(NextVertexPin* nextvp : v->nexts()) {
      Vertex* nextv = vertex[nextvp->vname];
      Pinname nextp = nextvp->pname;
      dfs_next(nextv);
      requireMin = std::min(requireMin, nextv->require() - pinLen(nextv, nextp));
    }
  }
  void dfs_pre(Vertex* v) {
    if (v->arrival() != INF) return ;
    if(!v->pres().size()) {
      v->setarrival(0);
      return ;
    }
    Arrival arrivalMax = -INF;
    for(PreVertexPin* prevp : v->pres()) {
      Vertex* prev = vertex[prevp->vname];
      Pinname prep = prevp->pname;
      dfs_pre(prev);
      arrivalMax = std::max(arrivalMax, prev->arrival() - pinLen(prev, prep));
    }
  }
  void calcArrival() {
    for (Vertex* v : ends_) {
      dfs_pre(v);
    }
  }
  void calcRequire(){
    for(Vertex* v : starts_) {
      dfs_next(v);
    }
  }
  void calcSlack();

 protected:
  Tclock t_;
  std::vector<Vertex*> vertexs_;
  std::vector<Vertex*> starts_;
  std::vector<Vertex*> ends_;
  std::vector<Edge*> edges_;
};

}  // namespace xiangdongSTA