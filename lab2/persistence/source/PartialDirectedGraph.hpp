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
        //Table = std::vector<std:: pair<int,Node<data_type> > >
      //  (in_ptrs_size*2, std :: pair<int,Node<data_type> >( 0 ,Node<data_type>(data,out_ptrs_size)));
          VV = std::vector<int>(in_ptrs_size*2,0);
          for(int i = 0 ;i<in_ptrs_size*2;++i)
          {
            Table.emplace_back(data,out_ptrs_size);
          }
        }

//    data_type get_data() { return *PointerMachine::Node<Type>::data_; }

  data_type get_data(unsigned int version) { return *Node<Type>::data_; }
  //tabla de modificaciones
  std:: vector<Node<data_type> > Table;
  std:: vector<int> VV;//versiones
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
    int i = 0 ;
    while(i<c_M && VV[i]<=id_version.second)i++;
    i--;
    //std::cout<<" version " <<std::endl;
   // for(int i = 0 ;i<c_M;i++)std::cout<< Table[i].first <<std::endl;

    //std::cout<<c_M<<" "<<i<<std::endl;
    /*return *dynamic_cast<PartialNode*>(
        &(Node<Type>::operator[](id_version.first)));*/
    return *dynamic_cast<PartialNode*>(Table[i].forward_[id_version.first]);
   // return *Table[i].second.forward_[id_version.first];
  }
};

template <typename Type, typename Node = PartialNode<Type>>
class PartialDirectedGraph : public DirectedGraph<Type, Node> {
 public:
  typedef Type data_type;

  PartialDirectedGraph(data_type const data, std::size_t const& out_ptrs_size,
                       std::size_t const& in_ptrs_size)
      : DirectedGraph<Type, Node>(data, out_ptrs_size,in_ptrs_size),
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
  Node* insert_V(data_type const data, Node* u, std::size_t position, unsigned int version)
  {
   // Node* next_node_ptr = dynamic_cast<Node*>(u->forward_[position]);
    Node* new_node = new Node(data, DirectedGraph<Type, Node>::out_ptrs_size_,in_ptrs_size_);
    u->VV[u->c_M] = version;
    if(u->c_M>0)
        u->Table[(u->c_M)] = u->Table[(u->c_M)-1];
    //std::cout<<u->c_M<<std::endl;
    new_node->Back_ptr[position] = u;    
    //Node ** tmp =&(u->Table[u->c_M].second.forward_[position]);
    u->Table[(u->c_M)].forward_[position] = new_node;
     (u->c_M)++; 
    //return dynamic_cast<Node*>(u->Table[u->c_M].se->forward_[position]);
    return dynamic_cast<Node*>(u->Table[u->c_M-1].forward_[position]);
    //return new_node;  
  }
  Node* insert_vertex(data_type const data, Node* u, std::size_t position, unsigned int version) {
    if(u->c_M < in_ptrs_size_*2)
    {
      return insert_V(data, u, position, version);
    } 
    else
    {
      Node* new_u = new Node(*(u->data_), DirectedGraph<Type, Node>::out_ptrs_size_,in_ptrs_size_);
      for(int i = 0;i<int(in_ptrs_size_);++i)
      {
        if(u->Back_ptr[i]!=nullptr)
        {
          add_edge(u->Back_ptr[i],new_u,i);
        }
      }
      return insert_V(data,new_u, position,version);
    }         
  }

  void add_edge(Node* u, Node* v, std::size_t position, unsigned int version) {
    if( (u->c_M)< in_ptrs_size_*2)
    {
      u->VV[u->c_M] = version;
      u->Table[u->c_M].forward_[position] = v;
      v->Back_ptr[position] = u;
      (u->c_M)++; 
    }
    else
    {
      Node* new_u = new Node(*(u->data_), DirectedGraph<Type, Node>::out_ptrs_size_,in_ptrs_size_); 
      for(int i = 0;i<int(in_ptrs_size_);++i)
      {
        if(u->Back_ptr[i]!=nullptr)
        {
          add_edge(u->Back_ptr[i],new_u,i);
        }
      }
      new_u->VV[u->c_M] = version;
      new_u->Table[u->c_M].forward_[position] = v;
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