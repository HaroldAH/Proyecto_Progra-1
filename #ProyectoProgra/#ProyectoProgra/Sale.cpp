#include "Sale.h"

using namespace std;

void Sale::sell(User& user, Event& event, Segment& segment,
    map<tuple<int, int>, Seating>& seatingMap, Discount& discount)
{

    if (!checkEventsAvailability(event))
        return;


    UserData* currentUser = getOrRegisterUser(user);
    if (!currentUser)
        return;


    int selectedEvent = chooseEvent(event);
    if (selectedEvent < 0)
        return;


    int selectedSegment = chooseSegment(segment, selectedEvent);


    List<List<Segment>>& segments = segment.getSegmentsByEvent();
    List<int>& segmentCounts = segment.getSegmentCount();
    int numSegments = segmentCounts.getAt(selectedEvent);


    Seating* seatingPtr = nullptr;
    while (true)
    {
        Seating& seating = ensureSeating(selectedEvent, selectedSegment, segments, seatingMap);

        int availableSeats = 0;
        for (int i = 0; i < seating.getNumberOfRows(); i++)
        {
            for (int j = 0; j < seating.getNumberOfColumns(); j++)
            {
                if (!seating.getSeatPurchased()[i][j])
                {
                    availableSeats++;
                }
            }
        }

        if (availableSeats > 0)
        {
            seatingPtr = &seating;
            break;
        }


        bool alternativeAvailable = false;
        if (numSegments > 1)
        {
            for (int seg = 1; seg <= numSegments; seg++)
            {
                if (seg == selectedSegment)
                    continue;

                Seating& otherSeating = ensureSeating(selectedEvent, seg, segments, seatingMap);
                int otherAvailable = 0;
                for (int r = 0; r < otherSeating.getNumberOfRows(); r++)
                {
                    for (int c = 0; c < otherSeating.getNumberOfColumns(); c++)
                    {
                        if (!otherSeating.getSeatPurchased()[r][c])
                            otherAvailable++;
                    }
                }
                if (otherAvailable > 0)
                {
                    alternativeAvailable = true;
                    break;
                }
            }
        }


        if (!alternativeAvailable)
        {
            cout << "El segmento seleccionado esta lleno. No se pueden comprar mas boletos." << endl;
            cout << "Presione Enter para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            return;
        }


        cout << "Ese segmento esta lleno. Seleccione otro segmento." << endl;
        selectedSegment = chooseSegment(segment, selectedEvent);
    }


    Seating& seating = *seatingPtr;


    int numTickets = buyTickets(currentUser, event, selectedEvent);
    if (numTickets <= 0)
        return;


    int* purchasedRows = new int[numTickets];
    char* purchasedCols = new char[numTickets];


    for (int i = 0; i < numTickets; i++)
    {
        int row = -1;
        char col = '\0';


        while (true)
        {
            cout << "Ingrese la columna del asiento (A-"
                << char('A' + seating.getNumberOfColumns() - 1) << "): ";
            string buffer;
            cin >> buffer;

            if (buffer.size() == 1)
            {
                col = toupper(buffer[0]);
                if (col >= 'A' && col < char('A' + seating.getNumberOfColumns()))
                    break;
                cout << "Columna fuera de rango. Intente de nuevo.\n";
                continue;
            }
            cout << "Error: ingrese SOLO una letra (A-"
                << char('A' + seating.getNumberOfColumns() - 1) << ").\n";
        }


        while (true)
        {
            cout << "Ingrese la fila del asiento (1-" << seating.getNumberOfRows() << "): ";
            if (cin >> row)
            {
                if (row >= 1 && row <= seating.getNumberOfRows())
                    break;
                cout << "Fila fuera de rango. Intente de nuevo.\n";
                continue;
            }
            cout << "Entrada invalida. Por favor ingrese un numero.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        int rowIndex = row - 1;
        int colIndex = col - 'A';


        if (seating.getSeatPurchased()[rowIndex][colIndex])
        {
            cout << "Asiento ocupado. Elija otro.\n";
            i--;
            continue;
        }


        seating.getSeatPurchased()[rowIndex][colIndex] = true;
        purchasedRows[i] = row;
        purchasedCols[i] = col;

        cout << "Asiento reservado con exito.\n";
    }


    float discountPercentage = applyDiscountIfWanted(discount);
    float ticketPrice = seating.getCost();
    float totalCost = ticketPrice * numTickets;
    if (discountPercentage > 0)
    {
        totalCost -= totalCost * (discountPercentage / 100.0f);
    }


    string cardNumber = askCardNumber();
    printInvoice(currentUser, event, selectedEvent, segments, selectedSegment,
        numTickets, ticketPrice, discountPercentage, totalCost,
        purchasedRows, purchasedCols, numTickets, cardNumber);

    cout << "Presione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    delete[] purchasedRows;
    delete[] purchasedCols;
}


bool Sale::checkEventsAvailability(Event& event) {

    if (event.getEventCount() == 0) {
        cout << "No hay eventos disponibles.\n";
        cout << "\nPresione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return false;
    }
    return true;
}

UserData* Sale::getOrRegisterUser(User& user) {

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

int Sale::chooseEvent(Event& event)
{
    if (event.getEventCount() == 0) {
        cout << "No hay eventos disponibles.\n";
        return -1;
    }

    cout << "Seleccione un evento:\n";
    for (int i = 1; i <= event.getEventCount(); i++) {

        cout << i << ". " << event.getEvents().getAt(i).getName() << "\n";
    }

    int selected = readIntInRange(1, event.getEventCount(),
        "Entrada invalida. Intente nuevamente: ");


    return selected;
}

int Sale::chooseSegment(Segment& segment, int selectedEvent)
{
    List<List<Segment>>& segments = segment.getSegmentsByEvent();
    List<int>& segmentCounts = segment.getSegmentCount();


    int numSegments = segmentCounts.getAt(selectedEvent);

    cout << "\nSegmentos disponibles:\n";
    for (int i = 1; i <= numSegments; i++) {
        cout << i << ". "
            << segments.getAt(selectedEvent).getAt(i).getName()
            << " - Precio: $"
            << segments.getAt(selectedEvent).getAt(i).getPrice() << "\n";
    }

    int chosen = readIntInRange(1, numSegments,
        "Entrada invalida. Intente nuevamente: ");


    return chosen;
}

Seating& Sale::ensureSeating(int selectedEvent, int selectedSegment, List<List<Segment>>& segments,
    map<std::tuple<int, int>, Seating>& seatingMap) {
    tuple<int, int> seatingKey = make_tuple(selectedEvent, selectedSegment);

    if (seatingMap.find(seatingKey) == seatingMap.end()) {
        Seating newSeating;
        Segment& seg = segments.getAt(selectedEvent).getAt(selectedSegment);
        newSeating.setNumberOfRows(seg.getRows());
        newSeating.setNumberOfColumns(seg.getSeats());
        newSeating.setCost(seg.getPrice());
        newSeating.initializeRoom();
        seatingMap[seatingKey] = newSeating;
    }

    Seating& seating = seatingMap[seatingKey];
    int availableSeats = 0;
    for (int i = 0; i < seating.getNumberOfRows(); i++) {
        for (int j = 0; j < seating.getNumberOfColumns(); j++) {
            if (!seating.getSeatPurchased()[i][j]) {
                availableSeats++;
            }
        }
    }
    if (availableSeats > 0) {
        seating.displaySeats();
    }
    else {
        // Este mensaje se repite con lo que haces en `sell()`
        cout << " " << endl;
    }

    return seating;
}

int Sale::buyTickets(UserData* currentUser, Event& event, int selectedEvent) {

    string userId = currentUser->getIdNumber();
    int currentTickets = event.getEvents().getAt(selectedEvent).getTicketsPurchasedByUser(userId);


    if (currentTickets >= 5) {
        cout << "Ya has comprado el numero maximo de 5 boletos para este evento.\n";
        cout << "\nPresione Enter para continuar...";
        cin.get();
        return 0;
    }

    while (true) {
        int maxTickets = 5 - currentTickets;
        cout << "\n ¿Cuantos boletos desea comprar? (max " << maxTickets << "): ";

        int numTickets = readIntInRange(1, maxTickets,
            "Entrada invalida. Por favor, ingrese un numero valido: "
        );

        if (event.getEvents().getAt(selectedEvent).purchaseTickets(userId, numTickets)) {
            return numTickets;
        }
        cout << "No se pudo completar la compra de boletos. Intente nuevamente.\n";
    }
}

float Sale::applyDiscountIfWanted(Discount& discount) {

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


void Sale::printInvoice(UserData* currentUser, Event& event, int selectedEvent, List<List<Segment>>& segments, int selectedSegment,
    int numTickets, float ticketPrice, float discountPercentage, float totalCost,
    int* purchasedRows, char* purchasedCols, int numPurchasedSeats, std::string cardNumber)
{
    cout << "\n\n==================== FACTURA ====================\n\n";
    cout << "Usuario: " << currentUser->getName() << endl;
    cout << "Cedula: " << currentUser->getIdNumber() << endl;
    cout << "Evento: " << event.getEvents().getAt(selectedEvent).getName() << endl;
    cout << "Segmento: " << segments.getAt(selectedEvent).getAt(selectedSegment).getName() << endl;
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


int Sale::readIntInRange(int minValue, int maxValue, const std::string& errorPrompt)
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

void Sale::cancelPurchase(User& user, Event& event, Segment& segment,
    std::map<std::tuple<int, int>, Seating>& seatingMap)
{
    // Verificar si hay eventos disponibles
    if (event.getEventCount() == 0) {
        cout << "No hay eventos disponibles para cancelar compras.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    // Solicitar cédula
    string idNumber;
    cout << "Ingrese su numero de cedula (9 digitos) para cancelar compra: ";
    cin >> idNumber;

    // Validar cédula
    if (idNumber.size() != 9 || idNumber.find_first_not_of("0123456789") != string::npos) {
        cout << "Cedula invalida o no existe.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    // Buscar usuario
    UserData* currentUser = user.searchUserById(idNumber);
    if (!currentUser) {
        cout << "Usuario no encontrado.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    // Mostrar eventos disponibles
    cout << "\nEventos disponibles:\n";
    for (int i = 1; i <= event.getEventCount(); i++) {
        cout << i << ". " << event.getEvents().getAt(i).getName() << "\n";
    }

    cout << "\nSeleccione el numero de evento para cancelar boletos: ";
    int selectedEvent = readIntInRange(1, event.getEventCount(), "Evento invalido. Intente de nuevo: ");

    // Verificar boletos comprados en el evento
    int purchasedTickets = event.getEvents().getAt(selectedEvent).getTicketsPurchasedByUser(idNumber);
    if (purchasedTickets == 0) {
        cout << "El usuario no tiene boletos comprados para este evento.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    cout << "Usted tiene " << purchasedTickets << " boletos para este evento.\n";

    cout << "\nCuantos boletos desea cancelar? ";
    int toCancel = readIntInRange(1, purchasedTickets, "Cantidad invalida. Intente de nuevo: ");

    // Obtener segmentos asociados al evento
    List<List<Segment>>& segments = segment.getSegmentsByEvent();
    List<int>& segmentCounts = segment.getSegmentCount();
    int numSegments = segmentCounts.getAt(selectedEvent);

    if (numSegments == 0) {
        cout << "El evento no tiene segmentos registrados.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    // Mostrar segmentos disponibles
    cout << "\nSegmentos disponibles para ese evento:\n";
    for (int i = 1; i <= numSegments; i++) {
        cout << i << ". " << segments.getAt(selectedEvent).getAt(i).getName()
            << " - Precio: $" << segments.getAt(selectedEvent).getAt(i).getPrice() << "\n";
    }

    cout << "\nSeleccione el numero de segmento para liberar asientos: ";
    int chosenSegment = readIntInRange(1, numSegments, "Segmento invalido. Intente de nuevo: ");

    // Verificar si en el segmento hay asientos vendidos
    tuple<int, int> seatingKey = make_tuple(selectedEvent, chosenSegment);
    if (seatingMap.find(seatingKey) == seatingMap.end()) {
        cout << "\nEn este segmento no hay asientos vendidos. Nada que cancelar.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    Seating& seating = seatingMap[seatingKey];

    cout << "\nIndique los asientos que desea liberar:\n";

    // Para cada boleto a cancelar, se solicita primero la columna y luego la fila.
    for (int i = 0; i < toCancel; i++) {
        char colChar = '\0';
        int row = -1;

        // Solicitar columna
        while (true) {
            cout << "Columna (A-" << char('A' + seating.getNumberOfColumns() - 1) << "): ";
            string colInput;
            cin >> colInput;
            if (colInput.size() == 1) {
                colChar = toupper(colInput[0]);
                int colIndex = colChar - 'A';
                if (colIndex >= 0 && colIndex < seating.getNumberOfColumns()) {
                    break; // Columna válida
                }
                else {
                    cout << "Columna fuera de rango.\n";
                }
            }
            else {
                cout << "Ingrese solo una letra.\n";
            }
        }

        // Solicitar fila
        while (true) {
            cout << "Fila (1-" << seating.getNumberOfRows() << "): ";
            if (cin >> row && row >= 1 && row <= seating.getNumberOfRows()) {
                break;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Fila invalida.\n";
        }

        int colIndex = colChar - 'A';
        // Intentar liberar el asiento
        if (seating.freeSeat(row - 1, colIndex)) {
            cout << "Asiento liberado correctamente.\n";
        }
        else {
            cout << "Ese asiento no estaba comprado. Intente otro.\n";
            i--; // Repetir este ciclo si la liberación falla
        }
    }

    // Actualizar la cantidad de boletos en el evento
    if (event.getEvents().getAt(selectedEvent).cancelTickets(idNumber, toCancel)) {
        cout << "\nSe han cancelado " << toCancel << " boletos correctamente.\n";
    }
    else {
        cout << "\nNo fue posible cancelar los boletos.\n";
    }

    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

