#pragma once

#include <vector>
#include <queue>
#include <iostream>

using namespace std;

template<class EdgeT, class VertexT, class NameT, class WeightT, class GraphT>
class PartialSolution{

	using Matrix = vector<vector<WeightT>>;
	using IJ = pair<size_t, size_t>;
	using Route = vector<IJ>;
	using Weight = numeric_limits<WeightT>;
	GraphT* graph;
	Route resultRoute;
	WeightT resultRecord;
	WeightT infinity = -1;
	WeightT infinity2 = -2;
	Matrix sourceM;
	vector<VertexT*> resultVertexVector;
public:
	PartialSolution(GraphT* g, WeightT record = Weight::max()):graph(g), resultRecord(record){}

	void solve() {
		
		size_t size = graph->getVertexCount();
		Matrix temp(size, vector<WeightT>(size, 0));
		Matrix m = temp;
		bool fill = false;
		for (size_t i = 0; i < size; i++) {
			for (size_t j = 0; j < size; j++) {
				if (i == j)
					m[i][j] = 0;
				else {
					if (graph->hasEdge(i, j)) {
						m[i][j] = graph->getEdge(i, j)->getWeight();
					}
					else {
						m[i][j] = Weight::max();
						fill = true;
					}
					
				}
			}
		}
		if (fill)
			m = floidFill(m, 1);
		for (size_t i = 0; i < m.size(); i++)
			m[i][i] = infinity;
		sourceM = m;
		littleRecursion(m, Route(), 0, 1);
		collectResult();
	}

	pair<vector<VertexT*>,WeightT> getResult() {
		return make_pair(resultVertexVector, resultRecord);
	}

private:
	void print(Matrix m) {
		return;
		for (size_t i = 0; i < m.size(); i++) {
			for (size_t j = 0; j < m.size(); j++)
				if (m[i][j] >= 0)
					cout << m[i][j] << " ";
				else
					cout << m[i][j] << " ";
			cout << endl;
		}
		cout << endl;
	}

	void collectResult() {
		
		vector<size_t> result;
		result.push_back(0);
		while (!resultRoute.empty()) {
			auto iter = resultRoute.begin();
			while (iter!= resultRoute.end()) {
				if (iter->first == result.back()) {
					result.push_back(iter->second);
					iter = resultRoute.erase(iter);
				}
				else if (iter->second == result.back()) {
					result.push_back(iter->first);
					iter = resultRoute.erase(iter);

				}
				else
					++iter;
			}
		}
		for (auto i : result)
			resultVertexVector.push_back(graph->getVertex(i));
		
	}

	WeightT cost(Route route) {
		WeightT result(0);
		for (IJ edge : route)
			result += sourceM[edge.first][edge.second];
		return result;
	}

	void littleRecursion(Matrix m, Route route, WeightT bottomLimit, size_t depth) {

		//cout << depth << endl;
		print(m);
		if (m.size() - depth + 1 <= 2) {
			addLastPairs(m, &route);
			WeightT curCost = cost(route);
			if (curCost < resultRecord)
			{
				//cout << "New record\n";
				resultRecord = curCost;
				resultRoute = route;
				return;
			}
			else {
				//cout << "Bad record\n";
				return;
			}
		}

		bottomLimit += substractRows(&m);
		print(m);
		bottomLimit += substractColumns(&m);
		print(m);
		if (bottomLimit > resultRecord)
			return;
		Route biggestCoefs = getCoefPair(m);

		for (auto coef : biggestCoefs) {
			Matrix newMatrix(m);
			print(newMatrix);
			removeRowAndColumn(&newMatrix, coef.first, coef.second);
			//newMatrix[coef.second][coef.first] = infinity2;
			Route newRoute(route);
			newRoute.push_back(coef);
			addInfinity(&newMatrix);
			print(newMatrix);
			//cout << "Pair " << coef.first << " " << coef.second << endl;
			littleRecursion(newMatrix, newRoute, bottomLimit, depth + 1);
		}

		/*m[coefPair.first][coefPair.second] = infinity;
		//cout << "Without edge\n";
		littleRecursion(m, route, bottomLimit, depth);*/
	}

