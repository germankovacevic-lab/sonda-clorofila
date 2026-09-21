# 06 · Videos y recursos de apoyo

## ¿Hay video-tutorial del aparato?

**Todavía no — y va a haber.** Ni DockEye ni ENSSAT publicaron video de armado
(verificado 2026-09). Este dossier escrito es hoy la guía más completa que
existe del instrumento. **El armado de la primera unidad va a ser filmado y
publicado acá**, paso a paso, como video-tutorial completo en castellano.
Si estás construyendo y te trabás en un paso concreto, escribinos:
hola@copernicus.com.ar — lo destrabamos y de paso mejora el dossier.

## Recursos por habilidad

El armado no usa ninguna técnica exótica: se descompone en cuatro habilidades
estándar del mundo maker, cada una con muchísimo material en español. Por cada
una: una guía escrita de referencia + qué buscar en YouTube.

### 1. Soldar tiras de pines a los módulos
- Búsqueda YouTube: `como soldar pines header arduino`
- Es la soldadura más básica que existe (pin pasante, pad grande). Regla del
  doc 02: soldadura brillante y cónica, sin puentes entre pines vecinos.

### 2. Programar el ESP32 con Arduino IDE
- Guía escrita (referencia clásica en español): https://www.luisllamas.es/como-programar-esp32-con-el-ide-de-arduino/
- Curso Prometec (texto + video): https://www.prometec.net/02-instalando-el-entorno-arduino-y-el-pluggin-esp32/
- Búsqueda YouTube: `instalar ESP32 arduino IDE español`
- Después seguir el doc 03 de este dossier (tiene los pasos específicos).

### 3. Conectar módulos I²C (sensor, RTC, driver)
- Guía escrita: https://www.luisllamas.es/arduino-i2c/
- Búsqueda YouTube: `bus I2C arduino explicado español`
- Lo específico nuestro (qué pin va con qué) está en las tablas del doc 02 —
  no hace falta entender el protocolo, solo conectar según tabla.

### 4. Imprimir el gabinete 3D
- El set imprimible completo está en `planos/`: `gabinete-dockeye-v9.stl` + `tapa-v1.stl`.
- Búsqueda YouTube: `imprimir STL cura tutorial español`
- Sin impresora propia: cualquier servicio de impresión 3D lo cotiza con el
  STL (parámetros en el doc 02: PLA/PETG negro opaco, relleno ≥50%).

## Bibliografía del método (para quien quiera el fundamento)

- Leeuw, Boss & Wright (2013) — fluorómetro de fitoplancton con electrónica
  de bajo costo, validado contra instrumentos comerciales: doi:10.3390/s130607872
- Lopin (2024) — sensores AS726x/AS7341 para clorofila con regresión PLS:
  github.com/KyleLopin/asm_chloro_test
