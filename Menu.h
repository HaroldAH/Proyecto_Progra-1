// Menu.h

#include "User.h"
#include "Event.h"
#include "Segment.h"
#include "discount.h" // Asegúrate de incluir todos los managers necesarios
#include "Seating.h"
#include <map>
#include <tuple>
#include <string>

// Declaración anticipada si Seating está definido en otro archivo
class Seating;

class Menu {
public:
    void showMenu();
    void venta();
    void showAbout();
    void configureDiscounts();
    void showEventMenu(Event &event);
    void listEventAndSegments(Event &event, Segment &segment);
    void sellTicket();
    void crearUsuario();
    void consultarVentas();
    User* buscarUsuarioPorCedula(const std::string &cedula);

private:
    // Declaración del método validateChoice con un solo parámetro
    int validateChoice(int maxOption);

    // Mapa para almacenar Seating por clave única
    std::map<std::tuple<int, int>, Seating> seatingMap;

    // Otros miembros privados
    Event event;
    Segment segment;
    Discount discountManager;
    User* usuarios = nullptr; // Inicialización adecuada
    int numUsuarios = 0;
    int capacidadUsuarios = 0;
};