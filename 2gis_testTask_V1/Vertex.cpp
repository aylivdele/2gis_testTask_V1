#include <stddef.h>
template <class NameT>
class Vertex {
private:
	NameT name;
public:
	Vertex(){}
	Vertex(NameT name): name(name) {}

	void setName(NameT name) {
		this->name = name;
	}

	NameT getName() {
		return name;
	}
};