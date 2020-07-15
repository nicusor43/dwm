/*  ____ _____  */
/* |  _ \_   _|  Derek Taylor (DistroTube) */
/* | | | || |  	http://www.youtube.com/c/DistroTube */
/* | |_| || |  	http://www.gitlab.com/dwt1/ */
/* |____/ |_|  	*/
#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */
/* appearance */
static const unsigned int borderpx = 2; /* border pixel of windows */
static const unsigned int snap = 32;	/* snap pixel */
static const unsigned int gappx = 6;	/* pixel gap between clients */
static const int showbar = 1;			/* 0 means no bar */
static const int topbar = 1;			/* 0 means bottom bar */
static const int horizpadbar = 6;		/* horizontal padding for statusbar */
static const int vertpadbar = 7;
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;       		/* vertical padding for statusbar */
/* Mononoki Nerd Font must be installed from AUR nerd-fonts-complete.
 * Otherwise, your default font will be Hack which is found in the standard
 * Arch repos and is listed as a dependency for this build. JoyPixels is also
 * a hard dependency and makes colored fonts and emojis possible.
 */
static const char *fonts[] = {"Wuncon Siji:size=12",
							  "Hack:size=10:antialias=true:autohint=true",
							  "JoyPixels:size=10:antialias=true:autohint=true"};
static const char col_gray1[] = "#282a36";
static const char col_gray2[] = "#282a36"; /* border color unfocused windows */
static const char col_gray3[] = "#ffffff";
static const char col_gray4[] = "#ffffff";
static const char col_cyan[] = "#008080"; /* border color focused windows and tags */
/* bar opacity 
 * 0xff is no transparency.
 * 0xee adds wee bit of transparency.
 * Play with the value to get desired transparency.
 */
static const unsigned int baralpha = 190;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3] = {
	/*               fg         bg         border   */
	[SchemeNorm] = {col_gray4, col_gray1, col_gray2},
	[SchemeSel] = {col_gray4, col_cyan, col_cyan},
};
static const unsigned int alphas[][3] = {
	/*               fg      bg        border     */
	[SchemeNorm] = {OPAQUE, baralpha, borderalpha},
	[SchemeSel] = {OPAQUE, baralpha, borderalpha},
};

/* tagging */
 static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" }; 
//static const char *tags[] = {"", "", "", "", "", "", "", "", ""};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{"Gimp", NULL, NULL, 0, 1, -1},
	{"Firefox", NULL, NULL, 1 << 8, 0, -1},
};

/* layout(s) */
static const float mfact = 0.50;  /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;	  /* number of clients in master area */
static const int resizehints = 0; /* 1 means respect size hints in tiled resizals */

#include "layouts.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{"[]=", tile}, /* first entry is default */
	{"><>", NULL}, /* no layout function means floating behavior */
	{"[M]", monocle},
	{"HHH", grid},
	{NULL, NULL},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                          \
	{MODKEY, KEY, view, {.ui = 1 << TAG}},                         \
		{MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}}, \
		{MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},          \
		{MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},
#define CMD(cmd)                                             \
	{                                                        \
		.v = (const char *[]) { "/bin/sh", "-c", cmd, NULL } \
	}

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                           \
	{                                                        \
		.v = (const char *[]) { "/bin/sh", "-c", cmd, NULL } \
	}

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-p", "Run: ", NULL};
static const char *termcmd[] = {"alacritty",  NULL};
static const char *tabtermcmd[] = {"alacritty", NULL};

/* brightness control*/
static const char *brupcmd[] = { "brightnessctl", "s", "10%+", NULL};
static const char *brdowncmd[] = { "brightnessctl", "s", "10%-", NULL};

static const char *prtcmd[] = { "maim", "|", "xclip", "-selection", "clipboard", "-t", "image/png", "-i", NULL};


