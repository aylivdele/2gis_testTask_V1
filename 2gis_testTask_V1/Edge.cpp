#include <stddef.h>

template <class NameT, class WeightT, class VertexT>
class Edge {
private:
	VertexT* v1, * v2;
	WeightT weight;
public:
	Edge() : v1(NULL), v2(NULL), weight(0) {}
	Edge(VertexT* v1, VertexT* v2): v1(v1), v2(v2), weight(0){}
	Edge(VertexT* v1, VertexT* v2, WeightT weight): v1(v1), v2(v2), weight(weight){}

	WeightT getWeight() {
		return weight;
	}

	void setWeight(WeightT weight) {
		this->weight = weight;
	}

	VertexT* getVertex1() {
		return v1;
	}

	VertexT* getVertex2() {
		return v2;
	}


};