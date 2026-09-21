# 03 · Firmware: carga y verificación

> Fuente: `vendor/DockEye/Firmware/Open_Fluorometer/DockEye.ino` y
> `vendor/DockEye/Firmware/README.md`. El firmware es UN solo archivo .ino.

## 1. Qué hace el firmware (para entender qué esperar)

Al encender, el aparato ejecuta **una** secuencia completa y se queda quieto:

1. Inicializa sensor AS7341, RTC y driver AW9523. Si alguno no responde, **se frena** e imprime el error por el puerto serie.
2. Crea/abre `Test.csv` en la SD y escribe el encabezado.
3. Parpadea el LED indicador 2 veces → **fase clorofila**: enciende el LED azul 60 segundos, midiendo y grabando continuamente los 10 canales del sensor.
4. Parpadea 3 veces → **fase turbidez**: enciende el LED infrarrojo 60 segundos, ídem.
5. Escribe en la SD un bloque `SUMMARY` con: promedio F8 con LED azul, promedio F8 con LED IR (fondo), la resta, y la clorofila-a calibrada con la ecuación `chla = 0,1332 × diff − 1,7127` (⚠️ esa ecuación es la calibración del aparato de ELLOS: para el nuestro se reemplaza tras calibrar — doc 04).
6. Queda en bucle infinito hasta que se lo apague y prenda de nuevo.

Cada medición vuelca por el puerto serie (9600 baudios) una barra de progreso y los valores crudos: eso es lo que se usa para verificar.

## 2. Preparar el entorno (Arduino IDE en castellano)

1. Instalar **Arduino IDE 2.x** desde arduino.cc (Archivo → Preferencias → Idioma: Español).
2. Agregar soporte ESP32: Archivo → Preferencias → *URLs adicionales de gestor de placas*, pegar:
   `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
3. Herramientas → Placa → Gestor de placas → buscar **esp32** (por Espressif Systems) → Instalar (versión 3.x).
4. Instalar librerías: Herramientas → Administrar bibliotecas, buscar e instalar:

| Librería | Autor | Nota |
|---|---|---|
| **Adafruit AS7341** | Adafruit | instala solas sus dependencias (BusIO, Sensor) — aceptar "Instalar todas" |
| **Adafruit AW9523** | Adafruit | solo si se usa el AW9523 |
| **RTClib** | Adafruit | maneja PCF8523 y DS3231 |
| **SdFat - Adafruit Fork** | Bill Greiman | ⚠️ elegir el **fork de Adafruit**, no el SdFat original |

   El repo no fija versiones exactas (**A VERIFICAR** solo si algo no compila: probar la última estable de cada una; el firmware es simple y usa APIs básicas).

5. Selección de placa:
   - Feather original: Herramientas → Placa → esp32 → **Adafruit Feather ESP32-S3 2MB PSRAM** (o la variante que corresponda al modelo comprado).
   - Devkit genérico: **ESP32S3 Dev Module**.
   - En ambos: `USB CDC On Boot: Enabled` (para que se vea el monitor serie por USB).

## 3. Ajustes al .ino según el hardware armado

Abrir `vendor/DockEye/Firmware/Open_Fluorometer/DockEye.ino` y guardarlo como copia de trabajo (ej. `firmware-sonda/sonda.ino` — no editar el vendor). Ajustes:

### 3.1 Si se armó la versión original (Feather+Adalogger+AW9523)
Nada que tocar. Flashear tal cual.

### 3.2 Si se usa RTC DS3231 en vez del Adalogger
```cpp
// línea 8:  RTC_PCF8523 rtc;   →
RTC_DS3231 rtc;
```
y más abajo, `rtc.initialized()` no existe para DS3231: reemplazar ese chequeo por `rtc.lostPower()`.

### 3.3 Si el CS de la microSD no es el pin 10
```cpp
// línea 20: const int SD_CHIP_SELECT = 10;  →
const int SD_CHIP_SELECT = <GPIO elegido>;
```

### 3.4 Si NO se usa AW9523 (LEDs por GPIO + transistor)
Hay que reemplazar el manejo de LEDs: quitar `Adafruit_AW9523.h`, el objeto `aw`, y las llamadas `aw.pinMode/aw.analogWrite`, por `pinMode(GPIO, OUTPUT)` y `digitalWrite(GPIO, HIGH/LOW)` sobre los GPIO elegidos para LED azul, LED IR y LED indicador. Son ~6 puntos de edición, todos evidentes buscando `aw.` en el archivo. Mantener la lógica idéntica (mismos tiempos, mismas fases).

### 3.5 Para uso en campo sin PC conectada
El `setup()` arranca con:
```cpp
while (!Serial) { delay(1); }
```
Eso **espera a que haya un monitor serie abierto**: conectado a un power bank, puede quedarse esperando para siempre. Para uso autónomo, comentar esas líneas (o encerrarlas en un timeout). **A VERIFICAR en banco**: probar el aparato con power bank y confirmar que arranca solo. Para todas las pruebas del doc 05, dejarlo como está (se trabaja con la PC conectada).

## 4. Flasheo

1. Conectar la placa por USB-C (cable de datos).
2. Seleccionar el puerto: Herramientas → Puerto (aparece al conectar; en Windows es COMx, en Mac /dev/cu.usbmodem…).
3. Botón **Subir** (flecha →). Si la placa no entra sola en modo de programación: mantener **BOOT**, tocar **RESET**, soltar BOOT, reintentar.
4. Éxito = "Hard resetting via RTS pin..." o similar, sin errores rojos.

## 5. Primera verificación (con el monitor serie)

Herramientas → Monitor Serie, **9600 baudios**. Apretar RESET en la placa. Debe verse, en orden:

1. Ningún mensaje de error. Los errores posibles y su significado:
   - `Could not find AS7341` → revisar cableado I²C del sensor (SDA/SCL invertidos es el clásico) o dirección.
   - `Couldn't find RTC` → cableado I²C del RTC o pila faltante.
   - `AW9523 not found? Check wiring!` → ídem para el driver.
   - `Error opening file!` → SD ausente, mal formateada (usar FAT32) o CS incorrecto.
