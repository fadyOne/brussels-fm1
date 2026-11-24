#!/usr/bin/env python3
"""Redessine le logo PIKNIK ELEKTRONIK en formes simples (barres, polygones, anneaux, triangles),
sans passer par une reduction de l'image : chaque lettre est une construction geometrique.
Sorties : un rendu de controle (IoU contre l'image d'origine) et un SVG.
Usage : /usr/bin/python3 tools/gen_piknik_logo_vec.py"""
import json, os, sys
from PIL import Image, ImageDraw

SRC = os.environ.get('PIK_SRC', os.path.join(os.path.dirname(__file__), 'assets', 'logo-piknik-black.png'))
OUT = os.environ.get('PIK_OUT', os.path.join(os.path.dirname(__file__), '..', '..', 'docs', 'logo-piknik'))

# --- une lettre = liste de formes : ('rect', x, y, w, h), ('poly', [(x,y)...]), ('ring', cx, cy, ro, ri, half)
# half = None (anneau complet) ou 'R' (moitie droite).  Coordonnees en pixels du fichier 500 x 500.
PRM = dict(bowl=0.36, leg=1.0, legx=0.55, diag=1.0, bar=0.8, kdy=0.0, mid=0.8, tri=24.5)   # regles par recherche, IoU contre l'image

def glyph(ch, x0, y0, w, h, s):
    sh = []
    bar = max(7, round(s * PRM['bar']))
    if ch == 'I':
        sh.append(('rect', x0, y0, w, h))
    elif ch in 'PR':
        ro = round(h * PRM['bowl'], 1); ri = ro - s
        cx = x0 + w - ro; cy = y0 + ro
        sh += [('rect', x0, y0, s, h), ('rect', x0 + s, y0, cx - x0 - s, s), ('rect', x0 + s, y0 + 2 * ro - s, cx - x0 - s, s),
               ('ring', cx, cy, ro, ri, 'R')]
        if ch == 'R':
            lw = s * PRM['leg']
            sh.append(('poly', [(x0 + w * PRM['legx'], y0 + 2 * ro - 1), (x0 + w * PRM['legx'] + lw, y0 + 2 * ro - 1), (x0 + w, y0 + h), (x0 + w - lw, y0 + h)]))
    elif ch == 'N':
        d = s * PRM['diag']
        sh += [('rect', x0, y0, s, h), ('rect', x0 + w - s, y0, s, h),
               ('poly', [(x0, y0), (x0 + d, y0), (x0 + w, y0 + h), (x0 + w - d, y0 + h)])]
    elif ch == 'E':
        gap = (h - 3 * bar) / 2
        sh += [('rect', x0, y0, s, h), ('rect', x0, y0, w, bar), ('rect', x0, y0 + bar + gap, w * PRM['mid'], bar), ('rect', x0, y0 + h - bar, w, bar)]
    elif ch == 'L':
        sh += [('rect', x0, y0, s, h), ('rect', x0, y0 + h - bar, w, bar)]
    elif ch == 'T':
        sh += [('rect', x0, y0, w, bar), ('rect', x0 + (w - s) / 2, y0, s, h)]
    elif ch == 'O':
        sh.append(('ring', x0 + w / 2, y0 + h / 2, w / 2, w / 2 - s * 1.0, None))
    elif ch == 'K':   # le K devient un triangle qui pointe a gauche, plus haut que les lettres
        cy = y0 + h / 2 + PRM['kdy']
        sh.append(('poly', [(x0, cy), (x0 + w, cy - PRM['tri']), (x0 + w, cy + PRM['tri'])]))
    return sh

# (lettre, x0, y0, w, h, s) mesures dans l'image d'origine
ROW1 = [('P', 38, 198, 31, 39, 9.5), ('I', 80, 198, 9, 39, 9.5), ('K', 97, 198, 63, 39, 9.5), ('N', 173, 198, 34, 39, 9.5),
        ('I', 217, 198, 10, 39, 9.5), ('K', 234, 198, 64, 39, 9.5)]
ROW2 = [('E', 37, 254, 29, 37, 8.5), ('L', 78, 254, 26, 37, 8.5), ('E', 115, 254, 29, 37, 8.5), ('K', 152, 254, 63, 37, 8.5),
        ('T', 223, 254, 35, 37, 8.5), ('R', 268, 254, 29, 37, 8.5), ('O', 305, 253, 37, 39, 8.5), ('N', 352, 254, 32, 37, 8.5),
        ('I', 395, 254, 8, 37, 8.5), ('K', 412, 254, 63, 37, 8.5)]

