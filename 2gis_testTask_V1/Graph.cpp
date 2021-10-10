#include <vector>
#include <iostream>
#include <stddef.h>
using namespace std;

template<class EdgeT, class VertexT, class NameT>
class Graph {
private:
	vector<VertexT*> vertexVector;
	vector<vector<EdgeT*>> matrix;
	bool directed;
public:
	Graph():   directed(false){}
	Graph(bool directed) : directed(directed) {}

	~Graph() {
		while (vertexVector.size() != 0)
			deleteVertex(vertexVector.size() - 1);
	}

	vector<vector<EdgeT*>> getMatrix() {
		return matrix;
	}

	VertexT* insertVertex() {
		return insertVertex(matrix.size());
	}

	VertexT* insertVertex(size_t index) {
		size_t size = matrix.size();
		if (index < 0 || index > size) 
			throw new exception("Invalid index " + index);
		vector<EdgeT*> newLine;
		newLine.assign(size, NULL);
		matrix.insert(matrix.begin() + index, newLine);
		++size;
		for (size_t i = 0; i < size; i++)
			matrix[i].insert(matrix[i].begin() + index, NULL);

		VertexT* vertex = new VertexT();
		vertexVector.insert(vertexVector.begin() + index, vertex);
		return vertex;
	}

	void deleteVertex(size_t index) {
		size_t size = matrix.size();
		if (index < 0 || index >= size)
			throw new exception("Invalid index " + index);
		deleteEdgesFromVertex(index);
		matrix.erase(matrix.begin() + index);
		--size;
		for (int i = 0; i < size; i++)
			matrix[i].erase(matrix[i].begin() + index);
		vertexVector.erase(vertexVector.begin() + index);
	}

	VertexT* getVertex(size_t index) {
		if (index < 0 || index >= vertexVector.size())
			throw new exception("Invalid index " + index);
		return vertexVector[index];
	}

	VertexT* getVertexFromName(NameT name) {
		for (VertexT* vertex : vertexVector)
			if (vertex->getName() == name)
				return vertex;
		throw new exception("No vertex with name " + name);
	};

	EdgeT* insertEdge(size_t index1, size_t index2) {
		size_t size = matrix.size();
		if (index1 < 0 || index1 >= size)
			throw new exception("Invalid index " + index1);
		if (index2 < 0 || index2 >= size)
			throw new exception("Invalid index " + index2);

		if (index1 == index2 || matrix[index1][index2] != NULL) 
			throw new exception("Edge already exists");
		EdgeT* newEdge = new EdgeT(vertexVector[index1], vertexVector[index2]);
		matrix[index1][index2] = newEdge;
		if (!directed)
			matrix[index2][index1] = newEdge;
		return newEdge;
	}

	void deleteEdge(size_t index1, size_t index2) {
		size_t size = matrix.size();
		if (index1 < 0 || index1 >= size)
			throw new exception("Invalid index " + index1);
		if (index2 < 0 || index2 >= size)
			throw new exception("Invalid index " + index2);

		if (index1 == index2 || matrix[index1][index2] == NULL) 
			throw new exception("No edge with given vertex indices");
		delete matrix[index1][index2];
		matrix[index1][index2] = NULL;
		if (!directed)
			matrix[index2][index1] = NULL;
	}

	void deleteEdgesFromVertex(size_t index) {
		size_t size = matrix.size();
		if (index < 0 || index >= size) 
			throw new exception("Invalid index " + index);

		for (int i = 0; i < size; i++) {
			if (matrix[i][index] != NULL) {
				delete matrix[i][index];
				matrix[i][index] = NULL;
				if (!directed)
					matrix[index][i] = NULL;
			}
		}
	}

	bool hasEdge(size_t index1, size_t index2) {
		size_t size = matrix.size();
		if (index1 < 0 || index1 >= size)
			throw new exception("Invalid index " + index1);
		if (index2 < 0 || index2 >= size)
			throw new exception("Invalid index " + index2);
		if (index1 == index2)
			return false;
		if (matrix[index1][index2] != NULL)
			return true;
		return false;
	}

	bool hasEdge(VertexT* vertex1, VertexT* vertex2) {
		return hasEdge(getVertexIndex(vertex1), getVertexIndex(vertex2));
	}

	EdgeT* getEdge(VertexT* vertex1, VertexT* vertex2) {
		EdgeT* edge = getEdge(getVertexIndex(vertex1), getVertexIndex(vertex2));
		if (edge == NULL)
			return getEdge(getVertexIndex(vertex2), getVertexIndex(vertex1));
		return edge;
	}

	EdgeT* getEdge(size_t index1, size_t index2) {
		size_t size = matrix.size();
		if (index1 < 0 || index1 >= size)
			throw new exception("Invalid index " + index1);
		if (index2 < 0 || index2 >= size)
			throw new exception("Invalid index " + index2);
		if (index1 == index2 || matrix[index1][index2] == NULL)
			//throw new exception("No edge with such indecis: " + index1 + " " + index2);
			return NULL;
		return matrix[index1][index2];
	}

	int getVertexCount() {
		return vertexVector.size();
	}

	int getVertexIndex(VertexT* vertex) {
		auto it = find(vertexVector.begin(), vertexVector.end(), vertex);
		if (it != vertexVector.end())
			return it - vertexVector.begin();
		else
			return -1;
	}

	vector<VertexT*> getVertexVector() {
		return vertexVector;
	}

	void printL() {
		VertexT* v;
		int size = vertexVector.size();
		for (int i = 0; i < size; i++) {
			v = getVertex(i);
			cout << "*" << v->getName();
			for (int j = 0; j < size; j++) {
				v = getVertex(j);
				if (hasEdge(i, j))
					cout << "->" << v->getName();
			}
			cout << endl;
		}
		
	};

	void printM() {
		size_t i, j;
		VertexT* v;
		EdgeT* e;
		cout << "  ";
		for (i = 0; i < vertexVector.size(); i++) {
			v = getVertex(i);
			cout << "    " << v->getName();
		}
		cout << endl;
		for (i = 0; i < 6 * vertexVector.size(); i++)
			cout << "_";
		cout << endl;
		for (i = 0; i < vertexVector.size(); i++) {
			v = getVertex(i);
			cout << v->getName() << "|";
			for (j = 0; j < vertexVector.size(); j++)
				if (hasEdge(i, j)) {
					e = getEdge(i, j);
					cout << "    " << e->getWeight();
				}
				else
					cout << "    " << "0";
			cout << endl;
		}
		

	}


};