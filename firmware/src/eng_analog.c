/* SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 Leo Kuroshita (@kurogedelic), Hügelton Instruments
 * Copyright (C) 2026 Juan Ignacio Antonucci (Juan Jiant) */
/* ANALOG: two band-limited oscillators (saw / square / tri / sine / PWM),
 * noise, drive and a trapezoidal low-pass. (JIANT 0.4, TRIO folded in: TRIO's sounds play here, project.c) three more
 * WAVEs in a loop of their own (analog_ext): SYNC (osc 2's saw restarted by osc 1, DTN its ratio 1 .. 8.9: the sync
 * lead's sweep), RING (osc 1's saw times osc 2's sine at that ratio: bells, metal), SAW3 (three saws, osc 2 DTN cents up,
 * osc 3 down: TRIO's fat stack). MIX: osc 1 against the other(s) as before. INT (0.4, once KTR): osc 2 INT semitones
 * from osc 1, as a classic two-oscillator synth (fifths, octaves; SAW3: its osc 2). */
static const char *const N_ANALOG_WAVE[] = {"SAW", "SQR", "TRI", "SIN", "PWM", "SYNC", "RING", "SAW3"};
enum { AW_SYNC = 5, AW_RING, AW_SAW3 };
/* (JIANT 0.5) FILTER TYPE (P_FTYPE): LP as before (analog_render's loop); HP BP from the same state-variable filter
 * (analog_ext's loop, every WAVE); COMB the filter open, the part's sum through a feedback comb tuned to the newest
 * note and CUT (+-32 semitones about it), RES its feedback (voice.c track_comb) */
enum { FT_LP, FT_HP, FT_BP, FT_COMB, FT_MS20 };   /* (BRUSSELS 0.1: FT_MS20 the Korg-35, dsp.c k35_lp; its third
                                                   * state in *hz, the caller's spare slot) */
static inline int32_t svf_mode(const tsvf_t *c, const k35_t *kc, int32_t in, int32_t *ic1, int32_t *ic2, int32_t *hz,
                               uint32_t ft, int32_t k)
{
    int32_t v3, v1, v2;
    if (ft == FT_MS20)
        return k35_lp(kc, in, ic1, ic2, hz);
    v3 = in - *ic2;
    v1 = (c->a1 * *ic1 + c->a2 * v3) >> 13;
    v2 = *ic2 + ((c->a2 * *ic1 + c->a3 * v3) >> 13);
    *ic1 = clamp(2 * v1 - *ic1, -150000, 150000);
    *ic2 = clamp(2 * v2 - *ic2, -150000, 150000);
    return ft == FT_HP ? in - v2 - ((k * v1) >> 12) : ft == FT_BP ? (k * v1) >> 12 : ft == FT_COMB ? in : v2;
}

static void analog_note_on(track_t *t, voice_t *v)
{
    (void)t;
    v->ph[1] = v->ph[0] + 0x40000000u;
    v->s[0] = v->s[1] = v->s[3] = 0;                  /* filter (s[3]: the Korg-35's high-pass state) */
    v->s[4] = v->s[5] = 0;                            /* (BRUSSELS 0.2) the body's split (analog_ext) */
    v->s[6] = v->s[7] = v->s[8] = v->s[9] = 0;        /* (BRUSSELS) the VOWEL's two band-passes */
    if (!v->s[2])
        v->s[2] = 0x1234567 + (int32_t)v->age;        /* noise state */
}

/* (0.6.45) MORPH: the plain WAVEs (SAW SQR TRI SIN) on towards the next ones: the place, Q8 from WAVE (0..1536), the
 * knob and the SHAPE modulation (LFO / ENV -> SHAPE) together; -1: none (the WAVE as it is). (0.6.81) Round in a
 * loop: SIN on towards SAW (it did nothing from SIN) */
static int32_t an_mpos(const int16_t *p, int32_t mq8, const vmod_t *m)   /* (mq8: the morph Q8, track_t.mo_cur) */
{
    int32_t mo;
    if (p[P_E0] >= 4 || (!mq8 && m->shape == (64 << 8)))
        return -1;
    mo = clamp(mq8 + m->shape - (64 << 8), 0, 127 << 8);
    return clamp(p[P_E0] * 256 + mo * 768 / (127 << 8), 0, 1536);
}
/* (0.6.81) MORPH on PWM SYNC RING SAW3 (and the SHAPE modulation on the last three, which had none): how far, Q8
 * 0..127 << 8, the knob and SHAPE together */
static int32_t an_mext(int32_t mq8, const vmod_t *m)
{
    return clamp(mq8 + m->shape - (64 << 8), 0, 127 << 8);
}
static inline int32_t an_osc(uint32_t w, uint32_t ph, uint32_t inc)
{
    w &= 3u;
    return w == 1u ? osc_pulse(ph, inc, 0x80000000u) : w == 2u ? osc_tri(ph) : w == 3u ? osc_sine(ph) : osc_saw(ph, inc);
}
static inline int32_t an_morph(int32_t pos, uint32_t ph, uint32_t inc)
{
    int32_t a = an_osc((uint32_t)pos >> 8, ph, inc), f = pos & 255;
    return f ? a + (((an_osc(((uint32_t)pos >> 8) + 1u, ph, inc) - a) * f) >> 8) : a;
}

/* (BRUSSELS) HAUT (P_AHAUT), on the MS20 type only. 0 sends the whole signal through the Korg-35, as the
 * type alone did. Above 0, a low-pass at that cutoff (CUT's 30 Hz .. 16 kHz curve) is the grave: it is added
 * back after the Korg-35, which hears only what the low-pass removed. lo1/lo2 are the voice's s[4]/s[5]. */
