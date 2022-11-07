%{
/*
 *   Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007
 *   NOVELL (All rights reserved)
 *   Copyright (c) 2010-2012
 *   Canonical Ltd.
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of version 2 of the GNU General Public
 *   License published by the Free Software Foundation.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, contact Canonical, Ltd.
 */

#define YYERROR_VERBOSE 1
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/apparmor.h>

#include <iostream>

/* #define DEBUG */

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define CIDR_32 htonl(0xffffffff)
#define CIDR_24 htonl(0xffffff00)
#define CIDR_16 htonl(0xffff0000)
#define CIDR_8  htonl(0xff000000)

/* undefine linux/capability.h CAP_TO_MASK */
#ifdef CAP_TO_MASK
#undef CAP_TO_MASK
#endif

#define CAP_TO_MASK(x) (1ull << (x))

#define EXEC_MODE_EMPTY		0
#define EXEC_MODE_UNSAFE	1
#define EXEC_MODE_SAFE		2

int parser_token = 0;

%}

%token TOK_ID
%token TOK_CONDID
%token TOK_CONDLISTID
%token TOK_CARET
%token TOK_OPEN
%token TOK_CLOSE
%token TOK_MODE
%token TOK_END_OF_RULE
%token TOK_EQUALS
%token TOK_ARROW
%token TOK_ADD_ASSIGN
%token TOK_LE
%token TOK_SET_VAR
%token TOK_BOOL_VAR
%token TOK_VALUE
%token TOK_IF
%token TOK_ELSE
%token TOK_NOT
%token TOK_DEFINED
%token TOK_CHANGE_PROFILE
%token TOK_NETWORK
%token TOK_UNIX
%token TOK_CREATE
%token TOK_SHUTDOWN
%token TOK_ACCEPT
%token TOK_CONNECT
%token TOK_LISTEN
%token TOK_SETOPT
%token TOK_GETOPT
%token TOK_SETATTR
%token TOK_GETATTR
%token TOK_HAT
%token TOK_UNSAFE
%token TOK_SAFE
%token TOK_COLON
%token TOK_LINK
%token TOK_OWNER
%token TOK_OTHER
%token TOK_SUBSET
%token TOK_AUDIT
%token TOK_DENY
%token TOK_ALLOW
%token TOK_PROFILE
%token TOK_SET
%token TOK_ALIAS
%token TOK_PTRACE
%token TOK_OPENPAREN
%token TOK_CLOSEPAREN
%token TOK_COMMA
%token TOK_FILE
%token TOK_MOUNT
%token TOK_REMOUNT
%token TOK_UMOUNT
%token TOK_PIVOTROOT
%token TOK_IN
%token TOK_DBUS
%token TOK_SIGNAL
%token TOK_SEND
%token TOK_RECEIVE
%token TOK_BIND
%token TOK_READ
%token TOK_WRITE
%token TOK_EAVESDROP
%token TOK_PEER
%token TOK_TRACE
%token TOK_TRACEDBY
%token TOK_READBY
%token TOK_ABI
%token TOK_USERNS

 /* rlimits */
%token TOK_RLIMIT
%token TOK_SOFT_RLIMIT
%token TOK_RLIMIT_CPU
%token TOK_RLIMIT_FSIZE
%token TOK_RLIMIT_DATA
%token TOK_RLIMIT_STACK
%token TOK_RLIMIT_CORE
%token TOK_RLIMIT_RSS
%token TOK_RLIMIT_NOFILE
%token TOK_RLIMIT_OFILE
%token TOK_RLIMIT_AS
%token TOK_RLIMIT_NPROC
%token TOK_RLIMIT_MEMLOCK
%token TOK_RLIMIT_LOCKS
%token TOK_RLIMIT_SIGPENDING
%token TOK_RLIMIT_MSGQUEUE
%token TOK_RLIMIT_NICE
%token TOK_RLIMIT_RTPRIO

/* capabilities */
%token TOK_CAPABILITY

/* debug flag values */
%token TOK_FLAGS

/*
%code requires {
}
*/ 

// %language "c++"

%union {
	char *id;
	char *flag_id;
	char *mode;

	int fmode;
	uint64_t cap;
	unsigned int allowed_protocol;
	char *set_var;
	char *bool_var;
	char *var_val;
	int boolean;
};

