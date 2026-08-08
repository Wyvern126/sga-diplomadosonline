import os
from sistema import GestionAcademica

def limpiar_pantalla():
    # Limpia la terminal según el sistema operativo (cls para Windows, clear para Mac/Linux)
    os.system('cls' if os.name == 'nt' else 'clear')

def main():
    sistema = GestionAcademica()

    while True:
        limpiar_pantalla()
        print("=========================================")
        print("    SGA-DO: SISTEMA DIPLOMADOSONLINE     ")
        print("=========================================")
        print("1. Registrar Alumno")
        print("2. Registrar Profesor")
        print("3. Registrar Notas a un Alumno")
        print("4. Deshacer Último Registro de Nota")
        print("5. Generar Cola de Certificados")
        print("6. Mostrar Reporte General")
        print("7. Salir")
        print("=========================================")

        opcion = input("Seleccione una opción (1-7): ").strip()

        try:
            if opcion == "1":
                sistema.registrar_alumno()
                input("\nPresione ENTER para continuar...")
            elif opcion == "2":
                sistema.registrar_profesor()
                input("\nPresione ENTER para continuar...")
            elif opcion == "3":
                sistema.registrar_nota()
                input("\nPresione ENTER para continuar...")
            elif opcion == "4":
                sistema.deshacer_ultima_nota()
                input("\nPresione ENTER para continuar...")
            elif opcion == "5":
                sistema.generar_cola_certificados()
                input("\nPresione ENTER para continuar...")
            elif opcion == "6":
                sistema.mostrar_reporte_general()
            elif opcion == "7":
                print("\nGuardando datos de forma segura. ¡Hasta luego!")
                break
            else:
                input("\nOpción inválida. Presione ENTER para intentar de nuevo...")
        except Exception as e:
            input(f"\nError de ejecución: {e}. Presione ENTER para continuar...")

if __name__ == "__main__":
    main()