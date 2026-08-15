from abc import ABC, abstractmethod

class ProgramaAcademico(ABC):
    def __init__(self, nombre: str):
        self._nombre = nombre

    @property
    def nombre(self):
        return self._nombre

    @abstractmethod
    def evaluar_aprobacion(self, notas: list) -> bool:
        """Metodo polimorfico que cada subclase implementara."""
        pass


class Curso(ProgramaAcademico):
    def __init__(self):
        super().__init__("Curso")

    def evaluar_aprobacion(self, notas: list) -> bool:
        if not notas or len(notas) < 3:
            return False
        promedio = sum(notas) / len(notas)
        return promedio >= 10.0  


class Diplomado(ProgramaAcademico):
    def __init__(self):
        super().__init__("Diplomado")

    def evaluar_aprobacion(self, notas: list) -> bool:
        if not notas or len(notas) < 3:
            return False
        promedio = sum(notas) / len(notas)
        return promedio >= 14.0  


class Bootcamp(ProgramaAcademico):
    def __init__(self):
        super().__init__("Bootcamp")

    def evaluar_aprobacion(self, notas: list) -> bool:
        if not notas or len(notas) < 3:
            return False
        
        return all(nota >= 14.0 for nota in notas)


class Persona:
    def __init__(self, cedula: str, nombre_completo: str, correo: str):
        self._cedula = cedula
        self._nombre_completo = nombre_completo
        self._correo = correo

    @property
    def cedula(self): return self._cedula

    @property
    def nombre_completo(self): return self._nombre_completo

    @property
    def correo(self): return self._correo


class Alumno(Persona):
    def __init__(self, cedula: str, nombre_completo: str, correo: str, programa: ProgramaAcademico):
        super().__init__(cedula, nombre_completo, correo)
        self.programa = programa
        self.notas = []  

    def agregar_nota(self, nota: float):
        if len(self.notas) < 3:
            self.notas.append(nota)
            return True
        return False

    def calcular_promedio(self) -> float:
        if not self.notas:
            return 0.0
        return sum(self.notas) / len(self.notas)

    def esta_aprobado(self) -> bool:
        return self.programa.evaluar_aprobacion(self.notas)


class Profesor(Persona):
    def __init__(self, cedula: str, nombre_completo: str, correo: str, especialidad: str, materia: str):
        super().__init__(cedula, nombre_completo, correo)
        self.especialidad = especialidad
        self.materia = materia