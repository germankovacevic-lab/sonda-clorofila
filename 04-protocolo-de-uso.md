# 04 · Protocolo de uso y calibración

> Fuente: procedimiento de extracción y estándares de
> `vendor/DockEye/Procedures/Chlorophyll a extraction/README.md`, guía de
> calibración de `vendor/Chlorophyll-Fluorometer/calibration.md` (ENSSAT) y
> scripts de `vendor/DockEye/Data Analysis/`.
>
> Este doc lo usa quien opere el instrumento. El constructor solo necesita
> entenderlo por arriba + ejecutar las pruebas del doc 05.

## 1. Regla de oro

**Cubetas de VIDRIO o CUARZO, nunca plástico.** Los estándares de calibración
van en acetona al 80%, que disuelve el plástico. Y una cubeta atacada por
solvente queda opaca → basura. Todas las cubetas del kit son de vidrio/cuarzo
con 4 caras transparentes.

## 2. Medición de rutina (muestra de campo)

### 2.1 Toma de muestra
1. Tomar agua sub-superficial (~20–30 cm de profundidad) del espejo de agua, en frasco limpio y opaco o envuelto (la clorofila se degrada con luz y calor).
2. Rotular: fecha, hora, punto GPS, quién.
3. Medir lo antes posible (ideal < 2 h; mantener fresco y a oscuras hasta medir).

### 2.2 Medición
1. Homogeneizar la muestra (invertir el frasco suavemente 10 veces; no agitar violento, hace burbujas).
2. Enjuagar la cubeta 2 veces con la propia muestra; llenar a ~3/4. **Sin burbujas** en las caras ópticas; secar el exterior con papel suave (huellas y gotas desvían luz).
3. Insertar la cubeta en el gabinete, cerrar la tapa.
4. Encender el aparato (o RESET). Corre solo: 2 parpadeos → 60 s de LED azul → 3 parpadeos → 60 s de LED IR → resumen. Total ~2,5 min.
5. Apagar, retirar cubeta, enjuagar con agua destilada.

### 2.3 Blanco
En cada tanda de mediciones (mínimo al inicio y al final), correr una medición
idéntica con **agua destilada**. Ese valor es el cero del día y baja junto con
los datos.

### 2.4 Registro
- Los datos quedan en `Test.csv` de la microSD. **Renombrar el archivo tras cada corrida** (ej. `2026-09-21_laguna-X_p1.csv`), porque el firmware agrega al mismo archivo.
- Llevar planilla paralela: fecha/hora, punto, blanco del día, archivo asociado, observaciones (color del agua, clima).
- El valor que interesa: en el bloque `SUMMARY`, la resta `Chl − Turb F8` (señal de fluorescencia corregida por fondo) y la `Calibrated Chlorophyll-a` **solo si ya se cargó nuestra ecuación de calibración** (ver §3.4).

## 3. Calibración (resumen)

La calibración convierte "cuentas" del sensor en µg/L. Se hace al construir el
aparato, y se repite si se cambia cualquier componente óptico o parámetro del
sensor.

### 3.1 Preparar extracto de clorofila (fuente: espinaca)
Del procedimiento DockEye:
1. Picar y pesar ~4 g de hojas de espinaca fresca.
2. Moler en mortero con 5–10 mL de acetona fría hasta que no queden trozos visibles.
3. Pasar a frasco de vidrio ámbar con 20 mL de acetona al 80%.
4. Sonicar 30 min en baño de hielo (si no hay sonicador: **A VERIFICAR** — DockEye no da alternativa; en la práctica reposo prolongado en frío y oscuridad extrae igual, con menor eficiencia).
5. Centrifugar 5 min a 3000 rpm (o dejar decantar y filtrar) y quedarse con el líquido claro (sobrenadante).
6. La concentración real del extracto madre se determina contra un instrumento de referencia (laboratorio o fluorómetro comercial). **Este paso requiere un tercero con instrumento calibrado** — es el único eslabón que no se resuelve con el kit.

### 3.2 Estándares
Diluir en acetona al 80% para obtener 5 puntos: **100, 50, 20, 10 y 5 µg/L**
(cada uno se logra diluyendo el anterior: 2×, 2,5×, 2×, 2×).

### 3.3 Medir la curva
1. Medir blanco (acetona 80% sola).
2. Medir cada estándar de menor a mayor concentración, con el protocolo normal (§2.2), enjuagando la cubeta con el estándar siguiente entre puntos.
3. En planilla (Excel/LibreOffice): graficar señal (promedio F8 con LED azul, menos fondo) vs. concentración. Ajustar recta: `señal = a × concentración + b`. Debe dar R² > 0,95; si no, revisar burbujas, luz externa, cubeta sucia.

### 3.4 Cargar la calibración al firmware
Invertir la recta y reemplazar en el .ino (línea ~114):
```cpp
float calibrated_chla = 0.1332 * diff - 1.7127;  // ← ecuación de DockEye, NO la nuestra
```
por los coeficientes propios (`chla = (diff − b) / a`). Anotar fecha de
calibración y coeficientes en la hoja del instrumento. Re-flashear.

> Nota: los números 0,1332 y −1,7127 que trae el código son la calibración del
> prototipo de COAST-Lab. **Sin recalibrar, las lecturas "calibradas" de nuestro
> aparato no son confiables** — sí lo son las cuentas crudas y sus diferencias.

### 3.5 Turbidez
DockEye no publica protocolo de calibración de turbidez (el canal NIR se usa
sobre todo como fondo para la clorofila). Si se quiere turbidez en NTU:
calibrar con estándares de formazina o suspensiones caseras validadas —
**A VERIFICAR / fase 2**, no bloquea la entrega.

## 4. Mantenimiento

- Cubetas: lavar con agua destilada tras cada uso; guardar secas y envueltas. Nunca cepillar por dentro con nada abrasivo.
- No dejar muestras/estándares dentro del aparato.
- Acetona: manipular con ventilación, lejos de fuentes de calor; guardar en frasco de vidrio rotulado.
- Revisar cada tanto que el gabinete siga sellado a la luz (prueba de linterna del doc 02 §5).
