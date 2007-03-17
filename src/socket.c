/******************************************************************************/
/***  File:  socket.c
/***
/***  Function: Routines to allow connection to the xtrek server.
/***
/***  Revisions:
/***    Kevin P. Smith 1/29/89 UDP stuff v1.0 by Andy McFadden  Feb-Apr 1992
/***    UDP protocol v1.0
/******************************************************************************/


#include "config.h"
#include "copyright2.h"

#include <stdio.h>
#include <sys/types.h>
#include <winsock.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <process.h>

#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "packets.h"
#include "wtext.h"
#include "playerlist.h"
#include "map.h"
#include "local.h"
#include "proto.h"

/* #define INCLUDE_SCAN            /* include Amdahl scanning beams */
#define INCLUDE_VISTRACT        /* include visible tractor beams */

#define NETSTAT

#ifdef GATEWAY
/* (these values are now defined in "main.c":) char *gw_mach        =
 * "charon";     |client gateway; strcmp(serverName) int   gw_serv_port   =
 * 5000;         |what to tell the server to use int   gw_port        = 5001;
 * |where we will contact gw int   gw_local_port  = 5100;         |where we
 * expect gw to contact us
 * 
 * The client binds to "5100" and sends "5000" to the server (TCP).  The server
 * sees that and sends a UDP packet to gw on port udp5000, which passes it
 * through to port udp5100 on the client.  The client-gw gets the server's
 * host and port from recvfrom.  (The client can't use the same method since
 * these sockets are one-way only, so it connect()s to gw_port (udp5001) on
 * the gateway machine regardless of what the server sends.)
 * 
 * So all we need in .gwrc is: udp 5000 5001 tde.uts 5100
 * 
 * assuming the client is on tde.uts.  Note that a UDP declaration will work for
 * ANY server, but you need one per player, and the client has to have the
 * port numbers in advance.
 * 
 * If we're using a standard server, we're set.  If we're running through a
 * gatewayed server, we have to do some unpleasant work on the server side... */
#endif


void
dummy (void)
{
}

#ifdef SHORT_PACKETS
char numofbits[256] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1,
    2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1,
    2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2,
    3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1,
    2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2,
    3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2,
    3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3,
    4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1,
    2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2,
    3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2,
    3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3,
    4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2,
    3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3,
    4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3,
    4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4,
    5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

static int vtsize[9] = { 4, 8, 8, 12, 12, 16, 20, 20, 24 };     /* How big is the torppacket */
static int vtdata[9] = { 0, 3, 5, 7, 9, 12, 14, 16, 18 };       /* How big is Torpdata */
int vtisize[9] = { 4, 7, 9, 11, 13, 16, 18, 20, 22 };   /* 4 byte Header + torpdata */

/* S_P2 */
int shortversion = SHORTVERSION;        /* Which version do we use? */

#endif /* SHORT_PACKETS */

#ifdef PACKET_LOG
void Log_Packet (char type,
                 int act_size);
int ALL_BYTES = 0;              /* To log all bytes */

#endif

struct packet_handler handlers[] = {
    {0, NULL},                  /* record 0 */
    {sizeof (struct mesg_spacket), handleMessage},      /* SP_MESSAGE */
    {sizeof (struct plyr_info_spacket), handlePlyrInfo},        /* SP_PLAYER_INFO */
    {sizeof (struct kills_spacket), handleKills},       /* SP_KILLS */
    {sizeof (struct player_spacket), handlePlayer},     /* SP_PLAYER */
    {sizeof (struct torp_info_spacket), handleTorpInfo},        /* SP_TORP_INFO */
    {sizeof (struct torp_spacket), handleTorp}, /* SP_TORP */
    {sizeof (struct phaser_spacket), handlePhaser},     /* SP_PHASER */
    {sizeof (struct plasma_info_spacket), handlePlasmaInfo},    /* SP_PLASMA_INFO */
    {sizeof (struct plasma_spacket), handlePlasma},     /* SP_PLASMA */
    {sizeof (struct warning_spacket), handleWarning},   /* SP_WARNING */
    {sizeof (struct motd_spacket), handleMotd}, /* SP_MOTD */
    {sizeof (struct you_spacket), handleSelf},  /* SP_YOU */
    {sizeof (struct queue_spacket), handleQueue},       /* SP_QUEUE */
    {sizeof (struct status_spacket), handleStatus},     /* SP_STATUS */
    {sizeof (struct planet_spacket), handlePlanet},     /* SP_PLANET */
    {sizeof (struct pickok_spacket), handlePickok},     /* SP_PICKOK */
    {sizeof (struct login_spacket), handleLogin},       /* SP_LOGIN */
    {sizeof (struct flags_spacket), handleFlags},       /* SP_FLAGS */
    {sizeof (struct mask_spacket), handleMask}, /* SP_MASK */
    {sizeof (struct pstatus_spacket), handlePStatus},   /* SP_PSTATUS */
    {sizeof (struct badversion_spacket), handleBadVersion},     /* SP_BADVERSION */
    {sizeof (struct hostile_spacket), handleHostile},   /* SP_HOSTILE */
    {sizeof (struct stats_spacket), handleStats},       /* SP_STATS */
    {sizeof (struct plyr_login_spacket), handlePlyrLogin},      /* SP_PL_LOGIN */
    {sizeof (struct reserved_spacket), handleReserved}, /* SP_RESERVED */
    {sizeof (struct planet_loc_spacket), handlePlanetLoc},      /* SP_PLANET_LOC */

#ifdef HANDLE_SCAN
    {sizeof (struct scan_spacket), handleScan}  /* SP_SCAN (ATM) */
#else
    {0, dummy},                 /* won't be called */
#endif

    {sizeof (struct udp_reply_spacket), handleUdpReply},        /* SP_UDP_STAT */
    {sizeof (struct sequence_spacket), handleSequence}, /* SP_SEQUENCE */
    {sizeof (struct sc_sequence_spacket), handleSequence},      /* SP_SC_SEQUENCE */

#ifdef RSA
    {sizeof (struct rsa_key_spacket), handleRSAKey},    /* SP_RSA_KEY */
#else
    {0, dummy},                 /* #31, and dummy won't */
#endif

    {sizeof (struct generic_32_spacket), handleGeneric32},  /* SP_GENERIC_32 */
    {0, dummy},                 /* 33 */
    {0, dummy},                 /* 34 */
    {0, dummy},                 /* 35 */
    {0, dummy},                 /* 36 */
    {0, dummy},                 /* 37 */
    {0, dummy},                 /* 38 */
    {sizeof (struct ship_cap_spacket), handleShipCap},  /* SP_SHIP_CAP */

#ifdef SHORT_PACKETS
    {sizeof (struct shortreply_spacket), handleShortReply},     /* SP_S_REPLY */
    {-1, handleSMessage},       /* SP_S_MESSAGE */
    {-1                         /* sizeof(struct
                                   warning_s_spacket) */ , handleSWarning},
    /* SP_S_WARNING */
    {sizeof (struct youshort_spacket), handleSelfShort},        /* SP_S_YOU */
    {sizeof (struct youss_spacket), handleSelfShip},    /* SP_S_YOU_SS */
    {-1, /* variable */ handleVPlayer}, /* SP_S_PLAYER */
#else
    {0, dummy},                 /* 40 */
    {0, dummy},                 /* 41 */
    {0, dummy},                 /* 42 */
    {0, dummy},                 /* 43 */
    {0, dummy},                 /* 44 */
    {0, dummy},                 /* 45 */
#endif
    {sizeof (struct ping_spacket), handlePing}, /* SP_PING */

#ifdef SHORT_PACKETS
    {-1, /* variable */ handleVTorp},   /* SP_S_TORP */
    {-1, handleVTorpInfo},      /* SP_S_TORP_INFO */
    {20, handleVTorp},          /* SP_S_8_TORP */
    {-1, handleVPlanet},        /* SP_S_PLANET */
#else
    {0, dummy},                 /* 47 */
    {0, dummy},                 /* 48 */
    {0, dummy},                 /* 49 */
    {0, dummy},                 /* 50 */
#endif

    {0, dummy},                 /* 51 */
    {0, dummy},                 /* 52 */
    {0, dummy},                 /* 53 */
    {0, dummy},                 /* 54 */
    {0, dummy},                 /* 55 */

#ifdef SHORT_PACKETS            /* S_P2 */
    {0, dummy},                 /* SP_S_SEQUENCE not yet
                                 * implemented */
    {-1, handleVPhaser},        /* SP_S_PHASER */
    {-1, handleVKills},         /* SP_S_KILLS */
    {sizeof (struct stats_s_spacket), handle_s_Stats},  /* SP_S_STATS */
#else
    {0, dummy},                 /* 56 */
    {0, dummy},                 /* 57 */
    {0, dummy},                 /* 58 */
    {0, dummy},                 /* 59 */
#endif

#ifdef FEATURE_PACKETS
    {sizeof (struct feature_cpacket), handleFeature},   /* CP_FEATURE; 60 */
#endif

};

int sizes[] = {
    0,                          /* record 0 */
    sizeof (struct mesg_cpacket),       /* CP_MESSAGE */
    sizeof (struct speed_cpacket),      /* CP_SPEED */
    sizeof (struct dir_cpacket),        /* CP_DIRECTION */
    sizeof (struct phaser_cpacket),     /* CP_PHASER */
    sizeof (struct plasma_cpacket),     /* CP_PLASMA */
    sizeof (struct torp_cpacket),       /* CP_TORP */
    sizeof (struct quit_cpacket),       /* CP_QUIT */
    sizeof (struct login_cpacket),      /* CP_LOGIN */
    sizeof (struct outfit_cpacket),     /* CP_OUTFIT */
    sizeof (struct war_cpacket),        /* CP_WAR */
    sizeof (struct practr_cpacket),     /* CP_PRACTR */
    sizeof (struct shield_cpacket),     /* CP_SHIELD */
    sizeof (struct repair_cpacket),     /* CP_REPAIR */
    sizeof (struct orbit_cpacket),      /* CP_ORBIT */
    sizeof (struct planlock_cpacket),   /* CP_PLANLOCK */
    sizeof (struct playlock_cpacket),   /* CP_PLAYLOCK */
    sizeof (struct bomb_cpacket),       /* CP_BOMB */
    sizeof (struct beam_cpacket),       /* CP_BEAM */
    sizeof (struct cloak_cpacket),      /* CP_CLOAK */
    sizeof (struct det_torps_cpacket),  /* CP_DET_TORPS */
    sizeof (struct det_mytorp_cpacket), /* CP_DET_MYTORP */
    sizeof (struct copilot_cpacket),    /* CP_COPILOT */
    sizeof (struct refit_cpacket),      /* CP_REFIT */
    sizeof (struct tractor_cpacket),    /* CP_TRACTOR */
    sizeof (struct repress_cpacket),    /* CP_REPRESS */
    sizeof (struct coup_cpacket),       /* CP_COUP */
    sizeof (struct socket_cpacket),     /* CP_SOCKET */
    sizeof (struct options_cpacket),    /* CP_OPTIONS */
    sizeof (struct bye_cpacket),        /* CP_BYE */
    sizeof (struct dockperm_cpacket),   /* CP_DOCKPERM */
    sizeof (struct updates_cpacket),    /* CP_UPDATES */
    sizeof (struct resetstats_cpacket), /* CP_RESETSTATS */
    sizeof (struct reserved_cpacket),   /* CP_RESERVED */

#ifdef INCLUDE_SCAN
    sizeof (struct scan_cpacket),       /* CP_SCAN (ATM) */
#else
    0,
#endif
    sizeof (struct udp_req_cpacket),    /* CP_UDP_REQ */
    sizeof (struct sequence_cpacket),   /* CP_SEQUENCE */

#ifdef RSA
    sizeof (struct rsa_key_cpacket),    /* CP_RSA_KEY */
#else
    0,                          /* 37 */
#endif

    0,                          /* 38 */
    0,                          /* 39 */
    0,                          /* 40 */
    0,                          /* 41 */

#ifdef PING
    sizeof (struct ping_cpacket),       /* CP_PING_RESPONSE */
#else
    0,
#endif

#ifdef SHORT_PACKETS
    sizeof (struct shortreq_cpacket),   /* CP_S_REQ */
    sizeof (struct threshold_cpacket),  /* CP_S_THRS */
    -1,                         /* CP_S_MESSAGE */
#else
    0,                          /* 43 */
    0,                          /* 44 */
    0,                          /* 45 */
#endif

    0,                          /* 46 */
    0,                          /* 47 */
    0,                          /* 48 */
    0,                          /* 49 */
    0,                          /* 50 */
    0,                          /* 51 */
    0,                          /* 52 */
    0,                          /* 53 */
    0,                          /* 54 */
    0,                          /* 55 */
    0,                          /* 56 */
    0,                          /* 57 */
    0,                          /* 58 */
    0,                          /* 59 */

#ifdef FEATURE_PACKETS
    sizeof (struct feature_cpacket),    /* CP_FEATURE; 60 */
#endif

};

#define NUM_PACKETS (sizeof(handlers) / sizeof(handlers[0]) - 1)
#define NUM_SIZES (sizeof(sizes) / sizeof(sizes[0]) - 1)


#ifdef PACKET_LOG
/* stuff useful for logging server packets to see how much bandwidth * netrek
 * is really using */
int log_packets = 0;            /* whether or not to be
                                 * logging packets */
int packet_log[NUM_PACKETS];    /* number of packets logged */
int outpacket_log[NUM_SIZES];

#endif /* PACKET_LOG */

int serverDead = 0;

#define BUFSIZE 2048
char buf[BUFSIZE];

static int udpLocalPort = 0;
static int udpServerPort = 0;
static LONG serveraddr = 0;
static LONG sequence = 0;
static int drop_flag = 0;
static int chan = -1;           /* tells sequence checker
                                 * where packet is from */
static short fSpeed, fDirection, fShield, fOrbit, fRepair, fBeamup, fBeamdown,
    fCloak, fBomb, fDockperm, fPhaser, fPlasma, fPlayLock, fPlanLock,
    fTractor, fRepress;

/* print the SP_S_TORP* packets.  */
/* sbuf = pointer to packet buff  */
/* type=1 print SP_S_TORP         */
/* type=2 print SP_S_8_TORP       */
/* type=3 print SP_S_TORP_INFO    */
void print_sp_s_torp(char *sbuf, int type)
{
  unsigned char which, *data, infobitset, *infodata;
  unsigned char bitset;         
  int     dx, dy;
  int     shiftvar;
  int     i;
  char    status, war;
  register int shift = 0;                      /* How many torps are 
                                                * * extracted (for shifting) */
  if ( (type == 1) || (type == 2) )
    {
      /* now we must find the data ... :-) */
      if (sbuf[0] == SP_S_8_TORP)
	{						 /* MAX packet */
	  bitset = 0xff;
	  which = sbuf[1];
	  data = &sbuf[2];
	}
      else
	{						 /* Normal Packet */
	  bitset = sbuf[1];
	  which = sbuf[2];
	  data = &sbuf[3];
	}
      LineToConsole("  bitset=0x%0X, which=%d, ", bitset, which);
#ifdef CORRUPTED_PACKETS
      /* we probably should do something clever here - jmn */
#endif
      for (shift = 0, i = 0; i < 8; i++, bitset >>= 1)
	{
	  if (bitset & 01)
	    {
	      dx = (*data >> shift);
	      data++;
	      shiftvar = (unsigned char) *data;	 /* to silence gcc */
	      shiftvar <<= (8 - shift);
	      dx |= (shiftvar & 511);
	      shift++;
	      dy = (*data >> shift);
	      data++;
	      shiftvar = (unsigned char) *data;	 /* to silence gcc */
	      shiftvar <<= (8 - shift);
	      dy |= (shiftvar & 511);
	      shift++;
	      if (shift == 8)
		{
		  shift = 0;
		  data++;
		}
	      LineToConsole("dx=%d, dy=%d, ",dx, dy);
	    }
	}
    }
  else if (type == 3)
    {
      /* now we must find the data ... :-) */
      bitset = sbuf[1];
      which = sbuf[2];
      infobitset = sbuf[3];
      /* Where is the data ? */
      data = &sbuf[4];
      LineToConsole("  bitset=0x%0X, which=%d, infobitset=0x%0X, ",
	      bitset, which, infobitset);
      infodata = &sbuf[vtisize[numofbits[(unsigned char) sbuf[1]]]];
      for (shift = 0, i = 0; i < 8; bitset >>= 1, infobitset >>= 1, i++)
	{
	  if (bitset & 01)
	    {
	      dx = (*data >> shift);
	      data++;
	      shiftvar = (unsigned char) *data;	 /* to silence gcc */
	      shiftvar <<= (8 - shift);
	      dx |= (shiftvar & 511);
	      shift++;
	      dy = (*data >> shift);
	      data++;
	      shiftvar = (unsigned char) *data;	 /* to silence gcc */
	      shiftvar <<= (8 - shift);
	      dy |= (shiftvar & 511);
	      shift++;
	      if (shift == 8)
		{
		  shift = 0;
		  data++;
		}
	      LineToConsole("dx=%d, dy=%d, ",dx, dy);
	    } 
	  /* Now the TorpInfo */
	  if (infobitset & 01)
	    {
	      war = (unsigned char) *infodata & 15 /* 0x0f */ ;
	      status = ((unsigned char) *infodata & 0xf0) >> 4;
	      infodata++;
	      LineToConsole("war=0x%0X, status=0x%0X, ", war, status);
	    }					 /* if */
	}						 /* for */
    }
}

/* reset all the "force command" variables */
void
resetForce (void)
{
    fSpeed = fDirection = fShield = fOrbit = fRepair = fBeamup = fBeamdown =
        fCloak = fBomb = fDockperm = fPhaser = fPlasma = fPlayLock =
        fPlanLock = fTractor = fRepress = -1;
}

/* If something we want to happen hasn't yet, send it again.
 * 
 * The low byte is the request, the high byte is a max count.  When the max
 * count reaches zero, the client stops trying.  Checking is done with a
 * macro for speed & clarity. */
