
/***  File:  input.c                                                        ***/
/***                                                                        ***/
/***  Function:  Handles keyboard input.                                    ***/
/***                                                                        ***/
/***  Revisions:                                                            ***/
/***    27Aug00  - Ssheldon - added function comment headers                ***/
/***                                                                        ***/
/******************************************************************************/

#include <setjmp.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include "config.h"
#include "copyright.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "packets.h"
#include "playerlist.h"
#include "spopt.h"
#include "map.h"
#include "proto.h"

#ifdef __BORLANDC__
#ifndef CYGWIN
//   #pragma warn -rvl    //Turon off "parameter never used..." warning
//   #pragma warn -pro    //Turon off "parameter never used..." warning
#endif
#endif

int detallow = 1;               /* flag used to figure out
                                 * if we're allowing det */

#ifdef DOC_WIN
static int docline = 0;
static int xtrekrcline = 0;
#endif

int opened_info = -2;           /* counter for infowin
                                 * popup, 6/1/93 LAB */

#ifdef THREADED
extern jmp_buf env;
#endif

struct obtype *target;
unsigned char key = ' ';

void phaseraction (W_Event * data);

/* this used to be 177 for an unknown reason...I think it may * have included
 * various control characters.  We don't support * those anyway right?? - jn */
#define MAXKEY 224
#define MAXASCII 128

/* this method of avoiding a massive switch should represent * massive
 * performance gains... instead of having to test * n/2 times for n different
 * keys, the key is run directly * via an array of input functions whose
 * index happens * to coorespond to the value of input char. - jn */
/* SRS - moved prototypes for Key??? functions to proto.h */

typedef struct
{
    void (*handler) ();
}
key_handler_type;

key_handler_type key_handlers[MAXKEY] = {
    emptyKey,                   /* \0 */
    emptyKey,                   /* 1 */
    emptyKey,                   /* 2 */
    emptyKey,                   /* 3 */
    emptyKey,                   /* 4 */
    emptyKey,                   /* 5 */
    emptyKey,                   /* 6 */
    emptyKey,                   /* 7 */
    emptyKey,                   /* 8 */
    emptyKey,                   /* 9 */
    emptyKey,                   /* 10 */
    emptyKey,                   /* 11 */
    emptyKey,                   /* 12 */
    emptyKey,                   /* 13 */
    emptyKey,                   /* 14 */
    emptyKey,                   /* 15 */
    emptyKey,                   /* 16 */
    emptyKey,                   /* 17 */
    emptyKey,                   /* 18 */
    emptyKey,                   /* 19 */
    emptyKey,                   /* 20 */
    emptyKey,                   /* 21 */
    emptyKey,                   /* 22 */
    emptyKey,                   /* 23 */
    emptyKey,                   /* 24 */
    emptyKey,                   /* 25 */
    emptyKey,                   /* 26 */
    emptyKey,                   /* 27 */
    emptyKey,                   /* 28 */
    emptyKey,                   /* 29 */
    emptyKey,                   /* 30 */
    emptyKey,                   /* 31 */
    Key32,                      /* space */
    Key33,                      /* ! */
    Key34,                      /* " */
    Key35,                      /* # */
    Key36,                      /* $ */
    Key37,                      /* % */
    Key38,                      /* & */
    Key39,                      /* ' */
    Key40,                      /* ( */
    Key41,                      /* ) */
    Key42,                      /* * */
    Key43,                      /* + */
    Key44,                      /* , */
    Key45,                      /* - */
    Key46,                      /* . */
    Key47,                      /* / */
    Key48,                      /* 0 */
    Key49,                      /* 1 */
    Key50,                      /* 2 */
    Key51,                      /* 3 */
    Key52,                      /* 4 */
    Key53,                      /* 5 */
    Key54,                      /* 6 */
    Key55,                      /* 7 */
    Key56,                      /* 8 */
    Key57,                      /* 9 */
    Key58,                      /* : */
    Key59,                      /* ; */
    Key60,                      /* < */
    Key61,                      /* = */
    Key62,                      /* > */
    Key63,                      /* ?  -  you know this is
                                 * boring as hell... */
    Key64,                      /* @ */
    Key65,                      /* A */
    Key66,                      /* B */
    Key67,                      /* C */
    Key68,                      /* D */
    Key69,                      /* E */
    Key70,                      /* F */
    Key71,                      /* G */
    Key72,                      /* H */
    Key73,                      /* I */
    Key74,                      /* J */
    Key75,                      /* K */
    Key76,                      /* L */
    Key77,                      /* M */
    Key78,                      /* N */
    Key79,                      /* O */
    Key80,                      /* P */
    Key81,                      /* Q */
    Key82,                      /* R */
    Key83,                      /* S */
    Key84,                      /* T */
    Key85,                      /* U */
    Key86,                      /* V */
    Key87,                      /* W */
    Key88,                      /* X */
    Key89,                      /* Y */
    Key90,                      /* Z */
    Key91,                      /* [ */
    Key92,                      /* \ */
    Key93,                      /* ]  -  ascii is fucked... */
    Key94,                      /* ^ */
    Key95,                      /* _ */
    Key96,                      /* ` */
    Key97,                      /* a */
    Key98,                      /* b */
    Key99,                      /* c */
    Key100,                     /* d */
    Key101,                     /* e */
    Key102,                     /* f */
    Key103,                     /* g */
    Key104,                     /* h */
    Key105,                     /* i */
    Key106,                     /* j */
    Key107,                     /* k */
    Key108,                     /* l */
    Key109,                     /* m */
    Key110,                     /* n */
    Key111,                     /* o */
    Key112,                     /* p */
    Key113,                     /* q */
    Key114,                     /* r */
    Key115,                     /* s */
    Key116,                     /* t */
    Key117,                     /* u */
    Key118,                     /* v */
    Key119,                     /* w */
    Key120,                     /* x */
    Key121,                     /* y */
    Key122,                     /* z */
    Key123,                     /* { */
    Key124,                     /* | */
    Key125,                     /* } - my wife was once
                                 * bitten by a lhama */
    Key126,                     /* ~ */
    Key127,                     /* delete */
    emptyKey,                   /* 128 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    Key131,                     /* ^# */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    Key144,                     /* ^0 */
    Key145,                     /* ^1 */
    Key146,                     /* ^2 */
    Key147,                     /* ^3 */
    Key148,                     /* ^4 */
    Key149,                     /* ^5 */
    Key150,                     /* ^6 */
    Key151,                     /* ^7 */
    Key152,                     /* ^8 */
    Key153,                     /* ^9 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    Key160,                     /* ^@ */
    emptyKey,                   /* 31 */
    Key162,                     /* ^B */
    Key163,                     /* ^C */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    Key175,                     /* ^O */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    Key180,                     /* ^T */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    Key187,                     /* ^[ */
    emptyKey,                   /* 31 */
    Key189,                     /* ^] */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    Key193,                     /* ^a */
    Key194,                     /* ^b */
    Key195,                     /* ^c */
    emptyKey,                   /* 31 */
    Key197,                     /* ^e */
    Key198,                     /* ^f */
    emptyKey,                   /* 31 */
    Key200,                     /* ^h */
    emptyKey,                   /* 31 */
    Key202,                     /* ^j */
    emptyKey,                   /* 31 */
    Key204,                     /* ^l */
    Key205,                     /* ^m */
    Key206,                     /* ^n */
    Key207,                     /* ^o */
    Key208,                     /* ^p */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    Key212,                     /* ^t */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    Key218,                     /* ^z */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 31 */
    emptyKey,                   /* 223 - No llamas were hurt \
                                   in the making of this code */
};

/******************************************************************************/
/***  lockBase() - for use with twarp key                                   ***/
/******************************************************************************/
static void
lockBase (void)
{
    register int i;
    register struct player *j;
    register int targnum = -1;

    for (i = 0, j = &players[i]; i < nplayers; i++, j++)
    {
        if (j->p_status != PALIVE)
            continue;
        if (j == me)
            continue;
        if ((j->p_ship.s_type == STARBASE) && (j->p_team == me->p_team))
            targnum = i;
    }
    if (targnum != -1)
    {
        sendPlaylockReq (targnum);
        me->p_playerl = (short) targnum;
    }
}

/******************************************************************************/
/***  lockPlanetOrBase()                                                    ***/
/******************************************************************************/
static void
lockPlanetOrBase (W_Window ww,
                  int x,
                  int y)        /* special version of
                                 * gettarget, 6/1/93 LAB */
{
    register int i;
    register struct player *j;
    register struct planet *k;
    int g_x, g_y;
    double dist, closedist;
    register int targtyp = 0, targnum = -1;

    if (ww == mapw)
    {
        g_x = x * GWIDTH / GWINSIDE;
        g_y = y * GWIDTH / GWINSIDE;
    }
    else
    {
        g_x = me->p_x + ((x - TWINSIDE / 2) * scaleFactor);
        g_y = me->p_y + ((y - TWINSIDE / 2) * scaleFactor);
    }
    closedist = GWIDTH;

    for (i = 0, k = &planets[i]; i < nplanets; i++, k++)
    {
        dist = hypot ((double) (g_x - k->pl_x), (double) (g_y - k->pl_y));
        if (dist < closedist)
        {
            targtyp = PLANETTYPE;
            targnum = i;
            closedist = dist;
        }
    }

    for (i = 0, j = &players[i]; i < nplayers; i++, j++)
    {
        if (j->p_status != PALIVE)
            continue;
        if (j->p_flags & PFCLOAK)
            continue;
        if (j == me)
            continue;
        if ((j->p_ship.s_type == STARBASE) && (j->p_team == me->p_team))
        {
            dist = hypot ((double) (g_x - j->p_x), (double) (g_y - j->p_y));
            if (dist < closedist)
            {
                targtyp = PLAYERTYPE;
                targnum = i;
                closedist = dist;
            }
        }
    }

    if (targtyp == PLAYERTYPE)
    {
        sendPlaylockReq (targnum);
        me->p_playerl = (short) targnum;
    }
    else
    {
        sendPlanlockReq (targnum);
        me->p_planet = (short) targnum;
    }

}

