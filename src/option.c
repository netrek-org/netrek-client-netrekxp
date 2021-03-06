/******************************************************************************/
/***  File:  option.c
/***
/***  Function:
/***
/***  Revisions:
/***    ssheldon - Cleaned up source code, added #include "proto.h"
/***               and function header comments
/******************************************************************************/


#include <stdio.h>
#include <ctype.h>

#include "config.h"
#include "copyright.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "packets.h"
#include "playerlist.h"
#include "map.h"
#include "proto.h"

int notdone;                    /* not done flag */

static char newkeys[14];
static char newbuttons[10];
static char newlogin[PSEUDOSIZE];

char *localmes[] = { "Show owner on local planets",
                     "Show resources on local planets",
                     "Show nothing on local planets",
                     ""
};

char *galacticmes[] = { "Show owner on galactic map",
                        "Show resources on galactic map",
                        "Show nothing on galactic map",
                        ""
};

#ifdef ROTATERACE
char *rotatemess[] = { "Don't rotate galaxy",
                       "Rotate galaxy 90 degrees",
                       "Rotate galaxy 180 degrees",
                       "Rotate galaxy 270 degrees",
                       ""
};

#endif

static char *lockoptions[] = { "Don't show lock icon",
                               "Show lock icon on galactic map only",
                               "Show lock icon on tactical map only",
                               "Show lock icon on both map windows",
                               ""
};

static char *plistoptions[] = { "Custom player list",
                                "Old Player List",
                                "COW Player List",
                                "Kill Watch Player List",
                                "BRMH Player List",
                                ""
};


static char *dashboardoptions[] = { "Text dashboard",
                                    "COW style dashboard",
                                    "KRP style dashboard",
                                    "LABs new dashboard",
                                    ""
};

/* useful for options that are an int with a range */
struct int_range
{
    int min_value;              /* value is >= this */
    int max_value;              /* value is <= this */
    int increment;              /* a click raises/lowers
                                 * this amount */
};

static char *phaserdispmess[] = { "Don't log phaser hits",
                                  "Log phasers on all window",
                                  "Log phasers on team window",
                                  "Log phasers on indiv window",
                                  "Log phasers on kill window",
                                  "Log phasers on review window",
                                  ""
};

#ifdef PHASER_STATS
static char *phaserstatmess[] = { "Don't show phaser statistics",
                                  "Show phaser statistics",
                                  ""
};
#endif

#ifdef XTRA_MESSAGE_UI
static char *messagehudmess[] = { "Message typed on message line",
                                  "Message typed on local top",
                                  "Message typed on local bottom",
                                  ""
};
#endif

static char *playerlistobservermess[] = { "Display all players in playerlist",
                                          "Display in game players only",
                                          "Display observers only",
                                          "Display players, then observers",
                                          "Playerlist is off",
                                          ""
};

static char *shrinkphasermess[] = { "COW style for phasers shrink",
                                    "BRMH style for phasers shrink",
                                    ""
};

static char *agricolormess[] = { "Race color for AGRI name",
                                 "White color for AGRI name",
                                 "Gray color for AGRI name",
                                 ""
};

static char *bitmaptypemess[] = { "Mono bitmaps",
                                  "New color bitmaps",
                                  "Old color bitmaps",
                                  "Shaded old color bitmaps",
                                  "Experimental high res bitmaps",
                                  ""
};

static char *windowmovemess[] = { "Disable moving of internal windows",
                                  "Enable moving of internal windows",
                                  ""
};

static char *plistmessagingmess[] = { "Disable player list messaging",
                                      "Enable player list messaging",
                                      ""
};

static char *savebigmess[] = { "Save options without comments",
                               "Save options with comments",
                               ""
};

static char *timermess[] = { "Timer shows nothing (off)",
                            "Timer shows time of day",
                            "Timer shows time on server",
                            "Timer shows time in ship",
                            "Timer shows user-set time",
                            "Timer shows game-related info",
                            ""
};

static char *planetbitmapmess[] = { "Show Bronco bitmaps on local map",
                                    "Show Moo bitmaps on local map",
                                    "Show Rabbitear bitmaps on local map",
                                    "Show New color bitmaps on local map",
                                    ""
};

static char *planetbitmapgalaxymess[] = { "Show Bronco bitmaps on galactic map",
                                          "Show Moo bitmaps on galactic map",
                                          "Show Rabbitear bitmaps on galactic map",
                                          "Show New color bitmaps on galactic map",
                                          ""
};

static char *autowarmess[] = { "Don't auto set war declarations",
                              "Set war with nonzero player teams",
                              "Set war with largest enemy team",
                              ""
};

static char *viewrangemess[] = { "Don't show range circle",
                                 "Only show range circle if cloaked",
                                 "Only show range circle if carrying",
                                 "Show range circle at all times",
                                 ""
};

static char *armycountmess[] = { "Don't show army counts",
                                 "Show army counts on local",
                                 "Show army counts on map",
                                 "Show army counts on local and map",
                                 ""
};

/* Only one of op_option, op_targetwin, and op_string should be defined. If
 * op_string is defined, op_size should be too and op_text is used without a
 * "Don't" prefix. if op_range is defined, there should be a %d in op_text
 * for it, op_size will be non-useful, and the 'Don't ' prefix won't appear */