%type <id> 	TOK_ID
%type <id>	TOK_CONDID
%type <id>	TOK_CONDLISTID
%type <mode> 	TOK_MODE
%type <fmode>   file_mode
%type <prof>	profile_base
%type <prof> 	profile
%type <prof>	rules
%type <prof>	hat
%type <prof>	local_profile
%type <prof>	cond_rule
%type <network_entry> network_rule
%type <user_entry> rule
%type <user_entry> file_rule
%type <user_entry> file_rule_tail
%type <user_entry> link_rule
%type <user_entry> frule
%type <mnt_entry> mnt_rule
%type <cond_entry> opt_conds
%type <cond_entry> cond
%type <cond_entry_list> cond_list
%type <cond_entry_list> opt_cond_list
%type <flags>	flags
%type <flags>	flagvals
%type <flags>	flagval
%type <cap>	caps
%type <cap>	capability
%type <user_entry> change_profile
%type <set_var> TOK_SET_VAR
%type <bool_var> TOK_BOOL_VAR
%type <var_val>	TOK_VALUE
%type <val_list> valuelist
%type <boolean> expr
%type <id>	id_or_var
%type <id>	opt_id_or_var
%type <boolean> opt_subset_flag
%type <boolean> opt_audit_flag
%type <boolean> opt_owner_flag
%type <boolean> opt_profile_flag
%type <boolean> opt_flags
%type <boolean> opt_perm_mode
%type <id>	opt_id
%type <prefix>  opt_prefix
%type <fmode>	dbus_perm
%type <fmode>	dbus_perms
%type <fmode>	opt_dbus_perm
%type <dbus_entry>	dbus_rule
%type <fmode>	signal_perm
%type <fmode>	signal_perms
%type <fmode>	opt_signal_perm
%type <signal_entry>	signal_rule
%type <fmode>	ptrace_perm
%type <fmode>	ptrace_perms
%type <fmode>	opt_ptrace_perm
%type <ptrace_entry>	ptrace_rule
%type <fmode>	net_perm
%type <fmode>	net_perms
%type <fmode>	opt_net_perm
%type <unix_entry>	unix_rule
%type <id>	opt_target
%type <id>	opt_named_transition
%type <boolean> opt_exec_mode
%type <boolean> opt_file
%type <fmode>  	userns_perm
%type <fmode>  	userns_perms
%type <fmode>	opt_userns_perm
%type <userns_entry>	userns_rule
%%


list:	 preamble
	{
	}
	profilelist;

profilelist:	{};

profilelist:	profilelist profile
	{
	};

opt_profile_flag:
	| TOK_PROFILE
	| hat_start

opt_id: {}
	| TOK_ID {}

opt_id_or_var: {}
	| id_or_var {}

profile_base: TOK_ID opt_id_or_var opt_cond_list flags TOK_OPEN
	{
	}
    rules TOK_CLOSE
	{
	};

profile:  opt_profile_flag profile_base
	{
	};

local_profile:   TOK_PROFILE profile_base
	{
	};

hat: hat_start profile_base
	{
	};

preamble: {}
	| preamble alias {};
	| preamble varassign {};
	| preamble abi_rule {};

alias: TOK_ALIAS TOK_ID TOK_ARROW TOK_ID TOK_END_OF_RULE
	{
	};

varassign:	TOK_SET_VAR TOK_EQUALS valuelist
	{
	}

varassign:	TOK_SET_VAR TOK_ADD_ASSIGN valuelist
	{
	}

varassign:	TOK_BOOL_VAR TOK_EQUALS TOK_VALUE
	{
	}

valuelist:	TOK_VALUE
	{
	}

valuelist:	valuelist TOK_VALUE
	{
	}

flags:	{};

opt_flags: {}
	| TOK_CONDID TOK_EQUALS
	{
	}

flags:	opt_flags TOK_OPENPAREN flagvals TOK_CLOSEPAREN
	{
	};

flagvals:	flagvals flagval
	{
	};

flagvals:	flagval
	{
	};

flagval:	TOK_VALUE
	{
	};

opt_subset_flag: {}
	| TOK_SUBSET {}
	| TOK_LE {}

opt_audit_flag: {}
	| TOK_AUDIT {};

