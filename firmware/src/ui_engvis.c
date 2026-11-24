/* SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 Leo Kuroshita (@kurogedelic), Hügelton Instruments
 * Copyright (C) 2026 Juan Ignacio Antonucci (Juan Jiant)
 * Copyright (C) 2026 Piknik Elektronik (BRUSSELS visuals & characters) */
/* (JIANT 0.9.4) Three engines draw their own creature where every other engine draws its being (ui_being.c): EDIT's
 * SOUND (ui_lzedit.c graph_sound_lz, on the left) and HOME's BEING (ui_lzhome.c, large in the middle); DRUM-X's kit
 * on EDIT > KIT and SOUND 1..4 (the whole panel), the colony staying on KIT 2, KIT 3 and X-MOD. Line art only,
 * antialiased (ui_organic.c og_line), in the beings' thermal colours (bg_col: heat_col; GREY and MONO their greys),
 * from small point tables (int8) and the synthesis state the UI already reads; nothing in the audio path. Each moves
 * only with its sound: in silence it rests (its time runs with the level, as the beings').
 *   ANIMAL  a creature between a bird, an insect and a mammal: its outline, legs, wing, antennae / ears / crest are
 *           the same points in the three (VX_AN), mixed by weights that glide (a metamorphosis, ~0.6 s) to the MODE's
 *           animal (CMOS WHSTL a bird, PULSE WING an insect, GLOTT a mammal), leaned by SIZE past its presets' sizes
 *           (smaller: towards the insect; larger: the mammal) and stirred by CHAOS. With the call (each voice's syllable envelope, CMOS's
 *           chirp gate, WHSTL's syrinx amplitude): the beak / jaw opens and the throat (syrinx, larynx) swells, the
 *           wings buzz at the pulse rate (PULSE, WING), the legs stridulate, the ribs breathe (GLOTT).
 *   DRUM-X  the kit as a colony of small beings with eyes and a mouth, standing as a drum kit does: the kick large in
 *           the middle (its core hot), the snare flat on its stand, the toms on little legs, the hi-hat two shells (CH
 *           together, OH the top one lifted, its mouth open), the crash on a long stem and the cowbell a small head (BELL),
 *           the clap two little ones clapping together, the rim (RS) a spark on the snare's edge. A hit squashes its
 *           being (springing back), shuts its eyes and opens its mouth in a shout, as hard as the hit (the lane's own
 *           envelope: its velocity, its accent; it recovers with the sound's DECAY): a GHOST a small squash, a FLAM two
 *           (its hit to come a shadow), a ratchet repeated squashes. On EDIT > KIT (EDIT 1) the sound last struck named
 *           over it; on SOUND 1..4 the sound edited lit and named; HOME's BEING for a DRUM track. KIT 2, KIT 3 and
 *           X-MOD keep the colony of 0.5 (ui_being.c graph_colony: a being per sound, in two rows).
 * (JIANT 0.9.6) Six more, ported from the approved mock-up (the family's): line art in the same thermal colours (the JIANT
 * palette: DRUM-X's ramp, the mock-up's; the others as the beings), each moving only with its sound, the bichos' face
 * (vx_face) and outline (vx_blob) shared with DRUM-X's beings:
 *   ANALOG  CÉLULA: a cell whose membrane (three isotherms) is two cycles of the oscillators (WAVE, MORPH, DETUNE, MIX,
 *           NOISE, INT) through the filter (CUT on the note, RES) and DRIVE, in polar form; spikes where DRIVE clips it, a
 *           ring at CUT glowing and spreading with RES, cilia with NOISE, the two oscillators orbiting at DETUNE's beat
 *   FM6     the six operators as Atomium balls wired as the algorithm (FM6_CELL, fm6_routes: carriers on the output's
 *           floor, modulators above), each as large and hot as its LEVEL times its envelope now (the sounding notes'),
 *           a window band and a meridian, pulses down the tubes, FB a loop back to its ball; on EDIT, on OPERATOR /
 *           OPERATOR 2 (the operator picked in the accent, DETUNE a shimmer) and HOME. OP ENV: the envelope, its ball
 *           riding it
 *   WHEEL   TUBÍCOLAS: nine tube worms, one a drawbar, as tall as its level (the digits under them), their crowns as
 *           many tentacles as the harmonic; the rotor a current bending them (its real phase and speed), PERC the
 *           worm it strikes, CLICK bubbles, DUST motes
 *   NOISE   ENJAMBRE: a swarm of protozoa round a queen with a face: COLOR their walk (white jitter .. brown drift), FREQ
 *           their swirl and the ring they keep, PEAK how tight; DUST's impulses flare them; LFSR / META the register
 *           marching in columns, one a bit
 *   PHASE   CARACOL: a snail whose shell's chambers sit at the bent phase (WAVE / WAVE2 at DCW, crowding where it
 *           rushes), the foot's sole the wave, FB frays the spiral, SUB its outer whorl, the eyestalks pulling in on a
 *           note; the second line a little one following (RING a thread)
 *   LOFI    ORGANISMO DE CIRCUITO: a caterpillar whose two skins are the last 256 samples out (the scope's: nothing more
 *           from the audio), 8 segments the byte's bits (BYTE: the highest next to the head; the legs lift with their bit,
 *           the antennae the two lowest), lit by the wave's level elsewhere (FLOAT, PLS TRI SAW NOIS WRAM). (0.9.10, V1
 *           "DIP") CHIP sets how far it has turned into a chip, gliding there (~0.6 s): 8BIT / FULL the organism, 4BIT and
 *           STEP half way, 4B/2 nearly, 1BIT a DIP seen from above (back and belly the long edges, the back legs the top
 *           row of pins and the front ones the bottom row, each jutting with its bit, the rings the die's pads, the head
 *           folded into the notch with pin 1's dot, the wave two traces on the die), its skins stair-stepped to the bits.
 *           The other knobs: DUTY the rings' width, ALGO the formula's bits on the back (on the chip a ROM), WAV# the
 *           table as spines, CRUSH / GRIT a stutter (the drawing held 1..6 frames) and flat steps on the skin, VAR the
 *           segments scrambled, DECAY (STEP) the rings dimming as the 4-bit volume falls, MOTION the crawl's speed, VIB a
 *           wave and a shiver, FEEDBK two echoes of older samples, BEND the segments crowding to the head, MASK the
 *           picture torn in bands, LOOP the segments repeating and the tail curling; the FILTER: LP as it was, HP the
 *           insides gone and cilia growing with CUT, BP only the body's middle, COMB a double skin, a closed CUT blurring
 *           and swelling it, RES ringing contours and the peak's ring; DIST the skin clipped with spikes, MORPH the head's
 *           species
 * Each takes a rectangle (vx_draw: EDIT's left panel, HOME's middle, or a whole panel) and scales to it.
 * State: vxs (in the pool: no global of the RAM region moves). MENU > ANIM OFF: they rest. */
#define VX_POOL __attribute__((section(".pool")))
#define VX_CODE __attribute__((noinline))                 /* (each creature its own function: its size on its own) */
#define VX_NE 9                                          /* the creatures' slots: 0 kept free (VOICE, retired), ANIMAL 1, ANALOG 3, FM6 4, WHEEL 5, NOISE 6, PHASE 7, LOFI 8 */
#define VX_NSW 56                                        /* NOISE's swarm (the mock-up's 84: its cost) */
typedef struct { int32_t cx, cy, s; } vxf_t;            /* a unit (Q4: 16) -> Q4 px: c + u * s / 256 */
static struct {
    uint32_t tm[VX_NE], fr[VX_NE];                       /* each one's time (it runs with its sound), the frame it ran */
    int32_t en[VX_NE];                                   /* its level now (0..256) */
    uint32_t mt[VX_NE], ms[VX_NE];                       /* (0.9.6) the mock-up's time (Q8 frames, 30 a second: with the sound), ms */
    uint32_t nage[VX_NE], nms[VX_NE];                    /* .. the newest note seen (voice_t.age), when it came (fm1_ms) */
    int16_t wv[256];                                     /* ANALOG's / PHASE's two cycles; LOFI's last 256 samples out (Q12) */
    int16_t sw[VX_NSW][5];                               /* NOISE: the swarm (x y vx vy Q8 units, its flare Q8) */
    uint32_t rs, swt;                                    /* .. its random state, the mock-up frames it has run */
    uint32_t rph[2], bub[9], fhold;                      /* WHEEL: the rotors as last seen sounding, CLICK's bubbles; OP ENV's hold */
    int16_t bd[8];                                       /* LOFI: the byte's bits, slewed (0..256) */
    uint32_t crawl, cmt;                                 /* .. its crawl (16-bit turns), the time it was taken at */
    int16_t lm;                                          /* .. (0.9.10) the morph to the chip (Q8, gliding to CHIP's; -1 unset) */
    uint8_t lst;                                         /* .. the mock-up frame its samples were last taken (the stutter) */
    int16_t dal, frel;                                   /* lines drawn this much fainter (256 - alpha); OP ENV: the release from */
    int16_t w[3];                                        /* ANIMAL: bird insect mammal weights (0..256) */
    uint8_t age[8], last;                                /* DRUM-X: each lane's age seen; the lane struck last */
    uint32_t hitm[8], tl[8], kms;                        /* .. its last hit (fm1_ms), its being's time; the ms drawn */
    uint8_t kc;                                          /* .. drawing it (its colours: vx_col) */
    uint8_t init;
    uint32_t ph_young;                                   /* PHASE: the youngest voice's age last frame */
    uint32_t ph_t0;                                      /* .. when the fall started (fm1_ms) */
    uint8_t ph_di, ph_fall, ph_n;                        /* .. the target (0..2), falling, keys counted */
} vxs VX_POOL;

/* DRUM-X's beings in the thermal ramp of the mock-up the kit was chosen from (cyan blue violet red orange yellow white:
 * the CYBER palette). GREY and MONO stay grey. Every other palette: the four knob colours, flat, so a cold stroke
 * and a hot one do not melt into one tint. Bands: navy, green, gold, red. */
static const uint8_t VK_HP[8] = {0, 26, 51, 82, 118, 154, 200, 255};
static const uint16_t VK_HC[8] = {0x13D4u, 0x2DFCu, 0x4B5Fu, 0x8B7Fu, 0xFA49u, 0xFC43u, 0xFE87u, 0xFFFFu};
static uint16_t vx_white(int32_t h)
{
    static const uint8_t K[4] = {2, 3, 1, 0};            /* navy, green, gold, red: KNOB 3, 4, 2, 1 */
    return T_K(K[(uint32_t)clamp(h, 0, 255) >> 6]);
}
static uint16_t vx_col(int32_t h)
{
    uint32_t k = 0;
    if (h <= -100) {                                     /* (the Belgian flag: -100 black, -101 yellow, -102 red, as it is) */
        static const uint16_t FL[3] = {RGB(58, 58, 58), RGB(242, 210, 0), RGB(213, 19, 23)};
        uint16_t c = FL[clamp(-h - 100, 0, 2)];
        return ux.mono || settings.palette == UI_GREY_INDEX || settings.palette == UI_BW_INDEX ? ux_gray(ux_luma(c) >> 3) : c;
    }
    if (h < -1)                                          /* (0.9.6: -2 the theme's: the operator picked, the mock-up's) */
        return T_THEME;
    if (h < 0)                                           /* (a muted sound: dim, as the colony's) */
        return ux.mono || settings.palette == UI_BW_INDEX ? ux_gray(ux_luma(T_DIM) >> 3) : T_DIM;
    h = clamp(h, 0, 255);
    if (vxs.kc && !ux.mono && settings.palette != UI_JIANT_INDEX && settings.palette != UI_GREY_INDEX &&
        settings.palette != UI_BW_INDEX)
        return vx_white(h);
    if (!vxs.kc || settings.palette != UI_JIANT_INDEX)
        return bg_col(h);
    while (k < 6u && h > VK_HP[k + 1u]) k++;
    return ux_mix(VK_HC[k], VK_HC[k + 1u], (h - VK_HP[k]) * 100 / (VK_HP[k + 1u] - VK_HP[k]));
}
/* (0.9.7) HOME's beings (ui_lzhome.c, variant A): the same ramp on JIANT, every other palette as the beings (bg_col) */
static uint16_t vk_heat(int32_t h)
{
    uint32_t k = 0;
    h = clamp(h, 0, 255);
    if (settings.palette != UI_JIANT_INDEX)
        return bg_col(h);
    while (k < 6u && h > VK_HP[k + 1u]) k++;
    return ux_mix(VK_HC[k], VK_HC[k + 1u], (h - VK_HP[k]) * 100 / (VK_HP[k + 1u] - VK_HP[k]));
}
static void vx_line(const vxf_t *f, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t h)
{
    int32_t X0 = f->cx + ((x0 * f->s) >> 8), Y0 = f->cy + ((y0 * f->s) >> 8);
    int32_t X1 = f->cx + ((x1 * f->s) >> 8), Y1 = f->cy + ((y1 * f->s) >> 8);
    int32_t a = 256 - vxs.dal, dx = X1 - X0, dy = Y1 - Y0, ax = dx < 0 ? -dx : dx, ay = dy < 0 ? -dy : dy, n = ax > ay ? ax : ay;
    uint16_t c = vx_col(h);
    /* (BRUSSELS) a second stroke, one pixel beside the line, in the text colour: dark ink on a light
     * screen, light ink on a dark one. The heat colour stays on the line itself. The flag keeps its own.
     * WHITE is one stroke only: the ligne claire, in the parameter colours. */
    if (vxs.kc && n >= 16 && settings.palette != UI_WHITE_INDEX) {
        int32_t ox = (-dy << 4) / n, oy = (dx << 4) / n;
        og_line_a(X0 + ox, Y0 + oy, X1 + ox, Y1 + oy, h <= -100 ? c : T_TEXT, a);
    }
    og_line_a(X0, Y0, X1, Y1, c, a);
}
/* a smooth curve through n points (Q4 units, x y pairs): quadratic arcs from midpoint to midpoint, each point their
 * control; closed or open (from the first point to the last). vx_bez: its 1 + 3 e points into o (e: its arcs, returned) */
static uint32_t vx_bez(const int32_t *p, uint32_t n, int closed, int32_t *o)
{
    uint32_t i, k, e = closed ? n : n - 1u;
    for (i = 0; i < e; i++, o += 6) {
        const int32_t *a = &p[2u * (i ? i - 1u : (closed ? n - 1u : 0u))], *b = &p[2u * i], *c = &p[2u * ((i + 1u) % n)];
        int32_t x0 = (a[0] + b[0]) / 2, y0 = (a[1] + b[1]) / 2, x1 = (b[0] + c[0]) / 2, y1 = (b[1] + c[1]) / 2;
        if (!closed && i == 0u) { x0 = b[0]; y0 = b[1]; }
        if (!closed && i + 1u == e) { x1 = c[0]; y1 = c[1]; }
        if (i == 0u) { o[0] = x0; o[1] = y0; }
        for (k = 1; k <= 3u; k++) {
            int32_t t = (int32_t)k * 85, u = 256 - t;   /* (t / 256: thirds) */
            o[2u * k] = k == 3u ? x1 : (u * u * x0 + 2 * u * t * b[0] + t * t * x1) >> 16;
            o[2u * k + 1u] = k == 3u ? y1 : (u * u * y0 + 2 * u * t * b[1] + t * t * y1) >> 16;
        }
    }
    return e;
}
static void vx_curve(const vxf_t *f, const int32_t *p, uint32_t n, int closed, int32_t h)
{
    int32_t o[2 * 49];                                   /* (16 points closed at most) */
    uint32_t i, e = 3u * vx_bez(p, n, closed, o);
    for (i = 0; i < e; i++)
        vx_line(f, o[2u * i], o[2u * i + 1u], o[2u * i + 2u], o[2u * i + 3u], h);
}
/* an ellipse (Q4 units), n sides, its top at angle a0 .. a1 (16-bit turns) */
static void vx_arc(const vxf_t *f, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t a0, uint32_t a1, uint32_t n,
                   int32_t h)
{
    uint32_t i;
    int32_t px = 0, py = 0;
    for (i = 0; i <= n; i++) {
        uint32_t a = a0 + (uint32_t)(((uint64_t)(a1 - a0) * i) / n);
        int32_t x = cx + ((rx * og_cos(a)) >> 15), y = cy + ((ry * og_sin(a)) >> 15);
        if (i) vx_line(f, px, py, x, y, h);
        px = x;
        py = y;
    }
}
static uint32_t vx_rnd(uint32_t *r) { *r ^= *r << 13; *r ^= *r >> 17; *r ^= *r << 5; return *r; }
/* the level of track t now (0..256: its part's peak, slewed: a hit at once, a quick fall), k: which drawing; its time
 * runs with it (once a frame) */
static int32_t vx_level(const track_t *t, uint32_t k, int32_t en_home)
{
    uint32_t dt = ui.frame - vxs.fr[k], dm = fm1_ms - vxs.ms[k];
    int32_t v;
    if (!dt)
        return vxs.en[k];
    if (dt > 8u) dt = 8u;
    vxs.fr[k] = ui.frame;
    vxs.ms[k] = fm1_ms;
    if (dm > 100u) dm = 100u;                            /* (a pause: no jump) */
    if (vxs.en[k])                                       /* (0.9.6) the mock-up's time: 30 frames a second while it sounds */
        vxs.mt[k] += dm * 7680u / 1000u;
    if (ui_prefs & PREF_ANIM_OFF)
        return vxs.en[k] = 0;                            /* (ANIM OFF: at rest) */
    if (en_home >= 0)
        v = en_home;                                     /* (HOME: its own, eco_step's) */
    else {
        v = ((track_t *)t)->peak * 256 / 7000;
        ((track_t *)t)->peak = 0;
        v = v > 256 ? 256 : v;
        v = v > vxs.en[k] ? v : vxs.en[k] + (v - vxs.en[k] - 2) / 3;
    }
    v = v > 4 ? v : 0;                                   /* (HOME's level rests a little above 0: silence) */
    vxs.en[k] = v;
    if (v)
        vxs.tm[k] += dt * (uint32_t)(120 + v * 4);
    return v;
}

/* ------------------------------------------------------------------------------------------------ DRUM-X --- */
/* the kit as a colony of small beings (units ~124 x 90: x right, y down, the floor at 40), each drum one with eyes and
 * a mouth: the kick large in the middle, the snare flat on its stand, the toms on little legs, the hi-hat two shells,
 * the crash on a long stem, the cowbell a small head over the kick, the clap two little ones clapping */
enum { VK_KICK, VK_SNARE, VK_CLAP, VK_HATC, VK_HATO, VK_TOM, VK_RIM, VK_BELL };
/* 256 * 2^(-ms / half), Q8 (the squash's fall) */
static int32_t vx_fall(uint32_t ms, uint32_t half)
{
    uint32_t n = ms / half;
    int32_t v;
    if (n > 8u) return 0;
    v = 256 >> n;
    return v - (int32_t)((uint32_t)v * (ms % half) / (2u * half));
}
/* a being's outline (ui_being.c bg_outline's, its lobes 3, the spikes' sharpness 4), squashed: at (x, y), radius r
 * (Q4 units), x stretched by ax / 256, then scaled sx sy / 256 about x and its foot (by: y's place after the squash) */
typedef struct { int32_t x, y, by, ax, sx, sy, sp, len, wob, rip, lob, shp; } vxb_t;   /* (0.9.6: lob lobes, shp 4 or 1) */
static void vx_blob(const vxf_t *f, const vxb_t *b, int32_t r, int outer, uint32_t t, uint32_t nv, int32_t h)
{
    uint32_t i, lb = (uint32_t)b->lob;
    int32_t px = 0, py = 0;
    for (i = 0; i <= nv; i++) {
        uint32_t th = (i % nv) * 65536u / nv;
        int32_t rr = r + ((((r * b->wob) >> 8) * ((9175 * og_sin(lb * th + t) + 3932 * og_sin((lb + 2u) * th - 2u * t)) >> 15)) >> 15), x, y;
        if (outer) {
            int32_t c2 = og_sin(((uint32_t)b->sp * th >> 1) + 0x4000u), p = c2 > 0 ? c2 : 0;
            if (b->shp > 1) {
                p = (p * p) >> 15;
                p = (p * p) >> 15;                       /* (c2^4) */
            }
            rr += (((((r * b->len) >> 8) * p) >> 15) * 26) >> 4;
            rr += ((((r * b->rip) >> 8) * og_sin(th * 18u + 3u * t)) >> 15) / 14;
        }
        x = b->x + (((((rr * og_cos(th)) >> 15) * b->ax) >> 8) * b->sx >> 8);
        y = b->by + ((((rr * og_sin(th)) >> 15) * b->sy) >> 8);
        if (i) vx_line(f, px, py, x, y, h);
        px = x;
        py = y;
    }
}
/* a small dot at (x, y) (Q4 units), radius r (Q4 px: under 1.5 px a 2 x 2 dot) */
static void vx_dot(const vxf_t *f, int32_t x, int32_t y, int32_t r, int32_t h)
{
    uint16_t c = vx_col(h);
    int32_t X = f->cx + ((x * f->s) >> 8), Y = f->cy + ((y * f->s) >> 8);
    if (r >= 32) {
        og_node(X, Y, r >> 4, c);
        return;
    }
    X = (X - 8) >> 4;
    Y = (Y - 8) >> 4;
    for (r = 0; r < 4; r++)
        og_plot(X + (r & 1), Y + (r >> 1), c, 256);
}
/* the bichos' face (DRUM-X's beings, the 0.9.6 creatures): two eyes ex either side of x at ey (shut: short lines,
 * else dots of er Q4 px), the mouth at my: a line mw either side, or open (mo) an ellipse mx wide; their heats */
