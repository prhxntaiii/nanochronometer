#!/usr/bin/env python3
"""
gen_ico.py  –  NanoChrono icon generator
Generates  assets/nanochrono.ico  with sizes  256, 128, 64, 48, 32, 16
using only Python standard library (no Pillow required).

Design:  dark charcoal circle  +  phosphor-green clock face  +  NC monogram
"""
import struct
import math
import os
import sys

# ── Canvas primitives (tiny software rasterizer) ─────────────────────────────

def make_canvas(size):
    """RGBA flat array, row-major"""
    return bytearray(size * size * 4)

def px(canvas, size, x, y, r, g, b, a=255):
    if 0 <= x < size and 0 <= y < size:
        i = (y * size + x) * 4
        # Alpha-blend over existing
        sa = a / 255.0
        da = canvas[i+3] / 255.0
        oa = sa + da * (1 - sa)
        if oa > 0:
            canvas[i]   = int((r * sa + canvas[i]   * da * (1-sa)) / oa)
            canvas[i+1] = int((g * sa + canvas[i+1] * da * (1-sa)) / oa)
            canvas[i+2] = int((b * sa + canvas[i+2] * da * (1-sa)) / oa)
        canvas[i+3] = int(oa * 255)

def circle_aa(canvas, size, cx, cy, r, col, fill=False, width=1.5):
    """Anti-aliased circle (outline or fill)"""
    ir = int(r + 2)
    R, G, B, A = col
    for dy in range(-ir, ir+1):
        for dx in range(-ir, ir+1):
            dist = math.sqrt(dx*dx + dy*dy)
            if fill:
                if dist <= r - 0.5:
                    px(canvas, size, int(cx+dx), int(cy+dy), R, G, B, A)
                elif dist <= r + 0.5:
                    alpha = int(A * (r + 0.5 - dist))
                    px(canvas, size, int(cx+dx), int(cy+dy), R, G, B, alpha)
            else:
                d = abs(dist - r)
                if d < width/2 + 1:
                    alpha = int(A * max(0, 1 - max(0, d - width/2)))
                    px(canvas, size, int(cx+dx), int(cy+dy), R, G, B, alpha)

def line_aa(canvas, size, x0, y0, x1, y1, col, width=1.5):
    """Wu's anti-aliased line"""
    R, G, B, A = col
    dx = x1 - x0
    dy = y1 - y0
    length = math.sqrt(dx*dx + dy*dy)
    if length < 0.001: return
    steps = max(int(length * 2), 1)
    for i in range(steps + 1):
        t = i / steps
        x = x0 + dx * t
        y = y0 + dy * t
        # Draw with perpendicular thickness
        nx = -dy / length
        ny =  dx / length
        hw = width / 2
        for w in range(int(-hw*2-2), int(hw*2+3)):
            ox = nx * w * 0.5
            oy = ny * w * 0.5
            d = abs(w * 0.5)
            alpha = int(A * max(0, 1 - max(0, d - hw)))
            px(canvas, size, int(x+ox+0.5), int(y+oy+0.5), R, G, B, alpha)

def render_icon(size):
    canvas = make_canvas(size)
    s = size
    cx = s / 2
    cy = s / 2

    # Background circle (very dark, slightly blue-green tint)
    circle_aa(canvas, s, cx, cy, s/2 - 1, (8, 14, 8, 255), fill=True)

    # Outer ring - dim green
    circle_aa(canvas, s, cx, cy, s/2 - 2, (32, 80, 32, 220), fill=False, width=max(1, s*0.025))

    # Middle ring - accent green
    r2 = s * 0.42
    circle_aa(canvas, s, cx, cy, r2, (0, 200, 50, 180), fill=False, width=max(1, s*0.018))

    # Clock tick marks (12 marks)
    for i in range(12):
        angle = math.radians(i * 30 - 90)
        r_outer = s * 0.38
        r_inner = s * 0.30 if i % 3 == 0 else s * 0.34
        w = max(1.0, s * 0.025) if i % 3 == 0 else max(0.8, s * 0.015)
        x0 = cx + math.cos(angle) * r_inner
        y0 = cy + math.sin(angle) * r_inner
        x1 = cx + math.cos(angle) * r_outer
        y1 = cy + math.sin(angle) * r_outer
        alpha = 255 if i % 3 == 0 else 160
        line_aa(canvas, s, x0, y0, x1, y1, (0, 255, 65, alpha), width=w)

    # Hour hand (pointing to ~10 o'clock for visual interest)
    angle_h = math.radians(-60)
    r_hand = s * 0.22
    line_aa(canvas, s,
            cx + math.cos(angle_h + math.pi) * s*0.05,
            cy + math.sin(angle_h + math.pi) * s*0.05,
            cx + math.cos(angle_h) * r_hand,
            cy + math.sin(angle_h) * r_hand,
            (0, 255, 65, 240), width=max(1.5, s*0.03))

    # Minute hand (pointing to ~2 o'clock)
    angle_m = math.radians(60)
    r_min = s * 0.28
    line_aa(canvas, s,
            cx + math.cos(angle_m + math.pi) * s*0.06,
            cy + math.sin(angle_m + math.pi) * s*0.06,
            cx + math.cos(angle_m) * r_min,
            cy + math.sin(angle_m) * r_min,
            (0, 255, 65, 255), width=max(1, s*0.022))

    # Seconds hand (sweeping  – thin, bright)
    angle_s = math.radians(135)
    line_aa(canvas, s,
            cx + math.cos(angle_s + math.pi) * s*0.09,
            cy + math.sin(angle_s + math.pi) * s*0.09,
            cx + math.cos(angle_s) * s*0.32,
            cy + math.sin(angle_s) * s*0.32,
            (100, 255, 100, 255), width=max(0.8, s*0.012))

    # Center dot
    circle_aa(canvas, s, cx, cy, max(2, s*0.04), (0, 255, 65, 255), fill=True)
    circle_aa(canvas, s, cx, cy, max(1, s*0.02), (180, 255, 180, 255), fill=True)

    # Glow halo (very subtle)
    circle_aa(canvas, s, cx, cy, s/2 * 0.95, (0, 255, 65, 18), fill=False, width=s*0.08)

    return canvas