opt_owner_flag: {}
	| TOK_OWNER {};
	| TOK_OTHER {};

opt_perm_mode: {}
	| TOK_ALLOW {}
	| TOK_DENY {}

opt_prefix: opt_audit_flag opt_perm_mode opt_owner_flag
	{
	}

rules:	{ /* nothing */ 
	};

rules: rules abi_rule {}

rules:  rules opt_prefix rule
	{
	};


rules: rules opt_prefix TOK_OPEN rules TOK_CLOSE
	{
	};

rules: rules opt_prefix network_rule
	{
	}

rules:  rules opt_prefix mnt_rule
	{
	}

rules:  rules opt_prefix dbus_rule
	{
	}

rules:  rules opt_prefix signal_rule
	{
	}

rules:  rules opt_prefix ptrace_rule
	{
	}

rules:  rules opt_prefix unix_rule
	{
	}

rules:  rules opt_prefix userns_rule
	{
	}

rules:	rules opt_prefix change_profile
	{
	};

rules:  rules opt_prefix capability
	{
	};

rules:	rules hat
	{
	};

rules:	rules local_profile
	{
	};

rules:	rules cond_rule
	{
	}

rules: rules TOK_SET TOK_RLIMIT TOK_ID TOK_LE TOK_VALUE opt_id TOK_END_OF_RULE

cond_rule: TOK_IF expr TOK_OPEN rules TOK_CLOSE

cond_rule: TOK_IF expr TOK_OPEN rules TOK_CLOSE TOK_ELSE TOK_OPEN rules TOK_CLOSE

cond_rule: TOK_IF expr TOK_OPEN rules TOK_CLOSE TOK_ELSE cond_rule

expr:	TOK_NOT expr

expr:	TOK_BOOL_VAR

expr:	TOK_DEFINED TOK_SET_VAR

expr:	TOK_DEFINED TOK_BOOL_VAR

id_or_var: TOK_ID {}
id_or_var: TOK_SET_VAR {};

opt_target: /* nothing */ {}
opt_target: TOK_ARROW id_or_var {};

opt_named_transition: {}
	| TOK_ARROW id_or_var
	{
	};

rule: file_rule {}
	| link_rule {}

abi_rule: TOK_ABI TOK_ID TOK_END_OF_RULE
	| TOK_ABI TOK_VALUE TOK_END_OF_RULE

opt_exec_mode: {}
	| TOK_UNSAFE {};
	| TOK_SAFE {};

opt_file: {}
	| TOK_FILE {}

frule:	id_or_var file_mode opt_named_transition TOK_END_OF_RULE

frule:	file_mode opt_subset_flag id_or_var opt_named_transition TOK_END_OF_RULE

file_rule: TOK_FILE TOK_END_OF_RULE
	| opt_file file_rule_tail {}


file_rule_tail: opt_exec_mode frule

file_rule_tail: opt_exec_mode id_or_var file_mode id_or_var

link_rule: TOK_LINK opt_subset_flag id_or_var TOK_ARROW id_or_var TOK_END_OF_RULE

network_rule: TOK_NETWORK TOK_END_OF_RULE

network_rule: TOK_NETWORK TOK_ID TOK_END_OF_RULE

network_rule: TOK_NETWORK TOK_ID TOK_ID TOK_END_OF_RULE

cond: TOK_CONDID

cond: TOK_CONDID TOK_EQUALS TOK_VALUE

cond: TOK_CONDID TOK_EQUALS TOK_OPENPAREN valuelist TOK_CLOSEPAREN

cond: TOK_CONDID TOK_IN TOK_OPENPAREN valuelist TOK_CLOSEPAREN

opt_conds: {}
	| opt_conds cond

cond_list: TOK_CONDLISTID TOK_EQUALS TOK_OPENPAREN opt_conds TOK_CLOSEPAREN

opt_cond_list: {}
	| cond_list {}

mnt_rule: TOK_MOUNT opt_conds opt_id TOK_END_OF_RULE

mnt_rule: TOK_MOUNT opt_conds opt_id TOK_ARROW opt_conds TOK_ID TOK_END_OF_RULE

mnt_rule: TOK_REMOUNT opt_conds opt_id TOK_END_OF_RULE

mnt_rule: TOK_UMOUNT opt_conds opt_id TOK_END_OF_RULE