/* (BRUSSELS) VOWEL: the mouth between HAUT and the MS-20 (MS20 type only). Two band-passes at the formants of the
 * vowels A E I O U (Hz, Peterson & Barney, men; to tune by ear), in parallel on the MS-20's input, mixed in by the
 * knob. The knob 1..127 walks A -> E -> I -> O -> U. 0 = closed: no stage at all, no state touched, the sound is
 * bit-identical (an_ms20 gets a NULL vow_t). Each band-pass has two states: s[6..9] of the voice. */
static const uint16_t VOW_F1[5] = {700, 530, 270, 570, 300};
static const uint16_t VOW_F2[5] = {1100, 1840, 2290, 840, 870};
#define VOW_Q 900                                  /* damping, Q12 (about 4.5): a vowel's formant, not a whistle */
typedef struct { tsvf_t f[2]; int32_t mix; } vow_t;
static void vowel_coef(vow_t *w, int32_t v)       /* v 1..127 (the caller has checked it is open) */
{
    int32_t q = (v - 1) * 1024 / 126, seg = q >> 8, fr, h1, h2, i1 = 0, i2 = 0;
    if (seg > 3)
        seg = 3;
    fr = q - seg * 256;                            /* 0..256 across the segment */
    h1 = VOW_F1[seg] + ((int32_t)(VOW_F1[seg + 1] - VOW_F1[seg]) * fr >> 8);
    h2 = VOW_F2[seg] + ((int32_t)(VOW_F2[seg + 1] - VOW_F2[seg]) * fr >> 8);
    while (i1 < 127 && CUTOFF_HZ[i1] < h1)        /* the cutoff index of each formant (CUT's own Hz table) */
        i1++;
    while (i2 < 127 && CUTOFF_HZ[i2] < h2)
        i2++;
    tsvf_coef_k(&w->f[0], i1 << 8, VOW_Q);
    tsvf_coef_k(&w->f[1], i2 << 8, VOW_Q);
    w->mix = v;
}
static inline int32_t vow_bp(const tsvf_t *c, int32_t in, int32_t *ic1, int32_t *ic2)
{
    int32_t v3 = in - *ic2;
    int32_t v1 = (c->a1 * *ic1 + c->a2 * v3) >> 13;
    int32_t v2 = *ic2 + ((c->a2 * *ic1 + c->a3 * v3) >> 13);
    *ic1 = clamp(2 * v1 - *ic1, -150000, 150000);
    *ic2 = clamp(2 * v2 - *ic2, -150000, 150000);
    return (VOW_Q * v1) >> 12;                     /* the band-pass, unity at its formant (as svf_mode's FT_BP) */
}
extern int32_t mouth_now(const int16_t *p, int32_t knob);   /* (mod.c) PAS -> VOW: the dice's vowel, else the knob */
static inline int32_t an_vowel(const vow_t *w, int32_t x, int32_t *vs)
{
    int32_t b = (vow_bp(&w->f[0], x, &vs[0], &vs[1]) + vow_bp(&w->f[1], x, &vs[2], &vs[3])) >> 1;
    return x + (((b - x) * w->mix) >> 7);          /* the knob: 0 dry (never reached: closed is NULL), 127 the vowel */
}
static inline int32_t an_ms20(int32_t x, int32_t haut, tsvf_t *flo, int32_t *lo1, int32_t *lo2, const tsvf_t *flt,
                              const k35_t *k35, int32_t *ic1, int32_t *ic2, int32_t *hz, uint32_t ft, int32_t kd,
                              const vow_t *vw, int32_t *vs)
{
    if (ft == FT_MS20 && haut > 0) {
        int32_t body = tsvf_lp(flo, x, lo1, lo2), hi = x - body;
        if (vw)                                    /* after HAUT: the grave is not touched by the mouth */
            hi = an_vowel(vw, hi, vs);
        return k35_lp(k35, hi, ic1, ic2, hz) + body;
    }
    if (vw)
        x = an_vowel(vw, x, vs);
    return svf_mode(flt, k35, x, ic1, ic2, hz, ft, kd);
}

/* (0.6.45) MORPH: the plain WAVEs blended toward the next (an_mpos), osc 2 as analog_render's, the same noise, drive
 * and filter as analog_ext (a loop of its own: the others' as they were) */
static __attribute__((noinline)) void analog_morph(track_t *t, voice_t *v, int32_t *out, uint32_t n, const vmod_t *m,
                                                   int32_t mpos0, int32_t mpos1)   /* (0.6.50: ramped mpos0 -> mpos1) */
{
    const int16_t *p = t->p;
    uint32_t i, inc1 = m->inc, ph0 = v->ph[0], ph1 = v->ph[1], ft = (uint32_t)clamp(p[P_FTYPE], 0, FT_MS20);
    uint32_t inc2 = p[P_E1] || p[P_E7] ? cents_inc(m->pitch16 + p[P_E7] * 16, p[P_E1], 0) : inc1;
    int32_t m2 = p[P_E2] * 258, m1 = 32767 - m2, nz = p[P_E3] * 200, drv = p[P_E6], kd = 8192 - p[P_E5] * 7600 / 127;
    int32_t drive = 32768 + drv * 512, cut = (p[P_E4] << 8) + m->cutoff + ((v->pitch16 - 60 * 16) << 2);
    int32_t haut = p[P_AHAUT], sub = p[P_ASUB] * 258;
    int32_t ic1 = v->s[0], ic2 = v->s[1], nst = v->s[2], hz = v->s[3], lo1 = v->s[4], lo2 = v->s[5];
    tsvf_t flt, flo;
    k35_t k35;
    tsvf_coef(&flt, cut, p[P_E5]);
    if (ft == FT_MS20) {
        k35_coef(&k35, cut, p[P_E5], drv);
        if (haut > 0)
            tsvf_coef(&flo, haut << 8, 0);
    }
    int32_t dq = ((mpos1 - mpos0) << 8) / (int32_t)(n ? n : 1u);
    vow_t vw;                                      /* (BRUSSELS) VOWEL: open on the MS-20 type only */
    const vow_t *vp = 0;
    int32_t vs[4] = {v->s[6], v->s[7], v->s[8], v->s[9]};
    {
        int32_t vow = mouth_now(p, p[P_VOWEL]);
        if (ft == FT_MS20 && vow > 0) {
            vowel_coef(&vw, vow);
            vp = &vw;
        }
    }
    for (i = 0; i < n; i++) {
        int32_t mpos = mpos0 + ((dq * (int32_t)i) >> 8);
        int32_t s = mulq15(an_morph(mpos, ph0, inc1), m1) + mulq15(an_morph(mpos, ph1, inc2), m2);
        ph0 += inc1;
        ph1 += inc2;
        if (nz)
            s += mulq15((int32_t)(noise32(&nst) >> 17) - 16384, nz);
        if (drv)
            s = softclip(((s >> 2) * (drive >> 2)) >> 11);
        {
            int32_t y = an_ms20(s >> 1, haut, &flo, &lo1, &lo2, &flt, &k35, &ic1, &ic2, &hz, ft, kd, vp, vs);
            if (sub)
                y += mulq15(osc_sine(ph0) >> 1, sub);
            out[i] += voice_amp(soft_knee(y, 16000) << 1, m, i) << 1;
        }
    }
    v->ph[0] = ph0;
    v->ph[1] = ph1;
    v->s[0] = ic1;
    v->s[1] = ic2;
    v->s[2] = nst;
    v->s[3] = hz;
    v->s[4] = lo1;
    v->s[5] = lo2;
    v->s[6] = vs[0];
    v->s[7] = vs[1];
    v->s[8] = vs[2];
    v->s[9] = vs[3];
}

