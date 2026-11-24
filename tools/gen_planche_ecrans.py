#!/usr/bin/env python3
"""Genere, pour la planche docs/creatures/index.html, la liste de TOUS les ecrans du firmware :
JIANT (palette d'origine) / notre palette sombre (NIGHT) / notre palette claire (WHITE).
Les ecrans qui dessinent une creature passent en premier. Usage : /usr/bin/python3 tools/gen_planche_ecrans.py"""
import glob, html, os, re
from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
PPM = os.path.join(HERE, '..', 'build', 'wf', 'ppm')
OUT = os.path.join(HERE, '..', '..', 'docs', 'creatures')
SRC = [('jiant', 'AIR_JIANT_', 'JIANT'), ('sombre', 'NIGHT_', 'notre sombre'), ('clair', 'WHITE_', 'notre clair')]

names = sorted(f[len(PPM) + 1 + len('NIGHT_'):-4] for f in glob.glob(os.path.join(PPM, 'NIGHT_*.ppm')))

# ecrans qui dessinent une creature (ou une petite icone du moteur) : a remplacer en priorite
DRAW = re.compile(r'^(home$|home_being|home_faces|home_idle|mock_home|splash|v6_|vx_|dx_|drumx|edit_(analog|fm6|voice|wheel)|'
                  r'gen_explore|fm6_alg_|presets|save_sound|save_presets|mixer_tracks4)')
# deja remplaces ou deja a nous sur la planche (une carte en haut)
DONE = {'splash', 'home', 'v6_analog_full', 'v6_phase_full', 'v6_lofi_full', 'v6_voice_full', 'v6_wheel_full',
        'v6_noise_full', 'v6_fm6_full', 'v6_animal_full', 'dx_sound', 'dx_sound2', 'save_presets', 'save_sound'}

GROUPS = [('about', 'À propos'), ('arp', 'Arp'), ('auto', 'Automation'), ('batt', 'Batterie'), ('comp', 'Compo'),
          ('confirm', 'Confirmations'), ('countin', 'Décompte'), ('dlg', 'Dialogues'), ('drum', 'Drum'), ('ed', 'Édition'),
          ('edit', 'Édition moteur'), ('env', 'Enveloppes'), ('fx', 'Effets'), ('gen', 'Génération'), ('glo', 'Global'),
          ('head', 'En-tête'), ('home', 'Home'), ('key', 'Clavier'), ('layer', 'Calques'), ('lente', 'Lente'),
          ('lfo', 'LFO'), ('live', 'Live'), ('ly', 'Calques FX'), ('menu', 'Menu'), ('message', 'Messages'),
          ('mixer', 'Mixer'), ('mod', 'Mod'), ('motion', 'Motion'), ('name', 'Noms'), ('page', 'Pages'),
          ('perform', 'Perform'), ('project', 'Projet'), ('reverb', 'Réverb'), ('roll', 'Roll'), ('save', 'Save'),
          ('song', 'Song'), ('words', 'Mots')]

def group(n):
    p = n.split('_')[0]
    for k, label in GROUPS:
        if p == k: return label
    return 'Divers'

for d, _, _ in SRC: os.makedirs(os.path.join(OUT, 'ecrans', d), exist_ok=True)
n_ok = 0
for n in names:
    for d, pre, _ in SRC:
        p = os.path.join(PPM, pre + n + '.ppm'); q = os.path.join(OUT, 'ecrans', d, n + '.png')
        if os.path.exists(p) and (not os.path.exists(q) or os.path.getmtime(q) < os.path.getmtime(p)):
            Image.open(p).convert('RGB').save(q, optimize=True)
    n_ok += 1

def art(n):
    f = ''.join('<figure><img loading="lazy" src="ecrans/%s/%s.png" width="240" height="240" alt="%s %s">'
                '<figcaption><span class="tag %s">%s</span></figcaption></figure>'
                % (d, n, html.escape(lab), n, 'j' if d == 'jiant' else 'p', lab) for d, _, lab in SRC)
    return '<article class="mini" id="e-%s"><h4>%s</h4><div class="trio">%s</div></article>' % (n, n, f)

draw = [n for n in names if DRAW.match(n)]
todo = [n for n in draw if n not in DONE]
done = [n for n in draw if n in DONE]
rest = [n for n in names if not DRAW.match(n)]
byg = {}
for n in rest: byg.setdefault(group(n), []).append(n)

out = ['<h2 class="autres" id="autres">Tous les autres écrans JIANT</h2>',
       '<p class="intro">En haut de la page, les 14 écrans déjà dessinés par nous. Ici, <strong>tous les autres écrans du firmware</strong> : à gauche la palette JIANT d\'origine, au milieu notre palette sombre, à droite notre palette claire. D\'abord ceux qui dessinent encore une créature JIANT (à remplacer en priorité), ensuite les pages sans dessin, classées par famille.</p>',
       '<nav class="gnav"><a href="#g-draw">Avec un dessin · %d</a><a href="#g-done">Déjà vus plus haut · %d</a>%s</nav>'
       % (len(todo), len(done), ''.join('<a href="#g-%s">%s · %d</a>' % (re.sub(r'\W', '', g), g, len(v)) for g, v in sorted(byg.items())))]
out.append('<section class="grp" id="g-draw"><h3>Avec un dessin JIANT, à remplacer en priorité · %d écrans</h3>%s</section>' % (len(todo), ''.join(art(n) for n in todo)))
out.append('<section class="grp" id="g-done"><h3>Autres états des écrans déjà dessinés plus haut · %d écrans</h3>%s</section>' % (len(done), ''.join(art(n) for n in done)))
for g, v in sorted(byg.items()):
    out.append('<section class="grp" id="g-%s"><h3>%s · %d écrans</h3>%s</section>' % (re.sub(r'\W', '', g), g, len(v), ''.join(art(n) for n in v)))
open(os.path.join(OUT, 'ecrans-fragment.html'), 'w').write('\n'.join(out))
print(len(names), 'ecrans :', len(todo), 'avec dessin a remplacer,', len(done), 'deja vus,', len(rest), 'sans dessin')
