/* SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 Piknik Elektronik
 * BRUSSELS 0.2: the beings as Brussels characters, drawn in line (the fanfare's, docs/creatures): the cone of fries,
 * Manneken Pis, an Atomium, a waffle, a chou, the mussels, a praline, the sauce bottle. Drawn where JIANT's isotherm beings were (being_lines, HOME's
 * eco_being, the DRUM-X colony, the save slots), moved by the same swim, level and hits (ui_being.c, ui_lzhome.c). The
 * drawing itself is vx_being (ui_engvis.c: the same lines as the kit's). Included by ui_being.c */
enum { BX_FRIES, BX_MANNEKEN, BX_ATOMIUM, BX_WAFFLE, BX_CHOU, BX_MUSSEL, BX_PRALINE, BX_SAUCE, BX_N };
enum { BXF_HIT = 1, BXF_SLEEP = 2, BXF_TINY = 4 };      /* the shout on a hit; asleep (muted: dim); half size (10 px tall) */
#define BX_W 16                                          /* the box a being keeps to, in px at scale 1 (the old cells') */
#define BX_H 20

/* the character an isotherm genome stood for: by engine (ui_being.c EGEN's species), DRUM-X's beings the fries */
static uint32_t bx_of_genome(const void *g);
static void vx_being(uint32_t who, int32_t cx, int32_t cy, int32_t r, int32_t en, uint32_t t, uint32_t fl);
/* character who at (cx, cy) Q4, the size of a being of radius r Q4 (BXF_TINY: 10 px tall, r unused), at level en
 * 0..256 and time t: it hops with the sound, squashes and shouts on a hit */
static void brux_draw(uint32_t who, int32_t cx, int32_t cy, int32_t r, int32_t en, uint32_t t, uint32_t fl)
{
    vx_being(who % BX_N, cx, cy, r, en, t, fl);
}