/* PWM's width: SHAPE as before, (0.6.81) MORPH narrowing it further (toward ~4 %) */
static uint32_t an_pw(const track_t *t, const vmod_t *m)
{
    int32_t q = (m->shape - (64 << 8)) + (t->p[P_E0] == 4 ? t->mo_cur * 2 : 0);
    return 0x80000000u + (uint32_t)(clamp(q, -16384, 65000) << 15);
}

/* SYNC RING SAW3 (see the top): a render of their own (the other WAVEs' loop untouched), the same noise, drive and
 * filter */
static __attribute__((noinline)) void analog_ext(track_t *t, voice_t *v, int32_t *out, uint32_t n, const vmod_t *m)
{
    const int16_t *p = t->p;
    uint32_t wave = (uint32_t)p[P_E0], i, inc1 = m->inc, ph0 = v->ph[0], ph1 = v->ph[1], ph2 = v->ph[2], r, inc2, inc3;
    uint32_t ft = (uint32_t)clamp(p[P_FTYPE], 0, FT_MS20), pw = an_pw(t, m);
    int32_t y, lo1, lo2, haut, sub = p[P_ASUB] * 258;
    tsvf_t flo;
    int32_t mx = wave >= AW_SYNC ? an_mext(t->mo_cur, m) : 0;   /* (0.6.81) MORPH, SHAPE on SYNC RING SAW3 */
    int32_t det = p[P_E1] + (wave == AW_SAW3 ? mx * 40 / (127 << 8) : 0), m2 = p[P_E2] * 258, m1 = 32767 - m2, nz = p[P_E3] * 200, drv = p[P_E6], kd = 8192 - p[P_E5] * 7600 / 127;
    int32_t drive = 32768 + drv * 512, cut = (p[P_E4] << 8) + m->cutoff + ((v->pitch16 - 60 * 16) << 2);
    int32_t ic1 = v->s[0], ic2 = v->s[1], nst = v->s[2], hz = v->s[3];
    tsvf_t flt;
    k35_t k35;
    tsvf_coef(&flt, cut, p[P_E5]);
    haut = p[P_AHAUT];
    if (ft == FT_MS20) {
        k35_coef(&k35, cut, p[P_E5], drv);
        if (haut > 0)
            tsvf_coef(&flo, haut << 8, 0);
    }
    lo1 = v->s[4];
    lo2 = v->s[5];
    r = 16u + (uint32_t)det + (wave != AW_SAW3 ? (uint32_t)(mx * 64 / (127 << 8)) : 0u);   /* SYNC / RING: osc 2 at r / 16
                                                         * of osc 1 (1 .. 8.9; 0.6.81: MORPH up to 4 more) */
    inc2 = wave == AW_SAW3 ? cents_inc(m->pitch16 + p[P_E7] * 16, det, 0) : (inc1 >> 4) * r;   /* (SAW3: osc 2 at INT) */
    if (wave != AW_SAW3 && p[P_E7]) {                   /* (JIANT 0.5) SYNC / RING: INT moves osc 2 too, semitones */
        inc2 = (cents_inc(m->pitch16 + p[P_E7] * 16, 0, 0) >> 4) * r;
        r = inc1 >> 8 ? (inc2 / (inc1 >> 8)) >> 4 : r; /* (its ratio to osc 1, x16: the restart below) */
        if (r < 1u) r = 1u;
    }
    inc3 = wave == AW_SAW3 ? cents_inc(m->pitch16, -det, 0) : 0u;
    if (wave < AW_SYNC)                                 /* (JIANT 0.5: HP BP COMB) the plain WAVEs: osc 2 as analog_render */
        inc2 = det || p[P_E7] ? cents_inc(m->pitch16 + p[P_E7] * 16, det, 0) : inc1;
    vow_t vw;                                      /* (BRUSSELS) VOWEL: open on the MS-20 type only */
    const vow_t *vp = 0;
    int32_t vs[4] = {v->s[6], v->s[7], v->s[8], v->s[9]};
    {
        int32_t vow = mouth_now(p, p[P_VOWEL]);
        if (ft == FT_MS20 && vow > 0) {
            vowel_coef(&vw, vow);
            vp = &vw;
        }
    }
    for (i = 0; i < n; i++) {
        int32_t a = osc_saw(ph0, inc1), b, s;
        if (wave < AW_SYNC) {
            switch (wave) {
            case 1: a = osc_pulse(ph0, inc1, 0x80000000u); b = osc_pulse(ph1, inc2, 0x80000000u); break;
            case 2: a = osc_tri(ph0); b = osc_tri(ph1); break;
            case 3: a = osc_sine(ph0); b = osc_sine(ph1); break;
            case 4: a = osc_pulse(ph0, inc1, pw); b = osc_pulse(ph1, inc2, pw); break;
            default: b = osc_saw(ph1, inc2); break;
            }
        } else if (wave == AW_SAW3) {
            b = (osc_saw(ph1, inc2) + osc_saw(ph2, inc3)) >> 1;
            ph2 += inc3;
        } else if (wave == AW_SYNC) {
            b = osc_saw(ph1, inc2);
        } else {
            b = ((a >> 1) * (osc_sine(ph1) >> 1)) >> 13;   /* RING (32 bits) */
        }
        ph1 += inc2;
        if (wave == AW_SYNC && ph0 + inc1 < ph0)        /* osc 1 starts a cycle: osc 2 with it */
            ph1 = ((ph0 + inc1) >> 4) * r;              /* (the new cycle's fraction, at osc 2's rate) */
        ph0 += inc1;
        s = mulq15(a, m1) + mulq15(b, m2);
        if (nz)
            s += mulq15((int32_t)(noise32(&nst) >> 17) - 16384, nz);
        if (drv)
            s = softclip(((s >> 2) * (drive >> 2)) >> 11);
        y = an_ms20(s >> 1, haut, &flo, &lo1, &lo2, &flt, &k35, &ic1, &ic2, &hz, ft, kd, vp, vs);
        if (sub)
            y += mulq15(osc_sine(ph0) >> 1, sub);
        out[i] += voice_amp(soft_knee(y, 16000) << 1, m, i) << 1;
    }
    v->ph[0] = ph0;
    v->ph[1] = ph1;
    v->ph[2] = ph2;
    v->s[6] = vs[0];
    v->s[7] = vs[1];
    v->s[8] = vs[2];
    v->s[9] = vs[3];
    v->s[4] = lo1;
    v->s[5] = lo2;
    v->s[0] = ic1;
    v->s[1] = ic2;
    v->s[2] = nst;
    v->s[3] = hz;
}

