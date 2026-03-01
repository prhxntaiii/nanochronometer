#!/usr/bin/env python3
"""
check_font_name.py  -  Muestra el family name (nameID=1) que GDI usará
                        para una fuente TTF u OTF.

Uso:
    python check_font_name.py ruta\a\fuente.ttf

Replica exactamente la lógica de ttf_read_family() en main.c:
  platformID=3, encodingID=1, nameID=1  (Windows Unicode BMP)
"""

import sys
import struct

def be16(data, off): return struct.unpack_from('>H', data, off)[0]
def be32(data, off): return struct.unpack_from('>I', data, off)[0]

def read_family_name(path):
    with open(path, 'rb') as f:
        data = f.read()

    if len(data) < 12:
        return None, "Archivo demasiado pequeño"

    num_tables = be16(data, 4)
    if 12 + num_tables * 16 > len(data):
        return None, "Tabla de offsets truncada"

    # Buscar tabla 'name'
    name_off = None
    for i in range(num_tables):
        rec = 12 + i * 16
        tag = data[rec:rec+4]
        if tag == b'name':
            name_off = be32(data, rec + 8)
            break

    if name_off is None:
        return None, "Tabla 'name' no encontrada"

    if name_off + 6 > len(data):
        return None, "Offset de tabla 'name' fuera de rango"

    count      = be16(data, name_off + 2)
    str_offset = be16(data, name_off + 4)

    results = []
    for i in range(count):
        nr   = name_off + 6 + i * 12
        plat   = be16(data, nr + 0)
        encid  = be16(data, nr + 2)
        langid = be16(data, nr + 4)
        nameid = be16(data, nr + 6)
        length = be16(data, nr + 8)
        offset = be16(data, nr + 10)

        if nameid != 1:
            continue

        abs_off = name_off + str_offset + offset
        if abs_off + length > len(data):
            continue

        raw = data[abs_off:abs_off+length]

        if plat == 3 and encid == 1:
            # UTF-16 BE  ← lo que usa main.c
            try:
                name = raw.decode('utf-16-be')
                results.append((plat, encid, langid, name, True))
            except:
                pass
        elif plat == 1:
            # Mac Roman  (informativo)
            try:
                name = raw.decode('mac-roman')
                results.append((plat, encid, langid, name, False))
            except:
                pass

    return results, None

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Uso: python check_font_name.py <archivo.ttf|.otf>")
        sys.exit(1)

    path = sys.argv[1]
    results, err = read_family_name(path)

    if err:
        print(f"ERROR: {err}")
        sys.exit(1)

    if not results:
        print("No se encontraron registros nameID=1.")
        sys.exit(1)

    print(f"\nArchivo: {path}\n")
    print(f"{'Plat':>6}  {'Enc':>4}  {'Lang':>6}  {'Compatible con main.c':^22}  Nombre")
    print("-" * 70)
    for plat, enc, lang, name, is_win in results:
        compat = "✓  (se usará este)" if is_win else "   (Mac Roman, info)"
        print(f"{plat:>6}  {enc:>4}  {lang:#06x}  {compat:<22}  {name!r}")

    # Mostrar el que usará main.c
    win_names = [r for r in results if r[4]]
    if win_names:
        print(f"\n→ main.c usará: {win_names[0][3]!r}")
    else:
        print("\n⚠ No hay registro Win Unicode (plat=3, enc=1). main.c no cargará la fuente.")
