/* beeplite.c
 *
 */

#include "config.h"
#ifdef BEEPLITE
#include "copyright.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>
#include "Wlib.h"

#include "defs.h"
#include "struct.h"
#include "data.h"
#include "map.h"
#include "proto.h"
#include "SDL.h"
#include "SDL_mixer.h"

void rcdlite(struct distress *dist)
/* the info */
{
    char message[100];
    int len = 0;
    
    if ((dist == NULL) ||
    /* (dist->sender == me->p_no) || */
        (distlite[dist->distype] == NULL))

      return;

    len = makedistress(dist, message, distlite[dist->distype]);

    if (len <= 0)
      return;

    makelite(dist, message);
}


void litedefaults(void)
{
    if (distlite[take] == NULL)
      distlite[take] = "/c/l";
    if (distlite[base_ogg] == NULL)
      distlite[base_ogg] = "/g/m";
    if (distlite[pickup] == NULL)
      distlite[pickup] = "/p";
    if (distlite[help] == NULL)
      distlite[help] = "%?%S=SB%{/c%}";
}

void liteplanet(struct planet *l)
{
    beep_lite_cycle_time_planet = 10 * server_ups / 5;
    emph_planet_seq_n[l->pl_no] = beep_lite_cycle_time_planet;
    l->pl_flags |= PLREDRAW;			 /* Leave redraw on until * * 
						  * done highlighting */
}

void liteplayer(struct player *j)
{
    if (!j || (j->p_flags & PFCLOAK) || (!paradise && (j->p_flags & PFOBSERV)))
      return;

    redrawPlayer[j->p_no] = 1;

    beep_lite_cycle_time_player = 10 * server_ups / 5;
    emph_player_seq_n[j->p_no] = beep_lite_cycle_time_player;
}


/* small permutation on makedistress.  Searches for the highliting *
 * arguments, ignores everything else. */

int makelite(struct distress * dist, char *pm)
/* the info */
/* macro to parse, used for distress and macro */
{
    struct player *sender;
    struct player *j;
    struct planet *l;
    char   *strcap(char *s);
    char    c;


    sender = &players[dist->sender];

    if (!(*pm))
    {
      return (0);
    }

    /* first step is to substitute variables */
    while (*pm)
    {
        if (*pm == '/')
	{
	    pm++;

	    if (!pm)
	      continue;

	    switch (c = *(pm++))
	    {

	    case 'P':				 /* push player id into buf */
	    case 'G':				 /* push friendly player id * 
						  * 
						  * * into buf */
	    case 'H':				 /* push enemy target player
						  * * * id into buf */

	    case 'p':				 /* push player id into buf */
	    case 'g':				 /* push friendly player id * 
						  * 
						  * * into buf */
	    case 'h':				 /* push enemy target player
						  * * * id into buf */

	        switch (c)
		{
		case 'p':
		    j = &players[dist->tclose_j];
		    break;
		case 'g':
		    j = &players[dist->tclose_fr];
		    break;
		case 'h':
		    j = &players[dist->tclose_en];
		    break;
		case 'P':
		    j = &players[dist->close_j];
		    break;
		case 'G':
		    j = &players[dist->close_fr];
		    break;
		default:
		    j = &players[dist->close_en];
		    break;
		}
	        liteflag = (F_beeplite_flags & (LITE_PLAYERS_MAP | LITE_PLAYERS_LOCAL));
	        liteplayer(j);
	        break;

	    case 'B':				 /* highlites planet nearest
						  * * * sender */
	    case 'b':
	        l = &planets[dist->close_pl];
	        if (F_beeplite_flags & LITE_PLANETS)
		  liteplanet(l);
	        break;
	    case 'L':				 /* highlites planet nearest
						  * * * pointer */
	    case 'l':
	        l = &planets[dist->tclose_pl];
	        if (F_beeplite_flags & LITE_PLANETS)
		  liteplanet(l);
	        break;
	    case 'U':				 /* highlites enemy nearest * 
						  * 
						  * * pointer */
	    case 'u':
	        j = &players[dist->tclose_en];
	        liteflag = (F_beeplite_flags & (LITE_PLAYERS_MAP | LITE_PLAYERS_LOCAL));
	        liteplayer(j);
	        break;
	    case 'c':				 /* highlites sender */
	    case 'I':
	    case 'i':
	        liteflag = (F_beeplite_flags & (LITE_PLAYERS_MAP | LITE_PLAYERS_LOCAL));
	        liteplayer(sender);
	        break;
	    case 'M':				 /* highlites me */
	    case 'm':
	        if (F_beeplite_flags & LITE_SELF)
		{
		    liteflag = (LITE_PLAYERS_MAP | LITE_PLAYERS_LOCAL);
		    liteplayer(me);
		}
	        break;
	    case '0':

	        if (F_beeplite_flags & LITE_SOUNDS)
		{

#if defined(SOUND)
                    if (sound_toggle)
                        Play_Sound(MESSAGE_WAV, SF_MESSAGE);
                    else
		        W_Beep();
#endif
		}
	        break;

/* SDL sounds sent to default (MESSAGE_WAV) for sounds not in client yet */
#ifdef SOUND
	    case '1':
	        if (F_beeplite_flags & LITE_SOUNDS)
	          Play_Sound(MESSAGE1_WAV, SF_MESSAGE);
	        break;
	    case '2':
	        if (F_beeplite_flags & LITE_SOUNDS)
	          Play_Sound(MESSAGE2_WAV, SF_MESSAGE);
	        break;
	    case '3':
  	        if (F_beeplite_flags & LITE_SOUNDS)
	          Play_Sound(MESSAGE3_WAV, SF_MESSAGE);
	        break;
	    case '4':
	        if (F_beeplite_flags & LITE_SOUNDS)
	          Play_Sound(MESSAGE4_WAV, SF_MESSAGE);
	        break;
	    case '5':
	        if (F_beeplite_flags & LITE_SOUNDS)
	          Play_Sound(MESSAGE5_WAV, SF_MESSAGE);
	        break;
	    case '6':
	        if (F_beeplite_flags & LITE_SOUNDS)
	          Play_Sound(MESSAGE6_WAV, SF_MESSAGE);
	        break;
	    case '7':
	        if (F_beeplite_flags & LITE_SOUNDS)
	          Play_Sound(MESSAGE7_WAV, SF_MESSAGE);
	        break;
	    case '8':
	        if (F_beeplite_flags & LITE_SOUNDS)
	          Play_Sound(MESSAGE_WAV, SF_MESSAGE);
	        break;
	    case '9':
	        if (F_beeplite_flags & LITE_SOUNDS)
	          Play_Sound(MESSAGE_WAV, SF_MESSAGE);
	        break;
#endif

	      /* Text between:  /|    |   will be displayed with TTS */
	    case '|':
	        if (F_beeplite_flags & LITE_TTS)
		{
		    for (tts_len = 0; (*pm != '|' && tts_len < tts_max_len); tts_len++)
		      lastIn[tts_len] = *pm++;
		    lastIn[tts_len] = '\0';
		    tts_timer = tts_time * server_ups / 5;
		}
	        break;

	    default:
	        /* try to continue * bad macro character is skipped entirely, *
	         * * * the message will be parsed without whatever argument has
	         * * * occurred. - jn */
	        warning("Bad Macro character in distress!");
	        fprintf(stderr, "Unrecognizable special character in distress pass 1: %c\n", *(pm - 1));
	        break;
	    }
	}
        else
	{
	    pm++;
	}

    }

  return (1);
}
#endif /* BEEPLITE */
