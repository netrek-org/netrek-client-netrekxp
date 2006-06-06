/* data.h */
#include "copyright.h"
#include <time.h>

#ifndef _h_data
#define _h_data

#define MAP_PIX   0
#define LOCAL_PIX 1
#define GHOST_PIX 2
#define GENO_PIX  3
#define GREET_PIX 4

/* Bitmap defintions */
#include "bitmaps.h"

#define crossmask_width         16
#define crossmask_height        16
#define shield_width            20
#define shield_height           20
#define cloak_width             20
#define cloak_height            20

#define etorp_width             3
#define etorp_height            3
#define eplasmatorp_width       7
#define eplasmatorp_height      7
#define mplasmatorp_width       5
#define mplasmatorp_height      5
#define mtorp_width             3
#define mtorp_height            3

extern struct player *players;
extern struct player *me;
extern struct torp *torps;
extern struct plasmatorp *plasmatorps;
extern struct status *status;
extern struct ship *myship;
extern struct stats *mystats;
extern struct planet *planets;
extern struct phaser *phasers;
extern struct message *messages;
extern struct mctl *mctl;
extern struct team *teams;
extern struct ship shipvals[];
extern struct memory universe;
extern struct planet pdata[];

extern unsigned int oldalert;
extern int alertBorderColor;
extern int remap[];
extern int udcounter;
extern char *title;
extern struct plupdate pl_update[];
extern char buttonmap[];
extern int messpend;
#ifdef XTRA_MESSAGE_UI
extern int messageHUD;          /* Show message being typed on the local display */
extern int messageHoldThresh;      /* Threshold value for putting a message on hold */
extern int messMouseDelta;      /* To keep track of mouse movement delta        */
#endif
extern int lastcount;
extern int mdisplayed;
extern int lastm;
extern time_t delay;
extern time_t rdelay;
extern time_t tdelay;
extern int showPlanetNames;
extern int warnShields;
extern int showStats;
extern int showHints;
#ifdef RECORDGAME
extern FILE *recordFile;        /* recorder */
extern int inplayback;
extern int playback;
#define PL_PAUSE 1
#define PL_FORWARD 2
#define PL_REVERSE 3
#endif
extern FILE *logFile;           /* message log */
extern int msgBeep;             /* ATM - msg beep */
extern int warncount;
extern int warntimer;
extern int infomapped;
extern int mustexit;
extern int messtime;
extern int keepPeace;
extern int gen_distress;

#ifdef GATEWAY
extern unsigned LONG netaddr;   /* for blessing */

#endif

extern int messageon;

#ifdef RSA
extern char testdata[];
extern int useRsa;
extern char key_name[];
extern char client_type[];
extern char client_arch[];
extern char client_creator[];
extern char client_comments[];
extern char client_key_date[];

#endif

#ifdef META
extern char *metaServer[];
extern int metaPort;
extern char *metaCache;
#ifdef METAPING
extern int metaPing;
#endif
#endif

#define NUM_DIST 27        /* Number of distress macros */

#ifdef NBT
extern int MacroMode;
extern int macrocnt;
extern struct macro_list macro[];       /* NBT 2/26/93 */

#endif

#ifdef ROTATERACE
extern int rotate;
extern int rotate_deg;

#endif

extern int updatesPerSec;

extern int SBhours;

extern int showMySpeed;

#ifdef JUBILEE_PHASERS
extern int colorfulPhasers;
#endif

#ifdef SHORT_PACKETS
extern int why_dead;
extern int tryShort, tryShort1;
extern int recv_short;
extern int recv_mesg;
extern int recv_kmesg;
extern int recv_threshold;
extern char recv_threshold_s[];
extern int recv_warn;

/* S_P2 */
extern int shortversion;        /* Which version do we use? */

#endif

extern int ingame;              /* If player is in game - to distinguish between whether
                                   to use double buffering on the local and map window */
