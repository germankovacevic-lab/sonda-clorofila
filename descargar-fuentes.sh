#!/usr/bin/env bash
# Clona el diseño del ENSSAT en vendor/, pineado al commit con el que se
# escribió este dossier. La fuente manda.
# (DockEye ya viene incluido en vendor/DockEye/ — es MIT desde 2026-09-21.)
set -euo pipefail
mkdir -p vendor && cd vendor
[ -d Chlorophyll-Fluorometer ] || git clone https://github.com/EnssatPhotonicsProjects/Chlorophyll-Fluorometer
git -C Chlorophyll-Fluorometer checkout 876ba0dcd2a57655762b38ec11ed856d72429feb
echo "ENSSAT descargado y pineado. DockEye ya está incluido en vendor/DockEye/."
