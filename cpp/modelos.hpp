#ifndef MODELOS_HPP
#define MODELOS_HPP

#include <string>
#include <vector>
#include <memory>
#include <numeric>

class Persona {
protected:
    std::string cedula;
    std::string nombreCompleto;
    std::string correo;

public:
    Persona(const std::string& ced, const std::string& nom, const std::string& cor)
        : cedula(ced), nombreCompleto(nom), correo(cor) {}
    
    virtual ~Persona() = default;

    std::string getCedula() const { return cedula; }
    std::string getNombreCompleto() const { return nombreCompleto; }
    std::string getCorreo() const { return correo; }
};

class Profesor : public Persona {
private:
    std::string especialidad;
    std::string materia;

public:
    Profesor(const std::string& ced, const std::string& nom, const std::string& cor,
             const std::string& esp, const std::string& mat)
        : Persona(ced, nom, cor), especialidad(esp), materia(mat) {}

    std::string getEspecialidad() const { return especialidad; }
    std::string getMateria() const { return materia; }
};

class ProgramaAcademico {
protected:
    std::string nombre;

public:
    ProgramaAcademico(const std::string& nom) : nombre(nom) {}
    virtual ~ProgramaAcademico() = default;

    std::string getNombre() const { return nombre; }
    virtual bool evaluarAprobacion(const std::vector<double>& notas, double promedio) const = 0;
};

class Curso : public ProgramaAcademico {
public:
    Curso() : ProgramaAcademico("Curso") {}
    bool evaluarAprobacion(const std::vector<double>& notas, double promedio) const override {
        return promedio >= 10.0;
    }
};

class Diplomado : public ProgramaAcademico {
public:
    Diplomado() : ProgramaAcademico("Diplomado") {}
    bool evaluarAprobacion(const std::vector<double>& notas, double promedio) const override {
        return promedio >= 14.0;
    }
};

class Bootcamp : public ProgramaAcademico {
public:
    Bootcamp() : ProgramaAcademico("Bootcamp") {}
    bool evaluarAprobacion(const std::vector<double>& notas, double promedio) const override {
        if (promedio < 14.0) return false;
        for (double n : notas) {
            if (n < 14.0) return false;
        }
        return true;
    }
};

class Alumno : public Persona {
private:
    std::shared_ptr<ProgramaAcademico> programa;
    std::vector<double> notas;

public:
    Alumno(const std::string& ced, const std::string& nom, const std::string& cor,
           std::shared_ptr<ProgramaAcademico> prog)
        : Persona(ced, nom, cor), programa(prog) {}

    void agregarNota(double nota) {
        if (notas.size() < 3) {
            notas.push_back(nota);
        }
    }

    void eliminarUltimaNota() {
        if (!notas.empty()) {
            notas.pop_back();
        }
    }

    double calcularPromedio() const {
        if (notas.empty()) return 0.0;
        double suma = std::accumulate(notas.begin(), notas.end(), 0.0);
        return suma / notas.size();
    }

    bool estaAprobado() const {
        if (notas.size() < 3) return false;
        return programa->evaluarAprobacion(notas, calcularPromedio());
    }

    std::shared_ptr<ProgramaAcademico> getPrograma() const { return programa; }
    const std::vector<double>& getNotas() const { return notas; }
};

#endif