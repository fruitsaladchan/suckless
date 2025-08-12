/* See LICENSE file for copyright and license details. */
/* Default settings; can be overriden by command line. */


static int topbar = 1;     /* -b  option; if 0, dmenu appears at bottom     */
static int fuzzy  = 1;     /* -F  option; if 0, dmenu doesn't use fuzzy matching */
static int centered = 1;   /* -c option; centers dmenu on screen */
static int min_width = 750; /* minimum width when centered */
static const float menu_height_ratio =
    3.5f; /* This is the ratio used in the original calculation */
/* -fn option overrides fonts[0]; default X11 font or font set */
static const char *fonts[] = {"FiraCode Nerd font:size=8.5"};
static const char *prompt =
    NULL; /* -p  option; prompt to the left of input field */

 static const char *colors[SchemeLast][2] = {
/*     fg         bg       */
   [SchemeNorm] = {"#bbbbbb", "#222222"},
   [SchemeSel] = {"#eeeeee", "#005577"},
   [SchemeOut] = {"#000000", "#00ffff"},
   // [SchemeSelHighlight] = { "#ffc978", "#005577" },
   // [SchemeNormHighlight] = { "#ffc978", "#222222" },
};
/* -l and -g options; controls number of lines and columns in grid if > 0 */
static unsigned int lines = 17;
static unsigned int columns = 1;

/*
 * Characters not considered part of a word while deleting words
 * for example: " /?\"&[]"
 */
static const char worddelimiters[] = " ";

/* Size of the window border */
static unsigned int border_width = 0; //3 is default