mnt_rule: TOK_PIVOTROOT opt_conds opt_id opt_target TOK_END_OF_RULE

dbus_perm: TOK_VALUE
	| TOK_BIND {}
	| TOK_SEND {}
	| TOK_RECEIVE {}
	| TOK_READ {}
	| TOK_WRITE {}
	| TOK_EAVESDROP {}
	| TOK_MODE

dbus_perms: {}
	| dbus_perms dbus_perm {}
	| dbus_perms TOK_COMMA dbus_perm {}

opt_dbus_perm: {}
	| dbus_perm  {}
	| TOK_OPENPAREN dbus_perms TOK_CLOSEPAREN {}

dbus_rule: TOK_DBUS opt_dbus_perm opt_conds opt_cond_list TOK_END_OF_RULE

net_perm: TOK_VALUE
	| TOK_CREATE {}
	| TOK_BIND {}
	| TOK_LISTEN {}
	| TOK_ACCEPT {}
	| TOK_CONNECT {}
	| TOK_SHUTDOWN {}
	| TOK_GETATTR {}
	| TOK_SETATTR {}
	| TOK_GETOPT {}
	| TOK_SETOPT {}
	| TOK_SEND {}
	| TOK_RECEIVE {}
	| TOK_READ {}
	| TOK_WRITE {}
	| TOK_MODE

net_perms: {}
	| net_perms net_perm {}
	| net_perms TOK_COMMA net_perm {}

opt_net_perm: {}
	| net_perm  {}
	| TOK_OPENPAREN net_perms TOK_CLOSEPAREN {}

unix_rule: TOK_UNIX opt_net_perm opt_conds opt_cond_list TOK_END_OF_RULE

signal_perm: TOK_VALUE
	| TOK_SEND {}
	| TOK_RECEIVE {}
	| TOK_READ {}
	| TOK_WRITE {}
	| TOK_MODE

signal_perms: {}
	| signal_perms signal_perm {}
	| signal_perms TOK_COMMA signal_perm {}

opt_signal_perm: {}
	| signal_perm {}
	| TOK_OPENPAREN signal_perms TOK_CLOSEPAREN {}

signal_rule: TOK_SIGNAL opt_signal_perm opt_conds TOK_END_OF_RULE

ptrace_perm: TOK_VALUE
	| TOK_TRACE {}
	| TOK_TRACEDBY {}
	| TOK_READ {}
	| TOK_WRITE {}
	| TOK_READBY {}
	| TOK_MODE

ptrace_perms: {}
	| ptrace_perms ptrace_perm {}
	| ptrace_perms TOK_COMMA ptrace_perm {}

opt_ptrace_perm: {}
	| ptrace_perm {}
	| TOK_OPENPAREN ptrace_perms TOK_CLOSEPAREN {}

ptrace_rule: TOK_PTRACE opt_ptrace_perm opt_conds TOK_END_OF_RULE

userns_perm: TOK_VALUE
	| TOK_CREATE {}

userns_perms: {}
	| userns_perms userns_perm {}
	| userns_perms TOK_COMMA userns_perm {}

opt_userns_perm: {}
	| userns_perm {}
	| TOK_OPENPAREN userns_perms TOK_CLOSEPAREN {}

userns_rule: TOK_USERNS opt_userns_perm opt_conds TOK_END_OF_RULE

hat_start: TOK_CARET {}
	| TOK_HAT {}

file_mode: TOK_MODE

change_profile: TOK_CHANGE_PROFILE opt_exec_mode opt_id opt_named_transition TOK_END_OF_RULE

capability:	TOK_CAPABILITY caps TOK_END_OF_RULE

caps: {}
	| caps TOK_ID {}
%%

#define MAXBUFSIZE 4096

void vprintyyerror(const char *msg, va_list argptr)
{
	char buf[MAXBUFSIZE];

	vsnprintf(buf, sizeof(buf), msg, argptr);
}

void printyyerror(const char *msg, ...)
{
	va_list arg;

	va_start(arg, msg);
	vprintyyerror(msg, arg);
	va_end(arg);
}

void yyerror(const char *msg, ...)
{
	va_list arg;

	va_start(arg, msg);
	vprintyyerror(msg, arg);
	va_end(arg);

	exit(1);
}
