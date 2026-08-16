import os
from collections import deque
from modelos import Alumno, Profesor, Curso, Diplomado, Bootcamp

class GestionAcademica:
    def __init__(self):
        self.alumnos = {}     
        self.profesores = []   
        self.pila_notas = []   
        self.cola_certificados = deque() 

        self.file_alumnos = "alumnos.txt"
        self.file_profesores = "profesores.txt"
        self.file_certificados = "certificados_pendientes.txt"

        self._cargar_datos_iniciales()

    def _instanciar_programa(self, nombre_prog: str):
        nombre_prog = nombre_prog.strip().capitalize()
        if nombre_prog == "Curso": return Curso()
        elif nombre_prog == "Diplomado": return Diplomado()
        elif nombre_prog == "Bootcamp": return Bootcamp()
        return Curso()

    def _cargar_datos_iniciales(self):
        if os.path.exists(self.file_alumnos):
            with open(self.file_alumnos, "r", encoding="utf-8") as f:
                for line in f:
                    data = [x.strip() for x in line.strip().split(",")]
                    if len(data) >= 4:
                        ced, nom, cor, prog_nom = data[0], data[1], data[2], data[3]
                        alumno = Alumno(ced, nom, cor, self._instanciar_programa(prog_nom))
                        if len(data) > 4:
                            for n in data[4:]:
                                try:
                                    val = float(n)
                                    if val > 0: alumno.agregar_nota(val)
                                except ValueError: pass
                        self.alumnos[ced] = alumno

        if os.path.exists(self.file_profesores):
            with open(self.file_profesores, "r", encoding="utf-8") as f:
                for line in f:
                    data = [x.strip() for x in line.strip().split(",")]
                    if len(data) >= 5:
                        self.profesores.append(Profesor(data[0], data[1], data[2], data[3], data[4]))

    def _guardar_alumnos_txt(self):
        with open(self.file_alumnos, "w", encoding="utf-8") as f:
            for al in self.alumnos.values():
                notas_str = ",".join(str(n) for n in al.notas) if al.notas else "0,0,0"
                f.write(f"{al.cedula}, {al.nombre_completo}, {al.correo}, {al.programa.nombre}, {notas_str}\n")

    def _guardar_profesores_txt(self):
        with open(self.file_profesores, "w", encoding="utf-8") as f:
            for pr in self.profesores:
                f.write(f"{pr.cedula}, {pr.nombre_completo}, {pr.correo}, {pr.especialidad}, {pr.materia}\n")

    def registrar_alumno(self):
        print("\n--- 1. Registrar alumno ---")
        cedula = input("Cedula/ID: ").strip()
        if cedula in self.alumnos:
            print("Error: ya existe un alumno con esa cedula.")
            return
        nombre = input("Nombre completo: ").strip()
        correo = input("Correo: ").strip()
        print("Programa: 1. Curso | 2. Diplomado | 3. Bootcamp")
        tipo = input("Opcion (1-3): ").strip()

        if tipo == "1": prog = Curso()
        elif tipo == "2": prog = Diplomado()
        elif tipo == "3": prog = Bootcamp()
        else:
            print("Opcion invalida.")
            return

        self.alumnos[cedula] = Alumno(cedula, nombre, correo, prog)
        self._guardar_alumnos_txt()
        print("Alumno registrado y guardado en disco")

    def registrar_profesor(self):
        print("\n--- 2. Registrar profesor ---")
        cedula = input("Cedula/ID: ").strip()
        nombre = input("Nombre completo: ").strip()
        correo = input("Correo: ").strip()
        especialidad = input("Especialidad: ").strip()
        materia = input("Materia: ").strip()

        self.profesores.append(Profesor(cedula, nombre, correo, especialidad, materia))
        self._guardar_profesores_txt()
        print("Profesor registrado y guardado en disco")

    def registrar_nota(self):
        print("\n--- 3. Registrar notas a un alumno ---")
        cedula = input("Ingrese cedula del alumno: ").strip()
        if cedula not in self.alumnos:
            print("Error: alumno no encontrado.")
            return

        alumno = self.alumnos[cedula]
        
        if len(alumno.notas) >= 3:
            print(f"El alumno {alumno.nombre_completo} ya posee las 3 notas maximas registradas.")
            return

        print(f"\nRegistrando notas para: {alumno.nombre_completo}")
        print(f"Notas actuales: {alumno.notas} (maximo 3)")
        print("Escriba 's' o 'salir' para regresar al menú principal.")

        while len(alumno.notas) < 3:
            num_nota = len(alumno.notas) + 1
            entrada = input(f"Ingrese nota {num_nota} (0-20): ").strip().lower()

            if entrada in ["s", "salir"]:
                print("Finalizando ingreso de notas.")
                break

            try:
                nota = float(entrada)
                if 0 <= nota <= 20:
                    alumno.agregar_nota(nota)
                    self.pila_notas.append({"cedula": cedula, "nota": nota})
                    self._guardar_alumnos_txt()
                    print(f"Nota {num_nota} ({nota}) registrada con éxito.")
                else:
                    print("Error: La calificacion debe estar entre 0 y 20.")
            except ValueError:
                print("Error: ingrese un numero valido o 's' para salir.")

        if len(alumno.notas) == 3:
            print(f"\nSe han completado las 3 notas de {alumno.nombre_completo}")

    def deshacer_ultima_nota(self):
        print("\n--- 4. Deshacer ultimo registro (LIFO) ---")
        if not self.pila_notas:
            print("No hay notas recientes para deshacer.")
            return

        accion = self.pila_notas.pop()
        ced = accion["cedula"]
        if ced in self.alumnos and self.alumnos[ced].notas:
            removida = self.alumnos[ced].notas.pop()
            self._guardar_alumnos_txt()
            print(f"Exito: se elimino la última nota ({removida}) del alumno {ced}.")

    def generar_cola_certificados(self):
        print("\n--- 5. Generar cola de certificados (FIFO) ---")
        self.cola_certificados.clear()

        for al in self.alumnos.values():
            if al.esta_aprobado():
                self.cola_certificados.append(al)

        total = len(self.cola_certificados)
        with open(self.file_certificados, "w", encoding="utf-8") as f:
            f.write("=========================================\n")
            f.write("   REPORTE DE CERTIFICADOS PENDIENTES    \n")
            f.write("=========================================\n")
            f.write(f"Total de graduandos en cola: {total}\n\n")

            i = 1
            while self.cola_certificados:
                al = self.cola_certificados.popleft()
                f.write(f"{i}. [{al.cedula}] {al.nombre_completo}\n")
                f.write(f"   - Programa: {al.programa.nombre}\n")
                f.write(f"   - Promedio Final: {al.calcular_promedio():.1f}\n")
                f.write(f"   - Estatus: APROBADO\n\n")
                i += 1
            f.write("=========================================\n")
            f.write("* Fin del reporte - Generado por SGA-DO *\n")

        print(f"Reporte exportado exitosamente a '{self.file_certificados}'.")

    def mostrar_reporte_general(self):
        print("\n=========================================")
        print("          REPORTE GENERAL SGA-DO         ")
        print("=========================================")
        print("\n--- PROFESORES ACTIVOS ---")
        if not self.profesores:
            print("No hay profesores registrados.")
        for p in self.profesores:
            print(f"[{p.cedula}] {p.nombre_completo} | Esp: {p.especialidad} | Mat: {p.materia}")

        print("\n--- ALUMNOS REGISTRADOS ---")
        if not self.alumnos:
            print("No hay alumnos registrados.")
        for a in self.alumnos.values():
            estatus = "APROBADO" if a.esta_aprobado() else "REPROBADO"
            print(f"[{a.cedula}] {a.nombre_completo} | Prog: {a.programa.nombre} | Notas: {a.notas} | Prom: {a.calcular_promedio():.1f} | Estatus: {estatus}")
        print("=========================================")
        
        input("\nPresione ENTER para regresar al menú principal...")