static Key keys[] = {
	/* modifier             key        function        argument */
	{MODKEY | ShiftMask, XK_Return, spawn, {.v = dmenucmd}},
	{MODKEY, XK_Return, spawn, {.v = termcmd}},
	{Mod1Mask, XK_Return, spawn, {.v = tabtermcmd}},
	{MODKEY, XK_b, togglebar, {0}},
	{MODKEY | ShiftMask, XK_j, rotatestack, {.i = +1}},
	{MODKEY | ShiftMask, XK_k, rotatestack, {.i = -1}},
	{MODKEY, XK_j, focusstack, {.i = +1}},
	{MODKEY, XK_k, focusstack, {.i = -1}},
	{MODKEY, XK_i, incnmaster, {.i = +1}},
	{MODKEY, XK_d, incnmaster, {.i = -1}},
	{MODKEY, XK_h, setmfact, {.f = -0.05}},
	{MODKEY, XK_l, setmfact, {.f = +0.05}},
	{MODKEY | ControlMask, XK_Return, zoom, {0}},
	{MODKEY, XK_Tab, view, {0}},
	{MODKEY | ShiftMask, XK_c, killclient, {0}},

	{0, XF86XK_AudioMute, spawn, CMD("pulsemixer --toggle-mute")},
	{0, XF86XK_AudioLowerVolume, spawn,CMD("pulsemixer --change-volume -5")},
	{0, XF86XK_AudioRaiseVolume, spawn,CMD("pulsemixer --change-volume +5")},
	{0, XF86XK_MonBrightnessUp, spawn, {.v = brupcmd}},
	{0, XF86XK_MonBrightnessDown, spawn, {.v = brdowncmd}},
	{0, XK_Print, spawn, CMD("maim | xclip -selection clipboard -t image/png")},
	{ControlMask, XK_Print, spawn, CMD("maim -s -u | xclip -selection clipboard -t image/png -i")},
	/* Layout manipulation */
	{MODKEY, XK_Tab, cyclelayout, {.i = -1}},
	{MODKEY | ShiftMask, XK_Tab, cyclelayout, {.i = +1}},
	{MODKEY, XK_space, setlayout, {0}},
	{MODKEY | ShiftMask, XK_space, togglefloating, {0}},

	/* Switch to specific layouts */
	{MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
	{MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
	{MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
	{MODKEY, XK_g, setlayout, {.v = &layouts[3]}},

	{MODKEY, XK_0, view, {.ui = ~0}},
	{MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},

	/* Switching between monitors */
	{MODKEY, XK_comma, focusmon, {.i = -1}},
	{MODKEY, XK_period, focusmon, {.i = +1}},
	{MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
	{MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},

	/* Apps Launched with SUPER + ALT + KEY */
	{MODKEY | Mod1Mask, XK_b, spawn, CMD("tabbed -r 2 surf -pe x '.surf/html/homepage.html'")},
	{MODKEY | Mod1Mask, XK_c, spawn, CMD("alacritty -e cmus")},
	{MODKEY | Mod1Mask, XK_e, spawn, CMD("alacritty -e neomutt")},
	{MODKEY | Mod1Mask, XK_f, spawn, CMD("firefox")},
	{MODKEY | Mod1Mask, XK_h, spawn, CMD("st -e htop")},
	{MODKEY | Mod1Mask, XK_i, spawn, CMD("st -e irssi")},
	{MODKEY | Mod1Mask, XK_l, spawn, CMD("lutris")},
	{MODKEY | Mod1Mask, XK_n, spawn, CMD("st -e newsboat")},
	{MODKEY | Mod1Mask, XK_r, spawn, CMD("alacritty -e ranger")},

	/* Dmenu scripts launched with ALT + CTRL + KEY */
	{ControlMask, XK_KP_Begin, spawn, CMD("cmus-remote -u")},
	{ControlMask, XK_KP_Right, spawn, CMD("cmus-remote -n")},
	{ControlMask, XK_KP_Left, spawn, CMD("cmus-remote -r")},
	{ControlMask, XK_KP_2, spawn, CMD("./.dmenu/dmenu-reddio.sh")},
	{Mod1Mask | ControlMask, XK_s, spawn, CMD("./.dmenu/dmenu-surfraw.sh")},
	{Mod1Mask | ControlMask, XK_i, spawn, CMD("./.dmenu/dmenu-scrot.sh")},

	TAGKEYS(XK_1, 0)
		TAGKEYS(XK_2, 1)
			TAGKEYS(XK_3, 2)
				TAGKEYS(XK_4, 3)
					TAGKEYS(XK_5, 4)
						TAGKEYS(XK_6, 5)
							TAGKEYS(XK_7, 6)
								TAGKEYS(XK_8, 7)
									TAGKEYS(XK_9, 8){MODKEY | ShiftMask, XK_q, quit, {0}},
	{MODKEY | ShiftMask, XK_r, quit, {1}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click           event mask   button          function        argument */
	{ClkLtSymbol, 0, Button1, setlayout, {0}},
	{ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
	{ClkWinTitle, 0, Button2, zoom, {0}},
	{ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
	{ClkClientWin, MODKEY, Button1, movemouse, {0}},
	{ClkClientWin, MODKEY, Button2, togglefloating, {0}},
	{ClkClientWin, MODKEY, Button3, resizemouse, {0}},
	{ClkTagBar, 0, Button1, view, {0}},
	{ClkTagBar, 0, Button3, toggleview, {0}},
	{ClkTagBar, MODKEY, Button1, tag, {0}},
	{ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