2. `AW9523 found!` (si aplica).
3. `Begin chlorophyll measurements.` + LED azul encendido (se ve por la ranura de la cubeta si la tapa está abierta) + barra de progreso `Chlorophyll-a: [###...] 42%` + líneas `Raw F8: <número>`.
4. Al minuto: `End chlorophyll measurements.` → `Begin turbidity measurements.` + LED IR encendido (⚠️ el IR es invisible al ojo; se ve **con la cámara del celular**, que lo capta violáceo) + líneas `Raw NIR: <número>`.
5. Al final: bloque `==== FINAL AVERAGE RESULTS ====` con los promedios.

6. Apagar, sacar la microSD, abrirla en la PC: debe existir `Test.csv` con encabezado, ~2 minutos de filas de datos y el bloque `SUMMARY` al final. ⚠️ El firmware **agrega** al mismo `Test.csv` en cada corrida: para cada ensayo nuevo conviene borrar o renombrar el archivo previo.

## 6. Ajuste del RTC (fecha y hora)

El firmware original **no pone en hora el RTC** (solo avisa si no corre). Para ponerlo en hora una única vez: cargar el ejemplo `RTClib → pcf8523` (o `ds3231`) del IDE, que ajusta el reloj a la hora de compilación, y después volver a flashear el firmware de la sonda. Con la pila puesta, la hora se conserva.

## 7. Parámetros del sensor (no tocar sin motivo)

El firmware fija: `ATIME=255`, `ASTEP=999`, ganancia `256X` → tiempo de integración ≈ 0,7 s por lectura. Son los valores con los que DockEye calibró. Si el sensor **satura** (valores clavados en 65535) o queda sin señal, se ajusta la ganancia — pero eso invalida comparaciones con datos previos: anotar SIEMPRE cualquier cambio en la hoja del instrumento (el CSV graba ATIME/ASTEP/GAIN en cada fila, lo que permite auditar).