struct option
{
    int op_num;
    char *op_text;              /* text to display when on */
    int *op_option;             /* variable to test/modify
                                 * (optional) */
    W_Window *op_targetwin;     /* target window to
                                 * map/unmap (optional) */
    char *op_string;            /* string to modify
                                 * (optional) */
    int op_size;                /* size of *op_string
                                 * (optional) */
    char **op_array;            /* array of strings to
                                 * switch between */
    struct int_range *op_range; /* struct defining an
                                 * integer range option */
};

/* for the paged options menus */
struct option_menu
{
    int page_num;               /* page number of this menu */
    struct option_menu *Next;
    struct option *menu;        /* pointers to arrary of
                                 * options */
    int numopt;                 /* number of options in this
                                 * menu page */
    int updated;                /* 1 if options can be
                                 * changed externally */
};

/* pointer to first entry in the options menu list */
struct option_menu *FirstMenu = NULL;
struct option_menu *CurrentMenu = NULL; /* menu currently looked at */
int MenuPage = 0;               /* current menu page */
int MaxOptions = 0;             /* maximum number of options
                                 * in all menu pages */
struct int_range MenuPages = { 0, 1, 1 };


/* updates: use of the int range thing... */
struct int_range updates_range = { 1, 50, 1 };
struct int_range redraw_updates_range = { 0, 5, 1 };

struct int_range phaserShrinkRng = { 0, 16, 1 };

/* range of menus. Will be updated when menu list is assembled */
struct int_range Menus_Range = { 0, 1, 1 };

struct int_range bitmap_range = { 0, 4, 1 };

struct int_range keepInfo_range = { 0, 100, 1 };

struct int_range enPhasRng = { 0, 10, 1 };

#ifdef XTRA_MESSAGE_UI
struct int_range messagehud_range = { 0, 2, 1 };
#endif

#ifdef MOTION_MOUSE
struct int_range clickdelay_range = { 0, 50, 1 };
#endif

struct int_range playerlistobserver_range = { 0, 4, 1 };

struct int_range planetbitmaprange = { 0, 3, 1 };

struct int_range planetbitmapgalaxyrange = { 0, 3, 1 };

int saveOpts = 1;    /* Temp flag to use for save options action */

/* menus */

struct option Ship_Menu[] = {
    {0, "Ship Menu", &MenuPage, 0, 0, 0, NULL, &Menus_Range},
    {1, "Page %d (click to change)", &MenuPage, 0, 0, 0, NULL, &Menus_Range},
    {1, "", &colorClient, 0, 0, 0, bitmaptypemess, &bitmap_range},
    {1, "draw ships to 256 positions", &fullBitmapRotation, 0, 0, 0, NULL, NULL},
    {1, "vary shields bitmap w/ dam", &varyShields, 0, 0, 0, NULL, NULL},
    {1, "vary shields color w/ dam", &varyShieldsColor, 0, 0, 0, NULL, NULL},
    {1, "use warning shields", &warnShields, 0, 0, 0, NULL, NULL},
    {1, "use warning hull", &vary_hull, 0, 0, 0, NULL, NULL},
    {1, "show my speed on local", &showMySpeed, 0, 0, 0, NULL, NULL},
    {1, "show other's speed on local", &showOtherSpeed, 0, 0, 0, NULL, NULL},
    {1, "show cloakers on local", &showCloakers, 0, 0, 0, NULL, NULL},
    {1, "show tractor/pressor beams", &showTractorPressor, 0, 0, 0, NULL, NULL},
    {1, "show tractors after lock", &continueTractor, 0, 0, 0, NULL, NULL},
    {1, "show tractor target ID", &tractorID, 0, 0, 0, NULL, NULL},
    {1, "show all players tract/press", &showAllTractorPressor, 0, 0, 0, NULL, NULL},
    {1, "report kills", &reportKills, 0, 0, 0, NULL, NULL},
    {1, "show heading tic mark", &headingTic, 0, 0, 0, NULL, NULL},
    {1, "show lock line", &lockLine, 0, 0, 0, NULL, NULL},
    {1, "show det circle", &detCircle, 0, 0, 0, NULL, NULL},
    {1, "done", &notdone, 0, 0, 0, NULL, NULL},
    {-1, NULL, 0, 0, 0, 0, NULL, NULL}
};

