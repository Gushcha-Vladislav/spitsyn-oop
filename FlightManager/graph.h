#ifndef GRAPH_H
#define GRAPH_H

#include <vertex.h>
#include <iterator.h>
#include <iostream>
#include "exceptions.h"
#include <QGraphicsScene>
#include "invariant.h"

namespace GraphLib {
    template< template<class E> class V, class E>
    class Graph {
        private:
            std::vector<V<E>,AllocatorLib::AllocImpl<V<E>>> *vertices;

        public:
            Graph(){
                this->vertices = new std::vector<V<E>,AllocatorLib::AllocImpl<V<E>>>;
            }

            Graph( std::vector<int,AllocatorLib::AllocImpl<V<E>>> vertices){
                this->vertices = vertices;
            }

            V<E> add_vertex(V<E>* vertex){
                this->vertices->push_back(*vertex);
                return this->vertices->at(this->vertices->size()-1);
            }

            ~Graph(){
                this->vertices->clear();
                delete this->vertices;
            }

            void delete_vertex(int vertex_id){
                for(auto iter = this->vertices->begin();iter!=this->vertices->end(); ++iter)
                {
                    auto iterEdge = iter->get_edges()->begin();
                    while (iterEdge != iter->get_edges()->end()) {
                        if(iterEdge->get_to_id() == vertex_id){
                            iterEdge = iter->get_edges()->erase(iterEdge);
                        }
                        else
                            ++iterEdge;
                    }

                }
                for(auto iter = this->vertices->begin();iter!=this->vertices->end(); ++iter)
                {
                    if(iter->get_id() == vertex_id){
                        this->vertices->erase(iter);
                        break;
                    }
                }
            }

            bool add_edge(int vertex_id, E *edge){
                if (Invariant<Graph>::loopChecker(vertex_id, edge->get_to_id())){
                    for(auto iter = this->vertices->begin();iter!=this->vertices->end(); ++iter)
                    {
                        if(iter->get_id() == vertex_id){
                            iter->add_edge(edge);
                            return true;
                        }
                    }
                }
                else {
                    throw EdgeLoopException(vertex_id, edge);
                }

            }

            void delete_edge(int vertex_id, int edge_id){
                for(auto iter = this->vertices->begin();iter!=this->vertices->end(); ++iter)
                {
                    if(iter->get_id() == vertex_id)
                        iter->delete_edge(edge_id);
                }
            }

            V<E> *getVertex(int vertex_id){
                int i = 0;
                for(auto iter = this->vertices->begin();iter!=this->vertices->end(); ++iter,i++)
                {
                    if(iter->get_id() == vertex_id){
                        return &this->vertices->at(i);
                    }
                }
                return nullptr;
            }

            V<E> *getVertex(std::string vertex_name){
                int i = 0;
                for(auto iter = this->vertices->begin();iter!=this->vertices->end(); ++iter,i++)
                {
                    if(!iter->get_name().compare(vertex_name)){
                        return &this->vertices->at(i);
                    }
                }
                return nullptr;
            }

            std::vector<V<E>,AllocatorLib::AllocImpl<V<E>>>* getVertices(){
                return this->vertices;
            }

        QGraphicsScene* draw();
        std::string find_way(int from_id, int to_id);

            typedef Iterator<V<E>>* iterator;

            iterator begin(){
                return new Iterator<V<E>>(vertices, 0);
            }
            iterator end(){
                return new Iterator<V<E>>(this->vertices, this->vertices->size());
            }
        private:
    };
}


#include <graph.cpp>
#endif // GRAPH_H
