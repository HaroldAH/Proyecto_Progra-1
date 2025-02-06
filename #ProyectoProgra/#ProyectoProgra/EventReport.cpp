#include "EventReport.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

EventReport::EventReport() {
}

EventReport::~EventReport() {
}

void EventReport::generateGlobalReport(Event& evento,
    Segment& segmento,
    map<tuple<int, int>, Seating>& seatingMap,
    string& fileName)
{
    ofstream reporte(fileName, ios::out);
    if (!reporte.is_open()) {
        cerr << "Error al crear el archivo de reporte: " << fileName << endl;
        return;
    }

    // ====================================
    // Uso de localtime_s en lugar de localtime
    time_t now = time(nullptr);
    tm localNow;                       // Estructura tm en el stack
    localtime_s(&localNow, &now);      // Llamada "segura"

    char fechaHora[80];
    strftime(fechaHora, sizeof(fechaHora), "%d/%m/%Y %H:%M:%S", &localNow);
    // ====================================

    reporte << "================== REPORTE DE EVENTOS ==================\n";
    reporte << "Actualizado el: " << fechaHora << "\n\n";

    int totalEventos = evento.getEventCount();
    if (totalEventos == 0) {
        reporte << "No hay eventos registrados en el sistema.\n";
        reporte.close();
        cout << "No se pudo generar reporte: no hay eventos.\n";
        return;
    }

    for (int i = 1; i <= totalEventos; i++) {
        reporte << "================== REPORTE DEL EVENTO #" << i << " ==================\n";
        reporte << "Nombre del Evento: "
            << evento.getEvents().getAt(i).getName() << "\n\n";

        int totalDisponibles = 0;
        int totalVendidos = 0;
        float gananciasTotales = 0.0f;

        // Se obtienen todos los segmentos asociados al evento i
        List<Segment> listaSegmentos = segmento.getSegmentsByEvent().getAt(i);
        int numSegmentos = segmento.getSegmentCount().getAt(i);

        for (int segIndex = 1; segIndex <= numSegmentos; segIndex++) {
            Segment seg = listaSegmentos.getAt(segIndex);

            reporte << "-------------------- SEGMENTO #" << segIndex << " --------------------\n";
            reporte << "  Nombre del Segmento   : " << seg.getName() << "\n";
            reporte << "  Precio por asiento    : $" << fixed << setprecision(2) << seg.getPrice() << "\n";
            reporte << "  Filas                 : " << seg.getRows()
                << "    |    Asientos por fila: " << seg.getSeats() << "\n";

            // La clave en el map seatingMap es (eventoId, segmentoId)
            tuple<int, int> key = make_tuple(i, segIndex);

            if (seatingMap.find(key) != seatingMap.end()) {
                const Seating& sala = seatingMap.at(key);
                int filas = sala.getNumberOfRows();
                int columnas = sala.getNumberOfColumns();
                int totalAsientos = filas * columnas;
                int vendidos = 0;

                for (int r = 0; r < filas; r++) {
                    for (int c = 0; c < columnas; c++) {
                        if (sala.getSeatPurchased()[r][c]) {
                            vendidos++;
                        }
                    }
                }

                int disponibles = totalAsientos - vendidos;
                float ganancias = vendidos * seg.getPrice();

                reporte << "  Asientos Disponibles  : " << disponibles << "\n";
                reporte << "  Asientos Vendidos     : " << vendidos << "\n";
                reporte << "  Ganancias Segmento    : $" << fixed << setprecision(2) << ganancias << "\n\n";

                totalDisponibles += disponibles;
                totalVendidos += vendidos;
                gananciasTotales += ganancias;

                // Imprimir el mapa de asientos (X = vendido, [ ] = disponible)
                reporte << "  Mapa de Asientos:\n";
                for (int r = 0; r < filas; r++) {
                    for (int c = 0; c < columnas; c++) {
                        reporte << (sala.getSeatPurchased()[r][c] ? "[X]" : "[ ]");
                    }
                    reporte << "\n";
                }
            }
            else {
                reporte << "  No se ha configurado la sala para este segmento.\n";
            }
            reporte << "\n";
        }

        reporte << "================== RESUMEN DEL EVENTO ==================\n";
        reporte << "  Total de asientos disponibles: " << totalDisponibles << "\n";
        reporte << "  Total de asientos vendidos   : " << totalVendidos << "\n";
        reporte << "  Ganancias generadas          : $" << fixed << setprecision(2) << gananciasTotales << "\n";
        reporte << "========================================================\n\n";
    }

    reporte << "=======================================================\n";
    reporte.close();

    cout << "\nReporte actualizado exitosamente en \"" << fileName << "\".\n";
}