def shapes():
    out = []
    for ch, x0, y0, w, h, s in ROW1 + ROW2:
        out.append((ch, glyph(ch, x0, y0, w, h, s)))
    return out

def render(size=500, k=4):
    big = size * k
    m = Image.new('L', (big, big), 0)
    for ch, sh in shapes():
        for s in sh:
            t = Image.new('L', (big, big), 0); d = ImageDraw.Draw(t)
            if s[0] == 'rect':
                _, x, y, w, h = s; d.rectangle([x * k, y * k, (x + w) * k - 1, (y + h) * k - 1], fill=255)
            elif s[0] == 'poly':
                d.polygon([(x * k, y * k) for x, y in s[1]], fill=255)
            else:
                _, cx, cy, ro, ri, half = s
                d.ellipse([(cx - ro) * k, (cy - ro) * k, (cx + ro) * k, (cy + ro) * k], fill=255)
                if ri > 0: d.ellipse([(cx - ri) * k, (cy - ri) * k, (cx + ri) * k, (cy + ri) * k], fill=0)
                if half == 'R': d.rectangle([0, 0, cx * k - 1, big], fill=0)
            m = Image.composite(Image.new('L', (big, big), 255), m, t)
    return m.resize((size, size), Image.LANCZOS)

def svg():
    p = ['<svg xmlns="http://www.w3.org/2000/svg" viewBox="30 185 455 120" fill="currentColor" fill-rule="evenodd">']
    for ch, sh in shapes():
        for s in sh:
            if s[0] == 'rect': p.append('<rect x="%g" y="%g" width="%g" height="%g"/>' % s[1:])
            elif s[0] == 'poly': p.append('<polygon points="%s"/>' % ' '.join('%g,%g' % q for q in s[1]))
            else:
                _, cx, cy, ro, ri, half = s
                if half == 'R':
                    p.append('<path d="M%g %g A%g %g 0 0 1 %g %g L%g %g A%g %g 0 0 0 %g %g Z"/>' %
                             (cx, cy - ro, ro, ro, cx, cy + ro, cx, cy + ri, ri, ri, cx, cy - ri))
                else:
                    p.append('<path d="M%g %g a%g %g 0 1 0 0.01 0 Z M%g %g a%g %g 0 1 1 0.01 0 Z"/>' %
                             (cx - ro, cy, ro, ro, cx - ri, cy, ri, ri))
    p.append('</svg>')
    return '\n'.join(p)

if __name__ == '__main__':
    src = Image.open(SRC).convert('L'); src = Image.eval(src, lambda v: 255 - v)   # encre = blanc
    mine = render()
    a = src.point(lambda v: 255 if v > 127 else 0); b = mine.point(lambda v: 255 if v > 127 else 0)
    inter = sum(1 for p, q in zip(a.getdata(), b.getdata()) if p and q); uni = sum(1 for p, q in zip(a.getdata(), b.getdata()) if p or q)
    print('IoU global %.3f' % (inter / uni))
    for ch, x0, y0, w, h, s in ROW1 + ROW2:
        box = (x0 - 3, y0 - 6, x0 + w + 4, y0 + h + 7)
        ca, cb = a.crop(box), b.crop(box)
        i = sum(1 for p, q in zip(ca.getdata(), cb.getdata()) if p and q); u = sum(1 for p, q in zip(ca.getdata(), cb.getdata()) if p or q)
        print(' ', ch, x0, 'IoU %.3f' % (i / u))
    if '--write' in sys.argv:
        os.makedirs(OUT, exist_ok=True)
        Image.eval(mine, lambda v: 255 - v).save(os.path.join(OUT, 'redessin.png'))
        diff = Image.merge('RGB', (Image.eval(a, lambda v: 255 - v).point(lambda v: v), Image.eval(b, lambda v: 255 - v), Image.eval(b, lambda v: 255 - v)))
        # rouge = dans l'original seulement, bleu = dans notre dessin seulement
        px = Image.new('RGB', a.size, (255, 255, 255)); P = px.load(); A = a.load(); Bm = b.load()
        for y in range(a.size[1]):
            for x in range(a.size[0]):
                if A[x, y] and Bm[x, y]: P[x, y] = (0, 0, 0)
                elif A[x, y]: P[x, y] = (230, 40, 40)
                elif Bm[x, y]: P[x, y] = (40, 90, 230)
        px.save(os.path.join(OUT, 'ecarts.png'))
        open(os.path.join(OUT, 'logo.svg'), 'w').write(svg())
        print('ecrit dans', os.path.abspath(OUT))
