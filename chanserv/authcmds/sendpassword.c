/* Automatically generated by refactor.pl.
 *
 *
 * CMDNAME: sendpassword
 * CMDALIASES: sendpass
 * CMDLEVEL: QCMD_HELPER
 * CMDARGS: 1
 * CMDDESC: Sends the users current password by email.
 * CMDFUNC: csa_dosendpw
 * CMDPROTO: int csa_dosendpw(void *source, int cargc, char **cargv);
 * CMDHELP: Usage: @UCOMMAND@ <username>
 * CMDHELP: Sends the password for the specified account to the specified users email address.
 */

#include "../chanserv.h"
#include "../authlib.h"
#include "../../lib/irc_string.h"
#include <stdio.h>
#include <string.h>

int csa_dosendpw(void *source, int cargc, char **cargv) {
  reguser *rup;
  nick *sender=source;

  if (cargc<1) {
    chanservstdmessage(sender, QM_NOTENOUGHPARAMS, "sendpassword");
    return CMD_ERROR;
  }

  if (!(rup=findreguser(sender, cargv[0])))
    return CMD_ERROR;

  if(UHasHelperPriv(rup)) {
    chanservstdmessage(sender, QM_REQUESTPASSPRIVUSER);
    cs_log(sender,"SENDPASSWORD FAIL privilidged user %s",rup->username);
    return CMD_ERROR;
  }

  /* we don't reset the throttling timer
  rup->lastemailchange=time(NULL);
  csdb_updateuser(rup);
  */

  if(rup->lastauth) {
    csdb_createmail(rup, QMAIL_REQPW);
  } else {
    csdb_createmail(rup, QMAIL_NEWACCOUNT); /* user hasn't authed yet and needs to do the captcha */
  }
  chanservstdmessage(sender, QM_MAILQUEUED);
  cs_log(sender,"SENDPASSWORD username %s", rup->username);

  return CMD_OK;
}