extern int ghoststart;
extern int ghost_pno;
extern int keepInfo;
extern int showPlanetOwner;
extern int phaserShrink;
extern int phaserShrinkStyle;
extern int theirPhaserShrink;
extern int shrinkPhaserOnMiss;
extern int colorClient;
extern int dynamicBitmaps;
extern int colorWeapons;
extern int newDashboard, old_db;
extern int detCircle;
extern int showArmy;
extern int niftyNewMessages;
extern int fastQuit;
extern char *shipnos;
extern int sock;
extern int xtrekPort;
extern int queuePos;
extern int pickOk;
extern int lastRank;
extern int promoted;
extern int loginAccept;
extern unsigned localflags;
extern int tournMask;
extern int nextSocket;
extern char *serverName;
extern char *serverNick;
extern char defaultsFile[80];
extern int loggedIn;
extern int reinitPlanets;
extern int lastUpdate[];
extern int timerDelay;
extern int redrawDelay;
extern int reportKills;
extern int phaserWindow;
#ifdef PHASER_STATS
extern int phaserStats;
extern int phaserStatTry;
extern int phaserStatHit;
extern unsigned long phaserStatDamage;
#endif

extern int scanplayer;
extern int commMode;            /* UDP */
extern int commModeReq;         /* UDP */
extern int commStatus;          /* UDP */
extern int commSwitchTimeout;   /* UDP */
extern int udpTotal;            /* UDP */
extern int udpDropped;          /* UDP */
extern int udpRecentDropped;    /* UDP */
extern int udpSock;             /* UDP */
extern int udpDebug;            /* UDP */
extern int udpClientSend;       /* UDP */
extern int udpClientReceive;    /* UDP */
extern int udpSequenceCheck;    /* UDP */
extern int weaponUpdate;

#ifdef GATEWAY
extern int gw_serv_port, gw_port, gw_local_port;        /* UDP */
extern char *gw_mach;           /* UDP */

#endif
extern int baseUdpLocalPort;    /* UDP */


extern int showTractorPressor;
extern int showAllTractorPressor;
extern int showLock;
extern int planetBitmap;
extern int planetBitmapGalaxy;
extern int planetHighlighting;
extern int rotatePlanets;
extern int logging;
extern int continueTractor;
extern int tcounter;
extern int autoKey;
extern int extraAlertBorder;

/* udp options */
extern int tryUdp, tryUdp1;

extern int debug;

extern double Sin[], Cos[];

extern W_Icon stipple, clockpic, clockhandpic, clockhandmask, genopic;

extern W_Icon base_expview;
extern W_Icon expview[BMP_SHIPEXPL_FRAMES];
extern W_Icon base_sbexpview;
extern W_Icon sbexpview[BMP_SBEXPL_FRAMES];

extern W_Icon torp_cloud_bitmaps;
extern W_Icon cloudC[NUM_CTORP_TYPES][BMP_TORPDET_FRAMES];
extern W_Icon plasma_cloud_bitmaps;
extern W_Icon plcloudC[NUM_CTORP_TYPES][BMP_TORPDET_FRAMES];
extern W_Icon mtorp_bitmaps;
extern W_Icon mtorpC[NUM_CTORP_TYPES][BMP_TORP_FRAMES];
extern W_Icon torp_bitmaps;
extern W_Icon torpC[NUM_CTORP_TYPES][BMP_TORP_FRAMES];
extern W_Icon mplasma_bitmaps;
extern W_Icon mplasmaC[NUM_CTORP_TYPES][BMP_TORP_FRAMES];
extern W_Icon plasma_bitmaps;
extern W_Icon plasmaC[NUM_CTORP_TYPES][BMP_TORP_FRAMES];

extern W_Icon cloud[BMP_TORPDET_FRAMES];
extern W_Icon plasmacloud[BMP_TORPDET_FRAMES];
extern W_Icon etorp, mtorp;
extern W_Icon eplasmatorp, mplasmatorp;

#ifdef VSHIELD_BITMAPS
#define SHIELD_FRAMES 5
extern W_Icon base_vshield;
extern W_Icon shield[SHIELD_FRAMES], cloakicon;
extern int varyShields;

#else
extern W_Icon shield, cloakicon;

#endif

extern W_Icon tractbits, pressbits;

//Ships
extern W_Icon ship_bitmaps[5];
extern W_Icon fed_bitmaps[NUM_TYPES][SHIP_VIEWS],
    kli_bitmaps[NUM_TYPES][SHIP_VIEWS], rom_bitmaps[NUM_TYPES][SHIP_VIEWS],
    ori_bitmaps[NUM_TYPES][SHIP_VIEWS], ind_bitmaps[NUM_TYPES][SHIP_VIEWS];

extern W_Icon ship_bitmaps1[5];
extern W_Icon fed_bitmaps1[NUM_TYPES][SHIP_VIEWS],
    kli_bitmaps1[NUM_TYPES][SHIP_VIEWS], rom_bitmaps1[NUM_TYPES][SHIP_VIEWS],
    ori_bitmaps1[NUM_TYPES][SHIP_VIEWS], ind_bitmaps1[NUM_TYPES][SHIP_VIEWS];