struct option Weapons_Menu[] = {
    {0, "Weapons Menu", &MenuPage, 0, 0, 0, NULL, &Menus_Range},
    {1, "Page %d (click to change)", &MenuPage, 0, 0, 0, NULL, &Menus_Range},
    {1, "use auto aim phasers", &autoPhaser, 0, 0, 0, NULL, NULL},
    {1, "use color weapon bitmaps", &colorWeapons, 0, 0, 0, NULL, NULL},
    {1, "show weapons on galactic", &weaponsOnMap, 0, 0, 0, NULL, NULL},
    {1, "use colorful phasers", &colorfulPhasers, 0, 0, 0, NULL, NULL},
    {1, "highlight friendly phasers", &highlightFriendlyPhasers, 0, 0, 0, NULL, NULL},
    {1, "enemy phaser width: %d", &enemyPhasers, 0, 0, 0, NULL, &enPhasRng},
    {1, "", &phaserShrinkStyle, 0, 0, 0, shrinkphasermess, NULL},
    {1, "shrink my phaser by %d/16", &phaserShrink, 0, 0, 0, NULL, &phaserShrinkRng},
    {1, "shrink their phasers by %d/16", &theirPhaserShrink, 0, 0, 0, NULL, &phaserShrinkRng},
    {1, "shrink phasers on a miss", &shrinkPhaserOnMiss, 0, 0, 0, NULL, NULL},
    {1, "done", &notdone, 0, 0, 0, NULL, NULL},
    {-1, NULL, 0, 0, 0, 0, NULL, NULL}
};

struct option Planet_Menu[] = {
    {0, "Planet Menu", &MenuPage, 0, 0, 0, NULL, &Menus_Range},
    {1, "Page %d (click to change)", &MenuPage, 0, 0, 0, NULL, &Menus_Range},
    {1, "", &planetBitmap, 0, 0, 0, planetbitmapmess, &planetbitmaprange},
    {1, "", &planetBitmapGalaxy, 0, 0, 0, planetbitmapgalaxymess, &planetbitmapgalaxyrange},
    {1, "highlight planets", &planetHighlighting, 0, 0, 0, NULL, NULL},
    {1, "rotate new planets", &rotatePlanets, 0, 0, 0, NULL, NULL},
    {1, "show planet names on local", &showPlanetNames, 0, 0, 0, NULL, NULL},
    {1, "", &showArmy, 0, 0, 0, armycountmess, NULL},
    {1, "show owner on galactic", &showPlanetOwner, 0, 0, 0, NULL, NULL},
    {1, "show IND planets", &showIND, 0, 0, 0, NULL, NULL},
    {1, "show AGRI in caps on map", &agriCAPS, 0, 0, 0, NULL, NULL},
    {1, "", &agriColor, 0, 0, 0, agricolormess, NULL},
    {1, "", &showLock, 0, 0, 0, lockoptions, NULL},
#ifdef ROTATERACE
    {1, "autorotate galaxy", &autoRotate, 0, 0, 0, NULL, NULL},
    {1, "", &rotate, 0, 0, 0, rotatemess, NULL},
#endif
    {1, "sort the planetlist window", &sortPlanets, 0, 0, 0, NULL, NULL},
    {1, "done", &notdone, 0, 0, 0, NULL, NULL},
    {-1, NULL, 0, 0, 0, 0, NULL, NULL}
};

struct option Control_Menu[] = {
    {0, "Control Menu", &MenuPage, 0, 0, 0, NULL, &Menus_Range},
    {1, "Page %d (click to change)", &MenuPage, 0, 0, 0, NULL, &Menus_Range},
    {1, "stay peaceful when reborn", &keepPeace, 0, 0, 0, NULL, NULL},
    {1, "", &autoSetWar, 0, 0, 0, autowarmess, NULL},
    {1, "use new distress", &newDistress, 0, 0, 0, NULL, NULL},
#ifdef MOTION_MOUSE
    {1, "use continuous mouse", &continuousMouse, 0, 0, 0, NULL, NULL},
    {1, "fix continuous mouse", &continuousMouseFix, 0, 0, 0, NULL, NULL},
    {1, "%d updates repeat delay", &clickDelay, 0, 0, 0, NULL, &clickdelay_range},
#endif
    {1, "allow wheel actions", &allowWheelActions, 0, 0, 0, NULL, NULL},
    {1, "new keymap entries: %s", 0, 0, newkeys, 13, NULL, NULL},
    {1, "new buttonmap entries: %s", 0, 0, newbuttons, 9, NULL, NULL},
    {1, "new login name: %s", 0, 0, newlogin, PSEUDOSIZE, NULL, NULL},
    {1, "ignore the capslock key", &ignoreCaps, 0, 0, 0, NULL, NULL},
    {1, "%d updates per second", &updatesPerSec, 0, 0, 0, 0, &updates_range},
    {1, "%d 1/10 sec screen refresh delay", &redrawDelay, 0, 0, 0, 0,
     &redraw_updates_range},
    {1, "", &windowMove, 0, 0, 0, windowmovemess, NULL},
    {1, "avoid message kludge", &niftyNewMessages, 0, 0, 0, NULL, NULL},
    {1, "", &playerListMessaging, 0, 0, 0, plistmessagingmess, NULL},
    {1, "have only msg key start msg", &messageKeyOnly, 0, 0, 0, NULL, NULL},
    {1, "beep on private message", &beepOnPrivateMessage, 0, 0, 0, NULL, NULL},
    {1, "hide console window", &hideConsole, 0, 0, 0, NULL, NULL},
    {1, "done", &notdone, 0, 0, 0, NULL, NULL},
    {-1, NULL, 0, 0, 0, 0, NULL, NULL}
};