#define FCHECK_FLAGS(flag, force, const) {                      \
        if (force > 0) {                                        \
            if (((me->p_flags & flag) && 1) ^ ((force & 0xff) && 1)) {  \
                speedReq.type = const;                          \
                speedReq.speed = (force & 0xff);                \
                sendServerPacket((struct player_spacket *) &speedReq);  \
                V_UDPDIAG(("Forced %d:%d\n", const, force & 0xff));     \
                force -= 0x100;                                 \
                if (force < 0x100) force = -1;  /* give up */   \
            } else                                              \
                force = -1;                                     \
        }                                                       \
}
#define FCHECK_VAL(value, force, const) {                       \
        if (force > 0) {                                        \
            if ((value) != (force & 0xff)) {                    \
                speedReq.type = const;                          \
                speedReq.speed = (force & 0xff);                \
                sendServerPacket((struct player_spacket *) &speedReq);  \
                V_UDPDIAG(("Forced %d:%d\n", const, force & 0xff));     \
                force -= 0x100;                                 \
                if (force < 0x100) force = -1;  /* give up */   \
            } else                                              \
                force = -1;                                     \
        }                                                       \
}
#define FCHECK_TRACT(flag, force, const) {                      \
        if (force > 0) {                                        \
            if (((me->p_flags & flag) && 1) ^ ((force & 0xff) && 1)) {  \
                tractorReq.type = const;                        \
                tractorReq.state = ((force & 0xff) >= 0x40);    \
                tractorReq.pnum = (force & 0xff) & (~0x40);     \
                sendServerPacket((struct player_spacket *)&tractorReq); \
                V_UDPDIAG(("Forced %d:%d/%d\n", const,          \
                        tractorReq.state, tractorReq.pnum));    \
                force -= 0x100;                                 \
                if (force < 0x100) force = -1;  /* give up */   \
            } else                                              \
                force = -1;                                     \
        }                                                       \
}

void
checkForce (void)
{
    struct speed_cpacket speedReq;
    struct tractor_cpacket tractorReq;

    FCHECK_VAL (me->p_speed, fSpeed, CP_SPEED); /* almost always repeats */

    FCHECK_FLAGS (PFSHIELD, fShield, CP_SHIELD);
    FCHECK_FLAGS (PFORBIT, fOrbit, CP_ORBIT);
    FCHECK_FLAGS (PFREPAIR, fRepair, CP_REPAIR);
    FCHECK_FLAGS (PFBEAMUP, fBeamup, CP_BEAM);
    FCHECK_FLAGS (PFBEAMDOWN, fBeamdown, CP_BEAM);
    FCHECK_FLAGS (PFCLOAK, fCloak, CP_CLOAK);
    FCHECK_FLAGS (PFBOMB, fBomb, CP_BOMB);
    FCHECK_FLAGS (PFDOCKOK, fDockperm, CP_DOCKPERM);
    FCHECK_VAL (phasers[me->p_no].ph_status, fPhaser, CP_PHASER);       /* bug: dir 0 */
    FCHECK_VAL (plasmatorps[me->p_no].pt_status, fPlasma, CP_PLASMA);   /* (ditto) */
    FCHECK_FLAGS (PFPLOCK, fPlayLock, CP_PLAYLOCK);
    FCHECK_FLAGS (PFPLLOCK, fPlanLock, CP_PLANLOCK);

    FCHECK_TRACT (PFTRACT, fTractor, CP_TRACTOR);
    FCHECK_TRACT (PFPRESS, fRepress, CP_REPRESS);
}


void
setNoDelay (int fd)
{
    int status;
    int option_value = 1;

    status = setsockopt (fd, IPPROTO_TCP, TCP_NODELAY,
                         (char *) &option_value, sizeof (option_value));
    if (status < 0)
    {
        LineToConsole ("setsockopt() failed, %s\n", strerror (errno));
        /* can still play without this, proceed */
    }
}

void
connectToServer (int port)
{
    int s;
    struct sockaddr_in addr;
    struct sockaddr_in naddr;
    int len;
    fd_set readfds;
    struct timeval timeout;
    struct hostent *hp;
    int optval;

    serverDead = 0;
    if (sock != -1)
    {
        shutdown (sock, 2);
        closesocket (sock);
        sock = -1;
    }

    LineToConsole ("Waiting for connection (port %d). \n", port);

    if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
        LineToConsole ("I can't create a socket\n");
        terminate (2);
    }

    /* allow local address resuse */
    optval = 1;
    if (setsockopt
        (s, SOL_SOCKET, SO_REUSEADDR, (char *) &optval, sizeof (int)) < 0)
    {
        perror ("setsockopt");
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons ((u_short) port);

    if (bind (s, (struct sockaddr *) &addr, sizeof (addr)) < 0)
    {

        perror ("bind");        /* NEW */
        terminate (1);
    }
    if (listen (s, 1) < 0)
        perror ("listen");

    len = sizeof (naddr);

  tryagain:
    timeout.tv_sec = 240;       /* four minutes */
    timeout.tv_usec = 0;
    FD_ZERO (&readfds);
    FD_SET (s, &readfds);

    if (s >= max_fd)
        max_fd = s + 1;

    if (select (max_fd, &readfds, NULL, NULL, &timeout) == 0)
    {
        LineToConsole ("Well, I think the server died!\n");
        terminate (0);
    }

    sock = accept (s, (struct sockaddr *) &naddr, &len);

    if (sock == -1)
    {
        goto tryagain;
    }

    if (sock >= max_fd)
        max_fd = sock + 1;

    LineToConsole ("Got connection.\n");

    closesocket (s);
    setNoDelay (sock);
    pickSocket (port);          /* new socket != port */


    /* This is necessary; it tries to determine who the caller is, and set
     * "serverName" and "serveraddr" appropriately. */
    len = sizeof (struct sockaddr_in);

    if (getpeername (sock, (struct sockaddr *) &addr, &len) < 0)
    {
        perror ("unable to get peername");
        serverName = "nowhere";
    }
    else
    {
        serveraddr = addr.sin_addr.s_addr;
        hp = gethostbyaddr ((char *) &addr.sin_addr.s_addr, sizeof (LONG),
                            AF_INET);
        if (hp != NULL)
        {
            serverName = (char *) malloc (strlen (hp->h_name) + 1);
            strcpy (serverName, hp->h_name);
        }
        else
        {
            serverName =
                (char *) malloc (strlen (inet_ntoa (addr.sin_addr)) + 1);
            strcpy (serverName, inet_ntoa (addr.sin_addr));
        }
    }
    LineToConsole ("Connection from server %s (0x%x)\n", serverName, serveraddr);

}


void
callServer (int port,
            char *server)
{
    int s;
    struct sockaddr_in addr;
    struct hostent *hp;

    serverDead = 0;

    LineToConsole ("Calling %s on port %d\n", server, port);

    if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
        LineToConsole ("I can't create a socket\n");
        terminate (0);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons ((u_short) port);

    if ((addr.sin_addr.s_addr = inet_addr (server)) == -1)
    {
        if ((hp = gethostbyname (server)) == NULL)
        {
            LineToConsole ("Who is %s?\n", server);
            terminate (0);
        }
        else
        {
            addr.sin_addr.s_addr = *(LONG *) hp->h_addr;
        }
    }
    serveraddr = addr.sin_addr.s_addr;

    if (connect (s, (struct sockaddr *) &addr, sizeof (addr)) < 0)
    {
        LineToConsole ("Server not listening!\n");
        terminate (0);
    }
    LineToConsole ("Got connection.\n");

    sock = s;

    if (sock >= max_fd)
        max_fd = sock + 1;

#ifdef TREKHOPD
    /* We use a different scheme from gw: we tell the server who we want to
     * connect to and what our local UDP port is; it picks its own UDP ports
     * and tells us what they are.  This is MUCH more flexible and avoids a
     * number of problems, and may come in handy if the blessed scheme changes.
     * It's also slightly more work. */
    {
        extern int port_req, use_trekhopd, serv_port;
        extern char *host_req;
        struct mesg_cpacket msg;
        struct mesg_spacket reply;
        int n, count, *ip;
        char *buf;

        if (use_trekhopd)
        {
            msg.type = SP_MESSAGE;
            msg.group = msg.indiv = msg.pad1 = 0;
            ip = (int *) msg.mesg;
            *(ip++) = htons (port_req);
            *(ip++) = htons (gw_local_port);
            STRNCPY (msg.mesg + 8, login, 8);
            strcpy (msg.mesg + 16, host_req);
            if (gwrite (s, &msg, sizeof (struct mesg_cpacket)) < 0)
            {
                LineToConsole ("trekhopd init failure\n");
                terminate (1);
            }
            LineToConsole ("--- trekhopd request sent, awaiting reply\n");
            /* now block waiting for reply */
            count = sizeof (struct mesg_spacket);

            for (buf = (char *) &reply; count; buf += n, count -= n)
            {
                if ((n = recv (s, buf, count, 0)) <= 0)
                {
                    perror ("trekhopd read");
                    terminate (1);
                }
            }

            if (reply.type != SP_MESSAGE)
            {
                LineToConsole ("Got bogus reply from trekhopd (%d)\n",
                         reply.type);
                terminate (1);
            }
            ip = (int *) reply.mesg;
            gw_serv_port = ntohl (*ip++);
            gw_port = ntohl (*ip++);
            serv_port = ntohl (*ip++);
            LineToConsole ("--- trekhopd reply received\n");

            /* LineToConsole ("ports = %d/%d, %d\n", gw_serv_port, gw_port, serv_port); */
        }
    }
#endif /* TREKHOPD */

    pickSocket (port);          /* new socket != port */
}

int
isServerDead (void)
{
    return (serverDead);
}

void
socketPause (void)
{
    struct timeval timeout;
    fd_set readfds;

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    FD_ZERO (&readfds);
    FD_SET (sock, &readfds);
    if (udpSock >= 0)           /* new */
        FD_SET (udpSock, &readfds);

    select (max_fd, &readfds, 0, 0, &timeout);
}

int
readFromServer (fd_set * readfds)
{
    int retval = 0;

    if (serverDead)
        return (0);

    if (!readfds)
    {
        struct timeval timeout;
        int rs;
        fd_set mask;

        readfds = &mask;

//    tryagain:
        FD_ZERO (readfds);
        FD_SET (sock, readfds);
        if (udpSock >= 0)
            FD_SET (udpSock, readfds);
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        if ((rs = select (max_fd, readfds, 0, 0, &timeout)) == 0)
        {
            dotimers ();
            return 0;
        }
    }
    if (udpSock >= 0 && FD_ISSET (udpSock, readfds))
    {
        /* WAS V_ */
        UDPDIAG (("Activity on UDP socket\n"));
        chan = udpSock;
        if (commStatus == STAT_VERIFY_UDP)
        {
            warning ("UDP connection established");
            sequence = 0;       /* reset sequence #s */
            resetForce ();

            if (udpDebug)
                printUdpInfo ();
            UDPDIAG (("UDP connection established.\n"));

            commMode = COMM_UDP;
            commStatus = STAT_CONNECTED;
            commSwitchTimeout = 0;
            if (udpClientReceive != MODE_SIMPLE)
                sendUdpReq (COMM_MODE + udpClientReceive);
            if (udpWin)
            {
                udprefresh (UDP_CURRENT);
                udprefresh (UDP_STATUS);
            }
        }
        retval += doRead (udpSock);
    }

    /* Read info from the xtrek server */
    if (FD_ISSET (sock, readfds))
    {
        chan = sock;
        if (commMode == COMM_TCP)
            drop_flag = 0;      /* just in case */
        /* Bug fix for unnecessary redraws with UDP on - reported by TP */
        if (commMode == COMM_UDP)
            doRead (sock);
        else
            retval += doRead (sock);
    }

    dotimers ();
    return (retval != 0);       /* convert to 1/0 */
}

void
dotimers (void)
{
    /* if switching comm mode, decrement timeout counter */
    if (commSwitchTimeout > 0)
    {
        if (!(--commSwitchTimeout))
        {
            /* timed out; could be initial request to non-UDP server (which
             * won't be answered), or the verify packet got lost en route to
             * the server.  Could also be a request for TCP which timed out
             * (weird), in which case we just reset anyway. */
            commModeReq = commMode = COMM_TCP;
            commStatus = STAT_CONNECTED;
            if (udpSock >= 0)
                closeUdpConn ();
            if (udpWin)
            {
                udprefresh (UDP_CURRENT);
                udprefresh (UDP_STATUS);
            }
            warning ("Timed out waiting for UDP response from server");
            UDPDIAG (("Timed out waiting for UDP response from server\n"));
        }
    }
    /* if we're in a UDP "force" mode, check to see if we need to do something */
    if (udpClientSend > 1 && commMode == COMM_UDP)
        checkForce ();
}

int
getvpsize (char *bufptr)
{
    int size;

    switch (*bufptr)
    {
    case SP_S_MESSAGE:
        size = ((unsigned char) bufptr[4]);     /* IMPORTANT  Changed */
        break;
    case SP_S_WARNING:
        if ((unsigned char) bufptr[1] == STEXTE_STRING ||
            (unsigned char) bufptr[1] == SHORT_WARNING)
        {
            size = ((unsigned char) bufptr[3]);
        }
        else
            size = 4;           /* Normal Packet */
        break;
    case SP_S_PLAYER:
        if ((unsigned char) bufptr[1] & (unsigned char) 128)
        {                       /* Small +extended Header */
            size = ((unsigned char) (bufptr[1] & 0x3f) * 4) + 4;
        }
        else if ((unsigned char) bufptr[1] & 64)
        {                       /* Small Header */
            if (shortversion >= SHORTVERSION)   /* S_P2 */
                size =
                    ((unsigned char) (bufptr[1] & 0x3f) * 4) + 4 +
                    (bufptr[2] * 4);
            else
                size = ((unsigned char) (bufptr[1] & 0x3f) * 4) + 4;
        }
        else
        {                       /* Big Header */
            size = ((unsigned char) bufptr[1] * 4 + 12);
        }
        break;
    case SP_S_TORP:
        size = vtsize[numofbits[(unsigned char) bufptr[1]]];
        break;
    case SP_S_TORP_INFO:
        size = (vtisize[numofbits[(unsigned char) bufptr[1]]]
                + numofbits[(unsigned char) bufptr[3]]);
        break;
    case SP_S_PLANET:
        size = ((unsigned char) bufptr[1] * VPLANET_SIZE) + 2;
        break;
    case SP_S_PHASER:          /* S_P2 */
        switch ((unsigned char) bufptr[1] & 0x0f)
        {
        case PHFREE:
        case PHHIT:
        case PHMISS:
            size = 4;
            break;
        case PHHIT2:
            size = 8;
            break;
        default:
            size = sizeof (struct phaser_s_spacket);

            break;
        }
        break;
    case SP_S_KILLS:           /* S_P2 */
        size = ((unsigned char) bufptr[1] * 2) + 2;
        break;
    default:
        LineToConsole ("Unknown variable packet\n");
        /* terminate(1); */
        return -1;
        break;
    }
    if ((size % 4) != 0)
    {
        size += (4 - (size % 4));
    }

    return size;
}

int
doRead (int asock)
{
    struct timeval timeout;
    fd_set readfds;
    char *bufptr;
    int size;
    int count;
    int temp;

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    count =
        recv (asock, buf, BUFSIZE - /* space for packet frag */ BUFSIZE / 4,
              0);

    if (debug)
    {
        LineToConsole ("read %d bytes from %s socket\n",
                count, asock == udpSock ? "UDP" : "TCP");
    }

    if (count <= 0)
    {
        if (asock == udpSock)
        {
            if (WSAGetLastError () == WSAECONNREFUSED)
            {
                struct sockaddr_in addr;

                UDPDIAG (("asock=%d, sock=%d, udpSock=%d, errno=%d\n",
                          asock, sock, udpSock, errno));
                UDPDIAG (("count=%d\n", count));
                UDPDIAG (("Hiccup(%d)!  Reconnecting\n", errno));
                addr.sin_addr.s_addr = serveraddr;
                addr.sin_port = htons ((u_short) udpServerPort);
                addr.sin_family = AF_INET;
                if (connect
                    (udpSock, (struct sockaddr *) &addr, sizeof (addr)) < 0)
                {
                    perror ("connect");
                    UDPDIAG (("Unable to reconnect\n"));
                    /* and fall through to disconnect */
                }
                else
                {
                    UDPDIAG (("Reconnect successful\n"));
                    return (0);
                }
            }
            UDPDIAG (("*** UDP disconnected (res=%d, err=%d)\n",
                      count, errno));
            warning ("UDP link severed");
            printUdpInfo ();
            closeUdpConn ();
            commMode = commModeReq = COMM_TCP;
            commStatus = STAT_CONNECTED;
            if (udpWin)
            {
                udprefresh (UDP_STATUS);
                udprefresh (UDP_CURRENT);
            }
            return (0);
        }
        LineToConsole ("1) Got read() of %d. Server dead\n", count);
        perror ("1) read()");
        serverDead = 1;
        return (0);
    }
    bufptr = buf;
    while (bufptr < buf + count)
    {
        /* this goto label for a bug w/ short packets */
      computesize:
        if (*bufptr < 1 || *bufptr > NUM_PACKETS
            || handlers[*bufptr].size == 0)
        {
#ifndef CORRUPTED_PACKETS
            int i;
#endif

            LineToConsole ("Unknown packet type: %d\n", *bufptr);

#ifndef CORRUPTED_PACKETS
            LineToConsole ("count: %d, bufptr at %d,  Content:\n", count,
                            bufptr - buf);
            for (i = 0; i < count; i++)
            {
                LineToConsole ("0x%x, ", (unsigned int) buf[i]);
            }
#endif

            return (0);
        }
        size = handlers[*bufptr].size;

#ifdef SHORT_PACKETS
        if (size == -1)
        {                       /* variable packet */
            size = getvpsize (bufptr);
            if (size <= 0)
            {
                LineToConsole ("Bad short-packet size value (%d)\n", size);
                return 0;
            }

            if (debug)
            {
                LineToConsole ("read variable packet size %d, type %d\n",
                                size, *bufptr);
            }
        }
#endif /* SHORT_PACKETS */

        if (size == 0)
        {
            LineToConsole ("Variable packet has 0 length! type=%d Trying to read more!\n",
                          *bufptr);
        }
        /* read broke in the middle of a packet, wait until we get the rest */
        while (size > count + (buf - bufptr) || size == 0)
        {
            /* We wait for up to ten seconds for rest of packet. If we don't
             * get it, we assume the server died. */
//        tryagain1:
            timeout.tv_sec = 20;
            timeout.tv_usec = 0;
            FD_ZERO (&readfds);
            FD_SET (asock, &readfds);
            /* readfds=1<<asock; */
            if ((temp = select (max_fd, &readfds, 0, 0, &timeout)) == 0)
            {
                LineToConsole ("Packet fragment.  Server must be dead\n");
                serverDead = 1;
                return (0);
            }

            /* 88=largest short packet, messages */
            if (size == 0)
            {
                temp = recv (asock, buf + count, 88, 0);
            }
            else
                temp =
                    recv (asock, buf + count, size - (count + (buf - bufptr)),
                          0);
            count += temp;
            if (temp <= 0)
            {
                LineToConsole ("2) Got read() of %d.  Server is dead\n", temp);
                serverDead = 1;
                return (0);
            }
            /* go back to the size computation, hopefully with the rest of the */
            /* aborted packet in the buffer. */
            if (size == 0)
                goto computesize;
        }
        if (handlers[*bufptr].handler != NULL)
        {
            if (asock != udpSock ||
                (!drop_flag || *bufptr == SP_SEQUENCE
                 || *bufptr == SP_SC_SEQUENCE))
            {
                if (debug)
                {
                    LineToConsole ("read packet %d\n", *bufptr);
                }

#ifdef RECORDGAME
                if (recordFile != NULL && ckRecordPacket (*bufptr))
                {
                    if (fwrite (bufptr, size, 1, recordFile) != 1)
                    {
                        perror ("write: (recordFile)");
                        fclose (recordFile);
                        recordFile = NULL;
                    }
                }
#endif

#ifdef PACKET_LOG
                if (log_packets)
                {
                    (void) Log_Packet ((char) (*bufptr), size);
                    print_packet(bufptr, size);
                }
#endif

                if (asock == udpSock)
                    packets_received++;

                (*(handlers[*bufptr].handler)) (bufptr
#ifdef CORRUPTED_PACKETS
                                                , asock
#endif
                    );
            }
            else
            {
                if (debug)
                {
                    if (drop_flag)
                    {
                        LineToConsole ("%d bytes dropped.\n", size);
                    }
                }
                UDPDIAG (("Ignored type %d\n", *bufptr));
            }
        }
        else
        {
            LineToConsole ("Handler for packet %d not installed...\n", *bufptr);
        }

        bufptr += size;

    }
    return (1);
}

