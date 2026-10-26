# 03 · Firmware: carga y verificación

> Fuente: `vendor/DockEye/Firmware/DockEye/DockEye_stable/DockEye_050226.ino`.
> El firmware es UN solo archivo .ino.
>
> **Actualización 2026-10-26:** COAST-Lab publicó el firmware que usan en campo
> desde hace meses (`DockEye_050226.ino`). Corrige un error en la lógica de
> medición de fondo del firmware original (que estimaba el fondo de la clorofila
> con el LED IR encendido) agregando una **fase de fondo a oscuras** propia de
> cada corrida. El firmware viejo quedó en `.../DockEye_stable/old/` — **no
> usarlo**. Fuente: respuesta de Adam Lucas (COAST-Lab) en el
> [issue #11](https://github.com/COAST-Lab/DockEye/issues/11).

## 1. Qué hace el firmware (para entender qué esperar)

Al encender, el aparato ejecuta **una** secuencia completa (~3 minutos) y se queda quieto:

1. Inicializa sensor AS7341, RTC y driver AW9523. Si alguno no responde, **se frena** e imprime el error por el puerto serie. Si el RTC está sin hora (primera vez o pila agotada), lo pone automáticamente a la hora de compilación del firmware.
2. Crea/abre en la SD un CSV con la fecha del día (`Test AAAA-MM-DD.csv`) y escribe el encabezado. Las corridas del mismo día se agregan al mismo archivo.
3. **Fase fondo**: 60 segundos con **todos los LEDs apagados**, midiendo y grabando continuamente los 10 canales del sensor. Es la medición de luz ambiente que se resta después; con la tapa puesta debería dar ~0 cuentas (salvo sol directo).
4. **Fase clorofila**: LED azul encendido 60 segundos, ídem.
5. **Fase turbidez**: LED infrarrojo encendido 60 segundos, ídem.
6. Escribe en la SD un bloque `SUMMARY` con, para cada variable, el promedio crudo, el promedio del fondo, la resta, y el valor calibrado: `chla = 0,0871 × diff − 1,3054` y `turbidez (NTU) = 0,0081 × diff − 11,787` (⚠️ esas ecuaciones son la calibración del aparato de ELLOS: para el nuestro se reemplazan tras calibrar — doc 04). Enciende el LED indicador.
7. Queda en bucle infinito hasta que se lo apague y prenda de nuevo.

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

Abrir `vendor/DockEye/Firmware/DockEye/DockEye_stable/DockEye_050226.ino` y guardarlo como copia de trabajo (ej. `firmware-sonda/sonda.ino` — no editar el vendor). Ajustes:

### 3.1 Si se armó la versión original (Feather+Adalogger+AW9523)
Nada que tocar. Flashear tal cual.

### 3.2 Si se usa RTC DS3231 en vez del Adalogger
```cpp
// RTC_PCF8523 rtc;   →
RTC_DS3231 rtc;
```
y en el `setup()`, `rtc.initialized()` no existe para DS3231: dejar el chequeo solo con `rtc.lostPower()`.

### 3.3 Si el CS de la microSD no es el pin 10
```cpp
// const int SD_CHIP_SELECT = 10;  →
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
3. `Begin background measurements.` + **todos los LEDs apagados** + barra de progreso `Background: [###...] 42%` + líneas `Raw F8:` y `Raw NIR:`. Con la tapa cerrada los valores deben ser ~0; con la tapa abierta suben con la luz del ambiente (sirve como prueba rápida de que la tapa sella la luz).
4. Al minuto: `End background measurements.` → `Begin chlorophyll measurements.` + LED azul encendido (se ve por la ranura de la cubeta si la tapa está abierta) + líneas `Raw F8: <número>`.
5. Al minuto: `End chlorophyll measurements.` → `Begin turbidity measurements.` + LED IR encendido (⚠️ el IR es invisible al ojo; se ve **con la cámara del celular**, que lo capta violáceo) + líneas `Raw NIR: <número>`.
6. Al final: bloque `==== FINAL AVERAGE RESULTS ====` con promedios, fondo restado y valores calibrados de clorofila y turbidez.

7. Apagar, sacar la microSD, abrirla en la PC: debe existir `Test AAAA-MM-DD.csv` (con la fecha del día) con encabezado, ~3 minutos de filas de datos y el bloque `SUMMARY` al final. ⚠️ El firmware **agrega** al archivo del día en cada corrida: entre corridas del mismo día, identificar cada ensayo por su hora (columna de fecha/hora) o renombrar el archivo previo.

## 6. Ajuste del RTC (fecha y hora)

Si el RTC está sin inicializar o perdió la hora (pila agotada), el firmware **lo ajusta solo a la fecha y hora de compilación** del .ino. Alcanza con compilar y flashear con la PC en hora: la primera corrida deja el reloj puesto, y con la pila colocada la hora se conserva. Si el reloj quedara con hora vieja (p. ej. se flasheó un binario compilado días atrás), re-flashear recién compilado.

## 7. Parámetros del sensor (no tocar sin motivo)

El firmware fija: `ATIME=255`, `ASTEP=999`, ganancia `256X` → tiempo de integración ≈ 0,7 s por lectura. Son los valores con los que DockEye calibró. Si el sensor **satura** (valores clavados en 65535) o queda sin señal, se ajusta la ganancia — pero eso invalida comparaciones con datos previos: anotar SIEMPRE cualquier cambio en la hoja del instrumento (el CSV graba ATIME/ASTEP/GAIN en cada fila, lo que permite auditar).
