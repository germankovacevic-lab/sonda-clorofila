# 02 · Guía de armado

> Fuente: README de `vendor/DockEye/` (sección *Wiring Instructions*) + firmware
> `vendor/DockEye/Firmware/Open_Fluorometer/DockEye.ino` + manual de ensamble de
> `vendor/Chlorophyll-Fluorometer/` (ENSSAT) como referencia de buenas prácticas.
>
> El original advierte: *"Instructions for mounting the LEDs and light sensor to
> the cuvette holder will be added soon"* — es decir, **el propio repo DockEye no
> documenta el montaje óptico fino**; esa parte acá está resuelta con criterio
> propio y marcada A VERIFICAR donde corresponde.

## 0. Principio a respetar SIEMPRE

La geometría óptica es el instrumento:

- LED(s) apuntando a una cara de la cubeta.
- Sensor AS7341 mirando la cara **a 90°** (nunca enfrentado al LED: se satura).
- Todo dentro del gabinete **cerrado y opaco** (cámara oscura).

Cualquier luz ambiente que se cuele o cualquier corrimiento mecánico entre
mediciones arruina la repetibilidad.

## 1. Soldadura de módulos

Para cada módulo (ESP32, AS7341, AW9523/driver, RTC, microSD): soldar las tiras
de pines que vienen incluidas. Reglas:

1. Pines macho hacia abajo para pinchar en protoboard (o hembra según el plan de montaje).
2. Soldadura brillante y cónica en cada pin; sin puentes entre pines vecinos.
3. Si se usa el sandwich original Feather + Adalogger: al Feather pines macho, al Adalogger **pines apilables (stacking)**, y el Adalogger se enchufa arriba del Feather (así lo indica el README de DockEye).

## 2. Conexionado eléctrico

Todos los sensores hablan **I²C**: comparten los mismos dos cables (SDA y SCL)
en paralelo, más alimentación. La microSD habla **SPI** (4 cables propios).

### 2.1 Tabla de conexiones — versión original (Feather + Adalogger + AW9523)

Del README de DockEye:

| Desde | Pin | Hacia | Pin |
|---|---|---|---|
| Protoboard riel **+** | — | ESP32-S3 | 3.3V |
| Protoboard riel **−** | — | ESP32-S3 | GND |
| AW9523 | VIN | riel + | |
| AW9523 | GND | riel − | |
| AW9523 | SCL | ESP32-S3 | SCL |
| AW9523 | SDA | ESP32-S3 | SDA |
| AS7341 | VIN | riel + | |
| AS7341 | GND | riel − | |
| AS7341 | SCL | ESP32-S3 | SCL |
| AS7341 | SDA | ESP32-S3 | SDA |
| **LED azul 430 nm** | ánodo (+) | riel + | |
| LED azul 430 nm | cátodo (−) | AW9523 | **pin 10** |
| **LED IR 910 nm** | ánodo (+) | riel + | |
| LED IR 910 nm | cátodo (−) | AW9523 | **pin 0** |

- El Adalogger (RTC + SD) no lleva cables: se **apila** sobre el Feather. El firmware espera la SD en chip-select **10** (constante `SD_CHIP_SELECT = 10` del .ino), que es el CS del Adalogger apilado.
- El AW9523 en modo LED actúa de sumidero de corriente constante (~37 mA por pin): por eso los LEDs van del riel + al pin del AW9523, **sin resistencia**.
- El firmware además maneja un **LED en el pin 3 del AW9523 como indicador** (parpadea para marcar inicio/fin de cada fase; función `blinkLED`). El README de wiring no lo menciona. **A VERIFICAR / decisión práctica:** conectar un LED común (cualquier color) + al riel +, − al pin 3 del AW9523, montado visible en el gabinete. Si no se conecta, el aparato funciona igual pero sin señal visual de progreso.
- Direcciones I²C que espera el firmware: AS7341 en `0x39`, AW9523 en `0x58` (defaults de ambos módulos; no tocar jumpers de dirección).

### 2.2 Tabla de conexiones — versión "todo local" (devkit ESP32-S3 + módulos sueltos)

Si se arma con sustitutos (ver doc 01), el mapa equivalente:

