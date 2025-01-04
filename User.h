#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <vector>

#include <string>

#include <string>

class User {
private:
    std::string cedula;
    std::string nombre;
    std::string fechaNacimiento;
    int cantidadEntradasCompradas;

public:
    // Constructores
    User(std::string cedula, std::string nombre, std::string fechaNacimiento, int cantidadEntradasCompradas);
    User();

    // Getters
    std::string getCedula() const;
    std::string getNombre() const;
    std::string getFechaNacimiento() const;
    int getCantidadEntradasCompradas() const;

    // Setters
    void setCedula(const std::string& cedula);
    void setNombre(const std::string& nombre);
    void setFechaNacimiento(const std::string& fechaNacimiento);
    void setCantidadEntradasCompradas(int cantidad);

    // Métodos adicionales
    void obtenerDatos() const;
    bool comprarEntradas(int cantidad); // Retorna true si la compra fue exitosa
};