static void vx_face(const vxf_t *f, int32_t x, int32_t ey, int32_t ex, int32_t er, int shut, int32_t my, int32_t mw,
                    int32_t mo, int32_t mx, int32_t he, int32_t hm, int32_t hl)
{
    int32_t k;
    for (k = -1; k <= 1; k += 2) {
        if (shut)
            vx_line(f, x + k * ex - 22, ey, x + k * ex + 22, ey, he);
        else
            vx_dot(f, x + k * ex, ey, er, he);
    }
    if (mo)
        vx_arc(f, x, my + mo / 2, mx, mo, 0, 65536u, 12, hm);
    else
        vx_line(f, x - mw, my, x + mw, my, hl);
}
/* .. as the mock-up's bface: at (x, y) for a body of radius r (Q4 units), m its shout (0..256), shut its eyes; hot
 * warms the mouth (0..256) */
static void vx_bface(const vxf_t *f, int32_t x, int32_t y, int32_t r, int32_t m, int shut, int32_t hot)
{
    int32_t ey = y - r / 10, mo = clamp(m, 0, 256) * r * 42 / 25600, er = ((r * f->s) >> 8) / 10;
    vx_face(f, x, ey, r * 36 / 100, er, shut, mo > 8 ? ey + r * 36 / 100 : ey + r * 34 / 100, r * 14 / 100,
            mo > 8 ? mo : 0, r * 16 / 100 + mo * 22 / 100, 243, 205 + hot / 5, 154);
}
/* the angle (16-bit turns) of t * r radians, t the mock-up's frames (mt, Q8), r = r1000 / 1000 */
static uint32_t vx_ang(uint32_t mt, int32_t r1000)
{
    return mt * (uint32_t)r1000 * 2670u >> 16;          /* (mod 2^32: an angle's 16 bits exact) */
}
/* the squash of a hit (Q8): e (0..256) at age ms (22 rad/s, e^-6t) */
static int32_t vx_sq(int32_t e, uint32_t age)
{
    return (((e * og_cos(age * 229u)) >> 15) * vx_fall(age, 115u)) >> 8;
}
/* (BRUSSELS) the kit's drawings are the planche's DRUM card (docs/creatures/creatures.js): coordinates in the card's px
 * (1 px = 8 Q4 units), about (120, 150); a thing struck squashes about its foot (ox, oy: Q4) by sx, sy (Q8) */
#define VK_X(X) (((X) - 120) * 8)
#define VK_Y(Y) (((Y) - 150) * 8 + 80)
typedef struct { vxf_t f; int32_t ox, oy, sx, sy; } vxk_t;
static void vk_pt(const vxk_t *k, int32_t x, int32_t y, int32_t *X, int32_t *Y)
{
    *X = k->ox + (((x - k->ox) * k->sx) >> 8);
    *Y = k->oy + (((y - k->oy) * k->sy) >> 8);
}
static void vx_line_c(const vxf_t *f, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t c);
static uint16_t vk_ink;                              /* flat colour for one character; 0 keeps the heat */
static void vk_line(const vxk_t *k, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t h)
{
    int32_t a, b, c, d;
    vk_pt(k, x0, y0, &a, &b);
    vk_pt(k, x1, y1, &c, &d);
    if (vk_ink) vx_line_c(&k->f, a, b, c, d, vk_ink);
    else vx_line(&k->f, a, b, c, d, h);
}
/* the planche's flat fries: yellow, orange, red, teal, blue. CYBER keeps its ramp; GREY and MONO stay grey. */
static const uint16_t VK_FRY[5] = {RGB(242, 196, 0), RGB(240, 120, 24), RGB(210, 32, 40), RGB(24, 160, 150), RGB(36, 96, 200)};
static int vk_mix(void)
{
    return vxs.kc && !ux.mono && settings.palette != UI_JIANT_INDEX && settings.palette != UI_GREY_INDEX &&
           settings.palette != UI_BW_INDEX;
}
static void vx_line_c(const vxf_t *f, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t c)
{
    og_line_a(f->cx + ((x0 * f->s) >> 8), f->cy + ((y0 * f->s) >> 8), f->cx + ((x1 * f->s) >> 8), f->cy + ((y1 * f->s) >> 8), c, 256 - vxs.dal);
}
static void vk_line_c(const vxk_t *k, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t c)
{
    int32_t a, b, d, e;
    vk_pt(k, x0, y0, &a, &b);
    vk_pt(k, x1, y1, &d, &e);
    vx_line_c(&k->f, a, b, d, e, c);
}
/* a flat block in the card's px (w by h), one line per row, so a closed shape reads as a colour, not a contour */
static void vk_cell(const vxk_t *k, int32_t px, int32_t py, int32_t w, int32_t h, uint16_t c)
{
    int32_t x0 = VK_X(px), x1 = VK_X(px + w), y0 = VK_Y(py), y1 = VK_Y(py + h), y;
    if (y1 < y0) { int32_t t = y0; y0 = y1; y1 = t; }
    for (y = y0; y <= y1; y += 8) vk_line_c(k, x0, y, x1, y, c);
}
/* a polyline of n points (px pairs, VK_X / VK_Y'd by the caller: Q4), closed or not */
static void vk_poly(const vxk_t *k, const int32_t *p, uint32_t n, int closed, int32_t h)
{
    uint32_t i;
    for (i = 0; i + 1u < n; i++) vk_line(k, p[2 * i], p[2 * i + 1], p[2 * i + 2], p[2 * i + 3], h);
    if (closed && n > 2u) vk_line(k, p[2 * n - 2], p[2 * n - 1], p[0], p[1], h);
}
/* an ellipse arc (Q4 centre and radii, angles in 16-bit turns) */
static void vk_ell(const vxk_t *k, int32_t cx, int32_t cy, int32_t rx, int32_t ry, uint32_t a0, uint32_t a1, int32_t h)
{
    uint32_t i, span = a1 - a0, n = (uint32_t)(((rx > ry ? rx : ry) >> 3) * (int32_t)(span >> 4) / 1434);
    int32_t px = 0, py = 0;
    if (n < 6u) n = 6u;
    if (n > 30u) n = 30u;
    for (i = 0; i <= n; i++) {
        uint32_t a = a0 + (uint32_t)(((uint64_t)span * i) / n);
        int32_t x, y;
        vk_pt(k, cx + ((rx * og_cos(a)) >> 15), cy + ((ry * og_sin(a)) >> 15), &x, &y);
        if (i) {
            if (vk_ink) vx_line_c(&k->f, px, py, x, y, vk_ink);
            else vx_line(&k->f, px, py, x, y, h);
        }
        px = x;
        py = y;
    }
}
/* the little ones' faces: at (x, y) (Q4, squashed), size kp px, m its shout (0..256), shut its eyes */
static void vk_face(const vxk_t *k, int32_t x, int32_t y, int32_t kp, int32_t m, int shut, int32_t lit)
{
    int32_t X, Y;
    vk_pt(k, x, y, &X, &Y);
    vx_bface(&k->f, X, Y, kp * 8, m, shut, lit < 0 ? 0 : 60);
}
/* the heat of a lane's drawing (-1: muted), the sound edited a little hotter */
static int32_t vk_h(int32_t lit, int32_t base, int32_t e, int32_t span)
{
    return lit < 0 ? -1 : clamp(base + e * span / 256 + (lit > 0 ? 30 : 0), 0, 255);
}
/* the cone of fries (the planche's BD; ANIMAL's insect form): its fries, the cone, its face; k squashes it about its foot */
static void vk_fries(const vxk_t *k, int32_t lit, int32_t e)
{
    int32_t i;
    /* large enough to read (the kit page, HOME's fries): each fry its own colour, the cone a mosaic, as the planche */
    if (vk_mix() && lit >= 0 && k->f.s >= 256) {
        int32_t py, px;
        for (py = 157; py < 200; py += 3) {
            int32_t t = py - 157, xl = 96 + 24 * t / 43, xr = 144 - 24 * t / 43;
            for (px = xl; px + 2 < xr; px += 3)
                vk_cell(k, px, py, 3, 3, VK_FRY[(px / 3 + py / 3 * 2) % 5]);
        }
        for (i = 0; i < 7; i++) {
            int32_t x = 102 + i * 6, fh = 18 + ((i * 7) % 5) * 3;
            vk_cell(k, x - 2, 158 - fh, 4, fh, VK_FRY[i % 5]);
        }
        vk_face(k, VK_X(120), VK_Y(170), 13, e, e > 90, lit);
        return;
    }
    for (i = 0; i < 7; i++) {
        int32_t fx = VK_X(102 + i * 6), fh = (18 + ((i * 7) % 5) * 3) * 8, p4[8];
        p4[0] = fx - 16; p4[1] = VK_Y(160); p4[2] = fx - 16; p4[3] = VK_Y(158) - fh; p4[4] = fx + 16; p4[5] = VK_Y(158) - fh;
        p4[6] = fx + 16; p4[7] = VK_Y(160);
        vk_poly(k, p4, 4, 0, lit < 0 ? -1 : 225 + (i & 1) * 15);
    }
    {
        int32_t cn[6] = {VK_X(96), VK_Y(157), VK_X(120), VK_Y(200), VK_X(144), VK_Y(157)};
        vk_poly(k, cn, 3, 1, vk_h(lit, 205, e, 40));
    }
    vk_line(k, VK_X(104), VK_Y(164), VK_X(120), VK_Y(190), lit < 0 ? -1 : 120);
    vk_line(k, VK_X(136), VK_Y(164), VK_X(120), VK_Y(190), lit < 0 ? -1 : 120);
    vk_face(k, VK_X(120), VK_Y(170), 13, e, e > 90, lit);
}
/* the Manneken Pis, from the statue's drawing (320 = its axis, 580 = its feet; 1 statue unit = 0.16 px), his jet reaching
 * further with the bell (e, 0..256) */
