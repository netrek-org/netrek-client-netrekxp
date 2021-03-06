/******************************************************************************/
/***  File:  death.c                                                        ***/
/***                                                                        ***/
/***  Function:                                                             ***/
/***                                                                        ***/
/***  Revisions:                                                            ***/
/***    ssheldon - Cleaned up source code, added #include "proto.h"         ***/
/***               and function header comments                             ***/
/******************************************************************************/

#include <setjmp.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>

#include "config.h"
#include "copyright.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

extern jmp_buf env;
W_Font deathFont;

static char *teamstring[9] = { "", "and the Feds",
    "and the Roms", "",
    "and the Klis", "", "", "",
    "and the Oris"
};

/******************************************************************************/
/***  death()                                                               ***/
/******************************************************************************/
void
death (void)
{
    W_Event event;
    int i;
    ingame = 0;
#ifdef AUTOKEY
    if (autoKey)
        autoKeyAllOff ();
#endif /* AUTOKEY */


    W_ClearWindow (w);
    W_ClearWindow (mapw);
    if (oldalert != PFGREEN)
    {
        if (extraAlertBorder)
        {
            W_ChangeBorder (w, gColor);
            W_ChangeBorder (mapw, gColor);
            alertBorderColor = gColor;
        }
        else
            alertBorderColor = W_White;
        W_ChangeBorder (baseWin, gColor);
        oldalert = PFGREEN;
        /* Need a border refresh on death with double buffering, as the
           W_ChangeBorder above does not change the double buffered local
           or map windows, but rather than non-double buffered "entry" window*/
        forceBorder = 1;
    }
    if (W_IsMapped (statwin))
    {
        W_UnmapWindow (statwin);
        showStats = 1;
    }
    else
    {
        showStats = 0;
    }
    if (infomapped)
        destroyInfo ();
//    W_UnmapWindow (planetw);
    W_UnmapWindow (rankw);
    W_UnmapWindow (war);
    if (optionWin)
        optiondone ();

#ifdef SOUND
    if (soundWin)
        sounddone ();
#endif

    if (promoted)
    {
        if (mystats->st_rank >= nranks)
            sprintf (rankmessage, "Congratulations, you have been promoted!");
        else
            sprintf (rankmessage, "Congratulations, you have scummed up to %s!",
                 paradise ? ranks2[mystats->st_rank].name : ranks[mystats->st_rank].name);
        W_WriteText (w, 50, 80, W_Yellow, rankmessage,
                     strlen (rankmessage), W_BoldFont);
    }


    deathFont = W_RegularFont;

#ifdef DEBUG
    LineToConsole ("Death cause: %d\n", me->p_whydead);
#endif

    switch (me->p_whydead)
    {
    case KQUIT:
        strcpy (deathmessage, "           You QUIT!!");
        break;
    case KTORP:
        sprintf (deathmessage,
                 "You were thumped by a photon torpedo from %s (%c%c).",
                 players[me->p_whodead].p_name,
                 teamlet[players[me->p_whodead].p_team],
                 shipnos[me->p_whodead]);
        break;
    case KPHASER:
        sprintf (deathmessage,
                 "You were phasered to death by %s (%c%c).",
                 players[me->p_whodead].p_name,
                 teamlet[players[me->p_whodead].p_team],
                 shipnos[me->p_whodead]);
        break;
    case KPLANET:
    	/* different message if killed by a star [BDyess] */
        if(paradise && planets[me->p_whodead].pl_flags && PLSTAR)
            sprintf (deathmessage,
                     "You were burned to a crisp by %s [star]",
                     planets[me->p_whodead].pl_name);
	else
            sprintf (deathmessage,
                     "You were killed by planetary fire from %s (%c).",
                     planets[me->p_whodead].pl_name,
                     teamlet[planets[me->p_whodead].pl_owner]);
        break;
    case KSHIP:
        sprintf (deathmessage,
                 "You were killed by the explosion of %s (%c%c).",
                 players[me->p_whodead].p_name,
                 teamlet[players[me->p_whodead].p_team],
                 shipnos[me->p_whodead]);
        break;
    case KDAEMON:
        strcpy (deathmessage, "You were killed by a dying daemon.");
        break;
    case KWINNER:
        sprintf (deathmessage, "Galaxy has been conquered by %s (%c%c) %s.",
                 players[me->p_whodead].p_name,
                 teamlet[players[me->p_whodead].p_team],
                 shipnos[players[me->p_whodead].p_no],
                 teamstring[players[me->p_whodead].p_team]);
        deathFont = W_BoldFont;
        srand ((unsigned) time (NULL));
        i = RANDOM() % 2;
        switch (i)
        {
            case 0:
                W_WriteScaleBitmap(0, 0, GWINSIDE, GWINSIDE,
                                   BMP_GENO_WIDTH,
                                   BMP_GENO_HEIGHT,
                                   0, genopic, foreColor, mapw);
                break;
            case 1:
            default:
                W_WriteScaleBitmap(0, 0, GWINSIDE, GWINSIDE,
                                   BMP_GENO_WIDTH,
                                   BMP_GENO_HEIGHT,
                                   0, genopic2, foreColor, mapw);
                break;
        }
#ifdef SOUND
        i = MUSIC_OFFSET + (RANDOM() % (NUM_MUSIC - MUSIC_OFFSET)); // Play random end theme
        Play_Music(i);
#endif
        break;
    case KGHOST:
        strcpy (deathmessage, "You were killed by a confused daemon.");
        break;
    case KGENOCIDE:
        sprintf (deathmessage,
                 "You were GENOCIDED by %s (%c%c) %s. You suck!",
                 players[me->p_whodead].p_name,
                 teamlet[players[me->p_whodead].p_team],
                 shipnos[me->p_whodead],
                 teamstring[players[me->p_whodead].p_team]);
        deathFont = W_BoldFont;
        srand ((unsigned) time (NULL));
        i = RANDOM() % 2;
        switch (i)
        {
            case 0:
                W_TileWindow (mapw, genopic);
                break;
            case 1:
            default:
                W_TileWindow (mapw, genopic2);
                break;
        }
#ifdef SOUND
        i = MUSIC_OFFSET + (RANDOM() % (NUM_MUSIC - MUSIC_OFFSET)); // Play random end theme
        Play_Music(i);
#endif
        break;
    case KPROVIDENCE:
        strcpy (deathmessage, "You were nuked by GOD.");
        break;
    case KPLASMA:
        sprintf (deathmessage,
                 "You were SMACKed by a plasma torpedo from %s (%c%c).",
                 players[me->p_whodead].p_name,
                 teamlet[players[me->p_whodead].p_team],
                 shipnos[me->p_whodead]);
        break;
    case TOURNEND:
        strcpy (deathmessage, "The tournament game has ended.");
        break;
    case KOVER:
        strcpy (deathmessage, "The game is over!");
        break;
    case TOURNSTART:
        strcpy (deathmessage, "The tournament game has begun!");
        break;
    case KBADBIN:
        strcpy (deathmessage, "Your netrek executable didn't verify correctly.");
        break;
    /* Unfortunately the numbering of some whydead messages is inconsistent between
       paradise and Vanilla.  KTORP2 overlaps with KMISSILE */
    case KTORP2:
        if (paradise)
            sprintf (deathmessage, "You were killed by a missile from %s (%c%c).",
                     players[me->p_whodead].p_name,
                     teamlet[players[me->p_whodead].p_team],
                     shipnos[me->p_whodead]);
        else
            strcpy (deathmessage, "You were killed by detonated torpedo.");
        break;
    /* Another Vanilla/paradise overlap problem.  KSHIP2 overlaps with KASTEROID */
    case KSHIP2:
        if (paradise)
            sprintf(deathmessage, "You were smashed to bits by an asteroid.");
        else
            strcpy (deathmessage, "You were killed by chain reaction explosion.");
        break;
    case KPLASMA2:
        strcpy (deathmessage, "You were killed by zapped plasma.");
        break;
    default:
        strcpy (deathmessage, "You were killed by something unknown to this game?");
        break;
    }

    W_WriteText (w, 50, 60, W_Cyan, deathmessage, strlen (deathmessage),
                 deathFont);


    /* This is a good time to expire all the torps and phasors that
       we have missed the TFREE and PFREE packes for. */
    resetWeaponInfo ();


#ifndef THREADED
    while (W_EventsPending ())
        W_NextEvent (&event);

    longjmp (env, 0);
#else
    if (!playback) /* If we are not playing back a recorded game, do this */
        terminate2 (0);
    else
    {                           /* Otherwise we aren't within a thread, so... */
        while (W_EventsPending ())
            W_NextEvent (&event);
        
        ingame = 0;
        longjmp (env, 0);
    }
#endif /* Threaded */
}

/******************************************************************************/
/***  updatedeath()                                                         ***/
/******************************************************************************/
void
updatedeath (void)
{
    if (deathFont != W_BoldFont)        /* Initialise deathFont */
        deathFont = W_RegularFont;

    if (promoted)
        W_WriteText (w, 50, 80, W_Yellow, rankmessage, strlen (rankmessage), W_BoldFont);
    W_WriteText (w, 50, 60, W_Cyan, deathmessage, strlen (deathmessage), deathFont);
}