void
handleTorp (struct torp_spacket *packet)
{
    struct torp *thetorp;

#ifdef CORRUPTED_PACKETS
    if (ntohs (packet->tnum) >= MAXPLAYER * MAXTORP)
    {
        LineToConsole ("handleTorp: bad index %d\n", ntohs (packet->tnum));
        return;
    }
#endif

    weaponUpdate = 1;
    thetorp = &torps[ntohs (packet->tnum)];

    thetorp->t_x = ntohl (packet->x);
    thetorp->t_y = ntohl (packet->y);
    thetorp->t_dir = packet->dir;
    /* Updatefuse requires minimum value of 2 to ensure a redraw, updateFuse decremented
       before drawing torp in local.c */
    thetorp->t_updateFuse = MAX(2, TORP_UPDATE_FUSE * server_ups / 10);


#ifdef ROTATERACE
    if (rotate)
    {
        rotate_coord (&thetorp->t_x, &thetorp->t_y, rotate_deg,
                      GWIDTH / 2, GWIDTH / 2);
        rotate_dir (&thetorp->t_dir, rotate_deg);
    }
#endif
}

void
handleTorpInfo (struct torp_info_spacket *packet)
{
    struct torp *thetorp;

#ifdef CORRUPTED_PACKETS
    if (ntohs (packet->tnum) >= MAXPLAYER * MAXTORP)
    {
        LineToConsole ("handleTorpInfo: bad index %d\n", ntohs (packet->tnum));
        return;
    }
#endif

    weaponUpdate = 1;
    thetorp = &torps[ntohs (packet->tnum)];
    /* Updatefuse requires minimum value of 2 to ensure a redraw, updateFuse decremented
       before drawing torp in local.c */
    thetorp->t_updateFuse = MAX(2, TORP_UPDATE_FUSE * server_ups / 10);

    if (packet->status == TEXPLODE && thetorp->t_status == TFREE)
    {
        /* FAT: redundant explosion; don't update p_ntorp */
        /* LineToConsole ("texplode ignored\n"); */
        return;
    }

    if (packet->status && thetorp->t_status == TFREE)
    {
        players[thetorp->t_owner].p_ntorp++;

        /* BORG TEST */

#ifdef BD
        if (bd)
            bd_new_torp (ntohs (packet->tnum), thetorp);
#endif
    }
    if (packet->status == TFREE && thetorp->t_status)
    {
        players[thetorp->t_owner].p_ntorp--;
    }
    thetorp->t_war = packet->war;

    if (packet->status != thetorp->t_status)
    {
        /* FAT: prevent explosion reset */
        thetorp->t_status = packet->status;
        if (thetorp->t_status == TEXPLODE)
        {
            /* Fuse requires minimum value of 2 to ensure a redraw, fuse decremented
               before drawing torp in local.c */
            thetorp->t_fuse = MAX(2, BMP_TORPDET_FRAMES * server_ups / 10);
        }
    }
}

void
handleStatus (struct status_spacket *packet)
{
    status->tourn = packet->tourn;
    /* T-mode sounds - avoid playing on game entry by using
       placeholder setting for oldtourn */
    if (oldtourn != status->tourn && oldtourn != 2)
    {
#ifdef SOUND
#ifdef RECORDGAME
      if (!inplayback) {
#endif
        if (status->tourn)
            Play_Sound(START_TMODE_WAV, SF_INFO);
        else
            Play_Sound(STOP_TMODE_WAV, SF_INFO);
#ifdef RECORDGAME
      }
#endif	
#endif
    }
    oldtourn = status->tourn;
    status->armsbomb = ntohl (packet->armsbomb);
    status->planets = ntohl (packet->planets);
    status->kills = ntohl (packet->kills);
    status->losses = ntohl (packet->losses);
    status->time = ntohl (packet->time);
    status->timeprod = ntohl (packet->timeprod);

    if (debug)
    {
        LineToConsole ("SERVER STATS:\n");
        LineToConsole ("time: %d\n", status->time / (60 * 60 * 10));
        LineToConsole ("kills: %d\n", status->kills);
        LineToConsole ("losses: %d\n", status->losses);
        LineToConsole ("planets: %d\n", status->planets);
        LineToConsole ("armsbomb: %d\n", status->armsbomb);
    }
}

void
handleSelf (struct you_spacket *packet)
{

#ifdef CORRUPTED_PACKETS
    if (packet->pnum >= MAXPLAYER)
    {
        LineToConsole ("handleSelf: bad index %d\n", packet->pnum);
        return;
    }
#endif

    if (!F_many_self)
    {
        me = (ghoststart ? &players[ghost_pno] : &players[packet->pnum]);
        myship = &(me->p_ship);
        mystats = &(me->p_stats);
    }

#ifdef PLIST2
    if (me->p_hostile != packet->hostile)
    {
        me->p_hostile = packet->hostile;
        PlistNoteHostile (packet->pnum);
    }
#else
    me->p_hostile = packet->hostile;
#endif

    me->p_swar = packet->swar;
    me->p_armies = packet->armies;

    me->p_flags = ntohl (packet->flags);
    if (me->p_flags & PFPLOCK)
    {
        redrawPlayer[me->p_playerl] = 1;
    }
    me->p_damage = ntohl (packet->damage);
    me->p_shield = ntohl (packet->shield);
    me->p_fuel = ntohl (packet->fuel);
    me->p_etemp = ntohs (packet->etemp);
    me->p_wtemp = ntohs (packet->wtemp);
    me->p_whydead = ntohs (packet->whydead);
    me->p_whodead = ntohs (packet->whodead);

#ifdef INCLUDE_VISTRACT
    if (packet->tractor & 0x40)
        me->p_tractor = (short) ((short) packet->tractor & (~0x40));      /* ATM - visible
                                                                 * tractors */

#endif
}


void
handlePlayer (struct player_spacket *packet)
{
    register struct player *pl;
    int x, y;

#ifdef CORRUPTED_PACKETS
    if (packet->pnum >= MAXPLAYER)
    {
        LineToConsole ("handlePlayer: bad index %d\n", packet->pnum);
        return;
    }
#endif


    pl = &players[packet->pnum];

    pl->p_dir = packet->dir;
    pl->p_speed = packet->speed;
    PlistNoteSpeed (packet->pnum);

    if (F_cloak_maxwarp && pl != me)
    {
        if (pl->p_speed == 0xf)
            pl->p_flags |= PFCLOAK;
        else if (pl->p_flags & PFCLOAK)
            pl->p_flags &= ~PFCLOAK;
    }
    x = ntohl (packet->x);
    y = ntohl (packet->y);

#ifdef WARP_DEAD
    if (F_dead_warp && pl->p_speed == 14 && x == -10000 && y == -10000
        && (pl->p_status != PEXPLODE))
    {
        pl->p_status = PEXPLODE;
        x = pl->p_x;
        y = pl->p_y;
        if (pl->p_dir > DEADPACKETS)
            pl->p_explode = BMP_SHIPEXPL_FRAMES;
        else
            pl->p_explode = 0;
        redrawPlayer[packet->pnum] = 1;
        PlistNoteUpdate (packet->pnum);
    }
#endif

    pl->p_x = x;
    pl->p_y = y;

    redrawPlayer[packet->pnum] = 1;

    if (me == pl)
    {
        extern int my_x, my_y;  /* From short.c */

        my_x = x;
        my_y = y;
    }


#ifdef ROTATERACE
    if (rotate)
    {
        rotate_coord (&pl->p_x, &pl->p_y, rotate_deg, GWIDTH / 2, GWIDTH / 2);
        rotate_dir (&pl->p_dir, rotate_deg);
    }
#endif
}


void
handleWarning (struct warning_spacket *packet)
{
    packet->mesg[sizeof (packet->mesg) - 1] = '\0';     /* guarantee null
                                                         * termination */
    warning (packet->mesg);
}

void
sendShortPacket (char type,
                 char state)
{
    struct speed_cpacket speedReq;

    speedReq.type = type;
    speedReq.speed = state;
    sendServerPacket ((struct player_spacket *) &speedReq);

    /* if we're sending in UDP mode, be prepared to force it */
    if (commMode == COMM_UDP && udpClientSend >= 2)
    {
        switch (type)
        {
        case CP_SPEED:
            fSpeed = (short) (state | 0x100);
            break;
        case CP_DIRECTION:
            fDirection = (short) (state | 0x100);
            break;
        case CP_SHIELD:
            fShield = (short) (state | 0xa00);
            break;
        case CP_ORBIT:
            fOrbit = (short) (state | 0xa00);
            break;
        case CP_REPAIR:
            fRepair = (short) (state | 0xa00);
            break;
        case CP_CLOAK:
            fCloak = (short) (state | 0xa00);
            break;
        case CP_BOMB:
            fBomb = (short) (state | 0xa00);
            break;
        case CP_DOCKPERM:
            fDockperm = (short) (state | 0xa00);
            break;
        case CP_PLAYLOCK:
            fPlayLock = (short) (state | 0xa00);
            break;
        case CP_PLANLOCK:
            fPlanLock = (short) (state | 0xa00);
            break;
        case CP_BEAM:
            if (state == 1)
                fBeamup = 1 | 0x500;
            else
                fBeamdown = 2 | 0x500;
            break;
        }

        /* force weapons too? */
        if (udpClientSend >= 3)
        {
            switch (type)
            {
            case CP_PHASER:
                fPhaser = (short) (state | 0x100);
                break;
            case CP_PLASMA:
                fPlasma = (short) (state | 0x100);
                break;
            }
        }
    }
}

void
sendServerPacket (struct player_spacket *packet)
/* Pick a random type for the packet */
{
    int size;

    if (serverDead)
        return;
    if (packet->type < 1 || packet->type > NUM_SIZES
        || sizes[packet->type] == 0)
    {
        LineToConsole ("Attempt to send strange packet %d!\n", packet->type);
        return;
    }
    size = sizes[packet->type];

#ifdef PACKET_LOG
    if (log_packets)
    {
        Log_OPacket (packet->type, size);
        print_opacket((char *) packet,size);
    }
#endif

    if (commMode == COMM_UDP)
    {
        /* for now, just sent everything via TCP */
    }
    if (commMode == COMM_TCP || !udpClientSend)
    {
        /* special case for verify packet */
        if (packet->type == CP_UDP_REQ)
        {
            if (((struct udp_req_cpacket *) packet)->request == COMM_VERIFY)
                goto send_udp;
        }
        /* business as usual (or player has turned off UDP transmission) */
        if (gwrite (sock, (char *) packet, size) != size)
        {
            LineToConsole ("gwrite failed.  Server must be dead\n");
            serverDead = 1;
        }
    }
    else
    {
        /* UDP stuff */
        switch (packet->type)
        {
        case CP_SPEED:
        case CP_DIRECTION:
        case CP_PHASER:
        case CP_PLASMA:
        case CP_TORP:
        case CP_QUIT:
        case CP_PRACTR:
        case CP_SHIELD:
        case CP_REPAIR:
        case CP_ORBIT:
        case CP_PLANLOCK:
        case CP_PLAYLOCK:
        case CP_BOMB:
        case CP_BEAM:
        case CP_CLOAK:
        case CP_DET_TORPS:
        case CP_DET_MYTORP:
        case CP_REFIT:
        case CP_TRACTOR:
        case CP_REPRESS:
        case CP_COUP:
        case CP_DOCKPERM:

#ifdef INCLUDE_SCAN
        case CP_SCAN:
#endif

        case CP_PING_RESPONSE:
            /* non-critical stuff, use UDP */
          send_udp:
            packets_sent++;

            V_UDPDIAG (("Sent %d on UDP port\n", packet->type));
            if (gwrite (udpSock, (char *) packet, size) != size)
            {
                UDPDIAG (("gwrite on UDP failed.  Closing UDP connection\n"));
                warning ("UDP link severed");
                /* serverDead=1; */
                commModeReq = commMode = COMM_TCP;
                commStatus = STAT_CONNECTED;
                commSwitchTimeout = 0;
                if (udpWin)
                {
                    udprefresh (UDP_STATUS);
                    udprefresh (UDP_CURRENT);
                }
                if (udpSock >= 0)
                    closeUdpConn ();
            }
            break;

        default:
            /* critical stuff, use TCP */
            if (gwrite (sock, (char *) packet, size) != size)
            {
                LineToConsole ("gwrite failed.  Server must be dead\n");
                serverDead = 1;
            }
        }
    }
}

void
handlePlanet (struct planet_spacket *packet)
{
    struct planet *plan;

    /* FAT: prevent excessive redraw */
    int redraw = 0;

#ifdef CORRUPTED_PACKETS
    if (packet->pnum >= MAXPLANETS)
    {
        LineToConsole ("handlePlanet: bad index %d\n", packet->pnum);
        return;
    }
#endif

    plan = &planets[packet->pnum];

    if (plan->pl_owner != packet->owner)
        redraw = 1;
    plan->pl_owner = packet->owner;
    if (plan->pl_owner < FED || plan->pl_owner > ORI)
        plan->pl_owner = NOBODY;
    if (plan->pl_info != packet->info)
        redraw = 1;
    plan->pl_info = packet->info;
    /* Redraw the planet because it was updated by server */

    if (plan->pl_flags != (int) ntohs (packet->flags))
        redraw = 1;
    plan->pl_flags = (int) ntohs (packet->flags);

    if (plan->pl_armies != (signed int) ntohl (packet->armies))
    {
        /* don't redraw when armies change unless it crosses the '4' * army
         * limit. Keeps people from watching for planet 'flicker' * when
         * players are beaming */
        int planetarmies = ntohl (packet->armies);

        if ((plan->pl_armies < 5 && planetarmies > 4) ||
            (plan->pl_armies > 4 && planetarmies < 5))
            redraw = 1;
    }
    plan->pl_armies = ntohl (packet->armies);

#ifndef RECORDGAME
    if (plan->pl_info == 0)
    {
        plan->pl_owner = NOBODY;
    }
#endif
    if (redraw)
    {
        plan->pl_flags |= PLREDRAW;
    }
    if (W_IsMapped (planetw))    /* planet window */
        updatePlanetw ();
}

void
handlePhaser (struct phaser_spacket *packet)
{
    struct phaser *phas;

#ifdef CORRUPTED_PACKETS
    if (packet->pnum >= MAXPLAYER)
    {
        LineToConsole ("handlePhaser: bad index %d\n", packet->pnum);
        return;
    }
    if (packet->status == PHHIT &&
        (ntohl (packet->target) < 0 || ntohl (packet->target) >= MAXPLAYER))
    {
        LineToConsole ("handlePhaser: bad target %d\n", ntohl (packet->target));
        return;
    }
#endif

    weaponUpdate = 1;
    phas = &phasers[packet->pnum];
    phas->ph_status = packet->status;
    phas->ph_dir = packet->dir;
    phas->ph_x = ntohl (packet->x);
    phas->ph_y = ntohl (packet->y);
    phas->ph_target = (short) (ntohl (packet->target));
    phas->ph_fuse = 0;
#ifdef SOUND
    phas->sound_phaser = 1;
#endif
    /* normalized fuses */
    /* Updatefuse requires minimum value of 2 to ensure a redraw, updateFuse decremented
       before drawing phaser in local.c */
    phas->ph_updateFuse = MAX(2, PHASER_UPDATE_FUSE * server_ups / 10);
    phas->ph_maxfuse = MAX(1, (players[packet->pnum].p_ship.s_phaserfuse * server_ups) / 10);

#ifdef ROTATERACE
    if (rotate)
    {
        rotate_coord (&phas->ph_x, &phas->ph_y, rotate_deg, GWIDTH / 2,
                      GWIDTH / 2);
        rotate_dir (&phas->ph_dir, rotate_deg);
    }
#endif
}