/******************************************************************************/
/***  getctrlkey()                                                          ***/
/******************************************************************************/
unsigned char
getctrlkey (unsigned char **s)
{
    unsigned char *str = *s;
    unsigned char c;

    /* Character is control key. */
    if (*str == '^')
    {
        str++;

        /* check for '^' key being specified with "^^" */
        if (*str != '^')
            c = (unsigned char) (*str + 96);
        else
            c = *str;
    }
    else
        c = *str;
    str++;
    *s = str;
    return (c);
}


/******************************************************************************/
/***  getkeyfromctrl()                                                      ***/
/******************************************************************************/
unsigned char
getkeyfromctrl (unsigned char c)
{
    /* Character is control key. */
    if (c == '^' || (c >= 32 && c <= 127))
        return c;
    else
        return (unsigned char) (c - 96);
}

/******************************************************************************/
/***  initkeymap() - sets up default keymaps and buttonmaps, then builds the
      ship specific versions                                                ***/
/******************************************************************************/
void
initkeymap (void)
{
    int j;
    unsigned char *str;

    if ((str = (unsigned char *) stringDefault ("keymap")) != NULL)
    	keymapAdd(str, (char*) default_keymap);

    /* See if we can get macroKey to work. What a hack -SAC */
    if ((str = (unsigned char *) stringDefault ("macroKey")) != NULL)
    {
        char *p;

        if (strlen (str) == 1)
        {
            /* This is a little pointless, but it'll preform as per
             * the documentation */
            default_keymap[*str - 32] = 'X';
        }
        else if (!strcmpi (str, "TAB"))
        {
            p = "^i";
            default_keymap[getctrlkey (&p) - 32] = 'X';
        }
        else if (!strcmpi (str, "ESC"))
        {
            p = "^[";
            default_keymap[getctrlkey (&p) - 32] = 'X';
        }
    }

    if ((str = (unsigned char *) stringDefault ("ckeymap")) != NULL)
        ckeymapAdd(str, (char*) default_keymap);

#ifdef MOUSE_AS_SHIFT
    if ((str = (unsigned char *) stringDefault ("b1keymap")) != NULL)
    {
        b1_as_shift = 1;
        while (*str != '\0' && *(str + 1) != '\0')
        {
            if (*str >= 32 && *str < 176)
            {
                default_keymap[*str - 32 + 192] = *(str + 1);
            }
            str += 2;
        }
    }

    if ((str = (unsigned char *) stringDefault ("b2keymap")) != NULL)
    {
        b2_as_shift = 1;
        while (*str != '\0' && *(str + 1) != '\0')
        {
            if (*str >= 32 && *str < 176)
            {
                default_keymap[*str - 32 + 288] = *(str + 1);
            }
            str += 2;
        }
    }

    if ((str = (unsigned char *) stringDefault ("b3keymap")) != NULL)
    {
        b3_as_shift = 1;
        while (*str != '\0' && *(str + 1) != '\0')
        {
            if (*str >= 32 && *str < 176)
            {
                default_keymap[*str - 32 + 384] = *(str + 1);
            }
            str += 2;
        }
    }

    if ((str = (unsigned char *) stringDefault ("b4keymap")) != NULL)
    {
        b4_as_shift = 1;
        while (*str != '\0' && *(str + 1) != '\0')
        {
            if (*str >= 32 && *str < 176)
            {
                default_keymap[*str - 32 + 480] = *(str + 1);
            }
            str += 2;
        }
    }

    if ((str = (unsigned char *) stringDefault ("b5keymap")) != NULL)
    {
        b5_as_shift = 1;
        while (*str != '\0' && *(str + 1) != '\0')
        {
            if (*str >= 32 && *str < 176)
            {
                default_keymap[*str - 32 + 576] = *(str + 1);
            }
            str += 2;
        }
    }

#endif

    /* note: not stored on server */
    if ((str = (unsigned char *) stringDefault ("buttonmap")) != NULL)
    	buttonmapAdd(str, (char*) default_buttonmap);
    
    /* Build ship specific keymaps */
    for (j = 0; j < nshiptypes; j++)
        buildShipKeymap(getship(j));
}

void
keymapAdd(char *str, char *kmap)
{
    while (*str != '\0' && *(str + 1) != '\0')
    {
        if (*str >= 32 && *str < 128)
        {
            kmap[*str - 32] = *(str + 1);
        }
        str += 2;
    }
}

void
ckeymapAdd(char *cstr, char *kmap)
{
    unsigned char c1, c2;

    while (*cstr != '\0')
    {
        if (*cstr >= 32 && *cstr < MAXASCII)
        {
            c1 = (unsigned char) (getctrlkey (&cstr) - 32);
            c2 = getctrlkey (&cstr);
            kmap[c1] = c2;
        }
    }
}

void
buttonmapAdd(char *str, char *bmap)
{
    while (*str != '\0' && *(str + 1) != '\0')
    {
        switch (*str++)
        {
            case '1':
                bmap[1] = getctrlkey (&str);
                break;
            case '2':
                bmap[2] = getctrlkey (&str);
                break;
            case '3':
                bmap[3] = getctrlkey (&str);
                break;
            /* XButton 1 */
            case '4':
                bmap[4] = getctrlkey (&str);
                break;
            /* XButton 2 */
            case '5':
                bmap[5] = getctrlkey (&str);
                break;
            /* Wheel Up */
            case '6':
                bmap[6] = getctrlkey (&str);
                break;
            /* Wheel Down */
            case '7':
                bmap[7] = getctrlkey (&str);
                break;

#ifdef SHIFTED_MOUSE
            case '8':
                bmap[8] = getctrlkey (&str);
                break;
            case '9':
                bmap[9] = getctrlkey (&str);
                break;
            case 'a':
                bmap[10] = getctrlkey (&str);
                break;
            case 'b':
                bmap[11] = getctrlkey (&str);
                break;
            case 'c':
                bmap[12] = getctrlkey (&str);
                break;
            case 'd':
                bmap[13] = getctrlkey (&str);
                break;
            case 'e':
                bmap[14] = getctrlkey (&str);
                break;
            case 'f':
                bmap[15] = getctrlkey (&str);
                break;
            case 'g':
                bmap[16] = getctrlkey (&str);
                break;
            case 'h':
                bmap[17] = getctrlkey (&str);
                break;
            case 'i':
                bmap[18] = getctrlkey (&str);
                break;
            case 'j':
                bmap[19] = getctrlkey (&str);
                break;
            case 'k':
                bmap[20] = getctrlkey (&str);
                break;
            case 'l':
                bmap[21] = getctrlkey (&str);
                break;
            case 'm':
                bmap[22] = getctrlkey (&str);
                break;
#endif /* SHIFTED_MOUSE */
                
            default:
                LineToConsole ("%c ignored in buttonmap\n", *(str - 1));
                break;
        }
    }
}

/******************************************************************************/
/***  buildShipKeymap() -  initializes the ship specific keymaps and
      buttonmaps, then searches defaults file for the ship specific entries   */
/******************************************************************************/
void
buildShipKeymap(struct ship *shipp)
{
    char keybuf[40], ckeybuf[40], buttonbuf[40];
    char *keymap_str, *ckeymap_str, *buttonmap_str;
    int i;

#ifdef MOUSE_AS_SHIFT
    memcpy(shipp->s_keymap, default_keymap, 672);
#else
    memcpy(shipp->s_keymap, default_keymap, 96);
#endif
    memcpy(shipp->s_buttonmap, default_buttonmap, 23);
  
    sprintf(keybuf, "keymap-%c%c", shipp->s_desig[0], shipp->s_desig[1]);
    sprintf(ckeybuf, "ckeymap-%c%c", shipp->s_desig[0], shipp->s_desig[1]);
    sprintf(buttonbuf, "buttonmap-%c%c", shipp->s_desig[0], shipp->s_desig[1]);


    keymap_str = stringDefault (keybuf);
    ckeymap_str = stringDefault (ckeybuf);
    buttonmap_str = stringDefault (buttonbuf);

    /* If individual ship keymap is found, reset the ship's keymap first before
       parsing the keymap string */
    if (keymap_str || ckeymap_str)
    {
        for (i = 0; i < 95; i++)
        {
            shipp->s_keymap[i] = (unsigned char) (i + 32);
            shipp->s_keymap[i + 96] = (unsigned char) (i + 32 + 96);

#ifdef MOUSE_AS_SHIFT
            shipp->s_keymap[i + 192] = (unsigned char) (i + 32);
            shipp->s_keymap[i + 288] = (unsigned char) (i + 32);
            shipp->s_keymap[i + 384] = (unsigned char) (i + 32);
            shipp->s_keymap[i + 480] = (unsigned char) (i + 32);
            shipp->s_keymap[i + 576] = (unsigned char) (i + 32);
#endif
        }
        shipp->s_keymap[95] = 0;
    }
    
    /* If individual ship buttonmap is found, reset the ship's buttonmap first before
       parsing the buttonmap string */
    if (buttonmap_str)
    {
        for (i = 1; i < 23; i++)
            shipp->s_buttonmap[i] = 0;
    }

    if (keymap_str)
        keymapAdd(keymap_str, (char*) shipp->s_keymap);

    if (ckeymap_str)
        ckeymapAdd(ckeymap_str, (char*) shipp->s_keymap);
        
    if (buttonmap_str)
        buttonmapAdd(buttonmap_str, (char*) shipp->s_buttonmap);
}

/******************************************************************************/
/***  initinput()                                                           ***/
/******************************************************************************/
void
initinput (void)
{
    /* Nothing doing... */
}

/******************************************************************************/
/***  detsetallow()                                                         ***/
/******************************************************************************/
RETSIGTYPE
detsetallow (int _dummy)
{
    detallow = 1;
}

#ifdef THREADED
/******************************************************************************/
/***  input()                                                               ***/
/******************************************************************************/
void
input ()
{
    W_Event event;

    /* Under Windows we spawn off a separate thread to handle network interaction;
       this is because it would require some awkward rewrites to integrate
       event detection and handling into the select() mechanism.
       It probably also increases performance */
    // Close HANDLE from previously created thread.
    if (InputThread != NULL)
    {
        WaitForSingleObject (InputThread, INFINITE);
        CloseHandle (InputThread);
    }
    InputThread = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) input2, 0, 0, &InputThreadID);

    while (1)
    {
        if (!W_WaitForEvent ()) /* W_WaitForEvent returns 0 if W_TerminateWait is called */
            break;
        process_event ();
    }

    /* clear exitFlag */
    exitFlag = 0;

    while (W_EventsPending ())
        W_NextEvent (&event);
    
    ingame = 0;
    longjmp (env, globalerr);
}

