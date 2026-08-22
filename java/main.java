import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        GestionAcademica sistema = new GestionAcademica();
        Scanner scanner = new Scanner(System.in);

        while (true) {
            System.out.println("\n=========================================");
            System.out.println("    SGA-DO: SISTEMA DIPLOMADOSONLINE     ");
            System.out.println("=========================================");
            System.out.println("1. Registrar alumno");
            System.out.println("2. Registrar profesor");
            System.out.println("3. Registrar notas a un alumno");
            System.out.println("4. Deshacer ultimo registro de nota");
            System.out.println("5. Generar cola de certificados");
            System.out.println("6. Mostrar reporte general");
            System.out.println("7. Salir");
            System.out.println("=========================================");
            System.out.print("Seleccione una opcion (1-7): ");

            String opcion = scanner.nextLine().trim();

            switch (opcion) {
                case "1":
                    sistema.registrarAlumno(scanner);
                    pausa(scanner);
                    break;
                case "2":
                    sistema.registrarProfesor(scanner);
                    pausa(scanner);
                    break;
                case "3":
                    sistema.registrarNota(scanner);
                    pausa(scanner);
                    break;
                case "4":
                    sistema.deshacerUltimaNota();
                    pausa(scanner);
                    break;
                case "5":
                    sistema.generarColaCertificados();
                    pausa(scanner);
                    break;
                case "6":
                    sistema.mostrarReporteGeneral();
                    pausa(scanner);
                    break;
                case "7":
                    System.out.println("\nGuardando datos de forma segura.");
                    scanner.close();
                    return;
                default:
                    System.out.println("\nOpción invalida.");
                    pausa(scanner);
                    break;
            }
        }
    }

    private static void pausa(Scanner scanner) {
        System.out.print("\nPresione ENTER para continuar.");
        scanner.nextLine();
    }
}