void
handleMessage (struct mesg_spacket *packet)
{
    if (packet->m_from >= MAXPLAYER)
        packet->m_from = 255;

#ifdef CORRUPTED_PACKETS
    packet->mesg[sizeof (packet->mesg) - 1] = '\0';
#endif

    /* LineToConsole ("flags: 0x%x\n", packet->m_flags);
     * LineToConsole ("from: %d\n", * packet->m_from);
     * LineToConsole ("recpt: %d\n", packet->m_recpt);
     * LineToConsole ("mesg: * %s\n", packet->mesg); */


    dmessage (packet->mesg, packet->m_flags, packet->m_from, packet->m_recpt);
}

void
handleQueue (struct queue_spacket *packet)
{
    queuePos = ntohs (packet->pos);
}

void
sendTeamReq (int team,
             int ship)
{
    struct outfit_cpacket outfitReq;

    outfitReq.type = CP_OUTFIT;
    outfitReq.team = (char) team;
    outfitReq.ship = (char) ship;
    sendServerPacket ((struct player_spacket *) &outfitReq);
}

void
handlePickok (struct pickok_spacket *packet)
{
    pickOk = packet->state;
}

void
sendLoginReq (char *name,
              char *pass,
              char *login,
              char query)
{
    struct login_cpacket packet;

    strcpy (packet.name, name);
    strcpy (packet.password, pass);
    if (strlen (login) > 15)
        login[15] = 0;
    strcpy (packet.login, login);
    packet.type = CP_LOGIN;
    packet.query = query;
    //packet.pad2 = 0x69; /* Paradise support */
    //packet.pad3 = 0x43; /* Paradise support */
    sendServerPacket ((struct player_spacket *) &packet);
}

void
handleLogin (struct login_spacket *packet)
{
    loginAccept = packet->accept;
    if (packet->accept)
    {
        /* no longer needed .. we have it in xtrekrc bcopy(packet->keymap,
         * mystats->st_keymap, 96); */
        mystats->st_flags = ntohl (packet->flags);

    }
}

void
sendTractorReq (char state,
                char pnum)
{
    struct tractor_cpacket tractorReq;

    tractorReq.type = CP_TRACTOR;
    tractorReq.state = state;
    tractorReq.pnum = pnum;
    sendServerPacket ((struct player_spacket *) &tractorReq);

    if (state)
        fTractor = (short) (pnum | 0x40);
    else
        fTractor = 0;
}

void
sendRepressReq (char state,
                char pnum)
{
    struct repress_cpacket repressReq;

    repressReq.type = CP_REPRESS;
    repressReq.state = state;
    repressReq.pnum = pnum;
    sendServerPacket ((struct player_spacket *) &repressReq);

    if (state)
        fRepress = (short) (pnum | 0x40);
    else
        fRepress = 0;
}

void
sendDetMineReq (short int torp)
{
    struct det_mytorp_cpacket detReq;

    detReq.type = CP_DET_MYTORP;
    detReq.tnum = htons (torp);
    sendServerPacket ((struct player_spacket *) &detReq);
}

void
handlePlasmaInfo (struct plasma_info_spacket *packet)
{
    struct plasmatorp *thetorp;

#ifdef CORRUPTED_PACKETS
    if (ntohs (packet->pnum) >= MAXPLAYER * MAXPLASMA)
    {
        LineToConsole ("handlePlasmaInfo: bad index %d\n", packet->pnum);
        return;
    }
#endif

    weaponUpdate = 1;
    thetorp = &plasmatorps[ntohs (packet->pnum)];
    /* Updatefuse requires minimum value of 2 to ensure a redraw, updateFuse decremented
       before drawing torp in local.c */
    thetorp->pt_updateFuse = MAX(2, PLASMA_UPDATE_FUSE * server_ups / 10);
    if (packet->status == PTEXPLODE && thetorp->pt_status == PTFREE)
    {
        /* FAT: redundant explosion; don't update p_nplasmatorp */
        return;
    }
    if (!thetorp->pt_status && packet->status)
    {
        players[thetorp->pt_owner].p_nplasmatorp++;
    }
    if (thetorp->pt_status && !packet->status)
    {
        players[thetorp->pt_owner].p_nplasmatorp--;
    }
    thetorp->pt_war = packet->war;
    if (thetorp->pt_status != packet->status)
    {
        /* FAT: prevent explosion timer from being reset */
        thetorp->pt_status = packet->status;
        if (thetorp->pt_status == PTEXPLODE)
        {
            /* Fuse requires minimum value of 2 to ensure a redraw, fuse decremented
               before drawing torp in local.c */
            thetorp->pt_fuse = MAX(2, BMP_TORPDET_FRAMES * server_ups / 10);
        }
    }
}

void
handlePlasma (struct plasma_spacket *packet)
{
    struct plasmatorp *thetorp;

#ifdef CORRUPTED_PACKETS
    if (ntohs (packet->pnum) >= MAXPLAYER * MAXPLASMA)
    {
        LineToConsole ("handlePlasma: bad index %d\n", packet->pnum);
        return;
    }
#endif

    weaponUpdate = 1;
    thetorp = &plasmatorps[ntohs (packet->pnum)];
    thetorp->pt_x = ntohl (packet->x);
    thetorp->pt_y = ntohl (packet->y);
    /* Updatefuse requires minimum value of 2 to ensure a redraw, updateFuse decremented
       before drawing torp in local.c */
    thetorp->pt_updateFuse = MAX(2, PLASMA_UPDATE_FUSE * server_ups / 10);

#ifdef ROTATERACE
    if (rotate)
    {
        rotate_coord (&thetorp->pt_x, &thetorp->pt_y, rotate_deg, GWIDTH / 2,
                      GWIDTH / 2);
    }
#endif
}

void
handleFlags (struct flags_spacket *packet)
{

#ifdef CORRUPTED_PACKETS
    if (packet->pnum >= MAXPLAYER)
    {
        LineToConsole ("handleFlags: bad index %d\n", packet->pnum);
        return;
    }
#endif

    if (players[packet->pnum].p_flags != ntohl (packet->flags)
#ifdef INCLUDE_VISTRACT
        || players[packet->pnum].p_tractor !=
        ((short) packet->tractor & (~0x40))
#endif
        )
    {
        /* FAT: prevent redundant player update */
        redrawPlayer[packet->pnum] = 1;
    }
    else
        return;

    players[packet->pnum].p_flags = ntohl (packet->flags);

#ifdef INCLUDE_VISTRACT
    if (packet->tractor & 0x40)
        players[packet->pnum].p_tractor = (short) ((short) packet->tractor & (~0x40));    /* ATM - visible
                                                                                 * tractors */
    else
#endif /* INCLUDE_VISTRACT */

        players[packet->pnum].p_tractor = -1;
}

void
handleKills (struct kills_spacket *packet)
{

#ifdef CORRUPTED_PACKETS
    if (packet->pnum >= MAXPLAYER)
    {
        LineToConsole ("handleKills: bad index %d\n", packet->pnum);
        return;
    }
#endif

    if (players[packet->pnum].p_kills != ntohl (packet->kills) / 100.0)
    {
        players[packet->pnum].p_kills = (float) (ntohl (packet->kills) / 100.0);
        /* FAT: prevent redundant player update */
        PlistNoteUpdate (packet->pnum);

#ifdef ARMY_SLIDER
        if (me == &players[packet->pnum])
        {
            calibrate_stats ();
            redrawStats ();
        }
#endif /* ARMY_SLIDER */
    }
}

void
handlePStatus (struct pstatus_spacket *packet)
{
    register struct player *j;

#ifdef CORRUPTED_PACKETS
    if (packet->pnum >= MAXPLAYER)
    {
        LineToConsole ("handlePStatus: bad index %d\n", packet->pnum);
        return;
    }
#endif

    j = &players[packet->pnum];

    if (packet->status == j->p_status)
        return;

    if (packet->status == PEXPLODE)
    {
        j->p_explode = 0;
    }

    /* Ignore DEAD status. Instead, we treat it as PEXPLODE. This gives us time
     * to animate all the frames necessary for the explosions at our own pace. */
    else if (packet->status == PDEAD)
    {
        packet->status = PEXPLODE;

        if (j->p_status == PEXPLODE)    /* Prevent redundant updates */
            return;
    }

    /* guarantees we won't miss the kill message since this is TCP */
    else if (packet->status == PALIVE && j->p_status != PALIVE)
        j->p_kills = 0.;

    /* update the player list, especially if this signals a new arrival */
    PlistNoteUpdate (packet->pnum);
    if (j->p_status == PFREE || packet->status == PFREE)
        PlistNoteArrive (packet->pnum);

    j->p_status = packet->status;
    redrawPlayer[packet->pnum] = 1;
}

void
handleMotd (struct motd_spacket *packet)
{
    packet->line[sizeof (packet->line) - 1] = '\0';
    newMotdLine (packet->line);
}

void
sendMessage (char *mes,
             int group,
             int indiv)
{
    struct mesg_cpacket mesPacket;

#ifdef SHORT_PACKETS
    if (recv_short)
    {
        int size;

        size = strlen (mes);
        size += 5;              /* 1 for '\0', 4
                                 * packetheader */
        if ((size % 4) != 0)
            size += (4 - (size % 4));
        mesPacket.pad1 = (char) size;
        sizes[CP_S_MESSAGE] = size;
        mesPacket.type = CP_S_MESSAGE;
    }
    else
#endif

        mesPacket.type = CP_MESSAGE;
    mesPacket.group = (char) group;
    mesPacket.indiv = (char) indiv;
    STRNCPY (mesPacket.mesg, mes, 80);
    sendServerPacket ((struct player_spacket *) &mesPacket);
}


void
handleMask (struct mask_spacket *packet)
{
    tournMask = packet->mask;
}

void
sendOptionsPacket (void)
{
    struct options_cpacket optPacket;

    optPacket.type = CP_OPTIONS;
    optPacket.flags = htonl (ST_MAPMODE +       /* always on */
                             ST_NAMEMODE * showPlanetNames + ST_SHOWSHIELDS +  /* always on */
                             ST_KEEPPEACE * keepPeace + ST_SHOWLOCAL * 1 +      /* This client no longer supports */
                             ST_SHOWGLOBAL * 1);        /* showlocal and showgalactic, set to 1 */
    MCOPY (mystats->st_keymap, optPacket.keymap, 96);
    sendServerPacket ((struct player_spacket *) &optPacket);
}

void
pickSocket (int old)
{
    int newsocket;
    struct socket_cpacket sockPack;

    newsocket = (getpid () & 32767);
    while (newsocket < 2048 || newsocket == old)
    {
        newsocket = (newsocket + 10687) & 32767;
    }
    sockPack.type = CP_SOCKET;
    sockPack.socket = htonl (newsocket);
    sockPack.version = (char) SOCKVERSION;
    sockPack.udp_version = (char) UDPVERSION;
    sendServerPacket ((struct player_spacket *) &sockPack);
    /* Did we get new socket # sent? */
    if (serverDead)
        return;
    nextSocket = newsocket;
}

void
handleBadVersion (struct badversion_spacket *packet)
{
    switch (packet->why)
    {
    case 0:
        LineToConsole ("Sorry, this is an invalid client version.\n");
        LineToConsole ("You need a new version of the client code.\n");
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        LineToConsole ("Sorry, but you cannot play netrek now.\n");
        LineToConsole ("Try again later.\n");
        break;
    default:
        LineToConsole ("Unknown message from handleBadVersion.\n");
        return;
    }
    terminate (1);
}

long
gwrite (int fd, char *buf, register int bytes)
{
    LONG orig = bytes;
    register LONG n;

    while (bytes)
    {
        n = send (fd, buf, bytes, 0);
        if (n < 0)
        {
            if (fd == udpSock)
            {
                LineToConsole ("Tried to write %d, 0x%x, %d\n", fd, buf, bytes);
                perror ("write");
                printUdpInfo ();
            }
            return (-1);
        }
        bytes -= n;
        buf += n;
    }
    return (orig);
}

void
handleHostile (struct hostile_spacket *packet)
{
    register struct player *pl;

#ifdef CORRUPTED_PACKETS
    if (packet->pnum >= MAXPLAYER)
    {
        LineToConsole ("handleHostile: bad index %d\n", packet->pnum);
        return;
    }
#endif

    pl = &players[packet->pnum];
    if (pl->p_swar != packet->war || pl->p_hostile != packet->hostile)
    {
        /* FAT: prevent redundant player update & redraw */
        pl->p_swar = packet->war;
        pl->p_hostile = packet->hostile;
        PlistNoteHostile (packet->pnum);
        redrawPlayer[packet->pnum] = 1;
    }
}

void
handlePlyrLogin (struct plyr_login_spacket *packet,
                 int sock)
{
    register struct player *pl;

#ifdef CORRUPTED_PACKETS
    if (sock == udpSock)
    {
        LineToConsole ("garbage packet discarded.\n");
        return;
    }
#endif

#ifdef CORRUPTED_PACKETS
    if (packet->pnum >= MAXPLAYER)
    {
        LineToConsole ("handlePlyrLogin: bad index %d\n", packet->pnum);
        return;
    }
    if (packet->rank >= NUMRANKS)
    {
        LineToConsole ("handlePlyrLogin: bad rank %d\n", packet->rank);
        return;
    }
    packet->name[sizeof (packet->name) - 1] = '\0';
    packet->monitor[sizeof (packet->monitor) - 1] = '\0';
    packet->login[sizeof (packet->login) - 1] = '\0';
#endif

    pl = &players[packet->pnum];

    strcpy (pl->p_name, packet->name);
    strcpyp_return (pl->p_monitor, packet->monitor, sizeof (pl->p_monitor));
    strcpy (pl->p_login, packet->login);
    pl->p_stats.st_rank = packet->rank;

    /* LineToConsole ("read player login %s, %s, %s\n", pl->p_name, pl->p_monitor,
     * pl->p_login); */

    if (packet->pnum == me->p_no)
    {
        /* This is me.  Set some stats */
        if (lastRank == -1)
        {
            if (loggedIn)
            {
                lastRank = packet->rank;
            }
        }
        else
        {
            if (lastRank != packet->rank)
            {
                lastRank = packet->rank;
                promoted = 1;
            }
        }
    }


    PlistNoteUpdate (packet->pnum);
}

void
handleStats (struct stats_spacket *packet)
{
    register struct player *pl;

#ifdef CORRUPTED_PACKETS
    if (packet->pnum >= MAXPLAYER)
    {
        LineToConsole ("handleStats: bad index %d\n", packet->pnum);
        return;
    }
#endif

    pl = &players[packet->pnum];

    pl->p_stats.st_tkills = ntohl (packet->tkills);
    pl->p_stats.st_tlosses = ntohl (packet->tlosses);
    pl->p_stats.st_kills = ntohl (packet->kills);
    pl->p_stats.st_losses = ntohl (packet->losses);
    pl->p_stats.st_tticks = ntohl (packet->tticks);
    pl->p_stats.st_tplanets = ntohl (packet->tplanets);
    pl->p_stats.st_tarmsbomb = ntohl (packet->tarmies);
    pl->p_stats.st_sbkills = ntohl (packet->sbkills);
    pl->p_stats.st_sblosses = ntohl (packet->sblosses);
    pl->p_stats.st_armsbomb = ntohl (packet->armies);
    pl->p_stats.st_planets = ntohl (packet->planets);

    if ((pl->p_ship.s_type == STARBASE) && (SBhours))
    {
        pl->p_stats.st_sbticks = ntohl (packet->maxkills);
    }
    else
    {
        pl->p_stats.st_maxkills = ntohl (packet->maxkills) / 100.0;
    }
    pl->p_stats.st_sbmaxkills = ntohl (packet->sbmaxkills) / 100.0;


    /* For some reason, we get updates for freed players.  When this happens,
     * don't bother the update the player list. */
    if (pl->p_status != PFREE)
        PlistNoteUpdate (packet->pnum);
}

void
handlePlyrInfo (struct plyr_info_spacket *packet)
{
    register struct player *pl;
    static int lastship = -1;

#ifdef CORRUPTED_PACKETS
    if (packet->pnum >= MAXPLAYER)
    {
        LineToConsole ("handlePlyrInfo: bad index %d\n", packet->pnum);
        return;
    }
    if (packet->team > ALLTEAM)
    {
        LineToConsole ("handlePlyrInfo: bad team %d\n", packet->team);
        return;
    }
#endif

    pl = &players[packet->pnum];

    PlistNoteUpdate (packet->pnum);
    if ((pl->p_team != packet->team) || /*  Check 0 system default */
        ((pl->p_team == 0) && (pl->p_mapchars[0] != teamlet[0])))
    {
        pl->p_team = packet->team;
        pl->p_mapchars[0] = teamlet[pl->p_team];
        PlistNoteArrive (packet->pnum);

        if (pl == me)
            redrawall = 1;      /* Update the map if I
                                 * change teams */
    }

    getship (&pl->p_ship, packet->shiptype);
    pl->p_mapchars[1] = shipnos[pl->p_no];


    if (me == pl && lastship != me->p_ship.s_type)
    {
        redrawTstats ();
        calibrate_stats ();
        redrawStats ();         /* TSH */
    }
    redrawPlayer[packet->pnum] = 1;
}

void
sendUpdatePacket (LONG speed)
{
    struct updates_cpacket packet;

    packet.type = CP_UPDATES;
    timerDelay = speed;
    packet.usecs = htonl (speed);
    sendServerPacket ((struct player_spacket *) &packet);
}