/******************************************************************************/
/***  input2()                                                              ***/
/******************************************************************************/
void
input2 ()
#else
/******************************************************************************/
/***  input() again - nonthreaded                                           ***/
/******************************************************************************/
input ()
#endif
{
    fd_set readfds;

    while (1)
    {

        if (keepInfo > 0 && opened_info != -2 &&        /* 6/1/93 LAB */
            opened_info < 0 && infomapped)
            destroyInfo ();

        if (keepInfo > 0 && opened_info != -2)
            opened_info--;

        FD_ZERO (&readfds);

#ifndef VMS

        FD_SET (sock, &readfds);

        if (udpSock >= 0)
            FD_SET (udpSock, &readfds);

        /* Skip read from server if select results in error. */
        if (select (max_fd, &readfds, 0, 0, 0) > 0)
        {
#else
        vms_wait_either (&readfds, xsock, sock, udpSock);
        {
#endif

#ifndef THREADED
            if (W_EventsPending ())
            {
                process_event ();
            }
#endif /* !THREADED */

            if (FD_ISSET (sock, &readfds) ||
                (udpSock >= 0 && FD_ISSET (udpSock, &readfds)))
            {
                intrupt (&readfds);

                if (isServerDead ())
                {
                    LineToConsole ("Whoops!  We've been ghostbusted!\n");
                    LineToConsole ("Pray for a miracle!\n");
                    /* UDP fail-safe */
                    commMode = commModeReq = COMM_TCP;
                    commSwitchTimeout = 0;
                    if (udpSock >= 0)
                        closeUdpConn ();
                    if (udpWin)
                    {
                        udprefresh (UDP_CURRENT);
                        udprefresh (UDP_STATUS);
                    }
                    connectToServer (nextSocket);
                    LineToConsole ("Yea!  We've been resurrected!\n");
                }
            }

        }
    }
}

/******************************************************************************/
/***  process_event()                                                       ***/
/******************************************************************************/
int
process_event (void)
{
    W_Event data;
    int loop = 0;
    W_Callback handler;

    do
    {
        loop++;
        if (!W_SpNextEvent (&data))
            continue;           /* continues at loop bottom */
        switch ((int) data.type)
        {
        case W_EV_KEY:
            if ((handler = W_GetWindowKeyDownHandler (data.Window)) != NULL)
                (*handler) (&data);

#ifdef DOC_WIN
            else if (data.Window == docwin)
                switch (data.key)
                {
                case 'f':
                    docline += 28;

                    if (docline >= maxdoclines)
                        docline = maxdoclines - 28;

                    showdocs (docline);
                    break;
                case 'b':
                    docline -= 28;

                    if (docline < 0)
                        docline = 0;

                    showdocs (docline);
                    break;
                case 'F':
                    docline += 4;

                    if (docline >= maxdoclines)
                        docline = maxdoclines - 28;

                    showdocs (docline);
                    break;
                case 'B':
                    docline -= 4;

                    if (docline < 0)
                        docline = 0;

                    showdocs (docline);
                    break;
                default:
                    data.Window = w;
                    keyaction (&data);
                    break;
                }
            else if (data.Window == xtrekrcwin)
                switch (data.key)
                {
                case 'f':
                    xtrekrcline += 28;

                    if (xtrekrcline >= maxxtrekrclines)
                        xtrekrcline = maxxtrekrclines - 28;

                    showxtrekrc (xtrekrcline);
                    break;
                case 'b':
                    xtrekrcline -= 28;

                    if (xtrekrcline < 0)
                        xtrekrcline = 0;

                    showxtrekrc (xtrekrcline);
                    break;
                case 'F':
                    xtrekrcline += 4;

                    if (xtrekrcline >= maxxtrekrclines)
                        xtrekrcline = maxxtrekrclines - 28;

                    showxtrekrc (xtrekrcline);
                    break;
                case 'B':
                    xtrekrcline -= 4;

                    if (xtrekrcline < 0)
                        xtrekrcline = 0;

                    showxtrekrc (xtrekrcline);
                    break;
                default:
                    data.Window = w;
                    keyaction (&data);
                    break;
                }
#endif

            else if (messageon)
                smessage (data.key);
            else
                keyaction (&data);
            break;
#ifdef MOTION_MOUSE
        case W_EV_CM_BUTTON:
#endif
        case W_EV_BUTTON:
            if ((handler = W_GetWindowButtonHandler (data.Window)) != NULL)
                (*handler) (&data);
            else
                buttonaction (&data);
            break;

        case W_EV_EXPOSE:
            if ((handler = W_GetWindowExposeHandler (data.Window)) != NULL)
                (*handler) (&data);
            else if (data.Window == mapw)
                redrawall = 1;
            else if (data.Window == warnw)
                W_ClearWindow (warnw);
            else if (data.Window == messagew)
                W_ClearWindow (messagew);

#ifdef XTREKRC_HELP
            else if (defWin && (data.Window == defWin))
                showdef ();
#endif

#ifdef DOC_WIN
            else if (docwin && (data.Window == docwin))
                showdocs (docline);
            else if (xtrekrcwin && (data.Window == xtrekrcwin))
                showxtrekrc (xtrekrcline);
#endif

            break;

        default:
            break;
        }
    }
    while (W_EventsQueued ());
    return loop;
}


/******************************************************************************/
/***  keyaction()                                                           ***/
/******************************************************************************/
void
keyaction (W_Event * data)
{

    fastQuit = 0;               /* any event, cancel
                                 * fastquit! */

    if (data->Window != mapw && data->Window != w && data->Window != infow
        && data->Window != playerw && data->Window != planetw)
        return;

    key = data->key;

    if (data->Window == infow)
    {
        int x, y;

        if (findMouseInWin (&x, &y, w))
        {                       /* local window */
            data->Window = w;
            data->x = x;
            data->y = y;
        }
        else if (findMouseInWin (&x, &y, mapw))
        {                       /* galactic window */
            data->Window = mapw;
            data->x = x;
            data->y = y;
        }
        else if (findMouseInWin (&x, &y, planetw))
        {                       /* planet list window */
            data->Window = planetw;
            data->x = x;
            data->y = y;
        }
        else if (findMouseInWin (&x, &y, playerw))
        {                       /* player list window */
            data->Window = playerw;
            data->x = x;
            data->y = y;
        }
    }

    if (localflags & (PFREFIT))
    {
        struct shiplist *temp;
        temp = shiptypes;
        while (temp) {
        if (temp->ship->s_letter == key) {
                sendRefitReq(temp->ship->s_type);
                localflags &= ~(PFREFIT);
                return;
            }
           temp = temp->next;
        }
        localflags &= ~(PFREFIT);
        return;
    }

    if (key >= 32 && key < MAXKEY)
    {
        key = myship->s_keymap[key - 32];
    }
    else
    {
        W_Beep ();
        return;
    }

#ifdef RECORDGAME
    /* If playing a recorded game, do not use regular keys. */
    /* What follows is a hardcoded list of commands */
    if (playback)
    {
        struct obtype *target;

        switch (key)
        {
            case 'Q':
            case 'q':
                /* Instant Quit */
                terminate (0);
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '!':
            case '#':
            case '%':
            case '(':
            case ')':
            case '<':
            case '>':
            case '@':
            case 'R':
                pbsetspeed (key);
                return;
                break;
            case 'l':
                target = gettarget (data->Window, data->x, data->y, TARG_PLAYER);
				if (target->o_num == -1)
					return;
                pblockplayer (target->o_num);
                return;
                break;
            case ';':
                target = gettarget (data->Window, data->x, data->y, TARG_PLANET);
                pblockplanet (target->o_num);
                return;
                break;
            case 'G':
            case 'H':
            case 'h':
            case 'I':
            case 'i':
            case 'L':
            case 'S':
            case '?':
                /* Do the normal function */
                /* Used for commands that do not try to send packets */
                break;
            default:
                /* If key is not in above list dont run it. */
                return;
        }
    }
#endif

    /* suggestion:  later we can add an option removing this to the emptyKey()
     * function.  This would improve input efficiency considerably when
     * "singleMacro" is non-NULL. - jn */

    if ((!MacroMode)
        && singleMacro[0] != '\0' && INDEX ((char *) singleMacro, data->key))
    {
        MacroMode = 1;
        MacroNum = -1;
    }

    if (MacroMode)
    {
        doMacro (data);
        return;
    }

    /* If the mouse is in player list window - enable info and lock, as
       well as certain RCDs.  This part has to be after macros, so the RCDs
       will work on the players from this list */
    if (data->Window == playerw)
    {
        switch (key)
        {
            case 'i':
            case 'I':
            case 'l':
            case 146:   /* ^2 */
            case 149:   /* ^5 */
            case 153:   /* ^9 */
            case 197:   /* ^e */
            case 198:   /* ^f */
            case 200:   /* ^h */
            case 206:   /* ^n */
            case 207:   /* ^o */
            case 208:   /* ^p */
                break;
            default:
                return;
        }
    }
    /* If the mouse is in planet list window - enable info and lock, as
       well as certain RCDs.  This part has to be after macros, so that RCDs
       will work on the planets from this list */
    if (data->Window == planetw)
    {
        switch (key)
        {
            case 'i':
            case 'I':
            case 'l':
            case 194: /* ^b */
            case 195: /* ^c */
            case 204: /* ^l */
            case 205: /* ^m */
            case 212: /* ^t */
            case 144: /* ^0 */
            case 145: /* ^1 */
            case 150: /* ^6 */
                break;
            default:
                return;
        }
    }

#ifdef MOTION_MOUSE
    if ((data->type == W_EV_CM_BUTTON) &&       /*  KOC - 10/18/95     */
        (!motion_mouse_enablable) &&    /* Hack for            */
        (key != 107))           /* continuous_steer    */
        return;

#endif

    (*(key_handlers[key].handler)) (data);
}

#ifdef MOUSE_AS_SHIFT
/******************************************************************************/
/***  mkeyaction()                                                          ***/
/******************************************************************************/
void
mkeyaction (W_Event * data)
{
    unsigned char key = data->key;

    fastQuit = 0;               /* any event, cancel
                                 * fastquit! */

    if (!INDEX ("sbogadc", key) || !(localflags & PFREFIT))
    {
        if (key >= 32 && key < 176)
        {
            int offset = 0;

            switch (data->modifier)
            {
            case W_LBUTTON:
                offset = 192;
                break;

            case W_MBUTTON:
                offset = 288;
                break;

            case W_RBUTTON:
                offset = 384;
                break;

            case W_XBUTTON1:
                offset = 480;
                break;

            case W_XBUTTON2:
                offset = 576;
                break;
            }

            key = myship->s_keymap[key - 32 + offset];
        }
    }

    data->key = key;
    keyaction (data);
}

#endif

/******************************************************************************/
/***  buttonaction()                                                        ***/
/******************************************************************************/
void
buttonaction (W_Event * data)
{
    unsigned char course;
    struct obtype *gettarget (W_Window ww,
                              int x,
                              int y,
                              int targtype),
	*target;

    fastQuit = 0;               /* any event, cancel
                                 * fastquit! */

#ifdef RECORDGAME
    /* While playing back recorded games, ignore the mouse */
    if (playback)
        return;
#endif

    if (data->Window != w && data->Window != mapw
        && data->Window != infow && data->Window != playerw
        && data->Window != planetw)
        return;
    
    if (data->Window == planetw)
    {
    	int x, y;
    	
    	if (findMouseInWin (&x, &y, planetw))
    	{
    	    data->Window = planetw;
            data->x = x;
            data->y = y;
        }
        if (data->key == W_LBUTTON)
        {
            sortPlanets = !sortPlanets;
            /* Do a full refresh */
            W_UnmapWindow (planetw);
            W_MapWindow (planetw);
        }
        else if (data->key == W_RBUTTON)
            W_UnmapWindow (planetw);

        return;
    }

    if (data->Window == playerw)
    {
    	int x, y;
    	
    	if (findMouseInWin (&x, &y, playerw))
    	{
    	    data->Window = playerw;
            data->x = x;
            data->y = y;
        }
        if (data->key == W_LBUTTON)
        {
            target = gettarget (data->Window, data->x, data->y, TARG_PLAYER);
            if (target->o_num == -1 || messageon || !playerListMessaging)
                return;
            /* Here we will have to enter message_on () followed by
               smessage to player */
#ifdef SOUND
            Play_Sound(MESSAGE_WAV, SF_MESSAGE);
#endif
            message_on ();
            data->key = players[target->o_num].p_mapchars[1];
            smessage (data->key);
	    return;
	}
        else if (data->key == W_RBUTTON)
	{
            if (messageon || !playerListMessaging)
		return;
            /* Here we will have to enter message_on () followed by
               smessage to player */
#ifdef SOUND
            Play_Sound(MESSAGE_WAV, SF_MESSAGE);
#endif
            message_on ();
            data->key = 'T';
            smessage (data->key);
	    return;
	}
        else if (data->key == W_MBUTTON)
	{
            if (messageon || !playerListMessaging)
		return;
           /* Here we will have to enter message_on () followed by
              smessage to player */
#ifdef SOUND
           Play_Sound(MESSAGE_WAV, SF_MESSAGE);
#endif
           message_on ();
           data->key = 'A';
           smessage (data->key);
           return;
	}
        return;
    }

    if (messageon)
        message_off ();         /* ATM */

#ifdef SHIFTED_MOUSE
    if (data->key >= W_LBUTTON && data->key <= W_XBUTTON2_4)
#else
    if (data->key >= W_LBUTTON && data->key <= W_WHEELDOWN)
#endif
    {
        if (myship->s_buttonmap[data->key] != '\0')
        {
            data->key = myship->s_buttonmap[data->key];
            keyaction (data);
            return;
        }
    }

#ifdef MOTION_MOUSE
    if ((data->type == W_EV_CM_BUTTON) &&       /*  KOC - 10/18/95     */
        (!motion_mouse_enablable) &&    /* Hack for            */
        (data->key != W_RBUTTON))       /* continuous_steer    */
        return;
#endif

    if (data->Window == infow)
    {
        int x, y;

        if (findMouseInWin (&x, &y, w))
        {                       /* local window */
            data->Window = w;
            data->x = x;
            data->y = y;
        }
        else if (findMouseInWin (&x, &y, mapw))
        {                       /* galactic window */
            data->Window = mapw;
            data->x = x;
            data->y = y;
        }
    }

    if (data->key == W_RBUTTON)
    {
        course = (unsigned char) (getcourse (data->Window, data->x, data->y));
        set_course (course);
    }
    else if (data->key == W_LBUTTON)
    {
        course = (unsigned char) (getcourse (data->Window, data->x, data->y));
        sendTorpReq (course);
    }
    else if (data->key == W_MBUTTON)
    {
        phaseraction(data);
    }
    else if (data->key == W_XBUTTON1)
    {
        unsigned char course;

        /* f = launch plasma torpedos */

#ifdef AUTOKEY
        if (autoKey)
            autoKeyPlasmaReqOn ();
        else
        {
            course = getcourse (data->Window, data->x, data->y);
            sendPlasmaReq (course);
        }
#else
        course = (unsigned char) (getcourse (data->Window, data->x, data->y));
        sendPlasmaReq (course);
#endif /* AUTOKEY */
    }
    else if (data->key == W_XBUTTON2)
    {
#ifdef AUTOKEY
        if (autoKey && !(localflags & PFREFIT))
            autoKeyBombReqOn ();
        else
            bomb_planet ();
#else
        bomb_planet ();
#endif /* AUTOKEY */
    }
    else if (data->key == W_WHEELUP)
    {
        scaleFactor -= 2;
        if (scaleFactor < 10)
            scaleFactor = 10;
        W_FastClear = 1;
        if (viewBox)
            redrawall = 1;
    }
    else if (data->key == W_WHEELDOWN)
    {
        scaleFactor += 2;
        if (scaleFactor > 40)
            scaleFactor = 40;
        W_FastClear = 1;
        if (viewBox)
            redrawall = 1;
    }
#ifdef SHIFTED_MOUSE
    else if (data->key == W_RBUTTON2)
    {
        set_speed (me->p_ship.s_maxspeed / 2);
        localflags &= ~(PFREFIT);
    }
    else if (data->key == W_LBUTTON2)
    {
        set_speed (99);         /* Max speed... */
        localflags &= ~(PFREFIT);
    }
    else if (data->key == W_MBUTTON2)
    {
        detmine ();
    }
    else if (data->key == W_XBUTTON1_2)
    {
        set_speed (0);
    }
    else if (data->key == W_XBUTTON2_2)
    {
        static unsigned long lastdet = 0;
        unsigned long curtime;

#ifdef AUTOKEY
        if (autoKey)
            autoKeyDetReqOn ();
        else
            sendDetonateReq ();
#else
        /* want to limit these to one per update */
        curtime = mstime ();
        if (curtime >= lastdet + 100)       /* Allow one per 100 ms */
        {
            sendDetonateReq ();
            lastdet = curtime;
        }
#endif /* AUTOKEY */
    }
    else if (data->key == W_RBUTTON3)
    {
        if (!infomapped)
        {
            inform (data->Window, data->x, data->y, 'i');
            opened_info = keepInfo * server_ups / 10;     /* 5/31/93 LAB */
        }
        else
        {
            destroyInfo ();
            opened_info = -2;
        }
    }
    else if (data->key == W_LBUTTON3)
    {
        shield_tog ();
    }
    else if (data->key == W_MBUTTON3)
    {
        cloak ();
    }
    else if (data->key == W_XBUTTON1_3)
    {
#ifdef AUTOKEY
        if (autoKey)
            autoKeyBeamUpReqOn ();
        else
#endif

            beam_up ();
    }
    else if (data->key == W_XBUTTON2_3)
    {
#ifdef AUTOKEY
        if (autoKey)
            autoKeyBeamDownReqOn ();
        else
#endif

            beam_down ();
    }
    else if (data->key == W_RBUTTON4)
    {
        lockPlanetOrBase (data->Window, data->x, data->y);
    }
    else if (data->key == W_LBUTTON4)
    {
        struct obtype *gettarget (W_Window ww,
                                  int x,
                                  int y,
                                  int targtype),
         *target;

        if (me->p_flags & (PFTRACT | PFPRESS))
            sendTractorReq (0, (char) (me->p_no));
        else
        {
            target = gettarget (data->Window, data->x, data->y, TARG_PLAYER);
		    if (target->o_num == -1)
			    return;
            me->p_tractor = (short) (target->o_num);
            sendTractorReq (1, (char) (target->o_num));
        }
    }
    else if (data->key == W_MBUTTON4)
    {
        struct obtype *gettarget (W_Window ww,
                                  int x,
                                  int y,
                                  int targtype),
         *target;

        if (me->p_flags & (PFTRACT | PFPRESS))
            sendRepressReq (0, (char) (me->p_no));
        else
        {
            target = gettarget (data->Window, data->x, data->y, TARG_PLAYER);
		    if (target->o_num == -1)
			    return;
            me->p_tractor = (short) (target->o_num);
            sendRepressReq (1, (char) (target->o_num));
        }
    }
    else if (data->key == W_XBUTTON1_4)
    {
        target =
            gettarget (data->Window, data->x, data->y, TARG_PLAYER | TARG_PLANET);

    	if (target->o_num == -1)
		    return;

        if (target->o_type == PLAYERTYPE)
        {
            if (players[target->o_num].p_team != me->p_team)
            {
                /* PFSEEN is not sent to client, so we have to check
                 * if player's coordinates are (-10000,-10000) to know
                 * wether we can see him or not
                 */
                if (players[target->o_num].p_x != -10000 &&
                    players[target->o_num].p_y != -10000)
                {
                    sendPlaylockReq (target->o_num);
                    me->p_playerl = (short) (target->o_num);
                }
            }
            else
            {
                sendPlaylockReq (target->o_num);
                me->p_playerl = (short) (target->o_num);
            }
        }
        else
        {                           /* It's a planet */
            sendPlanlockReq (target->o_num);
            me->p_planet = (short) (target->o_num);
        }
    }
    else if (data->key == W_XBUTTON2_4)
    {
        sendPractrReq ();
    }
#endif /* SHIFTED_MOUSE */
}

/******************************************************************************/
/***  phaseraction()                                                        ***/
/******************************************************************************/
void
phaseraction (W_Event * data)
{
    unsigned char course;
    int x, y, dx, dy;
    int distance;
    struct obtype *gettarget (W_Window ww,
                              int x,
                              int y,
                              int targtype),
    *target;

    if (F_auto_weapons && autoPhaser)
    {
        target = gettarget (data->Window, data->x, data->y, TARG_ENEMY | TARG_CLOAK | TARG_PLASMA);
        if (target->o_type == PLAYERTYPE && target->o_num == me->p_no)
        { 
            /* failed to find a target */
            course = (unsigned char) (getcourse (data->Window, data->x, data->y));
            sendPhaserReq (course);
            return;
        }
        dx = target->o_dist_x - me->p_x;
        dy = target->o_dist_y - me->p_y;
  
        /* Sanity check on distance.  */
        /* Check ship max phaser range for phasers.  Sometimes phasers are fired to "point". */

        distance = (int) sqrt((double) dx*dx + (double) dy*dy);
        if (distance > (PHASEDIST * me->p_ship.s_phaserdamage / 100))
        { 
            course = (unsigned char) (getcourse (data->Window, data->x, data->y));
            sendPhaserReq (course);
            return;
        }

        /* Everything checks out.  Scale to window coordinates. */
        if (data->Window == w)
        {
            x = dx / scaleFactor + TWINSIDE / 2;
            y = dy / scaleFactor + TWINSIDE / 2;
        }
        else
        {
            x = target->o_dist_x * GWINSIDE / GWIDTH;
            y = target->o_dist_y * GWINSIDE / GWIDTH;
        }
        course = (unsigned char) (getcourse (data->Window, x, y));
    }
    else
        course = (unsigned char) (getcourse (data->Window, data->x, data->y));
    sendPhaserReq (course);
}

/******************************************************************************/
/***  getcourse()                                                           ***/
/******************************************************************************/
int
getcourse (W_Window ww,
           int x,
           int y)
{
    if (ww == mapw)
    {
        int me_x, me_y;

        me_x = me->p_x * GWINSIDE / GWIDTH;
        me_y = me->p_y * GWINSIDE / GWIDTH;
        return ((unsigned char) nint ((atan2 ((double) (x - me_x),
                                              (double) (me_y -
                                                        y)) / 3.14159 *
                                       128.) + 0.5));
    }
    else
        return ((unsigned char) nint ((atan2 ((double) (x - TWINSIDE / 2),
                                              (double) (TWINSIDE / 2 - y))
                                       / 3.14159 * 128.) + 0.5));
}


/******************************************************************************/
/***  detmine()                                                             ***/
/******************************************************************************/
int
detmine (void)
{
    register int i = 0;

    if (paradise)
        sendDetMineReq(-1);
    else
    {
    for (i = 0; i < ntorps; i++)
    {
        if (torps[i + (me->p_no * ntorps)].t_status == TMOVE ||
            torps[i + (me->p_no * ntorps)].t_status == TSTRAIGHT)
        {
            sendDetMineReq ((short) (i + (me->p_no * ntorps)));
            if (recv_short)
                break;          /* Let the server det for me */
        }
    }
    }
    return (0);
}

/******************************************************************************/
/***  emptyKey()                                                            ***/
/******************************************************************************/
void
emptyKey (void)
{
    W_Beep ();
}

/******************************************************************************/
/***  macro_on()                                                            ***/
/******************************************************************************/
void
macro_on (void)
{
    warning ("In Macro Mode");
    MacroMode = 1;
    MacroNum = -1;
}

/******************************************************************************/
/***  doMacro()                                                             ***/
/******************************************************************************/
void
doMacro (W_Event * data)
{
    struct obtype *gettarget (W_Window ww,
                              int x,
                              int y,
                              int targtype),
     *target;
    int targettype;
    enum dist_type i;
    int c;
    char who;
    int found = 0;

    warning (" ");              /* We are here now, so turn off macro mode */
    MacroMode = 0;


    if (data->key == '?')
    {
        showMacroWin ();
        return;
    }


    /* user defined macros *OVERRIDE* receiver configurable distresses.  */

    if (UseNewMacro)
    {
        /* sorry guys, I only program in kludge - jn 6/3/93 */

        if (MacroNum > -1)
        {                       /* macro identified, who to? */
            if (MacroNum >= MAX_MACRO)
            {
                LineToConsole ("Unknown Macro Num!  There is a macro bug!!\n");
            }

            if (!pmacro (MacroNum, data->key, data))
                W_Beep ();

            MacroNum = -1;
            return;
        }
    }


    for (c = 0; c < macrocnt; ++c)
    {
        if (macro[c].key == data->key)
        {
            if (!UseNewMacro)
            {
                if (rejectMacro && macro[c].type != NBTM)
                    warning ("NEWMACROs not allowed at this server!!");
                else
                    pnbtmacro (c);

                return;
            }


            /* Use New Macro */

            switch (macro[c].type)
            {
            case NBTM:

                pnbtmacro (c);
                return;
                break;

            case NEWM:

                warning ("Send macro to which player?");
                MacroNum = c;
                MacroMode = 1;  /* Need another key */
                return;
                break;

            case NEWMMOUSE:
                {
                    /* first translate into who, then send */
                    switch (macro[c].who)
                    {
                        struct player *j;
                        struct planet *l;

                    case MACRO_FRIEND:
                    case MACRO_ENEMY:
                    case MACRO_PLAYER:

                        targettype = TARG_PLAYER;
                        if (macro[c].who == MACRO_ENEMY)
                            targettype |= TARG_ENEMY;
                        else if (macro[c].who == MACRO_FRIEND)
                            targettype |= TARG_FRIEND;

                        target = gettarget (data->Window, data->x, data->y,
                                            TARG_PLAYER | TARG_CLOAK);
						if (target->o_num == -1)
							return;
                        if (target->o_type == PLAYERTYPE)
                        {
                            j = &players[target->o_num];
                            if (j->p_flags & PFCLOAK)
                                maskrecip = 1;
                            who = j->p_mapchars[1];
                        }
                        else
                        {
                            who = me->p_mapchars[1];
                            warning ("Can only send a message to a player");
                        }
                        break;

                    case MACRO_TEAM:
                        target = gettarget (data->Window, data->x, data->y,
                                            TARG_PLAYER | TARG_PLANET);
						if (target->o_num == -1)
							return;
                        if (target->o_type == PLANETTYPE)
                        {
                            l = &planets[target->o_num];
                            who = teamlet[l->pl_owner];
                        }
                        else if (target->o_type == PLAYERTYPE)
                        {
                            j = &players[target->o_num];
                            who = j->p_mapchars[0];
                        }
                        else
                        {
                            who = me->p_mapchars[1];
                            warning ("Player or planet only please");
                        }
                        break;

                    default:
                        who = me->p_mapchars[1];
                        break;
                    }

                    if (!pmacro (c, who, data))
                        W_Beep ();
#ifdef MULTILINE_MACROS
                    if (macro[c].multi == 1)
                    {
                    	found = 1;
                    	break;          /* Loop again */
                    }
#endif
                    return;
                    break;
                }

#ifdef MULTILINE_MACROS

            case NEWMULTIM:

                if (!pmacro (c, macro[c].who, data))
                    W_Beep ();

                found = 1;
                break;          /* Loop again */
#endif

            case NEWMSPEC:

                if (!pmacro (c, macro[c].who, data))
                    W_Beep ();
                return;
                break;


            default:
                LineToConsole ("Unknown Macro Type!  Jeff's a twink!!\n");
                warning ("Unknown macro type (eg There is a macro bug)");
                return;
                break;
            }
        }
    }

    if (found)
        return;

    /* scan for distress call here */

    for (i = take; distmacro[i].name; i++)
    {
        if (distmacro[i].c == data->key)
        {
            emergency (i, data);
            return;
        }
    }

    warning ("Unknown macro");
    W_Beep ();
}

/******************************************************************************/
/***  Key32()                                                               ***/
/******************************************************************************/
void
Key32 (void)
{
    /* ' ' = clear special windows */
#ifdef SOUND
    W_UnmapWindow (soundWin);
#endif

#ifdef DOC_WIN
    W_UnmapWindow (xtrekrcwin);
    W_UnmapWindow (docwin);
#endif

    W_UnmapWindow (spWin);

    W_UnmapWindow (pStats);
    W_UnmapWindow (statwin);
    W_UnmapWindow (planetw);
    W_UnmapWindow (rankw);
    if (infomapped)
        destroyInfo ();
    W_UnmapWindow (helpWin);
    W_UnmapWindow (playerw2);
    W_UnmapWindow (macroWin);

#ifdef XTREKRC_HELP
    if (defWin)
        W_UnmapWindow (defWin);
#endif

    W_UnmapWindow (war);
    if (optionWin)
        optiondone ();

    if (udpWin)
        udpdone ();

    if (showHints)
    {
        W_UnmapWindow (hintWin);
        showHints = 0;
    }
}

/******************************************************************************/
/***  Key33()                                                               ***/
/******************************************************************************/
void
Key33 (void)
{
    set_speed (11);
}

/******************************************************************************/
/***  Key34()                                                               ***/
/******************************************************************************/
void
Key34 (W_Event * data)
{
    saveOptions();
}

/******************************************************************************/
/***  Key35()                                                               ***/
/******************************************************************************/
void
Key35 (void)
{
    set_speed (me->p_ship.s_maxspeed / 2);
}

/******************************************************************************/
/***  Key36()                                                               ***/
/******************************************************************************/
void
Key36 (void)
{
    sendTractorReq (0, (char) (me->p_no));
}

/******************************************************************************/
/***  Key37()                                                               ***/
/******************************************************************************/
void
Key37 (void)
{
    if (paradise)
        set_speed(me->p_ship.s_maxspeed);
    else
        set_speed (99);             /* Max speed... */
}

/******************************************************************************/
/***  Key38()                                                               ***/
/******************************************************************************/
void
Key38 (void)
{
    reread_defaults ();
}

/******************************************************************************/
/***  Key39()                                                               ***/
/******************************************************************************/
void
Key39 (void)
{
    if (W_IsMapped (playerw2))
        W_UnmapWindow (playerw2);
    else
        W_MapWindow (playerw2);
}

/******************************************************************************/
/***  Key40()                                                               ***/
/******************************************************************************/
void
Key40 (void)
{
    set_speed (10);
}

/******************************************************************************/
/***  Key41()                                                               ***/
/******************************************************************************/
void
Key41 (W_Event * data)
{
    set_speed (10);
}

/******************************************************************************/
/***  Key42()                                                               ***/
/******************************************************************************/
void
Key42 (void)
{
    if (!(me->p_flags & PFPLOCK))
        lockBase ();
    sendPractrReq ();
}

/******************************************************************************/
/***  Key43()                                                               ***/
/******************************************************************************/
void
Key43 (void)
{
    /* UDP: pop up UDP control window */
    if (udpWin != NULL && W_IsMapped (udpWin))
        udpdone ();
    else
    {
        char buf[80];

        udpwindow ();
        sprintf (buf, "UDP client version %.1f", (float) UDPVERSION / 10.0);
        warning (buf);
    }

}

/******************************************************************************/
/***  Key44()                                                               ***/
/******************************************************************************/
void
Key44 (void)
{
    if (W_IsMapped (pStats))
    {
        W_UnmapWindow (pStats);
    }
    else
    {
        W_MapWindow (pStats);
        redrawPStats ();
    }

}

/******************************************************************************/
/***  Key45()                                                               ***/
/******************************************************************************/
void
Key45 (void)
{
    sendShortReq (SPK_SALL, 1);
}

/******************************************************************************/
/***  Key46()                                                               ***/
/******************************************************************************/
void
Key46 (void)
{
    if (paradise)
        set_speed(98);  /* afterburners */
    else
        emptyKey();
}

/******************************************************************************/
/***  Key47()                                                               ***/
/******************************************************************************/
void
Key47 (void)
{
    sortPlayers = !sortPlayers;
    RedrawPlayerList ();
}

/******************************************************************************/
/***  Key48()                                                               ***/
/******************************************************************************/
void
Key48 (void)
{
    set_speed (0);
}

/******************************************************************************/
/***  Key49()                                                               ***/
/******************************************************************************/
void
Key49 (void)
{
    set_speed (1);
}

/******************************************************************************/
/***  Key50()                                                               ***/
/******************************************************************************/
void
Key50 (void)
{
    set_speed (2);
}

/******************************************************************************/
/***  Key51()                                                               ***/
/******************************************************************************/
void
Key51 (void)
{
    set_speed (3);
}

/******************************************************************************/
/***  Key52()                                                               ***/
/******************************************************************************/
void
Key52 (void)
{
    set_speed (4);
}

/******************************************************************************/
/***  Key53()                                                               ***/
/******************************************************************************/
void
Key53 (void)
{
    set_speed (5);
}

/******************************************************************************/
/***  Key54()                                                               ***/
/******************************************************************************/
void
Key54 (void)
{
    set_speed (6);
}

/******************************************************************************/
/***  Key55()                                                               ***/
/******************************************************************************/
void
Key55 (void)
{
    set_speed (7);
}

/******************************************************************************/
/***  Key56()                                                               ***/
/******************************************************************************/
void
Key56 (void)
{
    set_speed (8);
}

/******************************************************************************/
/***  Key57()                                                               ***/
/******************************************************************************/
void
Key57 (void)
{
    set_speed (9);
}

/******************************************************************************/
/***  Key58()                                                               ***/
/******************************************************************************/
void
Key58 (void)
{
    logging = !logging;
    if (logging)
        warning ("Message logging is ON");
    else
        warning ("Message logging is OFF");

}

/******************************************************************************/
/***  Key59()                                                               ***/
/******************************************************************************/
void
Key59 (W_Event * data)
{
    lockPlanetOrBase (data->Window, data->x, data->y);

}

/******************************************************************************/
/***  Key60()                                                               ***/
/******************************************************************************/
void
Key60 (void)
{
    set_speed (me->p_speed - 1);
}

/******************************************************************************/
/***  Key61()                                                               ***/
/******************************************************************************/
void
Key61 (void)
{
    /* UDP: request for full update */
    sendUdpReq (COMM_UPDATE);
}

/******************************************************************************/
/***  Key62()                                                               ***/
/******************************************************************************/
void
Key62 (void)
{
    set_speed (me->p_speed + 1);
}

/******************************************************************************/
/***  Key63()                                                               ***/
/******************************************************************************/
void
Key63 (void)
{
    if (W_IsMapped (phaserwin))
        phaserWindow = 1;
    if (!W_IsMapped (reviewWin))
    {
        if (W_IsMapped (messwa))
        {
            W_UnmapWindow (messwa);
            W_UnmapWindow (phaserwin);
            W_UnmapWindow (messwt);
            W_UnmapWindow (messwi);
            W_UnmapWindow (messwk);
        }
        else
        {
            W_MapWindow (reviewWin);
        }
    }
    else
    {
        W_UnmapWindow (reviewWin);
        W_MapWindow (messwa);
        W_MapWindow (messwt);
        W_MapWindow (messwi);
        W_MapWindow (messwk);
        if (phaserWindow)
            W_MapWindow (phaserwin);
        if (W_IsMapped (statwin))
        {
            W_UnmapWindow (statwin);
            W_MapWindow (statwin);
        }
    }
    if (optionWin)
    {
        optionredrawtarget (reviewWin);
        optionredrawtarget (messwa);
        optionredrawtarget (phaserwin);
        optionredrawtarget (messwt);
        optionredrawtarget (messwi);
        optionredrawtarget (messwk);
    }
}

/******************************************************************************/
/***  Key64()                                                               ***/
/******************************************************************************/
void
Key64 (void)
{
    set_speed (12);
}

/******************************************************************************/
/***  Key65()                                                               ***/
/******************************************************************************/
void
Key65 (W_Event * data)
{
    emptyKey ();
}

/******************************************************************************/
/***  Key66()                                                               ***/
/******************************************************************************/
void
Key66 (void)
{
    emptyKey ();
}

/******************************************************************************/
/***  Key67()                                                               ***/
/******************************************************************************/
void
Key67 (void)
{
    sendCoupReq ();
}

/******************************************************************************/
/***  Key68()                                                               ***/
/******************************************************************************/
void
Key68 (void)
{
    detmine ();

#ifdef AUTOKEY
    if (autoKey)
        autoKeyAllOff ();       /* xx */
#endif
}

/******************************************************************************/
/***  Key69()                                                               ***/
/******************************************************************************/
void
Key69 (W_Event * data)
{
    emergency (help, data);
}

/******************************************************************************/
/***  Key70()                                                               ***/
/******************************************************************************/
void
Key70 (W_Event * data)
{
    emergency (carrying, data);
}

/******************************************************************************/
/***  Key71()                                                               ***/
/******************************************************************************/
void
Key71 (W_Event * data)
{
#ifdef HOCKEY_LINES
    if (!playing_hockey)
        return;

    showHockeyLinesMap = !showHockeyLinesMap;

    if (optionWin)
        optionredrawoption (&showHockeyLinesMap);

    redrawall = 1;
#else
    emptyKey ();
#endif
}

/******************************************************************************/
/***  Key72()                                                               ***/
/******************************************************************************/
void
Key72 (W_Event * data)
{
#ifdef HOCKEY_LINES
    if (!playing_hockey)
        return;

    showHockeyLinesLocal = !showHockeyLinesLocal;

    if (optionWin)
        optionredrawoption (&showHockeyLinesLocal);
#else
    emptyKey ();
#endif
}

/******************************************************************************/
/***  Key73()                                                               ***/
/******************************************************************************/
void
Key73 (W_Event * data)
{
    /* I = get extended information */
    if (!infomapped)
    {
        inform (data->Window, data->x, data->y, key);
        opened_info = keepInfo * server_ups / 10;
    }
    else
    {
        destroyInfo ();
        opened_info = -2;
    }

}

/******************************************************************************/
/***  Key74()                                                               ***/
/******************************************************************************/
void
Key74 (W_Event * data)
{
    if (paradise)
        set_speed(99); /* warp! */
    else
        emptyKey ();
}

/******************************************************************************/
/***  Key75()                                                               ***/
/******************************************************************************/
void
Key75 (void)
{
    emptyKey ();
}

/******************************************************************************/
/***  Key76()                                                               ***/
/******************************************************************************/
void
Key76 (void)
{

    if (sortPlayers)
    {
        switch (playerListObserver)
        {
        case (0):
            playerListObserver++;
            break;
        case (1):
            playerListObserver++;
            break;
        case (2):
            playerListObserver++;
            break;
        case (3):
            playerListObserver++;
            W_UnmapWindow (playerw);
            break;
        case (4):
            playerListObserver = 0;
            W_MapWindow (playerw);
            break;
        }

        if (W_IsMapped (playerw))
            RedrawPlayerList ();
    }
    else if (W_IsMapped (playerw))
        W_UnmapWindow (playerw);
    else
        W_MapWindow (playerw);
}

/******************************************************************************/
/***  Key77()                                                               ***/
/******************************************************************************/
void
Key77 (W_Event * data)
{

#ifdef TOOLS
    showToolsWin ();
#else
    emptyKey ();
#endif
}

/******************************************************************************/
/***  Key78()                                                               ***/
/******************************************************************************/
void
Key78 (void)
{
    /* N = Toggle Name mode */
    showPlanetNames = !showPlanetNames;
    if (optionWin)
        optionredrawoption (&showPlanetNames);
}

/******************************************************************************/
/***  Key79()                                                               ***/
/******************************************************************************/
void
Key79 (void)
{
    if (optionWin != NULL && W_IsMapped (optionWin))
        optiondone ();
    else
        optionwindow ();
}

/******************************************************************************/
/***  Key80()                                                               ***/
/******************************************************************************/
void
Key80 (void)
{
    if (W_IsMapped (planetw))
    {
        W_UnmapWindow (planetw);
    }
    else
    {
        W_MapWindow (planetw);
    }

}

/******************************************************************************/
/***  Key81()                                                               ***/
/******************************************************************************/
void
Key81 (void)
{

#ifdef SOUND
    Play_Sound(SELF_DESTRUCT_WAV, SF_INFO);
#endif

    sendQuitReq ();
}

/******************************************************************************/
/***  Key82()                                                               ***/
/******************************************************************************/
void
Key82 (void)
{
    sendRepairReq (1);
}

/******************************************************************************/
/***  Key83()                                                               ***/
/******************************************************************************/
void
Key83 (void)
{
    if (W_IsMapped (statwin))
    {
        W_UnmapWindow (statwin);
    }
    else
    {
        W_MapWindow (statwin);
    }

}

/******************************************************************************/
/***  Key84()                                                               ***/
/******************************************************************************/
void
Key84 (W_Event * data)
{
    if (me->p_flags & (PFTRACT | PFPRESS))
    {
        sendTractorReq (0, (char) (me->p_no));
        return;
    }
    target = gettarget (data->Window, data->x, data->y, TARG_PLAYER);
	if (target->o_num == -1)
		return;
    me->p_tractor = (short) (target->o_num);
    if (key == 'T')
    {
        sendTractorReq (1, (char) (target->o_num));
    }
    else
    {
        sendRepressReq (1, (char) (target->o_num));
    }
}

/******************************************************************************/
/***  Key85()                                                               ***/
/******************************************************************************/
void
Key85 (void)
{
    if (W_IsMapped (rankw))
    {
        W_UnmapWindow (rankw);
    }
    else
    {
        W_MapWindow (rankw);
    }

}

/******************************************************************************/
/***  Key86()                                                               ***/
/******************************************************************************/
void
Key86 (void)
{
#ifdef SOUND
    ChangeVolume (1);
#else
    emptyKey ();
#endif
}

/******************************************************************************/
/***  Key87()                                                               ***/
/******************************************************************************/
void
Key87 (W_Event * data)
{
    emptyKey ();
}

/******************************************************************************/
/***  Key88()                                                               ***/
/******************************************************************************/
void
Key88 (void)
{
    macro_on ();
}

/******************************************************************************/
/***  Key89()                                                               ***/
/******************************************************************************/
void
Key89 (W_Event * data)
{
    emptyKey ();
}

/******************************************************************************/
/***  Key90()                                                               ***/
/******************************************************************************/
void
Key90 (W_Event * data)
{
    emptyKey ();
}

/******************************************************************************/
/***  Key91()                                                               ***/
/******************************************************************************/
void
Key91 (void)
{
    shield_down ();
}

/******************************************************************************/
/***  Key92()                                                               ***/
/******************************************************************************/
void
Key92 (void)
{
    timeBank[T_USER] = time (NULL);
    timerType = T_USER;
}

/******************************************************************************/
/***  Key93()                                                               ***/
/******************************************************************************/
void
Key93 (void)
{
    shield_up ();
}

/******************************************************************************/
/***  Key94()                                                               ***/
/******************************************************************************/
void
Key94 (W_Event * data)
{
    if (me->p_flags & (PFTRACT | PFPRESS))
        sendRepressReq (0, (char) (me->p_no));
    target = gettarget (data->Window, data->x, data->y, TARG_PLAYER);
	if (target->o_num == -1)
		return;
    me->p_tractor = (short) (target->o_num);
    sendRepressReq (1, (char) (target->o_num));
}

/******************************************************************************/
/***  Key95()                                                               ***/
/******************************************************************************/
void
Key95 (W_Event * data)
{
    if (me->p_flags & (PFTRACT | PFPRESS))
        sendTractorReq (0, (char) (me->p_no));
    target = gettarget (data->Window, data->x, data->y, TARG_PLAYER);
	if (target->o_num == -1)
		return;
    me->p_tractor = (short) (target->o_num);
    sendTractorReq (1, (char) (target->o_num));
}

/******************************************************************************/
/***  Key96()                                                               ***/
/******************************************************************************/
void
Key96 (void)
{
    if (spWin != NULL && W_IsMapped (spWin))
        spdone ();
    else
        spwindow ();
}

/******************************************************************************/
/***  Key97()                                                               ***/
/******************************************************************************/
void
Key97 (W_Event * data)
{
    emptyKey ();
}

/******************************************************************************/
/***  Key98()                                                               ***/
/******************************************************************************/
void
Key98 (void)
{

#ifdef AUTOKEY
    if (autoKey && !(localflags & PFREFIT))
        autoKeyBombReqOn ();
    else
        bomb_planet ();
#else
    bomb_planet ();
#endif /* AUTOKEY */

}

/******************************************************************************/
/***  Key99()                                                               ***/
/******************************************************************************/
void
Key99 (void)
{
    cloak ();

}

/******************************************************************************/
/***  Key100()                                                               ***/
/******************************************************************************/
void
Key100 (void)
{
    static unsigned long lastdet = 0;
    unsigned long curtime;

#ifdef AUTOKEY
    if (autoKey)
        autoKeyDetReqOn ();
    else
        sendDetonateReq ();
#else
    /* want to limit these to one per update */
    curtime = mstime ();
    if (curtime >= lastdet + 100)       /* Allow one per 100 ms */
    {
        sendDetonateReq ();
        lastdet = curtime;
    }
#endif /* AUTOKEY */
    showdetCircle = 1;

}

/******************************************************************************/
/***  Key101()                                                              ***/
/******************************************************************************/
void
Key101 (void)
{
    if (me->p_flags & PFDOCKOK)
        sendDockingReq (0);
    else
        sendDockingReq (1);
}

/******************************************************************************/
/***  Key102()                                                              ***/
/******************************************************************************/
void
Key102 (W_Event * data)
{
    unsigned char course;

    /* f = launch plasma torpedos */

#ifdef AUTOKEY
    if (autoKey)
        autoKeyPlasmaReqOn ();
    else
    {
        course = getcourse (data->Window, data->x, data->y);
        sendPlasmaReq (course);
    }
#else
    course = (unsigned char) (getcourse (data->Window, data->x, data->y));
    sendPlasmaReq (course);
#endif /* AUTOKEY */

}

/******************************************************************************/
/***  Key103()                                                              ***/
/******************************************************************************/
void
Key103 (W_Event * data)
{
    emptyKey ();
}

/******************************************************************************/
/***  Key104()                                                              ***/
/******************************************************************************/
void
Key104 (void)
{
    /* h = Map help window */
    if (W_IsMapped (helpWin))
    {
        W_UnmapWindow (helpWin);
    }
    else
    {
#ifdef RECORDGAME
        if(playback)
            pbfillhelp();
        else
#endif
            fillhelp ();
        W_MapWindow (helpWin);
    }
    if (optionWin)
        optionredrawtarget (helpWin);

}

/******************************************************************************/
/***  Key105()                                                              ***/
/******************************************************************************/
void
Key105 (W_Event * data)
{
    if (!infomapped)
    {
        inform (data->Window, data->x, data->y, key);
        opened_info = keepInfo * server_ups / 10;
    }
    else
    {
        destroyInfo ();
        opened_info = -2;
    }
}

/******************************************************************************/
/***  Key106()                                                              ***/
/******************************************************************************/
void
Key106 (W_Event * data)
{
    emptyKey ();
}

/******************************************************************************/
/***  Key107()                                                              ***/
/******************************************************************************/
void
Key107 (W_Event * data)
{
    unsigned char course;

    /* Observers can't move.  Also incorrectly removes the lock flag even though
       you are still locked */
    if (me->p_flags & PFOBSERV && !paradise)
       return;

    course = (unsigned char) (getcourse (data->Window, data->x, data->y));
    set_course (course);
    me->p_flags &= ~(PFPLOCK | PFPLLOCK);
}

/******************************************************************************/
/***  Key108()                                                              ***/
/******************************************************************************/
void
Key108 (W_Event * data)
{
    target =
        gettarget (data->Window, data->x, data->y, TARG_PLAYER | TARG_PLANET);

	if (target->o_num == -1)
		return;

    if (target->o_type == PLAYERTYPE)
    {
        if (players[target->o_num].p_team != me->p_team)
        {
            /* PFSEEN is not sent to client, so we have to check
             * if player's coordinates are (-10000,-10000) to know
             * wether we can see him or not
             */
            if (players[target->o_num].p_x != -10000 &&
                players[target->o_num].p_y != -10000)
            {
                sendPlaylockReq (target->o_num);
                me->p_playerl = (short) (target->o_num);
            }
        }
        else
        {
            sendPlaylockReq (target->o_num);
            me->p_playerl = (short) (target->o_num);
        }
    }
    else
    {                           /* It's a planet */
        sendPlanlockReq (target->o_num);
        me->p_planet = (short) (target->o_num);
    }
}

/******************************************************************************/
/***  Key109()                                                              ***/
/******************************************************************************/
void
Key109 (void)
{

#ifdef SOUND
    Play_Sound(MESSAGE_WAV, SF_MESSAGE);
#endif

    message_on ();
}

/******************************************************************************/
/***  Key110()                                                              ***/
/******************************************************************************/
void
Key110 (W_Event * data)
{
    emptyKey ();
}

/******************************************************************************/
/***  Key111()                                                              ***/
/******************************************************************************/
void
Key111 (void)
{

#ifdef AUTOKEY
    if (autoKey)
        autoKeyOrbitReqOn ();
    else
        sendOrbitReq (1);
#else
    sendOrbitReq (1);
#endif /* AUTOKEY */

}

/******************************************************************************/
/***  Key112()                                                              ***/
/******************************************************************************/
void
Key112 (W_Event * data)
{
#ifdef AUTOKEY
    if (autoKey)
        autoKeyPhaserReqOn ();
    else
        phaseraction(data);
#else
    phaseraction (data);
#endif /* AUTOKEY */

}

/******************************************************************************/
/***  Key113()                                                              ***/
/******************************************************************************/
void
Key113 (void)
{

#ifdef SOUND
    Play_Sound(SELF_DESTRUCT_WAV, SF_INFO);
#endif

    fastQuit = 1;
    sendQuitReq ();
}

/******************************************************************************/
/***  Key114()                                                              ***/
/******************************************************************************/
void
Key114 (void)
{
    localflags |= PFREFIT;
    warning(blk_refitstring);
}

/******************************************************************************/
/***  Key115()                                                              ***/
/******************************************************************************/
void
Key115 (void)
{
    shield_tog ();
}

/******************************************************************************/
/***  Key116()                                                              ***/
/******************************************************************************/
void
Key116 (W_Event * data)
{
    unsigned char course;

#ifdef AUTOKEY
    if (autoKey)
        autoKeyTorpReqOn ();
    else
    {
        course = getcourse (data->Window, data->x, data->y);
        sendTorpReq (course);
    }
#else
    course = (unsigned char) (getcourse (data->Window, data->x, data->y));
    sendTorpReq (course);
#endif /* AUTOKEY */
}

/******************************************************************************/
/***  Key117()                                                              ***/
/******************************************************************************/
void
Key117 (void)
{
    shield_tog ();
}

/******************************************************************************/
/***  Key118()                                                              ***/
/******************************************************************************/
void
Key118 (W_Event * data)
{
#ifdef SOUND
    ChangeVolume (-1);
#else
    emptyKey ();
#endif
}

/******************************************************************************/
/***  Key119()                                                              ***/
/******************************************************************************/
void
Key119 (void)
{
    /* w = map war stuff */
    if (W_IsMapped (war))
        W_UnmapWindow (war);
    else
        warwindow ();

}

/******************************************************************************/
/***  Key120()                                                              ***/
/******************************************************************************/
void
Key120 (void)
{

#ifdef AUTOKEY
    if (autoKey)
        autoKeyBeamDownReqOn ();
    else
#endif

        beam_down ();

}

/******************************************************************************/
/***  Key121()                                                              ***/
/******************************************************************************/
void
Key121 (W_Event * data)
{
    if (me->p_flags & (PFTRACT | PFPRESS))
    {
        sendTractorReq (0, (char) (me->p_no));
        return;
    }
    target = gettarget (data->Window, data->x, data->y, TARG_PLAYER);
	if (target->o_num == -1)
		return;
    me->p_tractor = (short) (target->o_num);
    if (key == 'T')
    {
        sendTractorReq (1, (char) (target->o_num));
    }
    else
    {
        sendRepressReq (1, (char) (target->o_num));
    }

}

/******************************************************************************/
/***  Key122()                                                              ***/
/******************************************************************************/
void
Key122 (void)
{

#ifdef AUTOKEY
    if (autoKey)
        autoKeyBeamUpReqOn ();
    else
#endif

        beam_up ();

}

/******************************************************************************/
/***  Key123()                                                              ***/
/******************************************************************************/
void
Key123 (void)
{
    cloak_on ();
}

/******************************************************************************/
/***  Key124()                                                              ***/
/******************************************************************************/
void
Key124 (void)
{
    sendShortReq (SPK_ALL, 1);
}

/******************************************************************************/
/***  Key125()                                                              ***/
/******************************************************************************/
void
Key125 (void)
{
    cloak_off ();
}

/******************************************************************************/
/***  Key126()                                                              ***/
/******************************************************************************/
void
Key126 (W_Event * data)
{

#ifdef SOUND
    if ((soundWin != NULL) && W_IsMapped (soundWin))
        sounddone ();
    else
        soundwindow ();
#else
    emptyKey ();
#endif
}

/******************************************************************************/
/***  Key127()                                                              ***/
/******************************************************************************/
void
Key127 (W_Event * data)
{
    emptyKey ();
}

/******************************************************************************/
/***  reread_defaults()                                                     ***/
/******************************************************************************/
void
reread_defaults (void)
{
    int i;
    char mbuf[80];

    if (strlen (defaultsFile) > 0)
    {
        sprintf (mbuf, "Re-reading  %s", defaultsFile);
        warning (mbuf);
        initDefaults (defaultsFile);
        resetdefaults ();
        /* Set Window Allowed Messages ( since it got moved to newwin() )*/
        for (i = 0; i < 6; i++)
            W_SetWAM (wam_windows[i]);
        initkeymap ();
    }
    else
    {
        warning ("no default file found");
    }
}

/******************************************************************************/
/***  Key180()                                                              ***/
/******************************************************************************/
void
Key180 (W_Event * data)
{
    emergency (take, data);
}

/******************************************************************************/
/***  Key212()                                                              ***/
/******************************************************************************/
void
Key212 (W_Event * data)
{
    emergency (take, data);
}

/******************************************************************************/
/***  Key187() - turn left 1/16 turn                                        ***/
/******************************************************************************/
void
Key187 (void)
{
    unsigned char course;

    /* Server must allow turn keys */
    if (!F_turn_keys) return;

    /* Observers can't turn */
    if (me->p_flags & PFOBSERV && !paradise)
       return;

    course = (unsigned char) (me->p_dir - 16);
    set_course (course);
    me->p_flags &= ~(PFPLOCK | PFPLLOCK);
}

/******************************************************************************/
/***  Key189() - turn right 1/16 turn                                       ***/
/******************************************************************************/
void
Key189 (void)
{
    unsigned char course;

    /* Server must allow turn keys */
    if (!F_turn_keys) return;

    /* Observers can't turn */
    if (me->p_flags & PFOBSERV && !paradise)
       return;

    course = (unsigned char) (me->p_dir + 16);
    set_course (course);
    me->p_flags &= ~(PFPLOCK | PFPLLOCK);
}

/******************************************************************************/
/***  Key193()                                                              ***/
/******************************************************************************/
void
Key193 (void)
{
    scaleFactor -= 2;
    if (scaleFactor < 10)
        scaleFactor = 10;
    W_FastClear = 1;
    if (viewBox)
        redrawall = 1;
}

/******************************************************************************/
/***  Key218()                                                              ***/
/******************************************************************************/
void
Key218 (void)
{
    scaleFactor += 2;
    if (scaleFactor > 40)
        scaleFactor = 40;
    W_FastClear = 1;
    if (viewBox)
        redrawall = 1;
}

/******************************************************************************/
/***  Key207()                                                              ***/
/******************************************************************************/
void
Key207 (W_Event * data)
{
    emergency (ogg, data);
}

/******************************************************************************/
/***  Key194()                                                              ***/
/******************************************************************************/
void
Key194 (W_Event * data)
{
    emergency (bomb, data);
}

/******************************************************************************/
/***  Key195()                                                              ***/
/******************************************************************************/
void
Key195 (W_Event * data)
{
    emergency (space_control, data);
}

/******************************************************************************/
/***  Key145()                                                              ***/
/******************************************************************************/
void
Key145 (W_Event * data)
{
    emergency (save_planet, data);
}

/******************************************************************************/
/***  Key146()                                                              ***/
/******************************************************************************/
void
Key146 (W_Event * data)
{
    emergency (base_ogg, data);
}

/******************************************************************************/
/***  Key147()                                                              ***/
/******************************************************************************/
void
Key147 (W_Event * data)
{
    emergency (help1, data);
}

/******************************************************************************/
/***  Key148()                                                              ***/
/******************************************************************************/
void
Key148 (W_Event * data)
{
    emergency (help2, data);
}

/******************************************************************************/
/***  Key197()                                                              ***/
/******************************************************************************/
void
Key197 (W_Event * data)
{
    emergency (escorting, data);
}

/******************************************************************************/
/***  Key208()                                                              ***/
/******************************************************************************/
void
Key208 (W_Event * data)
{
    emergency (ogging, data);
}

/******************************************************************************/
/***  Key205()                                                              ***/
/******************************************************************************/
void
Key205 (W_Event * data)
{
    emergency (bombing, data);
}

/******************************************************************************/
/***  Key204()                                                              ***/
/******************************************************************************/
void
Key204 (W_Event * data)
{
    emergency (controlling, data);
}


/******************************************************************************/
/***  Key175()                                                              ***/
/******************************************************************************/
void
Key175 (W_Event * data)
{
    emergency (ogging, data);
}

/******************************************************************************/
/***  Key162()                                                              ***/
/******************************************************************************/
void
Key162 (W_Event * data)
{
    emergency (bombing, data);
}

/******************************************************************************/
/***  Key163()                                                              ***/
/******************************************************************************/
void
Key163 (W_Event * data)
{
    emergency (controlling, data);
}

/******************************************************************************/
/***  Key149()                                                              ***/
/******************************************************************************/
void
Key149 (W_Event * data)
{
    emergency (asw, data);
}

/******************************************************************************/
/***  Key150()                                                              ***/
/******************************************************************************/
void
Key150 (W_Event * data)
{
    emergency (asbomb, data);
}

/******************************************************************************/
/***  Key151()                                                              ***/
/******************************************************************************/
void
Key151 (W_Event * data)
{
    emergency (doing1, data);
}

/******************************************************************************/
/***  Key152()                                                              ***/
/******************************************************************************/
void
Key152 (W_Event * data)
{
    emergency (doing2, data);
}

/******************************************************************************/
/***  Key198()                                                              ***/
/******************************************************************************/
void
Key198 (W_Event * data)
{
    emergency (free_beer, data);
}

/******************************************************************************/
/***  Key206()                                                              ***/
/******************************************************************************/
void
Key206 (W_Event * data)
{
    emergency (no_gas, data);
}

/******************************************************************************/
/***  Key200()                                                              ***/
/******************************************************************************/
void
Key200 (W_Event * data)
{
    emergency (crippled, data);
}

/******************************************************************************/
/***  Key153()                                                              ***/
/******************************************************************************/
void
Key153 (W_Event * data)
{
    emergency (pickup, data);
}

/******************************************************************************/
/***  Key144()                                                              ***/
/******************************************************************************/
void
Key144 (W_Event * data)
{
    emergency (pop, data);
}

/******************************************************************************/
/***  Key160()                                                              ***/
/******************************************************************************/
void
Key160 (W_Event * data)
{
    emergency (other1, data);
}

/******************************************************************************/
/***  Key131()                                                              ***/
/******************************************************************************/
void
Key131 (W_Event * data)
{
    emergency (other2, data);
}

/******************************************************************************/
/***  Key202()                                                              ***/
/******************************************************************************/
void
Key202 (W_Event * data)
{
    if (paradise)
    {
        /* suspend warp toggle [BDyess] */
        if (me->p_flags & PFWPSUSPENDED)
            set_speed(96); /* unsuspend */
        else
            set_speed(97); /* suspend */
    }
    else
      emptyKey ();
}