#define MK_X(x) (VK_X(196) + ((((x) - 320) * 41) >> 5))
#define MK_Y(y, b) (VK_Y(198) + ((((y) - 580) * 41) >> 5) - (b))
static void vk_mk_poly(const vxf_t *f, const int16_t *p, uint32_t n, int closed, int32_t bob, int32_t h)
{
    int32_t q[2 * 12];
    uint32_t i;
    for (i = 0; i < n; i++) { q[2 * i] = MK_X(p[2 * i]); q[2 * i + 1] = MK_Y(p[2 * i + 1], bob); }
    for (i = 0; i + 1u < n; i++) {
        if (vk_ink) vx_line_c(f, q[2 * i], q[2 * i + 1], q[2 * i + 2], q[2 * i + 3], vk_ink);
        else vx_line(f, q[2 * i], q[2 * i + 1], q[2 * i + 2], q[2 * i + 3], h);
    }
    if (closed) {
        if (vk_ink) vx_line_c(f, q[2 * n - 2], q[2 * n - 1], q[0], q[1], vk_ink);
        else vx_line(f, q[2 * n - 2], q[2 * n - 1], q[0], q[1], h);
    }
}
static void vk_mk_ell(const vxf_t *f, int32_t x, int32_t y, int32_t rx, int32_t ry, uint32_t a0, uint32_t a1, int32_t bob, int32_t h)
{
    int32_t px = 0, py = 0, i, n = 12, cx = MK_X(x), cy = MK_Y(y, bob), arx = (rx * 41) >> 5, ary = (ry * 41) >> 5;
    for (i = 0; i <= n; i++) {
        uint32_t a = a0 + (uint32_t)(((uint64_t)(a1 - a0) * (uint32_t)i) / (uint32_t)n);
        int32_t X = cx + ((arx * og_cos(a)) >> 15), Y = cy + ((ary * og_sin(a)) >> 15);
        if (i) {
            if (vk_ink) vx_line_c(f, px, py, X, Y, vk_ink);
            else vx_line(f, px, py, X, Y, h);
        }
        px = X;
        py = Y;
    }
}
static void vk_manneken(const vxf_t *f, int32_t e, int32_t lit)
{
    static const int16_t BASE[8] = {218, 625, 250, 584, 422, 584, 415, 625}, SH[16] = {296, 226, 252, 228, 208, 238, 170, 256, 152, 280, 160, 300, 196, 326, 236, 352},
        AR[6] = {212, 262, 226, 290, 244, 318}, TO[12] = {344, 212, 384, 236, 408, 270, 418, 320, 412, 350, 392, 372},
        TI[10] = {400, 262, 412, 300, 410, 336, 386, 362, 358, 374}, NOSE[6] = {360, 160, 369, 178, 358, 182},
        L1[10] = {248, 380, 246, 420, 252, 460, 262, 520, 264, 566}, L2[8] = {324, 412, 324, 470, 322, 530, 324, 566},
        L3[8] = {392, 404, 392, 450, 386, 520, 376, 566},
        F1[12] = {264, 566, 258, 578, 264, 584, 314, 584, 316, 575, 304, 566},
        F2[12] = {336, 566, 334, 578, 340, 584, 398, 584, 396, 574, 376, 566}, NK[4] = {292, 212, 296, 226};
    static const int16_t HW[17] = {256, 307, 272, 307, 280, 305, 286, 303, 291, 301, 296, 298, 299, 294, 302, 289, 304};
    int32_t bob = ((og_sin(fm1_ms * 40u) * 16) >> 15) + ((e * 36) >> 8), c = vk_h(lit, 205, e, 40), c2 = lit < 0 ? -1 : 160, i, x0, y0, x1, y1, x2, y2, x3, y3, px = 0, py = 0;
    if (ui_prefs & PREF_ANIM_OFF) bob = 0;
    vk_ink = vk_mix() && lit >= 0 ? VK_FRY[1] : 0;       /* plinth orange */
    vk_mk_poly(f, BASE, 4, 1, bob, lit < 0 ? -1 : 120);        /* the plinth, anchored right under his feet */
    if (vk_ink) vk_ink = VK_FRY[3];                      /* the body teal, not the same yellow as the rest */
    vk_mk_ell(f, 322, 150, 62, 66, 0, 65536u, bob, c);        /* the head, its curls in waves over the top and the back */
    for (i = 0; i <= 16; i++) {
        uint32_t a = (uint32_t)(31130 - i * 2253);
        int32_t x = MK_X(322 + ((62 * HW[i] >> 8) * og_cos(a) >> 15)), y = MK_Y(150 + ((66 * HW[i] >> 8) * og_sin(a) >> 15), bob);
        if (i) {
            if (vk_ink) vx_line_c(f, px, py, x, y, VK_FRY[0]);
            else vx_line(f, px, py, x, y, c);
        }
        px = x;
        py = y;
    }
    vk_mk_ell(f, 280, 166, 7, 10, 5215u, 43807u, bob, c);     /* the ear, the cheek */
    vk_mk_ell(f, 368, 184, 8, 12, (uint32_t)-10430, 10430u, bob, lit < 0 ? -1 : 170);
    vk_mk_ell(f, 338, 158, 6, 3, 32768u, 65536u, bob, c);     /* eyes, nose, mouth */
    vk_mk_ell(f, 356, 162, 5, 3, 32768u, 65536u, bob, c);
    vk_mk_poly(f, NOSE, 3, 0, bob, c);
    vk_mk_ell(f, 350, 196, 6, 3, 2086u, 30682u, bob, c);
    vk_mk_poly(f, NK, 2, 0, bob, c);                          /* the neck, the shoulder, the left arm on his hip */
    vk_mk_poly(f, SH, 8, 0, bob, c);
    vk_mk_poly(f, AR, 3, 0, bob, c2);
    vk_mk_ell(f, 250, 360, 16, 12, 0, 65536u, bob, c);
    vk_mk_poly(f, TO, 6, 0, bob, c);                          /* the torso, the right arm in front */
    vk_mk_poly(f, TI, 5, 0, bob, c2);
    vk_mk_ell(f, 356, 376, 14, 9, 0, 65536u, bob, c);
    vk_mk_poly(f, L1, 5, 0, bob, c);                          /* the legs, the feet */
    vk_mk_poly(f, L2, 4, 0, bob, c);
    vk_mk_poly(f, L3, 4, 0, bob, c);
    vk_mk_poly(f, F1, 6, 0, bob, c);
    vk_mk_poly(f, F2, 6, 0, bob, c);
    x0 = MK_X(360); y0 = MK_Y(380, bob);                       /* the jet: from his hand, reaching with the bell */
    x1 = MK_X(392 + 10 * e / 256); y1 = MK_Y(402, bob);
    x2 = MK_X(424 + 16 * e / 256); y2 = MK_Y(480 + 30 * e / 256, bob);
    x3 = MK_X(442 + 46 * e / 256); y3 = MK_Y(558 + 70 * e / 256, bob);
    for (i = 0; i <= 10; i++) {
        int32_t u = i * 256 / 10, v = 256 - u;
        int32_t a = (v * v >> 8) * v >> 8, b = 3 * (v * v >> 8) * u >> 8, d = 3 * (u * u >> 8) * v >> 8, g = (u * u >> 8) * u >> 8;
        int32_t x = (a * x0 + b * x1 + d * x2 + g * x3) >> 8, y = (a * y0 + b * y1 + d * y2 + g * y3) >> 8;
        if (i) {
            if (vk_ink) vx_line_c(f, px, py, x, y, VK_FRY[4]);
            else vx_line(f, px, py, x, y, vk_h(lit, 150, e, 90));
        }
        px = x;
        py = y;
    }
    vk_ink = 0;
}
/* the kit at (cx, cy) Q4 px, s: a unit's px (Q8); sel: the sound edited (lit), -1 none */
static void vx_kit(const track_t *t, int32_t cx, int32_t cy, int32_t s, int32_t en_home, int32_t sel)
{
    vxf_t f = {cx, cy, s};
    drum_lane_t *K = drum_kit_of(t);
    int32_t e[8], lt[8], en = vx_level(t, 2, en_home), i;
    uint32_t ag[8], now = fm1_ms, dt = now - vxs.kms;
    if (dt > 100u) dt = 100u;                            /* (a pause: no jump) */
    vxs.kms = now;
    vxs.kc = 1;
    for (i = 0; i < 8; i++) {                            /* each lane's hit now: its envelope (velocity, accent, decay) */
        int32_t a = K ? K[i].age : 255;
        lt[i] = dx_lane_muted((uint32_t)i) ? -1 : i == sel;
        e[i] = K && K[i].x.live && lt[i] >= 0 && !(ui_prefs & PREF_ANIM_OFF) ? clamp(K[i].x.ea >> 22, 0, 256) : 0;
        if (K && K[i].dm == 2u) e[i] = e[i] * 2 / 3;   /* (GHOST: a small squash) */
        if (K && a < vxs.age[i] && e[i]) {               /* a new hit (a FLAM's two, a ratchet's each) */
            vxs.hitm[i] = now;
            vxs.last = (uint8_t)i;
        }
        vxs.age[i] = (uint8_t)a;
        ag[i] = now - vxs.hitm[i] < 4000u ? now - vxs.hitm[i] : 4000u;
        if (en || e[i])                                  /* (its time runs with the sound: in silence it rests) */
            vxs.tl[i] += dt * (uint32_t)(60 * 256 + e[i] * 300) * 30u / 256000u;
    }
    {   /* (BRUSSELS) the planche's kit: the mussels (CH, OH), the waffle (SD), the praline (CP), the choux (TM), the spark (RS),
         * the fries cone (BD), the sauce bottle pouring over them, Manneken Pis (CB) */
        vxk_t k;
        int32_t mo = e[VK_HATC] > e[VK_HATO] ? e[VK_HATC] : e[VK_HATO], lift = e[VK_HATO] * 72 / 256, sq, c, x, y, hh;
        uint32_t mt = vxs.mt[2];
        k.f = f;
        vx_line(&f, VK_X(8), VK_Y(206), VK_X(232), VK_Y(206), 70);                       /* the floor */
        /* the mussels, on their stem */
        vk_ink = vk_mix() && lt[VK_HATC] >= 0 ? VK_FRY[3] : 0;
        if (vk_ink) vx_line_c(&f, VK_X(26), VK_Y(206), VK_X(26), VK_Y(148), VK_FRY[3]);
        else vx_line(&f, VK_X(26), VK_Y(206), VK_X(26), VK_Y(148), lt[VK_HATC] < 0 && lt[VK_HATO] < 0 ? -1 : 95);
        {
            int32_t lm = lt[VK_HATO] < 0 && lt[VK_HATC] < 0 ? -1 : lt[VK_HATO] > 0 || lt[VK_HATC] > 0;
            sq = e[VK_HATC] * 6 / 10 ? vx_sq(e[VK_HATC] * 6 / 10, ag[VK_HATC]) : 0;
            k.ox = VK_X(26); k.oy = VK_Y(146); k.sx = 256 + sq * 36 / 256; k.sy = 256 - sq * 51 / 256;
            c = vk_h(lm, 150, mo, 90);
            vk_ell(&k, VK_X(26), VK_Y(144), 16 * 8, 9 * 8, 2086u, 30682u, c);
            for (i = 1; i <= 3; i++)
                vk_ell(&k, VK_X(26), VK_Y(144), (16 * 8 * 10 - i * 34 * 8) / 10, (9 * 8 * 10 - i * 16 * 8) / 10, 4693u, 28075u, lm < 0 ? -1 : 115);
            vk_ell(&k, VK_X(26), VK_Y(142) - lift, 16 * 8, 7 * 8, 34854u, 63450u, c);
            vk_ell(&k, VK_X(26), VK_Y(142) - lift, 10 * 8, 4 * 8, 37461u, 60843u, lm < 0 ? -1 : 115);
            if (vk_ink) vk_ink = VK_FRY[4];
            vk_ell(&k, VK_X(26), VK_Y(144), (16 * 8 * 10 - 2 * 34 * 8) / 10, (9 * 8 * 10 - 2 * 16 * 8) / 10, 4693u, 28075u, lm < 0 ? -1 : 115);
        }
        /* the waffle (SD): a regular grid, its face above so the squares stay clear */
        {
            int32_t kx0 = VK_X(44), ky0 = VK_Y(158), kw = 36 * 8, kh = 22 * 8, p4[8];
            vk_ink = vk_mix() && lt[VK_SNARE] >= 0 ? VK_FRY[0] : 0;
            if (vk_ink) vx_line_c(&f, VK_X(62), VK_Y(176), VK_X(54), VK_Y(206), VK_FRY[2]);
            else vx_line(&f, VK_X(62), VK_Y(176), VK_X(54), VK_Y(206), lt[VK_SNARE] < 0 ? -1 : 95);
            if (vk_ink) vx_line_c(&f, VK_X(62), VK_Y(176), VK_X(70), VK_Y(206), VK_FRY[2]);
            else vx_line(&f, VK_X(62), VK_Y(176), VK_X(70), VK_Y(206), lt[VK_SNARE] < 0 ? -1 : 95);
            if (vk_ink) vx_line_c(&f, VK_X(62), VK_Y(176), VK_X(62), VK_Y(206), VK_FRY[2]);
            else vx_line(&f, VK_X(62), VK_Y(176), VK_X(62), VK_Y(206), lt[VK_SNARE] < 0 ? -1 : 95);
            sq = e[VK_SNARE] ? vx_sq(e[VK_SNARE], ag[VK_SNARE]) : 0;
            k.ox = VK_X(62); k.oy = VK_Y(176); k.sx = 256 + sq * 36 / 256; k.sy = 256 - sq * 51 / 256;
            if (vk_ink) vk_cell(&k, 44, 158, 36, 22, VK_FRY[0]);
            p4[0] = kx0; p4[1] = ky0; p4[2] = kx0 + kw; p4[3] = ky0; p4[4] = kx0 + kw; p4[5] = ky0 + kh; p4[6] = kx0; p4[7] = ky0 + kh;
            vk_poly(&k, p4, 4, 1, vk_h(lt[VK_SNARE], 165, e[VK_SNARE], 80));
            if (vk_ink) vk_ink = VK_FRY[2];
            for (i = 1; i < 4; i++) vk_line(&k, kx0 + i * kw / 4, ky0, kx0 + i * kw / 4, ky0 + kh, lt[VK_SNARE] < 0 ? -1 : 130);
            for (i = 1; i < 3; i++) vk_line(&k, kx0, ky0 + i * kh / 3, kx0 + kw, ky0 + i * kh / 3, lt[VK_SNARE] < 0 ? -1 : 130);
            vk_face(&k, VK_X(62), VK_Y(150), 8, e[VK_SNARE], e[VK_SNARE] > 90, lt[VK_SNARE]);
        }
        /* the rim's spark (RS) */
        if (e[VK_RIM] > 25 || lt[VK_RIM] > 0)
            for (i = 0; i < 4; i++) {
                uint32_t a = (uint32_t)i * 16384u / 2u * 2u / 2u;
                int32_t r = (6 * 8 * e[VK_RIM] / 256) + 8;
                vx_line(&f, VK_X(82) - ((r * og_cos(a)) >> 15), VK_Y(150) - ((r * og_sin(a)) >> 15),
                        VK_X(82) + ((r * og_cos(a)) >> 15), VK_Y(150) + ((r * og_sin(a)) >> 15), vk_h(lt[VK_RIM], 200, e[VK_RIM], 50));
            }
        /* the choux (TM), on little legs */
        x = VK_X(92); y = VK_Y(118);
        vk_ink = vk_mix() && lt[VK_TOM] >= 0 ? VK_FRY[1] : 0;
        if (vk_ink) {
            vx_line_c(&f, x - 24, y + 64, x - 48, y + 141, VK_FRY[4]);
            vx_line_c(&f, x + 24, y + 64, x + 48, y + 141, VK_FRY[4]);
        } else {
            vx_line(&f, x - 24, y + 64, x - 48, y + 141, lt[VK_TOM] < 0 ? -1 : 80);
            vx_line(&f, x + 24, y + 64, x + 48, y + 141, lt[VK_TOM] < 0 ? -1 : 80);
        }
        sq = e[VK_TOM] ? vx_sq(e[VK_TOM], ag[VK_TOM]) : 0;
        k.ox = x; k.oy = y + 70; k.sx = 256 + sq * 36 / 256; k.sy = 256 - sq * 51 / 256;
        c = vk_h(lt[VK_TOM], 150, e[VK_TOM], 90);
        vk_ell(&k, x, y, 70, 70, 0, 65536u, c);
        if (vk_ink) vk_ink = VK_FRY[0];
        vk_ell(&k, x, y + 32, 58, 58, 37683u, 60621u, lt[VK_TOM] < 0 ? -1 : 115);
        vk_ell(&k, x - 32, y, 45, 58, (uint32_t)-7301, 8344u, lt[VK_TOM] < 0 ? -1 : 115);
        vk_face(&k, x + 8, y + 8, 7, e[VK_TOM], e[VK_TOM] > 90, lt[VK_TOM]);
        /* the cone of fries (BD) */
        vk_ink = 0;
        vx_line(&f, VK_X(108), VK_Y(206), VK_X(114), VK_Y(188), lt[VK_KICK] < 0 ? -1 : 95);
        vx_line(&f, VK_X(132), VK_Y(206), VK_X(126), VK_Y(188), lt[VK_KICK] < 0 ? -1 : 95);
        sq = e[VK_KICK] ? vx_sq(e[VK_KICK], ag[VK_KICK]) : 0;
        k.ox = VK_X(120); k.oy = VK_Y(200); k.sx = 256 + sq * 36 / 256; k.sy = 256 - sq * 51 / 256;
        vk_fries(&k, lt[VK_KICK], e[VK_KICK]);
        /* the sauce bottle, in the air over the cone: a drop, small puddles on the fries */
        {
            int32_t ang = -8866 + ((og_sin(vx_ang(mt, 1300)) * 626) >> 15), cs = og_cos((uint32_t)ang & 0xFFFFu), sn = og_sin((uint32_t)ang & 0xFFFFu), q[16];
            static const int8_t BB[16] = {-12, -5, 6, -5, 10, -2, 10, 2, 6, 5, -12, 5, -16, 2, -16, -2}, NN[8] = {-16, -3, -16, 3, -22, 1, -22, -1};
            int32_t du = (int32_t)((mt * 7u / 300u) & 255u), bx = VK_X(130), by = VK_Y(100);
            int mix = vk_mix() && lt[VK_BELL] >= 0;
            hh = lt[VK_BELL] < 0 ? -1 : 235;
            for (i = 0; i < 8; i++) {
                q[2 * i] = bx + (((BB[2 * i] * 8 * cs) - (BB[2 * i + 1] * 8 * sn)) >> 15);
                q[2 * i + 1] = by + (((BB[2 * i] * 8 * sn) + (BB[2 * i + 1] * 8 * cs)) >> 15);
            }
            for (i = 0; i < 8; i++) {
                if (mix) vx_line_c(&f, q[2 * i], q[2 * i + 1], q[2 * ((i + 1) & 7)], q[2 * ((i + 1) & 7) + 1], VK_FRY[2]);
                else vx_line(&f, q[2 * i], q[2 * i + 1], q[2 * ((i + 1) & 7)], q[2 * ((i + 1) & 7) + 1], hh);
            }
            for (i = 0; i < 4; i++) {
                int32_t nx = bx + (((NN[2 * i] * 8 * cs) - (NN[2 * i + 1] * 8 * sn)) >> 15), ny = by + (((NN[2 * i] * 8 * sn) + (NN[2 * i + 1] * 8 * cs)) >> 15);
                int32_t mx = bx + (((NN[2 * ((i + 1) & 3)] * 8 * cs) - (NN[2 * ((i + 1) & 3) + 1] * 8 * sn)) >> 15);
                int32_t my = by + (((NN[2 * ((i + 1) & 3)] * 8 * sn) + (NN[2 * ((i + 1) & 3) + 1] * 8 * cs)) >> 15);
                if (mix) vx_line_c(&f, nx, ny, mx, my, VK_FRY[2]);
                else vx_line(&f, nx, ny, mx, my, hh);
            }
            if (mix) {
                int32_t dx = VK_X(116) + 3 * 8 * du / 256, dy = VK_Y(117) + 20 * 8 * du / 256;
                vx_line_c(&f, dx - 8, dy, dx + 8, dy, VK_FRY[1]);
                vx_line_c(&f, VK_X(120) - 36, VK_Y(142), VK_X(120) + 36, VK_Y(142), VK_FRY[1]);
                vx_line_c(&f, VK_X(112) - 24, VK_Y(145), VK_X(112) + 24, VK_Y(145), VK_FRY[1]);
                vx_line_c(&f, VK_X(128) - 24, VK_Y(145), VK_X(128) + 24, VK_Y(145), VK_FRY[2]);
            } else {
                vx_dot(&f, VK_X(116) + 3 * 8 * du / 256, VK_Y(117) + 20 * 8 * du / 256, 22, lt[VK_BELL] < 0 ? -1 : 175);
                vx_dot(&f, VK_X(118), VK_Y(124), 16, lt[VK_BELL] < 0 ? -1 : 150);
                vx_arc(&f, VK_X(120), VK_Y(142), 36, 13, 0, 65536u, 10, lt[VK_BELL] < 0 ? -1 : 175);
                vx_arc(&f, VK_X(112), VK_Y(145), 29, 10, 0, 65536u, 8, lt[VK_BELL] < 0 ? -1 : 175);
                vx_arc(&f, VK_X(128), VK_Y(145), 29, 10, 0, 65536u, 8, lt[VK_BELL] < 0 ? -1 : 165);
            }
        }
        /* a single praline at the foot (CP) */
        vk_ink = vk_mix() && lt[VK_CLAP] >= 0 ? VK_FRY[2] : 0;
        sq = e[VK_CLAP] ? vx_sq(e[VK_CLAP], ag[VK_CLAP]) : 0;
        k.ox = VK_X(164); k.oy = VK_Y(200); k.sx = 256 + sq * 36 / 256; k.sy = 256 - sq * 51 / 256;
        {
            int32_t p4[8] = {VK_X(159), VK_Y(190), VK_X(169), VK_Y(190), VK_X(170), VK_Y(200), VK_X(158), VK_Y(200)};
            vk_poly(&k, p4, 4, 1, vk_h(lt[VK_CLAP], 150, e[VK_CLAP], 90));
        }
        vk_line(&k, VK_X(161), VK_Y(193), VK_X(167), VK_Y(193), lt[VK_CLAP] < 0 ? -1 : 115);
        vx_dot(&f, VK_X(161), VK_Y(196), 13, lt[VK_CLAP] < 0 ? -1 : 240);
        vx_dot(&f, VK_X(166), VK_Y(196), 13, lt[VK_CLAP] < 0 ? -1 : 240);
        /* Manneken Pis, his jet reaching with the bell */
        vk_manneken(&f, e[VK_BELL], lt[VK_BELL]);
    }
    vk_ink = 0;
    /* a FLAM's hit to come: the being's shadow, doubled */
    for (i = 1; i < 8; i++)
        if (K && K[i].fl) {
            static const int16_t PX[8] = {120, 62, 164, 26, 26, 92, 82, 196}, PY[8] = {178, 168, 195, 144, 138, 118, 150, 160};
            vx_arc(&f, VK_X(PX[i]) + 2 * 16, VK_Y(PY[i]) + 16, 6 * 16, 3 * 16, 0, 65536u, 12, 64);
        }
    vxs.kc = 0;
}

/* ------------------------------------------------------------------------------------------------ ANIMAL --- */
/* the three animals, the same points in each (units, facing left, ~100 x 70): the outline (16, from the beak / snout
 * / mouthparts' tip round the head, the back, the tail, the belly, the throat, the chin), four limbs (hip knee foot:
 * a bird's two legs, the others folded into its belly; an insect's front, middle and jumping legs; a mammal's four), the
 * wing (5: a bird's folded wing, an insect's forewings, a mammal's ribcage), two head strokes (3: a crest, antennae,
 * ears), the eye, the mouth's corner, the throat's organ (syrinx, tymbal, larynx) */
enum { VA_OUT = 0, VA_LIMB = 32, VA_WING = 56, VA_HEAD = 66, VA_EYE = 78, VA_MTH = 80, VA_ORG = 82, VA_NP = 84 };
static const int8_t VX_AN[3][VA_NP] = {
    {   /* a bird: the rooster (BRUSSELS, the planche's ANIMAL card): a high head, its comb, a long neck, a leaf-like wing, two long legs; the sickle tail is drawn apart */
        -50, -24, -44, -30, -37, -34, -31, -29, -26, -18, -16, -8, 2, -7, 16, -3, 22, 5, 18, 12, 8, 18, -8, 20, -22, 15, -32, 4,
        -37, -11, -45, -21,
        -12, 17, -9, 27, -14, 36, 0, 17, 3, 27, -2, 36, 2, 18, 2, 18, 2, 18, 4, 17, 4, 17, 4, 17,
        -22, -4, -8, -10, 16, -3, 2, 8, -14, 5,
        -42, -31, -40, -40, -37, -32, -37, -32, -34, -39, -31, -30,
        -39, -27, -45, -22, -41, -16},
    {   /* an insect (a cricket) */
        -46, 3, -44, -8, -37, -13, -29, -11, -21, -13, 0, -11, 20, -9, 36, -5, 50, 0, 37, 4, 22, 8, 2, 8, -18, 7, -28, 6,
        -35, 7, -43, 7,
        -24, 6, -33, 16, -40, 25, -14, 7, -10, 19, -18, 28, 2, 6, 24, -12, 36, 26, -8, 7, -2, 18, 4, 27,
        -22, -13, 0, -16, 42, -7, 30, -2, -16, -7,
        -43, -9, -32, -36, 8, -46, -41, -9, -22, -32, 24, -38,
        -38, -5, -42, 4, 18, -2},
    {   /* a mammal (a fox) */
        -52, -6, -38, -17, -29, -22, -20, -17, -10, -15, 8, -16, 22, -15, 30, -15, 52, -19, 35, -2, 22, 6, 2, 8, -14, 6, -22, 2,
        -31, -1, -45, -1,
        -14, 4, -17, 18, -15, 32, -8, 4, -9, 18, -6, 32, 18, 4, 26, 17, 20, 32, 23, 4, 31, 17, 26, 32,
        -14, -9, 0, -11, 12, -7, 6, 5, -10, 4,
        -28, -21, -26, -33, -20, -20, -24, -20, -19, -31, -15, -17,
        -36, -13, -38, -3, -27, 2},
};
static const uint8_t VA_EYE_R[3] = {3, 5, 2};
/* MODE -> the animal on the line insect 0 .. bird 128 .. mammal 256; the middle of its presets' SIZE */
static const uint8_t VA_POS[AN_NMODE] = {128, 0, 128, 0, 255}, VA_MID[AN_NMODE] = {64, 30, 50, 70, 64};   /* (.. its presets' SIZE) */
/* the side rooster from the HTML trial (docs/coq.html): one filled bird, html px about (124, 140) */
#define CQX(X) ((((X) - 124) * 1440) / 192)
#define CQY(Y) ((((Y) - 140) * 1120) / 144)
static int32_t cq_isqrt(int32_t n)
{
    int32_t x, y;
    if (n <= 0) return 0;
    x = n;
    y = 1;
    while (x > y) { x = (x + y) >> 1; y = n / (x ? x : 1); }
    return x;
}
static void cq_ell(const vxf_t *f, int32_t hx, int32_t hy, int32_t hrx, int32_t hry, uint16_t c)
{
    int32_t cx = CQX(hx), cy = CQY(hy), rx = hrx * 1440 / 192, ry = hry * 1120 / 144, y;
    int32_t step = f->s > 64 ? 4096 / f->s : 8;
    if (step < 4) step = 4;
    if (rx < 8) rx = 8;
    if (ry < 8) ry = 8;
    for (y = -ry; y <= ry; y += step) {
        int32_t num = ry * ry - y * y, w = num > 0 ? rx * cq_isqrt(num) / ry : 0;
        vx_line_c(f, cx - w, cy + y, cx + w, cy + y, c);
    }
}
static void cq_seg(const vxf_t *f, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t c)
{
    int32_t ax = CQX(x0), ay = CQY(y0), bx = CQX(x1), by = CQY(y1);
    int32_t dx = bx - ax, dy = by - ay, len = cq_isqrt(dx * dx + dy * dy), ox, oy;
    if (len < 1) len = 1;
    ox = -dy * 48 / len;
    oy = dx * 48 / len;
    vx_line_c(f, ax - ox, ay - oy, bx - ox, by - oy, c);
    vx_line_c(f, ax, ay, bx, by, c);
    vx_line_c(f, ax + ox, ay + oy, bx + ox, by + oy, c);
}
static void cq_sickle(const vxf_t *f, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint16_t c)
{
    int32_t i, px = CQX(x0), py = CQY(y0), X0 = px, Y0 = py, X1 = CQX(x1), Y1 = CQY(y1), X2 = CQX(x2), Y2 = CQY(y2), X3 = CQX(x3), Y3 = CQY(y3);
    for (i = 1; i <= 8; i++) {
        int32_t u = i * 32, v = 256 - u, a = (v * v >> 8) * v >> 8, b = 3 * ((v * v >> 8) * u >> 8);
        int32_t d = 3 * ((u * u >> 8) * v >> 8), g = (u * u >> 8) * u >> 8;
        int32_t x = (a * X0 + b * X1 + d * X2 + g * X3) >> 8, y = (a * Y0 + b * Y1 + d * Y2 + g * Y3) >> 8;
        int32_t dx = x - px, dy = y - py, len = cq_isqrt(dx * dx + dy * dy), ox, oy;
        if (len < 1) len = 1;
        ox = -dy * 70 / len;
        oy = dx * 70 / len;
        vx_line_c(f, px + ox, py + oy, x + ox, y + oy, c);
        vx_line_c(f, px, py, x, y, c);
        vx_line_c(f, px - ox, py - oy, x - ox, y - oy, c);
        px = x;
        py = y;
    }
}
static void vx_coq(const vxf_t *f, int32_t act)
{
    int pic = !ux.mono && settings.palette != UI_GREY_INDEX && settings.palette != UI_BW_INDEX;
    uint16_t red = pic ? RGB(210, 24, 34) : T_TEXT, orange = pic ? RGB(232, 120, 24) : T_TEXT;
    uint16_t gold = pic ? RGB(240, 176, 0) : T_TEXT, amber = pic ? RGB(242, 196, 74) : T_TEXT;
    uint16_t teal = pic ? RGB(26, 100, 112) : T_TEXT, teal2 = pic ? RGB(14, 60, 72) : T_TEXT;
    int32_t up = act * 8 / 256, sway = og_sin(fm1_ms * 28u) >> 13, open = act * 10 / 256;
    cq_sickle(f, 128, 132, 168, 70 + sway, 200, 48 + sway, 214, 78, teal2);
    cq_sickle(f, 124, 140, 170, 96 + sway, 206, 78, 220, 108, teal);
    cq_sickle(f, 122, 148, 168, 124, 204, 112, 218, 140, teal2);
    cq_ell(f, 118, 142, 46, 32, teal2);
    cq_ell(f, 126, 140, 28, 16, orange);
    cq_ell(f, 138, 146, 22, 11, gold);
    cq_ell(f, 88, 136, 22, 26, orange);
    cq_ell(f, 72, 100 - up, 22, 20, orange);
    cq_ell(f, 62, 78 - up, 8, 14, red);
    cq_ell(f, 74, 64 - up, 9, 16, red);
    cq_ell(f, 88, 76 - up, 7, 12, red);
    cq_ell(f, 78, 122 - up / 2, 8, 14, red);
    cq_ell(f, 40, 104 - up + open, 16, 6, amber);
    cq_ell(f, 64, 98 - up, 7, 7, pic ? RGB(246, 241, 228) : T_TEXT);
    cq_ell(f, 63, 98 - up, 3, 3, pic ? RGB(26, 22, 20) : T_BG);
    cq_seg(f, 104, 168, 98, 204, gold);
    cq_seg(f, 98, 204, 86, 208, gold);
    cq_seg(f, 98, 204, 108, 208, gold);
    cq_seg(f, 128, 170, 136, 204, gold);
    cq_seg(f, 136, 204, 124, 208, gold);
    cq_seg(f, 136, 204, 148, 208, gold);
}
#undef CQX
#undef CQY
static VX_CODE void vx_animal(const track_t *t, int32_t cx, int32_t cy, int32_t s, int32_t en_home)
{
    vxf_t f = {cx + 160, cy + 128, s * 88 / 100};
    const int16_t *p = t->p;
    uint32_t mode = an_mode(p), i;
    int32_t en = vx_level(t, 1, en_home), act = 0, w[3], m, tgt, bw, w1, w2, fd;
    uint32_t tm = vxs.tm[1];
    for (i = 0; i < NVOICE; i++) {                       /* the call now: each voice's level (its syllable's, CMOS's gate) */
        const voice_t *v = &t->v[i];
        int32_t a;
        if (!v->active) continue;
        a = v->env_out > 0 ? v->env_out >> 7 : 0;
        if (mode == AN_WHSTL) a = a * clamp((int32_t)v->ph[1], 0, 24000) / 16000;
        if (a > act) act = a;
    }
    act = clamp(act, 0, 256) * clamp(en * 2, 0, 256) >> 8;
    tgt = VA_POS[mode];
    if (mode != AN_CMOS) {                               /* SIZE (CMOS: CAP there) past the mode's own animals' sizes */
        int32_t d = p[P_E3] - VA_MID[mode];
        d = d > 12 ? d - 12 : d < -12 ? d + 12 : 0;
        tgt += d * 6 / 5;
        if (mode != AN_GLOTT)
            tgt += (og_sin(tm >> 2) * p[P_E2]) >> 17;   /* (CHAOS stirs it) */
    }
    tgt = clamp(tgt, 0, 256);
    m = vxs.w[0];
    if (!vxs.w[1]) { vxs.w[1] = 1; m = tgt; }           /* (the first look: at once) */
    if (vxs.w[2] != (int16_t)ui.frame) {                 /* (the metamorphosis glides: once a frame) */
        m += (tgt - m) / 5 + (tgt > m) - (tgt < m);
        vxs.w[2] = (int16_t)ui.frame;
    }
    vxs.w[0] = (int16_t)m;
    w[0] = m >= 128 ? (256 - m) * 2 : m * 2;             /* bird */
    w[1] = m < 128 ? (128 - m) * 2 : 0;                  /* insect */
    w[2] = m > 128 ? (m - 128) * 2 : 0;                  /* mammal */
    if (w[0] > 256) w[0] = 256;
    bw = w[0];                                           /* (BRUSSELS: the bird at its full size, fading with its weight; the insect is the cone of fries, the mammal Manneken Pis) */
    w1 = w[1]; w2 = w[2];
    w[0] = 256; w[1] = w[2] = 0;
    fd = 256 - bw;
    vxs.dal = (int16_t)fd;
    vx_coq(&f, act);                                     /* one rooster, the HTML trial, in place of the stacked outlines */
    vxs.dal = 0;
    if (w1 > 10) {                                       /* the insect form: the cone of fries, struck by the call */
        vxk_t k;
        int32_t sq = (en * og_sin(tm >> 1)) >> 15;
        k.f.cx = cx; k.f.cy = cy - ((192 * s * 9 / 5) >> 8); k.f.s = s * 9 / 5;
        k.ox = VK_X(120); k.oy = VK_Y(200); k.sx = 256 + sq * 36 / 256; k.sy = 256 - sq * 51 / 256;
        vxs.dal = (int16_t)(256 - w1);
        vk_fries(&k, 0, en);
        vxs.dal = 0;
    }
    if (w2 > 10) {                                       /* the mammal form: Manneken Pis, his jet reaching with the call */
        vxf_t f2;
        f2.cx = cx - ((608 * s * 3 / 2) >> 8); f2.cy = cy - ((144 * s * 3 / 2) >> 8); f2.s = s * 3 / 2;
        vxs.dal = (int16_t)(256 - w2);
        vk_manneken(&f2, en, 0);
        vxs.dal = 0;
    }
}

