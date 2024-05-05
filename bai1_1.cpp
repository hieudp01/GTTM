#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include <stdio.h>

typedef struct Edge {
   std::string id;
   std::string from;
   std::string to;
   std::string shape;
}Edge;

typedef struct EdgeJunc {
   std::string id;
   std::string lane_id;
   std::string shape;
   std::string type;
}EdgeJunc;

typedef struct Connection {
   std::string from;
   std::string to;
   std::string via;
}Connection;

typedef struct Junction {
   std::string id;
   std::string type;
   std::string shape;
}Junction;

std::vector<Edge> readEdgesFromFile(const std::string &fileName) {
    std::ifstream file(fileName);
    std::vector<Edge> edges;
    std::string line;
    Edge currentEdge;

    while (getline(file, line)) {
        if (line.find("<edge ") != std::string::npos) {
            currentEdge = Edge();
            size_t id_pos = line.find("id=\"") + 4;
            size_t id_end = line.find("\"", id_pos);
            currentEdge.id = line.substr(id_pos, id_end - id_pos);
            
            size_t from_pos = line.find("from=\"") + 6;
            size_t from_end = line.find("\"", from_pos);
            currentEdge.from = line.substr(from_pos, from_end - from_pos);

            size_t to_pos = line.find("to=\"") + 4;
            size_t to_end = line.find("\"", to_pos);
            currentEdge.to = line.substr(to_pos, to_end - to_pos);
            
         
            
        } else if (line.find("<lane ") != std::string::npos) {
            size_t id_pos = line.find("id=\"") + 4;
            size_t id_end = line.find("\"", id_pos);

            if(line.find("E") != std::string::npos) {
                if (line.find("disallow") != std::string::npos) {
                    size_t shape_pos = line.find("shape=\"") + 7;
                    size_t shape_end = line.find("\"", shape_pos);
                    currentEdge.shape = line.substr(shape_pos, shape_end -shape_pos);   
                }
            }

        } else if (line.find("</edge>") != std::string::npos) {
            if (currentEdge.id.find("E") != std::string::npos) {
                edges.push_back(currentEdge);
            }
        }
    }
    file.close();
    return edges;
}

std::vector<Connection> readConnectionsFromFile(const std::string &fileName) {
    std::ifstream file(fileName);
    std::vector<Connection> connections;
    std::string line;
    Connection currentConn;
    while (getline(file, line)) {
        if (line.find("<connection ") != std::string::npos) {
            if (line.find("via") != std::string::npos) {
                currentConn = Connection();
                size_t from_pos = line.find("from=\"") + 6;
                size_t from_end = line.find("\"", from_pos);
                currentConn.from = line.substr(from_pos, from_end - from_pos);
            
                size_t to_pos = line.find("to=\"") + 4;
                size_t to_end = line.find("\"", from_pos);
                currentConn.to = line.substr(to_pos, to_end - to_pos);

                size_t via_pos = line.find("via=\"") + 5;
                size_t via_end = line.find("\"", via_pos);
                currentConn.via = line.substr(via_pos, via_end - via_pos);
                
                if(currentConn.from.find("E") != std::string::npos) {
                    connections.push_back(currentConn);
                }
            
            }
        }
    }

    return connections;
}

std::vector<EdgeJunc> readEdgeJuncsFromFile(const std::string& fileName) {
    std::ifstream file(fileName);
    std::vector<EdgeJunc> EdgeJuncs;
    std::string line;
    EdgeJunc currentEdgeJunc;
    while (getline(file, line)) {
        if (line.find("<edge ") != std::string::npos) {
            if (line.find(":J") != std::string::npos) {
                size_t id_pos = line.find("id=\"") + 4;
                size_t id_end = line.find("\"", id_pos);
                currentEdgeJunc.id = line.substr(id_pos, id_end - id_pos);

            }
        } else if (line.find("<lane ") != std::string::npos) {
            if (line.find(":J") != std::string::npos) {
                size_t lane_id_pos = line.find("id=\"") + 4;
                size_t lane_id_end = line.find("\"", lane_id_pos);
                currentEdgeJunc.lane_id = line.substr(lane_id_pos, lane_id_end - lane_id_pos);
                
                size_t shape_pos = line.find("shape=\"") + 7;
                size_t shape_end = line.find("\"", shape_pos);
                currentEdgeJunc.shape = line.substr(shape_pos, shape_end - shape_pos);


            }
        } else {
            EdgeJuncs.push_back(currentEdgeJunc);
        }
    }

    return EdgeJuncs;
}