struct option Window_Menu[] = {
    {0, "Info Menu", &MenuPage, 0, 0, 0, NULL, &Menus_Range},
    {1, "Page %d (click to change)", &MenuPage, 0, 0, 0, NULL, &Menus_Range},
    {1, "show \"all\" message window", 0, &messwa, 0, 0, NULL, NULL},
    {1, "show \"team\" message window", 0, &messwt, 0, 0, NULL, NULL},
    {1, "show \"your\" message window", 0, &messwi, 0, 0, NULL, NULL},
    {1, "show \"kill\" message window", 0, &messwk, 0, 0, NULL, NULL},
    {1, "show \"total\" message window", 0, &reviewWin, 0, 0, NULL, NULL},
    {1, "show phaser log window", 0, &phaserwin, 0, 0, NULL},
    {1, "show statistic window", 0, &statwin, 0, 0, NULL, NULL},
    {1, "show help window", 0, &helpWin, 0, 0, NULL, NULL},
    {1, "show hints window", &showHints, &hintWin, 0, 0, NULL, NULL},
#ifdef XTREKRC_HELP
    {1, "show xtrekrc defaults window", 0, &defWin, 0, 0, NULL, NULL},
#endif
#ifdef DOC_WIN
    {1, "show netrekrc.txt", 0, &xtrekrcwin, 0, 0, NULL, NULL},
    {1, "show documentation window", 0, &docwin, 0, 0, NULL, NULL},
#endif
#ifdef TOOLS
    {1, "show shell tools window", 0, &toolsWin, 0, 0, NULL, NULL},
#endif
#ifdef SOUND
    {1, "show sound control window", 0, &soundWin, 0, 0, NULL, NULL},
#endif
    {1, "show UDP control window", 0, &udpWin, 0, 0, NULL, NULL},
    {1, "show ping stats window", 0, &pStats, 0, 0, NULL},
    {1, "show short packets window", 0, &spWin, 0, 0, NULL, NULL},
    {1, "done", &notdone, 0, 0, 0, NULL, NULL},
    {-1, NULL, 0, 0, 0, 0, NULL, NULL}
};

struct option Playerlist_Menu[] = {
    {0, "Playerlist Menu", &MenuPage, 0, 0, 0, NULL, &Menus_Range},
    {1, "Page %d (click to change)", &MenuPage, 0, 0, 0, NULL, &Menus_Range},
    {1, "", &playerListStyle, 0, 0, 0, plistoptions, NULL},
    {1, "show alternate player list", 0, &playerw2, 0, 0, NULL, NULL},
    {1, "sort the playerlist", &sortPlayers, 0, 0, 0, NULL, 0},
    {1, "put my team first in the list", &sortMyTeamFirst, 0, 0, 0, NULL, 0},
    {1, "partition the playerlist", &partitionPlist, 0, 0, 0, NULL, 0},
    {1, "", &playerListObserver, 0, 0, 0, playerlistobservermess, &playerlistobserver_range},
    {1, "blank zero kills", &playerListBlankZeroKills, 0, 0, 0, NULL, 0},
    {1, "hack player list", &playerListHack, 0, 0, 0, NULL, 0},
    {1, "done", &notdone, 0, 0, 0, NULL, NULL},
    {-1, NULL, 0, 0, 0, 0, NULL, NULL}
};

struct option Visual_Menu[] = {
    {0, "Visuals Menu", &MenuPage, 0, 0, 0, NULL, &Menus_Range},
    {1, "Page %d (click to change)", &MenuPage, 0, 0, 0, NULL, &Menus_Range},
    {1, "", &newDashboard, 0, 0, 0, dashboardoptions, NULL},
    {1, "show packet lights", &packetLights, 0, 0, 0, NULL, NULL},
    {1, "", &timerType, 0, 0, 0, timermess, NULL},
    {1, "keep info on for %d x .1s (0=perm)", &keepInfo, 0, 0, 0, 0, &keepInfo_range},
#ifdef BEEPLITE
    {1, "use beeplite", &useLite, 0, 0, 0, NULL, NULL},
#endif
    {1, "omit team letter on map", &omitTeamLetter, 0, 0, 0, NULL, NULL},
    {1, "draw view box on map", &viewBox, 0, 0, 0, NULL, NULL},
    {1, "", &viewRange, 0, 0, 0, viewrangemess, NULL},
    {1, "show info limits on local", &infoRange, 0, 0, 0, NULL, NULL},
    {1, "draw stars on local", &showStars, 0, 0, 0, NULL, NULL},
    {1, "draw warp streaks", &warpStreaks, 0, 0, 0, NULL, NULL},
    {1, "alert on extra border(s)", &extraAlertBorder, 0, 0, 0, NULL, NULL},
#ifdef PHASER_STATS
    {1, "", &phaserStats, 0, 0, 0, phaserstatmess, NULL},
#endif
#ifdef XTRA_MESSAGE_UI
    {1, "", &messageHUD, 0, 0, 0, messagehudmess, &messagehud_range},
#endif
    {1, "use double buffering", &doubleBuffering, 0, 0, 0, NULL, NULL},
    {1, "done", &notdone, 0, 0, 0, NULL, NULL},
    {-1, NULL, 0, 0, 0, 0, NULL, NULL}
};