/* ============================================================================ (0.9.6) the mock-up's family === */
/* the mock-up's frame (uFrame): its unit space 124 x 94 fitted in the rectangle A (x y w h px), k / 100 larger, lifted
 * dy (Q4 units) */
static vxf_t vx_uf(const int32_t *A, int32_t k, int32_t dy)
{
    vxf_t f;
    int32_t a = A[2] * 256 / 124, b = A[3] * 256 / 94;
    f.s = (a < b ? a : b) * k / 100;
    f.cx = (A[0] * 2 + A[2]) * 8;
    f.cy = (A[1] * 2 + A[3]) * 8 - ((dy * f.s) >> 8);
    return f;
}
static int32_t vx_hash(uint32_t n) { n = (n ^ 61u) ^ (n >> 16); n *= 9u; n ^= n >> 4; n *= 668265261u; return (int32_t)((n ^ (n >> 15)) & 255u); }
/* ms since track t's newest note (k: which creature saw it; 4000 at most); *note: that note */
static uint32_t vx_age(const track_t *t, uint32_t k, int32_t *note)
{
    uint32_t i, a = 0, n = 60;
    for (i = 0; i < NVOICE; i++)
        if (t->v[i].active && t->v[i].age > a) { a = t->v[i].age; n = t->v[i].note; }
    if (a > vxs.nage[k])
        vxs.nms[k] = fm1_ms;
    vxs.nage[k] = a;
    if (note) *note = (int32_t)n;
    return fm1_ms - vxs.nms[k] < 4000u ? fm1_ms - vxs.nms[k] : 4000u;
}
/* a ring (Q4 units), its sides as many as its size asks */
static void vx_ring(const vxf_t *f, int32_t x, int32_t y, int32_t rx, int32_t ry, int32_t h)
{
    uint32_t n = (uint32_t)clamp(((rx + ry) * f->s) >> 12, 8, 28);
    vx_arc(f, x, y, rx, ry, 0, 65536u, n, h);
}
/* text s centred on (x, its capitals' top y) Q4 px */
static void vx_text(int32_t x, int32_t y, const char *s, uint16_t c)
{
    cv_text((x >> 4) - text_w(&AF_X, s) / 2, (y >> 4) - AF_X_CAP_Y, &AF_X, s, c);
}

/* ------------------------------------------------------------------------------------------- ANALOG --- */
/* 2^(l / 256), Q12 */
static int32_t vx_x2(int32_t l)
{
    int32_t i = l >> 8, f = l & 255, m = 4096 + ((f * (2690 + ((f * 1407) >> 8))) >> 8);
    return i >= 0 ? m << i : i > -31 ? m >> -i : 0;
}
/* (BRUSSELS) MANNEKEN, the ANALOG creature: Manneken-Pis big and centred, twinkling sky behind him,
 * and the 4 MS-20 patch cables plugged straight into his extremities (shoulder, elbow, head, hand, feet). */
static void vx_jack(int dst, int32_t id, int32_t bob, int32_t *x, int32_t *y)
{
    int32_t sx, sy;
    if (!dst) {
        switch (id) {
        case MS_LFO: sx = 190; sy = 242; break;          /* left shoulder */
        case MS_ENV: sx = 156; sy = 286; break;          /* left elbow */
        case MS_VEL: sx = 244; sy = 356; break;          /* left hand on hip */
        case MS_KEY: sx = 276; sy = 154; break;          /* left ear / head */
        default: {
            static const int16_t PX[4] = {190, 156, 250, 276}, PY[4] = {242, 286, 460, 154};
            sx = PX[id & 3]; sy = PY[id & 3];
            break;
        }
        }
    } else {
        switch (id) {
        case MD_PITCH: sx = 348; sy = 112; break;        /* curls / top of head */
        case MD_CUT:   sx = 366; sy = 574; break;        /* right foot */
        case MD_SHP:   sx = 362; sy = 380; break;        /* right hand */
        case MD_AMP:   sx = 276; sy = 574; break;        /* left foot */
        case MD_HAUT:  sx = 398; sy = 254; break;        /* right shoulder */
        case MD_VOW:   sx = 364; sy = 194; break;        /* mouth / cheek */
        default: {
            static const int16_t DX[4] = {366, 415, 362, 276}, DY[4] = {574, 600, 380, 574};
            sx = DX[id & 3]; sy = DY[id & 3];
            break;
        }
        }
    }
    *x = MK_X(sx);
    *y = MK_Y(sy, bob);
}
/* (BRUSSELS) the patched slots as slack cords plugged into Manneken-Pis's extremities. An empty slot draws nothing.
 * While the note sounds the belly follows the source and a bead runs the cord. */
static void vx_cords(const track_t *t, const vxf_t *fm, int32_t bob, int32_t en)
{
    uint32_t k;
    for (k = 0; k < NMSLOT; k++) {
        const int16_t *p = &t->p[P_M1SRC + 3u * k];
        int32_t src = p[0], dst = p[1], amt = p[2], ad, live = 0, sag, h, x0, y0, x1, y1, i, px, py;
        if (!src || !dst || !amt || src >= MS_N || dst >= MD_N)
            continue;
        ad = amt < 0 ? -amt : amt;
        if (en > 8) {
            if (src == MS_LFO)
                live = mulq15(t->lfo_val, t->lfo_fade);
            else if (src == MS_ENV)
                live = t->m_env;
        }
        sag = 70 + (ad >> 2) + (int32_t)k * 16 + (live >> 10);
        if (en > 8)
            sag += og_sin(ui.frame * 200u + k * 9000u) * 20 >> 15;
        vx_jack(0, src, bob, &x0, &y0);
        vx_jack(1, dst, bob, &x1, &y1);
        h = 210 + (ad >> 2);
        if (h > 250) h = 250;
        px = x0;
        py = y0;
        for (i = 1; i <= 10; i++) {
            int32_t tt = i * 256 / 10, u = 256 - tt, mx = (x0 + x1) / 2, my = (y0 + y1) / 2 + sag;
            int32_t x = (u * u * x0 + 2 * u * tt * mx + tt * tt * x1) >> 16;
            int32_t y = (u * u * y0 + 2 * u * tt * my + tt * tt * y1) >> 16;
            vx_line(fm, px, py, x, y, h);
            vx_line(fm, px, py + 4, x, y + 4, h - 50);
            px = x;
            py = y;
        }
        vx_dot(fm, x0, y0, 26, 245);
        vx_dot(fm, x1, y1, 26, 245);
        if (en > 8) {
            int32_t tt = (int32_t)((ui.frame * 10u + k * 64u) & 255u), u = 256 - tt;
            int32_t mx = (x0 + x1) / 2, my = (y0 + y1) / 2 + sag;
            vx_dot(fm, (u * u * x0 + 2 * u * tt * mx + tt * tt * x1) >> 16,
                   (u * u * y0 + 2 * u * tt * my + tt * tt * y1) >> 16, 18, 255);
        }
    }
}
static VX_CODE void vx_analog(const track_t *t, const int32_t *A, int32_t en_home)
{
    const int16_t *p = t->p;
    int32_t en = vx_level(t, 3, en_home), note, i;
    uint32_t mt = vxs.mt[3], age = vx_age(t, 3, &note);
    vxf_t f = vx_uf(A, 112, 32), fm;
    int32_t nz = (p[P_E3] * 516 >> 8), det = p[P_E1];
    int32_t live = en > 8 ? en : 0;
    int32_t sway = live ? (og_sin(vx_ang(mt, 70)) * (60 + live / 3)) >> 15 : 0;
    int32_t lift = live ? (og_sin(vx_ang(mt, 150) + 12000u) * (40 + live / 3)) >> 15 : 0;
    int32_t sh = det / 2;
    int32_t H = A[3] * 80 / 100, s = H * 4096 / 692;
    int32_t cx = (A[0] * 2 + A[2]) * 8, cy = (A[1] * 2 + A[3]) * 8 + A[3] * 14 / 10;
    int32_t bob = (ui_prefs & PREF_ANIM_OFF) ? 0 : (((og_sin(fm1_ms * 40u) * 16) >> 15) + ((en * 36) >> 8));
    (void)age; (void)note;
    vxs.kc = 1;
    fm.s = s;
    fm.cx = cx - ((608 * s) >> 8) + ((sway * f.s) >> 8);
    fm.cy = cy - ((175 * s) >> 8) - ((lift * f.s) >> 8);
    if (sh > 6) {                                        /* DETUNE: faint shimmer copy either side */
        vxf_t fs = fm;
        vxs.dal = 140;
        fs.cx = fm.cx + ((sh * f.s) >> 8);
        vk_manneken(&fs, en, 0);
        fs.cx = fm.cx - ((sh * f.s) >> 8);
        vk_manneken(&fs, en, 0);
        vxs.dal = 0;
    }
    vk_manneken(&fm, en, 0);
    for (i = 0; nz > 8 && i < 24; i++) {                 /* NOISE: sparks in the sky behind him */
        int32_t x = (vx_hash((uint32_t)i * 7u + (mt >> 9)) - 128) * 3, y = (vx_hash((uint32_t)i * 13u + 5u + (mt >> 9)) - 128) * 3;
        if (i * 255 / 24 < nz) vx_dot(&f, x, y, 16, 120 + (nz * 100 >> 8));
    }
    vx_cords(t, &fm, bob, en);                           /* (BRUSSELS) the patched cables on Manneken-Pis's extremities */
    vxs.kc = 0;
}

/* ---------------------------------------------------------------------------------------------- FM6 --- */
#ifndef VX_FM6_HOOK
#define VX_FM6_HOOK(k, x, y, r) ((void)0)               /* ui_render.c (host tests): each bicho as drawn (its lint) */
#endif
static const uint8_t FM6_CELL[32][6];                    /* (ui_graph.c) */
static void fm6_routes(uint32_t a, uint8_t *m);
static uint32_t fm6_fb_op(uint32_t a);
static uint32_t fm6_car_ops(uint32_t a);
static uint32_t fm6_alg_of(const track_t *t);
static void fop_freq(const uint8_t *op, char *s);
typedef struct {
    const uint8_t *p;                                    /* the patch as it plays (the macros on it) */
    int32_t env[6], amp[6], rat[6], rad[6], x[6], y[6];  /* per operator 1..6: its envelope now, level x envelope (0..256),
                                                          * ratio (Q8), radius, place (Q4 units) */
    uint32_t alg, car, fbo, st[6];                       /* the algorithm, its carriers, the feedback operator; each stage */
    uint8_t m[6];
} vxfm_t;
/* the operators of track t now: the sounding notes' envelopes (the loudest), each one's level, ratio, size, place */
static void vx_fm_ops(const track_t *t, vxfm_t *o)
{
    uint32_t tr = (uint32_t)(t - trk) % NTRK, k, v, hi = 0, top = 0;
    o->p = fm6_eff[tr].ok ? fm6_eff[tr].p : fm6_patch[tr];
    o->alg = fm6_alg_of(t);
    o->car = fm6_car_ops(o->alg);
    o->fbo = fm6_fb_op(o->alg);
    fm6_routes(o->alg, o->m);
    for (k = 0; k < 6u; k++) {
        const uint8_t *op = o->p + (5u - k) * FP_OP;
        int32_t e = 0, ol = op[FP_OL] % 100u, a = ol * 256 / 99, c = op[FP_FC] & 31;
        o->st[k] = 4;
        for (v = 0; v < FM6_POLY; v++) {                 /* (msfa's level: scaleout units 0..127, the stage's L 0..99) */
            const fm6_env_t *n = &fm6_note[tr][v].env[5u - k];
            int32_t s = (((n->level >> 16) - n->outlevel + 4256) / 32 - 28) * 256 / 99;
            if (!fm6_note[tr][v].live || !t->v[v].active) continue;
            if (s > e || o->st[k] == 4u) { o->st[k] = n->ix | (n->down ? 0u : 8u); }
            e = s > e ? s : e;
        }
        o->env[k] = clamp(e, 0, 256);
        o->amp[k] = ol ? (a * ((a * 154 >> 8) + 102) / 256) * o->env[k] / 256 : 0;   /* ((ol / 99) ^ 1.6) x env */
        o->rat[k] = op[FP_MODE] ? 256 : (c ? c * 256 : 128) * (100 + op[FP_FF] % 100u) / 100;
        o->rad[k] = 120 * (141 + 141 * ol / 99) / 256 * (200 + (o->env[k] * 82 >> 8)) / 256;
        hi = (FM6_CELL[o->alg][k] & 7u) > hi ? FM6_CELL[o->alg][k] & 7u : hi;
        top = (uint32_t)(FM6_CELL[o->alg][k] >> 4) > top ? (uint32_t)FM6_CELL[o->alg][k] >> 4 : top;
    }
    for (k = 0; k < 6u; k++) {                           /* (fmPos: columns 26 apart, the carriers on the floor 38) */
        o->x[k] = ((int32_t)(FM6_CELL[o->alg][k] & 7u) * 2 - (int32_t)hi) * 13 * 16;
        o->y[k] = 28 * 16 - (int32_t)(FM6_CELL[o->alg][k] >> 4) * (top >= 3u ? 324 : 352);
    }
}
/* operator k's sphere of the Atomium at (x, y): the same ball as the nine of HOME, a window band and a meridian,
 * no face. Its size and heat are its level now (it pulses at its ratio). sel: the operator picked. sh: DETUNE's
 * shimmer (Q4 units) */
static VX_CODE void vx_fm_bicho(const vxf_t *f, const vxfm_t *o, uint32_t k, int32_t x, int32_t y, int32_t r, uint32_t age, uint32_t mt,
                        int sel, int32_t sh)
{
    int32_t a = o->amp[k], d, band;
    int32_t oc = sel ? -2 : 100 + (a * 140 >> 8);
    (void)age;
    if (!(o->p[(5u - k) * FP_OP + FP_OL] % 100u)) {      /* (silent: a cold outline) */
        vx_ring(f, x, y, r, r, 20);
        return;
    }
    r = r * (256 + ((og_sin(vx_ang(mt, 220 * (o->rat[k] < 12 * 256 ? o->rat[k] : 12 * 256) / 256)) * 15 >> 15) * a >> 8)) / 256;
    band = sel ? -2 : oc - 40 > 20 ? oc - 40 : 20;
    if (sh) {                                            /* (DETUNE: a shimmer, two faint copies) */
        vxs.dal = 102;
        for (d = -1; d <= 1; d += 2)
            vx_ring(f, x + d * sh, y, r, r, 51 + (a * 77 >> 8));
        vxs.dal = 0;
    }
    vx_ring(f, x, y, r, r, oc);
    vx_arc(f, x, y, r, r * 32 / 100, 0, 65536u, 14, band);   /* the window band */
    vx_arc(f, x, y, r * 36 / 100, r, 0, 65536u, 14, band);   /* a meridian, so it reads as a ball */
}
/* the operator's number by its bicho */
static void vx_fm_num(const vxf_t *f, const vxfm_t *o, uint32_t k, int32_t x, int32_t y, int sel)
{
    char n[2] = {(char)('1' + k), 0};
    vx_text(f->cx + ((x * f->s) >> 8), f->cy + ((y * f->s) >> 8) - 64, n,
            sel ? T_THEME : o->p[(5u - k) * FP_OP + FP_OL] % 100u ? T_MID : T_DIM);
}
/* COLONIA: the operators wired as the algorithm, the carriers standing on the output; sel the operator picked (or 6),
 * op2: OPERATOR 2 (its DETUNE a shimmer) */
