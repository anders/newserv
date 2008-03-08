#include "../chanserv.h"

cslang *cslanguages[MAXLANG];
unsigned int cslangcount;

sstring *csmessages[MAXLANG][MAXMESSAGES];

char *defaultmessages[MAXMESSAGES] = {
  /*  0 */ "Channel %s is protected by %s.",
  /*  1 */ "%s: Unknown command.",
  /*  2 */ "To use the %s command, you must use /msg %s@%s.",
  /*  3 */ "%s is only available to authed users.",
  /*  4 */ "%s is only available to unauthed users.",
  /*  5 */ "The following commands are available to you:",
  /*  6 */ "End of list.",
  /*  7 */ "Done.",
  /*  8 */ "%s: Not enough parameters.",
  /*  9 */ "Channel %s is unknown or suspended.",
  /* 10 */ "You do not have sufficient access on %s to use %s.",
  /* 11 */ "Current channel flags for %s: %s",
  /* 12 */ "Current forced modes on %s: %s",
  /* 13 */ "You do not have sufficient privileges to use %s.",
  /* 14 */ "Unknown user %s.",
  /* 15 */ "User %s is not authed.",
  /* 16 */ "Known users on %s:",
  /* 17 */ "Username        Flags",
  /* 18 */ "Username        Flags         Last join       Last changed    Info",
  /* 19 */ "No known users found on %s.",
  /* 20 */ "Flags for %s on %s: %s",
  /* 21 */ "User %s is not known on %s.",
  /* 22 */ "Invalid or disallowed flag specified.",
  /* 23 */ "%s is authed as %s.",
  /* 24 */ "Network Staff  : Helper",
  /* 25 */ "Network Staff  : IRC Operator",
  /* 26 */ "Network Staff  : IRC Administrator",
  /* 27 */ "Network Staff  : Developer",
  /* 28 */ "%s is known on the following channels:",
  /* 29 */ "Channel                        Flags",
  /* 30 */ "%s is not known on any channels.",
  /* 31 */ "Current autolimit setting on %s: %d",
  /* 32 */ "Bans on %s will be removed after: %s",
  /* 33 */ "Bans on %s will not be automatically removed.",
  /* 34 */ "%s is not a valid channel name.",
  /* 35 */ "%s is already registered.",
  /* 36 */ "User flags for %s: %s",
  /* 37 */ "Welcome message for %s: %s",
  /* 38 */ "Default info line: %s",
  /* 39 */ "Info line on %s: %s",
  /* 40 */ "%s is a protected helper or oper nick.  Please use a different nickname.",
  /* 41 */ "I created a fakeuser %s to prevent your nick being stolen.",
  /* 42 */ "Sorry, your nick %s is not protected.",
  /* 43 */ "%s is authed as %s.  Cannot reclaim nick.",
  /* 44 */ "Username or password incorrect.",
  /* 45 */ "You are now logged in as %s.",
  /* 46 */ "User %s created, account information will be sent to \"%s\"",
  /* 47 */ "A user with that name already exists.  Change your nick and try again.",
  /* 48 */ "You're already on %s.",
  /* 49 */ "You're not on %s.",
  /* 50 */ "You're already opped on %s.",
  /* 51 */ "%s is not on %s.",
  /* 52 */ "%s is already opped on %s.",
  /* 53 */ "Channel settings prevent %s being opped on %s.",
  /* 54 */ "You're not opped on %s.",
  /* 55 */ "You're already voiced on %s.",
  /* 56 */ "You're not voiced on %s.",
  /* 57 */ "%s is already voiced on %s.",
  /* 58 */ "Registered bans on %s:\n"
  /* .. */ "ID  Hostmask                      Expires            Set by           Reason",
  /* 59 */ "No bans on %s.",
  /* 60 */ "Removed channel ban %s from %s.",
  /* 61 */ "You need the master flag to remove registered ban %s from %s.",
  /* 62 */ "Removed registered ban %s from %s.",
  /* 63 */ "Channel settings prevent %s being voiced on %s.",
  /* 64 */ "Sorry, no help available for %s.",  
  /* 65 */ "-Information for user %s (using account %s)-----",
  /* 66 */ "-Information for account %s-----",
  /* 67 */ "Account users  : %s",
  /* 68 */ "Last auth      : %s",
  /* 69 */ "Last user@host : %s",
  /* 70 */ "User ID        : %d",
  /* 71 */ "Info           : %s",
  /* 72 */ "Staff comment  : %s",
  /* 73 */ "User flags     : %s",
  /* 74 */ "User created   : %s",
  /* 75 */ "Known on the following channels:\n"
  /* .. */ "Channel                        Flags", 
  /* 76 */ "Email address  : %s",
  /* 77 */ "Email last set : %s",
  /* 78 */ "Staff comment for %s: %s.",
  /* 79 */ "No staff comment for %s.",
  /* 80 */ "%s: Your account has been deleted or suspended.",
  /* 81 */ "Account %s has been deleted.",
  /* 82 */ "Wrong email address for account \"%s\"",
  /* 83 */ "Sorry, but you have already requested a password. You will have to wait atleast %.1f hours, before requesting again.",
  /* 84 */ "Mail queued for delivery",
  /* 85 */ "Sorry, but passwords do not match",
  /* 86 */ "Sorry, but new password is to short",
  /* 87 */ "Ok, password changed",
  /* 88 */ "Duration too long or invalid: %s",
  /* 89 */ "Ok, email changed to \"%s\"",
  /* 90 */ "Sorry, but first and second email addresses don't match",
  /* 91 */ "%s is not a valid email address",
  /* 92 */ "Email (%s) too short",
  /* 93 */ "\"%s\" is not your email address",
  /* 94 */ "Email (%s) must contain at least one at sign (@)",
  /* 95 */ "Email (%s) can't end with an at sign (@)",
  /* 96 */ "Email (%s) contains invalid characters",
  /* 97 */ "Channel %s is not suspended.",
  /* 98 */ "Channel %s is already suspended.",
  /* 99 */ "Channel type: %s",
  /* 100*/ "%s: unknown channel type",
  /* 101*/ "Original founder: %s",
  /* 102*/ "Added by: %s",
  /* 103*/ "%s is already known on %s.",
  /* 104*/ "Not removing channel owner %s from %s.",
  /* 105*/ "Not removing channel master %s from %s.",
  /* 106*/ "Can't find ban %s on %s.",
  /* 107*/ "Your language is currently set to: %s.",
  /* 108*/ "The following languages are currently known:",
  /* 109*/ "Unknown language code: %s.",
  /* 110*/ "Language       : %s",
  /* 111*/ "Channel type for %s is: %s",
  /* 112*/ "Statistics for %s:",
  /* 113*/ "Date added: %s",
  /* 114*/ "Since %s: maximum size %d, %d joins (%.1f joins/day).",
  /* 115*/ "Last active: %s",
  /* 116*/ "Your Q account is suspended.",
  /* 117*/ "Reason: %s",
  /* 118*/ "Expires: %s",
  /* 119*/ "Too many users AUTH'd to this account.",
  /* 120*/ "Duration must be between 1d and 1M.",
  /* 121*/ "Disconnecting %s (authed as %s)",
  /* 122*/ "User is already suspended.",
  /* 123*/ "User %s is not suspended.",
  /* 124*/ "Username:       Suspended: Email:                         Last user@host:",
  /* 125*/ "Found over %d %s, truncating list.",
  /* 126*/ "Found %u %s%s.",
  /* 127*/ "Attention: Your Q account has been suspended. You are being disconnected.",
  /* 128*/ "Username:       Flags:            Suspended: Email:                         Last user@host:",
  /* 129*/ "Username:       Suspend type: Suspended by:   Expires:        Reason:",
  /* 130*/ "Channel:                       Suspended by:   Reason:",
  /* 131*/ "You cannot request ownership of %s as there is a channel %s present.",
  /* 132*/ "You have been granted ownership of %s.",
  /* 133*/ "#:  User:                                              Authed:         Disconnected:   Reason:",
  /* 134*/ "Current modes on %s: %s",
  /* 135*/ "Domain:             Users:  Limit:",
  /* 136*/ "Too many accounts exist from this email domain",
  /* 137*/ "Password too weak - You need to avoid repeating characters, and have different character types (e.g. 1 number and 1 letter)",
  /* 138*/ "User %s is not a master on %s - you must promote user to master status first.",
  /* 139*/ "User %s is already an owner on %s.",
  /* 140*/ "WARNING: This command will give COMPLETE control over %s to %s, including the ability to remove you as owner.\nIf you are sure you want to do this, type /msg Q GIVEOWNER %s #%s %s",
  /* 141*/ "Hash value incorrect.",
  /* 142*/ "Showing %s for last %s.",
  /* 143*/ "Change your nickname to something with none of the following characters: `~^[]{}|_\\",
  /* 144*/ "Invalid digest algorithm.",
  /* 145*/ "Challenge has either not been requested or has expired.",
  /* 146*/ "To give the +n flag use GIVEOWNER. Change aborted.",
  /* 147*/ "You must specify at least one valid flag to add.",
  /* 148*/ "New ban is already contained within existing ban: %s",
  /* 149*/ "New ban would replace at least one existing ban (%s), use UNBANMASK %s first if you want to set this ban.",
  /* 150*/ "A temporary ban with this mask already exists, replacing it.",
  /* 151*/ "That permanent ban already exists.",
  /* 152*/ "A ban with the same mask and longer duration exists, aborting.",
  /* 153*/ "Replacing existing ban with the same mask as it has a shorter duration.",
  /* 154*/ "Channel has too many user entries, aborting.",
  /* 155*/ "Channel has too many bans set, aborting.",
};

void initmessages() {
  int i;
  int j;
  
  for (i=0;i<MAXLANG;i++) {
    cslanguages[i]=NULL;
    for (j=0;j<MAXMESSAGES;j++) {
      csmessages[i][j]=NULL;
    }
  }
}
