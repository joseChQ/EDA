// van.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <cassert>
#include <exception>
#include <iostream>
#include <memory>
#include <vector>
#include "DirectedGraph.h"
#include <map>
#include <queue>
#include <vector>
#include <set>
#include <utility>
int ix = 1;
int iy = 1;
std::map<int, int> rec;
std::map<int, int> rec1;
std::map<int, int> rec2;
void dfs(ADE::Node<int>* root_ptr)
{
    rec[ *root_ptr->data_] = ix;
    
    for (int i = 0; i < 2; ++i)
    {
       // ADE::Node<int>* tmp = &(*root_ptr)[i];
        ADE::Node<int>* tmp;
        tmp = &(*root_ptr->forward_[i]);
        if (tmp != nullptr){
            ix++;
            dfs(tmp);
        }
    }
}
void bfs(ADE::Node<int>* root_ptr)
{
    //rec1[*root_ptr->data_] = iy;
   // std::map<ADE::Node<int>*, int>d;
    std::queue<ADE::Node<int>*> q;
    rec1[*root_ptr->data_] = iy++;
    q.push(root_ptr);
    while (!q.empty())
    {
        ADE::Node<int>* tmp = q.front();
        q.pop();
        for (int i = 0; i < 2; ++i)
        {
            ADE::Node<int>* tmp2;
            tmp2 = &(*tmp->forward_[i]);
            if (tmp2 != nullptr)
            {
                rec1[*tmp2->data_] = iy++;
                q.push(tmp2);
            }
        }
    }
}
std::set<ADE::Node<int>* > black_list;
std::priority_queue<std::pair<int, ADE::Node<int>* >, std::vector<std::pair<int, ADE::Node<int>*>>> qp;
int bfs1(ADE::Node<int>* root_ptr, std::map<int,std::vector<ADE::Node<int>*>> &lv)
{
    //rec1[*root_ptr->data_] = iy;
    int datos = 1;
    std::map<ADE::Node<int>*, int>d;
    std::queue<ADE::Node<int>*> q;
  //  lv[*root_ptr->data_] = iy++;
    lv[1].push_back(root_ptr);
    q.push(root_ptr);
    d[root_ptr] = 1;
    while (!q.empty())
    {
        ADE::Node<int>* tmp = q.front();
        q.pop();
        for (int i = 0; i < 2; ++i)
        {
            ADE::Node<int>* tmp2;
            tmp2 = &(*tmp->forward_[i]);
            if (tmp2 != nullptr && black_list.find(tmp2)==black_list.end())
            {
                d[tmp2] = d[tmp] + 1;
                lv[d[tmp2]].push_back(tmp2);
                q.push(tmp2);
                datos++;
            }
        }
    }
    return datos;
}
int lg2(int x)
{
    int ans = 0;
    while (x)
    {
        ans++; 
        x >>= 1;
    }
    return ans;
}
int time = 0;
int INF = 10000;
int iz = 1;
void van(ADE::Node<int>* root_ptr)
{
    qp.emplace(time, root_ptr);
    time++;
    while (!qp.empty())
    {
        auto d = qp.top();
        qp.pop();
        std::map<int, std::vector<ADE::Node<int>*>> lv;
        int cnumber = bfs1(d.second, lv);
        cnumber = lg2(cnumber);
        
        while (cnumber>>1) {
            int _size = lv[(cnumber >> 1)].size();
            for (int k = _size-1 ;k>=0;--k) 
            {
                auto u = lv[(cnumber >> 1)][k];
                for (int i = 1; i >= 0; --i)
                {
                    ADE::Node<int>* tmp2;
                    tmp2 = &(*u->forward_[i]);
                    if (tmp2 != nullptr)
                    {
                        black_list.insert(tmp2);
                        qp.emplace(time,tmp2);
                        time++;
                    }
                }
            }
            cnumber >>= 1;
        }
       // std::cout << *d.second->data_ <<" "<<d.first<< std::endl;
        rec2[*d.second->data_] = iz++;
    }
}


bool _find(ADE::Node<int>* root_ptr,int key,int bloque, std::map<int, int> re,std::set<int>&A)
{
    //rec[]
    int bloq = re[*root_ptr->data_]/bloque + (re[*root_ptr->data_]%bloque?1:0);
    A.insert(bloq);
    ADE::Node<int>** p;
    for (p = &root_ptr; *p && *(*p)->data_ != key; p = &((*p)->forward_[*(*p)->data_ < key]))
    {
        bloq = re[(*(*p)->data_)]/bloque + (re[(*(*p)->data_)] % bloque ? 1 : 0);
        A.insert(bloq);
    }
    if (*p)
    {
        bloq = re[(*(*p)->data_)] / bloque + (re[(*(*p)->data_)] % bloque ? 1 : 0);
        A.insert(bloq);
    }
    return *(*p)->data_ == key;
}
int main()
{
    ADE::DirectedGraph<int> my_graph(2, 2, 2);
    //We can obtain the root and print their value.
    ADE::Node<int>* root_ptr = my_graph.get_root_ptr();
    ADE::Node<int>* p1 = my_graph.insert_vertex(-5, root_ptr, 0);
    ADE::Node<int>* p2 = my_graph.insert_vertex(8, root_ptr, 1);
    ADE::Node<int>* p3 = my_graph.insert_vertex(6, p2, 0);
    ADE::Node<int>* p4 = my_graph.insert_vertex(11, p2, 1);
    ADE::Node<int>* p5 = my_graph.insert_vertex(5, p3, 0);
    ADE::Node<int>* p6 = my_graph.insert_vertex(7, p3, 1);
    ADE::Node<int>* p7 = my_graph.insert_vertex(10, p4, 0);
    ADE::Node<int>* p8 = my_graph.insert_vertex(12, p4, 1);
    ADE::Node<int>* p9 = my_graph.insert_vertex(-7, p1, 0);
    ADE::Node<int>* p10 = my_graph.insert_vertex(-1, p1, 1);
    ADE::Node<int>* p11 = my_graph.insert_vertex(-2, p10, 0);
    ADE::Node<int>* p12 = my_graph.insert_vertex(0, p10, 1);
    ADE::Node<int>* p13 = my_graph.insert_vertex(-8, p9, 0);
    ADE::Node<int>* p14 = my_graph.insert_vertex(-6, p9, 1);
    dfs(root_ptr);
    bfs(root_ptr);
    van(root_ptr);
    // for (auto i : rec2) std::cout << i.first << " " << i.second << std::endl;
    int key = 0;
    int bloque = 3;
    std::set<int> A, B, C;
    //rec dfs,A
    //rec1 bfs,B
    //rec2 van,C
    _find(root_ptr, key, bloque, rec, A);//dfs
    _find(root_ptr, key, bloque, rec1, B);//dfs
    _find(root_ptr, key, bloque, rec2, C);//dfs
    std::cout << "DFS: " << A.size() << std::endl;
    std::cout << "BFS: " << B.size() << std::endl;
    std::cout << "VAN: " << C.size() << std::endl;
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