struct option Hockey_Menu[] = {
    {0, "Hockey Menu", &MenuPage, 0, 0, 0, NULL, &Menus_Range},
    {1, "Page %d (click to change)", &MenuPage, 0, 0, 0, NULL, &Menus_Range},
#ifdef HOCKEY_LINES
    {1, "show puck circle", &puckCircle, 0, 0, 0, NULL, NULL},
    {1, "show puck arrow", &puckArrow, 0, 0, 0, NULL, NULL},
    {1, "show hockey lines on local", &showHockeyLinesLocal, 0, 0, 0, NULL, NULL},
    {1, "show hockey lines on map", &showHockeyLinesMap, 0, 0, 0, NULL, NULL},
    {1, "show hockey score on map", &showHockeyScore, 0, 0, 0, NULL, NULL},
#endif
    {1, "done", &notdone, 0, 0, 0, NULL, NULL},
    {-1, NULL, 0, 0, 0, 0, NULL, NULL}
};

struct option Save_Menu[] = {
    {0, "Save Menu", &MenuPage, 0, 0, 0, NULL, &Menus_Range},
    {1, "Page %d (click to change)", &MenuPage, 0, 0, 0, NULL, &Menus_Range},
    {1, "", &saveBig, 0, 0, 0, savebigmess, NULL},
    {1, "save window placements", &saveWindow, 0, 0, 0, NULL, NULL},
    {1, "save macros", &saveMacro, 0, 0, 0, NULL, NULL},
    {1, "save RCD", &saveRCD, 0, 0, 0, NULL, NULL},
#ifdef BEEPLITE
    {1, "save beeplite macros", &saveBeeplite, 0, 0, 0, NULL, NULL},
#endif
#ifdef RCM
    {1, "save RCM", &saveRCM, 0, 0, 0, NULL, NULL},
#endif
    {1, "save options now", &saveOpts, 0, 0, 0, NULL, NULL},
    {1, "done", &notdone, 0, 0, 0, NULL, NULL},
    {-1, NULL, 0, 0, 0, 0, NULL, NULL}
};


#define NUMOPTIONS(menu) ((sizeof((menu))/sizeof((menu)[0]))-1)

/* option menu sizes and such */
#define OPTIONBORDER    2
#define OPTIONLEN       35

/* Set up the option menus and window. */
void
optionwindow (void)
{

    /* Init not done flag */
    notdone = 1;

    *newkeys = '\0';
    *newbuttons = '\0';
    *newlogin = '\0';
    if (FirstMenu == NULL)
    {
        MaxOptions = InitOptionMenus ();
        if (MaxOptions < 0)
        {
            LineToConsole ("InitOptionMenus() error %s!\n", MaxOptions);
            notdone = 0;
            return;
        }
    }

    /* Create window big enough to hold option windows */
    if (optionWin == NULL)
    {

        optionWin = W_MakeMenu ("option", TWINSIDE + 10, -BORDER + 10, OPTIONLEN,
                                 MaxOptions, baseWin, OPTIONBORDER);
        W_ResizeMenuToNumItems (optionWin, CurrentMenu->numopt);
        W_SetWindowKeyDownHandler (optionWin, optionaction);
        W_SetWindowButtonHandler (optionWin, optionaction);
        W_DefineArrowCursor (optionWin);
        CurrentMenu = FirstMenu;

        RefreshOptions ();
    }

    /* Map window */
    W_MapWindow (optionWin);
}

/* refresh all current options */
void
RefreshOptions (void)
{
    int i;
    struct option_menu *option;

    if (notdone == 0 || (option = CurrentMenu) == NULL)
        return;

    for (i = 0; i < option->numopt; i++)
    {
        optionrefresh (&(option->menu[i]));
    }
    if (option->numopt < MaxOptions)
        for (i = option->numopt; i < MaxOptions; i++)
        {
            OptionClear (i);
        }
}

/* blank out option line 'i' */
void
OptionClear (int i)
{
    char *blanktext = "                                               ";

    if (optionWin && notdone)
        W_WriteText (optionWin, 0, i, textColor, blanktext, OPTIONLEN, 0);
}

/* Redraw the specified option entry */
void
optionredrawtarget (W_Window win)
{
    register struct option *op;

    if (notdone == 0)
        return;

    for (op = CurrentMenu->menu; op->op_text; op++)
    {
        if (op->op_targetwin && win == *op->op_targetwin)
        {
            optionrefresh (op);
            break;
        }
    }
}

/* Redraw the specified option option */
void
optionredrawoption (int *ip)
{
    register struct option *op;

    if (notdone == 0)
        return;

    for (op = CurrentMenu->menu; op->op_num >= 0; op++)
    {
        if (ip == op->op_option)
        {
            optionrefresh (op);
            break;
        }
    }
}

