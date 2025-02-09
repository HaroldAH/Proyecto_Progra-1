#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "List.h"
#include "Segment.h"

class Event {
private:
    std::string name;
    std::string date;
    std::string description;
    int eventCount;             // Cantidad total de eventos creados
    List<Event> events;         // Lista que almacena los eventos
    List<int> purchasesByUser;  // Para el control de compras (cantidad de boletos por usuario)
    List<std::string> userIds;  // Para el control de usuarios (ID de cada usuario)

public:
    Event();
    ~Event();

    // Getters / Setters
    std::string getName();
    void setName(std::string& aName);

    std::string getDate();
    void setDate(std::string& aDate);

    std::string getDescription();
    void setDescription(std::string& aDescription);

    List<Event>& getEvents();
    int getEventCount();

    // Métodos para inicializar / seguimiento / venta
    void initializeTracking(int maxUsers);

    // Función de compra de boletos: ahora recibe la ventana SFML para mostrar mensajes de error
    bool purchaseTickets(const std::string& userId, int numTickets, sf::RenderWindow& window);

    // Retorna la cantidad de boletos que ha comprado el usuario para este evento
    int getTicketsPurchasedByUser(const std::string& userId);

    // Función de cancelación de boletos: utiliza la ventana SFML para mostrar mensajes
    bool cancelTickets(std::string userId, int numTickets, sf::RenderWindow& window);

    // Manejo de creación y asignación de eventos
    void assignEvents(Event& event, int numEvents);
    int getValidIntInput();
    std::string getValidStringInput(const std::string& prompt);
    bool isValidDate(std::string& date);

    // Método que recibe una ventana SFML para capturar los datos (nombre, fecha, descripción)
    // del nuevo evento y lo guarda (además de llamar a segment.saveSegments(...))
    void saveEvent(sf::RenderWindow& win, Segment& segment);
};

#endif // EVENT_H
