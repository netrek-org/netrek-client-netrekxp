#ifndef meta_h_header
#define meta_h_header
#ifdef META

/*
   meta.h
*/


/* Global Variables */

extern int num_servers;         /* The number of servers in the list */


/* Function Definitions */

void parsemeta (int metaType);
/*
 * Read and Parse the meta-server information, either from the meta-
 * server (if metaType = 1) or from the cache (if metaType = 2).
 *  
 * NOTE: This function sets the variable "num_servers" which is
 * later used to decide the hight of the meta-server window.
 */


void metawindow (void);
/*
 *  Show the meta server menu window
 */


void metaaction (W_Event * data);
/*
 *  Recieve an action in the meta server window.  Check selection to see
 *  if was valid.  If it was then we have a winner!
 */


void metainput (void);
/*  
 *  Wait for actions in the meta-server window.
 *
 *  This is really the meta-server window's own little input() function.
 *  It is needed so we don't have to use all the bull in the main
 *  input(). Plus to use it I'd have to call mapAll() first and the client
 *  would read in the default server and then call it up before I can
 *  select a server.
 */
 
int metablock(char *host);
/* Block connections to known servers not willing to handle default logins */


#endif /* defined META */
#endif /* defined meta_h_header */