static __attribute__((noinline)) void analog_render(track_t *t, voice_t *v, int32_t *out, uint32_t n, const vmod_t *m)
{
    const int16_t *p = t->p;
    uint32_t wave = (uint32_t)p[P_E0], i;
    int32_t det = p[P_E1], mix = p[P_E2], noise = p[P_E3];
    int32_t cut = (p[P_E4] << 8) + m->cutoff + ((v->pitch16 - 60 * 16) << 2);
    tsvf_t flt;
    int32_t drive = 32768 + p[P_E6] * 512;                       /* 1x .. 3x */
    uint32_t inc1 = m->inc;
    uint32_t inc2 = det || p[P_E7] ? cents_inc(m->pitch16 + p[P_E7] * 16, det, 0) : inc1;   /* (JIANT 0.4) osc 2: INT
                                                         * semitones away, DTN cents */
    uint32_t pw = an_pw(t, m);
    int32_t m2 = mix * 258, m1 = 32767 - m2;                    /* osc mix Q15 */
    int32_t nz = noise * 200, drv = p[P_E6];
    uint32_t ph0 = v->ph[0], ph1 = v->ph[1];                  /* state in locals: out[] may alias v->s[] */
    int32_t ic1 = v->s[0], ic2 = v->s[1], nst = v->s[2];
    tsvf_coef(&flt, cut, p[P_E5]);
    for (i = 0; i < n; i++) {
        int32_t a, b, s;
        switch (wave) {
        case 1:
            a = osc_pulse(ph0, inc1, 0x80000000u);
            b = osc_pulse(ph1, inc2, 0x80000000u);
            break;
        case 2:
            a = osc_tri(ph0);
            b = osc_tri(ph1);
            break;
        case 3:
            a = osc_sine(ph0);
            b = osc_sine(ph1);
            break;
        case 4:
            a = osc_pulse(ph0, inc1, pw);
            b = osc_pulse(ph1, inc2, pw);
            break;
        default:
            a = osc_saw(ph0, inc1);
            b = osc_saw(ph1, inc2);
            break;
        }
        ph0 += inc1;
        ph1 += inc2;
        s = mulq15(a, m1) + mulq15(b, m2);
        if (nz)
            s += mulq15((int32_t)(noise32(&nst) >> 17) - 16384, nz);
        if (drv)
            s = softclip(((s >> 2) * (drive >> 2)) >> 11);   /* pre-shifts: drive is up to 3x, no overflow */
        {   /* filter, linear up to half scale, then a soft knee (only resonance peaks saturate); soft_knee()
             * written out: the host compiler makes this loop 1 % slower with the call */
            int32_t y = tsvf_lp(&flt, s >> 1, &ic1, &ic2), a = y < 0 ? -y : y;
            if (a > 16000) {
                a = 16000 + (softclip((a - 16000) * 2) >> 1);
                y = y < 0 ? -a : a;
            }
            s = y << 1;
        }
        out[i] += voice_amp(s, m, i) << 1;
    }
    v->ph[0] = ph0;
    v->ph[1] = ph1;
    v->s[0] = ic1;
    v->s[1] = ic2;
    v->s[2] = nst;
}

/* the engine's render: SYNC RING SAW3 (0.4) and the FILTER TYPEs HP BP COMB (0.5) in analog_ext, the rest (LP) here
 * (a dispatch of its own: analog_render's loop as before) */
