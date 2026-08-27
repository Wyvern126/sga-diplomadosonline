import java.util.ArrayList;
import java.util.List;

abstract class Persona {
    protected String cedula;
    protected String nombreCompleto;
    protected String correo;

    public Persona(String cedula, String nombreCompleto, String correo) {
        this.cedula = cedula;
        this.nombreCompleto = nombreCompleto;
        this.correo = correo;
    }

    public String getCedula() { return cedula; }
    public String getNombreCompleto() { return nombreCompleto; }
    public String getCorreo() { return correo; }
}

class Profesor extends Persona {
    private String especialidad;
    private String materia;

    public Profesor(String cedula, String nombreCompleto, String correo, String especialidad, String materia) {
        super(cedula, nombreCompleto, correo);
        this.especialidad = especialidad;
        this.materia = materia;
    }

    public String getEspecialidad() { return especialidad; }
    public String getMateria() { return materia; }
}

abstract class ProgramaAcademico {
    protected String nombre;

    public ProgramaAcademico(String nombre) {
        this.nombre = nombre;
    }

    public String getNombre() { return nombre; }
    public abstract boolean evaluarAprobacion(List<Double> notas, double promedio);
}

class Curso extends ProgramaAcademico {
    public Curso() { super("Curso"); }

    @Override
    public boolean evaluarAprobacion(List<Double> notas, double promedio) {
        return promedio >= 10.0;
    }
}

class Diplomado extends ProgramaAcademico {
    public Diplomado() { super("Diplomado"); }

    @Override
    public boolean evaluarAprobacion(List<Double> notas, double promedio) {
        return promedio >= 14.0;
    }
}

class Bootcamp extends ProgramaAcademico {
    public Bootcamp() { super("Bootcamp"); }

    @Override
    public boolean evaluarAprobacion(List<Double> notas, double promedio) {
        if (promedio < 14.0) return false;
        for (double n : notas) {
            if (n < 14.0) return false;
        }
        return true;
    }
}

class Alumno extends Persona {
    private ProgramaAcademico programa;
    private List<Double> notas;

    public Alumno(String cedula, String nombreCompleto, String correo, ProgramaAcademico programa) {
        super(cedula, nombreCompleto, correo);
        this.programa = programa;
        this.notas = new ArrayList<>();
    }

    public void agregarNota(double nota) {
        if (this.notas.size() < 3) {
            this.notas.add(nota);
        }
    }

    public double calcularPromedio() {
        if (notas.isEmpty()) return 0.0;
        double suma = 0.0;
        for (double n : notas) suma += n;
        return suma / notas.size();
    }

    public boolean estaAprobado() {
        if (notas.size() < 3) return false;
        return programa.evaluarAprobacion(notas, calcularPromedio());
    }

    public ProgramaAcademico getPrograma() { return programa; }
    public List<Double> getNotas() { return notas; }
}