	WeightT substractColumns(Matrix *m) {
		WeightT substractSum = 0;
		for (size_t i = 0; i < m->size(); i++) {
			WeightT min = Weight::max();
			for (size_t j = 0; j < m->size(); j++) {
				if ((*m)[j][i] < min && (*m)[j][i] >= 0)
					min = (*m)[j][i];
				
			}
			if (min != 0) {
				for (size_t j = 0; j < m->size(); j++) {
					if ((*m)[j][i] > 0)
						(*m)[j][i] -= min;
				}
				substractSum += min;
			}
		}
		return substractSum;
	}

	WeightT substractRows(Matrix *m) {
		WeightT substractSum = 0;
		for (size_t i = 0; i < m->size(); i++) {
			WeightT min = Weight::max();
			for (size_t j = 0; j < m->size(); j++) {
				if ((*m)[i][j] < min && (*m)[i][j] >= 0)
					min = (*m)[i][j];
			}
			if (min != 0) {
				for (size_t j = 0; j < m->size(); j++) {
					if ((*m)[i][j] > 0)
						(*m)[i][j] -= min;
				}
				substractSum += min;
			}
			else continue;
		}	
		return substractSum;
	}

	Route getCoefPair(Matrix m) {
		Route maxPairs;
		WeightT maxCoef = 0;
		WeightT temp;
		for (size_t i = 0; i < m.size(); i++) {
			for (size_t j = 0; j < m.size(); j++) {
				if (m[i][j] == 0) {
					temp = calcCoef(m, i, j);
					if (temp > maxCoef) {
						maxPairs.clear();
						maxPairs.push_back(make_pair(i, j));
						maxCoef = temp;
					}
					else if (temp == maxCoef)
						maxPairs.push_back(make_pair(i, j));
				}
			}
		}
		return maxPairs;
	}

	WeightT calcCoef(Matrix m,size_t column, size_t row) {
		WeightT coef = 0;
		WeightT min = Weight::max();
		for (size_t i = 0; i < m.size(); i++)
			if (m[i][row] >= 0 && m[i][row] < min && i != column)
				min = m[i][row];
		coef += min;
		min = Weight::max();
		for (size_t i = 0; i < m.size(); i++)
			if (m[column][i] >= 0 && m[column][i] < min && i != row)
				min = m[column][i];
		coef += min;
		return coef;
	}



	void removeRowAndColumn(Matrix *m, size_t column, size_t row) {
		for (size_t i = 0; i < m->size(); i++)
			(*m)[i][row] = infinity2;
		for (size_t i = 0; i < m->size(); i++)
			(*m)[column][i] = infinity2;
	}

	void addInfinity(Matrix* m) {
		vector<bool> infRow(m->size(), false), infColumn(m->size(), false);
		for (size_t i = 0; i < m->size(); i++)
			for (size_t j = 0; j < m->size(); j++)
				if ((*m)[i][j] == infinity) {
					infRow[i] = true;
					infColumn[j] = true;
				}
		for (size_t i = 0; i < infRow.size(); i++)
			for (size_t j = 0; j < infColumn.size(); j++)
				if (!infRow[i] && !infColumn[j] && (*m)[i][j] != infinity2) {
					//cout << "Added infinity " << i << " " << j << endl;
					(*m)[i][j] = infinity;
					return;
				}

	}

	void addLastPairs(Matrix m, Route *route) {
		addInfinity(&m);

		WeightT minEdge = Weight::max();
		for (size_t i = 0; i < m.size(); i++) {
			for (size_t j = 0; j < m.size(); j++) {
				if (m[i][j] != infinity && m[i][j] != infinity2)
					route->push_back(IJ(i,j));
			}
		}
	}

	Matrix floidFill(Matrix m1, size_t n) {
		auto m2 = m1;
		for (size_t i = 0; i < m1.size(); i++) {
			for (size_t j = 0; j < m1.size(); j++) {
				if (m2[i][j] == Weight::max()) {
					WeightT sum = 0;
					if (m1[i][n - 1] == Weight::max() || m1[n - 1][j] == Weight::max())
						sum = Weight::max();
					else
						sum = m1[i][n - 1] + m1[n - 1][j];
					m2[i][j] = min(sum, m1[i][j]);
				}
			}
		}
		if (n >= m1.size())
			return m2;
		return floidFill(m2, ++n);
	}

};