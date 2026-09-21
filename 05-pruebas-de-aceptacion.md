# 05 · Pruebas de aceptación

Checklist que el constructor ejecuta y documenta ANTES de entregar el aparato.
Se entrega junto con el instrumento:

- esta lista tildada, con los valores medidos anotados,
- los archivos CSV de cada prueba (renombrados como se indica),
- la "hoja del instrumento": componentes usados (con longitudes de onda de los
  LEDs compradas), GPIO elegidos, y toda desviación respecto del diseño original.

Materiales para las pruebas: PC con monitor serie, agua destilada, leche (unas
gotas, para simular turbidez), las cubetas del kit, linterna.

## P1 — Inspección y encendido

- [ ] **P1.1** Inspección visual: soldaduras sin puentes, polaridad de LEDs correcta, cables firmes.
- [ ] **P1.2** Sin cortos: téster en continuidad entre 3V3 y GND da abierto.
- [ ] **P1.3** Conectado por USB, la placa enciende y el puerto serie aparece en el IDE.
- [ ] **P1.4** Al resetear, el monitor serie (9600 baudios) NO muestra ninguno de los errores (`Could not find AS7341`, `Couldn't find RTC`, `AW9523 not found`, `Error opening file!`). → Los tres periféricos I²C y la SD responden.

## P2 — Fecha y hora

- [ ] **P2.1** El RTC está en hora (ver doc 03 §6). Las filas del CSV muestran fecha/hora actual real, no 2000-01-01.
- [ ] **P2.2** Desconectar TODO 10 minutos, volver a encender: la hora sigue bien (la pila del RTC funciona).

## P3 — Respuesta de los LEDs

- [ ] **P3.1** Durante la fase clorofila se ve el LED azul encendido (tapa abierta, cubeta afuera).
- [ ] **P3.2** Durante la fase turbidez, la cámara del celular ve el LED IR encendido.
- [ ] **P3.3** El LED indicador (si se montó) parpadea 2 veces al inicio, entre fases y al final.

## P4 — El sensor ve los LEDs (prueba de canal)

Con cubeta llena de **agua destilada**, gabinete cerrado, correr una medición completa y abrir el CSV:

- [ ] **P4.1** Durante la fase clorofila (filas "Chlorophyll-a"), el canal **F1 (415 nm, primera columna de cuentas)** registra valores claramente mayores que con el aparato apagado → el sensor está viendo el LED azul (por dispersión residual). *Si F1 está clavado en 65535, hay demasiada luz directa: revisar geometría.*
- [ ] **P4.2** Durante la fase turbidez (filas "Turbidity"), el canal **NIR (última columna)** sube respecto del fondo.
- [ ] **P4.3** Ningún canal clavado en 65535 (saturación) ni todo en 0 (sensor tapado / LED muerto) durante las dos fases.

Guardar como `P4_agua-destilada.csv`.

## P5 — Oscuridad (sellado del gabinete)

- [ ] **P5.1** Prueba de linterna: en cuarto oscuro, linterna barriendo el gabinete cerrado por fuera; los valores del serie no se mueven apreciablemente.
- [ ] **P5.2** Medición con cubeta de agua destilada en cuarto iluminado vs. cuarto oscuro: los promedios F8 del SUMMARY difieren menos de 5%. Guardar ambos CSV (`P5_luz.csv`, `P5_oscuro.csv`).

## P6 — Respuesta a turbidez (prueba funcional del canal IR)

1. Medir agua destilada (ya hecho en P4).
2. Agregar 2–3 gotas de leche a la cubeta, mezclar, medir de nuevo.

- [ ] **P6.1** El promedio NIR de la fase turbidez con leche es **claramente mayor** (al menos 2×) que con agua destilada. → El LED IR sustituto (940/850 nm) alcanza; si la diferencia es marginal, escalar: probar más corriente de LED o cambiar a un IR de mayor potencia, y anotarlo.

Guardar `P6_leche.csv`.

## P7 — Respuesta a clorofila (prueba funcional del canal azul)

Sin estándares de laboratorio, alcanza una fuente casera de clorofila: licuar
unas hojas de espinaca en un poco de alcohol o agua, filtrar con filtro de café,
diluir hasta color verde apenas perceptible.

- [ ] **P7.1** El promedio F8 de la fase clorofila con el extracto diluido es claramente mayor que con agua destilada.
- [ ] **P7.2** Una dilución al medio del mismo extracto da señal intermedia (entre el extracto y el blanco). → El aparato ordena concentraciones correctamente.

Guardar `P7_extracto.csv` y `P7_extracto-diluido.csv`.

> Nota: esto NO es la calibración (doc 04, la hace el operador con estándares).
> Es solo la demostración de que el canal de clorofila responde y es monótono.

## P8 — Repetibilidad (la prueba que más importa)

Con la MISMA cubeta de agua destilada, **5 mediciones completas seguidas**,
sacando y volviendo a insertar la cubeta entre cada una:

- [ ] **P8.1** Coeficiente de variación del promedio F8 (desvío estándar / media) **< 5%** entre las 5 corridas.
- [ ] **P8.2** Repetir con el extracto de espinaca diluido: CV < 5%.

Si falla: casi siempre es mecánico (la cubeta no asienta igual, el gabinete
filtra luz, un LED flojo). No se entrega hasta que pase.

Guardar los 10 CSV como `P8_rep_blanco_1..5.csv` y `P8_rep_extracto_1..5.csv`.

## P9 — Autonomía (si se va a usar a batería)

- [ ] **P9.1** Con power bank y SIN PC conectada, el aparato completa una medición solo (ver doc 03 §3.5 — hay que haber quitado la espera del puerto serie) y el CSV queda bien escrito.

*(Opcional si el uso previsto es siempre con notebook al lado — dejarlo asentado.)*

## P10 — Entrega

- [ ] **P10.1** Hoja del instrumento completa (componentes, λ de los LEDs, GPIO, desviaciones, fecha).
- [ ] **P10.2** Todos los CSV de pruebas en una carpeta/pendrive o subidos donde se indique.
- [ ] **P10.3** El .ino final exacto que quedó flasheado (archivo, no captura).
- [ ] **P10.4** Kit completo: aparato, cubetas (mínimo 4), cable USB, microSD, fuente/power bank si se incluyó.

---

**Criterio de aceptación global:** P1–P8 todas tildadas con evidencia. P9 según
modalidad de uso. Cualquier ítem no cumplido se entrega declarado, no oculto.
