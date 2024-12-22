
#include <iostream>
#include <string>

using namespace std;

class User {
private:
    string cedula;              
    string nombre;                   
    string fechaNacimiento;          
    int cantidadEntradasCompradas;  

public:
    
    User(string cedula, string nombre, string fechaNacimiento, int cantidadEntradasCompradas);

   
    void comprarEntradas(int cantidad);

   
    void obtenerDatos();
};


