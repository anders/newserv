/* Automatically generated by refactor.pl.
 *
 *
 * CMDNAME: requestowner
 * CMDLEVEL: QCMD_AUTHED
 * CMDARGS: 1
 * CMDDESC: Requests ownership of a channel on which there are no owners.
 * CMDFUNC: csc_dorequestowner
 * CMDPROTO: int csc_dorequestowner(void *source, int cargc, char **cargv);
 * CMDHELP: Usage: requestowner <channel>
 * CMDHELP: Requests owner (+n) access to the named channel.  In order to use this command,
 * CMDHELP: there must be no owners left on the channel, and you must have the highest
 * CMDHELP: access level of the remaining users.  Where:
 * CMDHELP: channel - the channel to request ownership of
 */

#include "../chanserv.h"
#include "../../nick/nick.h"
#include "../../lib/flags.h"
#include "../../lib/irc_string.h"
#include "../../channel/channel.h"
#include "../../parser/parser.h"
#include "../../irc/irc.h"
#include "../../localuser/localuserchannel.h"
#include "../../lib/strlfunc.h"
#include <string.h>
#include <stdio.h>

int csc_dorequestowner(void *source, int cargc, char **cargv) {
  nick *sender=source;
  chanindex *cip;
  reguser *rup;
  regchan *rcp;
  regchanuser *rcup, *requester;
  int i;
  char flagbuf[20];
  flag_t oldflags;

  if (cargc<1) {
    chanservstdmessage(sender, QM_NOTENOUGHPARAMS, "requestowner");
    return CMD_ERROR;
  }

  if (!(cip=cs_checkaccess(sender, cargv[0], CA_VOICEPRIV, NULL, "requestowner", 0, 0)))
    return CMD_ERROR;

  rup=getreguserfromnick(sender);
  if(!rup)
    return CMD_ERROR;

  rcp=cip->exts[chanservext];
  requester=findreguseronchannel(rcp, rup);
  if (!requester || !rcp)
    return CMD_ERROR;

  for(i=0;i<REGCHANUSERHASHSIZE;i++) {
    for(rcup=rcp->regusers[i];rcup;rcup=rcup->nextbychan) {
      if (CUIsOwner(rcup) ||
         (CUIsMaster(rcup) && !CUHasMasterPriv(requester)) ||
         (CUIsOp(rcup) && !CUHasOpPriv(requester))) {
        chanservstdmessage(sender, QM_NOREQUESTOWNER, cip->name->content, CUIsOwner(rcup)?"owner":CUIsMaster(rcup)?"master":"op");
        return CMD_ERROR;
      }
    }
  }

  oldflags=requester->flags;
  strlcpy(flagbuf,printflags(oldflags,rcuflags),sizeof(flagbuf));

  CUSetOwner(requester);
  cs_log(sender,"REQUESTOWNER %s #%s (%s -> %s)",cip->name->content,requester->user->username,flagbuf,printflags(requester->flags,rcuflags));
  csdb_chanlevhistory_insert(rcp, sender, requester->user, oldflags, requester->flags);
  csdb_updatechanuser(requester);

  chanservstdmessage(sender, QM_GRANTEDOWNER, cip->name->content);
  return CMD_OK;
}