static VX_CODE void vx_fm6(const track_t *t, const int32_t *A, int32_t en_home, uint32_t sel, int op2)
{
    vxfm_t o;
    vxf_t f = vx_uf(A, 100, 0);
    int32_t en = vx_level(t, 4, en_home), x0 = 9999, x1 = -9999, y0 = 9999, s, l = 9999, r = -9999, k, j, fb;
    uint32_t mt = vxs.mt[4], age = vx_age(t, 4, 0);
    vxs.kc = 1;
    vx_fm_ops(t, &o);
    for (k = 0; k < 6; k++) {
        x0 = o.x[k] < x0 ? o.x[k] : x0; x1 = o.x[k] > x1 ? o.x[k] : x1; y0 = o.y[k] < y0 ? o.y[k] : y0;
        if (o.car >> k & 1u) { l = o.x[k] < l ? o.x[k] : l; r = o.x[k] > r ? o.x[k] : r; }
    }
    x0 -= 16 * 16; x1 += 18 * 16; y0 -= 13 * 16;         /* (fitTo: the colony fills the frame, at most 1.75 x) */
    s = f.s * 124 * 16 / (x1 - x0);
    j = f.s * 94 * 16 / (47 * 16 - y0);
    s = s < j ? s : j;
    s = s < f.s * 7 / 4 ? s : f.s * 7 / 4;
    f.cx -= ((x0 + x1) / 2 * s) >> 8;
    f.cy -= ((y0 + 47 * 16) / 2 * s) >> 8;
    f.s = s;
    l = (l < -160 ? l : -160) - 128;                    /* the output: the floor, an arrow out */
    r = (r > 160 ? r : 160) + 192;
    vx_line(&f, l, 38 * 16, r, 38 * 16, 36 + en / 2);
    vx_line(&f, r - 48, 36 * 16, r, 38 * 16, 77 + en / 2);
    vx_line(&f, r - 48, 40 * 16, r, 38 * 16, 77 + en / 2);
    for (k = 0; k < 6; k++)                              /* the wires: down, across, down; pulses as fast as the ratio */
        for (j = 0; j < 6; j++)
            if (o.m[k] >> j & 1u) {
                int32_t a = o.amp[j], ya = o.y[j] + o.rad[j], yb = o.y[k] - o.rad[k], my = (ya + yb) / 2, q[8];
                q[0] = o.x[j]; q[1] = ya; q[2] = o.x[j]; q[3] = my; q[4] = o.x[k]; q[5] = my; q[6] = o.x[k]; q[7] = yb;
                vx_curve(&f, q, 4, 0, 31 + (a * 154 >> 8));
                if (a > 8) {                             /* (two pulses along it, as fast as the modulator's ratio) */
                    int32_t n, dx = o.x[k] > o.x[j] ? o.x[k] - o.x[j] : o.x[j] - o.x[k], len = (my - ya) + dx + (yb - my);
                    int32_t rr = f.s * 7 >> 8, mr = o.rat[j] < 14 * 256 ? o.rat[j] : 14 * 256;
                    for (n = 0; n < 2; n++) {
                        int32_t d = (int32_t)(((mt * (uint32_t)(256 + mr / 4) * 786u >> 16) + (uint32_t)n * 128u) & 255u) * len / 256;
                        if (d < my - ya) vx_dot(&f, o.x[j], ya + d, rr, 154 + (a * 102 >> 8));
                        else if ((d -= my - ya) < dx) vx_dot(&f, o.x[j] + (o.x[k] > o.x[j] ? d : -d), my, rr, 154 + (a * 102 >> 8));
                        else vx_dot(&f, o.x[k], my + d - dx, rr, 154 + (a * 102 >> 8));
                    }
                }
            }
    fb = clamp(o.p[FP_FB] + t->p[P_E1], 0, 7);           /* FB: a loop back to the ball */
    {
        int32_t X = o.x[o.fbo], Y = o.y[o.fbo], rd = o.rad[o.fbo], q[10];
        q[0] = X + (rd * 205 >> 8); q[1] = Y - (rd * 102 >> 8);
        q[2] = X + rd + 48 + fb * 48 / 7; q[3] = Y - rd - 32 - fb * 32 / 7;
        q[4] = X + rd + 96 + fb * 64 / 7; q[5] = Y + ((og_sin(vx_ang(mt, 200)) * fb * 32 / 7) >> 15);
        q[6] = X + rd + 32; q[7] = Y + (rd * 179 >> 8);
        q[8] = X + (rd * 179 >> 8); q[9] = Y + rd / 4;
        vx_curve(&f, q, 5, 0, fb ? 64 + fb * 154 / 7 : 20);
        if (fb) vx_dot(&f, q[4], q[5], f.s >> 5, 154 + fb * 102 / 7);
    }
    for (k = 0; k < 6; k++) {
        const uint8_t *op = o.p + (5u - (uint32_t)k) * FP_OP;
        int32_t sh = op2 && (uint32_t)k == sel ? (op[FP_DET] > 7u ? op[FP_DET] - 7 : 7 - op[FP_DET]) * 8 : 0;
        int32_t br = en > 8 ? (og_sin(vx_ang(mt, 160) + (uint32_t)k * 7000u) * (o.rad[k] / 7 + 16)) >> 15 : 0;
        int32_t yy = o.y[k] - (en > 8 ? (og_sin(vx_ang(mt, 160) + (uint32_t)k * 7000u) * (4 * 16 + en / 4)) >> 15 : 0);
        vx_fm_bicho(&f, &o, (uint32_t)k, o.x[k], yy, o.rad[k] + br, age, mt, (uint32_t)k == sel, sh);
        vx_fm_num(&f, &o, (uint32_t)k, o.x[k] - o.rad[k] - 64, yy - o.rad[k] + 16, (uint32_t)k == sel);
        VX_FM6_HOOK(k, f.cx + ((o.x[k] * f.s) >> 8), f.cy + ((o.y[k] * f.s) >> 8), (145 * f.s) >> 8);   /* (the largest it gets) */
    }
    vxs.kc = 0;
}
/* OPERATOR / OPERATOR 2: the colony on the whole panel, the operator picked lit; "ALG 05" over it on the left, its
 * frequency on the right ("OP3 x1.50") */
static VX_CODE void vx_fm6_page(const track_t *t, uint32_t sel, int op2)
{
    static const int32_t A[4] = {4, 14, 232, 124};
    const uint8_t *pt = fm6_patch[(t - trk) % NTRK];
    uint32_t alg = fm6_alg_of(t);
    uint32_t hot = ui.hot_t ? (uint32_t)cur_page()->id[ui.hot_col & 3u] : 0u;
    char b[16] = {'A', 'L', 'G', ' ', (char)('0' + (alg + 1u) / 10u), (char)('0' + (alg + 1u) % 10u), 0};
    char fq[16] = {'O', 'P', (char)('1' + sel), ' ', 0};
    cv_oy = 0;
    vx_fm6(t, A, -1, sel, op2);
    if (!t->p[P_E0])
        str_cpy(b + 6, " PAT", 5);
    cv_text(10, 4, &AF_S, b, hot == P_E0 || hot == P_E7 ? T_ACCENT : T_MID);
    fop_freq(pt + (5u - sel) * FP_OP, fq + 4);
    cv_text_r(228, 4, &AF_S, fq, T_TEXT, T_SURF);
}
/* OP ENV: the picked operator's envelope as the patch has it (from L4 to L1 at R1, L2 at R2, L3 at R3, held while the key
 * is down: a dashed line where it is let go, back to L4 at R4; a stage as wide as its rate is slow), the stage KNOB 3 / 4
 * edit in the accent, its bicho riding the curve where the operator is now; "OP3" top left, the stage's R and L right */
static VX_CODE void vx_fm6env(const track_t *t)
{
    static const char *const STG[4] = {"ATK", "DEC", "SUS", "REL"};
    static const int32_t A[4] = {4, 12, 232, 126};
    const uint8_t *op = fop_bytes((uint32_t)(t - trk));
    vxfm_t o;
    vxf_t f = vx_uf(A, 100, 0);
    int32_t x[6], y[6], k, i, en = vx_level(t, 4, -1), px, py, e, r, u, sg = fop_stg & 3u;
    uint32_t mt = vxs.mt[4], age = vx_age(t, 4, 0), ko = fop_op % 6u, st;
    char b[24] = {'O', 'P', (char)('1' + ko), 0};
    (void)en;
    cv_oy = 0;
    vxs.kc = 1;
    vx_fm_ops(t, &o);
#define VFY(l) (22 * 16 - (int32_t)(l) * 42 * 16 / 99)
#define VFW(r) ((4 + (99 - (int32_t)(r)) * (99 - (int32_t)(r)) * 44 / 9801) * 112 * 16 / 216)
    x[0] = -56 * 16;
    y[0] = VFY(op[FP_L1 + 3]);
    for (k = 0; k < 3; k++) {
        x[k + 1] = x[k] + VFW(op[FP_R1 + k]);
        y[k + 1] = VFY(op[FP_L1 + k]);
    }
    x[5] = 56 * 16;
    x[4] = x[5] - VFW(op[FP_R1 + 3]);
    y[4] = y[3];
    y[5] = y[0];
    vx_line(&f, -60 * 16, 26 * 16, 60 * 16, 26 * 16, 26);
    for (i = -20 * 16; i < 22 * 16; i += 48)             /* the key let go */
        vx_line(&f, x[4], i, x[4], i + 22, 31);
    for (k = 0; k < 5; k++)                              /* (3: the hold, no stage of its own) */
        for (i = 0; i < 10; i++) {
            int32_t on = k != 3 && (k < 3 ? k : 3) == sg, dx = x[k + 1] - x[k], dy = y[k + 1] - y[k];
            int32_t ya = y[k] + dy * i / 10, d;
            for (d = 0; d <= on * 12; d += 12)
                vx_line(&f, x[k] + dx * i / 10, ya + d, x[k] + dx * (i + 1) / 10, y[k] + dy * (i + 1) / 10 + d,
                        on ? -2 : 38 + ((22 * 16 - ya) * 154 / (42 * 16)));
        }
    k = sg < 3 ? sg + 1 : 5;
    vx_ring(&f, x[k], y[k], 32, 32, -2);
    vx_ring(&f, x[k], y[k], 40, 40, -2);
    e = o.env[ko];                                       /* where the operator is now: on its stage, as far as its level */
    st = o.st[ko];                                       /* came (held: along the hold, 1.5 s across) */
    py = VFY(e * 99 / 256);
    k = (st & 7u) >= 4u ? 5 : st == 3u ? 3 : st < 3u ? (int32_t)st : 4;
    if (k == 3) {
        if (!vxs.fhold) vxs.fhold = fm1_ms | 1u;
        u = clamp((int32_t)(fm1_ms - vxs.fhold) * 256 / 1500, 0, 256);
    } else {
        int32_t l0 = k == 4 ? vxs.frel * 99 / 256 : op[FP_L1 + ((k + 3) & 3)], l1 = op[FP_L1 + (k < 4 ? k : 3)];
        vxs.fhold = 0;
        u = l1 != l0 ? clamp((e * 99 / 256 - l0) * 256 / (l1 - l0), 0, 256) : 256;
    }
    if (k == 5) { k = 4; u = 256; }
    if (k != 4) vxs.frel = (int16_t)e;                   /* (the level its release sets out from) */
    px = x[k] + (x[k + 1] - x[k]) * u / 256;
    r = 64 + (96 * e >> 8);
    vx_fm_bicho(&f, &o, ko, px, py - r, r, age, mt, 1, 0);
    k = sg < 3 ? (x[sg] + x[sg + 1]) / 2 : (x[4] + x[5]) / 2;
    k = clamp(k, -49 * 16, 49 * 16);                     /* (the stage's name under it, inside the curve's ends) */
    vx_text(f.cx + ((k * f.s) >> 8),
            f.cy + ((31 * 16 * f.s) >> 8) - 112, STG[sg], T_THEME);
#undef VFY
#undef VFW
    vxs.kc = 0;
    cv_text(10, 4, &AF_S, b, T_MID);
    str_cpy(b, STG[sg], sizeof b);
    str_cpy(b + str_len(b), "  R ", 5);
    fmt_int(b + str_len(b), op[FP_R1 + sg]);
    str_cpy(b + str_len(b), "  L ", 5);
    fmt_int(b + str_len(b), op[FP_L1 + sg]);
    cv_text_r(228, 4, &AF_S, b, T_TEXT, T_SURF);
}

/* -------------------------------------------------------------------------------------------- WHEEL --- */
/* LES PIGNONS DE LA GRAND-PLACE (the planche's WHEEL card, docs/creatures/creatures.js): nine gabled house fronts with
 * stepped gables, one per drawbar: a front's height is its bar's level, its digit stays under it; the rotor makes the
 * flags on the gables fly (its real speed and phase, DEPTH its reach), the level makes the fronts tremble, PERC lights the
 * windows of its bar (a note flashes some of them), CLICK's bubbles rise from the gables, DUST drifts. */
static VX_CODE void vx_wheel(const track_t *t, const int32_t *A, int32_t en_home)
{
    const int16_t *p = t->p;
    const drw_trk_t *T = &drw_t[drw_part(t)];
    vxf_t f = vx_uf(A, 100, 0);
    uint32_t n0 = vxs.nms[5], mt = vxs.mt[5], i;
    int32_t en = vx_level(t, 5, en_home), dr = (p[P_E6] * 516 >> 8), hb = (dr * 56 >> 8), perc = 0, pk, dep, click = (p[P_E5] * 516 >> 8), nf;
    uint32_t age = vx_age(t, 5, 0), pc = (uint32_t)p[P_E4] % 7u;
    vxs.kc = 1;
    if (en) { vxs.rph[0] = T->rph[0]; vxs.rph[1] = T->rph[1]; }   /* (the rotors as they turn while it sounds) */
    dep = (clamp(p[P_WRDEP], 0, 127) + 1) * (T->rwet >> 7) >> 7;  /* (0..256: DEPTH, the rotor on) */
    pk = pc ? 3 + (int32_t)((pc - 1u) & 1u) : -1;
    for (i = 0; i < NVOICE; i++)
        if (t->v[i].active && drw_v[drw_part(t)][i].perc > perc) perc = drw_v[drw_part(t)][i].perc;
    perc >>= 7;                                          /* (PERC's envelope, 0..256) */
    if (vxs.nms[5] != n0)                                /* a note: CLICK's bubbles out of the worms' mouths */
        for (i = 0; i < 9u; i++)
            if (drw_level(p, i) && vx_hash(i + vxs.nage[5] * 9u) < click) vxs.bub[i] = fm1_ms | 1u;
    nf = vx_fall(age, 154u);                             /* the note's flash */
    {                                                    /* (BRUSSELS) a fine starfield, drifting slowly behind the roofs */
        int32_t drift = (ui_prefs & PREF_ANIM_OFF) ? 0 : (int32_t)(fm1_ms / 48u);
        for (i = 0; i < 14u; i++) {
            int32_t sx = (vx_hash(i * 17u + 3u) * (124 * 16) / 256 + drift * (int32_t)(1u + i % 3u)) % (124 * 16) - 62 * 16;
            int32_t sy = -44 * 16 + vx_hash(i * 29u + 7u) * (38 * 16) / 256;
            vx_dot(&f, sx, sy, (i % 4u == 0u) ? 20 : 13, 42 + (int32_t)(i % 3u) * 13);
        }
    }
    vx_line(&f, -62 * 16, 21 * 16, 62 * 16, 21 * 16, 31 + (en * 77 >> 8));
    for (i = 0; i < 9u; i++) {
        static const int16_t GX[16] = {0, 0, 21, 21, 42, 42, 63, 63, 105, 105, 126, 126, 147, 147, 168, 168};
        static const int16_t GY[16] = {-1, 0, 0, 40, 40, 80, 80, 120, 120, 80, 80, 40, 40, 0, 0, -1};   /* (up from the shoulder; -1: the ground) */
        int32_t l = drw_level(p, i), X = (-48 + (int32_t)i * 12) * 16, pp = (int32_t)i == pk ? perc : 0, base = 21 * 16, k, r, c, q[32];
        int32_t h = 44 + l * 82 + (pp * 128 >> 8) + (en * l * 9 >> 8), c0 = (en * 51 >> 8) + hb + (pp * 102 >> 8);
        int32_t bend = ((og_sin((vxs.rph[1] >> 16) - i * 5734u) * dep) >> 15) * h / 60 * 6 / 256, sway = bend / 4;
        int32_t hop = en > 8 ? ((og_sin(vx_ang(mt, 140) + (uint32_t)i * 7000u) + 32767) * (24 + en / 8)) >> 15 : 0;   /* (BRUSSELS) bounce up only: the feet never sink through the square */
        int32_t lean = en > 8 ? (og_sin(vx_ang(mt, 80) + (uint32_t)i * 9000u) * (3 * 16 + en / 4)) >> 15 : 0;
        int32_t jx = lean;                               /* the whole gable breathes with the level */
        int32_t yw = base - h, x0 = X - 84 + jx, hv = clamp(130 + l * 12 + c0, 0, 255), fx, fy;
        char d[2] = {(char)('0' + l), 0};
        for (k = 0; k < 16; k++) {                       /* the stepped gable's outline, leaning with the rotor */
            int32_t y = (GY[k] < 0 ? base : yw - GY[k]) - hop;
            q[2 * k] = x0 + GX[k] + sway * (base - y) / (h + 120);
            q[2 * k + 1] = y;
        }
        for (k = 1; k < 16; k++)
            vx_line(&f, q[2 * k - 2], q[2 * k - 1], q[2 * k], q[2 * k + 1], hv);
        for (r = 0, k = yw + 48; k < base - 100; k += 104, r++)   /* the windows, two to a row; PERC and the note light some */
            for (c = 0; c < 2; c++) {
                int32_t wx = x0 + 32 + c * 64 + sway * (base - k) / (h + 120), wy = k - hop, lit = l && (((int32_t)i + r + c + (int32_t)(mt >> 8) / 20) % 4 == 0) ? (nf > pp ? nf : pp) : 0;
                vx_line(&f, wx, wy, wx + 40, wy, hv - 45);
                vx_line(&f, wx + 40, wy, wx + 40, wy + 48, hv - 45);
                vx_line(&f, wx + 40, wy + 48, wx, wy + 48, hv - 45);
                vx_line(&f, wx, wy + 48, wx, wy, hv - 45);
                if (lit > 20)
                    vx_dot(&f, wx + 20, wy + 24, 8 + lit * 14 / 256, 120 + (lit * 130 >> 8));
            }
        if (l) {                                         /* the door at its foot */
            vx_line(&f, x0 + 60, base - hop, x0 + 60, base - 44 - hop, hv - 45);
            vx_line(&f, x0 + 60, base - 44 - hop, x0 + 108, base - 44 - hop, hv - 45);
            vx_line(&f, x0 + 108, base - 44 - hop, x0 + 108, base - hop, hv - 45);
        }
        fx = x0 + 84 + sway * h / (h + 120);              /* the flag on the gable's top: the rotor's real speed and phase */
        fy = yw - 120 - hop;
        vx_line(&f, fx, fy, fx, fy - 64, hv - 20);
        {
            int32_t fa = 13 + (dep * 20 >> 8) + (en * 12 >> 8), yt = 0, yb = 0, xt = 0;
            for (k = 0; k <= 4; k++) {
                int32_t w = (og_sin((vxs.rph[1] >> 16) + vx_ang(mt, 900) + (uint32_t)i * 5734u + (uint32_t)k * 7400u) * fa >> 15) * k / 4;
                int32_t xx = fx + k * 14, ya = fy - 56 + w, yc = fy - 24 + w;
                if (k) {
                    vx_line(&f, xt, yt, xx, ya, 205);
                    vx_line(&f, xt, yb, xx, yc, 205);
                }
                xt = xx; yt = ya; yb = yc;
            }
            vx_line(&f, xt, yt, xt, yb, 205);
        }
        vx_text(f.cx + ((X * f.s) >> 8), f.cy + ((21 * 16 * f.s) >> 8) + 48, d, l ? T_MID : T_DIM);   /* its digit, under it */
    }
    for (i = 0; i < 9u; i++)                             /* CLICK's bubbles, rising */
        if (vxs.bub[i] && fm1_ms - vxs.bub[i] < 1200u) {
            int32_t a = (int32_t)(fm1_ms - vxs.bub[i]) * 256 / 1000, l = drw_level(p, i);
            vxs.dal = (int16_t)clamp(a, 0, 256);
            vx_ring(&f, (-48 + (int32_t)i * 12) * 16 + ((og_sin((uint32_t)a * 367u) * 24) >> 15),
                    21 * 16 - (44 + l * 82) - 120 - 64 - (a * 30 * 16 >> 8), 16 + a / 16, 16 + a / 16, 128);
            vxs.dal = 0;
        }
    if (T->dust)                                         /* DUST: motes drifting */
        for (i = 0; i < (uint32_t)((T->dust * 40 >> 8)); i++)
            vx_dot(&f, (vx_hash(i * 9u + 1u) * 124 / 256 - 62) * 16,
                   (int32_t)(((uint32_t)vx_hash(i * 3u + 2u) * 6u + (mt >> 4) * (uint32_t)(77 + vx_hash(i) / 2) / 256u) % 1504u) - 752, 16, 90);
    vxs.kc = 0;
}

/* -------------------------------------------------------------------------------------------- NOISE --- */
/* the swarm's step (the mock-up's, a frame of 30 a second): COLOR their walk, FREQ their swirl and its ring, PEAK
 * how tight, DUST's impulses flare them (DENSE) */