extern W_Icon ship_bitmapsG[5];
extern W_Icon fed_bitmapsG[NUM_TYPES][SHIP_VIEWS],
    kli_bitmapsG[NUM_TYPES][SHIP_VIEWS], rom_bitmapsG[NUM_TYPES][SHIP_VIEWS],
    ori_bitmapsG[NUM_TYPES][SHIP_VIEWS], ind_bitmapsG[NUM_TYPES][SHIP_VIEWS];

extern W_Icon ship_bitmapsM[5];
extern W_Icon fed_bitmapsM[NUM_TYPES][SHIP_VIEWS],
    kli_bitmapsM[NUM_TYPES][SHIP_VIEWS], rom_bitmapsM[NUM_TYPES][SHIP_VIEWS],
    ori_bitmapsM[NUM_TYPES][SHIP_VIEWS], ind_bitmapsM[NUM_TYPES][SHIP_VIEWS];
    
extern W_Icon ship_bitmapsT[5];
extern W_Icon fed_bitmapsT[NUM_TYPES][SHIP_VIEWS],
    kli_bitmapsT[NUM_TYPES][SHIP_VIEWS], rom_bitmapsT[NUM_TYPES][SHIP_VIEWS],
    ori_bitmapsT[NUM_TYPES][SHIP_VIEWS], ind_bitmapsT[NUM_TYPES][SHIP_VIEWS];

extern W_Icon ship_bitmapsHR[5];
extern W_Icon fed_bitmapsHR[NUM_TYPES],
    kli_bitmapsHR[NUM_TYPES], rom_bitmapsHR[NUM_TYPES],
    ori_bitmapsHR[NUM_TYPES], ind_bitmapsHR[NUM_TYPES];

// Planets
extern W_Icon planet_unknown_bitmap;
extern W_Icon planet_unknown[CPLANET_VIEWS];
extern W_Icon planet_unknown_NR;
extern W_Icon mplanet_unknown;
extern W_Icon planet_bitmaps[8], mplanet_bitmaps[8];
extern W_Icon planet_earth[NUMTEAMS], planet_klingus[NUMTEAMS], planet_orion[NUMTEAMS],
    planet_romulus[NUMTEAMS], planet_agri1[NUMTEAMS], planet_agri2[NUMTEAMS],
    planet_rock1[NUMTEAMS], planet_rock2[NUMTEAMS];
extern W_Icon mplanet_earth[NUMTEAMS], mplanet_klingus[NUMTEAMS], mplanet_orion[NUMTEAMS],
    mplanet_romulus[NUMTEAMS], mplanet_agri1[NUMTEAMS], mplanet_agri2[NUMTEAMS],
    mplanet_rock1[NUMTEAMS], mplanet_rock2[NUMTEAMS];
extern W_Icon army_bitmap, marmy_bitmap;
extern W_Icon wrench_bitmap, mwrench_bitmap;
extern W_Icon fuel_bitmap, mfuel_bitmap;
extern W_Icon base_planets;
extern W_Icon base_mplanets;
extern W_Icon bplanets[PLANET_VIEWS];
extern W_Icon bmplanets[MPLANET_VIEWS];
extern W_Color borderColor, backColor, textColor, myColor, warningColor,
    shipCol[5], rColor, yColor, gColor, unColor, foreColor;

/* jn - SMARTMACRO */
extern char lastMessage[];
extern int MacroNum;
extern char *classes[];
extern char teamlet[];
extern char *teamshort[];
extern char pseudo[PSEUDOSIZE];
extern char defpasswd[PSEUDOSIZE];
extern char login[PSEUDOSIZE];

extern struct rank ranks[NUMRANKS];

extern W_Window messagew, w, mapw, statwin, baseWin, infow, tstatw,
    war, warnw, helpWin, teamWin[4], qwin, messwa, messwt, messwi,
    messwk, planetw, rankw, playerw, playerw2, optionWin, reviewWin;
extern W_Window udpWin, phaserwin, hintWin;
extern W_Window waitWin, waitqWin, countWin, motdButtonWin, motdWin;

#ifdef SHORT_PACKETS
extern W_Window spWin;

#endif

#ifdef NBT
extern W_Window macroWin;

#endif

#ifdef META
extern W_Window metaWin;

