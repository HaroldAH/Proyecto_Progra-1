

#include <iostream>
#include <string>

using namespace std;

struct CodigoDescuento {
    string codigo;
    bool usado;
};

class Descuento {
private:
    float porcentajeDescuento;       
    int cantidadDescuentos;          
    CodigoDescuento* codigos;        

   
    string generarCodigo();

public:
    
    Descuento(float porcentaje, int cantidad);

   
    ~Descuento();

    
    bool verificarCodigo(const string& codigo);

    
    float obtenerPorcentajeDescuento() const;

    
    void mostrarCodigos() const;
};