void
handlePlanetLoc (struct planet_loc_spacket *packet)
{
    struct planet *pl;

#ifdef CORRUPTED_PACKETS
    if (packet->pnum >= MAXPLANETS)
    {
        LineToConsole ("handlePlanetLoc: bad index\n");
        return;
    }
#endif

    pl = &planets[packet->pnum];
    pl_update[packet->pnum].plu_x = pl->pl_x;
    pl_update[packet->pnum].plu_y = pl->pl_y;

    if (pl_update[packet->pnum].plu_update != -1)
    {
        pl_update[packet->pnum].plu_update = 1;
        /* LineToConsole ("update: %s, old (%d,%d) new (%d,%d)\n", pl->pl_name,
         * pl->pl_x, pl->pl_y, ntohl(packet->x),ntohl(packet->y)); */
    }
    else
        pl_update[packet->pnum].plu_update = 0;

    pl->pl_x = ntohl (packet->x);
    pl->pl_y = ntohl (packet->y);
    strcpy (pl->pl_name, packet->name);
    pl->pl_namelen = strlen (packet->name);
    pl->pl_flags |= (PLREDRAW | PLCLEAR);
    reinitPlanets = 1;

#ifdef ROTATERACE
    if (rotate)
    {
        rotate_coord (&pl->pl_x, &pl->pl_y, rotate_deg, GWIDTH / 2,
                      GWIDTH / 2);
    }
#endif
}


void
handleReserved (struct reserved_spacket *packet,
                int sock)
{
    struct reserved_cpacket response;

#ifdef CORRUPTED_PACKETS
    if (sock == udpSock)
    {
        LineToConsole ("garbage Reserved packet discarded.\n");
        return;
    }
#endif

#if !defined(BORG)

#ifndef RSA
    encryptReservedPacket (packet, &response, me->p_no);
    sendServerPacket ((struct player_spacket *) &response);
#else

    if (useRsa)
    {                           /* can use -o option for old
                                 * blessing */
        /* client sends back a 'reserved' packet just saying RSA_VERSION info */
        /* theoretically, the server then sends off a rsa_key_spacket * for the
         * client to then respond to */
        warning (RSA_VERSION);
        STRNCPY (response.resp, RSA_VERSION, RESERVED_SIZE);
        MCOPY (packet->data, response.data, RESERVED_SIZE);
        response.type = CP_RESERVED;

#ifdef DEBUG
        LineToConsole ("Sending RSA reserved response\n");
#endif
    }
    else
    {
        /* If server gods don't like NEWMACRO/SMARTMACRO they better install
         * RSA... */
        UseNewMacro = 1;
        UseSmartMacro = 1;
// SRS - Parameter 3 is incorrect?
        encryptReservedPacket (packet, &response, me->p_no);
    }

    sendServerPacket ((struct player_spacket *) &response);
#endif /* RSA */

#endif /* defined(BORG) */
}

void
handleShipCap (struct ship_cap_spacket *packet)
{
    unsigned short stype;

    stype = ntohs (packet->s_type);
    shipvals[stype].s_torpspeed = ntohs (packet->s_torpspeed);
    shipvals[stype].s_maxshield = ntohl (packet->s_maxshield);
    shipvals[stype].s_maxdamage = ntohl (packet->s_maxdamage);
    shipvals[stype].s_maxegntemp = ntohl (packet->s_maxegntemp);
    shipvals[stype].s_maxwpntemp = ntohl (packet->s_maxwpntemp);
    shipvals[stype].s_maxarmies = ntohs (packet->s_maxarmies);
    shipvals[stype].s_maxfuel = ntohl (packet->s_maxfuel);
    shipvals[stype].s_maxspeed = ntohl (packet->s_maxspeed);
    shipvals[stype].s_width = ntohs (packet->s_width);
    shipvals[stype].s_height = ntohs (packet->s_height);
    shipvals[stype].s_phaserdamage = ntohs (packet->s_phaserrange);
    getship (myship, myship->s_type);

    redrawTstats ();
    calibrate_stats ();
    redrawStats ();
}

void
handleGeneric32 (struct generic_32_spacket *packet)
{

    if (sizeof(struct generic_32_spacket) != GENERIC_32_LENGTH)
    {
#ifdef DEBUG
    	LineToConsole("Generic32 packet length of %d, ignoring packet.\n", sizeof(struct generic_32_spacket));
#endif
    	return;
    }
    if (packet->version < 'a') return;
    me->p_repair_time = packet->repair_time;
    me->pl_orbit = packet->pl_orbit;
    if (packet->version < 'b') return;
}

#ifdef RSA
void
handleRSAKey (struct rsa_key_spacket *packet)
{
    struct rsa_key_cpacket response;
    struct sockaddr_in saddr;
    int len;
    unsigned char *data;

#ifdef GATEWAY
    extern unsigned LONG netaddr;
    extern int serv_port;

#endif

    response.type = CP_RSA_KEY;
    /* encryptRSAPacket (packet, &response);      old style rsa-client  */

#ifdef GATEWAY
    /* if we didn't get it from -H, go ahead and query the socket */
    if (netaddr == 0)
    {
        len = sizeof (saddr);
        if (getpeername (sock, (struct sockaddr *) &saddr, &len) < 0)
        {
            perror ("getpeername(sock)");
            terminate (1);
        }
    }
    else
    {
        saddr.sin_addr.s_addr = htonl (netaddr);
        saddr.sin_port = htons (serv_port);
    }
#else
    /* query the socket to determine the remote host (ATM) */
    len = sizeof (saddr);
    if (getpeername (sock, (struct sockaddr *) &saddr, &len) < 0)
    {
        perror ("getpeername(sock)");
        terminate (1);
    }
#endif

    /* replace the first few bytes of the message */
    /* will be the low order bytes of the number */
    data = packet->data;
    MCOPY (&saddr.sin_addr.s_addr, data, sizeof (saddr.sin_addr.s_addr));
    data += sizeof (saddr.sin_addr.s_addr);
    MCOPY (&saddr.sin_port, data, sizeof (saddr.sin_port));

    rsa_black_box (response.resp, packet->data, response.public,
                   response.global);

    sendServerPacket ((struct player_spacket *) &response);
    /* #ifdef DEBUG */
    LineToConsole ("RSA verification requested.\n");
    /* #endif */
}

#endif

#ifdef INCLUDE_SCAN
void
handleScan (packet)
     struct scan_spacket *packet;
{
    struct player *pp;

#ifdef CORRUPTED_PACKETS
    if (packet->pnum >= MAXPLAYER)
    {
        LineToConsole ("handleScan: bad index\n");
        return;
    }
#endif

    if (packet->success)
    {
        pp = &players[packet->pnum];
        pp->p_fuel = ntohl (packet->p_fuel);
        pp->p_armies = ntohl (packet->p_armies);
        pp->p_shield = ntohl (packet->p_shield);
        pp->p_damage = ntohl (packet->p_damage);
        pp->p_etemp = ntohl (packet->p_etemp);
        pp->p_wtemp = ntohl (packet->p_wtemp);
        informScan (packet->pnum);
    }
}

informScan (p)
     int p;
{
}

#endif /* INCLUDE_SCAN */

/* UDP stuff */
void
sendUdpReq (int req)
{
    struct udp_req_cpacket packet;

    packet.type = CP_UDP_REQ;
    packet.request = (char) req;

    if (req >= COMM_MODE)
    {
        packet.request = COMM_MODE;
        packet.connmode = (char) (req - COMM_MODE);
        sendServerPacket ((struct player_spacket *) &packet);
        return;
    }
    if (req == COMM_UPDATE)
    {

#ifdef SHORT_PACKETS
        if (recv_short)
        {                       /* not necessary */
            /* Let the client do the work, and not the network :-) */

            resetWeaponInfo ();
        }
#endif

        sendServerPacket ((struct player_spacket *) &packet);
        warning ("Sent request for full update");
        return;
    }
    if (req == commModeReq)
    {
        warning ("Request is in progress, do not disturb");
        return;
    }
    if (req == COMM_UDP)
    {
        /* open UDP port */
        if (openUdpConn () >= 0)
        {
            UDPDIAG (("Bound to local port %d on fd %d\n", udpLocalPort,
                      udpSock));
        }
        else
        {
            UDPDIAG (("Bind to local port %d failed\n", udpLocalPort));
            commModeReq = COMM_TCP;
            commStatus = STAT_CONNECTED;
            commSwitchTimeout = 0;
            if (udpWin)
                udprefresh (UDP_STATUS);
            warning ("Unable to establish UDP connection");

            return;
        }
    }
    /* send the request */
    packet.type = CP_UDP_REQ;
    packet.request = (char) req;
    packet.port = htonl (udpLocalPort);

#ifdef GATEWAY
    if (!strcmp (serverName, gw_mach))
    {
        packet.port = htons (gw_serv_port);     /* gw port that server
                                                 * should call */
        UDPDIAG (("+ Telling server to contact us on %d\n", gw_serv_port));
    }
#endif

#ifdef UDP_PORTSWAP
    if (portSwap)
        packet.connmode = CONNMODE_PORT;        /* have him send his port */
#else
    packet.connmode = CONNMODE_PACKET;  /* we get addr from packet */
#endif

    sendServerPacket ((struct player_spacket *) &packet);

    /* update internal state stuff */
    commModeReq = req;
    if (req == COMM_TCP)
        commStatus = STAT_SWITCH_TCP;
    else
        commStatus = STAT_SWITCH_UDP;
    commSwitchTimeout = 25;     /* wait 25 updates (about
                                 * five seconds) */

    UDPDIAG (("Sent request for %s mode\n", (req == COMM_TCP) ?
              "TCP" : "UDP"));

#ifdef UDP_PORTSWAP
    if (!portSwap)
#endif
        if ((req == COMM_UDP) && recvUdpConn () < 0)
        {
            UDPDIAG (("Sending TCP reset message\n"));
            packet.request = COMM_TCP;
            packet.port = 0;
            commModeReq = COMM_TCP;
            sendServerPacket ((struct player_spacket *) &packet);
            /* we will likely get a SWITCH_UDP_OK later; better ignore it */
            commModeReq = COMM_TCP;
            commStatus = STAT_CONNECTED;
            commSwitchTimeout = 0;
        }


    if (udpWin)
        udprefresh (UDP_STATUS);
}

void
handleUdpReply (struct udp_reply_spacket *packet)
{
    struct udp_req_cpacket response;

    UDPDIAG (("Received UDP reply %d\n", packet->reply));
    commSwitchTimeout = 0;

    response.type = CP_UDP_REQ;

    switch (packet->reply)
    {
    case SWITCH_TCP_OK:
        if (commMode == COMM_TCP)
        {
            UDPDIAG (("Got SWITCH_TCP_OK while in TCP mode; ignoring\n"));
        }
        else
        {
            commMode = COMM_TCP;
            commStatus = STAT_CONNECTED;
            warning ("Switched to TCP-only connection");
            closeUdpConn ();
            UDPDIAG (("UDP port closed\n"));
            if (udpWin)
            {
                udprefresh (UDP_STATUS);
                udprefresh (UDP_CURRENT);
            }
        }
        break;
    case SWITCH_UDP_OK:
        if (commMode == COMM_UDP)
        {
            UDPDIAG (("Got SWITCH_UDP_OK while in UDP mode; ignoring\n"));
        }
        else
        {
            /* the server is forcing UDP down our throat? */
            if (commModeReq != COMM_UDP)
            {
                UDPDIAG (("Got unsolicited SWITCH_UDP_OK; ignoring\n"));
            }
            else
            {

#ifdef UDP_PORTSWAP
                if (portSwap)
                {
                    udpServerPort = ntohl (packet->port);
                    if (connUdpConn () < 0)
                    {
                        UDPDIAG (("Unable to connect, resetting\n"));
                        warning ("Connection attempt failed");
                        commModeReq = COMM_TCP;
                        commStatus = STAT_CONNECTED;
                        if (udpSock >= 0)
                            closeUdpConn ();
                        if (udpWin)
                        {
                            udprefresh (UDP_STATUS);
                            udprefresh (UDP_CURRENT);
                        }
                        response.request = COMM_TCP;
                        response.port = 0;
                        goto send;
                    }
                }
#else
                /* this came down UDP, so we MUST be connected */
                /* (do the verify thing anyway just for kicks) */
#endif

                UDPDIAG (("Connected to server's UDP port\n"));
                commStatus = STAT_VERIFY_UDP;
                if (udpWin)
                    udprefresh (UDP_STATUS);
                response.request = COMM_VERIFY; /* send verify request on
                                                 * UDP */
                response.port = 0;
                commSwitchTimeout = 25; /* wait 25 updates */
              send:
                sendServerPacket ((struct player_spacket *) &response);
            }
        }
        break;
    case SWITCH_DENIED:
        if (ntohs ((u_short) (packet->port)))
        {
            UDPDIAG (("Switch to UDP failed (different version)\n"));
            warning ("UDP protocol request failed (bad version)");
        }
        else
        {
            UDPDIAG (("Switch to UDP denied\n"));
            warning ("UDP protocol request denied");
        }
        commModeReq = commMode;
        commStatus = STAT_CONNECTED;
        commSwitchTimeout = 0;
        if (udpWin)
            udprefresh (UDP_STATUS);
        if (udpSock >= 0)
            closeUdpConn ();
        break;
    case SWITCH_VERIFY:
        UDPDIAG (("Received UDP verification\n"));
        break;
    default:
        LineToConsole ("netrek: Got funny reply (%d) in UDP_REPLY packet\n",
                         packet->reply);
        break;
    }
}

