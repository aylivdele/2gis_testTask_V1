// 2gis_testTask_V1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <fstream>

#include "Vertex.cpp"
#include "Edge.cpp"
#include "Graph.cpp"
#include "CompleteSolution.cpp"
#include "PartialSolution.cpp"


using namespace std;
using NameT = int;
using WeightT = int;
using VertexT = Vertex<NameT>;
using EdgeT = Edge<NameT, WeightT, VertexT>;
using GraphT = Graph<EdgeT, VertexT, string>;

void main(int argc, char* argv[])
{
    setlocale(LC_ALL, "rus");
    string fileName = "";
    bool print = false;
    string mode = "np_complete";
    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '-')
            switch (argv[i][1]) {
            case 'f':
                fileName = argv[i + 1];
                break;
            case 'm':
                if (strcmp(argv[i + 1], "np_complete") || strcmp(argv[i + 1] ,"np_partial"))
                    mode = argv[i + 1];
                else
                    cout << "Invalid mode " << argv[i + 1] << endl;
                break;
            case 'h':
                print = true;
                break;
            }
    }
    GraphT* graph = new GraphT(false);
    ifstream file(fileName);
    if (file)
    {
        int count = 0;
        int temp;

        while (!file.eof())
        {
            file >> temp;
            count++;
        }

        file.seekg(0, ios::beg);
        file.clear();

        count = (int)sqrt(count);

        for (int i = 0; i < count; i++)
            graph->insertVertex()->setName(i);

        for (int i = 0; i < count; i++) {
            for (int j = 0; j < count; j++) {
                file >> temp;
                if (j <= i || temp == 0)
                    continue;
                else
                    graph->insertEdge(i, j)->setWeight(temp);
            }
        }
        file.close();
        graph->printM();
    }
    else {
        cout << "Couln`t not open file\n";
        exit(-1);
    }

    if (print) {
        cout << "np_complete - решение задачи коммивояжера полным перебором\n";
        cout << "np_partial - решение задачи коммивояжера алгоритмом Литтла\n";
        cout << "Vertex.cpp - реализация вершины графа\n";
        cout << "Edge.cpp - реализация грани графа\n";
        cout << "Graph.cpp - реализация графа\n";
        cout << "CompleteSolution.cpp - реализация полного перебора\n";
        cout << "PatrialSolution.cpp - реализация алгоритма Литтла\n";
    }

    pair<vector<VertexT*>,WeightT> solution;
    
    if (mode == "np_complete") {
        auto completeSolution = CompleteSolution<EdgeT, VertexT, NameT, WeightT, GraphT>(graph);
        completeSolution.solve();
        solution = completeSolution.getResult();
        cout << "Complete solution:\n";
        for (auto vertex : solution.first)
            cout << vertex->getName() << " ";
        cout <<endl << solution.second << endl;
    }

    if (mode == "np_partial") {
        auto partialSolution = PartialSolution<EdgeT, VertexT, NameT, WeightT, GraphT>(graph, numeric_limits<WeightT>::max());
        partialSolution.solve();
        solution = partialSolution.getResult();

        cout << "Partial solution:\n";
        for (auto vertex : solution.first)
            cout << vertex->getName() << " ";
        cout << endl << solution.second << endl;
    }
}
