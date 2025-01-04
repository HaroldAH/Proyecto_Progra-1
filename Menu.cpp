// Menu.cpp
#include "Menu.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <vector>
#include "Utilities.h" // Si tienes funciones auxiliares

using namespace std;

// Definición de validateChoice con un solo parámetro
int Menu::validateChoice(int maxOption) {
    int choice;
    while (true) {
        cout << "Ingrese su eleccion (1-" << maxOption << "): ";
        cin >> choice;

        // Verificar si la entrada falló (no era un número)
        if (cin.fail()) {
            cin.clear(); // Limpiar el estado de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar la entrada inválida
            cout << "Entrada invalida. Por favor, ingrese un numero entre 1 y " << maxOption << ".\n";
            continue;
        }

        // Limpiar el buffer de entrada
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Verificar si la elección está dentro del rango permitido
        if (choice >= 1 && choice <= maxOption) {
            return choice;
        } else {
            cout << "Opcion fuera de rango. Por favor, ingrese un numero entre 1 y " << maxOption << ".\n";
        }
    }
}

// Definición de showAbout como método miembro de Menu
void Menu::showAbout()
{
    cout << "\n=========================================\n";
    cout << "               ACERCA DE                 \n";
    cout << "=========================================\n";
    cout << "Integrantes del Proyecto:\n";
    cout << "  - Harold Avila Hernandez\n";
    cout << "  - Cristhian Cordero Varela\n";
    cout << "=========================================\n";
    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Implementación de showMenu
void Menu::showMenu()
{
    int option, size = 6;

    do
    {
        cout << "\033[31m";
        // (ASCII Art omitido para brevedad)
        cout << "\033[37m";

        cout << "               +==========================+\n";
        cout << "                |     Menu Principal      |\n";
        cout << "                +==========================+\n";
        cout << "                | 1. Configurar Evento    |\n";
        cout << "                | 2. Configurar Descuentos|\n";
        cout << "                | 3. Vender Entrada       |\n";
        cout << "                | 4. Consultar Ventas     |\n";
        cout << "                | 5. Acerca de            |\n";
        cout << "                | 6. Salir                |\n";
        cout << "                +==========================+\n";

        option = validateChoice(size);

        switch (option)
        {
        case 1:
            showEventMenu(event);
            break;
        case 2:
            configureDiscounts();
            break;
        case 3:
            sellTicket();
            break;
        case 4:
            consultarVentas();
            break;
        case 5:
            showAbout();
            break;
        case 6:
            cout << "Saliendo del programa. ¡Gracias!" << endl;
            break;
        default:
            cout << "Opcion inválida. Intente nuevamente." << endl;
        }
    } while (option != 6);
}

// Implementación de configureDiscounts
void Menu::configureDiscounts()
{
    float discountPercentage;
    int discountCount;

    cout << "\n=== Configurar Descuentos ===" << endl;
    cout << "Ingrese el porcentaje de descuento: ";
    cin >> discountPercentage;

    cout << "Ingrese la cantidad de descuentos a generar: ";
    cin >> discountCount;

    // En vez de crear un Discount local, 
    // usa el discountManager del Menu:
    discountManager.configure(discountPercentage, discountCount);

    discountManager.showCodes();
    cout << "RECUERDE EL CODIGO\n";
    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Implementación de showEventMenu
void Menu::showEventMenu(Event &event)
{
    while (true)
    {
        int choice, size = 4;
        cout << "\n=== Configurar Eventos ===" << endl;
        cout << "1. Agregar nuevo evento" << endl;
        cout << "2. Modificar segmentos" << endl;
        cout << "3. Mostrar eventos actuales" << endl;
        cout << "4. Regresar al menu principal" << endl;

        choice = validateChoice(size);

        if (choice == 1)
        {
            event.saveEvent(event);
            continue;
        }

        if (choice == 2)
        {
            segment.saveSegments(event);
            continue;
        }

        if (choice == 3)
        {
            listEventAndSegments(event, segment);
            continue;
        }

        if (choice == 4)
        {
            return;
        }
        cout << endl
             << " Entrada invalida. Intente de nuevo." << endl
             << endl;
    }
}

// Implementación de listEventAndSegments
void Menu::listEventAndSegments(Event &event, Segment &segment)
{
    if (event.getEventCount() == 0)
    {
        cout << "No hay eventos guardados." << endl;
        return;
    }

    cout << endl
         << "Lista de eventos y segmentos:" << endl
         << endl;

    Segment **segmentsByEvent = segment.getSegmentsByEvent();

    for (int i = 0; i < event.getEventCount(); i++)
    {
        cout << "Evento #" << i + 1 << ":" << endl;
        cout << "Nombre: " << event.getEvents()[i].getName() << endl;
        cout << "Fecha: " << event.getEvents()[i].getDate() << endl;
        cout << "Descripcion: " << event.getEvents()[i].getDescription() << endl;

        cout << "Segmentos asociados:" << endl
             << endl;

        if (segmentsByEvent[i] == NULL)
        {
            cout << "  No hay segmentos asociados para este evento." << endl;
            cout << "--------------------------" << endl;
            continue;
        }

        int *segmentCounts = new int[event.getEventCount()];
        segment.getSegmentCount(event, segmentCounts);
        int numSegments = segmentCounts[i];

        for (int j = 0; j < numSegments; j++)
        {
            cout << "  Segmento #" << j + 1 << ":" << endl;
            cout << "    Nombre: " << segmentsByEvent[i][j].getName() << endl;
            cout << "    Filas: " << segmentsByEvent[i][j].getRows() << endl;
            cout << "    Asientos por fila: " << segmentsByEvent[i][j].getSeats() << endl;
            cout << "    Precio: " << segmentsByEvent[i][j].getPrice() << endl;
        }

        cout << "--------------------------" << endl;

        delete[] segmentCounts;
    }
}

// Implementación de sellTicket
void Menu::sellTicket()
{
    int choice, size = 2; // Número total de opciones

    cout << "\n+=======================+" << endl;
    cout << "|      VENTA DE BOLETOS |" << endl;
    cout << "+=======================+" << endl;

    cout << "1. Agregar usuario" << endl;
    cout << "2. Hacer compra" << endl;

    cout << "Seleccione una opcion: ";
    cin >> choice;

    // Validación de la opción
    while (choice < 1 || choice > size)
    {
        cout << "Opcion invalida. Intente nuevamente: ";
        cin >> choice;
    }

    // Procesar la opción seleccionada
    switch (choice)
    {
    case 1:
        crearUsuario();
        break;
    case 2:
        venta();
        break;
    default:
        cout << "Error inesperado. Intente nuevamente." << endl;
    }
}

// Implementación de crearUsuario
void Menu::crearUsuario()
{
    string cedula, nombre, fechaNacimiento;

    cout << "\n+=======================+" << endl;
    cout << "|    REGISTRO DE USUARIO|" << endl;
    cout << "+=======================+" << endl;

    // Solicitar datos del usuario
    cout << "Ingrese la cedula del usuario: ";
    cin >> cedula;

    cout << "Ingrese el nombre del usuario: ";
    cin.ignore();
    getline(cin, nombre);

    cout << "Ingrese la fecha de nacimiento del usuario (DD/MM/AAAA): ";
    cin >> fechaNacimiento;

    // Redimensionar el arreglo si es necesario
    if (numUsuarios == capacidadUsuarios)
    {
        int nuevaCapacidad = (capacidadUsuarios == 0) ? 1 : capacidadUsuarios * 2;
        User *nuevoArreglo = new User[nuevaCapacidad];

        // Copiar los usuarios existentes al nuevo arreglo
        for (int i = 0; i < numUsuarios; i++)
        {
            nuevoArreglo[i] = usuarios[i];
        }

        // Liberar memoria del arreglo anterior
        delete[] usuarios;

        // Actualizar puntero y capacidad
        usuarios = nuevoArreglo;
        capacidadUsuarios = nuevaCapacidad;
    }

    // Agregar el nuevo usuario al arreglo
    usuarios[numUsuarios] = User(cedula, nombre, fechaNacimiento, 0);
    numUsuarios++;

    cout << "Usuario registrado exitosamente.\n";
}

// Implementación de venta (compra de boletos)
void Menu::venta()
{
    if (event.getEventCount() == 0)
    {
        cout << "No hay eventos disponibles.\n";
        return;
    }

    // Solicitar número de cédula y verificar si el usuario existe
    string cedula;
    User *usuarioActual = nullptr;

    while (true)
    {
        cout << "Ingrese su número de cedula: ";
        cin >> cedula;

        usuarioActual = buscarUsuarioPorCedula(cedula);

        if (usuarioActual != nullptr)
        {
            cout << "Usuario encontrado. Bienvenido, " << usuarioActual->getNombre() << ".\n";
            break;
        }
        else
        {
            cout << "Usuario no encontrado. ¿Desea registrarse? (S/N): ";
            char opcion;
            cin >> opcion;
            if (opcion == 'S' || opcion == 's')
            {
                crearUsuario();
                // Después de crear el usuario, buscarlo nuevamente
                usuarioActual = buscarUsuarioPorCedula(cedula);
                if (usuarioActual != nullptr)
                {
                    cout << "Usuario registrado y encontrado. Bienvenido, " << usuarioActual->getNombre() << ".\n";
                    break;
                }
                else
                {
                    cout << "Error al registrar el usuario. Intente nuevamente.\n";
                }
            }
            else
            {
                cout << "Regresando al menu principal...\n";
                return;
            }
        }
    }

    // Mostrar eventos disponibles
    cout << "Seleccione un evento:\n";
    for (int i = 0; i < event.getEventCount(); i++)
    {
        cout << i + 1 << ". " << event.getEvents()[i].getName() << "\n";
    }

    int eventoSeleccionado = validateChoice(event.getEventCount());

    // Solicitar segmento
    Segment **segmentos = segment.getSegmentsByEvent();

    if (segmentos == NULL)
    {
        cout << "Error: No se pudieron obtener los segmentos.\n";
        return;
    }

    int *segmentCounts = new int[event.getEventCount()];
    segment.getSegmentCount(event, segmentCounts);

    int numSegmentos = segmentCounts[eventoSeleccionado - 1];
    delete[] segmentCounts;

    if (numSegmentos <= 0)
    {
        cout << "No hay segmentos disponibles para este evento.\n";
        return;
    }

    cout << "\nSegmentos disponibles: " << numSegmentos << endl;
    cout << "Seleccione un segmento: " << endl;
    for (int i = 0; i < numSegmentos; i++)
    {
        cout << i + 1 << ". " << segmentos[eventoSeleccionado - 1][i].getName()
             << " - Precio: $" << fixed << setprecision(2) << segmentos[eventoSeleccionado - 1][i].getPrice() << "\n";
    }

    int segmentoSeleccionado = validateChoice(numSegmentos);
    if (segmentoSeleccionado == -1)
    {
        cout << "Seleccion invalida. Regresando al menu principal.\n";
        return;
    }

    // Crear una clave única para el mapa
    auto seatingKey = make_tuple(eventoSeleccionado - 1, segmentoSeleccionado - 1);

    // Verificar si el segmento ya tiene un Seating asociado
    if (seatingMap.find(seatingKey) == seatingMap.end())
    {
        Seating newSeating;
        int filas = segmentos[eventoSeleccionado - 1][segmentoSeleccionado - 1].getRows();
        int columnas = segmentos[eventoSeleccionado - 1][segmentoSeleccionado - 1].getSeats();
        float precio = segmentos[eventoSeleccionado - 1][segmentoSeleccionado - 1].getPrice();

        newSeating.setAmountOfRows(filas);
        newSeating.setAmountOfColumns(columnas);
        newSeating.setCost(precio);
        newSeating.initializeRoom();

        seatingMap[seatingKey] = newSeating;
    }

    Seating &seating = seatingMap[seatingKey];

    seating.displaySeats();

    // Verificar si el usuario ya ha alcanzado el límite de boletos
    int boletosActuales = usuarioActual->getCantidadEntradasCompradas();
    if (boletosActuales >= 5)
    {
        cout << "Ya has comprado el numero maximo de 5 boletos.\n";
        return;
    }

    int numBoletos = 0;

    // Implementación de las validaciones solicitadas
    while (true)
    {
        cout << "\n¿Cuantos boletos desea comprar? ";
        cin >> numBoletos;

        // Verificar si la entrada falló (no era un número)
        if (cin.fail())
        {
            cin.clear(); // Limpiar el estado de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar la entrada inválida
            cout << "Entrada invalida. Por favor, ingrese un numero entre 1 y " << (5 - boletosActuales) << ".\n";
            continue;
        }

        // Limpiar el buffer de entrada
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Verificar si numBoletos es mayor a lo que puede comprar
        if (numBoletos > (5 - boletosActuales))
        {
            cout << "No puedes comprar mas de " << (5 - boletosActuales) << " boletos. Por favor, ingresa un número mas bajo.\n";

            // Preguntar al usuario si desea ingresar una nueva cantidad o salir
            char opcion;
            while (true)
            {
                cout << "¿Desea ingresar una nueva cantidad? (S/N): ";
                cin >> opcion;

                if (opcion == 'S' || opcion == 's')
                {
                    break; // Volver al inicio del ciclo para ingresar una nueva cantidad
                }
                else if (opcion == 'N' || opcion == 'n')
                {
                    cout << "Regresando al menu principal...\n";
                    return;
                }
                else
                {
                    cout << "Opcion invalida. Por favor, ingrese 'S' para si o 'N' para no.\n";
                }
            }
            continue; // Volver al inicio del ciclo para ingresar una nueva cantidad
        }

        // Verificar si numBoletos es menor a 1
        if (numBoletos < 1)
        {
            cout << "La cantidad de boletos debe ser al menos 1.\n";
            continue;
        }

        // Intentar comprar las entradas en la clase User
        if (usuarioActual->comprarEntradas(numBoletos))
        {
            // La compra fue exitosa
            break; // Salir del ciclo
        }
        else
        {
            // La compra fue rechazada por alguna razón (ya manejada en la función)
            cout << "No se pudo completar la compra de boletos.\n";

            // Preguntar al usuario si desea intentar nuevamente o salir
            char opcion;
            while (true)
            {
                cout << "¿Desea intentar nuevamente? (S/N): ";
                cin >> opcion;

                if (opcion == 'S' || opcion == 's')
                {
                    break; // Volver al inicio del ciclo para ingresar una nueva cantidad
                }
                else if (opcion == 'N' || opcion == 'n')
                {
                    cout << "Regresando al menu principal...\n";
                    return;
                }
                else
                {
                    cout << "Opción invalida. Por favor, ingrese 'S' para sí o 'N' para no.\n";
                }
            }
            continue; // Volver al inicio del ciclo para ingresar una nueva cantidad
        }
    }

    vector<pair<int, char>> asientosComprados; // Registro de asientos comprados

    for (int i = 0; i < numBoletos; i++)
    {
        int fila;
        char columnaLetra;

        cout << "Ingrese la columna del asiento que desea comprar (A-" << char('A' + seating.getAmountOfColumns() - 1) << "): ";
        cin >> columnaLetra;

        // Validar columna
        while (toupper(columnaLetra) < 'A' || toupper(columnaLetra) > char('A' + seating.getAmountOfColumns() - 1))
        {
            cout << "Columna invalida. Intente de nuevo: ";
            cin >> columnaLetra;
        }

        // Convertir a índice de columna
        int columna = toupper(columnaLetra) - 'A';

        cout << "\nIngrese la fila del asiento que desea comprar (1-" << seating.getAmountOfRows() << "): ";
        cin >> fila;

        // Validar fila
        while (fila < 1 || fila > seating.getAmountOfRows())
        {
            cout << "Fila invalida. Intente de nuevo: ";
            cin >> fila;
        }

        // Verificar si el asiento ya está comprado
        if (seating.getIsSeatPurchased()[fila - 1][columna])
        {
            cout << "El asiento ya esta ocupado. Por favor, elija otro.\n";
            i--; // Reintentar la compra
        }
        else
        {
            // Marcar el asiento como comprado
            seating.getIsSeatPurchased()[fila - 1][columna] = true;
            asientosComprados.push_back({fila, columnaLetra});
            cout << "Asiento reservado con exito.\n";
        }
    }

    // **Nuevo Paso: Solicitar Código de Descuento**
    string codigoDescuento;
    bool descuentoAplicado = false;
    float porcentajeDescuento = 0.0f;

    cout << "\n¿Tiene un codigo de descuento? (S/N): ";
    char usarDescuento;
    cin >> usarDescuento;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar el buffer

    if (usarDescuento == 'S' || usarDescuento == 's')
    {
        cout << "Ingrese su codigo de descuento: ";
        getline(cin, codigoDescuento);

        if (discountManager.verifyCode(codigoDescuento))
        {
            porcentajeDescuento = discountManager.getDiscountPercentage();
            descuentoAplicado = true;
            cout << "Descuento aplicado: " << porcentajeDescuento << "%\n";
        }
        else
        {
            cout << "Codigo de descuento invalido o ya utilizado.\n";
        }
    }

    // **Cálculo del Total con Descuento**
    float precioPorBoleto = seating.getCost();
    float totalCost = precioPorBoleto * numBoletos;

    if (descuentoAplicado)
    {
        float descuento = totalCost * (porcentajeDescuento / 100.0f);
        totalCost -= descuento;
    }

    // Solicitar el número de tarjeta
    string cardNumber;
    bool validCard = false;

    while (!validCard)
    {
        cout << "\nIngrese el numero de su tarjeta para completar la compra: ";
        cin >> cardNumber;

        if (cardNumber.length() >= 12 && cardNumber.find_first_not_of("0123456789") == string::npos)
        {
            validCard = true;
        }
        else
        {
            cout << "Numero de tarjeta invalido. Asegurese de ingresar al menos 12 digitos y solo numeros.\n";
        }
    }

    // **Imprimir la Factura**
    cout << "==================== FACTURA ====================" << endl;
    cout << "Usuario: " << usuarioActual->getNombre() << endl;
    cout << "Cedula: " << usuarioActual->getCedula() << endl;
    cout << "Evento: " << event.getEvents()[eventoSeleccionado - 1].getName() << endl;
    cout << "Segmento: " << segmentos[eventoSeleccionado - 1][segmentoSeleccionado - 1].getName() << endl;
    cout << "Numero de Tarjeta: ****-****-****-" << cardNumber.substr(cardNumber.length() - 4) << endl;
    cout << "Cantidad de boletos: " << numBoletos << endl;
    cout << "Precio por boleto: $" << fixed << setprecision(2) << precioPorBoleto << endl;

    if (descuentoAplicado)
    {
        cout << "Descuento aplicado: " << porcentajeDescuento << "%" << endl;
    }

    cout << "Asientos comprados: ";
    for (const auto &asiento : asientosComprados)
    {
        cout << "(Fila: " << asiento.first << ", Columna: " << asiento.second << ") ";
    }
    cout << endl;

    cout << "Total pagado: $" << fixed << setprecision(2) << totalCost << endl;
    cout << "===============================================" << endl;

    cout << "Presione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
// Implementación de buscarUsuarioPorCedula
User* Menu::buscarUsuarioPorCedula(const string &cedula)
{
    for (int i = 0; i < numUsuarios; i++)
    {
        if (usuarios[i].getCedula() == cedula)
        {                        // Comparar cédulas
            return &usuarios[i]; // Retorna un puntero al usuario encontrado
        }
    }
    return NULL; // No encontrado
}

// Implementación de consultarVentas
void Menu::consultarVentas() {
    if (event.getEventCount() == 0) {
        cout << "No hay eventos disponibles.\n";
        return;
    }

    // Mostrar todos los eventos disponibles
    cout << "\nEventos disponibles:\n";
    for (int i = 0; i < event.getEventCount(); i++) {
        cout << i + 1 << ". " << event.getEvents()[i].getName() << "\n";
    }

    // Solicitar selección de un evento
    int eventoSeleccionado;
    cout << "\nSeleccione un evento para ver sus segmentos: ";
    eventoSeleccionado = validateChoice(event.getEventCount());

    // Obtener segmentos asociados al evento seleccionado
    Segment** segmentos = segment.getSegmentsByEvent();
    if (segmentos == nullptr) {
        cout << "Error: No se pudieron obtener los segmentos del evento.\n";
        return;
    }

    int* segmentCounts = new int[event.getEventCount()];
    segment.getSegmentCount(event, segmentCounts);

    int numSegmentos = segmentCounts[eventoSeleccionado - 1];
    delete[] segmentCounts;

    if (numSegmentos <= 0) {
        cout << "No hay segmentos disponibles para este evento.\n";
        return;
    }

    // Mostrar segmentos disponibles
    cout << "\nSegmentos disponibles para el evento \"" << event.getEvents()[eventoSeleccionado - 1].getName() << "\":\n";
    for (int i = 0; i < numSegmentos; i++) {
        cout << i + 1 << ". " << segmentos[eventoSeleccionado - 1][i].getName()
             << " - Precio: " << segmentos[eventoSeleccionado - 1][i].getPrice() << "\n";
    }

    // Solicitar selección de un segmento
    int segmentoSeleccionado;
    cout << "\nSeleccione un segmento para ver su representacion gráfica: ";
    segmentoSeleccionado = validateChoice(numSegmentos);

    // Crear una clave única para el mapa
    auto seatingKey = std::make_tuple(eventoSeleccionado - 1, segmentoSeleccionado - 1);

    // Verificar si el segmento ya tiene un Seating asociado
    if (seatingMap.find(seatingKey) == seatingMap.end()) {
        Seating newSeating;
        int filas = segmentos[eventoSeleccionado - 1][segmentoSeleccionado - 1].getRows();
        int columnas = segmentos[eventoSeleccionado - 1][segmentoSeleccionado - 1].getSeats();
        float precio = segmentos[eventoSeleccionado - 1][segmentoSeleccionado - 1].getPrice();

        newSeating.setAmountOfRows(filas);
        newSeating.setAmountOfColumns(columnas);
        newSeating.setCost(precio);
        newSeating.initializeRoom();

        seatingMap[seatingKey] = newSeating;
    }

    // Mostrar la representación gráfica del segmento
    Seating& seating = seatingMap[seatingKey];
    cout << "\nRepresentacion grafica del segmento \"" << segmentos[eventoSeleccionado - 1][segmentoSeleccionado - 1].getName() << "\":\n";
    seating.displaySeats();

    cout << "Presione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}