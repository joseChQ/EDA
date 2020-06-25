// dtsmy.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <utility>

const unsigned int kNumberPhilosophers = 5;
const unsigned int kNumberActions = 1000;

int get_random(int low, int high) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(low, high);
    return distribution(gen);
}

class DiningPhilosophers;

struct Philosopher {
    Philosopher(unsigned int id, std::string name, unsigned int number_actions,
        DiningPhilosophers* table)
        : id_(id), name_(name), number_actions_(number_actions), table_(table) {}

    void operator()() {
        for (unsigned int i = 0; i < number_actions_; ++i) {
            Eat();
            // if (get_random(1, 2) < 2) {
            //   Think();
            // } else {
            //   Eat();
            // }
        }
    }

    void Think();
    void Eat();

    unsigned int id_;
    std::string name_;
    unsigned int number_actions_;
    DiningPhilosophers* table_;
};

class DiningPhilosophers {
public:
    DiningPhilosophers() {
        std::thread* philosophers[kNumberPhilosophers];
        std::string philosopher_names[] = {
            "Socrates", "Plato", "Aristotle", "Descartes", "Tomas",
        };
        for (unsigned int i = 0; i < kNumberPhilosophers; ++i) {
            philosophers[i] = new std::thread(
                Philosopher(i, philosopher_names[i], kNumberActions, this));
        }
        for (unsigned int i = 0; i < kNumberPhilosophers; ++i) {
            philosophers[i]->join();
        }
    }

    void PickFork(unsigned int id) {
        int idx = std::max(id, (id + 1) % kNumberPhilosophers);
        int idy = std::min(id, (id + 1) % kNumberPhilosophers);
        forks_[idx].lock();
        forks_[idy].lock();
    }

    void LeaveFork(unsigned int id) {
        forks_[id].unlock();
        forks_[(id + 1) % kNumberPhilosophers].unlock();
    }

private:
    std::mutex forks_[kNumberPhilosophers];
};

void Philosopher::Think() {
    std::cout << name_ + " is Thinking." << std::endl;
}

void Philosopher::Eat() {
    table_->PickFork(id_);
    std::cout << name_ + " is Eating." << std::endl;
    table_->LeaveFork(id_);
}
int main() {
    DiningPhilosophers table;
    return 0;
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
