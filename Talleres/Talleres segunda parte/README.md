Para los talleres necesitamos que todo el alumnado use el mismo entorno de desarrollo. Así nos aseguramos de que tengan todas las dependencias necesarias instaladas,  que tengan las versiones correctas del compilador y del sistema operativo. Además, necesitamos que trabajen dentro de una máquina virtual porque, como van a estar trabajando con drivers, al cometer un error podrían fácilmente colgar la máquina en la que trabajen. Cargar los drivers dentro de la VM les permite fácilmente reiniciarla sin perder los archivos con los que venían trabajando.

### Para trabajar en las PCs de los labos del 0+inf

Si usan las PCs de los laboratorios, solamente deben ejecutar esta línea para configurar el entorno de trabajo:

```
wget https://bit.ly/vm-talleres-ssoo -O - | bash
```

Cuando el script termine de ejecutar, va a imprimir por pantalla indicaciones.
### Para trabajar desde sus PCs personales

Testeado solamente en Linux, MacOS y Windows con arquitectura Intel. Si tenés una compu que no tenga arquitectura Intel (por ejemplo las últimas Mac con Apple Silicon), esto NO va a funcionar así que usá una PC de los labos o consultá a la lista de docentes.

Necesitás primero tener algunas dependencias instaladas:

* VirtualBox 7.0 (En Linux es recomendable usar la version de VirtualBox 6.1)
* Vagrant
* git
* En Linux además instalar:
  * curl (`sudo apt install curl` en Ubuntu)
  * bsdtar (`sudo apt install libarchive-tools` en Ubuntu)

Luego, deben hacer lo siguiente:

1) Clonar el repositorio del taller:

```
git clone https://github.com/sistemasoperativosdcuba/talleres.git talleres-ssoo
```

2) Ubicarse con la terminal en el directorio donde clonaron el repo:

```
cd talleres-ssoo
```

3) Ubicarse en el directorio del taller que se quiera trabajar. Por ejemplo:

```
cd taller3-drivers
```

4) Ejecutar:

```
vagrant up
```

5) Esperar un rato (~10 min) porque descarga una imagen de VM, instala paquetes en ella y la inicia.

6) Cuando el comando termine, ejecutar:

```
vagrant ssh
```

Y eso nos deja con la terminal abierta dentro de la VM. Si van al directorio `/vagrant` encontrarán los archivos del directorio actual allí montados, con lo cual pueden abrir y editar los archivos del taller con una IDE o editor de textos gráfico en su PC y compilar y ejecutar dentro de la VM.

Para poder salir del entorno de Vagrant, puede ejecutar:

```
exit
```

y luego apagar el entorno con el comando:

```
vagrant halt
```