#endif

extern int ping;                /* to ping or not to ping */
extern LONG packets_sent;       /* # all packets sent to
                                 * server */
extern LONG packets_received;   /* # all packets received */
extern W_Window pStats;

extern char rankmessage[80];
extern char deathmessage[80];
extern char outmessage[];
extern char *xdisplay_name;

extern int newDistress;
extern int UseNewMacro;
extern int UseSmartMacro;
extern int rejectMacro;
extern int maskrecip;
extern unsigned char singleMacro[MAX_MACRO];


extern int enemyPhasers;

extern char cloakChars[3];

extern int showIND;
extern int newPlist;
extern int playerListHack;


extern struct dmacro_list *distmacro;
extern struct dmacro_list dist_defaults[];
extern struct dmacro_list dist_prefered[];
extern int sizedist;

#ifdef BEEPLITE
extern char *distlite[];
extern int useLite;
extern int defLite;
extern int emph_planet_seq_n[];
extern int emph_player_seq_n[];
extern W_Icon emph_planet_seq[];
extern W_Icon emph_player_seq[];
extern W_Icon emph_player_seql[];

#define emph_planet_seq_frames 5
#define emph_planet_seq_width 24
#define emph_planet_seq_height 24
#define emph_player_seq_frames 3
#define emph_player_seq_width 24
#define emph_player_seq_height 24
#define emph_player_seql_frames 3
#define emph_player_seql_width 30
#define emph_player_seql_height 30
extern int beep_lite_cycle_time_player;
extern int beep_lite_cycle_time_planet;
extern int liteflag;
extern char F_beeplite_flags;

extern int tts_len;
extern int tts_max_len;
extern int last_tts_xpos;
extern int last_tts_width;
extern int tts_timer;
extern int tts_time;
extern int tts_ypos;
extern char lastIn[100];

#endif /* BEEPLITE */

#ifdef RCM
extern struct dmacro_list rcm_msg[];

#endif

extern int highlightFriendlyPhasers;

#ifdef IGNORE_SIGNALS_SEGV_BUS
/* Handle (hopefully) non-fatal segmentation and bus faults. */
extern int ignore_signals;

#endif

#ifdef MOTION_MOUSE
extern int continuousMouse;
extern int continuousMouseFix;
extern int motionThresh;
extern int motion_mouse_enablable;
extern int motion_mouse_steering;

#endif

#ifdef SHIFTED_MOUSE
extern int shiftedMouse;

#endif

extern int ignoreCaps;

#ifdef MOUSE_AS_SHIFT
extern int mouseAsShift;
extern int b1_as_shift;
extern int b2_as_shift;
extern int b3_as_shift;
extern int b4_as_shift;
extern int b5_as_shift;

#endif

#ifdef XTREKRC_HELP
extern W_Window defWin;

#endif

#ifdef CONTROL_KEY
extern int use_control_key;

#endif

#ifdef DOC_WIN
extern W_Window docwin, xtrekrcwin;
extern int maxdoclines, maxxtrekrclines;

#endif

#ifdef REFRESH_INTERVAL
extern int refresh_interval;

#endif

#ifdef BRMH
extern int max_fd;

#endif

#ifdef TOOLS
extern W_Window toolsWin;
extern int shellTools;
extern struct key_list macroKeys[MAX_KEY];
extern unsigned char keys[MAX_KEY];

#endif

#ifdef SOUND
#include "sound.h"
extern int sound_init;
extern int sound_toggle;
extern char *sounddir;
extern W_Window soundWin;

#endif

#ifdef HOCKEY_LINES
extern struct s_line local_hockey_lines[NUM_HOCKEY_LINES + 1];
extern struct s_line map_hockey_lines[NUM_HOCKEY_LINES + 1];
extern int showHockeyLinesLocal;
extern int showHockeyLinesMap;

#endif

#ifdef MULTILINE_MACROS
extern int multiline_enabled;

#endif

#ifdef FEATURE_PACKETS
extern int F_feature_packets;

#endif

extern int F_cloak_maxwarp;
extern int F_self_8flags;
extern int F_self_8flags2;
extern int F_self_19flags;
extern int F_ship_cap;
extern int F_show_all_tractors;

#ifdef RECORDGAME
extern int F_many_self;
#endif

#ifdef WARP_DEAD
extern int F_dead_warp;
#endif

#ifdef UDP_PORTSWAP
extern int portSwap;
#endif

