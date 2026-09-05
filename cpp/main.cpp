#include "sistema.hpp"
#include <iostream>
#include <string>

void pausa() {
    std::cout << "\nPresione ENTER para continuar.";
    std::string temp;
    std::getline(std::cin, temp);
}

int main() {
    GestionAcademica sistema;

    while (true) {
        std::cout << "\n=========================================\n";
        std::cout << "    SGA-DO: SISTEMA DIPLOMADOSONLINE     \n";
        std::cout << "=========================================\n";
        std::cout << "1. Registrar alumno\n";
        std::cout << "2. Registrar profesor\n";
        std::cout << "3. Registrar notas a un alumno\n";
        std::cout << "4. Deshacer ultimo registro de nota\n";
        std::cout << "5. Generar cola de certificados\n";
        std::cout << "6. Mostrar reporte general\n";
        std::cout << "7. Salir\n";
        std::cout << "=========================================\n";
        std::cout << "Seleccione una opcion (1-7): ";

        std::string opcion;
        std::getline(std::cin, opcion);

        if (opcion == "1") {
            sistema.registrarAlumno();
            pausa();
        } else if (opcion == "2") {
            sistema.registrarProfesor();
            pausa();
        } else if (opcion == "3") {
            sistema.registrarNota();
            pausa();
        } else if (opcion == "4") {
            sistema.deshacerUltimaNota();
            pausa();
        } else if (opcion == "5") {
            sistema.generarColaCertificados();
            pausa();
        } else if (opcion == "6") {
            sistema.mostrarReporteGeneral();
            pausa();
        } else if (opcion == "7") {
            std::cout << "\nGuardando datos de forma segura.\n";
            break;
        } else {
            std::cout << "\nOpción inválida.\n";
            pausa();
        }
    }

    return 0;
}