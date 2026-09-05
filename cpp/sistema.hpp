#ifndef SISTEMA_HPP
#define SISTEMA_HPP

#include "modelos.hpp"
#include <map>
#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

struct RegistroNota {
    std::string cedula;
    double nota;
};

class GestionAcademica {
private:
    std::map<std::string, Alumno> alumnos;
    std::vector<Profesor> profesores;
    std::stack<RegistroNota> pilaNotas; 
    std::queue<Alumno> colaCertificados; 

    const std::string FILE_ALUMNOS = "alumnos.txt";
    const std::string FILE_PROFESORES = "profesores.txt";
    const std::string FILE_CERTIFICADOS = "certificados_pendientes.txt";

    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }

    std::vector<std::string> split(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(trim(token));
        }
        return tokens;
    }

    std::shared_ptr<ProgramaAcademico> instanciarPrograma(std::string nombre) {
        std::transform(nombre.begin(), nombre.end(), nombre.begin(), ::tolower);
        if (nombre == "diplomado") return std::make_shared<Diplomado>();
        if (nombre == "bootcamp") return std::make_shared<Bootcamp>();
        return std::make_shared<Curso>();
    }

    void cargarDatosIniciales() {
        std::ifstream fAlumnos(FILE_ALUMNOS);
        if (fAlumnos.is_open()) {
            std::string linea;
            while (std::getline(fAlumnos, linea)) {
                if (linea.empty()) continue;
                auto data = split(linea, ',');
                if (data.size() >= 4) {
                    std::string ced = data[0];
                    std::string nom = data[1];
                    std::string cor = data[2];
                    std::string progNom = data[3];

                    Alumno al(ced, nom, cor, instanciarPrograma(progNom));
                    if (data.size() > 4) {
                        for (size_t i = 4; i < data.size(); ++i) {
                            try {
                                double val = std::stod(data[i]);
                                if (val > 0) al.agregarNota(val);
                            } catch (...) {}
                        }
                    }
                    alumnos.insert({ced, al});
                }
            }
            fAlumnos.close();
        }

        std::ifstream fProf(FILE_PROFESORES);
        if (fProf.is_open()) {
            std::string linea;
            while (std::getline(fProf, linea)) {
                if (linea.empty()) continue;
                auto data = split(linea, ',');
                if (data.size() >= 5) {
                    profesores.emplace_back(data[0], data[1], data[2], data[3], data[4]);
                }
            }
            fProf.close();
        }
    }

    void guardarAlumnosTxt() {
        std::ofstream pw(FILE_ALUMNOS);
        if (pw.is_open()) {
            for (const auto& pair : alumnos) {
                const Alumno& al = pair.second;
                pw << al.getCedula() << ", " << al.getNombreCompleto() << ", " 
                   << al.getCorreo() << ", " << al.getPrograma()->getNombre() << ", ";
                
                const auto& n = al.getNotas();
                if (n.empty()) {
                    pw << "0,0,0";
                } else {
                    for (size_t i = 0; i < n.size(); ++i) {
                        pw << n[i];
                        if (i < n.size() - 1) pw << ",";
                    }
                }
                pw << "\n";
            }
            pw.close();
        }
    }

    void guardarProfesoresTxt() {
        std::ofstream pw(FILE_PROFESORES);
        if (pw.is_open()) {
            for (const auto& pr : profesores) {
                pw << pr.getCedula() << ", " << pr.getNombreCompleto() << ", " 
                   << pr.getCorreo() << ", " << pr.getEspecialidad() << ", " << pr.getMateria() << "\n";
            }
            pw.close();
        }
    }