std::vector<Junction> readJunctionsFromFile(const std::string& fileName) {
    std::ifstream file(fileName);
    std::vector<Junction> Junctions;
    std::string line;
    Junction currentJunction;
    while (getline(file, line)) {
        if (line.find("<junction ") != std::string::npos) {
            size_t id_pos = line.find("id=\"") + 4;
            size_t id_end = line.find("\"", id_pos);
            currentJunction.id = line.substr(id_pos, id_end - id_pos);
             
            size_t type_pos = line.find("type=\"") + 6;
            size_t type_end = line.find("\"", type_pos);
            currentJunction.type = line.substr(type_pos, type_end - type_pos);

            size_t shape_pos = line.find("shape=\"") + 7;
            size_t shape_end = line.find("\"", shape_pos);
            currentJunction.shape = line.substr(shape_pos, shape_end - shape_pos);
            
            //std::cout << currentJunction.shape << std::endl;
            Junctions.push_back(currentJunction);
        
        } 
    }

    return Junctions;
}

void printEdge(const std::vector<Edge>& edges) {
    for (const auto& edge : edges) {
        if (edge.id == "E10") {
            std::cout << edge.id << " " << edge.from << " " << edge.to << " " << edge.shape << std:: endl;
        }
    }
}

void printEdge2Juncs(const std::vector<Edge>& edges, const std::vector<Connection>& connections, const std::vector<EdgeJunc>& EdgeJuncs) {
    for(const auto edge : edges) {
        for (const auto EdgeJunc : EdgeJuncs) {
            for (const auto connection : connections) {
                if (edge.id == connection.from) {
                    if (connection.via == EdgeJunc.lane_id) {
                        std::cout << edge.id << " " << edge.shape << " " <<  EdgeJunc.id << " " << EdgeJunc.shape << std::endl;
                    }
                }
            }
        }
    }
}

void printStart(const std::vector<Edge>& edges, const std::vector<Junction>& junctions) {
    std::cout << "The list of edge have type start: " << std::endl;
    for(const auto edge : edges) {
        for(const auto junction : junctions) {
            if (junction.id == edge.from) {
                if (junction.type == "dead_end") {
                    std::cout << edge.id << " " << edge.from << " " << edge.to << " " << edge.shape << std:: endl;
                }
            }
        }
    }
}

void splitStart(double x, std::string name, const std::vector<Edge> &edges) {
    for (const auto edge : edges) {
        if (edge.id == name) {
            double num_start, num_end;

            size_t space_pos = edge.shape.find(" ");
            if (space_pos != std::string::npos) {
                int count = 0;
                std::string first_part = edge.shape.substr(0, space_pos);
                std::string second_part = edge.shape.substr(space_pos + 1);
                
                std::istringstream iss(first_part);
                std::istringstream iss2(second_part);

                std::string start;
                std::string end;
                std::getline(iss, start, ',');
                std::getline(iss2, end, ',');

                std::istringstream iss_start(start);
                std::istringstream iss_end(end);
                iss_start >> num_start;

                iss_end >> num_end;
                std::cout << name << " ";
                std::cout << num_start << num_end << std::endl;
                while (num_start < num_end) {
                    std::cout << count << "_";
                    std::cout << num_start << " ";
                   
                    if (num_end - num_start < x) {
                        num_start = num_end;
                       

                    }
                    else {
                        num_start += x;
                    }
                    count += 1;
                }
                
            }
        }
    }
}

int main() {
    std::string fileName = "map2/map2.net.xml";
    std::vector<Edge> edges = readEdgesFromFile(fileName);
    std::vector<EdgeJunc> EdgeJuncs = readEdgeJuncsFromFile(fileName);
    std::vector<Connection> connections = readConnectionsFromFile(fileName);
    std::vector<Junction> junctions = readJunctionsFromFile(fileName);
    

    for(const auto EdgeJunc : EdgeJuncs) {
        //std::cout << EdgeJunc.id << std::endl;
    }
    return 0;
}