// Load Ship Bitmaps from .BMP files
extern char *fed_ship_bmp;
extern char *rom_ship_bmp;
extern char *kli_ship_bmp;
extern char *ori_ship_bmp;
extern char *ind_ship_bmp;
extern char *fed_ship_bmp_1;
extern char *rom_ship_bmp_1;
extern char *kli_ship_bmp_1;
extern char *ori_ship_bmp_1;
extern char *ind_ship_bmp_1;
extern char *fed_ship_bmp_G;
extern char *rom_ship_bmp_G;
extern char *kli_ship_bmp_G;
extern char *ori_ship_bmp_G;
extern char *ind_ship_bmp_G;
extern char *fed_ship_bmp_T;
extern char *rom_ship_bmp_T;
extern char *kli_ship_bmp_T;
extern char *ori_ship_bmp_T;
extern char *ind_ship_bmp_T;
extern char *fed_ship_bmp_M;
extern char *rom_ship_bmp_M;
extern char *kli_ship_bmp_M;
extern char *ori_ship_bmp_M;
extern char *ind_ship_bmp_M;
extern char *fed_ship_bmp_HR;
extern char *rom_ship_bmp_HR;
extern char *kli_ship_bmp_HR;
extern char *ori_ship_bmp_HR;
extern char *ind_ship_bmp_HR;

// Added to fix thread creation issue
HANDLE MainThread;
DWORD MainThreadID;
HANDLE InputThread;
DWORD InputThreadID;

// Missing global variables
extern int autoQuit;

extern int forceDisplay;
extern int forceMono;
extern int metaStatusLevel;
extern int mungScrollbarColors;
extern int showMotd;

extern char * saveFile;  /* file name to save options to */
extern int saveBig;     /* save options with comments */
extern int saveWindow;  /* save options with window placements */
extern int saveMacro;   /* save options with macros */
extern int saveRCD;     /* save options with RCD */
#ifdef BEEPLITE
extern int saveBeeplite; /* save options with Beeplite */
#endif
#ifdef RCM
extern int saveRCM;     /* save options with RCM */
#endif

extern int tpDotDist;   /* distance between tractor/pressor dots */

extern struct tractor *tractcurrent;
extern struct tractor *tractrunner;
extern struct tractor *tracthead;

extern int agriCAPS;    /* show agri names in caps on map */
extern int agriColor;   /* agri planets font color on map */

extern int windowMove;          /* allow moving of internal windows */
extern int mainResizeable;      /* allow resize of netrek window */
extern int playerListMessaging; /* allow message zoom when clicking on player list */
extern int observerMode;        /* set observer mode variable */
extern int observerPorts[];     /* predefined list of observer ports */
extern int showHockeyScore;     /* show hockey score on map */

extern char pigcall[];  /* pigcall string */
extern char cowid[];    /* cowid string */

extern int serverType;              /* server type variable */
extern int beepOnPrivateMessage;    /* beep if private message was received */
extern W_Window wam_windows[];      /* window allowed messages types */

extern int showStars;   /* show background stars on local */
extern int warpStreaks; /* show warp streaks */

/* time client connected to server */
extern time_t timeStart;    /* time when client connected to server */

/* timer data */                   
extern int timerType;       /* type of dashboard timer */
extern time_t timeBank[];   /* different kind of times to store for different timer types */

extern int omitTeamLetter;  /* don't show team letter on map */

extern int viewBox;     /* show local window box on map */

extern struct stringlist * defaults; /* pointer to defaults list */

extern SDBUFFER * localSDB;    /* double buffer for local window */
extern SDBUFFER * mapSDB;      /* double buffer for map window */

extern int doubleBuffering; /* Use double buffering to draw map/local */

extern int disableWinkey;   /* disable WinKey + ContextKey during the game */

extern int exitFlag;        /* indicates exit state for W_WaitForEvent() */

extern struct cons_buffer * consHead;    /* head of the linked list for console buffer */
extern struct cons_buffer * consTail;    /* tail of the linked list for console buffer */

extern int allowWheelActions;    /* allow mouse wheel actions */

extern WNDPROC lpfnDefRichEditWndProc;  /* default window procedure for our richedit controls */

extern int richText;    /* temporary variable to enable/disable rich text windows */
extern int newQuit;     /* new quit clock */
extern int newSound;	/* Use new SDL sound interface */
extern int newSoundAngles; /* Use SDL with angular 3D sound */
#endif /* _h_data */
