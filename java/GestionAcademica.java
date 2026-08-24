import java.io.*;
import java.util.*;

class RegistroNota {
    String cedula;
    double nota;

    public RegistroNota(String cedula, double nota) {
        this.cedula = cedula;
        this.nota = nota;
    }
}

public class GestionAcademica {
    private Map<String, Alumno> alumnos = new HashMap<>();
    private List<Profesor> profesores = new ArrayList<>();
    private Stack<RegistroNota> pilaNotas = new Stack<>();
    private Queue<Alumno> colaCertificados = new LinkedList<>();

    private final String FILE_ALUMNOS = "alumnos.txt";
    private final String FILE_PROFESORES = "profesores.txt";
    private final String FILE_CERTIFICADOS = "certificados_pendientes.txt";

    public GestionAcademica() {
        cargarDatosIniciales();
    }

    private ProgramaAcademico instanciarPrograma(String nombre) {
        switch (nombre.trim().toLowerCase()) {
            case "diplomado": return new Diplomado();
            case "bootcamp": return new Bootcamp();
            default: return new Curso();
        }
    }

    private void cargarDatosIniciales() {
        File fAlumnos = new File(FILE_ALUMNOS);
        if (fAlumnos.exists()) {
            try (BufferedReader br = new BufferedReader(new FileReader(fAlumnos))) {
                String line;
                while ((line = br.readLine()) != null) {
                    String[] data = line.split(",");
                    if (data.length >= 4) {
                        String ced = data[0].trim();
                        String nom = data[1].trim();
                        String cor = data[2].trim();
                        String progNom = data[3].trim();
                        
                        Alumno al = new Alumno(ced, nom, cor, instanciarPrograma(progNom));
                        if (data.length > 4) {
                            for (int i = 4; i < data.length; i++) {
                                try {
                                    double val = Double.parseDouble(data[i].trim());
                                    if (val > 0) al.agregarNota(val);
                                } catch (NumberFormatException ignored) {}
                            }
                        }
                        alumnos.put(ced, al);
                    }
                }
            } catch (IOException ignored) {}
        }

        File fProf = new File(FILE_PROFESORES);
        if (fProf.exists()) {
            try (BufferedReader br = new BufferedReader(new FileReader(fProf))) {
                String line;
                while ((line = br.readLine()) != null) {
                    String[] data = line.split(",");
                    if (data.length >= 5) {
                        profesores.add(new Profesor(data[0].trim(), data[1].trim(), data[2].trim(), data[3].trim(), data[4].trim()));
                    }
                }
            } catch (IOException ignored) {}
        }
    }

    private void guardarAlumnosTxt() {
        try (PrintWriter pw = new PrintWriter(new FileWriter(FILE_ALUMNOS))) {
            for (Alumno al : alumnos.values()) {
                StringBuilder notasStr = new StringBuilder();
                if (al.getNotas().isEmpty()) {
                    notasStr.append("0,0,0");
                } else {
                    for (int i = 0; i < al.getNotas().size(); i++) {
                        notasStr.append(al.getNotas().get(i));
                        if (i < al.getNotas().size() - 1) notasStr.append(",");
                    }
                }
                pw.println(al.getCedula() + ", " + al.getNombreCompleto() + ", " + al.getCorreo() + ", " + al.getPrograma().getNombre() + ", " + notasStr);
            }
        } catch (IOException ignored) {}
    }

    private void guardarProfesoresTxt() {
        try (PrintWriter pw = new PrintWriter(new FileWriter(FILE_PROFESORES))) {
            for (Profesor pr : profesores) {
                pw.println(pr.getCedula() + ", " + pr.getNombreCompleto() + ", " + pr.getCorreo() + ", " + pr.getEspecialidad() + ", " + pr.getMateria());
            }
        } catch (IOException ignored) {}
    }

    public void registrarAlumno(Scanner scanner) {
        System.out.println("\n--- 1. Registrar alumno ---");
        System.out.print("Cedula/ID: ");
        String cedula = scanner.nextLine().trim();

        if (alumnos.containsKey(cedula)) {
            System.out.println("Error: ya existe un alumno con esa cedula.");
            return;
        }

        System.out.print("Nombre completo: ");
        String nombre = scanner.nextLine().trim();
        System.out.print("Correo: ");
        String correo = scanner.nextLine().trim();
        System.out.println("Programa: 1. Curso | 2. Diplomado | 3. Bootcamp");
        System.out.print("Opcion (1-3): ");
        String tipo = scanner.nextLine().trim();

        ProgramaAcademico prog;
        if (tipo.equals("1")) prog = new Curso();
        else if (tipo.equals("2")) prog = new Diplomado();
        else if (tipo.equals("3")) prog = new Bootcamp();
        else {
            System.out.println("Opcion invalida.");
            return;
        }

        alumnos.put(cedula, new Alumno(cedula, nombre, correo, prog));
        guardarAlumnosTxt();
        System.out.println("Alumno registrado y guardado en disco");
    }

    public void registrarProfesor(Scanner scanner) {
        System.out.println("\n--- 2. Registrar profesor ---");
        System.out.print("Cedula/ID: ");
        String cedula = scanner.nextLine().trim();
        System.out.print("Nombre completo: ");
        String nombre = scanner.nextLine().trim();
        System.out.print("Correo: ");
        String correo = scanner.nextLine().trim();
        System.out.print("Especialidad: ");
        String especialidad = scanner.nextLine().trim();
        System.out.print("Materia: ");
        String materia = scanner.nextLine().trim();

        profesores.add(new Profesor(cedula, nombre, correo, especialidad, materia));
        guardarProfesoresTxt();
        System.out.println("Profesor registrado y guardado en disco");
    }

