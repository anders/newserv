/* Automatically generated by refactor.pl.
 *
 *
 * CMDNAME: adduser
 * CMDLEVEL: QCMD_AUTHED
 * CMDARGS: 20
 * CMDDESC: Adds one or more users to a channel as +aot.
 * CMDFUNC: csc_doadduser
 * CMDPROTO: int csc_doadduser(void *source, int cargc, char **cargv);
 * CMDHELP: Usage: ADDUSER <channel> [<flags>] <user1> [<user2> [<user3> [...]]]
 * CMDHELP: Adds the named user(s) to the channel, where:
 * CMDHELP: channel - the channel to use
 * CMDHELP: flags   - the list of flags to add for each user, introduced by + (for example
 * CMDHELP:           +gv).  See CHANLEV for valid flags.  If no flags are specified, 
 * CMDHELP:           +aot is used.  This command cannot be used to add masters (+m) or
 * CMDHELP:           owners (+n).
 * CMDHELP: user<n> - either a user's current nickname on the network or #accountname. Up to
 * CMDHELP:           18 users can be specified.
 * CMDHELP: ADDUSER requires master (+m) access on the named channel.
 */

#include "../chanserv.h"
#include "../../nick/nick.h"
#include "../../lib/flags.h"
#include "../../lib/irc_string.h"
#include "../../channel/channel.h"
#include "../../parser/parser.h"
#include "../../irc/irc.h"
#include "../../localuser/localuserchannel.h"
#include <string.h>
#include <stdio.h>

int csc_doadduser(void *source, int cargc, char **cargv) {
  nick *sender=source;
  chanindex *cip;
  regchanuser *rcup, *rcuplist;
  regchan *rcp;
  reguser *rup;
  flag_t addflags;
  char *flagbuf;
  unsigned int count=0;  
  int i;

  if (cargc<2) {
    chanservstdmessage(sender, QM_NOTENOUGHPARAMS, "adduser");
    return CMD_ERROR;
  }

  if (!(cip=cs_checkaccess(sender, cargv[0], CA_MASTERPRIV, NULL, "adduser", QPRIV_CHANGECHANLEV, 0)))
    return CMD_ERROR;

  rcp=cip->exts[chanservext];

  /* See if there are flags defined */
  if (*cargv[1] == '+') {
    /* If there are we now need at least 3 parameters */
    if (cargc<3) {
      chanservstdmessage(sender, QM_NOTENOUGHPARAMS, "adduser");
      return CMD_ERROR;
    }
    addflags=0;
    
    /* Set the flags.  We allow everything except personal flags and +mn. */
    setflags(&addflags, QCUFLAG_ALL & ~(QCUFLAGS_PERSONAL | QCUFLAG_MASTER | QCUFLAG_OWNER), cargv[1], rcuflags, 0);

    /* Remove impossible combinations */
    addflags=cs_sanitisechanlev(addflags);
    
    /* It helps a lot if they have actually set something */
    if (!addflags) {
      chanservstdmessage(sender, QM_NOFLAGSPECIFIED);
      return CMD_ERROR;
    }
    
    i++;
  } else {
    addflags=QCUFLAG_OP | QCUFLAG_AUTOOP | QCUFLAG_TOPIC;
  }

  flagbuf=printflags(addflags, rcuflags);  

  /* ugh */
  for (count=i=0;i<REGCHANUSERHASHSIZE;i++)
    for (rcuplist=rcp->regusers[i];rcuplist;rcuplist=rcuplist->nextbychan)
      count++;

  for (i=1;i<cargc;i++) {
    if (!(rup=findreguser(sender, cargv[i])))
      continue;

    if ((rcup=findreguseronchannel(rcp, rup))) {
      chanservstdmessage(sender, QM_ALREADYKNOWNONCHAN, cargv[i], cip->name->content);
      continue;
    }

    if(count++ >= MAXCHANLEVS) {
      chanservstdmessage(sender, QM_TOOMANYCHANLEVS);
      chanservstdmessage(sender, QM_DONE);
      return CMD_OK;
    }

    rcup=getregchanuser();
    rcup->chan=rcp;
    rcup->user=rup;
    rcup->flags = addflags;
    rcup->changetime=time(NULL);
    rcup->usetime=0;
    rcup->info=NULL;
   
    cs_log(sender,"CHANLEV %s #%s %s (+ -> %s)",cip->name->content,rup->username, flagbuf, flagbuf);
    addregusertochannel(rcup);
    csdb_createchanuser(rcup);
    csdb_chanlevhistory_insert(rcp, sender, rcup->user, 0, rcup->flags);
  }

  rcp->status |= QCSTAT_OPCHECK;
  cs_timerfunc(cip);

  chanservstdmessage(sender, QM_DONE);

  return CMD_OK;
}