public:
    GestionAcademica() {
        cargarDatosIniciales();
    }

    void registrarAlumno() {
        std::cout << "\n1. Registrar alumno.\n";
        std::string cedula, nombre, correo, tipo;
        std::cout << "Cedula/ID: "; std::getline(std::cin, cedula); cedula = trim(cedula);

        if (alumnos.find(cedula) != alumnos.end()) {
            std::cout << "Error: ya existe un alumno con esa cedula.\n";
            return;
        }

        std::cout << "Nombre completo: "; std::getline(std::cin, nombre); nombre = trim(nombre);
        std::cout << "Correo: "; std::getline(std::cin, correo); correo = trim(correo);
        std::cout << "Programa: 1. Curso | 2. Diplomado | 3. Bootcamp\nOpcion (1-3): ";
        std::getline(std::cin, tipo); tipo = trim(tipo);

        std::shared_ptr<ProgramaAcademico> prog;
        if (tipo == "1") prog = std::make_shared<Curso>();
        else if (tipo == "2") prog = std::make_shared<Diplomado>();
        else if (tipo == "3") prog = std::make_shared<Bootcamp>();
        else {
            std::cout << "Opción invalida.\n";
            return;
        }

        alumnos.insert({cedula, Alumno(cedula, nombre, correo, prog)});
        guardarAlumnosTxt();
        std::cout << "Alumno registrado y guardado en disco.\n";
    }

    void registrarProfesor() {
        std::cout << "\n2. Registrar profesor.\n";
        std::string cedula, nombre, correo, esp, mat;
        std::cout << "Cedula/ID: "; std::getline(std::cin, cedula); cedula = trim(cedula);
        std::cout << "Nombre completo: "; std::getline(std::cin, nombre); nombre = trim(nombre);
        std::cout << "Correo: "; std::getline(std::cin, correo); correo = trim(correo);
        std::cout << "Especialidad: "; std::getline(std::cin, esp); esp = trim(esp);
        std::cout << "Materia: "; std::getline(std::cin, mat); mat = trim(mat);

        profesores.emplace_back(cedula, nombre, correo, esp, mat);
        guardarProfesoresTxt();
        std::cout << "Profesor registrado y guardado en disco.\n";
    }

    void registrarNota() {
        std::cout << "\n3. Registrar notas a un Alumno.\n";
        std::string cedula;
        std::cout << "Ingrese cedula del alumno: "; std::getline(std::cin, cedula); cedula = trim(cedula);

        auto it = alumnos.find(cedula);
        if (it == alumnos.end()) {
            std::cout << "Error: alumno no encontrado.\n";
            return;
        }

        Alumno& alumno = it->second;
        if (alumno.getNotas().size() >= 3) {
            std::cout << "El alumno " << alumno.getNombreCompleto() << " ya posee las 3 notas maximas registradas.\n";
            return;
        }

        std::cout << "\nRegistrando notas para: " << alumno.getNombreCompleto() << "\n";
        std::cout << "Escriba 's' o 'salir' para regresar al menu principal.\n";

        while (alumno.getNotas().size() < 3) {
            size_t numNota = alumno.getNotas().size() + 1;
            std::cout << "Ingrese nota " << numNota << " (0-20): ";
            std::string entrada;
            std::getline(std::cin, entrada);
            entrada = trim(entrada);

            std::string lowerEntrada = entrada;
            std::transform(lowerEntrada.begin(), lowerEntrada.end(), lowerEntrada.begin(), ::tolower);

            if (lowerEntrada == "s" || lowerEntrada == "salir") {
                std::cout << "Finalizando ingreso de notas.\n";
                break;
            }

            try {
                double nota = std::stod(entrada);
                if (nota >= 0.0 && nota <= 20.0) {
                    alumno.agregarNota(nota);
                    pilaNotas.push({cedula, nota});
                    guardarAlumnosTxt();
                    std::cout << "-> Nota " << numNota << " (" << nota << ") registrada con exito.\n";
                } else {
                    std::cout << "Error: la calificación debe estar entre 0 y 20.\n";
                }
            } catch (...) {
                std::cout << "Error: ingrese un numero valido o 's' para salir.\n";
            }
        }

        if (alumno.getNotas().size() == 3) {
            std::cout << "\nSe han completado las 3 notas de " << alumno.getNombreCompleto() << ".\n";
        }
    }

    void deshacerUltimaNota() {
        std::cout << "\n4. Deshacer ultimo registro (LIFO).\n";
        if (pilaNotas.empty()) {
            std::cout << "No hay notas recientes para deshacer.\n";
            return;
        }

        RegistroNota accion = pilaNotas.top();
        pilaNotas.pop();

        auto it = alumnos.find(accion.cedula);
        if (it != alumnos.end() && !it->second.getNotas().empty()) {
            it->second.eliminarUltimaNota();
            guardarAlumnosTxt();
            std::cout << "Exito: se elimino la ultima nota (" << accion.nota << ") del alumno con cedula " << accion.cedula << ".\n";
        }
    }

    void generarColaCertificados() {
        std::cout << "\n5. Generar cola de certificados (FIFO).\n";
        while (!colaCertificados.empty()) colaCertificados.pop();

        for (const auto& pair : alumnos) {
            if (pair.second.estaAprobado()) {
                colaCertificados.push(pair.second);
            }
        }

        size_t total = colaCertificados.size();
        std::ofstream pw(FILE_CERTIFICADOS);
        if (pw.is_open()) {
            pw << "=========================================\n";
            pw << "   REPORTE DE CERTIFICADOS PENDIENTES    \n";
            pw << "=========================================\n";
            pw << "Total de graduandos en cola: " << total << "\n\n";

            int i = 1;
            while (!colaCertificados.empty()) {
                Alumno al = colaCertificados.front();
                colaCertificados.pop();

                pw << i << ". [" << al.getCedula() << "] " << al.getNombreCompleto() << "\n";
                pw << "   - Programa: " << al.getPrograma()->getNombre() << "\n";
                pw << "   - Promedio final: " << std::fixed << std::setprecision(1) << al.calcularPromedio() << "\n";
                pw << "   - Estatus: APROBADO\n\n";
                i++;
            }
            pw << "=========================================\n";
            pw << "* Fin del reporte - Generado por SGA-DO *\n";
            pw.close();

            std::cout << "Reporte exportado exitosamente a '" << FILE_CERTIFICADOS << "'.\n";
        } else {
            std::cout << "Error al generar reporte en disco.\n";
        }
    }

    void mostrarReporteGeneral() {
        std::cout << "\n=========================================\n";
        std::cout << "          REPORTE GENERAL SGA-DO         \n";
        std::cout << "=========================================\n";
        std::cout << "\n--- PROFESORES ACTIVOS ---\n";
        if (profesores.empty()) {
            std::cout << "No hay profesores registrados.\n";
        }
        for (const auto& p : profesores) {
            std::cout << "[" << p.getCedula() << "] " << p.getNombreCompleto() 
                      << " | Esp: " << p.getEspecialidad() << " | Mat: " << p.getMateria() << "\n";
        }

        std::cout << "\n--- ALUMNOS REGISTRADOS ---\n";
        if (alumnos.empty()) {
            std::cout << "No hay alumnos registrados.\n";
        }
        for (const auto& pair : alumnos) {
            const Alumno& a = pair.second;
            std::string estatus = a.estaAprobado() ? "APROBADO" : "REPROBADO";
            std::cout << "[" << a.getCedula() << "] " << a.getNombreCompleto() 
                      << " | Prog: " << a.getPrograma()->getNombre() << " | Notas: [";
            
            const auto& n = a.getNotas();
            for (size_t i = 0; i < n.size(); ++i) {
                std::cout << n[i] << (i < n.size() - 1 ? ", " : "");
            }
            std::cout << "] | Prom: " << std::fixed << std::setprecision(1) << a.calcularPromedio() 
                      << " | Estatus: " << estatus << "\n";
        }
        std::cout << "=========================================\n";
    }
};

#endif