static VX_CODE void vx_nz_step(const track_t *t, int32_t en)
{
    const int16_t *p = t->p;
    int32_t col = (p[P_E1] * 516 >> 8), peak = (p[P_E3] * 516 >> 8), rp = (12 * 256 + p[P_E2] * 26 * 256 / 127), dens = (p[P_E5] * 516 >> 8);
    int32_t sp = (4 + (p[P_E2] * 60 >> 8)) * (256 + en) / 256, jit = (256 - col) * 358 / 256 * (77 + en) / 256 + (col * 90 >> 8);
    int32_t pull = 3 + (peak * 15 >> 8), damp = 230 + (col * 20 >> 8), i;
    uint32_t r = vxs.rs ? vxs.rs : 2463534242u;
    for (i = 0; i < VX_NSW; i++) {
        int16_t *q = vxs.sw[i];
        int32_t x = q[0], y = q[1], ax = x < 0 ? -x : x, ay = y < 0 ? -y : y, rr = (ax > ay ? ax + ay * 3 / 8 : ay + ax * 3 / 8) + 1;
        int32_t vx = q[2], vy = q[3], k = (rr - rp) * pull / 256;
        vx = (vx * damp >> 8) + ((int32_t)(vx_rnd(&r) & 255u) - 128) * jit / 256 - y * sp / 1000 - x * k / rr;
        vy = (vy * damp >> 8) + (((int32_t)(vx_rnd(&r) & 255u) - 128) * jit / 256) * 4 / 5 + x * sp / 1000 - y * k / rr;
        if ((p[P_E0] & 3) == NZ_DUST && (int32_t)(vx_rnd(&r) & 65535u) < dens * dens / 50 * (51 + en) / 256) {
            q[4] = 256;                                  /* (an impulse: it flares, it jumps) */
            vx += ((int32_t)(vx_rnd(&r) & 255u) - 128) * 6;
            vy += ((int32_t)(vx_rnd(&r) & 255u) - 128) * 6;
        }
        vx -= x / 250;
        vy -= y / 166;
        q[4] = (int16_t)((q[4] * 217 >> 8));
        x += vx / 2;
        y += vy / 2;
        if (x > 62 * 256 || x < -62 * 256 || y > 46 * 256 || y < -46 * 256) { x = (x * 77 >> 8); y = (y * 77 >> 8); vx = vy = 0; }
        q[0] = (int16_t)clamp(x, -32000, 32000); q[1] = (int16_t)clamp(y, -32000, 32000);
        q[2] = (int16_t)clamp(vx, -32000, 32000); q[3] = (int16_t)clamp(vy, -32000, 32000);
    }
    vxs.rs = r;
}
/* ENJAMBRE */
static VX_CODE void vx_noise(const track_t *t, const int32_t *A, int32_t en_home)
{
    const int16_t *p = t->p;
    vxf_t f = vx_uf(A, 100, 0);
    int32_t en = vx_level(t, 6, en_home), md = p[P_E0] & 3, peak = (p[P_E3] * 516 >> 8), i, j, n;
    uint32_t mt = vxs.mt[6], age = vx_age(t, 6, 0), reg = 0;
    vxs.kc = 1;
    if (!vxs.sw[0][0] && !vxs.sw[0][1])                  /* (scattered, the first time) */
        for (i = 0; i < VX_NSW; i++) {
            vxs.sw[i][0] = (int16_t)((vx_hash((uint32_t)i * 13u + 1u) - 128) * 80);
            vxs.sw[i][1] = (int16_t)((vx_hash((uint32_t)i * 29u + 7u) - 128) * 60);
        }
    for (n = 0; vxs.swt < mt >> 8 && n < 6; n++, vxs.swt++)   /* (its frames as its time ran: still in silence) */
        vx_nz_step(t, en);
    vxs.swt = mt >> 8;
    if (md >= NZ_LFSR) {                                 /* LFSR / META: the register marching in columns, a column a bit */
        for (i = 0; i < NVOICE; i++)
            if (t->v[i].active) { reg = t->v[i].ph[1]; break; }
        for (i = 0; i < 16; i++) {
            int32_t on = md == NZ_LFSR ? (int32_t)(reg >> i & 1u) : vx_hash(((uint32_t)i + (mt >> 9)) % 16u * 7u + (uint32_t)(p[P_E5] >> 4)) > 128;
            int32_t X = -56 * 16 + i * 118;
            for (j = 0; j < 5; j++) {
                int32_t Y = (-36 + j * 6 + (on ? 0 : 36)) * 16;
                if (on) vx_line(&f, X - 16, Y, X + 22, Y, 128 + (en * 102 >> 8));
                else vx_dot(&f, X, Y, 16, 46);
            }
        }
    }
    for (i = md >= NZ_LFSR ? VX_NSW / 2 : 0; i < VX_NSW; i += 2) {   /* the pigeons: two wings flapping, flaring with DUST's impulses */
        const int16_t *q = vxs.sw[i];
        int32_t vx = q[2], vy = q[3], av = vx < 0 ? -vx : vx, bv = vy < 0 ? -vy : vy, v = (av > bv ? av + bv * 3 / 8 : bv + av * 3 / 8) + 1;
        int32_t s = v < 3 * 256 ? v : 3 * 256, x = q[0] / 16, y = q[1] / 16;
        int32_t c = q[4] > 51 ? 218 + (q[4] * 38 >> 8) : clamp(110 + (s * 40 >> 8) + (en * 46 >> 8), 0, 179);
        int32_t fl = (og_sin(vx_ang(mt, 467) + (uint32_t)i * 17731u) + 32768) >> 8, dy = (22 * fl) >> 8, wd = 44;
        vx_line(&f, x - wd, y - dy, x, y, c);
        vx_line(&f, x, y, x + wd, y - dy, c);
    }
    if (peak > 51) {                                     /* PEAK: subtle wind ring */
        int32_t rp = (14 * 16 + p[P_E2] * 20 * 16 / 127);
        vxs.dal = (int16_t)(256 - peak / 3);
        vx_ring(&f, 0, 0, rp, rp, 96 + (peak * 64 >> 8));
        vxs.dal = 0;
    }
    {   /* the street lamp breathes with the level; the pigeons and the rain stay where they fly */
        int32_t ocx = f.cx, ocy = f.cy, ln[8] = {-10 * 8, -8 * 8, 10 * 8, -8 * 8, 7 * 8, 13 * 8, -7 * 8, 13 * 8}, rf[6] = {-13 * 8, -8 * 8, 0, -18 * 8, 13 * 8, -8 * 8};
        int32_t hc = 150 + (en * 60 >> 8);
        if (en > 8) {
            f.cx += (og_sin(vx_ang(mt, 80)) * (4 + en / 24) * 16) >> 15;
            f.cy -= (og_sin(vx_ang(mt, 140)) * (6 + en / 16) * 16) >> 15;
        }
        vx_line(&f, -12, 78 * 8, -12, 14 * 8, 100);
        vx_line(&f, 12, 78 * 8, 12, 14 * 8, 100);
        vx_line(&f, -8 * 8, 78 * 8, -6 * 8, 71 * 8, 100);
        vx_line(&f, -6 * 8, 71 * 8, 6 * 8, 71 * 8, 100);
        vx_line(&f, 6 * 8, 71 * 8, 8 * 8, 78 * 8, 100);
        for (i = 0; i < 4; i++) vx_line(&f, ln[2 * i], ln[2 * i + 1], ln[2 * ((i + 1) & 3)], ln[2 * ((i + 1) & 3) + 1], hc);
        vx_line(&f, rf[0], rf[1], rf[2], rf[3], hc - 20);
        vx_line(&f, rf[2], rf[3], rf[4], rf[5], hc - 20);
        vx_dot(&f, 0, -20 * 8, 19, hc);
        vxs.dal = (int16_t)(256 - (42 + (en * 68 >> 8)));
        vx_ring(&f, 0, 16, (14 * 8) + (en * 24 >> 8), (14 * 8) + (en * 24 >> 8), 210);
        vxs.dal = 0;
        vx_bface(&f, 0, 3 * 8, 72, (en * 307 >> 8), age < 90u, 0);
        f.cx = ocx;
        f.cy = ocy;
    }
    for (i = 0; i < 9; i++) {                            /* the rain: short streaks falling (not over the flag) */
        int32_t u = (int32_t)(((mt >> 8) * 47u / 10u + (uint32_t)i * 28u) & 255u), x = 28 + (i * 23) % 180, y = 98 + u * 96 / 256;
        if (x < 78 && y < 150) continue;
        vx_line(&f, (x - 120) * 8, (y - 128) * 8, (x - 120) * 8 - 19, (y - 128) * 8 + 56, 100);
    }
    {   /* the Belgian flag at the left, waving: black, yellow, red (the black dim, as muted), its edges rippling */
        int32_t px = -104 * 8, py = -24 * 8, fw = 46 * 8, fh = 28 * 8, b, k;
        static const int16_t FH[3] = {-100, -101, -102};
        uint32_t ph = vx_ang(mt, 103);
        vx_line(&f, px, py - 24, px, py + 52 * 8, 185);
        vx_dot(&f, px, py - 32, 22, 210);
        for (b = 0; b < 3; b++) {
            int32_t pxl = 0, pyt = 0, pyb = 0;
            for (k = 0; k <= 5; k++) {
                int32_t u = (b * 256 + k * 256 / 5) / 3, xx = px + 16 + u * fw / 256;
                int32_t amp = (13 + u * 36 / 256) * 8 / 10, wt = (og_sin(ph + (uint32_t)u * 36u * 17u / 10u) * amp) >> 15;
                int32_t wb = (og_sin(ph + (uint32_t)u * 36u * 17u / 10u + 7300u) * amp) >> 15;
                if (k) {
                    vx_line(&f, pxl, pyt, xx, py + wt, FH[b]);
                    vx_line(&f, pxl, pyb, xx, py + fh + wb, FH[b]);
                }
                if (b == 0 || k == 0) vx_line(&f, xx, py + wt, xx, py + fh + wb, FH[b]);
                if (k && k < 5) vx_line(&f, xx, py + wt + 40, xx, py + fh + wb - 40, FH[b]);
                pxl = xx; pyt = py + wt; pyb = py + fh + wb;
            }
        }
    }
    vxs.kc = 0;
}

/* -------------------------------------------------------------------------------------------- PHASE --- */
/* (BRUSSELS) THEO PP BOY, the planche's PHASE card (docs/creatures/creatures.js): a small boy at the frame's middle, his
 * jet aimed at one of three targets in turn (the puddle low on the left, the far Atomium, the tree high on the right).
 * The jet's arc and its drops; the level lifts the jet, the note flashes the target's ring. Coordinates: the 240 px card's,
 * 1 px = 8 Q4 units (f's own scale is applied by vx_line & co). */
#define PH_X(X) (((X) - 120) * 8)
#define PH_Y(Y) (((Y) - 120) * 8 - 80)
/* a card pixel, turned about the feet (85, 174). ang is a 16-bit turn: positive leans to the right. */
static void ph_pt(int32_t x, int32_t y, int32_t ang, int32_t *ox, int32_t *oy)
{
    int32_t dx = x - 85, dy = y - 174, c = og_cos((uint32_t)ang), s = og_sin((uint32_t)ang);
    *ox = PH_X(85 + ((dx * c - dy * s) >> 15));
    *oy = PH_Y(174 + ((dx * s + dy * c) >> 15));
}
static VX_CODE void vx_phase(const track_t *t, const int32_t *A, int32_t en_home)
{
    vxf_t f = vx_uf(A, 104, 128);
    int32_t en = vx_level(t, 7, en_home), i, k, q[2 * 27], h;
    uint32_t mt = vxs.mt[7];
    (void)vx_age(t, 7, 0);                               /* (the note's time, kept current) */
    {                                                    /* a new key: the next target */
        uint32_t i, young = 100000u;
        for (i = 0; i < NVOICE; i++)
            if (t->v[i].active && t->v[i].age < young)
                young = t->v[i].age;
        if (!vxs.ph_n && !vxs.ph_di)
            vxs.ph_di = 1;                               /* before the first key he aims at the Atomium, across the picture */
        if (young < vxs.ph_young) {
            vxs.ph_di = (uint8_t)((vxs.ph_di + 1u) % 3u);
            vxs.ph_n++;
        }
        vxs.ph_young = young;
    }
    int32_t di = vxs.ph_di, face = di == 0 ? -1 : di == 2 ? 1 : 0, ang;
    static const int32_t TX[3] = {36, 150, 214};   /* the targets: the puddle, the Atomium, the tree */
    int32_t x0 = 87 + face * 6, tgt = TX[di], Lx = tgt - x0;
    int32_t flow = 115 + en * 141 / 256;                /* Q8: the jet's strength with the level */
    int32_t H = (16 + (Lx < 0 ? -Lx : Lx) * 32 / 100) * flow / 256;   /* the jet's height, px */
    int32_t dq = 115 + (og_sin(vx_ang(mt, 500)) * 77 >> 15);      /* where the jet peaks along its way (Q8) */
    uint32_t hh;
    vxf_t boy;                                           /* the boy, two and a half times the scene: his hand stays put */
    boy.s = f.s * 5 / 2;
    boy.cx = f.cx + ((PH_X(87) * (f.s - boy.s)) >> 8);
    boy.cy = f.cy + ((PH_Y(155) * (f.s - boy.s)) >> 8);
    /* Drunk, not a fall. He keeps peeing. About every couple of seconds he leans fifteen
     * degrees, one side or the other, and he is back in half a second. Silent, he only
     * leans a little toward the target. 2731 is fifteen degrees in a 16-bit turn. */
    ang = face * 730;
    if (en > 8) {
        uint32_t step = fm1_ms / 1700u, u = fm1_ms % 1700u;
        int32_t side = ((step * 13u + vxs.ph_n) & 1u) ? 1 : -1;
        ang += (og_sin(vx_ang(mt, 160)) * 500) >> 15;
        if (u < 560u)
            ang += side * ((og_sin(u * 32768u / 560u) * 2731) >> 15);
    }
    vxs.kc = 1;
    /* the ground, the three targets: the one aimed at lit */
    vx_line(&f, -60 * 16, PH_Y(203), PH_X(68), PH_Y(203), 26);   /* the ground stops under him, so the line does not cut the legs */
    vx_line(&f, PH_X(104), PH_Y(203), 60 * 16, PH_Y(203), 26);
    for (k = 0; k < 3; k++) {
        int32_t lit = k == di ? 1 : 0;
        h = lit ? 230 : 105;
        if (k == 0) {
            vx_ring(&f, PH_X(36), PH_Y(203), 22 * 8, 4 * 8, h);
            vx_ring(&f, PH_X(36), PH_Y(203), 13 * 8, 2 * 8 + 2, h);
        } else if (k == 1) {
            vx_ring(&f, PH_X(150), PH_Y(172), 3 * 8 * 3, 3 * 8 * 3, h);
            vx_dot(&f, PH_X(150), PH_Y(172), 14, h);
        } else {
            vx_ring(&f, PH_X(214), PH_Y(168), 10 * 8, 10 * 8, h);
            vx_ring(&f, PH_X(203), PH_Y(175), 7 * 8, 7 * 8, h);
            vx_ring(&f, PH_X(225), PH_Y(175), 7 * 8, 7 * 8, h);
        }
    }
    vx_line(&f, PH_X(150), PH_Y(179), PH_X(150), PH_Y(202), 90);            /* the Atomium's stem */
    vx_line(&f, PH_X(214), PH_Y(202), PH_X(214), PH_Y(178), 110);           /* the tree's trunk */
    /* the boy: one piece, turned on his feet. No stone under him: the legs stay visible.
     * Legs green (96), body red (224), curls gold (160), so the four knob colours stay apart. */
    {
        int32_t lg = 96, a0, b0, a1, b1, bd[12], ar[6], hx, hy;
        ph_pt(82, 173, ang, &a0, &b0); ph_pt(82, 161, ang, &a1, &b1); vx_line(&boy, a0, b0, a1, b1, lg);
        ph_pt(88, 173, ang, &a0, &b0); ph_pt(88, 161, ang, &a1, &b1); vx_line(&boy, a0, b0, a1, b1, lg);
        ph_pt(83, 161, ang, &bd[0], &bd[1]); ph_pt(80, 154, ang, &bd[2], &bd[3]); ph_pt(82, 146, ang, &bd[4], &bd[5]);
        ph_pt(87, 146, ang, &bd[6], &bd[7]); ph_pt(89, 154, ang, &bd[8], &bd[9]); ph_pt(87, 161, ang, &bd[10], &bd[11]);
        vx_curve(&boy, bd, 6, 1, 224);
        ph_pt(85, 140, ang, &hx, &hy); vx_ring(&boy, hx, hy, 44, 44, 224);
        for (i = 0; i < 3; i++) {                        /* the curls of his hair */
            ph_pt(81 + i * 3, 135, ang, &hx, &hy);
            vx_arc(&boy, hx, hy, 14, 12, 32768u, 65536u, 6, 185);
        }
        ph_pt(88, 139, ang, &hx, &hy); vx_dot(&boy, hx, hy, 7, 243);
        ph_pt(83, 149, ang, &ar[0], &ar[1]); ph_pt(79, 153, ang, &ar[2], &ar[3]); ph_pt(82, 156, ang, &ar[4], &ar[5]);
        vx_curve(&boy, ar, 3, 0, 224);
        ph_pt(87, 149, ang, &a0, &b0); ph_pt(91, 155, ang, &a1, &b1); vx_line(&boy, a0, b0, a1, b1, 224);
    }
    /* the jet stays, in its own colour (navy), and it still grows with the level. */
    h = 48;
    {
        int32_t hx, hy, sx, sy;
        ph_pt(87 + face * 5, 157, ang, &hx, &hy);        /* the jet leaves the hand, in the boy's own size */
        sx = boy.cx + ((hx * boy.s) >> 8);
        sy = boy.cy + ((hy * boy.s) >> 8);
        for (i = 0; i <= 26; i++) {
            uint32_t u = (uint32_t)i * 256u / 26u;
            int32_t up = (int32_t)u < dq ? (int32_t)u * 128 / dq : 128 + ((int32_t)u - dq) * 128 / (256 - dq);
            int32_t s15 = og_sin((uint32_t)up * 128u);  /* sin(pi * up), Q15 */
            int32_t xp = x0 + (int32_t)u * Lx / 256, yp = 158 + (int32_t)u * (199 - 158) / 256 - (H * s15 >> 15);
            q[2 * i] = PH_X(xp);
            q[2 * i + 1] = PH_Y(yp);
        }
        if (f.s) { q[0] = ((sx - f.cx) << 8) / f.s; q[1] = ((sy - f.cy) << 8) / f.s; }
        for (i = 1; i <= 26; i++)
            vx_line(&f, q[2 * (i - 1)], q[2 * (i - 1) + 1], q[2 * i], q[2 * i + 1], h);
        for (k = 0; k < 7; k++) {                        /* the drops, along the arc, moving with the frames */
            uint32_t uq = ((mt >> 8) * 7u + (uint32_t)k * 256u / 7u) & 255u;
            int32_t up = (int32_t)uq < dq ? (int32_t)uq * 128 / dq : 128 + ((int32_t)uq - dq) * 128 / (256 - dq);
            int32_t s15 = og_sin((uint32_t)up * 128u);
            int32_t xp = x0 + (int32_t)uq * Lx / 256 + 3, yp = 158 + (int32_t)uq * 41 / 256 - (H * s15 >> 15) - 3;
            vx_dot(&f, PH_X(xp), PH_Y(yp), 6, 170);
        }
    }
    for (k = 0; k < 3; k++) {                             /* the target's rings, widening as the note goes */
        hh = ((mt >> 8) * 3u + (uint32_t)k * 10u) % 30u;
        {
            int32_t rr = 3 + 9 * (int32_t)hh / 30;
            vx_ring(&f, PH_X(tgt), PH_Y(199), rr * 8, rr * 8 * 22 / 100 + 1, 130 + (en * 90 >> 8));
        }
    }
    vxs.kc = 0;
}
#undef PH_X
#undef PH_Y

/* --------------------------------------------------------------------------------------------- LOFI --- */
/* (0.9.10) V1 "DIP": what each knob does to the drawing (lofi_desc's slots, as WAVE / CHIP make them: the knobs 0..127,
 * -1 where the slot is another's), its frame and the bits now */
typedef struct {
    vxf_t f;
    int32_t B, F, vb, half, duty, algo, wav, cg, varv, decay, vib, bend, tr, sd, loop, lp, ft, cut, res, dist, morph, fbk;
    int32_t en, vol, fx, bc[8];
} vxlf_t;
#define VX_LERP(a, b, e) ((a) + ((((b) - (a)) * (e)) >> 8))   /* a .. b at e (Q8) */
#define VX_CX0 (-46 * 16)                                #define VX_CX1 (46 * 16)
#define VX_CT (-6 * 16)
#define VX_CB (18 * 16)
#define VX_CYM (6 * 16)
/* smoothstep((m - a) / w), Q8 */
static int32_t vx_ss(int32_t m, int32_t a, int32_t w)
{
    int32_t t = clamp((m - a) * 256 / w, 0, 256);
    return t * t * (768 - 2 * t) >> 16;
}
/* MASK: the shift (Q4 px) of the 5 px band at y (Q4 units): the picture torn in horizontal bands */
static int32_t vx_lf_dx(const vxlf_t *L, int32_t y)
{
    int32_t b;
    if (L->tr < 3) return 0;
    b = (L->f.cy + ((y * L->f.s) >> 8)) / 80;
    if (vx_hash((uint32_t)(b * 131 + L->sd)) > L->tr * 150 / 127) return 0;
    return (vx_hash((uint32_t)(b * 7 + L->sd * 3)) - 128) * L->tr * 3 / 254;
}
static vxf_t vx_lf_at(const vxlf_t *L, int32_t y)        /* (the frame, torn where y is) */
{
    vxf_t g = L->f;
    g.cx += vx_lf_dx(L, y);
    return g;
}
static void vx_lf_line(const vxlf_t *L, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t h)
{
    vxf_t g = vx_lf_at(L, (y0 + y1) / 2);
    vx_line(&g, x0, y0, x1, y1, h);
}
static void vx_lf_poly(const vxlf_t *L, const int32_t *p, uint32_t n, int closed, int32_t h)
{
    uint32_t i;
    for (i = 1; i < n; i++)
        vx_lf_line(L, p[2u * i - 2u], p[2u * i - 1u], p[2u * i], p[2u * i + 1u], h);
    if (closed)
        vx_lf_line(L, p[2u * n - 2u], p[2u * n - 1u], p[0], p[1], h);
}
/* a skin's sample j of n (Q12) from half (0 the belly's, 1 the back's) of the 256: held (CRUSH / GRIT: the rate),
 * blurred as CUT closes (LP BP COMB), quantised to CHIP's bits (the outline stair-stepped), clipped by DIST */
static int32_t vx_lf_s(const vxlf_t *L, int32_t j, int32_t n, int32_t half)
{
    const int16_t *H = vxs.wv + half * 128;
    int32_t k = 1 + L->cg * 10 / 127, idx = j * 127 / n / k * k, wn = L->ft == 1 ? 0 : ((127 - L->cut) * 14 + 63) / 127, s, q;
    if (wn) {
        for (s = 0, q = -wn; q <= wn; q++)
            s += H[clamp(idx + q, 0, 127)];
        s = (s / (2 * wn + 1)) * (256 + (127 - L->cut) * 384 / 127) >> 8;
    } else
        s = H[idx];
    if (L->vb == 1)
        s = s >= 0 ? 4096 : -4096;
    else if (L->vb == 4) {
        q = L->half ? 2 : 3;
        s = ((s * q + 2048) >> 12) * 4096 / q;
    }
    if (L->dist) {
        q = 4096 - L->dist * 2867 / 127;
        s = clamp((s * (256 + L->dist * 384 / 127)) >> 8, -q, q) * 4096 / q;
    }
    return s;
}
/* segment i's light (Q8): its bit (BYTE: the highest next to the head) or the wave's level there; LOOP repeats them;
 * STEP's DECAY lights them as a staircase, with the 4-bit volume */
