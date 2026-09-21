# 01 · Lista de materiales (BOM)

> Precios y disponibilidad **verificados 2026-09-21** publicación por publicación
> en MercadoLibre Argentina (sesión en vivo). Donde dice "verificado" es el precio
> de un listing concreto de ese día. Los part numbers del diseño original salen
> del README de `vendor/DockEye/`.
>
> Regla general: **el sensor AS7341 y el conjunto óptico son el corazón del
> instrumento — no escatimar ahí.** En el resto (micro, SD, RTC, cables) los
> genéricos sirven perfecto.

## A. Electrónica principal

### A1. Microcontrolador — ESP32-S3

- **Función:** cerebro del aparato; corre el firmware, habla con los sensores, escribe la SD.
- **Original:** Adafruit ESP32-S3 Feather (Adafruit **#5477**, ~USD 17,50 en adafruit.com).
- **En Argentina:** el Feather original de Adafruit **no se consigue local** (ML solo trae otros Feather a >$190.000). Lo que sí abunda:
  - Placa genérica **ESP32-S3 DevKitC / NodeMCU ESP32-S3** (WROOM-1, USB-C) — verificado 2026-09-21: **$18.644** (listing "Placa De Desarrollo Esp32 S3 N16r8 Wifi Bluetooth 2 Usb-c", stock local, llega hoy) · alternativa **$20.990** ("Nodemcu ESP32 S3 44 Pines Usb C", HobbyTronica 4.9, local).
- **Sustituto aceptable:** SÍ, cualquier devkit ESP32-S3 con USB nativo.
  - *Qué se pierde:* el formato Feather (el gabinete STL de DockEye está pensado para apilar Feather + Adalogger; con un devkit genérico la electrónica va montada aparte, ver 02-armado) y el cargador de batería LiPo integrado (se reemplaza con un power bank USB).
  - *Qué NO se pierde:* nada eléctrico. El firmware usa I²C + SPI estándar; el fabricante del original avisa que solo probó el S3 MINI-1, así que **cualquier variante requiere la prueba de humo del doc 05 igual**.
- ⚠️ El firmware original usa el pin **10** como chip-select de la SD; en un devkit genérico elegir un GPIO libre y ajustar una línea del firmware (ver 03-firmware).

### A2. Datalogger — RTC + ranura microSD

- **Función:** reloj de tiempo real (fecha/hora de cada medición) + tarjeta SD donde queda el CSV con los datos.
- **Original:** Adafruit Adalogger FeatherWing (Adafruit **#2922**): RTC **PCF8523** + slot microSD, se apila sobre el Feather.
- **En Argentina:** no se consigue local. Sustituto de dos módulos, ambos ubicuos en ML:
  - **Módulo lector microSD SPI** (genérico) — verificado 2026-09-21: **$2.799** (listing "Modulo Lector De Tarjetas Micro Sd" UNIBOT, local, llega hoy) · $3.059 Starwarecomar · $4.999 HobbyTronica.
  - **Módulo RTC DS3231** (I²C, con pila) — verificado 2026-09-21: **$6.999** (listing "Modulo Rtc Ds3231 Alta Precision Eeprom 24c32 c/pila", HobbyTronica 4.9, local, llega hoy) · $5.399 Educabot (sin confirmar pila).
- **Sustituto aceptable:** SÍ, con un cambio de UNA línea en el firmware: `RTC_PCF8523` → `RTC_DS3231` (la librería RTClib soporta ambos; ver 03-firmware). El DS3231 es incluso más preciso que el PCF8523.
- También: **tarjeta microSD** clase 10, 8–32 GB (ARS 8.000–15.000, cualquier marca conocida).

### A3. Driver de LEDs — AW9523

- **Función:** expansor I²C de 16 pines que enciende/apaga y regula los LEDs con corriente constante, comandado por el ESP32. El firmware original lo usa tal cual (`Adafruit_AW9523`).
- **Original:** Adafruit AW9523 breakout (Adafruit **#4886**, ~USD 4,95).
- **En Argentina:** **NO se consigue** — re-verificado 2026-09-21: la búsqueda "aw9523" en ML devuelve 84 resultados pero **ninguno es el chip** (todo repuestos de notebook y herramientas). Ausencia confirmada. Opciones:
  1. **Importarlo** (Adafruit/DigiKey/Mouser, o pedido puntual a importador tipo CDR Electrónica, cdronline.com.ar — cotizan a pedido). Es chico y barato: conviene sumarlo al mismo pedido que cualquier otra cosa que se importe. **← Recomendado: mantiene el firmware original sin tocar.**
  2. **Sustituto local:** manejar cada LED directo desde un GPIO del ESP32 con un transistor NPN (2N2222/PN2222, <ARS 1.000) + resistencia limitadora — es exactamente lo que hace el diseño ENSSAT (`vendor/Chlorophyll-Fluorometer/hardware/components.md`).
     - *Qué se pierde:* la regulación de corriente constante del AW9523 (la intensidad del LED queda atada a la estabilidad de la fuente → algo más de deriva entre mediciones) y hay que **modificar el firmware** (reemplazar las llamadas `aw.analogWrite(...)` por control de GPIO; ver 03-firmware). Para un instrumento que se calibra contra estándares, es tolerable, pero la opción 1 es más fiel al original.

### A4. Sensor espectral — AS7341 ⭐ (el corazón, sin sustituto)

- **Función:** sensor de 11 canales que mide la luz por bandas: el canal **F8 (680 nm)** capta la fluorescencia de la clorofila y el canal **NIR (910 nm)** la dispersión por turbidez.
- **Original:** Adafruit AS7341 breakout (Adafruit **#4698**, ~USD 15,95).
- **En Argentina — verificado 2026-09-21:** SÍ hay, casi todo envío internacional desde China (2–4 semanas):
  - **$19.752** (listing "Módulo Sensor As7341 11 Canales For Medición", China) · $18.076 ("Módulo Sensor Del Espectrómetro De Color Visible", China).
  - Clon del breakout Adafruit ("As7341 Sensor De Luz De 10 Canales Placa Divisora Stemma"): **$22.665–24.276** (China).
  - Única opción aparentemente con stock local: "As7341 Módulo De Sensor De Espectrómetro" a **$139.998** (6 cuotas, envío gratis) — solo si el plazo de China es inaceptable.
- **Sustituto aceptable: NO.** Todo el firmware y la calibración de DockEye dependen de este chip exacto. Cualquier módulo que traiga el chip **AS7341** sirve (el chip es el mismo AMS); lo que cambia entre módulos es solo el conector.
- ⚠️ Comprar **dos** si el presupuesto lo permite: es el componente más difícil de reponer rápido si uno viene fallado.

### A5. LED de excitación azul — 430 nm

- **Función:** ilumina la muestra para excitar la clorofila-a (pico de absorción ~430 nm).
- **Original:** Thorlabs **LED430L** (LED 430 nm, encapsulado 5 mm, ~USD 40–60 + envío internacional caro — Thorlabs no vende barato ni rápido a Argentina).
- **En Argentina:**
  - **Elumiled** — verificado 2026-09-21: **$5.603/u** (listing "Led 3w Azul 420-430nm Epileds Estrella Predisipadora Elumiled", local) · x2 $8.810 · x5 $18.434. Ojo: es formato **estrella 3 W con predisipadora**, no 5 mm — en ML no apareció 430 nm en encapsulado 5 mm; el estrella funciona (más potencia, requiere limitar corriente y adaptar la montura del gabinete). **← Primera opción.**
  - Fallback azul royal — verificado 2026-09-21: **$8.590 x10** ("X10 Led Epiled Azul Profundo 440nm 3 Watts", local, llega mañana) · 10 W 445–450 nm $14.999.
- **Sustituto aceptable:** SÍ, con matices:
  - Un 425–435 nm local (Elumiled) es sustituto pleno: misma zona de excitación.
  - Un 445–450 nm también **funciona** (la clorofila absorbe fuerte en toda esa banda) pero con eficiencia de excitación algo menor → menos señal → la pendiente de calibración cambia. **No es problema porque cada aparato se calibra individualmente** (doc 04), pero hay que anotar la longitud de onda usada en la hoja del instrumento.
  - ⚠️ NO usar LED "azul común" sin especificación (suelen ser 465–470 nm, ya lejos del pico).
- **A VERIFICAR al comprar:** que el datasheet/publicación declare la longitud de onda dominante.

### A6. LED infrarrojo — 910 nm

- **Función:** ilumina la muestra en infrarrojo; las partículas en suspensión dispersan esa luz hacia el sensor → estimación de turbidez (y de fondo para restar a la señal de clorofila).
- **Original:** Thorlabs **LED910L** (910 nm, 5 mm).
- **En Argentina:** los IR de 5 mm ubicuos son de **940 nm** — verificado 2026-09-21: **$2.080 x5** (listing "X5 Led 5mm Infrarrojo Ir-led Emisor 940nm", local, llega mañana) · x10 $6.460 Htec, llega hoy. ⚠️ En la misma búsqueda hay **fototransistores/receptores** 940 nm al mismo precio: comprar el **EMISOR**.
- **Sustituto aceptable:** SÍ — LED IR **940 nm** ó **850 nm** de 5 mm.
  - *Qué se pierde:* el canal NIR del AS7341 está centrado en 910 nm; a 940 nm el sensor lo ve con menos sensibilidad (a 850 nm también, del otro lado). Menos señal de turbidez → se compensa en calibración. **A VERIFICAR** en banco: que con agua turbia (unas gotas de leche en agua) el canal NIR responda claramente sobre el fondo (prueba incluida en doc 05).

### A7. Pasivos y conexión

| Ítem | Para qué | Dónde / precio aprox. |
|---|---|---|
| Protoboard 830 pts + 40 cables dupont | montar módulos y LEDs | verificado 2026-09-21: $17.637 (kit, local) |
| Cables dupont M-H x40 30 cm | conexiones I²C y alimentación | verificado 2026-09-21: $4.133 (CANDYHO, local) |
| Tiras de pines macho + hembra | soldar a los módulos | ARS 1.000–3.000 |
| Resistencias (kit x100 metal film 1% 1/4 W, 10 valores) | limitadora de LEDs (si no se usa AW9523) | verificado 2026-09-21: $4.500 (Micro Minds, local, llega hoy) |
| Transistores PN2222A | driver de LED alternativo (solo opción A3-2) | verificado 2026-09-21: $2.840 x10 ("Kit X10 Pn2222a", local, llega mañana) |
| Cable USB-C **de datos** | flasheo y alimentación | ARS 4.000–10.000 (verificar que sea de datos, no solo carga) |
| Power bank USB 5 V | uso en campo sin PC | ARS 20.000–40.000 (opcional) |

## B. Óptica y muestras

### B1. Cubetas — VIDRIO o CUARZO, 4 caras transparentes ⭐

- **Función:** contienen la muestra. El LED entra por una cara y el sensor mira por la cara **a 90°** → las **4 caras deben ser transparentes** (las cubetas de espectrofotómetro comunes tienen solo 2 caras pulidas: **NO sirven**).
- **Material: vidrio o cuarzo, NUNCA plástico.** La calibración usa acetona (disuelve el plástico) y los solventes arruinan la cubeta y contaminan la muestra.
- **Original:** cubetas de vidrio para fluorometría, paso óptico 10 mm (DockEye las compra en Amazon, baratas).
- **En Argentina — verificado 2026-09-21:** todo envío internacional China (2–4 sem.):
  - **$17.377 el pack de 4** (listing "4 Celdas Cúbicas De Cuarzo 3.5ml 10mm", China) — mejor valor: cubre blanco + muestra + 2 repuestos en una compra.
  - Packs de 2: $18.678–24.633 ("Pack De 2 Cubetas De Cuarzo Ópticas 10mm").
  - (Proveedores locales de laboratorio tipo Instrumental Pasteur: ~ARS 197.000 la pieza — evitar salvo urgencia.)
- **A VERIFICAR al comprar:** confirmar con el vendedor que las 4 caras son pulidas/transparentes ("cubeta para **fluorescencia**", no "para espectrofotómetro"). Comprar **al menos 4** (blanco, muestra, repuesto x2).

### B2. Consumibles de calibración

(los usa quien calibra, no necesariamente el constructor — ver doc 04)

| Ítem | Para qué | Dónde |
|---|---|---|
| Acetona pura (p.a. o al menos >99%) | extracción de clorofila y estándares | ⚠️ verificado 2026-09-21: en ML solo hay removedores cosméticos con acetona — la p.a. **se compra en droguería física** (Cicarelli/local), ~ARS 8.000–20.000/litro |
| Alcohol etílico 96% (alternativa de extracción) | extracción de clorofila | verificado 2026-09-21: $18.600 bidón 5 L ("Alcohol Etilico 96% Bidon 5 Litros", local, llega hoy) |
| Agua destilada | blancos y diluciones | ferretería/farmacia, ARS 2.000–5.000 x5 L |
| Espinaca fresca | fuente de clorofila para estándares | verdulería 🙂 |
| Frascos de vidrio color ámbar | los extractos se degradan con luz | verificado 2026-09-21: $13.775 x10 gotero 30 cc ámbar ("Frasco Gotero Vidrio Ambar C/pipeta 30cc Pack X 10u", local, llega hoy) |
| Jeringas descartables 10 ml | trasvase y dosificación | verificado 2026-09-21: $17.640 x100 ("Jeringas Descartable 10cc Pico Liso X 100", Gauss Bioingeniería, local) — alcanza un pack chico de farmacia |

## C. Mecánica

### C1. Gabinete impreso en 3D

- **Original:** `vendor/DockEye/Hardware/Housing/Open-Fluorometer-Main-Housing v9.stl` (usar la **v9**, la más reciente). Sostiene la cubeta, los LEDs y el sensor en la geometría de 90° correcta y bloquea la luz ambiente.
- **Impresión:** PLA o PETG **NEGRO opaco** (crítico: el gabinete es la cámara oscura del instrumento), relleno ≥50% en la zona óptica (criterio tomado del manual ENSSAT). Verificado 2026-09-21: los servicios de impresión 3D en ML publican **"precio a convenir"** (se cotiza con el STL en mano) — hay varios en CABA/GBA, incluido **"Delta 3D Lab" en Tigre**. Referencia de insumo: filamento PLA negro $18.990/kg (Hellbot, local) → una pieza de ~100 g debería rondar **ARS 15.000–30.000**, a confirmar cotizando con el STL.
- ⚠️ **La tapa solo está como `Open-Fluorometer-Lid v2.f3d`** (archivo Fusion 360, no STL). **A VERIFICAR:** exportar el STL desde Fusion 360 (gratis para uso personal) o resolver una tapa simple equivalente (una placa opaca que cierre bien a la luz). 
- **El gabinete se imprime en 3D, sin excepción** (decisión de proyecto 2026-09-21): es LO que garantiza la geometría LED-cubeta-sensor y es la parte fácil/barata (servicio de impresión). Sin impresora propia, mandar a imprimir el STL.

### C2. Tornillería y varios

- Tornillos M2.5/M3 con tuercas (fijar placas al gabinete): ARS 3.000–5.000 el surtido.
- Cinta aisladora negra / silicona negra: sellar fugas de luz.
- **A VERIFICAR:** el repo DockEye no publica el listado exacto de tornillería ("hardware for assembling the sensor", sin detalle) — resolver en el momento con el gabinete impreso en mano.

## Resumen de compra — dos caminos

**Camino 1 — “fiel al original” (recomendado si hay tiempo):**
importar de Adafruit/DigiKey: ESP32-S3 Feather #5477 + Adalogger #2922 + AW9523 #4886 + AS7341 #4698 (~USD 50 en partes + envío/impuestos) y LEDs locales (Elumiled 430 nm + IR 940 nm). Firmware original casi sin tocar.

**Camino 2 — “todo local/ML” (más rápido, más barato):**
ESP32-S3 devkit genérico + módulo microSD + RTC DS3231 + módulo AS7341 (China por ML) + LEDs locales + transistores. Requiere los ajustes de firmware del doc 03 (CS de SD, RTC, driver de LEDs).

### Costo total consolidado (verificado 2026-09-21, listings concretos de ML)

| Rubro | Verificado |
|---|---|
| ESP32-S3 devkit (local) | $18.644 |
| Módulo microSD SPI (local) | $2.799 |
| RTC DS3231 c/pila (local) | $6.999 |
| AS7341 (China, 2–4 sem.) | $19.752 |
| LED azul Elumiled 420–430 nm x2 (local) | $8.810 |
| LED IR 940 nm 5 mm x5 (local) | $2.080 |
| PN2222 x10 + kit resistencias x100 (local) | $7.340 |
| Protoboard 830 + cables dupont (local) | $21.770 |
| Cubetas cuarzo 10 mm x4 (China, 2–4 sem.) | $17.377 |
| Impresión 3D gabinete (a convenir, ref. ~100 g) | ~$15.000–30.000 |
| Varios no verificados (pines, microSD card, cable USB-C, tornillería) | ~$20.000–30.000 |

- **Escenario 1 — armado mínimo (1 sensor):** **~ARS 150.000–165.000**.
- **Escenario 2 — recomendado (2º AS7341 de respaldo):** **~ARS 170.000–185.000**.
- **Escenario 3 — sin esperar China (AS7341 "local" a $139.998 + cubetas China igual):** **~ARS 270.000–285.000** — solo pagar la prisa si hay una demo con fecha.
- Consumibles de calibración (alcohol 96% + frascos ámbar + jeringas, todo ML local): **~ARS 50.000** aparte; la acetona p.a. va por droguería física.

**Qué domina la incertidumbre:** (1) la **impresión 3D** — único rubro sin precio publicado, se cotiza con el STL; (2) el **AS7341 y las cubetas** — precio firme pero plazo China 2–4 semanas y riesgo aduana; todo el resto es stock local con entrega en 24 h. El rango viejo estimado (150–280 mil) queda **confirmado en su piso**: la compra real da ~$150–185 mil.

| Componente | ¿Se consigue en Argentina? (verificado 2026-09-21) | Sustituto |
|---|---|---|
| ESP32-S3 | ✅ local $18.644, llega hoy | devkit genérico |
| Adalogger (RTC+SD) | ❌ | microSD SPI $2.799 + DS3231 $6.999 (local) |
| AW9523 | ❌ ausencia re-confirmada | importar ó GPIO+transistor |
| AS7341 | ✅ ML China $19.752 (2–4 sem.) · "local" $139.998 | **no hay sustituto** |
| LED 430 nm | ⚠️ Elumiled 420–430 nm estrella 3 W $5.603 (no hay 5 mm) | sí, con nota de calibración |
| LED 910 nm | ⚠️ solo 940 nm: x5 $2.080 | sí, validar en banco |
| Cubetas cuarzo 4 caras | ✅ ML China: x4 $17.377 (2–4 sem.) | ninguno (nunca plástico) |
| Gabinete 3D | ✅ servicio, precio a convenir (~$15–30 mil) | — (sin plan B: se imprime sí o sí) |