/* Refresh the option window given by the option struct */
void
optionrefresh (register struct option *op)
{
    register int on;
    char buf[BUFSIZ];

    if (op == NULL || notdone == 0)
        return;

    if (op->op_string)
    {
        (void) sprintf (buf, op->op_text, op->op_string);
    }
    else if (op->op_array)
    {                           /* Array of strings */
        strcpy (buf, op->op_array[*op->op_option]);
    }
    else if (op->op_range)
    {
        (void) sprintf (buf, op->op_text, *(op->op_option));
    }
    else
    {
        /* Either a boolean or a window */
        if (op->op_option)
            on = *op->op_option;        /* use int for status */
        else if (op->op_targetwin)
            on = W_IsMapped (*op->op_targetwin);        /* use window for status */
        else
            on = 1;             /* shouldn't happen */

        if (!on)
            strcpy (buf, "Don't ");
        else
            buf[0] = '\0';
        strcat (buf, op->op_text);
    }

    if (islower (buf[0]))
        buf[0] = (char) (toupper (buf[0]));

    if (op->op_num == 0)
        W_WriteText (optionWin, 0, op->op_num, W_Yellow, buf, strlen (buf), 0);
    else if (op->op_num == 1)
        W_WriteText (optionWin, 0, op->op_num, W_Green, buf, strlen (buf), 0);
    else
        W_WriteText (optionWin, 0, op->op_num, textColor, buf, strlen (buf), 0);
}

