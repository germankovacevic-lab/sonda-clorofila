# Proyecto SONDA — Fluorómetro DIY de clorofila-a y turbidez

**Dossier constructivo · Copernicus · versión 2026-09**

## Qué es esto

Un instrumento de mesa, portátil y de bajo costo, que mide **clorofila-a** (indicador de algas/cianobacterias) y **turbidez** en muestras de agua. Se toma una muestra del espejo de agua con un frasco, se vuelca en una **cubeta de vidrio**, se mete la cubeta en el aparato, y en ~3 minutos el aparato escribe los resultados en una tarjeta SD.

El principio es **fluorometría**: la clorofila-a, iluminada con luz azul (~430 nm), re-emite luz roja (~680 nm). Un sensor espectral mide esa luz roja: a más clorofila, más señal. La turbidez se estima con un segundo LED infrarrojo y la luz que dispersan las partículas en suspensión.

## Para qué lo queremos

Copernicus monitorea espejos de agua por satélite (Sentinel-2). El satélite estima clorofila desde el espacio; este aparato nos da el **dato de verdad medido en el agua** para calibrar y validar esas estimaciones. No necesita ser un instrumento de laboratorio certificado: necesita ser **repetible y consistente**.

## Qué diseño replicamos

Este dossier NO es un diseño propio: es la guía en castellano para construir el
**DockEye** del COAST Lab (EE.UU.), un diseño abierto y publicado, usando de
manual complementario el **fluorómetro del ENSSAT** (Francia, escuela de
fotónica). Ambos repos completos están (o se clonan) en `vendor/`:

```bash
./descargar-fuentes.sh   # los clona en vendor/ pineados a los commits de referencia
```

> Los diseños originales no se redistribuyen acá (ver [LICENSE.md](LICENSE.md)):
> se descargan directo de sus repos. Los **STL del gabinete** quedan en
> `vendor/DockEye/Hardware/Housing/`.

Commits de referencia usados para este dossier (2026-09-21):

- DockEye: `aea70d69787ee7859948dcfca584f06a7d0c6c92` (2025-07-14)
- ENSSAT: `876ba0dcd2a57655762b38ec11ed856d72429feb` (2025-07-17)

**Ante cualquier duda, la fuente manda**: el código, los STL y los README de
`vendor/DockEye/` son el original. Este dossier traduce, ordena y localiza
(componentes conseguibles en Argentina), pero no inventa nada. Donde la fuente
no es clara, el punto está marcado **A VERIFICAR**.

## Mapa del dossier

| Doc | Contenido |
|---|---|
| [01-lista-de-materiales.md](01-lista-de-materiales.md) | Todos los componentes: qué son, dónde conseguirlos en Argentina, sustitutos |
| [02-armado.md](02-armado.md) | Conexiones (tabla de pines), soldadura, impresión 3D, ensamble |
| [03-firmware.md](03-firmware.md) | Cómo cargar el programa al ESP32 y verificar que todo responde |
| [04-protocolo-de-uso.md](04-protocolo-de-uso.md) | Cómo medir: muestra, blanco, registro, calibración |
| [05-pruebas-de-aceptacion.md](05-pruebas-de-aceptacion.md) | Checklist de entrega: qué tiene que demostrar el aparato terminado |

## Resumen del hardware (arquitectura)

```
                      ┌──────────────────────────┐
  USB-C / batería ───►│ ESP32-S3 (cerebro)       │
                      │  · corre el firmware     │
                      └────┬──────────┬──────────┘
                     I²C   │          │ I²C + SPI
              ┌────────────▼───┐  ┌───▼──────────────────┐
              │ AW9523         │  │ Adalogger            │
              │ driver de LEDs │  │ reloj (RTC) + SD     │
              └──┬─────────┬───┘  └──────────────────────┘
                 │         │
          LED azul       LED infrarrojo        ┌─────────────┐
          430 nm         910 nm                │ AS7341      │
             │              │                  │ sensor      │
             └──────► CUBETA ◄─── a 90° ──────►│ espectral   │
                    (vidrio, 10 mm)            └─────────────┘
                 dentro de gabinete impreso en 3D, oscuro
```

Todo va montado en el gabinete impreso en 3D de DockEye, que sostiene la cubeta
con los LEDs de un lado y el sensor **a 90 grados** (por eso la cubeta debe
tener las 4 caras transparentes).

## Presupuesto orientativo (2026-09, estimativo)

- **Vía local + sustitutos (ML/tiendas de electrónica):** ARS 150.000–280.000 aprox., según qué se consiga local y qué llegue de China por ML.
- **Vía importación de originales Adafruit (DigiKey/Mouser/courier):** ~USD 60–80 en partes + envío e impuestos; los LEDs Thorlabs originales suman ~USD 40–60 más.
- Detalle componente por componente en [01-lista-de-materiales.md](01-lista-de-materiales.md).

## Contacto técnico

Cualquier ambigüedad de la fuente está marcada **A VERIFICAR** en cada doc.
Antes de resolverla por las suyas, consultar con quien encargó el trabajo
(las decisiones de sustitución de componentes ópticos afectan la calibración).
