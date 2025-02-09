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
    List<int> purchasesByUser;  // (Stub) para control de compras
    List<std::string> userIds;  // (Stub) para control de usuarios
    
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
    bool purchaseTickets(const std::string& userId, int numTickets);
    int getTicketsPurchasedByUser(const std::string& userId);
    bool cancelTickets(string userId, int numTickets, sf::RenderWindow& window);

    // Manejo de creación y asignación de eventos
    void assignEvents(Event& event, int numEvents);
    int getValidIntInput();
    std::string getValidStringInput(const std::string& prompt);
    bool isValidDate(std::string& date);

    // * Método que recibe una ventana SFML para capturar datos del nuevo evento *
    void saveEvent(sf::RenderWindow& win, Segment& segment);
};

#endif // EVENT_H