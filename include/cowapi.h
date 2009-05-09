/* cowapi.h    -- The COW Aplication interface */

/* Copyright (c) 1996   Kurt Siegl
 * 
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
 *
 * The COW development team
 */

#ifndef _h_cowapi
#define _h_cowapi

/* COW mainloop - starts up a client window */

extern int cowmain (char *server,
                    int port,
                    char *name);


/* Variables passing Optional Arguments to cowmain */

extern char *deffile;
extern char *recordFileName;
extern char *logFileName;
extern char *display_host;
extern int passive;
extern int checking;
extern char *deffile;

#ifdef META
extern int usemeta;
#endif

/* Global COW Variables which may be set and/or used outside */

/* Version Information */
extern char cflags[], arch[], cdate[], cbugs[], cowid[], cwho[];
extern struct timeval tv_ctime;

#ifndef PSEUDOSIZE
#define PSEUDOSIZE 16
#endif
extern char pseudo[PSEUDOSIZE];
extern char defpasswd[PSEUDOSIZE];
extern char login[PSEUDOSIZE];

extern int baseUdpLocalPort;
extern int log_packets;

extern int ghoststart;
extern int ghost_pno;
extern int debug;
extern int ignore_signals;

extern char *title;

#ifdef GATEWAY
extern int use_trekhopd;
extern unsigned LONG netaddr;
#endif

#endif /* _h_cow */
