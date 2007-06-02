/* Automatically generated by refactor.pl.
 *
 *
 * CMDNAME: adduser
 * CMDLEVEL: QCMD_AUTHED
 * CMDARGS: 20
 * CMDDESC: Adds one or more users to a channel as +aot.
 * CMDFUNC: csc_doadduser
 * CMDPROTO: int csc_doadduser(void *source, int cargc, char **cargv);
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
  regchanuser *rcup;
  regchan *rcp;
  reguser *rup;
  int i;

  if (cargc<2) {
    chanservstdmessage(sender, QM_NOTENOUGHPARAMS, "adduser");
    return CMD_ERROR;
  }

  if (!(cip=cs_checkaccess(sender, cargv[0], CA_MASTERPRIV, NULL, "adduser", QPRIV_CHANGECHANLEV, 0)))
    return CMD_ERROR;

  rcp=cip->exts[chanservext];

  for (i=1;i<cargc;i++) {
    if (!(rup=findreguser(sender, cargv[i])))
      continue;

    if ((rcup=findreguseronchannel(rcp, rup))) {
      chanservstdmessage(sender, QM_ALREADYKNOWNONCHAN, cargv[i], cip->name->content);
      continue;
    }

    rcup=getregchanuser();
    rcup->chan=rcp;
    rcup->user=rup;
    rcup->flags = QCUFLAG_OP | QCUFLAG_AUTOOP | QCUFLAG_TOPIC;
    rcup->changetime=time(NULL);
    rcup->usetime=0;
    rcup->info=NULL;
   
    cs_log(sender,"CHANLEV %s #%s +aot (+ -> +aot)",cip->name->content,rup->username);
    addregusertochannel(rcup);
    csdb_createchanuser(rcup);
    csdb_chanlevhistory_insert(rcp, sender, rcup->user, 0, rcup->flags);
  }

  rcp->status |= QCSTAT_OPCHECK;
  cs_timerfunc(cip);

  chanservstdmessage(sender, QM_DONE);

  return CMD_OK;
}
