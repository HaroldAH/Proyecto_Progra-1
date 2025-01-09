#include "Sale.h"

using namespace std;

void Sale::sell(User &user, Event &event, Segment &segment, map<tuple<int, int>, Seating> &seatingMap, Discount &discount)
{
    if (!checkEventsAvailability(event)) return;

    UserData* currentUser = getOrRegisterUser(user);
    if (!currentUser) return;

    int selectedEvent = chooseEvent(event);
    
    if (selectedEvent < 0) return;

    int selectedSegment = chooseSegment(segment, selectedEvent);
    if (selectedSegment < 0) return;

    Segment** segments = segment.getSegmentsByEvent();
    Seating &seating = ensureSeating(selectedEvent, selectedSegment, segments, seatingMap);

    int numTickets = buyTickets(currentUser, seating);
    if (numTickets <= 0) return;

    vector<pair<int, char>> purchasedSeats(numTickets);

    for (int i = 0; i < numTickets; i++) {
        int row;
        char col;

        cout << "Ingrese la columna del asiento (A-"
             << char('A' + seating.getNumberOfColumns() - 1) << "): ";
        cin >> col;

        cout << "Ingrese la fila del asiento (1-"
             << seating.getNumberOfRows() << "): ";
        cin >> row;

       
        if (row < 1 || row > seating.getNumberOfRows() ||
            toupper(col) < 'A' ||
            toupper(col) >= 'A' + seating.getNumberOfColumns())
        {
            cout << "Asiento fuera de rango. Elija otro.\n";
            i--; 
            continue;
        }

        if (seating.getSeatPurchased()[row - 1][toupper(col) - 'A']) {
            cout << "Asiento ocupado. Elija otro.\n";
            i--;
            continue;
        }

        seating.getSeatPurchased()[row - 1][toupper(col) - 'A'] = true;
        purchasedSeats[i] = make_pair(row, toupper(col));
        cout << "Asiento reservado con exito.\n";
    }

    float discountPercentage = applyDiscountIfWanted(discount);
    float ticketPrice = seating.getCost();
    float totalCost = ticketPrice * numTickets;

    if (discountPercentage > 0) {
        totalCost -= totalCost * (discountPercentage / 100.0f);
    }

    string cardNumber = askCardNumber();
    printInvoice(currentUser, event, selectedEvent, segments, selectedSegment,
                 numTickets, ticketPrice, discountPercentage,
                 totalCost, purchasedSeats, cardNumber);

    cout << "Presione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

bool Sale::checkEventsAvailability(Event &event) {

    if (event.getEventCount() == 0) {
        cout << "No hay eventos disponibles.\n";
        return false;
    }
    return true;
}

UserData* Sale::getOrRegisterUser(User &user) {

    while (true) {

        string idNumber;
        cout << "Ingrese su numero de cedula: ";
        cin >> idNumber;

        UserData* currentUser = user.searchUserById(idNumber);
        if (currentUser) {
            cout << "Usuario encontrado. Bienvenido, "
                 << currentUser->getName() << ".\n";
            return currentUser;
        }

        cout << "Usuario no encontrado. Desea registrarse? (S/N): ";
        char option;
        cin >> option;

        if (tolower(option) != 's') {
            cout << "Regresando al menu principal...\n";
            return nullptr;
        }

        user.createUser(user, idNumber);

        currentUser = user.searchUserById(idNumber);
        if (currentUser) {
            cout << "Usuario registrado."<<endl << "Bienvenido "
                 << currentUser->getName() << "procederemos con la compra:"<<endl;
            return currentUser;
        }

        cout << "Error al registrar el usuario. Intente nuevamente.\n";
    }
}

int Sale::chooseEvent(Event &event) {

    if (event.getEventCount() == 0) {
        cout << "No hay eventos disponibles.\n";
        return -1;
    }

    cout << "Seleccione un evento:\n";
    for (int i = 0; i < event.getEventCount(); i++) {
        cout << i + 1 << ". " << event.getEvents()[i].getName() << "\n";
    }

    int selected = readIntInRange(
        1,                               
        event.getEventCount(),           
        "Entrada invalida. Intente nuevamente: "  
    );

   
    return selected - 1;
}

int Sale::chooseSegment(Segment &segment, int selectedEvent) {

    Segment** segments = segment.getSegmentsByEvent();
    int* segmentCounts = segment.getSegmentCount();

    if (!segments || !segmentCounts) {
        cout << "No hay datos de segmentos\n";
        return -1;
    }

    int numSegments = segmentCounts[selectedEvent];
    if (numSegments <= 0) {
        cout << "No hay segmentos disponibles para este evento.\n";
        return -1;
    }

    cout << "\nSegmentos disponibles:\n";
    for (int i = 0; i < numSegments; i++) {
        cout << i + 1 << ". "
             << segments[selectedEvent][i].getName()
             << " - Precio: $"
             << fixed << setprecision(2)
             << segments[selectedEvent][i].getPrice()
             << "\n";
    }

    
    int chosen = readIntInRange(
        1,
        numSegments,
        "Entrada invalida. Intente nuevamente: "
    );

    return chosen - 1;
}

Seating& Sale::ensureSeating(int selectedEvent, int selectedSegment, Segment** segments, map<tuple<int,int>,Seating> &seatingMap) {
    tuple<int,int> seatingKey = make_tuple(selectedEvent, selectedSegment);

    if (seatingMap.find(seatingKey) == seatingMap.end()) {
        Seating newSeating;
        Segment &seg = segments[selectedEvent][selectedSegment];
        newSeating.setNumberOfRows(seg.getRows());
        newSeating.setNumberOfColumns(seg.getSeats());
        newSeating.setCost(seg.getPrice());
        newSeating.initializeRoom();
        seatingMap[seatingKey] = newSeating;
    }

    Seating &seating = seatingMap[seatingKey];
    seating.displaySeats();
    return seating;
}

int Sale::buyTickets(UserData *currentUser, Seating &seating) {

    int currentTickets = currentUser->getTicketsPurchased();
    if (currentTickets >= 5) {
        cout << "Ya has comprado el numero maximo de 5 boletos.\n";
        return 0;
    }
    
    while (true) {
        int maxTickets = 5 - currentTickets;
        cout << "\nCuantos boletos desea comprar? (max " << maxTickets << "): ";

        
        int numTickets = readIntInRange(
            1,
            maxTickets,
            "Entrada invalida. Por favor, ingrese un numero valido: "
        );

        
        if (currentUser->purchaseTickets(numTickets)) {
            return numTickets;
        }
        cout << "No se pudo completar la compra de boletos. Intente nuevamente.\n";
    }
}

float Sale::applyDiscountIfWanted(Discount &discount) {
    cout << "\nTiene un codigo de descuento? (S/N): ";
    char useDiscount;
    cin >> useDiscount;

    if (tolower(useDiscount) != 's') {
        return 0.0f; 
    }

    cout << "Ingrese su codigo de descuento: ";
    cin.ignore();
    string discountCode;
    getline(cin, discountCode);

    if (discount.verifyCode(discountCode)) {
        float discountPercentage = discount.getDiscountPercentage();
        cout << "Descuento aplicado: " << discountPercentage << "%\n";
        return discountPercentage; 
    }

    cout << "Codigo de descuento invalido o ya utilizado.\n";
    return 0.0f; 
}

string Sale::askCardNumber() {
    while (true) {
        cout << "\nIngrese el numero de su tarjeta: ";
        string cardNumber;
        cin >> cardNumber;
        if (cardNumber.length() >= 12 && 
            cardNumber.find_first_not_of("0123456789") == string::npos)
        {
            return cardNumber;
        }
        cout << "Numero de tarjeta invalido. Ingrese solo numeros con al menos 12 digitos.\n";
    }
}

void Sale::printInvoice(UserData* currentUser,Event &event, int selectedEvent, Segment** segments,int selectedSegment,int numTickets,float ticketPrice,
 float discountPercentage, float totalCost, vector<pair<int, char>> &purchasedSeats, string &cardNumber)
{
   
    cout << "\n\n==================== FACTURA ====================\n";
    cout << "Usuario: " << currentUser->getName() << endl;
    cout << "Cedula: " << currentUser->getIdNumber() << endl;
    cout << "Evento: " << event.getEvents()[selectedEvent].getName() << endl;
    cout << "Segmento: " << segments[selectedEvent][selectedSegment].getName() << endl;
    cout << "Tarjeta: ****-****-****-" 
         << cardNumber.substr(cardNumber.length() - 4) << endl;
    cout << "Boletos: " << numTickets << " x $"
         << fixed << setprecision(2) << ticketPrice << endl;
    if (discountPercentage > 0) {
        cout << "Descuento: " << discountPercentage << "%\n";
    }
    cout << "Asientos comprados: ";
    for (int i = 0; i < (int)purchasedSeats.size(); i++) {
        cout << "(Fila: " << purchasedSeats[i].first
             << ", Columna: " << purchasedSeats[i].second << ") ";
    }
    cout << "\n";
    cout << "Total pagado: $" << fixed << setprecision(2) << totalCost << "\n";
    cout << "=================================================\n";
}

int Sale::readIntInRange(int minValue, int maxValue, const std::string &errorPrompt)
{
    int value;
    while (true) {
        cin >> value;
        
        if (!cin.fail() && value >= minValue && value <= maxValue) {
           
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << errorPrompt;
    }
}
