#include <iostream>
#include <vector>

using namespace std;

template<class EdgeT, class VertexT, class NameT, class WeightT, class GraphT>
class CompleteSolution {
private:
	GraphT* graph;
	WeightT resultDistance = (WeightT)-1;
	vector<VertexT*> resultVector;
	vector<VertexT*> currentVector;
	bool found = false;

	void bruteForce(size_t index) {
		bool bre = false;
		WeightT distance;
		if (index >= currentVector.size()) {
			distance = CalcDistance();
			if (distance != (WeightT)0 && (distance < resultDistance || !found)) {
				found = true;
				resultDistance = distance;
				resultVector.clear();
				for (VertexT* vertex : currentVector)
					resultVector.push_back(vertex);
			}
			return;
		}
		bruteForce(index + 1);
		for (size_t i = index + 1; i < currentVector.size(); i++) {
			swap(index, i);
			bruteForce(index + 1);
			swap(index, i);
		}
	}

	WeightT CalcDistance() {
		WeightT result = (WeightT)0;
		if (graph->hasEdge(currentVector[0], currentVector[currentVector.size() - 1]))
			result += graph->getEdge(currentVector[0], currentVector[currentVector.size() - 1])->getWeight();
		else
			return (WeightT)0;
		for (size_t i = 1; i < currentVector.size(); i++)
			if (graph->hasEdge(currentVector[i - 1], currentVector[i]))
				result += graph->getEdge(currentVector[i - 1], currentVector[i])->getWeight();
			else
				return (WeightT)0;
		return result;
	}

	void swap(int index1, int index2) {
		iter_swap(currentVector.begin() + index1, currentVector.begin() + index2);
	}
public:
	CompleteSolution(GraphT* graph): graph(graph){}

	void solve() {
		currentVector = graph->getVertexVector();
		bruteForce(1);
	}

	pair<vector<VertexT*>,WeightT> getResult() {
		if (found)
			return make_pair( resultVector, resultDistance);
		else
			return pair<vector<VertexT*>, WeightT>();
	}

	

};