// Copyright 2020 Roger Peralta Aranibar
#include <cassert>
#include <exception>
#include <iostream>
#include <memory>
#include <vector>

#include "DirectedGraph.hpp"
#include "PartialDirectedGraph.hpp"

int main() {
  // Create a DirectedGraph, a maximum of 5 edged each node and the root with
  // the starting valuee of 1.
  /*ADE::DirectedGraph<int> my_graph(2, 5);

  // We can obtain the root and print their value.
  ADE::Node<int>* root_ptr = my_graph.get_root_ptr();
  std::cout << "Root Value: " << *root_ptr->data_ << std::endl;

  // Also, we can insert a new vertex passing the new Node value, a Node
  // reference and the index of the pointer used from the reference node to
  // the
  // inserted node.
  ADE::Node<int>* inserted_node_ptr = my_graph.insert_vertex(3, root_ptr, 0);

  std::cout << *inserted_node_ptr->data_<<std::endl;
  // Likewise, the method to insert a new vertex, returns a reference of the
  // new
  // vertex inserted
  ADE::Node<int>* other_new_node_ptr = my_graph.insert_vertex(5, inserted_node_ptr, 1);

  // To add an edge between two vertex, we call the function using both Nodes
  // references and the index pointer that will link the first node to the
  // second.
  my_graph.add_edge(other_new_node_ptr, root_ptr, 3);

  // We traverse the Graph from a reference node (using any Node pointer
  // provided by the previous methods), with the operator []
  ADE::Node<int> retrived_node = (*root_ptr)[0][1];
  std::cout << "Node Value: " << *retrived_node.data_ << std::endl;

  // It is also possible to insertt between two Nodes.
  ADE::Node<int>* between_node_ptr = my_graph.insert_vertex(7, inserted_node_ptr, 1);
  retrived_node = (*root_ptr)[0][1];
  std::cout << "Between Value: " << *retrived_node.data_ << std::endl;

  retrived_node = (*root_ptr)[0][1][1];
  std::cout << "Next Value: " << *retrived_node.data_ << std::endl;
  */
  //int outside = 4;
  //int inside = 4;
  ADE :: Persistence :: PartialDirectedGraph<int> my_graph(3,4,5);
  ADE :: Persistence :: PartialNode<int> *root_ptr =my_graph.get_root_ptr();

  std::cout<< "Ndata root: "<< *root_ptr->data_ << std::endl;
  ADE:: Persistence :: PartialNode<int>* inserted_node_ptr = my_graph.insert_vertex(4, root_ptr, 0);
  //std::cout<< "rooTest forward version: 1 posisicion[0] = " << *(*root_ptr)[{0,1}].data_<< std::endl;  
  std::cout<< "Ndata nextroot: "<< *inserted_node_ptr->data_ << std::endl;
  ADE:: Persistence :: PartialNode<int>* Ptr_1 = my_graph.insert_vertex(7, root_ptr, 0);
  ADE:: Persistence :: PartialNode<int>* Ptr_2 = my_graph.insert_vertex(11, root_ptr, 0);
  ADE:: Persistence :: PartialNode<int>* Ptr_3 = my_graph.insert_vertex(13, Ptr_2, 1);
  //ADE:: Persistence :: PartialNode<int> retrived_node = (*root_ptr)[{0,2}]; 
  //std::cout << "Next Value: " << *retrived_node.data_ << std::endl;

  std::cout<< "rooTest forward version: 1 posisicion[0] = " << *(*root_ptr)[{0,1}].data_<< std::endl;  
  std::cout<< "rooTest forward version: 2 posisicion[0] = " << *(*root_ptr)[{0,2}].data_<< std::endl;
  std::cout<< "rooTest forward version: 3 posisicion[0] = " << *(*root_ptr)[{0,3}].data_<< std::endl;
  ADE:: Persistence :: PartialNode<int> retrived_node =(*root_ptr)[{0,2}];
  std::cout<< "next to ptr2 version 5 posisicion[1] = " << *(*root_ptr)[{0,3}][{1,5}].data_<< std::endl; 
  return 0;
}
