#include <iostream>
#include <vector>
#include <string>
#include <map>

template <typename T>
class DirectedGraph {
private:
    std::map<T, std::vector<T>> adjacencyList;

public:
    void addVertex(const T& vertex) {
        adjacencyList[vertex]; 
    }

    void addEdge(const T& from, const T& to) {
        addVertex(from);
        addVertex(to);
        
        adjacencyList[from].push_back(to);
    }

    void printGraph() const {
        std::cout << "Состояние графа:" << std::endl;
        std::cout << "----------------" << std::endl;
        
        for (const auto& pair : adjacencyList) {
            const T& vertex = pair.first;          
            const std::vector<T>& neighbors = pair.second; 

            std::cout << "Вершина [" << vertex << "] указывает на: ";
            
            if (neighbors.empty()) {
                std::cout << "никого (нет исходящих рёбер)";
            } else {
                for (size_t i = 0; i < neighbors.size(); ++i) {
                    std::cout << neighbors[i];
                    if (i < neighbors.size() - 1) {
                        std::cout << ", ";
                    }
                }
            }
            std::cout << std::endl;
        }
        std::cout << "----------------" << std::endl;
    }
};

int main() {
    system("chcp 65001 > nul");

    std::cout << "=== ПРИМЕР 1: Граф с целыми числами (int) ===" << std::endl;
    DirectedGraph<int> intGraph;

    intGraph.addEdge(1, 2); 
    intGraph.addEdge(1, 3); 
    intGraph.addEdge(2, 4); 
    intGraph.addVertex(5); 

    intGraph.printGraph(); 


    std::cout << "\n=== ПРИМЕР 2: Граф со строками (string) ===" << std::endl;
    DirectedGraph<std::string> stringGraph;

    stringGraph.addEdge("Москва", "Санкт-Петербург");
    stringGraph.addEdge("Москва", "Казань");
    stringGraph.addEdge("Санкт-Петербург", "Калининград");
    stringGraph.addEdge("Казань", "Сочи");
    

    stringGraph.printGraph(); 

    return 0;
}