/* deal with events sent to the option window */
void
optionaction (W_Event * data)
{
    register struct option *op;
    int i;
    register char *cp;

    if (data->y >= CurrentMenu->numopt)
    {
        W_Beep ();
        return;
    }
    if (notdone == 0)
        return;

    op = &(CurrentMenu->menu[data->y]);

    /* Update string; don't claim keystrokes for non-string options */
    /* deal with options with string input first */
    if (op->op_string == 0)
    {
        if (data->type == W_EV_KEY)
            return;
    }
    else
    {
        if (data->type == W_EV_BUTTON)
            return;
        switch (data->key)
        {

        case '\b':             /* delete character */
        case '\177':
            cp = op->op_string;
            i = strlen (cp);
            if (i > 0)
            {
                cp += i - 1;
                *cp = '\0';
            }
            break;

        case '\027':           /* word erase */
            cp = op->op_string;
            i = strlen (cp);
            /* back up over blanks */
            while (--i >= 0 && isspace (cp[i]))
                ;
            i++;
            /* back up over non-blanks */
            while (--i >= 0 && !isspace (cp[i]))
                ;
            i++;
            cp[i] = '\0';
            break;

        case '\025':           /* kill line */
        case '\030':
            op->op_string[0] = '\0';
            break;

        default:               /* add character to the list */
            if (data->key < 32 || data->key > 127)
                break;
            cp = op->op_string;
            i = strlen (cp);
            if (i < (op->op_size - 1) && !iscntrl (data->key))
            {
                cp += i;
                cp[1] = '\0';
                cp[0] = data->key;
            }
            else
                W_Beep ();
            break;
        }
    }

    /* Toggle int, if it exists */
    if (op->op_array)
    {
         /* Actions to be taken when certain options are selected. (Yes, this is
         * a hack). */
        if (op->op_option == &colorClient)
        {
            if (!dynamicBitmaps)
                return;
        }
        if (data->key == W_RBUTTON)
        {
            (*op->op_option)++;
            if (*(op->op_array)[*op->op_option] == '\0')
            {
                *op->op_option = 0;
            }
        }
        else if (data->key == W_MBUTTON)
        {
            /* set option number to zero on the middle key to ease shutoff */
            *op->op_option = 0;
        }
        else if (data->key == W_LBUTTON)
        {
            /* if left button, decrease option  */
            (*op->op_option)--;
            /* if decreased too far, set to top option */
            if (*(op->op_option) < 0)
            {
                *op->op_option = 0;
                while (*(op->op_array)[*op->op_option] != '\0')
                {
                    (*op->op_option)++;
                }
                (*op->op_option)--;
            }
        }


        /* Actions to be taken when certain options are selected. (Yes, this is
         * a hack). */

        if (op->op_option == &playerListStyle)
        {
            if (plistCustomLayout == 0 && playerListStyle == 0)
                playerListStyle = (data->key == W_LBUTTON) ? PLISTLASTSTYLE : 1;

            if (W_IsMapped (playerw) || W_IsMapped (playerw2))
                RedrawPlayerList ();
        }
        else if (op->op_option == &playerListObserver)
        {
            if (W_IsMapped (playerw) || W_IsMapped (playerw2))
                RedrawPlayerList ();
        }
        /* Let's see if this is our option changed */
        // Paradise bitmaps stay loaded at all times
        else if (op->op_option == &planetBitmap && !paradise)
        {
            if (planetBitmap != 3) // Color planet bitmaps stay loaded at all times
            {
                char *Planlib;
    
                for (i = 0; i < PLANET_VIEWS; i++)
                    free (bplanets[i]);

                switch (planetBitmap)
                {
                case 1:
                    Planlib = "bitmaps/planlibm/planM.bmp";
                    break;
                case 2:
                    Planlib = "bitmaps/planlibm/planR.bmp";
                    break;
                default:
                    Planlib = "bitmaps/planlibm/plan.bmp";
                    break;
                }

                base_planets = W_StoreBitmap3 (Planlib, BMP_PLANET_WIDTH, BMP_PLANET_HEIGHT * 9,
                                                BMP_PLANET000, w, LR_MONOCHROME);
  
                for (i = 0; i < PLANET_VIEWS; i++)
                {
                    bplanets[i] = W_PointBitmap2 (base_planets, 0, i, BMP_PLANET_WIDTH,
                                                    BMP_PLANET_HEIGHT);
                }
            }

            redrawall = 1;
        }
        // Paradise bitmaps stay loaded at all times
        else if (op->op_option == &planetBitmapGalaxy && !paradise)
        {
            if (planetBitmapGalaxy != 3) // Color planet bitmaps stay loaded at all times
            {
                char *MPlanlib;
    
                for (i = 0; i < MPLANET_VIEWS; i++)
                    free (bmplanets[i]);

                switch (planetBitmapGalaxy)
                {
                case 1:
                    MPlanlib = "bitmaps/planlibm/mplanM.bmp";
                    break;
                case 2:
                    MPlanlib = "bitmaps/planlibm/mplanR.bmp";
                    break;
                default:
                    MPlanlib = "bitmaps/planlibm/mplan.bmp";
                    break;
                }

                base_mplanets = W_StoreBitmap3 (MPlanlib, BMP_MPLANET_WIDTH, BMP_MPLANET_HEIGHT * 9,
                                                BMP_MPLANET000, mapw, LR_MONOCHROME);
    
                for (i = 0; i < PLANET_VIEWS; i++)
                {
                    bmplanets[i] = W_PointBitmap2 (base_mplanets, 0, i, BMP_MPLANET_WIDTH,
                                                    BMP_MPLANET_HEIGHT);
                }
            }

            redrawall = 1;
        }
#ifdef ROTATERACE
        else if (rotate != old_rotate)
            rotateGalaxy();
#endif
    }

    /* Is it a special non-linear option range? i.e updatesPerSec */
    else if (op->op_range && op->op_option == &updatesPerSec)
    {
        if (data->key == W_RBUTTON)
        {
            if (++updatesPerSec > server_fps) ; /* Don't exceed server fps */
            else 
                while (server_fps % updatesPerSec != 0) updatesPerSec++;
        }
        else if (data->key == W_MBUTTON)
            updatesPerSec = 1;
        else if (data->key == W_LBUTTON)
        {
            if (--updatesPerSec < 1) ; /* Don't divide by 0 */
            else
                while (server_fps % updatesPerSec != 0) updatesPerSec--;
        }
        /* wrap value around within updatesPerSec range */
        if (updatesPerSec > server_fps)
            updatesPerSec = 1;
        if (updatesPerSec < 1)
            updatesPerSec = server_fps;
        if (updatesPerSec != lastUpdateSpeed)
        {
            sendUpdatePacket (1000000 / updatesPerSec);
            lastUpdateSpeed = updatesPerSec;
        }
    }

    /* Does the button have a range of values? */
    else if (op->op_range)
    {
        if (data->key == W_RBUTTON)
        {
            (*op->op_option) += op->op_range->increment;
        }
        else if (data->key == W_MBUTTON)
        {
            (*op->op_option) = op->op_range->min_value;
        }
        else if (data->key == W_LBUTTON)
        {
            (*op->op_option) -= op->op_range->increment;
        }
        /* wrap value around within option range */
        if (*(op->op_option) > op->op_range->max_value)
            *(op->op_option) = op->op_range->min_value;
        if (*(op->op_option) < op->op_range->min_value)
            *(op->op_option) = op->op_range->max_value;
    }


    /* Is the option a toggle? */

    else if (op->op_option)
    {
        *op->op_option = !*op->op_option;

        /* Actions to be taken when certain options are selected.
         * (Yes, this is a hack). */

        if (op->op_option == &showPlanetOwner || op->op_option == &planetHighlighting)
            redrawall = 1;
#ifdef HOCKEY_LINES
        else if (op->op_option == &showHockeyLinesMap)
        {
            // We definitely don't want to be able to
            // change this option on non hockey server
            if (showHockeyLinesMap && !playing_hockey)
                showHockeyLinesMap = 0;
        }
        else if (op->op_option == &showHockeyLinesLocal)
        {
            // same as above
            if (showHockeyLinesLocal && !playing_hockey)
                showHockeyLinesLocal = 0;
        }
        else if (op->op_option == &showHockeyScore)
        {
            // same as above
            if (showHockeyScore && !playing_hockey)
                showHockeyScore = 0;
        }
        else if (op->op_option == &puckCircle)
        {
            // same as above
            if (puckCircle && !playing_hockey)
                puckCircle = 0;
        }
#endif
        /* Let's see if this is our double buffering changed */
        else if (op->op_option == &doubleBuffering)
        {
            W_ClearWindow (w);
            W_ClearWindow (mapw);
            redrawall = 1;
        }
        else if (op->op_option == &partitionPlist)
            RedrawPlayerList ();
        else if (op->op_option == &sortPlayers)
            RedrawPlayerList ();
        else if (op->op_option == &sortMyTeamFirst)
            RedrawPlayerList ();
        else if (op->op_option == &saveOpts)
        {
            saveOptions ();
            saveOpts = 1;
        }
    }

    /* Map/unmap window, if it exists */

    if (op->op_targetwin)
    {
        if (W_IsMapped (*op->op_targetwin))
            W_UnmapWindow (*op->op_targetwin);
        else
        {
            W_MapWindow (*op->op_targetwin);

            if (*op->op_targetwin == udpWin)
                udpwindow ();
            if (*op->op_targetwin == pStats)
                redrawPStats ();
            if (*op->op_targetwin == spWin)
                spwindow ();

#ifdef XTREKRC_HELP
            if (defWin && *op->op_targetwin == defWin)
                showdef ();
#endif

#ifdef SOUND
            if (*op->op_targetwin == soundWin)
                soundwindow ();
#endif

#ifdef DOC_WIN
            if (docwin && *op->op_targetwin == docwin)
                showdocs (0);

            if (xtrekrcwin && *op->op_targetwin == xtrekrcwin)
                showxtrekrc (0);
#endif
        }
    }

    /* deal with possible menu change */
    if (MenuPage != CurrentMenu->page_num)
    {
        SetMenuPage (MenuPage);
        RefreshOptions ();
    }
    else if (notdone)
        optionrefresh (op);
    else
    {
        optionrefresh (op);
        optiondone ();
    }

    return;
}