    public void registrarNota(Scanner scanner) {
        System.out.println("\n--- 3. Registrar notas a un alumno ---");
        System.out.print("Ingrese cedula del alumno: ");
        String cedula = scanner.nextLine().trim();

        if (!alumnos.containsKey(cedula)) {
            System.out.println("Error: alumno no encontrado.");
            return;
        }

        Alumno alumno = alumnos.get(cedula);
        if (alumno.getNotas().size() >= 3) {
            System.out.println("El alumno " + alumno.getNombreCompleto() + " ya posee las 3 notas maximas registradas.");
            return;
        }

        System.out.println("\nRegistrando notas para: " + alumno.getNombreCompleto());
        System.out.println("Notas actuales: " + alumno.getNotas() + " (maximo 3)");
        System.out.println("Escriba 's' o 'salir' para regresar al menu principal.");

        while (alumno.getNotas().size() < 3) {
            int numNota = alumno.getNotas().size() + 1;
            System.out.print("Ingrese Nota " + numNota + " (0-20): ");
            String entrada = scanner.nextLine().trim().toLowerCase();

            if (entrada.equals("s") || entrada.equals("salir")) {
                System.out.println("Finalizando ingreso de notas.");
                break;
            }

            try {
                double nota = Double.parseDouble(entrada);
                if (nota >= 0 && nota <= 20) {
                    alumno.agregarNota(nota);
                    pilaNotas.push(new RegistroNota(cedula, nota)); // Pila LIFO
                    guardarAlumnosTxt();
                    System.out.println("-> Nota " + numNota + " (" + nota + ") registrada con exito.");
                } else {
                    System.out.println("Error: la calificacion debe estar entre 0 y 20.");
                }
            } catch (NumberFormatException e) {
                System.out.println("Error: ingrese un numero valido o 's' para salir.");
            }
        }

        if (alumno.getNotas().size() == 3) {
            System.out.println("\nSe han completado las 3 notas de " + alumno.getNombreCompleto() + ".");
        }
    }

    public void deshacerUltimaNota() {
        System.out.println("\n--- 4. Deshacer ultimo registro (LIFO) ---");
        if (pilaNotas.isEmpty()) {
            System.out.println("No hay notas recientes para deshacer.");
            return;
        }

        RegistroNota accion = pilaNotas.pop();
        String ced = accion.cedula;
        if (alumnos.containsKey(ced) && !alumnos.get(ced).getNotas().isEmpty()) {
            List<Double> notas = alumnos.get(ced).getNotas();
            double removida = notas.remove(notas.size() - 1);
            guardarAlumnosTxt();
            System.out.println("Exito: se elimino la ultima nota (" + removida + ") del alumno con cedula " + ced + ".");
        }
    }

    public void generarColaCertificados() {
        System.out.println("\n--- 5. Generar cola de certificados (FIFO) ---");
        colaCertificados.clear();

        for (Alumno al : alumnos.values()) {
            if (al.estaAprobado()) {
                colaCertificados.add(al);
            }
        }

        int total = colaCertificados.size();
        try (PrintWriter pw = new PrintWriter(new FileWriter(FILE_CERTIFICADOS))) {
            pw.println("=========================================");
            pw.println("   REPORTE DE CERTIFICADOS PENDIENTES    ");
            pw.println("=========================================");
            pw.println("Total de graduandos en cola: " + total + "\n");

            int i = 1;
            while (!colaCertificados.isEmpty()) {
                Alumno al = colaCertificados.poll(); // Desencola (FIFO)
                pw.println(i + ". [" + al.getCedula() + "] " + al.getNombreCompleto());
                pw.println("   - Programa: " + al.getPrograma().getNombre());
                pw.println("   - Promedio final: " + String.format("%.1f", al.calcularPromedio()));
                pw.println("   - Estatus: APROBADO\n");
                i++;
            }
            pw.println("=========================================");
            pw.println("* Fin del reporte - Generado por SGA-DO *");

            System.out.println("Reporte exportado exitosamente a '" + FILE_CERTIFICADOS + "'.");
        } catch (IOException e) {
            System.out.println("Error al generar reporte: " + e.getMessage());
        }
    }

    public void mostrarReporteGeneral() {
        System.out.println("\n=========================================");
        System.out.println("          REPORTE GENERAL SGA-DO         ");
        System.out.println("=========================================");
        System.out.println("\n--- PROFESORES ACTIVOS ---");
        if (profesores.isEmpty()) {
            System.out.println("No hay profesores registrados.");
        }
        for (Profesor p : profesores) {
            System.out.println("[" + p.getCedula() + "] " + p.getNombreCompleto() + " | Esp: " + p.getEspecialidad() + " | Mat: " + p.getMateria());
        }

        System.out.println("\n--- ALUMNOS REGISTRADOS ---");
        if (alumnos.isEmpty()) {
            System.out.println("No hay alumnos registrados.");
        }
        for (Alumno a : alumnos.values()) {
            String estatus = a.estaAprobado() ? "APROBADO" : "REPROBADO";
            System.out.println("[" + a.getCedula() + "] " + a.getNombreCompleto() + " | Prog: " + a.getPrograma().getNombre() + " | Notas: " + a.getNotas() + " | Prom: " + String.format("%.1f", a.calcularPromedio()) + " | Estatus: " + estatus);
        }
        System.out.println("=========================================");
    }
}