# ── ICO format encoder ────────────────────────────────────────────────────────

def canvas_to_bmp_data(canvas, size):
    """
    Convert RGBA canvas to 32-bit ARGB BMP data (ICO bitmap DIB format).
    ICO bitmaps are stored bottom-to-top, BGRA order, with an AND mask.
    """
    # BITMAPINFOHEADER (40 bytes)
    width  = size
    height = size * 2  # combined XOR + AND masks
    planes = 1
    bpp    = 32
    compression = 0
    img_size = size * size * 4
    hdr = struct.pack('<IIIHHIIIIII',
        40,       # biSize
        width,    # biWidth
        height,   # biHeight (doubled for ICO)
        planes,   # biPlanes
        bpp,      # biBitCount
        compression,
        img_size,
        0, 0, 0, 0)

    # Pixel data (bottom-to-top, BGRA)
    rows = []
    for y in range(size - 1, -1, -1):
        row = bytearray(size * 4)
        for x in range(size):
            i = (y * size + x) * 4
            row[x*4+0] = canvas[i+2]   # B
            row[x*4+1] = canvas[i+1]   # G
            row[x*4+2] = canvas[i+0]   # R
            row[x*4+3] = canvas[i+3]   # A
        rows.append(row)
    pixel_data = b''.join(rows)

    # AND mask (all zeros = use alpha channel)
    row_bytes = ((size + 31) // 32) * 4
    and_mask = b'\x00' * (row_bytes * size)

    return hdr + pixel_data + and_mask

def build_ico(sizes):
    """Build an ICO file containing all given square sizes."""
    images = []
    for sz in sizes:
        canvas = render_icon(sz)
        bmp    = canvas_to_bmp_data(canvas, sz)
        images.append((sz, bmp))

    # ICO header
    count  = len(images)
    header = struct.pack('<HHH', 0, 1, count)   # reserved, type=1 (ICO), count

    # Directory entries (16 bytes each)
    offset = 6 + count * 16   # header + all dir entries
    dir_entries = bytearray()
    for (sz, bmp) in images:
        w = sz if sz < 256 else 0    # 0 means 256
        h = w
        dir_entries += struct.pack('<BBBBHHII',
            w, h,          # width, height (0 = 256)
            0,             # color count
            0,             # reserved
            1,             # planes
            32,            # bit count
            len(bmp),      # size of image data
            offset)        # offset of image data
        offset += len(bmp)

    # Assemble
    data = header + bytes(dir_entries)
    for (_, bmp) in images:
        data += bmp
    return data

# ── Entry point ───────────────────────────────────────────────────────────────

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    assets_dir = os.path.join(script_dir, '..', 'assets')
    os.makedirs(assets_dir, exist_ok=True)
    out_path = os.path.join(assets_dir, 'nanochrono.ico')

    if os.path.exists(out_path):
        print(f"[gen_ico] Custom icon already exists: {out_path}  (skipping generation)")
        sys.exit(0)

    print("[gen_ico] Generating nanochrono.ico ...")
    sizes = [256, 128, 64, 48, 32, 16]
    ico_data = build_ico(sizes)

    with open(out_path, 'wb') as f:
        f.write(ico_data)

    print(f"[gen_ico] Created {out_path}  ({len(ico_data):,} bytes, sizes: {sizes})")

if __name__ == '__main__':
    main()
