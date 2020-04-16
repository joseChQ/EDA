// Copyright 2020 Roger Peralta Aranibar
#ifndef SOURCE_PARTIALDIRECTEDGRAPH_HPP_
#define SOURCE_PARTIALDIRECTEDGRAPH_HPP_

#include <utility>

#include "DirectedGraph.hpp"
#include <vector>
#include <utility>
namespace ADE {
namespace Persistence {

template <typename Type>
class PartialNode : public Node<Type> {
 public:
  typedef Type data_type;
  PartialNode(data_type data, std::size_t const& out_ptrs_size,
              std::size_t const& in_ptrs_size)
      : Node<Type>(data, out_ptrs_size),
        c_M(0),
        Back_ptr(new PartialNode<Type>*[in_ptrs_size]())
        {
        Table = std::vector<std:: pair<int,Node<data_type> > >(in_ptrs_size*2);
        }

//    data_type get_data() { return *PointerMachine::Node<Type>::data_; }

  data_type get_data(unsigned int version) { return *Node<Type>::data_; }
  //tabla de modificaciones
  std:: vector< std::pair<int,Node<data_type> > > Table;
  int c_M;//me indica c_M(cantidad de modificaciones) en el nodo
  PartialNode ** Back_ptr;
  bool set_data(data_type const data) {
    *Node<Type>::data_ = data;
    return true;
  }

  bool set_ptr(PartialNode* ptr, unsigned int id) {
    Node<Type>::forward_ = ptr;
    return true;
  }

  PartialNode& operator[](std::size_t id) const {
    return *dynamic_cast<PartialNode*>(&(Node<Type>::operator[](id)));
  }

  PartialNode& operator[](
      std::pair<std::size_t, unsigned int> id_version) const {
    return *dynamic_cast<PartialNode*>(
        &(Node<Type>::operator[](id_version.first)));
  }
};

template <typename Type, typename Node = PartialNode<Type>>
class PartialDirectedGraph : public DirectedGraph<Type, Node> {
 public:
  typedef Type data_type;

  PartialDirectedGraph(data_type const data, std::size_t const& out_ptrs_size,
                       std::size_t const& in_ptrs_size)
      : DirectedGraph<Type, Node>(data, out_ptrs_size),
        in_ptrs_size_(in_ptrs_size),
        current_version(0) {}

  Node* get_root_ptr() {
    return DirectedGraph<Type, Node>::get_root_ptr();
  }

  Node* get_root_ptr(unsigned int version) {
    return DirectedGraph<Type, Node>::get_root_ptr();
  }

  Node* insert_vertex(data_type const data, Node* u, std::size_t position) {
    ++current_version;
    return insert_vertex(data, u, position, current_version);
  }

  void add_edge(Node* u, Node* v, std::size_t position) {
    ++current_version;
    return add_edge(u, v, position, current_version);
  }

 private:
  Node* insert_V(data_type const data, Node* u, std::size_t position,
                      unsigned int version)
  {
   // Node* next_node_ptr = dynamic_cast<Node*>(u->forward_[position]);
    Node* new_node = new Node(data, DirectedGraph<Type, Node>::out_ptrs_size_,in_ptrs_size_);
    u->Table[u->c_M].fi = version;
    if((u->c_M)>1)
        u->Table[u->c_M].se = u->Table[(u->c_M-1)].se;    
    u->Table[u->c_M].se->forward_[position] = new_node;
    new_node->Back_ptr[position] = u;
    (u->c_M)++;
    return dynamic_cast<Node*>(u->Table[u->c_M].se->forward_[position]);  
  }
  Node* insert_vertex(data_type const data, Node* u, std::size_t position,
                      unsigned int version) {
    if((u->c_M) < in_ptrs_size_)
    {
      return insert_V(data, u, position, current_version);
    } 
    else
    {
      Node* new_u = new Node(*(u->data), DirectedGraph<Type, Node>::out_ptrs_size_,in_ptrs_size_);
      for(int i = 0;i<int(in_ptrs_size_);++i)
      {
        if(u->Back_ptr[i]!=nullptr)
        {
          add_edge(u->Back_ptr[i],new_u,i);
        }
      }
      return insert_V(data,new_u, position);
    }         
  }

  void add_edge(Node* u, Node* v, std::size_t position, unsigned int version) {
    if( (u->c_M)< in_ptrs_size_)
    {
      u->Table[u->c_M].fi = version;
      u->Table[u->c_M].se = v;
      v->Back_ptr[version] = u;
      (u->c_M)++; 
    }
    else
    {
      Node* new_u = new Node(*(u->data), DirectedGraph<Type, Node>::out_ptrs_size_,in_ptrs_size_); 
      for(int i = 0;i<int(in_ptrs_size_);++i)
      {
        if(u->Back_ptr[i]!=nullptr)
        {
          add_edge(u->Back_ptr[i],new_u,i);
        }
      }
      new_u->Table[u->c_M].fi = version;
      new_u->Table[u->c_M].se = v;
      v->Back_ptr[version] = new_u; 
      (new_u->c_M)++;
    }
   // return DirectedGraph<Type, Node>::add_edge(u, v, position);
  }

  std::size_t in_ptrs_size_;
  unsigned int current_version;
};

}  // namespace Persistence
}  // namespace ADE

#endif  // SOURCE_PARTIALDIRECTEDGRAPH_HPP_