static int32_t vx_lf_light(const vxlf_t *L, int32_t i)
{
    int32_t k = 1 + (i - 1) % L->lp, v = vxs.wv[k * 30 + 6];
    v = L->B ? L->bc[8 - k] : clamp((v < 0 ? -v : v) * 115 / 100, 0, 4096) * 218 / 4096 + 13;
    if (L->decay >= 0)
        v = 9 - i <= (L->vol * 8 + 7) / 15 ? (v > 192 ? v : 192) : v / 4;
    return v;
}
/* a skin drawn point by point: its last point, its sample, COMB's second skin, RES's contours */
typedef struct { int32_t x, y, s, cx, cy, rx[3], ry[3]; } vxsk_t;
/* point j of n at (x, y) (Q4 units), its outward normal (nx, ny: Q8), its sample s (Q12): the skin (BP only its middle,
 * hotter with the sample, DIST, colder as CUT closes), COMB a second one, RES 1..3 ringing contours outside, HP's cilia
 * growing as CUT opens, DIST spikes where it clips; fx fades the marks (the chip) */
static void vx_lf_pt(const vxlf_t *L, vxsk_t *k, int32_t x, int32_t y, int32_t nx, int32_t ny, int32_t s, int32_t j, int32_t n)
{
    int32_t as = s < 0 ? -s : s, cx = x + ((nx * 42) >> 8), cy = y + ((ny * 42) >> 8), q;
    if (j) {
        int32_t al = 256;
        if (L->ft == 2) al = clamp(((og_sin((uint32_t)j * 32768u / (uint32_t)n) * 410) >> 15) - 51, 38, 256);
        vxs.dal = (int16_t)(256 - al);
        vx_lf_line(L, k->x, k->y, x, y, 46 + as * 130 / 4096 + (L->en * 40 >> 8) + L->dist * 60 / 127 -
                   (L->ft == 1 ? 0 : (127 - L->cut) * 45 / 127));
        if (L->ft == 3) {
            vxs.dal = 56;
            vx_lf_line(L, k->cx, k->cy, cx, cy, 36 + as * 90 / 4096);
        }
        vxs.dal = (int16_t)(256 - L->fx);
        if (L->dist > 12 && L->fx > 5 && as > 4014 && (k->s < 0 ? -k->s : k->s) <= 4014)
            vx_lf_line(L, x, y, x + ((nx * 56 * L->dist / 127) >> 8), y + ((ny * 88 * L->dist / 127) >> 8), 240);
    }
    if (L->res > 5 && L->fx > 5 && j % 6 == 0)
        for (q = 0; q < (L->res * 3 + 126) / 127; q++) {
            int32_t o = (q + 1) * 34, rx = x + ((nx * o) >> 8), ry = y + ((ny * o) >> 8);
            vxs.dal = (int16_t)(256 - (L->fx * (256 - q * 80) >> 8));
            if (j) vx_lf_line(L, k->rx[q], k->ry[q], rx, ry, 70 + L->res * 150 / 127 - (q + 1) * 35);
            k->rx[q] = rx;
            k->ry[q] = ry;
        }
    if (L->ft == 1) {
        int32_t c8 = L->cut * 256 / 127, hl = ((((c8 * (205 + (c8 * 51 >> 8))) >> 8) * 112 * (115 + L->en)) >> 16) * L->fx >> 8;
        int32_t st = (n * 254 / (6 + L->cut * 18 / 127) + 1) / 2;
        st = st > 2 ? st : 2;
        if (hl > 6 && j % st == st >> 1) {
            int32_t w = og_sin((uint32_t)j * 17732u + (uint32_t)L->en * 367u) >> 1;
            vxs.dal = 46;
            vx_lf_line(L, x, y, x + (((nx + ((ny * w) >> 15)) * hl) >> 8), y + (((ny - ((nx * w) >> 15)) * hl) >> 8),
                       60 + L->cut * 90 / 127 + (L->en * 50 >> 8));
        }
    }
    vxs.dal = 0;
    k->x = x; k->y = y; k->s = s; k->cx = cx; k->cy = cy;
}
/* ORGANISMO DE CIRCUITO -> LE TRAM SNCB (the planche's LOFI card, docs/creatures/creatures.js): a real tram seen from the
 * side, rolling on its rail under a wire; the wire's line is the wave, stair-stepped by the bits (CHIP's bits, CRUSH,
 * DIST, RES, HP's edges: vx_lf_pt; VIB trembles it, MORPH lowers it, LOOP lifts its rear, WAV# stands the table on it).
 * Its eight windows light with the bits (BYTE's, STEP's staircase; DUTY their width, VAR scatters them, HP leaves their
 * edges); ALGO's formula bits sit in the livery; FEEDBK's echoes trail the wire. It vibrates with the level: the body
 * bobs and rattles on its suspension, the wheels hop and turn with the crawl, the sleepers run back, the headlight's
 * beam grows; the pantograph (BEND slides it) sparks at the wire on every note. */
static VX_CODE void vx_lofi(const track_t *t, const int32_t *A, int32_t en_home)
{
    const int16_t *p = t->p;
    vxlf_t L;
    vxsk_t sk;
    int32_t a0 = A[2] * 256 / 124, b0 = (A[3] - 4) * 256 / 94, w = p[P_E1], chip = p[P_E0], BF, step, hold, fresh;
    uint32_t gap = ui.frame - vxs.fr[8], dm = fm1_ms - vxs.ms[8], new_fr = gap != 0u;
    int32_t en, i, j, k, row, m, e1, pk = 6000, jsc, nt, amp, px, vb, wr = 56, bob, sx, sc, bp[14], wq[8];
    int32_t rx[41], ry[41], sv[41];
    uint32_t mt, age, w0 = scope_w - 256u, byte = 0, cr, tt = 0, o, tc;
    vxf_t g;
    L.f.s = (a0 < b0 ? a0 : b0) * 106 / 100;
    L.f.cx = (A[0] * 2 + A[2]) * 8 - ((32 * L.f.s) >> 8);
    L.f.cy = (A[1] * 2 + A[3]) * 8 + 112;
    vxs.kc = 1;
    L.B = w == RW_BYTE;                                  /* the knobs, as the slots mean them now */
    L.F = w == RW_FLOAT;
    BF = L.B || L.F;
    step = chip == CHIP_STEP && !L.F;
    L.vb = chip == CHIP_8BIT ? 8 : chip == CHIP_1BIT ? 1 : 4;
    L.half = chip == CHIP_4B2;
    L.duty = w == RW_PLS ? p[P_E2] : -1;
    L.algo = BF ? p[P_E2] >> 2 : -1;
    L.wav = w == RW_WRAM ? (p[P_E2] >> 3) & 15 : -1;
    L.cg = (!BF && !step ? p[P_E3] : 0) + (L.B ? p[P_E5] : 0);   /* (CRUSH, GRIT: the rate) */
    L.varv = BF ? p[P_E3] : 0;
    L.decay = step ? p[P_E3] >> 2 : -1;
    L.vib = BF ? 0 : p[P_E5];
    L.fbk = L.F ? p[P_E5] : 0;
    L.bend = p[P_E6];
    L.tr = BF ? 0 : p[P_E7];
    L.loop = BF ? p[P_E7] : 0;
    L.lp = L.loop > 2 ? 8 >> (L.loop * 4 / 127 < 3 ? L.loop * 4 / 127 : 3) : 8;
    L.ft = p[P_FTYPE] & 3;
    L.cut = p[P_FCUT];
    L.res = p[P_FRES];
    L.dist = p[P_DIST];
    L.morph = w < RW_BYTE ? p[P_MORPH] : 0;
    en = vx_level(t, 8, en_home);
    L.en = en;
    /* CHIP -> the morph's target (8BIT / FULL the organism, 4BIT and STEP half way, 4B/2 more, 1BIT the chip): a glide */
    m = chip == CHIP_8BIT ? 0 : chip == CHIP_1BIT ? 256 : chip == CHIP_4B2 ? 179 : chip == CHIP_STEP ? 115 : 102;
    if (gap > 8u || vxs.lm < 0 || (ui_prefs & PREF_ANIM_OFF))
        vxs.lm = (int16_t)m;                             /* (just shown, unset, or at rest: there at once) */
    else if (new_fr) {
        j = (m - vxs.lm) * (int32_t)(dm < 100u ? dm : 100u) / 256;
        vxs.lm = (int16_t)(vxs.lm + (j ? j : (m > vxs.lm) - (m < vxs.lm)));
    }
    m = vxs.lm;
    e1 = vx_ss(m, 0, 218);                               /* the wire flattening into the chip */
    L.fx = 256 - (e1 * 205 >> 8);
    age = vx_age(t, 8, 0);
    nt = vx_fall(age, 154u);                             /* the note: the pantograph's sparks */
    for (i = 0, j = 0; i < 256; i++) {                   /* the last 256 samples out (the scope's): their peak */
        int32_t v2 = scope_buf[(w0 + (uint32_t)i) & (SCOPE_N - 1u)];
        v2 = v2 < 0 ? -v2 : v2;
        pk = v2 > pk ? v2 : pk;
    }
    jsc = (4096 << 15) / pk;
    for (i = 0, j = -1; i < NVOICE; i++)                 /* the newest voice: the formula's time, STEP's volume */
        if ((int32_t)t->v[i].age > j) { j = (int32_t)t->v[i].age; tt = (uint32_t)t->v[i].s[2]; L.vol = t->v[i].s[6] & 15; }
    /* CRUSH / GRIT: the drawing's inputs held 1 .. 6 frames (the stutter); taken each frame otherwise, and in silence */
    hold = 1 + (L.cg * 5 + 63) / 127;
    fresh = new_fr && (hold == 1 || !en || gap > 8u || (uint8_t)((vxs.mt[8] >> 8) - vxs.lst) >= (uint32_t)hold);
#define VH(i) vxs.wv[i]
    if (fresh) {
        for (i = 0; i < 256; i++)
            vxs.wv[i] = (int16_t)((scope_buf[(w0 + (uint32_t)i) & (SCOPE_N - 1u)] * jsc) >> 15);
        byte = (uint32_t)(VH(255) / 32 + 128) & 255u;   /* the byte now: BYTE's held sample, else the output's level */
        for (i = 0, j = 0; i < NVOICE; i++)
            if (L.B && t->v[i].active && t->v[i].age >= (uint32_t)j) { j = (int32_t)t->v[i].age; byte = (uint32_t)((t->v[i].s[0] + 32640) >> 8) & 255u; }
        for (i = 0; i < 8; i++)                          /* (its bits slewed) */
            vxs.bd[i] = (int16_t)(vxs.bd[i] + (((byte >> i & 1u) ? en : 0) - vxs.bd[i]) * 141 / 256);
        vxs.crawl += (((vxs.mt[8] - vxs.cmt) * (uint32_t)(307 + 4 * en) >> 8) * 10430u / 7680u) * (uint32_t)(BF ? vx_x2((p[P_E4] - 64) * 12) : 4096) >> 12;   /* the crawl */
        vxs.cmt = vxs.mt[8];
        vxs.lst = (uint8_t)(vxs.mt[8] >> 8);
    }
    mt = vxs.cmt;                                        /* (the time as taken: held by the stutter too) */
    cr = vxs.crawl;
    L.sd = (int32_t)((mt >> 8) / 3u);                    /* (MASK's bands: new every 3 frames, with the sound) */
    for (i = 0; i < 8; i++)
        L.bc[i] = clamp((vxs.bd[i] - 46) * 410 / 256, 0, 256);
    /* the wire over the tram: the wave (the bits' stair, CRUSH, DIST, RES, HP's edges: vx_lf_pt); VIB trembles it, MORPH
     * lowers it, LOOP lifts its rear end; the level makes it dance */
    amp = (80 + en * 40 / 256) * (256 - e1) / 256;
    for (j = 0; j <= 40; j++) {
        rx[j] = -960 + j * 1920 / 40;
        sv[j] = vx_lf_s(&L, j, 40, 0);
        vb = (og_sin(vx_ang(mt, 1299) + (uint32_t)j * 2048u) * (L.vib * 40 / 127)) >> 15;
        ry[j] = -512 + L.morph * 120 / 127 - (L.loop * 2 * j * j / 1600) - ((sv[j] * amp) >> 12) + vb;
    }
    for (j = 0; j <= 40; j++)
        vx_lf_pt(&L, &sk, rx[j], ry[j], 0, -256, sv[j], j, 41);
    /* the rail and its sleepers: they run back as the wheels turn (the ground's speed is the wheel's: 2 pi r) */
    vx_lf_line(&L, -992, 328, 992, 328, 90);
    sc = (int32_t)(((uint64_t)cr * 352u >> 16) % 1980u);
    for (k = 0; k < 11; k++) {
        int32_t x = -992 + ((k * 180 - sc) % 1980 + 1980) % 1980;
        vx_lf_line(&L, x, 328, x - 24, 376, 55);
    }
    /* the body: it breathes on its suspension with the level, and rests in silence */
    bob = en > 8 ? (og_sin(vx_ang(mt, 180)) * (24 + en / 8)) >> 15 : 0;
    sx = en > 8 ? (og_sin(vx_ang(mt, 90)) * (14 + en / 10)) >> 15 : 0;
    {   /* the outline: the rear, the roof, the sloped windscreen, the rounded nose, the floor */
        static const int16_t BO[14] = {-832, 224, -832, -96, -784, -160, 640, -160, 784, -16, 832, 80, 832, 224};
        for (i = 0; i < 7; i++) { bp[2 * i] = BO[2 * i] + sx; bp[2 * i + 1] = BO[2 * i + 1] + bob; }
        vx_lf_poly(&L, bp, 7, 1, 150 + (en * 60 >> 8));
    }
    vx_lf_line(&L, -832 + sx, 144 + bob, 832 + sx, 144 + bob, 105);       /* the livery's stripes */
    vx_lf_line(&L, -832 + sx, 176 + bob, 832 + sx, 176 + bob, 75);
    vx_text(L.f.cx + (((-16 + sx) * L.f.s) >> 8), L.f.cy + (((48 + bob) * L.f.s) >> 8), "SNCB", vx_col(190 + (en * 50 >> 8)));   /* its name on the side */
    /* the windows: one per bit (BYTE's, STEP's staircase; DUTY their width, VAR scatters them, HP leaves their edges) */
    for (i = 1; i <= 8; i++) {
        int32_t hw = L.duty >= 0 ? 40 + L.duty * 30 / 127 : 56, wy0 = -96 + bob, wy1 = 32 + bob, xc, v = vx_lf_light(&L, i);
        xc = -656 + (i - 1) * 160 + sx + (vx_hash((uint32_t)((L.varv >> 4) * 31 + i)) - 128) * L.varv * 24 / (127 * 128);
        wq[0] = xc - hw; wq[1] = wy0; wq[2] = xc + hw; wq[3] = wy0; wq[4] = xc + hw; wq[5] = wy1; wq[6] = xc - hw; wq[7] = wy1;
        vxs.dal = (int16_t)(L.ft == 1 ? 146 : 0);
        vx_lf_poly(&L, wq, 4, 1, 60 + (v * 180 >> 8));
        vxs.dal = 0;
        if (v > 115) {                                    /* a lit window glows: a dot, and the glass's diagonal */
            g = vx_lf_at(&L, (wy0 + wy1) / 2);
            vx_dot(&g, xc, (wy0 + wy1) / 2, (v - 115) * L.f.s / 2400, 200 + (v * 40 >> 8));
            vx_lf_line(&L, xc - hw + 12, wy1 - 10, xc - 8, wy0 + 10, 40 + (v * 80 >> 8));
        }
    }
    {   /* the two doors (between windows 3-4 and 6-7), the windscreen */
        static const int16_t DX[4] = {-280, -232, 200, 248};
        for (i = 0; i < 4; i++)
            vx_lf_line(&L, DX[i] + sx, -128 + bob, DX[i] + sx, 208 + bob, 85);
        vx_lf_line(&L, -280 + sx, -128 + bob, -232 + sx, -128 + bob, 85);
        vx_lf_line(&L, 200 + sx, -128 + bob, 248 + sx, -128 + bob, 85);
        wq[0] = 576 + sx; wq[1] = -96 + bob; wq[2] = 656 + sx; wq[3] = -96 + bob; wq[4] = 752 + sx; wq[5] = -16 + bob; wq[6] = 752 + sx; wq[7] = 32 + bob;
        vx_lf_poly(&L, wq, 4, 0, 120 + (en * 90 >> 8));
        vx_lf_line(&L, 576 + sx, 32 + bob, 752 + sx, 32 + bob, 120 + (en * 90 >> 8));
        vx_lf_line(&L, 576 + sx, -96 + bob, 576 + sx, 32 + bob, 120 + (en * 90 >> 8));
    }
    {   /* the headlight and its beam, with the level */
        g = vx_lf_at(&L, 112);
        vx_dot(&g, 816 + sx, 112 + bob, 28, 160 + (en * 90 >> 8));
        if (en > 20) {
            vxs.dal = (int16_t)(256 - (en * 150 >> 8));
            vx_lf_line(&L, 836 + sx, 100 + bob, 992, 36, 190);
            vx_lf_line(&L, 836 + sx, 124 + bob, 992, 188, 190);
            vxs.dal = 0;
        }
    }
    /* the wheels: two bogies of two, turning with the crawl, hopping a little with the level */
    for (k = 0; k < 4; k++) {
        int32_t wx = (k < 2 ? -608 + k * 160 : 448 + (k - 2) * 160) + sx / 2;
        int32_t wy = 272 + (((vx_hash((uint32_t)((mt >> 3) + (uint32_t)k * 7u)) - 128) * en / 256 * 8) >> 7);
        uint32_t a = cr + (uint32_t)k * 9000u;
        int32_t cs = og_cos(a) * wr >> 15, sn = og_sin(a) * wr >> 15;
        g = vx_lf_at(&L, wy);
        vx_ring(&g, wx, wy, wr, wr, 120 + (en * 70 >> 8));
        vx_line(&g, wx - cs, wy - sn, wx + cs, wy + sn, 150 + (en * 80 >> 8));
        vx_line(&g, wx + sn, wy - cs, wx - sn, wy + cs, 110);
        vx_dot(&g, wx, wy, 12, 180);
    }
    /* the pantograph on the roof under the wire, BEND sliding it along the roof; its contact sparks on the note */
    px = (L.bend - 64) * 4 - 288;
    {
        int32_t jj = clamp((px + 960) * 40 / 1920, 0, 40), cy = ry[jj] + 8, rf = -160 + bob, mid = (rf + cy) / 2;
        vx_lf_line(&L, px - 88 + sx, rf, px - 40, mid, 110 + (en * 70 >> 8));
        vx_lf_line(&L, px - 40, mid, px, cy, 110 + (en * 70 >> 8));
        vx_lf_line(&L, px + 88 + sx, rf, px + 40, mid, 110 + (en * 70 >> 8));
        vx_lf_line(&L, px + 40, mid, px, cy, 110 + (en * 70 >> 8));
        vx_lf_line(&L, px - 96 + sx, rf, px + 96 + sx, rf, 90);
        vx_lf_line(&L, px - 56, cy, px + 56, cy, 200);
        if (nt > 20)
            for (i = 0; i < 5; i++) {
                uint32_t a = (uint32_t)(vx_hash((uint32_t)((mt >> 5) + (uint32_t)i * 7u)) * 256u);   /* a random spark */
                int32_t len = nt * 110 / 256, ss = og_sin(a), cs = og_cos(a);
                vx_lf_line(&L, px, cy, px + (cs * len >> 15), cy + (ss * len >> 15), 235);
            }
    }
    /* ALGO: the formula's bits in the livery's band (4 rows, read at t) */
    if (L.algo >= 0)
        for (i = 1; i <= 8; i++) {
            tc = (tt >> 7) + (uint32_t)(i - 1) * 37u;
            o = L.F ? lofi_algo_at(1, (uint32_t)L.algo, tc, 0) >> 12 : lofi_algo_at(0, (uint32_t)L.algo, tc, 1u + (uint32_t)(L.varv >> 3)) >> 3;
            for (row = 0; row < 4; row++)
                if (o >> row & 1) {
                    int32_t y = 116 + row * 28 + bob;
                    g = vx_lf_at(&L, y);
                    vx_dot(&g, -656 + (i - 1) * 160 + sx, y, 12, 200 + (en * 50 >> 8));
                }
        }
    /* FEEDBK: two echoes of the wire, trailing behind */
    if (L.fbk > 2)
        for (i = 2; i >= 1; i--) {
            vxs.dal = (int16_t)(256 - L.fbk * 179 / 127 / i);
            for (j = 1; j <= 40; j++)
                vx_lf_line(&L, rx[j - 1] - i * 72, ry[j - 1] - i * 24, rx[j] - i * 72, ry[j] - i * 24, 60);
            vxs.dal = 0;
        }
    /* WAV#: the wave table stands on the wire, a spine per sample */
    if (L.wav >= 0)
        for (k = 0; k < 32; k++) {
            j = k * 40 / 32;
            {
                int32_t l = (int32_t)lofi_wram_nib((uint32_t)L.wav, (uint32_t)k) * 96 / 15;
                if (l > 4) vx_lf_line(&L, rx[j], ry[j], rx[j], ry[j] - l, 90 + l * 20 / 16);
            }
        }
#undef VH
    vxs.kc = 0;
}

