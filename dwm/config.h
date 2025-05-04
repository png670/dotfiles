/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"
#define BROWSER "icecat"

/* appearance */
static unsigned int borderpx  = 2;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static unsigned int gappih    = 7;       /* horiz inner gap between windows */
static unsigned int gappiv    = 7;       /* vert inner gap between windows */
static unsigned int gappoh    = 7;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 7;       /* vert outer gap between windows and screen edge */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char *fonts[]          = { "Terminus:size=12", "NotoColorEmoji:pixelsize=12:antialias=true:autohint=true"  };
static const char dmenufont[]       = "Terminus:size=12";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#770000";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n",  "spterm", "-g", "135x35", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:size=12", "-g", "50x20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spcalc",      spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ TERMINAL,   "spterm",   NULL,       SPTAG(0),     1,           1,          0,         -1 },
        { TERMINAL,  "spcalc",    NULL,      SPTAG(1),    1,           1,          0,         -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",	tile },	                /* Default: Master on left, slaves on right */
	{ "TTT",	bstack },               /* Master on top, slaves on bottom */

	{ "[@]",	spiral },               /* Fibonacci spiral */
	{ "[\\]",	dwindle },              /* Decreasing in size right and leftward */

	{ "[D]",	deck },	                /* Master on left, slaves in monocle-like mode on right */
	{ "[M]",	monocle },              /* All windows on top of eachother */

	{ "|M|",	centeredmaster },               /* Master in middle, slaves on sides */
	{ ">M>",	centeredfloatingmaster },       /* Same but master floats */

	{ "><>",	NULL },	                /* no layout function means floating behavior */
	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_j,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(-1) } }, \
	{ MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, \

#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };

#include <X11/XF86keysym.h>
#include "shiftview.c"

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
 	{ MODKEY, 			XK_b,	   spawn, 	   {.v = (const char*[]){ BROWSER, NULL } } },
	{ MODKEY|ShiftMask,		XK_w,      spawn,          {.v = (const char*[]){ TERMINAL, "-e", "nmtui", NULL } } },
 	{ MODKEY,			XK_n,	   spawn,	   {.v = (const char*[]){ TERMINAL, "-e", "lf", NULL } } },
 	{ MODKEY|ShiftMask,		XK_n,	   spawn,	   {.v = (const char*[]){ TERMINAL, "-e", "ncmpcpp", NULL } } },
	{ MODKEY,                       XK_e,      togglebar,      {0} },

        { MODKEY,		        XK_BackSpace,   togglescratch, 	 {.ui = 0} },
        { MODKEY|ShiftMask,	        XK_BackSpace,   togglescratch, 	 {.ui = 1} },

	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,			XK_o,	   incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,		XK_o,	   incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_z,      incrgaps,       {.i = +1 } },
	{ MODKEY,                       XK_x,      incrgaps,       {.i = -1 } },
	{ MODKEY,                       XK_a,      togglegaps,     {0} },
	{ MODKEY,			XK_f,	   togglefullscr,	 {0} },
	{ MODKEY|ShiftMask,             XK_a,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,		XK_space,  togglefloating, {0} },

	{ MODKEY,			XK_r,	setlayout, 	 {.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,		XK_r,	setlayout,	 {.v = &layouts[1]} }, /* bstack */
	{ MODKEY,			XK_t,	setlayout,	 {.v = &layouts[2]} }, /* monocle */
	{ MODKEY|ShiftMask,		XK_t,	setlayout,	 {.v = &layouts[3]} }, /* deck */
	{ MODKEY,		        XK_y,	setlayout,	 {.v = &layouts[4]} }, /* nrowgrid */
	{ MODKEY|ShiftMask,		XK_y,	setlayout,	 {.v = &layouts[5]} }, /* float */
	{ MODKEY,			XK_u,	setlayout,	 {.v = &layouts[6]} }, /* dwindle */
	{ MODKEY|ShiftMask,		XK_u,	setlayout,	 {.v = &layouts[7]} }, /* spiral */
	{ MODKEY,                       XK_i,   setlayout,       {.v = &layouts[8]} }, /* centeredmaster */
	{ MODKEY|ShiftMask,		XK_i,	setlayout,	 {.v = &layouts[9]} }, /* centeredfloatingmaster */


        { 0, XF86XK_AudioMute,           spawn,           SHCMD("amixer set Master toggle; kill -44 $(pidof dwmblocks)") },
        { 0, XF86XK_AudioRaiseVolume,    spawn,           SHCMD("amixer set Master 3%+; kill -44 $(pidof dwmblocks)") },
        { 0, XF86XK_AudioLowerVolume,    spawn,           SHCMD("amixer set Master 3%-; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_MonBrightnessUp,     spawn,           {.v = (const char*[]){ "xbacklight", "-inc", "15", NULL } } },
	{ 0, XF86XK_MonBrightnessDown,   spawn,           {.v = (const char*[]){ "xbacklight", "-dec", "15", NULL } } },

	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask           button          function        argument */
#ifndef __OpenBSD__
	{ ClkWinTitle,          0,                   Button2,        zoom,           {0} },
	{ ClkStatusText,        0,                   Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,                   Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,                   Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,                   Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,                   Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,           Button1,        sigdwmblocks,   {.i = 6} },
#endif
	{ ClkStatusText,        ShiftMask,           Button3,        spawn,          SHCMD(TERMINAL " -e nvim ~/.config/dwmblocks/config.h") },
	{ ClkClientWin,         MODKEY,              Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,              Button2,        defaultgaps,    {0} },
	{ ClkClientWin,         MODKEY,              Button3,        resizemouse,    {0} },
	{ ClkClientWin,		MODKEY,		     Button4,	     incrgaps,       {.i = +1} },
	{ ClkClientWin,		MODKEY,		     Button5,	     incrgaps,       {.i = -1} },
	{ ClkTagBar,            0,                   Button1,        view,           {0} },
	{ ClkTagBar,            0,                   Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,              Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,              Button3,        toggletag,      {0} },
	{ ClkTagBar,		0,		     Button4,	     shiftview,      {.i = -1} },
	{ ClkTagBar,		0,		     Button5,	     shiftview,      {.i = 1} },
	{ ClkRootWin,		0,		     Button2,	     togglebar,      {0} },
};

