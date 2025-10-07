#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

class sistema_despacho {
private:
    int* identificadores_paquetes;
    float* pesos_paquetes;
    char** destinos_paquetes;
    int* prioridades_paquetes;
    char* estados_paquetes;

    int capacidad_actual;
    int total_paquetes;
    int indice_frente_cola;
    int indice_fin_cola;
    int indice_tope_pila;

    void expandir_capacidad() {
        int nueva_capacidad = capacidad_actual * 2;
        cout << "\n[EXPANDIENDO] Capacidad llena. Duplicando de "
             << capacidad_actual << " a " << nueva_capacidad << "...\n";

        int* nuevos_identificadores = new int[nueva_capacidad];
        float* nuevos_pesos = new float[nueva_capacidad];
        char** nuevos_destinos = new char*[nueva_capacidad];
        int* nuevas_prioridades = new int[nueva_capacidad];
        char* nuevos_estados = new char[nueva_capacidad];

        for (int indice = 0; indice < total_paquetes; indice++) {
            nuevos_identificadores[indice] = identificadores_paquetes[indice];
            nuevos_pesos[indice] = pesos_paquetes[indice];
            nuevas_prioridades[indice] = prioridades_paquetes[indice];
            nuevos_estados[indice] = estados_paquetes[indice];

            nuevos_destinos[indice] = new char[strlen(destinos_paquetes[indice]) + 1];
            strcpy(nuevos_destinos[indice], destinos_paquetes[indice]);
        }

        for (int indice = 0; indice < total_paquetes; indice++) {
            delete[] destinos_paquetes[indice];
        }
        delete[] identificadores_paquetes;
        delete[] pesos_paquetes;
        delete[] destinos_paquetes;
        delete[] prioridades_paquetes;
        delete[] estados_paquetes;

        identificadores_paquetes = nuevos_identificadores;
        pesos_paquetes = nuevos_pesos;
        destinos_paquetes = nuevos_destinos;
        prioridades_paquetes = nuevas_prioridades;
        estados_paquetes = nuevos_estados;
        capacidad_actual = nueva_capacidad;

        cout << "[OK] Capacidad expandida exitosamente.\n";
    }

public:
    sistema_despacho(int capacidad_inicial = 50) {
        capacidad_actual = capacidad_inicial;
        total_paquetes = 0;
        indice_frente_cola = 0;
        indice_fin_cola = -1;
        indice_tope_pila = -1;

        identificadores_paquetes = new int[capacidad_actual];
        pesos_paquetes = new float[capacidad_actual];
        destinos_paquetes = new char*[capacidad_actual];
        prioridades_paquetes = new int[capacidad_actual];
        estados_paquetes = new char[capacidad_actual];

        cout << "\n Bienvenido al sistema de MegaEnvio: Sistema Integrado con Prioridad \n";
        cout << "Inicializando sistema con capacidad para " << capacidad_actual << " paquetes...\n\n";
    }

    ~sistema_despacho() {
        cout << "\nLiberando " << total_paquetes << " bloques de memoria asignada...\n";
        for (int indice = 0; indice < total_paquetes; indice++) {
            delete[] destinos_paquetes[indice];
        }
        delete[] identificadores_paquetes;
        delete[] pesos_paquetes;
        delete[] destinos_paquetes;
        delete[] prioridades_paquetes;
        delete[] estados_paquetes;
        cout << "Sistema cerrado. !Memoria libre!\n";
    }

    void recibir_paquete(int identificador, float peso, const char* destino, int prioridad) {
        if (prioridad < 1 || prioridad > 3) {
            cout << "[ERROR] Prioridad invalida. Debe ser 1-3.\n";
            return;
        }
        if (peso <= 0) {
            cout << "[ERROR] Peso invalido. Debe ser mayor a 0.\n";
            return;
        }

        if (total_paquetes >= capacidad_actual) {
            expandir_capacidad();
        }

        identificadores_paquetes[total_paquetes] = identificador;
        pesos_paquetes[total_paquetes] = peso;
        prioridades_paquetes[total_paquetes] = prioridad;
        estados_paquetes[total_paquetes] = 'R';

        destinos_paquetes[total_paquetes] = new char[strlen(destino) + 1];
        strcpy(destinos_paquetes[total_paquetes], destino);

        indice_tope_pila = total_paquetes;
        total_paquetes++;

        cout << "Paquete " << identificador << " Recibido. Estado: 'R'. Pila.top_pila = "
             << indice_tope_pila << ".\n";
    }

    void procesar_recepcion() {
        int indice_pop = -1;
        for (int indice = total_paquetes - 1; indice >= 0; indice--) {
            if (estados_paquetes[indice] == 'R') {
                indice_pop = indice;
                break;
            }
        }

        if (indice_pop == -1) {
            cout << "\n[ERROR] No hay paquetes en la Pila de Recepcion para procesar.\n";
            return;
        }

        estados_paquetes[indice_pop] = 'E';

        if (indice_fin_cola == -1) {
            indice_frente_cola = indice_pop;
            indice_fin_cola = indice_pop;
        } else {
            indice_fin_cola = indice_pop;
        }

        indice_tope_pila = -1;
        for (int indice = total_paquetes - 1; indice >= 0; indice--) {
            if (estados_paquetes[indice] == 'R') {
                indice_tope_pila = indice;
                break;
            }
        }

        cout << "\nPOP de Pila: Pkg " << identificadores_paquetes[indice_pop]
             << " transferido a Cola. Estado: 'E'.\n";
        cout << "frente_cola=" << indice_frente_cola
             << ", final_cola=" << indice_fin_cola << ".\n";
    }