/* ------------------------------------------------------------------------------------------------ the glue --- */
/* the engine's own creature in the rectangle x y w h (px: EDIT's left panel 8 14 108 112, HOME's middle 146 x 112, or a
 * whole panel: each fits itself to it), en_home: HOME's level for the track (-1: its own); 0 when the engine has none
 * (its being then) */
static int32_t vx_fit(int32_t w, int32_t h, int32_t uw, int32_t uh)   /* a unit's px (Q8): uw x uh units in w x h px */
{
    return w * 256 / uw < h * 256 / uh ? w * 256 / uw : h * 256 / uh;
}
static int vx_draw(const track_t *t, int32_t x, int32_t y, int32_t w, int32_t h, int32_t en_home)
{
    const engine_t *e = trk_engp(t);
    int32_t sway = 0, bob = 0, A[4], cx, cy, s;
    if (!(ui_prefs & PREF_ANIM_OFF)) {                   /* (BRUSSELS) every drawing, the same slow drift, only while it sounds */
        uint32_t i;
        for (i = 0; i < NVOICE && !t->v[i].active; i++) {}
        if (i < NVOICE) {
            sway = og_sin(ui.frame * 140u) * 3 >> 15;
            bob = og_sin(ui.frame * 90u + 9000u) * 2 >> 15;
        }
    }
    x += sway;
    y += bob;
    A[0] = x; A[1] = y; A[2] = w; A[3] = h;
    cx = x * 16 + w * 8;
    cy = y * 16 + h * 8;
    if (trk_eng(t) == ENGI_DRUM) {    /* (the kit ~128 x 92) */
        s = vx_fit(w, h, 128, 92);
        vx_kit(t, cx - (s >> 4), cy + (48 * s >> 8), s, en_home, -1);
    } else if (e == &ENG_ANIMAL)                        /* (the animal ~103 x 78) */
        vx_animal(t, cx, cy, vx_fit(w, h, 103, 78), en_home);
    else if (e == &ENG_ANALOG)
        vx_analog(t, A, en_home);
    else if (trk_eng(t) == ENGI_FM6)
        vx_fm6(t, A, en_home, 6u, 0);
    else if (e == &ENG_WHEEL)
        vx_wheel(t, A, en_home);
    else if (e == &ENG_NOISE)
        vx_noise(t, A, en_home);
    else if (e == &ENG_PHASE)
        vx_phase(t, A, en_home);
    else if (e == &ENG_LOFI)
        vx_lofi(t, A, en_home);
    else
        return 0;
    return 1;
}
/* (BRUSSELS) a being of HOME, the save slots and the DRUM-X colony: one of the fanfare's drawings, whole, at (cx, cy) Q4,
 * H px tall (r / 8; BXF_TINY 10), at level en (0..256) and time t. While the note sounds the whole drawing
 * breathes and shifts; quiet or asleep, it rests. The kit's own lines (vk_fries, vk_manneken, vx_kit's waffle, chou,
 * mussels, praline, sauce bottle), the Atomium drawn here. vx_bk: the drawing's box, (ux, uy) its middle and nat its
 * height (Q4 units at s = 256), fy its foot (the squash's) */
static void vx_bk(vxk_t *k, int32_t cx, int32_t cy, int32_t H, int32_t ux, int32_t uy, int32_t nat, int32_t fy, int32_t lift)
{
    int32_t s = H * 4096 / nat;
    k->f.s = s;
    k->f.cx = cx - ((ux * s) >> 8);
    k->f.cy = cy - ((uy * s) >> 8) - lift;
    k->ox = ux;
    k->oy = fy;
}
static void vx_being(uint32_t who, int32_t cx, int32_t cy, int32_t r, int32_t en, uint32_t t, uint32_t fl)
{
    int32_t H = fl & BXF_TINY ? 10 : clamp(r / 8, 8, 120), e = fl & BXF_HIT ? 256 : en, lit = fl & BXF_SLEEP ? -1 : 0, lift = 0, sq, i;
    int32_t h0 = lit < 0 ? -1 : 0, x, y, c;
    uint8_t kc = vxs.kc;
    int16_t dal = vxs.dal;
    vxk_t k;
    if (lit < 0) e = 0;
    lift = 0;
    sq = 0;
    if (e > 8) {                                         /* the whole drawing breathes with the note, and rests in silence */
        int32_t br = (og_sin(t) * (18 + e / 10)) >> 15, side = (og_sin(t + 20000u) * (5 + e / 24)) >> 15;
        H += H * br / 256;
        if (H < 8) H = 8;
        lift = br * 3;
        cx += side * 16;
    }
    vxs.kc = 1;
    vxs.dal = 0;
    k.sx = 256 + sq * 36 / 256;
    k.sy = 256 - sq * 51 / 256;
    switch (who) {
    case BX_FRIES:
        vx_bk(&k, cx, cy, H, 0, 184, 560, VK_Y(200), lift);
        vk_fries(&k, lit, e);
        break;
    case BX_MANNEKEN:
        vx_bk(&k, cx, cy, H, 608, 175, 692, 0, lift);
        vk_manneken(&k.f, e, lit);
        break;
    case BX_WAFFLE: {
        int32_t kx0 = VK_X(44), ky0 = VK_Y(158), kw = 36 * 8, kh = 22 * 8, p4[8], hl = lit < 0 ? -1 : 130;
        vx_bk(&k, cx, cy, H, VK_X(62), 272, 512, VK_Y(176), lift);
        for (i = -1; i <= 1; i++)
            vx_line(&k.f, VK_X(62), VK_Y(176), VK_X(62 + i * 8), VK_Y(206), lit < 0 ? -1 : 95);
        p4[0] = kx0; p4[1] = ky0; p4[2] = kx0 + kw; p4[3] = ky0; p4[4] = kx0 + kw; p4[5] = ky0 + kh; p4[6] = kx0; p4[7] = ky0 + kh;
        vk_poly(&k, p4, 4, 1, vk_h(lit, 165, e, 80));
        for (i = 1; i < 4; i++) vk_line(&k, kx0 + i * kw / 4, ky0, kx0 + i * kw / 4, ky0 + kh, hl);
        for (i = 1; i < 3; i++) vk_line(&k, kx0, ky0 + i * kh / 3, kx0 + kw, ky0 + i * kh / 3, hl);
        vk_face(&k, VK_X(62), VK_Y(150), 8, e, e > 90, lit);
        break;
    }
    case BX_CHOU:                                        /* the choux on its little legs */
        x = VK_X(92); y = VK_Y(118);
        vx_bk(&k, cx, cy, H, x, y + 35, 211, y + 70, lift);
        vx_line(&k.f, x - 24, y + 64, x - 48, y + 141, lit < 0 ? -1 : 80);
        vx_line(&k.f, x + 24, y + 64, x + 48, y + 141, lit < 0 ? -1 : 80);
        c = vk_h(lit, 150, e, 90);
        vk_ell(&k, x, y, 70, 70, 0, 65536u, c);
        vk_ell(&k, x, y + 32, 58, 58, 37683u, 60621u, lit < 0 ? -1 : 115);
        vk_ell(&k, x - 32, y, 45, 58, (uint32_t)-7301, 8344u, lit < 0 ? -1 : 115);
        vk_face(&k, x + 8, y + 8, 7, e, e > 90, lit);
        break;
    case BX_MUSSEL: {                                    /* the mussels on their stem, the shell opening with the sound */
        int32_t up = e * 72 / 256;
        x = VK_X(26);
        vx_bk(&k, cx, cy, H, x, 244, 568, VK_Y(146), lift);
        vx_line(&k.f, x, VK_Y(206), x, VK_Y(148), lit < 0 ? -1 : 95);
        c = vk_h(lit, 150, e, 90);
        vk_ell(&k, x, VK_Y(144), 16 * 8, 9 * 8, 2086u, 30682u, c);
        for (i = 1; i <= 3; i++)
            vk_ell(&k, x, VK_Y(144), (16 * 8 * 10 - i * 34 * 8) / 10, (9 * 8 * 10 - i * 16 * 8) / 10, 4693u, 28075u, lit < 0 ? -1 : 115);
        vk_ell(&k, x, VK_Y(142) - up, 16 * 8, 7 * 8, 34854u, 63450u, c);
        vk_ell(&k, x, VK_Y(142) - up, 10 * 8, 4 * 8, 37461u, 60843u, lit < 0 ? -1 : 115);
        break;
    }
    case BX_PRALINE:                                     /* (reads as a rectangle with dots: the fries instead) */
        vx_bk(&k, cx, cy, H, 0, 184, 560, VK_Y(200), lift);
        vk_fries(&k, lit, e);
        break;
    case BX_SAUCE: {                                     /* the sauce bottle standing, its neck up, a drop at its lip */
        static const int8_t BB[16] = {-12, -5, 6, -5, 10, -2, 10, 2, 6, 5, -12, 5, -16, 2, -16, -2}, NN[8] = {-16, -3, -16, 3, -22, 1, -22, -1};
        int32_t q[16];
        vx_bk(&k, cx, cy, H, 0, -48, 288, 80, lift);
        for (i = 0; i < 8; i++) { q[2 * i] = BB[2 * i + 1] * 8; q[2 * i + 1] = BB[2 * i] * 8; }
        vk_poly(&k, q, 8, 1, vk_h(lit, 205, e, 40));
        for (i = 0; i < 4; i++) { q[2 * i] = NN[2 * i + 1] * 8; q[2 * i + 1] = NN[2 * i] * 8; }
        vk_poly(&k, q, 4, 1, lit < 0 ? -1 : 175);
        vk_face(&k, 0, 0, 5, e, e > 90, lit);
        if (e > 40)
            vx_arc(&k.f, 0, -200 - e / 2, 14, 18, 0, 65536u, 8, lit < 0 ? -1 : 175);
        break;
    }
    default: {                                           /* the Atomium: nine spheres, a cube's eight corners round the centre */
        static const int8_t CB[8][3] = {{-1, -1, -1}, {1, -1, -1}, {-1, 1, -1}, {1, 1, -1}, {-1, -1, 1}, {1, -1, 1}, {-1, 1, 1}, {1, 1, 1}};
        int16_t px[8], py[8];
        int32_t h = vk_h(lit, 150, e, 60), a, b2;
        vx_bk(&k, cx, cy, H, 0, 0, 460, 190, lift);
        for (i = 0; i < 8; i++) { px[i] = (int16_t)((CB[i][0] - CB[i][2]) * 78); py[i] = (int16_t)(CB[i][1] * 86 + (CB[i][0] + CB[i][2]) * 40); }
        for (a = 0; a < 8; a++)
            for (b2 = a + 1; b2 < 8; b2++)               /* a tube where two corners share an edge of the cube */
                if ((CB[a][0] != CB[b2][0]) + (CB[a][1] != CB[b2][1]) + (CB[a][2] != CB[b2][2]) == 1)
                    vx_line(&k.f, px[a], py[a], px[b2], py[b2], h0 ? -1 : 110);
        for (i = 0; i < 8; i++)
            vx_line(&k.f, 0, 0, px[i], py[i], h0 ? -1 : 110);
        for (i = 0; i < 8; i++)
            vx_arc(&k.f, px[i], py[i], 40, 40, 0, 65536u, 12, vk_h(lit, 120, e, 60));
        vx_arc(&k.f, 0, 0, 58, 58, 0, 65536u, 14, h);   /* the ninth, the centre */
        vx_bface(&k.f, 0, 0, 58, e, e > 90, 0);
        break;
    }
    }
    vxs.kc = kc;
    vxs.dal = dal;
}
/* (BRUSSELS) the Piknik logo's outline. Colour modes: the five neon colours of the fries, crawling.
 * GREY and MONO stay grey. bob shifts the whole mark with the kit. */
static void vx_logo_outline(int32_t cx, int32_t y, int32_t bob)
{
    uint16_t i;
    int32_t x0 = cx - PIK_LOGO_W / 2;
    int neon = !ux.mono && settings.palette != UI_GREY_INDEX && settings.palette != UI_BW_INDEX;
    uint32_t shift = (fm1_ms / 160u) % 5u;
    for (i = 0; i < PIK_EDGE_N; i++)
        cv_rect(x0 + PIK_EDGE_RUNS[i][1], y + bob + PIK_EDGE_RUNS[i][0], PIK_EDGE_RUNS[i][2], 1,
                neon ? VK_FRY[(i + shift) % 5u]
                     : (settings.palette == UI_WHITE_INDEX ? vx_white(PIK_EDGE_RUNS[i][3]) : bg_col(PIK_EDGE_RUNS[i][3])));
}
/* EDIT > KIT (lane -1) and SOUND 1..4 (a DRUM track): the kit large on the panel, KIT over it on the left; on the right
 * the sound last struck (KIT) or the sound edited, lit (SOUND: lane) */
static void vx_kit_page(const track_t *t, int32_t lane)
{
    uint32_t l;
    if (lane < 0) {                                     /* (BRUSSELS) EDIT > KIT: BRUSSELS, the Piknik logo in neon under it */
        int32_t hit = 0, j, bob = 0;
        drum_lane_t *K = drum_kit_of(t);
        int neon = !ux.mono && settings.palette != UI_GREY_INDEX && settings.palette != UI_BW_INDEX;
        if (K)
            for (j = 0; j < 8; j++)
                if (K[j].x.live && (K[j].x.ea >> 22) > hit) hit = K[j].x.ea >> 22;
        if (!(ui_prefs & PREF_ANIM_OFF))
            bob = (og_sin(fm1_ms * 28u) * (3 + hit / 32)) >> 15;
        cv_text_in(0, 39 + bob, 240, &AF_L, "BRUSSELS", neon ? VK_FRY[(fm1_ms / 160u) % 5u] : T_TEXT, T_BG);
        vx_logo_outline(120, 75, bob);
    }
    else
        {   /* the planche's card, near 1 px for 1 px, sat on the foot of this panel */
            int32_t gh = (int32_t)graph_h(), s = 512, room = gh - 18;
            if (room < 118) s = 512 * room / 118;
            vx_kit(t, 120 * 16, (gh - 6) * 16 - ((VK_Y(206) * s) >> 8), s, -1, lane);
        }
    l = lane < 0 ? vxs.last & 7u : (uint32_t)lane & 7u;
    (void)l;                                            /* (the label over the right: the slogan, not the sound's name) */
    cv_text(8, 6 - AF_X_CAP_Y, &AF_X, "KIT", T_THEME);
    cv_text_r(232, 6 - AF_X_CAP_Y, &AF_X, "FREE PALESTINE", lane < 0 ? T_TEXT : T_THEME, T_SURF);
}
/* (BRUSSELS) the Flemish lion, EDIT > VOWEL of a MANNEKEN (ANALOG) track. Fine golden-yellow contour from
 * docs/creatures/creatures.js, gently breathing and waving its tail with the sound. */
static void vx_lion_page(const track_t *t)
{
    static const uint8_t L0[] = {
        100,101, 103,104, 106,102, 107,105, 107,108, 101,109, 101,112, 108,117, 108,120, 120,117, 120,120, 122,122,
        134,120, 139,116, 133,119, 125,120, 125,116, 127,115, 132,117, 134,113, 129,111, 128,114, 124,106, 135,102,
        142,102, 144,104, 150,103, 147,111, 151,107, 152,108, 154,107, 154,109, 150,114, 151,123, 150,124, 147,120,
        147,129, 144,126, 143,128, 146,134, 145,138, 143,140, 143,136, 139,139, 136,138, 139,134, 139,131, 131,135,
        133,128, 130,131, 127,139, 125,136, 123,140, 116,142, 115,140, 116,137, 109,138, 106,141, 106,143, 108,144,
        106,145, 102,143, 103,139, 99,139, 99,136, 98,139, 92,141, 100,140, 95,149, 93,144, 89,148, 88,147, 88,145,
        86,148, 84,147, 84,150, 82,151, 80,144, 79,146, 78,143, 76,145, 72,144, 74,139, 73,138, 70,138, 73,134,
        76,135, 77,134, 75,133, 77,131, 78,132, 78,134, 79,131, 80,131, 80,136, 82,138, 88,138, 94,133, 96,136,
        97,134, 96,134, 97,131, 103,130, 104,126, 108,129, 112,128, 114,124, 124,124, 113,123, 112,127, 110,128,
        107,128, 106,125, 105,125, 102,129, 98,129, 93,133, 87,132, 86,129, 87,124, 90,124, 91,127, 89,128, 92,130,
        95,128, 99,122, 97,121, 95,125, 93,122, 91,123, 90,121, 92,119, 92,116, 94,116, 93,113, 88,114, 88,110,
        91,109, 90,106, 92,104, 91,103, 96,104, 95,107, 96,106, 98,107, 96,104
    };
    static const uint8_t L1[] = {
        158,101, 162,102, 164,104, 163,108, 165,108, 164,111, 166,111, 166,112, 162,117, 160,116, 159,111, 161,106,
        158,104, 158,107, 155,106
    };
    static const uint8_t L2[] = {
        119,108, 121,108, 122,111, 119,109, 117,111, 118,114, 127,114, 118,115, 115,114, 115,111
    };
    static const uint8_t L3[] = {
        156,110, 160,112, 159,117, 160,118, 160,120, 161,120, 157,124, 158,126, 167,116, 166,122, 168,126, 164,129,
        166,126, 164,125, 157,131, 155,142, 158,150, 162,152, 164,149, 162,149, 161,150, 160,148, 163,139, 166,136,
        169,138, 167,142, 167,138, 164,140, 168,155, 165,157, 161,157, 160,162, 168,161, 170,165, 169,167, 166,168,
        164,166, 166,166, 167,164, 164,163, 155,171, 153,167, 152,169, 150,168, 150,170, 145,166, 144,168, 147,174,
        149,172, 156,176, 162,174, 165,175, 162,178, 166,180, 163,181, 162,183, 165,186, 162,188, 164,190, 164,194,
        164,195, 162,193, 160,198, 156,194, 156,203, 152,202, 150,200, 150,202, 147,200, 144,203, 142,199, 137,200,
        137,199, 133,199, 135,195, 139,194, 137,193, 139,192, 137,190, 138,189, 141,190, 143,189, 144,192, 147,194,
        150,191, 149,188, 150,184, 144,184, 145,182, 140,184, 134,185, 134,189, 129,194, 130,196, 133,195, 132,196,
        127,197, 124,200, 119,202, 124,193, 117,192, 115,190, 116,188, 120,186, 123,185, 120,189, 124,191, 128,186,
        132,185, 129,178, 130,169, 128,170, 128,169, 128,160, 130,158, 128,158, 124,152, 113,146, 112,142, 110,143,
        109,141, 113,139, 114,142, 118,143, 121,141, 126,141, 130,137, 133,137, 137,150, 139,150, 140,152, 146,162,
        151,165, 154,164, 158,158, 152,159, 148,158, 146,153, 146,147, 144,146, 144,149, 142,148, 142,146, 144,143,
        148,143, 150,145, 149,152, 152,151, 154,152, 152,155, 154,156, 156,154, 153,142, 155,130, 152,128, 152,126,
        156,117, 156,115, 154,114, 153,117, 152,114
    };
    static const uint8_t L4[] = {
        124,160, 126,160, 127,164, 123,166, 126,168, 125,169, 120,171, 115,176, 115,178, 117,181, 115,183, 115,187,
        114,189, 111,189, 113,187, 111,185, 109,186, 106,194, 104,191, 100,196, 98,195, 98,192, 96,194, 96,197,
        94,198, 93,196, 92,200, 89,196, 90,192, 85,194, 85,193, 82,193, 79,190, 80,186, 78,185, 83,181, 82,178,
        89,180, 90,187, 96,185, 100,179, 98,176, 102,171, 101,170, 94,174, 93,175, 96,176, 96,177, 92,179, 90,175,
        80,176, 78,173, 80,169, 82,168, 82,172, 86,172, 96,169, 89,169, 91,168, 90,166, 92,164, 95,164, 94,166,
        95,167, 104,167, 112,160, 117,161
    };
    static const uint8_t *const POLY[5] = {L0, L1, L2, L3, L4};
    static const uint16_t PLEN[5] = {146, 15, 10, 161, 67};
    int32_t en = vx_level(t, 3, -1), gh = (int32_t)graph_h(), S = (gh - 22) * 256 / 104;
    int32_t anim = !(ui_prefs & PREF_ANIM_OFF), live = anim && en > 8;
    int32_t bob = live ? (og_sin(fm1_ms * 36u) * (16 + (en >> 4)) >> 15) : 0;
    int32_t sw = live ? (og_sin(fm1_ms * 30u) * (24 + (en >> 3)) >> 15) : 0;
    int32_t cy = gh / 2, k, i;
    uint16_t col = vx_col(222 + (en * 28 >> 8));
    for (k = 0; k < 5; k++) {
        const uint8_t *P = POLY[k];
        int32_t n = PLEN[k];
        for (i = 0; i < n; i++) {
            int32_t j = (i + 1) % n;
            int32_t x0 = P[2 * i], y0 = P[2 * i + 1], x1 = P[2 * j], y1 = P[2 * j + 1];
            int32_t X0 = 120 * 16 + ((x0 - 120) * S >> 4) + (x0 > 150 ? sw * (x0 - 150) / 30 : 0);
            int32_t Y0 = cy * 16 + ((y0 - 152) * S >> 4) + bob;
            int32_t X1 = 120 * 16 + ((x1 - 120) * S >> 4) + (x1 > 150 ? sw * (x1 - 150) / 30 : 0);
            int32_t Y1 = cy * 16 + ((y1 - 152) * S >> 4) + bob;
            og_line_a(X0, Y0, X1, Y1, col, 256);
        }
    }
}