static void analog_dispatch(track_t *t, voice_t *v, int32_t *out, uint32_t n, const vmod_t *m)
{
    int32_t m1 = an_mpos(t->p, t->mo_cur, m), m0 = an_mpos(t->p, t->mo_prev, m);
    if (m1 >= 0 || m0 >= 0) {                         /* (0.6.45) MORPH; (0.6.50) ramped from the last block's */
        int32_t base = (int32_t)t->p[P_E0] * 256;      /* (as it leaves or comes: the plain WAVE) */
        analog_morph(t, v, out, n, m, m0 < 0 ? base : m0, m1 < 0 ? base : m1);
    }
    else if ((uint32_t)t->p[P_E0] >= AW_SYNC || t->p[P_FTYPE] || t->p[P_ASUB])
        analog_ext(t, v, out, n, m);
    else
        analog_render(t, v, out, n, m);
}

static const preset_t ANALOG_PRESETS[] = {
    {"GAUFRE", {0, 12, 64, 0, 90, 30, 10, 12}, {4, 70, 100, 50}, 20, 1 | TAG(LEAD), FX(0, 10, 45, 30), PAT(4)},
    {"SOFT PAD", {0, 20, 64, 4, 60, 10, 0, 0}, {80, 90, 110, 95}, 10, TAG(PAD), FX(0, 60, 20, 70), PAT(5)},
    {"SQR BASS", {1, 0, 0, 0, 50, 70, 40, 0}, {0, 60, 40, 30}, 30, 1 | TAG(BASS), FX(5, 0, 10, 10), PAT(2)},
    {"PWM STR", {4, 8, 40, 0, 75, 20, 0, 0}, {60, 80, 110, 85}, 8, TAG(PAD), FX(0, 50, 20, 60), PAT(5)},
    {"FRITURE", {0, 0, 0, 0, 50, 100, 25, 0}, {0, 55, 20, 30}, 48, 1 | TAG(BASS), FX(20, 0, 45, 15), PAT(1)},
    {"SINE KEY", {3, 6, 50, 0, 127, 0, 0, 0}, {2, 80, 30, 70}, 0, TAG(KEYS), FX(0, 30, 25, 40), PAT(6)},
    {"RAVE", {4, 30, 64, 0, 85, 20, 30, 7}, {20, 80, 110, 60}, 10, 1 | TAG(LEAD), FX(30, 40, 30, 30), PAT(13)},
    {"SUB BASS", {3, 0, 0, 0, 40, 0, 20, 0}, {0, 60, 100, 20}, 0, 1 | TAG(BASS), FX(0, 0, 0, 10), PAT(8)},
    {"PLUCK", {0, 8, 50, 0, 30, 40, 0, 0}, {0, 88, 0, 60}, 55, TAG(PLUCK), FX(0, 20, 50, 30), PAT(3)},
    {"BRASS", {0, 10, 64, 0, 45, 20, 10, 0}, {35, 70, 90, 45}, 40, TAG(LEAD), FX(0, 20, 20, 40), PAT(6)},
    {"WIND", {0, 0, 0, 90, 30, 90, 0, 0}, {60, 90, 60, 80}, 50, TAG(FX), FX(0, 30, 30, 70), PAT(5)},
    {"STRINGS", {0, 25, 64, 0, 70, 10, 0, 0}, {70, 90, 115, 90}, 5, TAG(PAD), FX(0, 60, 20, 70), PAT(5)},
    /* (JIANT 0.4: TRIO's, folded in) SYNC (ratio 1 + 11/16), RING (1 + 18/16), SAW3 (9 ct a side) */
    {"SYNC LEAD", {5, 11, 90, 0, 82, 30, 10, 0}, {2, 70, 100, 40}, 18, 1 | TAG(LEAD), FX(0, 10, 40, 25), PAT(4)},
    {"RING BELL", {6, 18, 100, 0, 110, 20, 0, 0}, {0, 92, 0, 80}, 0, TAG(BELL), FX(0, 20, 35, 55), PAT(7)},
    {"BALLEKES", {7, 9, 64, 0, 62, 45, 20, -12}, {0, 62, 60, 25}, 40, 1 | TAG(BASS), FX(10, 0, 10, 10), PAT(2)},
    /* (0.7.0) a palette for the song generator and the player: pads, a reese, a hoover, a dub stab, a cello, an
     * arp pluck, a lead, keys, an organ */
    {"JUNO PAD", {4, 14, 64, 2, 58, 12, 0, 0}, {45, 90, 115, 90}, 8, TAG(PAD), FX(0, 70, 25, 60), PAT(5)},
    {"REESE", {7, 22, 64, 0, 44, 20, 25, 0}, {2, 80, 110, 40}, 0, 1 | TAG(BASS), FX(10, 20, 10, 10), PAT(2)},
    {"HOOVER", {7, 40, 64, 0, 92, 15, 35, 12}, {10, 80, 110, 55}, 12, 1 | TAG(LEAD), FX(25, 50, 35, 35), PAT(13)},
    {"DUB STAB", {1, 6, 64, 0, 52, 55, 10, 7}, {0, 50, 20, 45}, 50, TAG(KEYS), FX(0, 15, 60, 55), PAT(6)},
    {"CELLO", {0, 4, 40, 2, 48, 18, 15, -12}, {30, 85, 110, 70}, 10, TAG(PAD), FX(5, 30, 20, 55), PAT(5)},
    {"ARP PLUCK", {1, 10, 50, 0, 44, 50, 0, 12}, {0, 62, 0, 40}, 60, TAG(PLUCK), FX(0, 25, 55, 35), PAT(3)},
    {"MOOG LEAD", {2, 6, 40, 0, 72, 40, 40, -12}, {6, 75, 100, 45}, 25, 1 | TAG(LEAD), FX(10, 0, 35, 25), PAT(4)},
    {"DARK PAD", {0, 18, 64, 6, 38, 30, 10, -12}, {70, 95, 120, 100}, 12, TAG(PAD), FX(0, 55, 30, 75), PAT(5)},
    {"GLASS KEYS", {3, 8, 60, 0, 120, 0, 0, 19}, {0, 82, 20, 65}, 0, TAG(KEYS), FX(0, 35, 35, 50), PAT(6)},
    {"ORGAN SQR", {1, 4, 64, 0, 100, 0, 5, 12}, {0, 64, 127, 30}, 0, TAG(KEYS), FX(0, 35, 10, 35), PAT(6)},
    /* (BRUSSELS, last: the numbers stay) the MS-20 bass heard on 18 Nov: FAT BASS's oscillator, CUT 196 Hz, RES 60 %,
     * DRIVE 60 %, ENV DEST FILTER +70 %, CHORUS ~15 %. TYPE MS20 by bit 5; HAUT OFF (0). One slow LFO -> CUT
     * (BRU_EXT, amount 18, rate 48), kept as heard 22 Nov. */
    {"FRITES", {7, 9, 64, 0, 38, 76, 76, -12}, {0, 62, 60, 25}, 45, 1 | TAG(BASS) | MS20_BIT, FX(10, 19, 0, 0), PAT(2)},
    /* (BRUSSELS) ZUKUNFT, chosen by ear: its SIN base (the old SAW3 base cracked at the start). SIN base:
     * the smoothest wave, no edge to crack. Osc 2 an octave down (MIX 76), a little noise, attack 12, the sustain
     * holds it, CUT ~217 Hz, RES ~46 %, DRIVE ~41 %, chorus ~16 %. The filter moves: LFO -> HAUT and LFO -> CUT (BRU_EXT). */
    {"ZUKUNFT", {3, 20, 76, 20, 40, 58, 52, -12}, {12, 60, 40, 50}, 40, 1 | TAG(LEAD) | MS20_BIT, FX(4, 45, 0, 8), PAT(6)},
    /* (BRUSSELS) DIKKENEK, chosen by ear: TRI base (no crack at the start), corps serré, rond clair, quinte éteinte,
     * écho long, tenue long, grave très profond. A short attack, DRIVE ~33 %. TYPE MS20 by bit 5. */
    {"DIKKENEK", {2, 6, 64, 0, 36, 70, 42, 0}, {4, 50, 100, 30}, 45, 1 | TAG(BASS) | MS20_BIT, FX(0, 0, 70, 0), PAT(1)},
    /* (BRUSSELS) STOEMP: a PWM base (the width moves by itself), MS-20, warm. Osc 2 an octave down, a little noise,
     * CUT ~217 Hz, RES ~46 %, DRIVE ~45 %, chorus ~16 %, a soft attack and a sustain that holds. No cable. */
    {"STOEMP", {4, 14, 76, 20, 40, 58, 57, -12}, {12, 60, 40, 50}, 40, TAG(PAD) | MS20_BIT, FX(4, 20, 0, 8), PAT(6)},
    /* (BRUSSELS) dark and strange: a sine, the second oscillator a tritone down, the filter low and round,
     * a slow loop of the LFO on the cutoff (BRU_EXT). No chorus, so it stays dry. */
    {"MOLENBEEK", {3, 12, 76, 0, 32, 46, 70, -6}, {14, 70, 90, 55}, 20, 1 | TAG(BASS) | MS20_BIT, FX(6, 0, 0, 6), PAT(2)},
    /* (BRUSSELS) dark and strange, another way: a triangle, a minor third up so the two oscillators rub,
     * HAUT at 60 Hz moved by a slow LFO (BRU_EXT). */
    {"IXELLES", {2, 10, 76, 20, 36, 58, 55, 3}, {12, 60, 40, 45}, 35, TAG(PAD) | MS20_BIT, FX(4, 20, 0, 10), PAT(6)},
    /* (BRUSSELS) strange and playful: PWM, a fifth up, a livelier LFO loop on the cutoff, a short echo.
     * Still deep (CUT ~322 Hz). */
    {"TROTINETTE", {4, 18, 64, 0, 48, 60, 40, 7}, {8, 40, 48, 30}, 50, TAG(PAD) | MS20_BIT, FX(0, 20, 32, 0), PAT(1)},
    /* (BRUSSELS) warm and playful, the Baudgirl side: a sine, a sixth up, a lot of chorus, and the LFO
     * loops on the chorus itself (BRU_EXT) instead of on the filter. Poly, so a chord is the sound. */
    {"PIKNIK", {3, 16, 76, 20, 44, 58, 48, 9}, {12, 70, 40, 50}, 25, TAG(PAD) | MS20_BIT, FX(0, 45, 20, 15), PAT(5)},
    /* (BRUSSELS) the late echo: a triangle, no second interval, the delay set long so the chord answers after.
     * A small slow loop on the cutoff (BRU_EXT). Poly. */
    {"STIB DELAY", {2, 18, 64, 0, 48, 60, 40, 0}, {8, 40, 48, 30}, 40, TAG(PAD) | MS20_BIT, FX(0, 0, 100, 0), PAT(5)},
    /* (BRUSSELS) dark poly: a sine, the second oscillator one semitone up so it beats, the filter low,
     * a wide slow loop on the cutoff (BRU_EXT). No chorus. */
    /* (BRUSSELS) MADOU, re-baked from the line the user chose on 20 Nov (the capture): gras moyen, rond clair,
     * no souffle, très lent, no chaleur, tenue long, quinte proche, ouverture grande, attaque sèche, mordant fort,
     * largeur serrée, écho court, no espace. The cutoff loop is very slow (BRU_EXT rate 24). */
    {"MADOU", {3, 8, 76, 0, 30, 64, 70, 1}, {4, 70, 90, 55}, 50, TAG(PAD) | MS20_BIT, FX(4, 0, 32, 0), PAT(5)},
    /* (BRUSSELS) the heaviest poly: PWM, an octave down with all the weight on it, the filter very low,
     * HAUT at 60 Hz moved by a slow LFO (BRU_EXT). */
    /* (BRUSSELS) baked from the chosen line (18 Nov, 2:10 PM): the heaviest, PWM, the mix full, a little air, a sharp
     * attack, a strong drive, a major-ish quinte (proche), a long echo, a lot of chorus and reverb. */
    {"HELMET", {4, 18, 100, 20, 28, 46, 70, 1}, {4, 80, 90, 60}, 30, TAG(PAD) | MS20_BIT, FX(8, 45, 70, 60), PAT(5)},
    /* (BRUSSELS) warm poly: a triangle, a major third up, a lot of chorus, and the LFO loops on the loudness
     * (BRU_EXT) so the chord breathes. */
    /* (BRUSSELS) baked from the chosen line (18 Nov, 1:57 PM): a triangle, a wide quinte (10), a lot of air,
     * the filter opened, a medium drive, a tight detune, no echo, a little reverb, the chorus full. */
    {"LOUISE", {2, 8, 76, 50, 44, 58, 50, 10}, {12, 60, 40, 50}, 50, TAG(PAD) | MS20_BIT, FX(0, 45, 0, 20), PAT(5)},
    /* (BRUSSELS) playful poly, baked from the chosen line (18 Nov, 1:55 PM): PWM, a seventh up, the mix full, a lot
     * of air, a slow attack, a long sustain, a strong drive, a lot of chorus and reverb, and the LFO loops on the pan
     * (BRU_EXT) so the chord wanders. */
    {"RIBAUCOURT", {4, 18, 100, 50, 40, 58, 70, 10}, {30, 50, 90, 40}, 30, TAG(PAD) | MS20_BIT, FX(0, 45, 32, 60), PAT(5)},
    /* (BRUSSELS) OSTEND: the sea, baked from the chosen line (18 Nov, 1:43 PM). A triangle, both oscillators
     * on the same note (no fifth, so no ding at the start), a tight detune, a lot of air (noise), a soft attack,
     * a small filter opening, a round resonance, a medium drive, a long echo, no reverb. One slow sine (about
     * six seconds) moves the loudness, the cutoff and the noise together (BRU_EXT): the crest is loud, bright
     * and full of foam, the trough pulls back. Poly. */
    {"OSTEND", {2, 8, 76, 50, 52, 46, 50, 0}, {12, 70, 90, 90}, 30, TAG(PAD) | MS20_BIT, FX(0, 45, 70, 0), PAT(5)},
    /* (BRUSSELS) point 2, the first cable that is the envelope into the lowpass. Same family as FRITES, which
     * stays as heard. CUT sits low, ENV DEST is 0, so only the cable opens the filter. One slot: EG -> BAS,
     * amount 63. The other three stay empty. */
    {"EG BAS", {7, 9, 64, 0, 22, 76, 60, -12}, {0, 90, 20, 40}, 0, 1 | TAG(BASS) | MS20_BIT, FX(0, 0, 0, 0), PAT(2)},
    /* (BRUSSELS) point 3, step dice (HASARD): 1/8 chance per 1/16 step. TRI base (smooth), octave down, MS-20.
     * Cable 1: HASARD -> BAS (amount 42), so the cutoff jumps on the steps the dice opens. */
    {"RANDOM KINDNESS", {2, 8, 68, 6, 42, 66, 54, -12}, {4, 65, 95, 35}, 12, 1 | TAG(BASS) | MS20_BIT, FX(0, 14, 16, 0), PAT(2)},
    /* (BRUSSELS) point 3, la note volee: TRI base, MS-20, warm delay. Cable 1: HASARD -> NOTE (amount 6,
     * <= ~1 semitone), so a note slips slightly off on the steps the dice opens and holds until the next. */
    {"NOTE VOLEE", {2, 10, 64, 8, 54, 48, 40, 0}, {6, 70, 95, 45}, 18, 1 | TAG(LEAD) | MS20_BIT, FX(0, 24, 34, 14), PAT(4)},
    /* (BRUSSELS) Paquet 2, pink noise (Voss-McCartney) -> HAUT: the low weight wanders without vibrato. */
    {"ROSE HAUT", {2, 10, 74, 12, 46, 56, 50, -12}, {10, 70, 96, 50}, 20, TAG(PAD) | MS20_BIT, FX(0, 22, 24, 12), PAT(5)},
    /* (BRUSSELS) Paquet 2, 3-in-8 Euclidean clock S&H (x..x..x.) -> BAS: rhythmic cutoff pulse on MS-20. */
    {"EUCLIDE", {4, 10, 68, 6, 44, 68, 50, -12}, {4, 65, 95, 35}, 14, 1 | TAG(BASS) | MS20_BIT, FX(0, 14, 28, 0), PAT(2)},
    /* (BRUSSELS) Paquet 3, Q30 logistic chaos (r=3.995, 16 455-step period) -> BAS + HAUT on MS-20. */
    {"CHAOS BAS", {7, 10, 64, 0, 40, 72, 56, -12}, {2, 68, 92, 34}, 14, 1 | TAG(BASS) | MS20_BIT, FX(0, 12, 24, 0), PAT(2)},
    /* (BRUSSELS) Paquet 3, clean SUB fundamental bypassing HAUT + VOWEL + resonant MS-20 top layer. */
    {"SOUS SOL", {4, 14, 70, 6, 52, 78, 64, 0}, {3, 72, 98, 36}, 18, 1 | TAG(BASS) | MS20_BIT, FX(0, 18, 22, 0), PAT(2)},
    /* (BRUSSELS) Paquet 4 (§7): EG -> BAS, EG -> PROB (opens HASARD on attack), HASARD -> VOIX, MG -> HAUT. */
    {"PHRASE", {7, 10, 66, 0, 34, 72, 58, -12}, {1, 52, 48, 34}, 0, 1 | TAG(BASS) | MS20_BIT, FX(0, 14, 24, 0), PAT(2)},
};

