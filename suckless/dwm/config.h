#include "movestack.c"
#include <X11/X.h>
#include <X11/XF86keysym.h>
#include "exitdwm.c"

/* appearance */
static const unsigned int borderpx  = 0;       /* border pixel of windows */
static const unsigned int snap      = 3;       /* snap pixel */
static const int swallowfloating    = 0;       /* 1 means swallow floating by default */
static const unsigned int gappih    = 9;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 9;       /* vert inner gap between windows */
static const unsigned int gappoh    = 9;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 9;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;       /* 1 means no outer gap when there is only one window */
static const int user_bh            = 16;      /* 2 is the default spacing around the bar's font */
static const int showbar            = 1;       /* 0 means no bar */
static int topbar                   = 1;       /* 0 means bottom bar */
static const int vertpad            = 0;       /* vertical padding of bar 11 is default */
static const int sidepad            = 0;       /* 100 default horizontal padding of bar, 11 for flat */
static const char *fonts[] = {"JetBrainsMono Nerd Font:size=9.5:style:Bold"};
static const char dmenufont[] = "FiraCode Nerd Font:size=8";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tags */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
   	    /* class   instance  title          tags mask  isfloating  isterminal  noswallow  monitor */
        { "Firefox", NULL,     NULL,           0,         0,          0,           0,        -1 },
        { "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
        { "Nsxiv",   NULL,     NULL,           0,         1,          0,           0,        -1 },
        { NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 },

};

static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */


#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

/* layout(s) */
static const Layout layouts[] = {
    {"", tile},
    {"󰭩", NULL},
    {"", monocle},  {"", centeredmaster}, {"", centeredfloatingmaster},
    {"", bstack},   {"", bstackhoriz},    {"", spiral},
    {"[\\]", dwindle}, {"", horizgrid},      {"󱗼", gaplessgrid},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

// #define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define SHCMD(cmd) { .v = (const char*[]){ "/usr/bin/fish", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-i", NULL };
static const char *termcmd[] = {"st", NULL};
static const char *browser[] = {"librewolf", NULL};
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "110x34", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_n,      xrdb,           {.v = NULL } },
    { MODKEY,                       XK_d,      spawn,          {.v = (const char*[]){ "dmenu_run", "-i", NULL } } },
	{ MODKEY, 	                    XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,		        XK_Return, togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY|ShiftMask,             XK_w,      spawn,          {.v = browser } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,		            	XK_a,	   togglegaps,     {0}},
	{ MODKEY|ShiftMask,		        XK_a,      defaultgaps,    {0} },
	{ MODKEY, 			            XK_z, 	   incrgaps, 	   {.i = +3}},
   	{ MODKEY, 	            		XK_x,	   incrgaps, 	   {.i = -3}},
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
/*  { MODKEY,                       XK_g,      incnmaster,     {.i = +1 } }, 
	{ MODKEY,                       XK_m,      incnmaster,     {.i = -1 } }, */
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY, 	            		XK_f,	   togglefullscr,  {0}},
	{ MODKEY,	                    XK_q,      killclient,     {0} },
	{ MODKEY, 		            	XK_t, 	   setlayout, 	   {.v = &layouts[0]}},
   	{ MODKEY|ShiftMask, 	    	XK_space,  setlayout, 	   {.v = &layouts[1]}},
    { MODKEY, 			            XK_i,	   setlayout, 	   {.v = &layouts[3]}},
    { MODKEY|ShiftMask, 		    XK_i,	   setlayout, 	   {.v = &layouts[4]}},
    { MODKEY|ShiftMask, 		    XK_t,	   setlayout, 	   {.v = &layouts[5]}},
    { MODKEY, 			            XK_y,	   setlayout, 	   {.v = &layouts[7]}},
    { MODKEY|ShiftMask, 		    XK_y,	   setlayout, 	   {.v = &layouts[8]}},
    { MODKEY, 			            XK_u,	   setlayout, 	   {.v = &layouts[9]}},
    { MODKEY|ShiftMask,		        XK_u,	   setlayout, 	   {.v = &layouts[10]}},
    { MODKEY,			            XK_w,	   togglefloating, {0}},
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
/*  { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } }, */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ControlMask|ShiftMask, XK_r,      quit,          {1} },
	{ 0,XF86XK_PowerOff,                       exitdwm,       {0} },
    { MODKEY,                      XK_p,  spawn, {.v = (const char*[]){ "wallpaper", NULL } } },
    { MODKEY|ShiftMask,            XK_n,  spawn, {.v = (const char*[]){ "dmenu-notes", NULL } } },
    { MODKEY|ShiftMask,            XK_p,  spawn, {.v = (const char*[]){ "passmenu", "-i", "-l", "10",  NULL } } },
    { 0, XF86XK_MonBrightnessUp,   spawn, {.v = (const char*[]){ "changebrightness", "up", NULL } } },
    { 0, XF86XK_MonBrightnessDown, spawn, {.v = (const char*[]){ "changebrightness", "down", NULL } } },
    { 0, XF86XK_AudioRaiseVolume,  spawn, SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%") },
    { 0, XF86XK_AudioLowerVolume,  spawn, SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%") },
    { 0, XF86XK_AudioMute, spawn,  SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle") },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

