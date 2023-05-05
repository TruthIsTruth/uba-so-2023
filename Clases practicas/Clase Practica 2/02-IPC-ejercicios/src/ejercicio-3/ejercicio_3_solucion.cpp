/**
 * Escriba un programa que cuente los números pares en el rango de 2 a mil
 *millones. El programa dividirá el cálculo entre varios procesos, ejecutándose
 *en paralelo. El número de procesos debe especificarse en la línea de comando.
 * El programa creará el número especificado de procesos y dividirá el rango
 * de números en subrangos y asignará un subrango a cada proceso.
 * El proceso primario informará a cada proceso secundario el subrango que le
 *corresponde usando pipes. Además, los procesos secundarios también deben usar
 *pipes para enviar sus recuentos al proceso primario. El padre debe leer el
 *recuento de cada hijo y debe generar el número total de pares.
 **/

#include <sys/wait.h>
#include <unistd.h>
#include <chrono>
#include <iostream>

#define RANGO_MIN 2L
#define RANGO_MAX 1000000001L

using namespace std;

int procesos;

bool esPar(long numero) {
  return (numero & 1) == 0;
}

long contarPares(long minimo, long maximo) {
  long cantidad = 0;
  for (long i = minimo; i < maximo; ++i) {
    if (esPar(i)) {
      cantidad++;
    }
  }
  return cantidad;
}

void cerrarPipes(int pipes[][2], int i) {
  for (int j = 0; j < procesos; ++j) {
    if (j != i) {
      close(pipes[j][0]);
      close(pipes[j][1]);
    }
  }
}

void ejecutarHijo(int pipes[][2], int i) {
  // ¿Por qué hay que cerrar los pipes que no vayamos a usar?
  cerrarPipes(pipes, i);

  long minimo;
  if (read(pipes[i][0], &minimo, sizeof(minimo)) == -1) {
    cout << "No se puede leer el pipe " << i << endl;
  }
  long maximo;
  if (read(pipes[i][0], &maximo, sizeof(maximo)) == -1) {
    cout << "No se puede leer el pipe " << i << endl;
  }

  // cout << i << ": minimo " << minimo << " maximo " << maximo << endl;

  long totalPares = contarPares(minimo, maximo);
  if (write(pipes[i][1], &totalPares, sizeof(totalPares)) == -1) {
    cout << "No se puede escribir el pipe " << i << endl;
  }
}

int main(int argc, char const* argv[]) {
  // Contar el tiempo de ejecución del programa
  auto start = chrono::steady_clock::now();

  // Parseamos los parámetros
  if (argc != 2) {
    cout << "Debe ejecutar con la cantidad de procesos N como parámetro. "
            "Ejemplo:"
         << endl;
    cout << argv[0] << " N" << endl;
    return 1;
  }
  procesos = atoi(argv[1]);
  int pipes[procesos][2];

  // Creamos los N pipes
  for (int i = 0; i < procesos; ++i) {
    if (pipe(pipes[i]) == -1) {
      cout << "No se puede crear el pipe." << endl;
    }
  }

  // Creamos los N hijos
  for (int i = 0; i < procesos; ++i) {
    int pid = fork();
    if (pid == -1) {
      cout << "No se forkear el proceso." << endl;
      return 1;
    }

    if (pid == 0) {
      ejecutarHijo(pipes, i);
      return 0;
    }
  }

  // El intervalo es: [RANGO_MIN, RANGO_MAX) (es decir, cerrado-abierto)
  long cantidad = ((RANGO_MAX - RANGO_MIN) + (procesos - 1)) / procesos;
  long inicio_rango = RANGO_MIN;
  for (int i = 0; i < procesos; ++i) {
    long fin_rango = min(inicio_rango + cantidad, RANGO_MAX);
    if (write(pipes[i][1], &inicio_rango, sizeof(inicio_rango)) == -1) {
      cout << "No se puede escribir el pipe " << i << endl;
    }
    if (write(pipes[i][1], &fin_rango, sizeof(fin_rango)) == -1) {
      cout << "No se puede escribir el pipe " << i << endl;
    }
    inicio_rango += cantidad;
  }

  // ¿Qué pasa si no hacemos wait?
  for (int i = 0; i < procesos; ++i) {
    wait(NULL);
  }

  // Recolectamos resultados
  long resultado = 0;
  for (int i = 0; i < procesos; ++i) {
    long resultado_i;
    if (read(pipes[i][0], &resultado_i, sizeof(resultado_i)) == -1) {
      cout << "No se puede leer el pipe " << i << endl;
    }
    // cout << "Resultado parcial para i " << i << " = " << resultado_i << endl;
    resultado += resultado_i;
  }

  // Imprimimos resultado total
  cout << "Resultado total: " << resultado << endl;
  auto end = chrono::steady_clock::now();
  cout << "Tiempo total (ms): "
       << chrono::duration_cast<chrono::milliseconds>(end - start).count()
       << " ms" << endl;

  return 0;
}