/* (BRUSSELS) what a preset keeps beyond its numbers: HAUT and the four cables of mod.c (src, dst, amt), by its ANALOG
 * number. A preset not listed here loads HAUT 0 and no cable, as before; every JIANT preset is unlisted. The table is
 * flash: 17 bytes per listed preset. Cable numbers are the enums (MS_LFO 1, MD_HAUT 30), checked in ui.c. */
typedef struct {
    uint8_t pi, haut;
    struct { uint8_t src, dst; int8_t amt; } cab[4];
    uint8_t rate;                /* the LFO's rate (P_LRATE); 0 = not stored, the default */
    uint8_t vowel;               /* (BRUSSELS) P_VOWEL; 0 = closed (not stored), the default */
    uint8_t sub;                 /* (BRUSSELS) P_ASUB; 0 = off (bit-identical), clean sub-sine bypass */
} bru_ext_t;
static const bru_ext_t BRU_EXT[] = {
{25, 0, {{1, 2, 18}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 48},      /* FRITES: heard 22 Nov, one slow loop, LFO -> CUT */
{26, 14, {{1, 30, 8}, {1, 2, 14}, {0, 0, 0}, {0, 0, 0}}, 48},     /* ZUKUNFT: the same two loops, a step stronger */
{27, 0, {{1, 2, 6}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 48},        /* DIKKENEK: one slow loop on the cutoff, small */
{28, 0, {{1, 2, 8}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 48},        /* STOEMP: one slow loop on the cutoff */
{29, 0, {{1, 2, 20}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 48},       /* MOLENBEEK: the wide loop, a step wider */
{30, 14, {{1, 30, 10}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 48},     /* IXELLES: LFO -> HAUT, a step stronger */
{31, 0, {{1, 2, 10}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 78, 33},  /* TROTINETTE (VOWEL E): the livelier loop, a step stronger */
{32, 0, {{1, 7, 12}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 48, 64},  /* PIKNIK (VOWEL I): the chorus loop, a step stronger */
{33, 0, {{1, 2, 8}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 48},        /* STIB DELAY: the small loop, a step stronger */
{34, 0, {{1, 2, 18}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 24},       /* MADOU: the cutoff loop, very slow (très lent, 20 Nov) */
{35, 14, {{1, 30, 12}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 60},     /* HELMET: LFO -> HAUT, the rate baked at 60 */
{36, 0, {{1, 4, 14}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 48, 64},  /* LOUISE (VOWEL I): the loudness loop, a step stronger */
{37, 0, {{1, 5, 20}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 78, 96},  /* RIBAUCOURT (VOWEL O): the pan loop, LFO rate 78 as baked */
{38, 0, {{1, 4, 36}, {1, 2, 28}, {1, 15, 24}, {0, 0, 0}}, 24},   /* OSTEND: one slow wave, LFO -> AMP, CUT, NOISE */
{39, 0, {{2, 2, 63}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 0},      /* EG BAS: EG -> CUT, full amount, three slots empty */
{40, 0, {{15, 2, 42}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 0},     /* HASARD BAS: HASARD -> BAS (cutoff jumps on 1/8 steps) */
{41, 0, {{15, 1, 6}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 0},      /* NOTE VOLEE: HASARD -> NOTE (subtle pitch slip on 1/8 steps) */
{42, 24, {{16, 30, 28}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 0},   /* ROSE HAUT: ROSE (pink) -> HAUT, slow organic drift */
{43, 0, {{17, 2, 44}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 0},     /* EUCLIDE: EUCL (3/8 clock S&H) -> BAS */
{44, 16, {{18, 2, 46}, {18, 30, 20}, {0, 0, 0}, {0, 0, 0}}, 0}, /* CHAOS BAS: CHAOS (Q30 logistic) -> BAS + HAUT */
{45, 44, {{1, 2, 26}, {1, 30, 18}, {0, 0, 0}, {0, 0, 0}}, 42, 33, 92}, /* SOUS SOL: clean SUB 92 bypassing HAUT/VOWEL/MS-20 */
{46, 18, {{2, 2, 54}, {2, 32, 63}, {15, 31, 48}, {1, 30, 22}}, 42, 0, 0}, /* PHRASE (§7): EG->BAS, EG->PROB, HASARD->VOIX, MG->HAUT */
};
static const bru_ext_t *bru_ext_of(uint32_t pi)
{
    uint32_t i;
    for (i = 0; i < NELEM(BRU_EXT); i++)
        if (BRU_EXT[i].pi == pi)
            return &BRU_EXT[i];
    return 0;
}

static const engine_t ENG_ANALOG = {
    .name = "ANALOG",
    .page_title = {"OSC", "FLT"},
    .edit = {
        {"WAVE", F_ENUM, 0, 7, 0, N_ANALOG_WAVE, 0},
        {"DETUNE", F_INT, 0, 127, 10, 0, "ct"},
        {"MIX", F_PCT, 0, 127, 64, 0, 0},
        {"NOISE", F_PCT, 0, 127, 0, 0, 0},
        {"CUT", F_CUTOFF, 0, 127, 90, 0, 0},
        {"RES", F_PCT, 0, 127, 30, 0, 0},
        {"DRIVE", F_PCT, 0, 127, 0, 0, 0},
        {"INT", F_SEMI, -24, 24, 0, 0, 0},             /* (JIANT 0.4: osc 2's interval, was KTR: the filter's key
                                                         * tracking, fixed at its old default, half) */
    },
    .presets = ANALOG_PRESETS,
    .npresets = NELEM(ANALOG_PRESETS),
    .note_on = analog_note_on,
    .render = analog_dispatch,
    .knob = {P_E4, P_E5, P_ATK, P_REL},
    .keep = 0x03,                /* the filter */
};