    void despachar_con_prioridad() {
        int indice_despacho = -1;
        bool hay_prioridad_alta = false;

        for (int indice = 0; indice < total_paquetes; indice++) {
            if (estados_paquetes[indice] == 'E' && prioridades_paquetes[indice] == 1) {
                indice_despacho = indice;
                hay_prioridad_alta = true;
                break;
            }
        }

        if (!hay_prioridad_alta) {
            for (int indice = 0; indice < total_paquetes; indice++) {
                if (estados_paquetes[indice] == 'E') {
                    indice_despacho = indice;
                    break;
                }
            }
        }

        if (indice_despacho == -1) {
            cout << "\n[ERROR] No hay paquetes en cola para despachar.\n";
            return;
        }

        if (hay_prioridad_alta) {
            cout << "\nAnalizando Cola... Detectada ALTA PRIORIDAD.\n";
        } else {
            cout << "\nDespachando paquete...\n";
        }

        estados_paquetes[indice_despacho] = 'D';

        cout << "DESPACHADO: Paquete " << identificadores_paquetes[indice_despacho]
             << " (Prioridad " << prioridades_paquetes[indice_despacho] << "). Estado: 'D'.\n";

        if (hay_prioridad_alta) {
            cout << "(SALTO DE COLA aplicado)\n";
        }
    }

    void reporte_estados() {
        int contador_recibidos = 0;
        int contador_en_cola = 0;
        int contador_despachados = 0;

        for (int indice = 0; indice < total_paquetes; indice++) {
            switch(estados_paquetes[indice]) {
                case 'R': contador_recibidos++; break;
                case 'E': contador_en_cola++; break;
                case 'D': contador_despachados++; break;
            }
        }

        cout << "\n Reporte de estados \n";
        cout << "Paquetes Recibidos (Pila 'R'): " << contador_recibidos << "\n";
        cout << "Paquetes En Cola ('E'): " << contador_en_cola << "\n";
        cout << "Paquetes Despachados ('D'): " << contador_despachados << "\n";
        cout << "Total de paquetes: " << total_paquetes << "\n";
        cout << "Capacidad actual: " << capacidad_actual << "\n";
        cout << "==========================\n";
    }

    void inspeccionar_frente() {
        for (int indice = 0; indice < total_paquetes; indice++) {
            if (estados_paquetes[indice] == 'E') {
                cout << "\nFrente de la cola:\n";
                cout << "  ID: " << identificadores_paquetes[indice]
                     << " | Peso: " << fixed << setprecision(1) << pesos_paquetes[indice] << " kg"
                     << " | Destino: " << destinos_paquetes[indice]
                     << " | Prioridad: ";

                switch(prioridades_paquetes[indice]) {
                    case 1: cout << "Alta\n"; break;
                    case 2: cout << "Media\n"; break;
                    case 3: cout << "Baja\n"; break;
                }
                return;
            }
        }

        cout << "\n[INFO] La cola esta vacia.\n";
    }

    void reporte_por_destino(const char* destino_buscado) {
        int contador = 0;
        float suma_pesos = 0.0;

        for (int indice = 0; indice < total_paquetes; indice++) {
            if (estados_paquetes[indice] == 'E' && strcmp(destinos_paquetes[indice], destino_buscado) == 0) {
                contador++;
                suma_pesos += pesos_paquetes[indice];
            }
        }

        cout << "\nReporte para destino '" << destino_buscado << "':\n";
        cout << "  Paquetes En Cola: " << contador << "\n";

        if (contador > 0) {
            float promedio = suma_pesos / contador;
            cout << "  Peso Promedio: " << fixed << setprecision(2)
                 << promedio << " kg\n";
        } else {
            cout << "  Peso Promedio: N/A (sin paquetes)\n";
        }
    }
};

void mostrar_menu() {
    cout << "\n Menu Principal \n";
    cout << "1. Recibir Nuevo Paquete (PUSH.0)\n";
    cout << "2. Procesar Recepcion (POP y ENCOLAR)\n";
    cout << "3. Despachar (DESENCOLAR con Prioridad)\n";
    cout << "4. Reporte de Estados\n";
    cout << "5. Inspeccionar Frente de Cola\n";
    cout << "6. Reporte por Destino\n";
    cout << "7. Salir (Liberar Memoria)\n";
    cout << "\nOpcion seleccionada: ";
}

int main() {
    sistema_despacho sistema(50);
    int opcion;

    do {
        mostrar_menu();
        cin >> opcion;
        cin.ignore();

        switch(opcion) {
            case 1: {
                int identificador, prioridad;
                float peso;
                char destino[100];

                cout << "Ingrese ID: ";
                cin >> identificador;
                cout << "Peso (kg): ";
                cin >> peso;
                cout << "Destino: ";
                cin.ignore();
                cin.getline(destino, 100);
                cout << "Prioridad (1=Alta, 2=Media, 3=Baja): ";
                cin >> prioridad;

                sistema.recibir_paquete(identificador, peso, destino, prioridad);
                break;
            }
            case 2:
                sistema.procesar_recepcion();
                break;
            case 3:
                sistema.despachar_con_prioridad();
                break;
            case 4:
                sistema.reporte_estados();
                break;
            case 5:
                sistema.inspeccionar_frente();
                break;
            case 6: {
                char destino[100];
                cout << "Ingrese el destino para el reporte: ";
                cin.getline(destino, 100);
                sistema.reporte_por_destino(destino);
                break;
            }
            case 7:
                cout << "\nCerrando sistema. Vuelva pronto! \n";
                break;
            default:
                cout << "\n[Error!] Opcion invalida. Intente nuevamente.\n";
        }

    } while(opcion != 7);

    return 0;
}