| Módulo | Pin módulo | ESP32-S3 devkit |
|---|---|---|
| AS7341 | VIN / VCC | 3V3 |
| AS7341 | GND | GND |
| AS7341 | SDA | GPIO 8 (o el SDA que se configure) |
| AS7341 | SCL | GPIO 9 (o el SCL que se configure) |
| RTC DS3231 | VCC / GND / SDA / SCL | 3V3 / GND / mismos SDA y SCL |
| microSD SPI | VCC | 3V3 (⚠️ verificar que el módulo sea de 3.3 V o tenga regulador) |
| microSD SPI | CS | GPIO libre (anotar; se pone en el firmware) |
| microSD SPI | SCK / MOSI / MISO | SPI del devkit (GPIO 12 / 11 / 13 típico en S3) |
| LED azul | ánodo | 3V3 vía resistencia limitadora* |
| LED azul | cátodo | colector del transistor PN2222 (emisor a GND, base a un GPIO vía 1 kΩ) |
| LED IR | ídem con segundo transistor y GPIO propio | |

\* Calcular la resistencia según el LED: R = (3,3 V − Vf) / I. Ej.: LED azul Vf≈3,0 V a 20 mA → R ≈ 15 Ω (redondear a 22 Ω comercial y medir). Para el IR Vf≈1,4 V a 20 mA → R ≈ 100 Ω.

⚠️ Los GPIO exactos del devkit dependen del modelo comprado. Regla: **elegirlos, anotarlos en la hoja del instrumento y ajustarlos en el firmware** (doc 03). No hay valores "mágicos": lo que importa es la consistencia.

## 3. Impresión 3D y ensamble mecánico

### 3.1 Qué imprimir

De `vendor/DockEye/Hardware/Housing/`:

- **`Open-Fluorometer-Main-Housing v9.stl`** — cuerpo principal (portacubeta + posiciones de LEDs y sensor). Usar la v9 (última versión; v7 y v8 son iteraciones anteriores).
- **Tapa: RESUELTA — usar [`planos/tapa-v1.stl`](planos/tapa-v1.stl) de este repo.** Diseño original de [Esteban Kovacevic](https://github.com/EstebanKovacevic) (CC BY-SA 4.0), creado porque la tapa de DockEye solo existe como `.f3d` de Fusion 360 y no se podía imprimir tal como estaba.

Parámetros: PLA o PETG **negro opaco**, relleno ≥50%, capa 0,2 mm. Verificar a contraluz que las paredes no transluzcan; si transluce, pintar de negro por dentro o subir perímetros.

### 3.2 Ensamble óptico

1. Insertar el **AS7341** en su alojamiento del gabinete, con la ventana del sensor mirando al hueco de la cubeta. Fijar con tornillos M2.5 o una gota de silicona en las esquinas de la placa (nunca sobre la ventana óptica).
2. Insertar los **dos LEDs** en sus alojamientos (a 90° del sensor). Los LEDs de 5 mm entran a presión en agujeros de 5 mm; asegurar con silicona por atrás.
3. Probar que la **cubeta** entra y sale sin forzar y siempre en la misma posición (el gabinete DockEye está diseñado para cubeta estándar de 10 mm).
4. Pasar el cableado por detrás y cerrar. Sellar cualquier rendija con cinta negra.

**A VERIFICAR:** la orientación exacta de LEDs/sensor dentro del gabinete v9 no está documentada en el repo (las instrucciones de montaje óptico "se agregarán pronto" según el README). Con la pieza impresa en mano es autoevidente dónde va cada cosa (los alojamientos tienen las formas de los componentes); si no lo fuera, consultar antes de pegar nada.

### 3.3 Electrónica en el gabinete

- Versión original Feather: el sandwich Feather+Adalogger va fijado dentro/junto al gabinete según el espacio de la v9.
- Versión devkit: montar protoboard/perforada en una caja auxiliar pegada al gabinete óptico, con los cables de LEDs y sensor entrando por un pasacables sellado a la luz.

## 4. Impresión 3D: sin plan B

El gabinete se imprime en 3D, sin excepción (decisión de proyecto, 2026-09-21): el
STL de DockEye ya trae el portacubeta y los alojamientos ópticos a 90° resueltos —
es la parte fácil y la que garantiza la geometría. No improvisar gabinetes
artesanales.

## 5. Checklist de fin de armado

- [ ] Soldaduras inspeccionadas (sin puentes, sin pines fríos).
- [ ] Continuidad verificada con téster: 3V3→VIN de cada módulo, GND común.
- [ ] Ningún corto entre + y − (téster en continuidad sobre los rieles, debe dar abierto).
- [ ] LEDs con polaridad correcta (pata larga = ánodo = +).
- [ ] Cubeta entra/sale suave y repetible.
- [ ] Gabinete cerrado no deja pasar luz (mirar en cuarto oscuro con linterna por fuera).
