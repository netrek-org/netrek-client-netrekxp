/******************************************************************************/
/***  File:  dmessage.c                                                     ***/
/***                                                                        ***/
/***  Function: for the client of a socket based protocol.                  ***/
/***                                                                        ***/
/***  Revisions:                                                            ***/
/***    ssheldon - Cleaned up source code, added #include "proto.h"         ***/
/***               and function header comments                             ***/
/******************************************************************************/

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

#include "config.h"
#include "copyright.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "version.h"
#include "proto.h"

static int version_sent = 0;

/******************************************************************************/
/***  dmessage()                                                            ***/
/******************************************************************************/
void
dmessage (char *message,
          unsigned char flags,
          unsigned char from,
          unsigned char to)
{
    register int len;
    W_Color color;
    char timebuf[10];
    time_t curtime;
    struct tm *tm;
    int take, destroy, team, kill, killp, killa, bomb, conq;
    struct distress dist;

    take = MTEAM + MTAKE + MVALID;
    destroy = MTEAM + MDEST + MVALID;
    kill = MALL + MKILL + MVALID;
    killp = MALL + MKILLP + MVALID;
    killa = MALL + MKILLA + MVALID;
    bomb = MTEAM + MBOMB + MVALID;
    team = MTEAM + MVALID;
    conq = MALL + MCONQ + MVALID;

    time (&curtime);
    tm = localtime (&curtime);
    sprintf (timebuf, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
    len = strlen (message);
    if (from == 255)
    {
        /* From God */
        color = W_God;
    }
    else
    {
        color = playerColor (&(players[from]));
    }

    /* aha! A new type distress/macro call came in. parse it appropriately */
    // Not quite sure why this paradise check is here..but apparently it's needed.
    // This check needs to be further investigated on the current paradise
    // server code, for bombing messages and RCDs.  BB 03/11/08
    if ((paradise ? gen_distress : 1) && flags == (MTEAM | MDISTR | MVALID))
    {
        HandleGenDistr (message, from, to, &dist);
        len = makedistress (&dist, message, distmacro[dist.distype].macro);

#ifdef BEEPLITE
        if (useLite)
	    rcdlite(&dist);
#endif
        if (len <= 0)
            return;
        flags ^= MDISTR;
    }


    if (!paradise && niftyNewMessages)
    {
        if (logging)
        {
            if (logFile != NULL)
            {
                fprintf (logFile, "%s: %s\n", timebuf, message);
                fflush (logFile);
            }
            else
            {
                LineToConsole ("%s: %s\n", timebuf, message);
            }
        }
        if (!(logging && logFile == NULL) && flags == conq)
        {
            /* output conquer stuff to stdout in addition to message window */
            LineToConsole ("%s\n", message);
            if (instr (message, "kill"))
            {
                LineToConsole ("NOTE: The server here does not properly set message flags\n");
                LineToConsole ("You should probably pester the server god to update....\n");
            }
        }
        if (flags == (MCONFIG + MINDIV + MVALID))
            return;

        if ((flags == team) || (flags == take) || (flags == destroy))
        {
            W_MessageAllowedWindows (WAM_TEAM, 0, 0, color, message, len, shipFont (me));
            if ((flags == team) &&
                !strncmp (message + 10, "     ", 5) && (message[15] == 0))
            {
                LineToConsole ("dmessage:flags==team PIG call from=%d\n", from);
                pmessage (pigcall, from, MINDIV);
            }
        }

        else if ((flags == kill) || (flags == killp) ||
                 (flags == killa) || (flags == bomb))
        {
            if (!reportKills)
                return;         /* HW */
                
            W_MessageAllowedWindows (WAM_KILL, 0, 0, color, message, len, 0);
            
#ifdef SOUND          
            if (why_dead &&
               (instr (message, "[plasma]") || instr (message, "[zapped plasma]")))
            	Play_Sound(PLASMA_KILL_WAV, SF_INFO);
#endif

        }

        else if (flags & MINDIV)
        {
            if (beepOnPrivateMessage && me->p_no != from && from != 255)
                MessageBeep (MB_ICONASTERISK);

            W_MessageAllowedWindows (WAM_INDIV, 0, 0, color, message, len, 0);
            if (!strncmp (message + 10, "     ", 5) && (message[15] == 0))
            {
                LineToConsole ("dmessage:MINDIV PIG call from=%d\n", from);
                pmessage (pigcall, from, MINDIV);
            }
#ifdef CLUECHECKBORG
            if (from == 255)
            {
                LineToConsole ("dmessage: Message from God %s\n", message);
                ClueRespond (message);
            }
#endif
        }
        else
        {                       /* if we don't know where
                                 * the message beLONGs by
                                 * this time, stick it in
                                 * the all board... */
            W_MessageAllowedWindows (WAM_ALL, 0, 0, color, message, len, 0);
            if (!strncmp (message + 10, "     ", 5) && (message[15] == 0))
            {
                pmessage (pigcall, from, MINDIV);
            }
        }

    }
    else
    {

        /* ok, here we have the old kludge nastiness that we can turn on if we
         * HAVE to.  yuk, blech, ptooie... */

        if ((strncmp (message, "GOD->ALL", 8) == 0 &&
             (instr (message, "was kill") ||
              instr (message, "killed by")) ||
              instr (message, "Credit for")) ||
	      instr (message, "burned to a crisp by") ||
	      instr (message, "shot down by") ||
            (*message != ' ' && instr (message, "We are being attacked")))
        {
            if (!reportKills)
                return;
            
            W_MessageAllowedWindows (WAM_KILL, 0, 0, color, message, len, 0);
            
#ifdef SOUND          
            if (why_dead &&
               (instr (message, "[plasma]") || instr (message, "[zapped plasma]")))
                Play_Sound(PLASMA_KILL_WAV, SF_INFO);
#endif

            if (logging)
            {
                if (logFile != NULL)
                {
                    fprintf (logFile, "%s ", timebuf);
                    fprintf (logFile, "%s\n", message);
                    fflush (logFile);
                }
                else
                {
                    LineToConsole ("%s ", message);
                    LineToConsole ("%s\n", timebuf);
                }
            }
            return;
        }
        switch (flags & (MTEAM | MINDIV | MALL))
        {
        case MTEAM:
            W_MessageAllowedWindows (WAM_TEAM, 0, 0, color, message, len, 0);
            if (!strncmp (message + 10, "     ", 5) && (message[15] == 0))
            {
                pmessage (pigcall, from, MINDIV);
            }
            if (logging)
            {
                if (logFile != NULL)
                {
                    fprintf (logFile, "%s ", timebuf);
                    fprintf (logFile, "%s\n", message);
                    fflush (logFile);
                }
                else
                {
                    LineToConsole ("%s ", message);
                    LineToConsole ("%s\n", timebuf);
                }
            }
            break;
        case MINDIV:
            if (!(flags & MCONFIG))
			{
				if (beepOnPrivateMessage && me->p_no != from && from != 255)
					MessageBeep (MB_ICONASTERISK);

                W_MessageAllowedWindows (WAM_INDIV, 0, 0, color, message, len, 0);
			}
            if (!strncmp (message + 10, "     ", 5) && (message[15] == 0))
            {
                pmessage (pigcall, from, MINDIV);
            }
            if (logging)
            {
                if (logFile != NULL)
                {
                    fprintf (logFile, "%s ", timebuf);
                    fprintf (logFile, "%s\n", message);
                    fflush (logFile);
                }
                else
                {
                    LineToConsole ("%s ", message);
                    LineToConsole ("%s\n", timebuf);
                }
            }
            break;
        default:
            W_MessageAllowedWindows (WAM_ALL, 0, 0, color, message, len, 0);
            if (!strncmp (message + 10, "     ", 5) && (message[15] == 0))
            {
                pmessage (pigcall, from, MINDIV);
            }
            if (logging)
            {
                if (logFile != NULL)
                {
                    fprintf (logFile, "%s ", timebuf);
                    fprintf (logFile, "%s\n", message);
                    fflush (logFile);
                }
                else
                {
                    LineToConsole ("%s", message);
                    LineToConsole ("%s\n", timebuf);
                }
            }
            break;
        }
    }
}

/******************************************************************************/
/***  instr()                                                               ***/
/******************************************************************************/
int
instr (char *string1,
       char *string2)
{
    char *s;
    int length;

    length = strlen (string2);
    for (s = string1; *s != 0; s++)
    {
        if (*s == *string2 && strncmp (s, string2, length) == 0)
            return (1);
    }
    return (0);
}

/******************************************************************************/
/***  sendVersion()                                                         ***/
/******************************************************************************/
void
sendVersion (void)
{
    char client_ver[80];

    if (!version_sent)
    {
        version_sent = 1;
        sprintf (client_ver, "@%s %s", version, mvers);

        pmessage (client_ver, (short) me->p_no, MINDIV | MCONFIG);
    }
}

#if CLUECHECKBORG
/******************************************************************************/
/***  ClueRespond()                                                         ***/
/******************************************************************************/
void
ClueRespond (char *message)
{
    char buf[80];
    FILE *cluefile;
    char response[20];

    cluefile = fopen ("clue.txt", "r");

    while (fgets (buf, 79, cluefile))
    {
        buf[strlen (buf) - 1] = '\0';
        fgets (response, 20, cluefile);
        response[strlen (response) - 1] = '\0';


        if (instr (message, buf))
        {
            pmessage (response, me->p_no, MINDIV);
            break;
        }

        fgets (buf, 79, cluefile);
    }
    fclose (cluefile);
}
#endif /* CLUECHECKBORG */
