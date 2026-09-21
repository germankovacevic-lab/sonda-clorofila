#!/usr/bin/env bash
# Clona los diseños originales (DockEye y ENSSAT) en vendor/, pineados a los
# commits con los que se escribió este dossier. La fuente manda.
set -euo pipefail
mkdir -p vendor && cd vendor
[ -d DockEye ] || git clone https://github.com/COAST-Lab/DockEye
git -C DockEye checkout aea70d69787ee7859948dcfca584f06a7d0c6c92
[ -d Chlorophyll-Fluorometer ] || git clone https://github.com/EnssatPhotonicsProjects/Chlorophyll-Fluorometer
git -C Chlorophyll-Fluorometer checkout 876ba0dcd2a57655762b38ec11ed856d72429feb
echo "Fuentes descargadas y pineadas. STLs del gabinete: vendor/DockEye/Hardware/Housing/"