#define MAX_PORT_RETRY  10
int
openUdpConn (void)
{
    struct sockaddr_in addr;
    struct hostent *hp;
    int attempts;

    if (udpSock >= 0)
    {
        LineToConsole ("netrek: tried to open udpSock twice\n");
        return (0);             /* pretend we succeeded
                                 * (this could be bad) */
    }
    if ((udpSock = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror ("netrek: unable to create DGRAM socket");
        return (-1);
    }

    if (udpSock >= max_fd)
        max_fd = udpSock + 1;

    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;

    errno = 0;
    udpLocalPort = (getpid () & 32767) + (RANDOM () % 256);

    /* if baseUdpLocalPort is defined, we want to start from that */
    if (baseUdpLocalPort)
    {
        udpLocalPort = baseUdpLocalPort;
        UDPDIAG (("using base port %d\n", baseUdpLocalPort));
    }

    for (attempts = 0; attempts < MAX_PORT_RETRY; attempts++)
    {
        while (udpLocalPort < 2048)
        {
            udpLocalPort = (udpLocalPort + 10687) & 32767;
        }

#ifdef GATEWAY
        /* we need the gateway to know where to find us */
        if (!strcmp (serverName, gw_mach))
        {
            UDPDIAG (("+ gateway test: binding to %d\n", gw_local_port));
            udpLocalPort = gw_local_port;
        }
#endif

        addr.sin_port = htons ((u_short) udpLocalPort);
        if (bind (udpSock, (struct sockaddr *) &addr, sizeof (addr)) >= 0)
            break;

        /* bind() failed, so find another port.  If we're tunneling through a
         * router-based firewall, we just increment; otherwise we try to mix it
         * up a little.  The check for ports < 2048 is done above. */
        if (baseUdpLocalPort)
            udpLocalPort++;
        else
            udpLocalPort = (udpLocalPort + 10687) & 32767;
    }
    if (attempts == MAX_PORT_RETRY)
    {
        perror ("netrek: bind");
        UDPDIAG (("Unable to find a local port to bind to\n"));
        closesocket (udpSock);
        udpSock = -1;
        return (-1);
    }
    UDPDIAG (("Local port is %d\n", udpLocalPort));

    /* determine the address of the server */
    if (!serveraddr)
    {
        if ((addr.sin_addr.s_addr = inet_addr (serverName)) == -1)
        {
            if ((hp = gethostbyname (serverName)) == NULL)
            {
                LineToConsole ("Who is %s?\n", serverName);
                terminate (0);
            }
            else
            {
                addr.sin_addr.s_addr = *(LONG *) hp->h_addr;
            }
        }
        serveraddr = addr.sin_addr.s_addr;
        UDPDIAG (("Found serveraddr == 0x%x\n", serveraddr));
    }
    return (0);
}

#ifdef UDP_PORTSWAP
int
connUdpConn ()
{
    struct sockaddr_in addr;

    addr.sin_addr.s_addr = serveraddr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons ((u_short) udpServerPort);

    UDPDIAG (("Connecting to host 0x%x on port %d\n", serveraddr,
              udpServerPort));
    if (connect (udpSock, (const struct sockaddr *) &addr, sizeof (addr)) < 0)
    {
        LineToConsole ("Error %d: ", WSAGetLastError ());
        perror ("netrek: unable to connect UDP socket");
        printUdpInfo ();        /* debug */
        return (-1);
    }

    return (0);
}

#endif

int
recvUdpConn (void)
{
    fd_set readfds;
    struct timeval to;
    struct sockaddr_in from;
    int fromlen, res;

    MZERO (&from, sizeof (from));       /* don't get garbage if
                                         * really broken */

    /* we patiently wait until the server sends a packet to us */
    /* (note that we silently eat the first one) */
    UDPDIAG (("Issuing recvfrom() call\n"));
    printUdpInfo ();
    fromlen = sizeof (from);

	//  tryagain:
    FD_ZERO (&readfds);
    FD_SET (udpSock, &readfds);
    to.tv_sec = 6;              /* wait 3 seconds, then
                                 * abort */
    to.tv_usec = 0;
    if ((res = select (max_fd, &readfds, 0, 0, &to)) <= 0)
    {
        if (!res)
        {
            UDPDIAG (("timed out waiting for response\n"));
            warning ("UDP connection request timed out");
            return (-1);
        }
        else
        {
            perror ("select() before recvfrom()");
            return (-1);
        }
    }
    if (recvfrom
        (udpSock, buf, BUFSIZE, 0, (struct sockaddr *) &from, &fromlen) < 0)
    {
        perror ("recvfrom");
        UDPDIAG (("recvfrom failed, aborting UDP attempt\n"));
        return (-1);
    }
    if (from.sin_addr.s_addr != (unsigned long) serveraddr)
    {
        /* safe? */
        serveraddr = from.sin_addr.s_addr;
        UDPDIAG (("Warning: from 0x%x, but server is 0x%x\n",
                  from.sin_addr.s_addr, serveraddr));
    }
    if (from.sin_family != AF_INET)
    {
        UDPDIAG (("Warning: not AF_INET (%d)\n", from.sin_family));
    }
    udpServerPort = ntohs (from.sin_port);
    UDPDIAG (("recvfrom() succeeded; will use server port %d\n",
              udpServerPort));

#ifdef GATEWAY
    if (!strcmp (serverName, gw_mach))
    {
        UDPDIAG (("+ actually, I'm going to use %d\n", gw_port));
        udpServerPort = gw_port;
        from.sin_port = htons (udpServerPort);
    }
#endif

    if (connect (udpSock, (struct sockaddr *) &from, sizeof (from)) < 0)
    {
        perror ("netrek: unable to connect UDP socket after recvfrom()");
        closesocket (udpSock);
        udpSock = -1;
        return (-1);
    }
    return (0);
}

int
closeUdpConn (void)
{
    V_UDPDIAG (("Closing UDP socket\n"));
    if (udpSock < 0)
    {
        LineToConsole ("netrek: tried to close a closed UDP socket\n");
        return (-1);
    }
    shutdown (udpSock, 2);
    closesocket (udpSock);
    udpSock = -1;
    return 0;
}

void
printUdpInfo (void)
{
    struct sockaddr_in addr;
    int len;

    len = sizeof (addr);
    if (getsockname (udpSock, (struct sockaddr *) &addr, &len) < 0)
    {
        /* perror("printUdpInfo: getsockname"); */
        return;
    }
    UDPDIAG (("LOCAL: addr=0x%x, family=%d, port=%d\n", addr.sin_addr.s_addr,
              addr.sin_family, ntohs (addr.sin_port)));

    if (getpeername (udpSock, (struct sockaddr *) &addr, &len) < 0)
    {
        /* perror("printUdpInfo: getpeername"); */
        return;
    }
    UDPDIAG (("PEER : addr=0x%x, family=%d, port=%d\n", addr.sin_addr.s_addr,
              addr.sin_family, ntohs (addr.sin_port)));
}

void
handleSequence (struct sequence_spacket *packet)
{
    static int recent_count = 0, recent_dropped = 0;
    LONG newseq;

    drop_flag = 0;
    if (chan != udpSock)
        return;                 /* don't pay attention to
                                 * TCP sequence #s */
    udpTotal++;
    recent_count++;

    /* update percent display every 256 updates (~50 seconds usually) */
    if (!(udpTotal & 0xff))
        if (udpWin)
            udprefresh (UDP_DROPPED);

    newseq = (LONG) ntohs (packet->sequence);
    /* LineToConsole ("read %d - ", newseq); */

    if (((unsigned short) sequence) > 65000 &&
        ((unsigned short) newseq) < 1000)
    {
        /* we rolled, set newseq = 65536+sequence and accept it */
        sequence = ((sequence + 65536) & 0xffff0000) | newseq;
    }
    else
    {
        /* adjust newseq and do compare */
        newseq |= (sequence & 0xffff0000);

        if (!udpSequenceCheck)
        {                       /* put this here so that
                                 * turning seq check */
            sequence = newseq;  /* on and off doesn't make
                                 * us think we lost */
            return;             /* a whole bunch of packets. */
        }
        if (newseq > sequence)
        {
            /* accept */
            if (newseq != sequence + 1)
            {
                udpDropped += (newseq - sequence) - 1;
                udpTotal += (newseq - sequence) - 1;    /* want TOTAL packets */
                recent_dropped += (newseq - sequence) - 1;
                recent_count += (newseq - sequence) - 1;
                if (udpWin)
                    udprefresh (UDP_DROPPED);
                UDPDIAG (("sequence=%d, newseq=%d, we lost some\n",
                          sequence, newseq));
            }
            sequence = newseq;
            /* S_P2 */
            if (shortversion == SHORTVERSION && recv_short)
            {
                if (F_self_19flags)
                {
                    me->p_flags &= ~(PFSHIELD|PFREPAIR|PFBOMB|PFCLOAK|
                                    PFWEP|PFENG|PFREFITTING|PFDOCKOK);
                    me->p_flags |= ((packet->flag8&0x01?PFSHIELD:0)|
                                    (packet->flag8&0x02?PFREPAIR:0)|
                                    (packet->flag8&0x04?PFBOMB:0)|
                                    (packet->flag8&0x08?PFCLOAK:0)|
                                    (packet->flag8&0x10?PFWEP:0)|
                                    (packet->flag8&0x20?PFENG:0)|
                                    (packet->flag8&0x40?PFREFITTING:0)|
                                    (packet->flag8&0x80?PFDOCKOK:0));
                }
                else
                {
                    me->p_flags =
                        (me->p_flags & 0xffff00ff) | (unsigned int) packet->
                        flag8 << 8;
                }
            }
        }
        else
        {
            /* reject */
            if (packet->type == SP_SC_SEQUENCE)
            {
                V_UDPDIAG (("(ignoring repeat %d)\n", newseq));
            }
            else
            {
                UDPDIAG (("sequence=%d, newseq=%d, ignoring transmission\n",
                          sequence, newseq));
            }
            /* the remaining packets will be dropped and we shouldn't count the
             * SP_SEQUENCE packet either */
            packets_received--;
            drop_flag = 1;
        }
    }
    /* LineToConsole ("newseq %d, sequence %d\n", newseq, sequence); */
    if (recent_count > UDP_RECENT_INTR)
    {
        /* once a minute (at 5 upd/sec), report on how many were dropped */
        /* during the last UDP_RECENT_INTR updates                       */
        udpRecentDropped = recent_dropped;
        recent_count = recent_dropped = 0;
        if (udpWin)
            udprefresh (UDP_DROPPED);
    }
}


#ifdef PACKET_LOG
static int Max_CPS = 0;
static int Max_CPSout = 0;
static time_t Start_Time = 0;
static double s2 = 0;
static int sumpl = 0;
static int numpl = 0;
static int outdata_this_sec = 0;
static double sout2 = 0;
static int sumout = 0;

/* HW clumsy but who cares ... :-) */
static int vari_sizes[NUM_PACKETS];
static int cp_msg_size;         /* For CP_S_MESSAGE */

void
Log_Packet (char type,
            int act_size)
/* HW */
{
    static time_t lasttime;
    static int data_this_sec;
    time_t this_sec;

    if (log_packets == 0)
        return;

    if (type <= 0 && type > NUM_PACKETS)
    {
        LineToConsole ("Attempted to log a bad packet? \n");
        return;
    }
    packet_log[type]++;
    /* data_this_sec += handlers[type].size; */
    data_this_sec += act_size;  /* HW */
    ALL_BYTES += act_size;      /* To get all bytes */
    if (handlers[type].size == -1)
    {                           /* vari packet */
        vari_sizes[type] += act_size;
    }
    this_sec = time (NULL);
    if (this_sec != lasttime)
    {
        lasttime = this_sec;
        if (log_packets > 1)
        {
            LineToConsole ("%d %d %d\n", this_sec - Start_Time,
                          data_this_sec, outdata_this_sec);
        }
        if (Start_Time == 0)
        {
            Start_Time = this_sec;
        }
        /* ignore baudage on the first few seconds of reception -- * that's
         * when we get crushed by the motd being sent */
        if (lasttime > Start_Time + 10)
        {
            if (data_this_sec > Max_CPS)
                Max_CPS = data_this_sec;
            if (outdata_this_sec > Max_CPSout)
                Max_CPSout = outdata_this_sec;
            sumpl += data_this_sec;
            s2 += (data_this_sec * data_this_sec);
            sout2 += outdata_this_sec * outdata_this_sec;
            sumout += outdata_this_sec;
            numpl++;
        }
        data_this_sec = 0;
        outdata_this_sec = 0;
    }
}

void
Log_OPacket (int tpe,
             int size)
{
    /* Log Packet will handle the per second resets of this */
    if (log_packets == 0)
        return;
    outpacket_log[tpe]++;
    outdata_this_sec += size;

#ifdef SHORT_PACKETS
    if (tpe == CP_S_MESSAGE)
        cp_msg_size += size;    /* HW */
#endif
}

/* print out out the cool information on packet logging */
void
Dump_Packet_Log_Info (void)
{
    int i;
    time_t Now;
    int total_bytes = 0;
    int outtotal_bytes = 0;
    int calc_temp;

    Now = time (NULL);

    LineToConsole ("Packet Logging Summary:\n");
    LineToConsole ("Start time: %s ", ctime (&Start_Time));
    LineToConsole (" End time: %s Elapsed play time: %3.2f min\n",
                    ctime (&Now), (float) ((Now - Start_Time) / 60));
    LineToConsole ("Maximum CPS in during normal play: %d bytes per sec\n", Max_CPS);
    LineToConsole ("Standard deviation in: %d\n",
                    (int) sqrt ((numpl * s2 - sumpl * sumpl) /
                    (numpl * (numpl - 1))));
    LineToConsole ("Maximum CPS out during normal play: %d bytes per sec\n",
                    Max_CPSout);
    LineToConsole ("Standard deviation out: %d\n",
                    (int) sqrt ((numpl * sout2 - sumout * sumout) /
                    (numpl * (numpl - 1))));

#ifdef SHORT_PACKETS
    /* total_bytes = ALL_BYTES; *//* Hope this works  HW */
    for (i = 0; i <= NUM_PACKETS; i++)
    {                           /* I think it must be <= */
        if (handlers[i].size != -1)
            total_bytes += handlers[i].size * packet_log[i];
        else
            total_bytes += vari_sizes[i];
    }                           /* The result should be ==
                                 * ALL_BYTES HW */
#else
    for (i = 0; i <= NUM_PACKETS; i++)
    {
        total_bytes += handlers[i].size * packet_log[i];
    }
#endif

    for (i = 0; i <= NUM_SIZES; i++)
    {

#ifdef SHORT_PACKETS
        if (handlers[i].size != -1)
            outtotal_bytes += outpacket_log[i] * sizes[i];
        else
            outtotal_bytes += cp_msg_size;      /* HW */
#else
        outtotal_bytes += outpacket_log[i] * sizes[i];
#endif
    }

    LineToConsole ("Total bytes received %d, average CPS: %4.1f\n",
                    total_bytes, (float) (total_bytes / (Now - Start_Time)));
    LineToConsole ("Server Packets Summary:\n");
    LineToConsole ("Num #Rcvd    Size   TotlBytes   %%Total\n");
    for (i = 0; i <= NUM_PACKETS; i++)
    {

#ifdef SHORT_PACKETS
        if (handlers[i].size != -1)
            calc_temp = handlers[i].size * packet_log[i];
        else
            calc_temp = vari_sizes[i];

        LineToConsole ("%3d %5d    %4d   %9d   %3.2f\n",
                        i, packet_log[i], handlers[i].size, calc_temp,
                        (float) (calc_temp * 100 / total_bytes));
#else
        calc_temp = handlers[i].size * packet_log[i];
        LineToConsole ("%3d %5d    %4d   %9d   %3.2f\n",
                        i, packet_log[i], handlers[i].size, calc_temp,
                        (float) (calc_temp * 100 / total_bytes));
#endif
    }
    LineToConsole ("Total bytes sent %d, average CPS: %4.1f\n",
                    outtotal_bytes, (float) (outtotal_bytes / (Now - Start_Time)));
    LineToConsole ("Client Packets Summary:\n");
    LineToConsole ("Num #Sent    Size   TotlBytes   %%Total\n");
    for (i = 0; i <= NUM_SIZES; i++)
    {

#ifdef SHORT_PACKETS
        if (sizes[i] == -1)
            calc_temp = cp_msg_size;
        else
            calc_temp = sizes[i] * outpacket_log[i];
        LineToConsole ("%3d %5d    %4d   %9d   %3.2f\n",
                        i, outpacket_log[i], sizes[i], calc_temp,
                        (float) (calc_temp * 100 / outtotal_bytes));
    }
#else
        calc_temp = sizes[i] * outpacket_log[i];
        LineToConsole ("%3d %5d    %4d   %9d   %3.2f\n",
                        i, outpacket_log[i], sizes[i], calc_temp,
                        (float) (calc_temp * 100 / outtotal_bytes));
    }
#endif
}