/* find the menu in the menus linked list that matches the one in the *
 * argument */
void
SetMenuPage (int pagenum)
{
    int i = 1;

    if (FirstMenu != NULL)
        for (CurrentMenu = FirstMenu; CurrentMenu->Next != NULL &&
             CurrentMenu->page_num != pagenum;
             i++, CurrentMenu = CurrentMenu->Next);

    W_ResizeMenuToNumItems (optionWin, CurrentMenu->numopt);
}

void
optiondone (void)
{
    int shpn;
    struct ship *shipp;

    /* Unmap window */
    W_UnmapWindow (optionWin);

    /* update default, myship, and individual ship keymaps/buttonmaps */
    keymapAdd(newkeys, (char*) default_keymap);
    buttonmapAdd(newbuttons, (char*) default_buttonmap);
    keymapAdd(newkeys, (char*) myship->s_keymap);
    buttonmapAdd(newbuttons, (char*) myship->s_buttonmap);
    for (shpn = 0; shpn < nshiptypes; shpn++)
    {
        shipp = getship(shpn);
        keymapAdd(newkeys, (char*) shipp->s_keymap);
        buttonmapAdd(newbuttons, (char*) shipp->s_buttonmap);
    }
    *newkeys = '\0';
    *newbuttons = '\0';
    
    /* update login */
    if (newlogin[0] != '\0')
    {
        STRNCPY (login, newlogin, PSEUDOSIZE);
        *newlogin = '\0';
    }

    sendOptionsPacket ();       /* update server as to the
                                 * client's options */
}

/* set up menus linked list */
int
InitOptionMenus (void)
{
    int i = 1;
    int maxopts = 0;


    /* AddOptMenu( &OptionsMenu, 0); */
    AddOptMenu (Control_Menu, 0);
    AddOptMenu (Ship_Menu, 0);
    AddOptMenu (Weapons_Menu, 0);
    AddOptMenu (Planet_Menu, 0);
    AddOptMenu (Visual_Menu, 0);
    AddOptMenu (Window_Menu, 0);
    AddOptMenu (Playerlist_Menu, 0);
    AddOptMenu (Hockey_Menu, 0);
    AddOptMenu (Save_Menu, 0);


    for (i = 0, CurrentMenu = FirstMenu; CurrentMenu != NULL;
         i++, CurrentMenu = CurrentMenu->Next)
    {
        CurrentMenu->page_num = i;      /* repage the menus.. */
        if (CurrentMenu->numopt > maxopts)
            maxopts = CurrentMenu->numopt;
    }
    CurrentMenu = FirstMenu;
    Menus_Range.max_value = i - 1;
    return maxopts;
}

void
AddOptMenu (struct option *NewMenu,
            int updated)
{
    struct option_menu *menuptr;
    struct option_menu *newmenu;
    int i = 0;


    menuptr = FirstMenu;

    newmenu = (struct option_menu *) malloc (sizeof (struct option_menu));

    if (newmenu == NULL)
    {
        perror ("Malloc Error adding a menu");
        return;
    }

    /* add to list */
    if (FirstMenu == NULL)
    {
        FirstMenu = newmenu;
    }
    else
    {
        for (i = 0, menuptr = FirstMenu; menuptr->Next != NULL;
             menuptr = menuptr->Next)
            i++;
        menuptr->Next = newmenu;
    }
    newmenu->page_num = i;
    newmenu->Next = NULL;
    newmenu->numopt = NumOptions (NewMenu);
    newmenu->menu = NewMenu;
    newmenu->updated = updated;
}

int
NumOptions (struct option *OpMenu)
{
    int i = 0;
    struct option *ptr;

    for (ptr = OpMenu, i = 0; ptr->op_num != -1; i++, ++ptr)
    {
        ptr->op_num = i;
    }


    return i;
}

/* a function that could be called regularly, to deal with menus that * might
 * be updated by external events. I.e. the udp menu! */
void
UpdateOptions (void)
{
    if (notdone == 0)
        return;                 /* don't update if menu
                                 * isn't in use */
    if (CurrentMenu->updated)
        RefreshOptions ();
}
