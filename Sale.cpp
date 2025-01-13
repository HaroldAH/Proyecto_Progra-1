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

    int numTickets = buyTickets(currentUser, event, selectedEvent);
    if (numTickets <= 0) return;

    int* purchasedRows = new int[numTickets];  
    char* purchasedCols = new char[numTickets];  

    for (int i = 0; i < numTickets; i++) {
        int row = -1;
        char col = '\0';

        while (true) {
            cout << "Ingrese la columna del asiento (A-"
                 << char('A' + seating.getNumberOfColumns() - 1) << "): ";
            string buffer;
            cin >> buffer;

            if (buffer.size() == 1) {
                col = toupper(buffer[0]);
                if (col >= 'A' && col < char('A' + seating.getNumberOfColumns())) {
                    break;
                }
                cout << "Columna fuera de rango. Intente de nuevo.\n";
                continue;
            }

            cout << "Error: ingrese SOLO una letra (A-"
                 << char('A' + seating.getNumberOfColumns() - 1) << ").\n";
        }

        while (true) {
            cout << "Ingrese la fila del asiento (1-"
                 << seating.getNumberOfRows() << "): ";

            if (cin >> row) {
                if (row >= 1 && row <= seating.getNumberOfRows()) {
                    break;
                }
                cout << "Fila fuera de rango. Intente de nuevo.\n";
                continue;
            }

            cout << "Entrada invalida. Por favor ingrese un numero.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (seating.getSeatPurchased()[row - 1][col - 'A']) {
            cout << "Asiento ocupado. Elija otro.\n";
            i--;
            continue;
        }

        seating.getSeatPurchased()[row - 1][col - 'A'] = true;
        purchasedRows[i] = row;
        purchasedCols[i] = col;

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
                 totalCost, purchasedRows, purchasedCols, numTickets, cardNumber);

    cout << "Presione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    delete[] purchasedRows;  
    delete[] purchasedCols;  
}

bool Sale::checkEventsAvailability(Event &event) {

    if (event.getEventCount() == 0) {
        cout << "No hay eventos disponibles.\n";
        cout << "\nPresione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();  
        return false;
    }
    return true;
}

UserData* Sale::getOrRegisterUser(User &user) {
    while (true) {
        string idNumber;
        cout << "Ingrese su numero de cedula (9 digitos): ";
        cin >> idNumber;

        if (idNumber.length() != 9 || idNumber.find_first_not_of("0123456789") != string::npos) {
            cout << "Cedula invalida. Debe contener exactamente 9 digitos numericos.\n";
            continue;
        }

        UserData* currentUser = user.searchUserById(idNumber);
        if (currentUser) {
            cout << "Usuario encontrado. Bienvenido, " << currentUser->getName() << ".\n";
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
            cout << endl << "Usuario registrado." << endl
                 << "Bienvenido " << currentUser->getName()
                 << ", procederemos con la compra:" << endl;
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

Seating& Sale::ensureSeating(int selectedEvent, int selectedSegment, Segment** segments, 
map<tuple<int,int>,Seating> &seatingMap) {
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

int Sale::buyTickets(UserData *currentUser, Event &event, int selectedEvent) {

    string userId = currentUser->getIdNumber();
    int currentTickets = event.getEvents()[selectedEvent].getTicketsPurchasedByUser(userId);

    if (currentTickets >= 5) {
        cout <<"Ya has comprado el numero maximo de 5 boletos para este evento.\n";
        cout <<"\nPresione Enter para continuar...";
        cin.get();
        return 0;
    }

    while (true) {
        int maxTickets = 5 - currentTickets;
        cout <<"\n ¿Cuantos boletos desea comprar? (max " << maxTickets << "): ";

        int numTickets = readIntInRange( 1, maxTickets,
            "Entrada invalida. Por favor, ingrese un numero valido: "
        );

        if (event.getEvents()[selectedEvent].purchaseTickets(userId, numTickets)) {
            return numTickets;
        }
        cout << "No se pudo completar la compra de boletos. Intente nuevamente.\n";
    }
}

float Sale::applyDiscountIfWanted(Discount &discount) {

    char useDiscount;

    cout << "\nTiene un codigo de descuento? (S/N): ";
    cin >> useDiscount;
    cin.ignore(); 
    useDiscount = tolower(useDiscount);

    if (useDiscount != 's') {
        return 0.0f;
    }

    while (true) {

        string discountCode;
        cout << "Ingrese su codigo de descuento: ";
        getline(cin, discountCode);

        if (discount.verifyCode(discountCode)) {
            float discountPercentage = discount.getDiscountPercentage();
            cout << "Descuento aplicado: " << discountPercentage << "%\n";
            return discountPercentage;
        }

        cout << "Codigo de descuento invalido o ya utilizado.\n";

        while (true) {
            cout << "Desea intentarlo nuevamente? (S/N): ";
            cin >> useDiscount;
            cin.ignore();
            useDiscount = tolower(useDiscount);

            if (useDiscount == 's' || useDiscount == 'n') {
                break; 
            }
            cout << "Opcion no valida. Por favor ingrese S o N.\n";
        }

        if (useDiscount == 'n') {
            break;
        }
    }

    cout << "No se aplico ningun descuento.\n";
    return 0.0f;
}

string Sale::askCardNumber() {
    string cardNumber;

    while (true) {
        cout << "\nIngrese el numero de su tarjeta (12 digitos): ";
        cin >> cardNumber;

        if (cardNumber.length() == 12 && 
            cardNumber.find_first_not_of("0123456789") == string::npos) {
            cout << "Numero de tarjeta valido.\n";
            break;
        }

        cout << "Numero de tarjeta invalido. Por favor, ingrese exactamente 12 digitos numericos.\n";
    }

    return cardNumber;
}


void Sale::printInvoice(UserData* currentUser, Event &event, int selectedEvent, Segment** segments, int selectedSegment,
                        int numTickets, float ticketPrice, float discountPercentage, float totalCost,
                        int* purchasedRows, char* purchasedCols, int numPurchasedSeats, std::string cardNumber) {
    cout << "\n\n==================== FACTURA ====================\n\n";
    cout << "Usuario: " << currentUser->getName() << endl;
    cout << "Cedula: " << currentUser->getIdNumber() << endl;
    cout << "Evento: " << event.getEvents()[selectedEvent].getName() << endl;
    cout << "Segmento: " << segments[selectedEvent][selectedSegment].getName() << endl;
    cout << "Tarjeta: ****-****-****-" << cardNumber.substr(cardNumber.length() - 4) << endl;
    cout << "Boletos: " << numTickets << " x $" << fixed << setprecision(2) << ticketPrice << endl;
    if (discountPercentage > 0) {
        cout << "Descuento: " << discountPercentage << "%\n";
    }
    cout << "Asientos comprados: ";
    for (int i = 0; i < numPurchasedSeats; i++) {
        cout << "(Fila: " << purchasedRows[i]
             << ", Columna: " << purchasedCols[i] << ") ";
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