void print_packet(char *packet, int size)
{
   int i;                        /* lcv */
   unsigned int j;
   unsigned char *data;
   int kills, pnum, nplanets;
   struct planet_s_spacket *plpacket;
   if(log_packets == 0) return;
   switch ( packet[0] )
     {
       case SP_MESSAGE:
         LineToConsole("\nS->C SP_MESSAGE\t");
	 if (log_packets > 1)
	   LineToConsole("  m_flags=0x%0X, m_recpt=%d, m_from=%d, mesg=\"%s\",", 
		   ((struct mesg_spacket *) packet)->m_flags, 
		   ((struct mesg_spacket *) packet)->m_recpt, 
		   ((struct mesg_spacket *) packet)->m_from, 
		   ((struct mesg_spacket *) packet)->mesg );
	 break;
       case SP_PLAYER_INFO  :                   /* general player info not */ 
						/* * elsewhere */
	 LineToConsole("\nS->C SP_PLAYER_INFO\t");
	 if (log_packets > 1)
	   LineToConsole("  pnum=%d, shiptype=%d, team=%d,",
		   ((struct plyr_info_spacket *) packet)->pnum,
		   ((struct plyr_info_spacket *) packet)->shiptype,
		   ((struct plyr_info_spacket *) packet)->team );
         break;
       case SP_KILLS        :                   /* # kills a player has */
	 LineToConsole("\nS->C SP_KILLS\t");
	 if (log_packets > 1)
	   LineToConsole("  pnum=%d, kills=%u,",
		   ((struct kills_spacket *) packet)->pnum,
		   ntohl(((struct kills_spacket *) packet)->kills) );
	 break;
       case SP_PLAYER       :                   /* x,y for player */
	 LineToConsole("\nS->C SP_PLAYER\t");
	 if (log_packets > 1)
	   LineToConsole("  pnum=%d, dir=%u, speed=%d,x=%ld, y=%d,",
		   ((struct player_spacket *) packet)->pnum,
		   ((struct player_spacket *) packet)->dir,
		   ((struct player_spacket *) packet)->speed,
		   ntohl(((struct player_spacket *) packet)->x),
		   ntohl(((struct player_spacket *) packet)->y) );
	 break;
       case SP_TORP_INFO    :                   /* torp status */
	 LineToConsole("\nS->C SP_TORP_INFO\t");
	 if (log_packets > 1)
	   LineToConsole("  war=%d, status=%d, tnum=%u,",
		   ((struct torp_info_spacket *) packet)->war,
		   ((struct torp_info_spacket *) packet)->status,
		   ntohs(((struct torp_info_spacket *) packet)->tnum) );
	 break;
       case SP_TORP         :                   /* torp location */
	 LineToConsole("\nS->C SP_TORP\t");
	 if (log_packets > 1)
	   LineToConsole("  dir=%d, tnum=%u, x=%u, y=%u,",
		   ((struct torp_spacket *) packet)->dir,
		   ntohs(((struct torp_spacket *) packet)->tnum),
		   ntohl(((struct torp_spacket *) packet)->x),
		   ntohl(((struct torp_spacket *) packet)->y) );
	 break;
       case SP_PHASER       :                   /* phaser status and * *
						 * direction */
	 LineToConsole("\nS->C SP_PHASER\t");
	 if (log_packets > 1)
	   LineToConsole("  pnum=%d, status=%d, dir=%u, x=%ld, y=%ld, target=%ld,",
		   ((struct phaser_spacket *) packet)->pnum,
		   ((struct phaser_spacket *) packet)->status,
		   ((struct phaser_spacket *) packet)->dir,
		   ntohl(((struct phaser_spacket *) packet)->x),
		   ntohl(((struct phaser_spacket *) packet)->y),
		   ntohl(((struct phaser_spacket *) packet)->target) );
	 break;
       case SP_PLASMA_INFO  :                  /* player login information */
	 LineToConsole("\nS->C SP_PLASMA_INFO\t");
	 if (log_packets > 1)
	   LineToConsole("  war=%d, status=%d  pnum=%u,",
		   ((struct plasma_info_spacket *) packet)->war,
		   ((struct plasma_info_spacket *) packet)->status,
		   ntohs(((struct plasma_info_spacket *) packet)->pnum) );
	 break;
       case SP_PLASMA       :                  /* like SP_TORP */
	 LineToConsole("\nS->C SP_PLASMA\t");
	 if (log_packets > 1)
	   LineToConsole("  pnum=%u, x=%ld, y=%ld,",
		   ntohs(((struct plasma_spacket *) packet)->pnum),
		   ntohl(((struct plasma_spacket *) packet)->x),
		   ntohl(((struct plasma_spacket *) packet)->y) );
	 break;
       case SP_WARNING      :                  /* like SP_MESG */
	 fprintf(stderr,"\nS->C SP_WARNING\t");
	 if (log_packets > 1)
	   LineToConsole("  mesg=\"%s\",",
		   ((struct warning_spacket *) packet)->mesg);
	 break;
       case SP_MOTD         :                  /* line from .motd screen */
	 fprintf(stderr,"\nS->C SP_MOTD\t");
	 if (log_packets > 1)
	   LineToConsole("  line=\"%s\",",
		   ((struct motd_spacket *) packet)->line);
	 break;
       case SP_YOU          :                  /* info on you? */
	 LineToConsole("\nS->C SP_YOU\t");
	 if (log_packets > 1)
	   LineToConsole("  pnum=%d, hostile=%d, swar=%d, armies=%d, flags=0x%0X, damage=%ld, shield=%ld, fuel=%ld, etemp=%u, wtemp=%u, whydead=%u, whodead=%u,",
		   ((struct you_spacket *) packet)->pnum,
		   ((struct you_spacket *) packet)->hostile,
		   ((struct you_spacket *) packet)->swar,
		   ((struct you_spacket *) packet)->armies,
		   ntohl(((struct you_spacket *) packet)->flags),
		   ntohl(((struct you_spacket *) packet)->damage),
		   ntohl(((struct you_spacket *) packet)->shield),
		   ntohl(((struct you_spacket *) packet)->fuel),
		   ntohs(((struct you_spacket *) packet)->etemp),
		   ntohs(((struct you_spacket *) packet)->wtemp),
		   ntohs(((struct you_spacket *) packet)->whydead),
		   ntohs(((struct you_spacket *) packet)->whodead) );
	 break;
       case SP_QUEUE        :                  /* estimated loc in queue? */
	 LineToConsole("\nS->C SP_QUEUE\t");
	 if (log_packets > 1)
	   LineToConsole("  pos=%u,",
		   ntohs(((struct queue_spacket *) packet)->pos) );
	 break;
       case SP_STATUS       :                  /* galaxy status numbers */
	 LineToConsole("\nS->C SP_STATUS\t");
	 if (log_packets > 1)
	   LineToConsole("  tourn=%d, armsbomb=%u, planets=%u, kills=%u, losses=%u, time=%u, timeprod=%lu,",
		   ((struct status_spacket *) packet)->tourn,
		   ntohl(((struct status_spacket *) packet)->armsbomb),
		   ntohl(((struct status_spacket *) packet)->planets),
		   ntohl(((struct status_spacket *) packet)->kills),
		   ntohl(((struct status_spacket *) packet)->losses),
		   ntohl(((struct status_spacket *) packet)->time),
		   ntohl(((struct status_spacket *) packet)->timeprod) );
	 break;
       case SP_PLANET       :                  /* planet armies & * *
						* facilities */
	 LineToConsole("\nS->C SP_PLANET\t");
	 if (log_packets > 1)
	   LineToConsole("  pnum=%d, owner=%d, info=%d, flags=0x%0X, armies=%ld,",
		   ((struct planet_spacket *) packet)->pnum,
		   ((struct planet_spacket *) packet)->owner,
		   ((struct planet_spacket *) packet)->info,
		   ntohs(((struct planet_spacket *) packet)->flags),
		   ntohl(((struct planet_spacket *) packet)->armies) );
	 break;
       case SP_PICKOK       :                  /* your team & ship was * *
						* accepted */
	 LineToConsole("\nS->C SP_PICKOK\t");
	 if (log_packets > 1)
	   LineToConsole("  state=%d,",
		   ((struct pickok_spacket *) packet)-> state );
	 break;
       case SP_LOGIN        :                  /* login response */
	 LineToConsole("\nS->C SP_LOGIN\t");
	 if (log_packets > 1)
	   LineToConsole("  accept=%d, flags=0x%0X, keymap=\"%s\",",
		   ((struct login_spacket *) packet)->accept,
		   ntohl(((struct login_spacket *) packet)->flags),
		   ((struct login_spacket *) packet)->keymap );
	 break;
       case SP_FLAGS        :                  /* give flags for a player */
	 LineToConsole("\nS->C SP_FLAGS\t");
	 if (log_packets > 1)
	   LineToConsole("  pnum=%d, flags=0x%0X,",
		   ((struct flags_spacket *) packet)->pnum,
		   ntohl(((struct flags_spacket *) packet)->flags) );
	 break;
       case SP_MASK         :                  /* tournament mode mask */
	 LineToConsole("\nS->C SP_MASK\t");
	 if (log_packets > 1)
	   LineToConsole("  mask=%d,",
		   ((struct mask_spacket *) packet)->mask );
	 break;
       case SP_PSTATUS      :                  /* give status for a player */
	 LineToConsole("\nS->C SP_PSTATUS\t");
	 if (log_packets > 1)
	   LineToConsole("  pnum=%d, status=%d,",
		   ((struct pstatus_spacket *) packet)->pnum,
		   ((struct pstatus_spacket *) packet)->status );
	 break;
       case SP_BADVERSION   :                  /* invalid version number */
	 LineToConsole("\nS->C SP_BADVERSION\t");
	 if (log_packets > 1)
	   LineToConsole("  why=%d,",
		   ((struct badversion_spacket *) packet)->why );
	 break;
       case SP_HOSTILE      :                  /* hostility settings for a
						* * * player */
	 LineToConsole("\nS->C SP_HOSTILE\t");
	 if (log_packets > 1)
	   LineToConsole("  pnum=%d, war=%d, hostile=%d,",
		   ((struct hostile_spacket *) packet)->pnum,
		   ((struct hostile_spacket *) packet)->war,
		   ((struct hostile_spacket *) packet)->hostile );
	 break;
       case SP_STATS        :                  /* a player's statistics */
	 LineToConsole("\nS->C SP_STATS\t");
	 if (log_packets > 1)
	   LineToConsole("  pnum=%d, tkills=%ld, tlosses=%ld, kills=%ld, losses=%ld, tticks=%ld, tplanets=%ld, tarmies=%ld, sbkills=%ld, sblosses=%ld, armies=%ld, planets=%ld, maxkills=%ld, sbmaxkills=%ld,",
		   ((struct stats_spacket *) packet)->pnum,
		   ntohl(((struct stats_spacket *) packet)->tkills),
		   ntohl(((struct stats_spacket *) packet)->tlosses),
		   ntohl(((struct stats_spacket *) packet)->kills),
		   ntohl(((struct stats_spacket *) packet)->losses),
		   ntohl(((struct stats_spacket *) packet)->tticks),
		   ntohl(((struct stats_spacket *) packet)->tplanets),
		   ntohl(((struct stats_spacket *) packet)->tarmies),
		   ntohl(((struct stats_spacket *) packet)->sbkills),
		   ntohl(((struct stats_spacket *) packet)->sblosses),
		   ntohl(((struct stats_spacket *) packet)->armies),
		   ntohl(((struct stats_spacket *) packet)->planets),
		   ntohl(((struct stats_spacket *) packet)->maxkills),
		   ntohl(((struct stats_spacket *) packet)->sbmaxkills) );
	 break;
       case SP_PL_LOGIN     :                  /* new player logs in */
	 LineToConsole("\nS->C SP_PL_LOGIN\t");
	 if (log_packets > 1)
	   LineToConsole("  pnum=%d, rank=%d, name=\"%s\", monitor=\"%s\", login=\"%s\",",
		   ((struct plyr_login_spacket *) packet)->pnum,
		   ((struct plyr_login_spacket *) packet)->rank,
		   ((struct plyr_login_spacket *) packet)->name,
		   ((struct plyr_login_spacket *) packet)->monitor,
		   ((struct plyr_login_spacket *) packet)->login );
	 break;
       case SP_RESERVED     :                  /* for future use */
	 LineToConsole("\nS->C SP_RESERVED\t");
         if (log_packets > 1)
           {
             LineToConsole("  data=");
             for( i = 0; i < 16; i++)
               LineToConsole("0x%0X ", (unsigned char)((struct reserved_spacket *) packet)->data[i]);
             LineToConsole(",");
           }
	 break;
       case SP_PLANET_LOC   :                  /* planet name, x, y */
	 LineToConsole("\nS->C SP_PLANET_LOC\t");
	 if (log_packets > 1)
	   LineToConsole("  pnum=%d, x=%ld, y=%ld, name=\"%s\",",
		   ((struct planet_loc_spacket *) packet)->pnum,
		   ntohl(((struct planet_loc_spacket *) packet)->x),
		   ntohl(((struct planet_loc_spacket *) packet)->y),
		   ((struct planet_loc_spacket *) packet)->name );
	 break;
#ifdef INCLUDE_SCAN
       /* NOTE: not implemented */
       case SP_SCAN         :                  /* ATM: results of player *
						* * scan */
	 LineToConsole("\nS->C SP_SCAN\t");
	 if(log_packets > 1)
	   LineToConsole("not implemented,");
	 break;
#endif
       case SP_UDP_REPLY    :                  /* notify client of UDP * *
						* status */
	 LineToConsole("\nS->C SP_UDP_REPLY\t");
	 if (log_packets > 1)
	   LineToConsole("  reply=%d, port=%d,",
		   ((struct udp_reply_spacket *) packet)->reply,
		   ntohl(((struct udp_reply_spacket *) packet)->port) );
	 break;
       case SP_SEQUENCE     :                  /* sequence # packet */
	 LineToConsole("\nS->C SP_SEQUENCE\t");
	 if (log_packets > 1)
	   LineToConsole("  flag8=0x%0X, sequence=%u,",
		   ((struct sequence_spacket *) packet)->flag8,
		   ntohs(((struct sequence_spacket *) packet)->sequence) );
	 break;
       case SP_SC_SEQUENCE  :                  /* this trans is * *
						* semi-critical info */
	 LineToConsole("\nS->C SP_SC_SEQUENCE\t");
	 if (log_packets > 1)
	   LineToConsole("  sequence=%u,",
		   ntohs(((struct sc_sequence_spacket *) packet)->sequence) );
	 break;
#ifdef RSA
       case SP_RSA_KEY      :                  /* handles binary * *
						* verification */
	 LineToConsole("\nS->C SP_RSA_KEY\t");
	 if(log_packets > 1)
	   {
	     LineToConsole("  data=");
	     for(i = 0; i < KEY_SIZE; i++)
	       LineToConsole("0x%0X ",((struct rsa_key_spacket *) packet)->data[i]);
	     LineToConsole(",");
	   }
	 break;
#endif
       case SP_SHIP_CAP     :                   /* Handles server ship mods */
	 LineToConsole("\nS->C SP_SHIP_CAP\t");
	 if (log_packets > 1)
	   LineToConsole("  operation=%d, s_type=%u, s_torpspeed=%u, s_phaserrange=%u, s_maxspeed=%d, s_maxfuel=%d, s_maxshield=%d, s_maxdamage=%d, s_maxwpntemp=%d, s_maxegntemp=%d, s_width=%u, s_height=%d, s_maxarmies=%d, s_letter=%d, s_name=\"%s\", s_desig1=%c, s_desig2=%c, s_bitmap=%u,",
		   ((struct ship_cap_spacket *) packet)->operation,
		   ntohs(((struct ship_cap_spacket *) packet)->s_type),
		   ntohs(((struct ship_cap_spacket *) packet)->s_torpspeed),
		   ntohs(((struct ship_cap_spacket *) packet)->s_phaserrange),
		   ((struct ship_cap_spacket *) packet)->s_maxspeed,
		   ((struct ship_cap_spacket *) packet)->s_maxfuel,
		   ((struct ship_cap_spacket *) packet)->s_maxshield,
		   ((struct ship_cap_spacket *) packet)->s_maxdamage,
		   ((struct ship_cap_spacket *) packet)->s_maxwpntemp,
		   ((struct ship_cap_spacket *) packet)->s_maxegntemp,
		   ntohs(((struct ship_cap_spacket *) packet)->s_width),
		   ntohs(((struct ship_cap_spacket *) packet)->s_height),
		   ntohs(((struct ship_cap_spacket *) packet)->s_maxarmies),
		   ((struct ship_cap_spacket *) packet)->s_letter,
		   ((struct ship_cap_spacket *) packet)->s_name,
		   ((struct ship_cap_spacket *) packet)->s_desig1,
		   ((struct ship_cap_spacket *) packet)->s_desig2,
		   ntohs(((struct ship_cap_spacket *) packet)->s_bitmap) );
	 break;
#ifdef SHORT_PACKETS
       case SP_S_REPLY      :                  /* reply to send-short * *
						* request */
	 LineToConsole("\nS->C SP_S_REPLY\t");
	 if (log_packets > 1)
	   fprintf(stderr,"  repl=%d, windside=%u, gwidth=%ld,",
		   ((struct shortreply_spacket *) packet)->repl,
		   ntohs(((struct shortreply_spacket *) packet)->winside),
		   ntohl(((struct shortreply_spacket *) packet)->gwidth) );
	 break;
       case SP_S_MESSAGE    :                  /* var. Message Packet */
	 LineToConsole("\nS->C SP_S_MESSAGE\t");
	 if (log_packets > 1)
	   LineToConsole("  m_flags=0x%0X, m_recpt=%u, m_from=%u, length=%u, mesg=\"%s\",",
	    	   ((struct mesg_s_spacket *) packet)->m_flags,
		   ((struct mesg_s_spacket *) packet)->m_recpt,
		   ((struct mesg_s_spacket *) packet)->m_from,
		   ((struct mesg_s_spacket *) packet)->length,
		   &( ((struct mesg_s_spacket *) packet)->mesg ) );
	 break;
       case SP_S_WARNING    :                  /* Warnings with 4  Bytes */
	 LineToConsole("\nS->C SP_S_WARNING\t");
	 if (log_packets > 1)
	   LineToConsole("  whichmessage=%u, argument=%d, argument2=%d,",
		   ((struct warning_s_spacket *) packet)->whichmessage,
		   ((struct warning_s_spacket *) packet)->argument,
		   ((struct warning_s_spacket *) packet)->argument2 );
	 break;
       case SP_S_YOU        :                  /* hostile,armies,whydead,etc
						* * * .. */
	 LineToConsole("\nS->C SP_S_YOU\t");
	 if (log_packets > 1)
	   LineToConsole("  pnum=%d, hostile=%d, swar=%d, armies=%d, whydead=%d, whodead=%d, flags=0x%0X,",
		   ((struct youshort_spacket *) packet)->pnum,
		   ((struct youshort_spacket *) packet)->hostile,
		   ((struct youshort_spacket *) packet)->swar,
		   ((struct youshort_spacket *) packet)->armies,
		   ((struct youshort_spacket *) packet)->whydead,
		   ((struct youshort_spacket *) packet)->whodead,
		   ntohl(((struct youshort_spacket *) packet)->flags) );
	 break;
       case SP_S_YOU_SS     :                  /* your ship status */
	 LineToConsole("\nS->C SP_S_YOU_SS\t");
	 if (log_packets > 1)
	   LineToConsole("  ddamage=%u, shield=%u, fuel=%u, etemp=%u, wtemp=%u,",
		   ntohs(((struct youss_spacket *) packet)->damage),
		   ntohs(((struct youss_spacket *) packet)->shield),
		   ntohs(((struct youss_spacket *) packet)->fuel),
		   ntohs(((struct youss_spacket *) packet)->etemp),
		   ntohs(((struct youss_spacket *) packet)->wtemp) );
	 break;
       case SP_S_PLAYER     :                  /* variable length player *
						* * packet */
	 LineToConsole("\nS->C SP_S_PLAYER\t");
	 if (log_packets > 1)
	   LineToConsole("  packets=%d, dir=%u, speed=%d, x=%ld, y=%ld,",
		   ((struct player_s_spacket *) packet)->packets,
		   ntohl(((struct player_s_spacket *) packet)->dir),
		   ((struct player_s_spacket *) packet)->speed,
		   ntohl(((struct player_s_spacket *) packet)->x),
		   ntohl(((struct player_s_spacket *) packet)->y) );
	 break;
#endif
#ifdef PING
       case SP_PING         :                  /* ping packet */
	 LineToConsole("\nS->C SP_PING\t");
	 if (log_packets > 1)
	   LineToConsole("  number=%u, lag=%u, tloss_sc=%u, tloss_cs=%u, iloss_sc=%u, iloss_cs=%u,",
		   ((struct ping_spacket *) packet)->number,
		   ((struct ping_spacket *) packet)->lag,
		   ((struct ping_spacket *) packet)->tloss_sc,
		   ((struct ping_spacket *) packet)->tloss_cs,
		   ((struct ping_spacket *) packet)->iloss_sc,
		   ((struct ping_spacket *) packet)->iloss_cs );
	 break;
#endif
#ifdef FEATURE_PACKETS
       case SP_FEATURE      :
	 LineToConsole("\nS->C SP_FEATURE\t");
	 if (log_packets > 1)
	   LineToConsole("  feature_type=%c, arg1=%d, arg2=%d, value=%d, name=\"%s\",",
		   ((struct feature_cpacket *) packet)->feature_type,
		   ((struct feature_cpacket *) packet)->arg1,
		   ((struct feature_cpacket *) packet)->arg2,
		   ntohl(((struct feature_cpacket *) packet)->value),
		   ((struct feature_cpacket *) packet)->name );
	 break;
#endif       
#ifdef SHORT_PACKETS
       case SP_S_TORP       :                  /* variable length torp * *
						* packet */
	 LineToConsole("\nS->C SP_S_TORP\t");
	 if (log_packets > 1)
	   print_sp_s_torp(packet, 1);
	 break;
       case SP_S_TORP_INFO  :                  /* SP_S_TORP with TorpInfo */
	 LineToConsole("\nS->C SP_S_TORP_INFO\t");
	 if (log_packets > 1)         /* struct built by hand in handleVTorp */
	   print_sp_s_torp(packet, 3);
	 break;
       case SP_S_8_TORP     :                  /* optimized SP_S_TORP */
	 LineToConsole("\nS->C SP_S_8_TORP\t");
	 if (log_packets > 1)
	   print_sp_s_torp(packet, 2);
	 break;
       case SP_S_PLANET     :                  /* see SP_PLANET */
	 LineToConsole("\nS->C SP_S_PLANET\t"); 
	 if (log_packets > 1)
	   {
	     plpacket = (struct planet_s_spacket *) &packet[2];
	     nplanets = packet[1];
             LineToConsole("nplanets = %d, ", nplanets);
             for(i = 0; i < nplanets; i++, plpacket++ )
	       LineToConsole(
		       "pnum = %d, pl_owner = %d, info = %d, flags = %d, armies = %d ", 
		       plpacket->pnum, 
		       plpacket->owner, 
		       plpacket->info, 
		       plpacket->armies,
		       ntohs(plpacket->flags) );
	   }
	 fprintf(stderr,"\n");
	 break;
       /* S_P2 */
       case SP_S_SEQUENCE   :                  /* SP_SEQUENCE for * *
						* compressed packets */
	 LineToConsole("\nS->C SP_S_SEQUENCE\t");
	 if (log_packets > 1)
	   LineToConsole("  No struct defined,");
	 break;
       case SP_S_PHASER     :                  /* see struct */
	 LineToConsole("\nS->C SP_S_PHASER\t");
	 if (log_packets > 1)
	   LineToConsole("  status=%d, pnum=%d, target=%d, dir=%d, x=%d, y=%d",
		   ((((struct phaser_s_spacket *) packet)->status) & 0x0f),
		   ((((struct phaser_s_spacket *) packet)->pnum) & 0x3f),
		   ((struct phaser_s_spacket *) packet)->target,
		   ((struct phaser_s_spacket *) packet)->dir,
		   (SCALE * (ntohs(((struct phaser_s_spacket*) packet)->x))),
		   (SCALE * (ntohs(((struct phaser_s_spacket*) packet)->y))) );
	 break;
       case SP_S_KILLS      :                  /* # of kills player have */
	 LineToConsole("\nS->C SP_S_KILLS\t");
	 if (log_packets > 1)
	   {
	     LineToConsole("  pnum=%d, ",
		      (unsigned char) packet[1]);
             data = &packet[2];
             for (j = 0; j < (unsigned) packet[1]; j++) 
	       {
		 kills = (unsigned short) *data++;
                 kills |= (unsigned short) ((*data & 0x03) << 8);
                 pnum = (unsigned char) *data++ >> 2;
                 LineToConsole("pnum = %d, kills = %d ",pnum, kills);
	       }
	   }
	 fprintf(stderr,"\n");
	 break;
       case SP_S_STATS      :                  /* see SP_STATS */
	 LineToConsole("\nS->C SP_S_STATS\t");
	 if (log_packets > 1)
	   LineToConsole("  pnum=%d, tplanets=%d, tkills=%d, tlosses=%d, kills=%d, losses=%d, tticks=%d, tarmies=%d, sbkills=%d, sblosses=%d, armies=%d, planets=%d, maxkills=%d, sbmaxkills=%d,",
		   ((struct stats_spacket *) packet)->pnum,
		   ntohl(((struct stats_spacket *) packet)->tplanets),
		   ntohl(((struct stats_spacket *) packet)->tkills),
		   ntohl(((struct stats_spacket *) packet)->tlosses),
		   ntohl(((struct stats_spacket *) packet)->kills),
		   ntohl(((struct stats_spacket *) packet)->losses),
		   ntohl(((struct stats_spacket *) packet)->tticks),
		   ntohl(((struct stats_spacket *) packet)->tarmies),
		   ntohl(((struct stats_spacket *) packet)->sbkills),
		   ntohl(((struct stats_spacket *) packet)->sblosses),
		   ntohl(((struct stats_spacket *) packet)->armies),
		   ntohl(((struct stats_spacket *) packet)->planets),
		   ntohl(((struct stats_spacket *) packet)->maxkills),
		   ntohl(((struct stats_spacket *) packet)->sbmaxkills) );
	 break;
#endif
     default: 
       LineToConsole("\nS->C UNKNOWN\t");
       if(log_packets > 1)
	 LineToConsole("  type=%d,",packet[0]);
     }
#ifdef nodef /* #ifdef SHORT_PACKETS */
   switch( *((char *) packet) )
     {
       /* variable length packets */
       case VPLAYER_SIZE    :
	 LineToConsole("\nS->C VPLAYER_SIZE\t");
	 if (log_packets > 1)
	   LineToConsole("  No struct defined, same enum value as SP_PLAYER,");
	 break;
       case SHORTVERSION    :                   /* other number blocks, like
						 * * * UDP Version */
	 LineToConsole("\nS->C SHORTVERSION\t");
	 if (log_packets > 1)
	   LineToConsole("  No struct defined, same enum value as SP_MOTD,");
	 break;
       case OLDSHORTVERSION :                   /* S_P2 */
	 LineToConsole("\nS->C OLDSHORTVERSION\t");
	 if (log_packets > 1)
	   LineToConsole("  No struct defined, same enum value as SP_WARNING,");
	 break;
     }
#endif
}
void print_opacket(char *packet, int size)
{
  int i;  /* lcv */
  switch(packet[0])
    {
      /* packets sent from remote client to xtrek server */
    case CP_MESSAGE      :                    /* send a message */
      LineToConsole("\nC->S CP_MESSAGE\t");
      if (log_packets > 1)
	LineToConsole("  group=%d, indiv=%d, mesg=\"%s\",",
		((struct mesg_cpacket *) packet)->group,
		((struct mesg_cpacket *) packet)->indiv,
		((struct mesg_cpacket *) packet)->mesg );
      break;
    case CP_SPEED        :                    /* set speed */
      LineToConsole("\nC->S CP_SPEED\t");
      if (log_packets > 1)
	LineToConsole("  speed=%d,",
		((struct speed_cpacket *) packet)->speed );
      break;
    case CP_DIRECTION    :                    /* change direction */
      LineToConsole("\nC->S CP_DIRECTION\t");
      if (log_packets > 1)
	LineToConsole("  dir=%u,",
		((struct dir_cpacket *) packet)->dir );
      break;
    case CP_PHASER       :                    /* phaser in a direction */
      LineToConsole("\nC->S CP_PHASER\t");
      if (log_packets > 1)
	LineToConsole("  dir=%u,",
		((struct phaser_cpacket *) packet)-> dir );
      break;
    case CP_PLASMA       :                    /* plasma (in a direction) */
      LineToConsole("\nC->S CP_PLAMSA\t");
      if (log_packets > 1)
	LineToConsole("  dir=%u,",
		((struct plasma_cpacket *) packet)->dir );
      break;
    case CP_TORP         :                    /* fire torp in a direction */
      LineToConsole("\nC->S CP_TORP\t");
      if (log_packets > 1)
	LineToConsole("  dir=%u,",
		((struct torp_cpacket *) packet)->dir );
      break;
    case CP_QUIT         :                    /* self destruct */
      LineToConsole("\nC->S CP_QUIT\t");
      if (log_packets > 1)
	LineToConsole("  no args,");
      break;
    case CP_LOGIN        :                    /* log in (name, password) */
      LineToConsole("\nC->S CP_LOGIN\t");
      if (log_packets > 1)
	LineToConsole("  query=%d, name=\"%s\", password=\"%s\", login=\"%s\",",
		((struct login_cpacket *) packet)->query,
		((struct login_cpacket *) packet)->name,
		((struct login_cpacket *) packet)->password,
		((struct login_cpacket *) packet)->login );
      break;
    case CP_OUTFIT       :                    /* outfit to new ship */
      LineToConsole("\nC->S CP_OUTFIT\t");
      if (log_packets > 1)
	LineToConsole("  team=%d, ship=%d,",
		((struct outfit_cpacket *) packet)->team,
		((struct outfit_cpacket *) packet)->ship );
      break;
    case CP_WAR          :                    /* change war status */
      LineToConsole("\nC->S CP_WAR\t");
      if (log_packets > 1)
	LineToConsole("  newmask=0x%0X,",
		((struct war_cpacket *) packet)->newmask );
      break;
    case CP_PRACTR       :                    /* create practice robot? */
      LineToConsole("\nC->S CP_PRACTR\t");
      if (log_packets > 1)
	LineToConsole("  no args,");
      break;
    case CP_SHIELD       :                    /* raise/lower sheilds */
      LineToConsole("\nC->S CP_SHIELD\t");
      if (log_packets > 1)
	LineToConsole("  state=%d,",
		((struct shield_cpacket *) packet)->state );
      break;
    case CP_REPAIR       :                    /* enter repair mode */
      LineToConsole("\nC->S CP_REPAIR\t");
      if (log_packets > 1)
	LineToConsole("  state=%d,",
		((struct repair_cpacket *) packet)-> state );
      break;
    case CP_ORBIT        :                    /* orbit planet/starbase */
      LineToConsole("\nC->S CP_ORBIT\t");
      if (log_packets > 1)
	LineToConsole("  state=%d,",
		((struct orbit_cpacket *) packet)->state );
      break;
    case CP_PLANLOCK     :                    /* lock on planet */
      LineToConsole("\nC->S CP_PLANLOCK\t");
      if (log_packets > 1)
	LineToConsole("  pnum = %d,",
		((struct planlock_cpacket *) packet)->pnum );
      break;
    case CP_PLAYLOCK     :                    /* lock on player */
      LineToConsole("\nC->S CP_PLAYLOCK\t");
      if (log_packets > 1)
	LineToConsole("  pnum=%d,",
		((struct playlock_cpacket *) packet)->pnum );
      break;
    case CP_BOMB         :                    /* bomb a planet */
      LineToConsole("\nC->S CP_BOMB\t");
      if (log_packets > 1)
	LineToConsole("  state=%d,",
		((struct bomb_cpacket *) packet)->state );
      break;
    case CP_BEAM         :                    /* beam armies up/down */
      LineToConsole("\nC->S CP_BEAM\t");
      if (log_packets > 1)
	LineToConsole("  state=%d,",
		((struct beam_cpacket *) packet)->state );
      break;
    case CP_CLOAK        :                    /* cloak on/off */
      LineToConsole("\nC->S CP_CLOAK\t");
      if (log_packets > 1)
	LineToConsole("  state=%d,",
		((struct cloak_cpacket *) packet)->state );
      break;
    case CP_DET_TORPS    :                    /* detonate enemy torps */
      LineToConsole("\nC->S CP_DET_TORPS\t");
      if (log_packets > 1)
	LineToConsole("  no args,");
      break;
    case CP_DET_MYTORP   :                    /* detonate one of my torps */
      LineToConsole("\nC->S CP_DET_MYTORP\t");
      if (log_packets > 1)
	LineToConsole("  tnum=%u,",
		ntohs(((struct det_mytorp_cpacket *) packet)->tnum) );
      break;
    case CP_COPILOT      :                    /* toggle copilot mode */
      LineToConsole("\nC->S CP_COPILOT\t");
      if (log_packets > 1)
	LineToConsole("  state=%d,",
                ((struct copilot_cpacket *) packet)->state );
      break;
    case CP_REFIT        :                    /* refit to different ship * 
					       * 
					       * * type */
      LineToConsole("\nC->S CP_REFIT\t");
      if (log_packets > 1)
	LineToConsole("  ship=%d,",
		((struct refit_cpacket *) packet)->ship );
      break;
    case CP_TRACTOR      :                    /* tractor on/off */
      LineToConsole("\nC->S CP_TRACTOR\t");
      if (log_packets > 1)
	LineToConsole("  state=%d, pnum=%d,",
		((struct tractor_cpacket *) packet)->state,
		((struct tractor_cpacket *) packet)->pnum );
      break;
    case CP_REPRESS      :                    /* pressor on/off */
      LineToConsole("\nC->S CP_REPRESS\t");
      if (log_packets > 1)
	LineToConsole("  state=%d, pnum=%d,",
		((struct repress_cpacket *) packet)->state,
		((struct repress_cpacket *) packet)->pnum );
      break;
    case CP_COUP         :                    /* coup home planet */
      LineToConsole("\nC->S CP_COUP\t");
      if (log_packets > 1)
	LineToConsole("  no args,");
      break;
    case CP_SOCKET       :                    /* new socket for * *
				               * reconnection */
      LineToConsole("\nC->S CP_SOCKET\t");
      if (log_packets > 1)
	LineToConsole("  version=%d, udp_version=%d\n, socket=%u,",
		((struct socket_cpacket *) packet)->version, 
		((struct socket_cpacket *) packet)->udp_version,
		ntohl(((struct socket_cpacket *) packet)->socket)  );
      break;
    case CP_OPTIONS      :                    /* send my options to be * * 
					       * saved */
      LineToConsole("\nC->S CP_OPTIONS\t");
      if (log_packets > 1)
	LineToConsole("  flags=0x%0X, keymap=\"%s\",",
		ntohl(((struct options_cpacket *) packet)->flags),
		((struct options_cpacket *) packet)->keymap );
      break;
    case CP_BYE          :                    /* I'm done! */
      LineToConsole("\nC->S CP_BYE\t");
      if (log_packets > 1)
	LineToConsole("  no args,");
      break;
    case CP_DOCKPERM     :                    /* set docking permissions */
      LineToConsole("\nC->S CP_DOCKPERM\t");
      if (log_packets > 1)
	LineToConsole("  state=%d,",
		((struct dockperm_cpacket *) packet)->state );
      break;
    case CP_UPDATES      :                    /* set number of usecs per * 
				               * 
					       * * update */
      LineToConsole("\nC->S CP_UPDATES\t");
      if (log_packets > 1)
	LineToConsole("  usecs=%u,",
		ntohl(((struct updates_cpacket *) packet)->usecs) );
      break;
    case CP_RESETSTATS   :                    /* reset my stats packet */
      LineToConsole("\nC->S CP_RESETSTATS\t");
      if (log_packets > 1)
	LineToConsole("  verify=%c,",
		((struct resetstats_cpacket *) packet)->verify );
      break;
    case CP_RESERVED     :                    /* for future use */
      LineToConsole("\nC->S CP_RESERVED\t");
      if (log_packets > 1)
	{
	  LineToConsole("  data=" );
	  for( i = 0; i < 16; i++) 
            LineToConsole("0x%0X ",  (unsigned char)((struct reserved_cpacket *) packet)->data[i]);
	  LineToConsole(", resp=" );
	  for( i = 0; i < 16; i++) 
            LineToConsole("0x%0X ",  (unsigned char)((struct reserved_cpacket *) packet)->resp[i]);
	  LineToConsole(",");
	}
      break;
#ifdef INCLUDE_SCAN
      /* NOTE: not implemented. */
    case CP_SCAN         :                    /* ATM: request for player * 
					       * 
					       * * scan */
      LineToConsole("\nC->S CP_SCAN\t");
      if (log_packets > 1)
	LineToConsole("  not implemented," );
      break;
#endif
    case CP_UDP_REQ      :                    /* request UDP on/off */
      LineToConsole("\nC->S CP_UDP_REQ\t");
      if (log_packets > 1)
	LineToConsole("  request=%d, connmode=%d, port=%d,",
		((struct udp_req_cpacket *) packet)->request,
		((struct udp_req_cpacket *) packet)->connmode,
		ntohl(((struct udp_req_cpacket *) packet)->port) );
      break;
    case CP_SEQUENCE     :                    /* sequence # packet */
      LineToConsole("\nC->S CP_SEQUENCE\t");
      if (log_packets > 1)
	LineToConsole("  sequence=%u,",
		ntohs(((struct sequence_cpacket *) packet)->sequence) );
      break;
#ifdef RSA
    case CP_RSA_KEY      :                    /* handles binary * *
					       * verification */
      LineToConsole("\nC->S CP_RSA_KEY\t");
      if (log_packets > 1)
	{
	LineToConsole("  global=");
	for(i = 0; i < KEY_SIZE; i++)
	  LineToConsole("0x%0X ",((struct rsa_key_cpacket *)packet)->global[i]);
	fprintf(stderr,",");
	LineToConsole("  public=");
	for(i = 0; i < KEY_SIZE; i++)
	  LineToConsole("0x%0X ",((struct rsa_key_cpacket *)packet)->public[i]);
	fprintf(stderr,",");
	LineToConsole("  resp=");
	for(i = 0; i < KEY_SIZE; i++)
	  LineToConsole("0x%0X ",((struct rsa_key_cpacket *)packet)->resp[i]);
	fprintf(stderr,",");
	}
      break;
#endif
    case CP_PING_RESPONSE :                   /* client response */
      LineToConsole("\nC->S CP_PING_RESPONSE\t");
      if (log_packets > 1)
	LineToConsole("  number=%u, pingme=%d, cp_sent=%lu, cp_recv=%lu",
		((struct ping_cpacket *) packet)->number,
		((struct ping_cpacket *) packet)->pingme,
		ntohl(((struct ping_cpacket *) packet)->cp_sent),
		ntohl(((struct ping_cpacket *) packet)->cp_recv) );
      break;
#ifdef SHORT_PACKETS
    case CP_S_REQ        :          
      LineToConsole("\nC->S CP_S_REQ\t");
      if (log_packets > 1)
	LineToConsole("  req=%d, version=%d,",
		((struct shortreq_cpacket *) packet)->req,
		((struct shortreq_cpacket *) packet)->version );
      break;
    case CP_S_THRS       :         
      LineToConsole("\nC->S CP_S_THRS\t");
      if (log_packets > 1)
	LineToConsole("  thresh=%u,",
		ntohs(((struct threshold_cpacket *) packet)->thresh) );
      break;
    case CP_S_MESSAGE    :                    /* vari. Message Packet */
      LineToConsole("\nC->S CP_S_MESSAGE\t");
      if (log_packets > 1)
	LineToConsole("  size=%d, group=%d, indiv=%d, mess=\"%s\",",
                ((struct mesg_cpacket *) packet)->pad1,
                ((struct mesg_cpacket *) packet)->group,
                ((struct mesg_cpacket *) packet)->indiv,
                ((struct mesg_cpacket *) packet)->mesg );
      break;
    case CP_S_RESERVED   :      
      LineToConsole("\nC->S CP_S_RESERVED\t");
      if (log_packets > 1)
	LineToConsole("  no struct defined,");
      break;
    case CP_S_DUMMY      :
      LineToConsole("\nC->S CP_S_DUMMY\t");
      if (log_packets > 1)
	LineToConsole("  no struct defined,");
      break;
#endif
#ifdef FEATURE_PACKETS
    case CP_FEATURE      :  
      LineToConsole("\nC->S CP_FEATURE\t");
      if (log_packets > 1)
	LineToConsole("  feature_type=%c, arg1=%d, arg2=%d, value=%d, name=\"%s\",",
		((struct feature_cpacket *) packet)->feature_type,
		((struct feature_cpacket *) packet)->arg1,
		((struct feature_cpacket *) packet)->arg2,
		ntohl(((struct feature_cpacket *) packet)->value),
		((struct feature_cpacket *) packet)->name );
      break;
#endif
    default             :
       LineToConsole("\nC->S UNKNOWN\t");
       if(log_packets > 1)
	 LineToConsole("  type=%d,",packet[0]);
    }
}

#endif

char *
strcpyp_return (register char *s1,
                register char *s2,
                register int length)
{
    while (length && *s2)
    {
        *s1++ = *s2++;
        length--;
    }
    if (length > 0)
    {
        while (length--)
            *s1++ = ' ';
    }
    return s1;
}

char *
strcpy_return (register char *s1,
               register char *s2)
{
    while (*s2)
        *s1++ = *s2++;
    return s1;
}
