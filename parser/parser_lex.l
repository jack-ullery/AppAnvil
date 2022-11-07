/*
 *   Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007
 *   NOVELL (All rights reserved)
 *   Copyright (c) 2010 - 2013
 *   Canonical Ltd. (All rights reserved)
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

/* Definitions section */
/* %option main */

/* options set to noXXX eliminates need to link with libfl */
%option noyywrap
/* set %option noyy_top_state in Makefile, so can be used when DEBUG=1 */
%option nounput
%option stack
%option nodefault

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <unordered_map>
#include <string>

#ifdef PDEBUG
#undef PDEBUG
#endif
/* #define DEBUG */
#ifdef DEBUG
static int yy_top_state(void);
#define PDEBUG(fmt, args...) fprintf(stderr, "Lexer (Line %d) (state %s): " fmt, current_lineno, state_names[YY_START].c_str(), ## args)
#else
#define PDEBUG(fmt, args...)	/* Do nothing */
#endif
#define NPDEBUG(fmt, args...)	/* Do nothing */

#define DUMP_PREPROCESS do { if (preprocess_only) ECHO; } while (0)

#define DUMP_AND_DEBUG(X...) \
do { \
	DUMP_PREPROCESS; \
	PDEBUG(X); \
} while (0)

#define EAT_TOKEN(X...) DUMP_AND_DEBUG(X)

#define RETURN_TOKEN(X) \
do { \
	DUMP_AND_DEBUG("Matched: '%s'	Returning(%s)\n", yytext, #X); \
	return (X); \
} while (0)

#define POP() \
do { \
	DUMP_AND_DEBUG(" (pop_to(%s)): Matched: %s\n", state_names[yy_top_state()].c_str(), yytext); \
	yy_pop_state(); \
} while (0)

#define POP_NODUMP() \
do { \
	PDEBUG(" (pop_to(%s)): Matched: %s\n", state_names[yy_top_state()].c_str(), yytext); \
	yy_pop_state(); \
} while (0)

#define PUSH(X) \
do { \
	DUMP_AND_DEBUG(" (push(%s)): Matched: %s\n", state_names[(X)].c_str(), yytext); \
	yy_push_state(X); \
} while (0)

#define POP_AND_RETURN(X) \
do { \
	POP(); \
	return (X); \
} while (0)

#define PUSH_AND_RETURN(X, Y) \
do { \
	PUSH(X); \
	return (Y); \
} while (0)

#define BEGIN_AND_RETURN(X, Y) \
do { \
	DUMP_AND_DEBUG(" (begin(%s)): Matched: %s\n", state_names[(X)].c_str(), yytext); \
	BEGIN(X); \
	return (Y); \
} while (0)


#define YY_NO_INPUT

#define STATE_TABLE_ENT(X) {X, #X }
extern unordered_map<int, string> state_names;

struct cb_struct {
	const char *fullpath;
	const char *filename;
};

static int include_dir_cb(int dirfd unused, const char *name, struct stat *st,
			  void *data)
{
	struct cb_struct *d = (struct cb_struct *) data;

	autofree char *path = NULL;

	if (asprintf(&path, "%s/%s", d->fullpath, name) < 0)
		yyerror("Out of memory");

	if (is_blacklisted(name, path))
		return 0;

	if (g_includecache->find(path)) {
		PDEBUG("skipping reinclude of \'%s\' in \'%s\'\n", path,
			d->filename);
		return 0;
	}

	/* Handle symlink here. See _aa_dirat_for_each in private.c */

	if (S_ISREG(st->st_mode)) {
		if (!(yyin = fopen(path,"r")))
			yyerror(_("Could not open '%s' in '%s'"), path, d->filename);
		PDEBUG("Opened include \"%s\" in \"%s\"\n", path, d->filename);
		(void) g_includecache->insert(path);
		update_mru_tstamp(yyin, path);
		push_include_stack(path);
		yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));
	}

	return 0;
}

void include_filename(char *filename, int search, bool if_exists)
{
	FILE *include_file = NULL;
	struct stat my_stat;
	autofree char *fullpath = NULL;
	bool cached;

	if (search) {
		include_file = search_path(filename, &fullpath, &cached);
		if (!include_file && cached) {
			goto skip;
		} else if (preprocess_only) {
			fprintf(yyout, "\n\n##included <%s>\n", filename);
		} else if (!include_file && preprocess_only) {
			fprintf(yyout, "\n\n##failed include <%s>\n", filename);
		}

	} else if (g_includecache->find(filename)) {
		/* duplicate entry skip */
		goto skip;
	} else {
		if (preprocess_only)
			fprintf(yyout, "\n\n##included \"%s\"\n", filename);
		fullpath = strdup(filename);
		include_file = fopen(fullpath, "r");
		if (include_file)
			/* ignore failure to insert into cache */
			(void) g_includecache->insert(filename);
	}

	if (!include_file) {
		if (if_exists)
			return;
		yyerror(_("Could not open '%s'"),
                        fullpath ? fullpath: filename);
	}

	if (fstat(fileno(include_file), &my_stat))
		yyerror(_("fstat failed for '%s'"), fullpath);

        if (S_ISREG(my_stat.st_mode)) {
		yyin = include_file;
		update_mru_tstamp(include_file, fullpath);
		PDEBUG("Opened include \"%s\"\n", fullpath);
		push_include_stack(fullpath);
		yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));
        } else if (S_ISDIR(my_stat.st_mode)) {
		struct cb_struct data = { fullpath, filename };
		update_mru_tstamp(include_file, fullpath);
		fclose(include_file);
		include_file = NULL;
		if (dirat_for_each(AT_FDCWD, fullpath, &data, include_dir_cb)) {
			yyerror(_("Could not process include directory"
				  " '%s' in '%s'"), fullpath, filename);;
		}
	}

	return;

skip:
	if (preprocess_only)
		fprintf(yyout, "\n\n##skipped duplicate include <%s>\n", filename);
	return;
}

static char *lsntrim(char *s, int l)
{
	const char *end = s + l;

	while (s <= end && isspace(*s))
		s++;
	return s;
}

static int rsntrim(const char *s, int l)
{
	const char *r = s + l;

	while (r > s && isspace(*--r))
		l--;
	return l;
}


%}

CARET		"^"
OPEN_BRACE 	\{
CLOSE_BRACE 	\}
SLASH		\/
COLON		:
AMPERSAND	&
END_OF_RULE	[,]
RANGE		-
MODE_CHARS 	([RrWwaLlMmkXx])|(([Pp]|[Cc])[Xx])|(([Pp]|[Cc])?([IiUu])[Xx])
MODES		{MODE_CHARS}+
WS		[[:blank:]]
NUMBER		[[:digit:]]+

ID_CHARS	[^ \t\r\n"!,]
ID 		{ID_CHARS}|(,{ID_CHARS}|\\[ ]|\\\t|\\\"|\\!|\\,)
IDS		{ID}+
INC_ID 		[^ \t\r\n"!,<>]|(,[^ \t\r\n"!,<>]|\\[ ]|\\\t|\\\"|\\!|\\,)
INC_IDS		{INC_ID}+
POST_VAR_ID_CHARS	[^ \t\n"!,=\+]
POST_VAR_ID 	{POST_VAR_ID_CHARS}|(,{POST_VAR_ID_CHARS}|\\[ ]|\\\t|\\\"|\\!|\\,|\\\(|\\\))
LIST_VALUE_ID_CHARS	([^ \t\n"!,()]|\\[ ]|\\\t|\\\"|\\!|\\,|\\\(|\\\))
LIST_VALUE_QUOTED_ID_CHARS [^\0"]|\\\"
LIST_VALUE_ID	{LIST_VALUE_ID_CHARS}+
QUOTED_LIST_VALUE_ID	\"{LIST_VALUE_QUOTED_ID_CHARS}+\"
ID_CHARS_NOEQ	[^ \t\n"!,=)]
LEADING_ID_CHARS_NOEQ [^ \t\n"!,=()+&]
ID_NOEQ		{ID_CHARS_NOEQ}|(,{ID_CHARS_NOEQ})
IDS_NOEQ	{LEADING_ID_CHARS_NOEQ}{ID_NOEQ}*
ALLOWED_QUOTED_ID 	[^\0"]|\\\"
QUOTED_ID	\"{ALLOWED_QUOTED_ID}*\"

IP		{NUMBER}\.{NUMBER}\.{NUMBER}\.{NUMBER}

HAT		hat{WS}*
PROFILE		profile{WS}*
KEYWORD         [[:alpha:]_]+
VARIABLE_NAME	[[:alpha:]][[:alnum:]_]*
SET_VAR_PREFIX  @
SET_VARIABLE	{SET_VAR_PREFIX}(\{{VARIABLE_NAME}\}|{VARIABLE_NAME})
BOOL_VARIABLE	$(\{{VARIABLE_NAME}\}|{VARIABLE_NAME})

LABEL		(\/|{SET_VARIABLE}{POST_VAR_ID}|{COLON}|{AMPERSAND}){ID}*
QUOTED_LABEL	\"(\/|{SET_VAR_PREFIX}|{COLON}|{AMPERSAND})([^\0"]|\\\")*\"

OPEN_PAREN 	\(
CLOSE_PAREN	\)
COMMA		\,
EQUALS		=
ADD_ASSIGN	\+=
ARROW		->
LT_EQUAL	<=
LT		<
GT		>

/* IF adding new state please update state_names table and default rule (just
 * above the state_names table) at the eof.
 *
 * The nodefault option is set so missing adding to the default rule isn't
 * fatal but can't take advantage of additional debug the default rule might
 * have.
 *
 * If a state is not added to the default rule it can result in the message
 * "flex scanner jammed"
 */
%x SUB_ID
%x SUB_ID_WS
%x SUB_VALUE
%x EXTCOND_MODE
%x EXTCONDLIST_MODE
%x NETWORK_MODE
%x LIST_VAL_MODE
%x LIST_COND_MODE
%x LIST_COND_VAL
%x LIST_COND_PAREN_VAL
%x ASSIGN_MODE
%x RLIMIT_MODE
%x MOUNT_MODE
%x DBUS_MODE
%x SIGNAL_MODE
%x PTRACE_MODE
%x UNIX_MODE
%x CHANGE_PROFILE_MODE
%x INCLUDE
%x INCLUDE_EXISTS
%x ABI_MODE
%x USERNS_MODE

%%

%{
/* Copied directly into yylex function */
	if (parser_token) {
		int t = parser_token;
		parser_token = 0;
		return t;
	}
%}

<INITIAL,SUB_ID_WS,INCLUDE,INCLUDE_EXISTS,LIST_VAL_MODE,EXTCOND_MODE,LIST_COND_VAL,LIST_COND_PAREN_VAL,LIST_COND_MODE,EXTCONDLIST_MODE,ASSIGN_MODE,NETWORK_MODE,CHANGE_PROFILE_MODE,RLIMIT_MODE,MOUNT_MODE,DBUS_MODE,SIGNAL_MODE,PTRACE_MODE,UNIX_MODE,ABI_MODE,USERNS_MODE>{
	{WS}+	{  DUMP_PREPROCESS; /* Ignoring whitespace */ }
}

<INCLUDE,INCLUDE_EXISTS,ABI_MODE>{
	(\<((([^"\>\t\r\n])+)|{QUOTED_ID})\>|{QUOTED_ID}|({INC_IDS}))	{ /* <filename> | <"filename"> | "filename" | filename */
		int lt = *yytext == '<'  ? 1 : 0;
		int len = yyleng - lt*2;
		char *s = yytext + lt;
		char * filename = lsntrim(s, yyleng);
		bool exists = YYSTATE == INCLUDE_EXISTS;

		filename = processid(filename, rsntrim(filename, len - (filename - s)));
		if (!filename)
			yyerror(_("Failed to process filename\n"));
		if (YYSTATE == ABI_MODE) {
			yylval.id = filename;
			if (lt)
				RETURN_TOKEN(TOK_ID);
			else
				RETURN_TOKEN(TOK_VALUE);
		}
		include_filename(filename, lt, exists);
		free(filename);
		POP_NODUMP();
	}
}

<<EOF>> {
	fclose(yyin);
	pop_include_stack();
	yypop_buffer_state();
	if ( !YY_CURRENT_BUFFER )
		yyterminate();
}

<INITIAL,MOUNT_MODE,DBUS_MODE,SIGNAL_MODE,PTRACE_MODE,UNIX_MODE>{
	(peer|xattrs)/{WS}*={WS}*\(	{
		/* we match to the = in the lexer so that we can switch scanner
		 * state.  By the time the parser see the = it may be too late
		 * as bison may have requested the next token from the scanner
		 */
		yylval.id = processid(yytext, yyleng);
		PUSH_AND_RETURN(EXTCONDLIST_MODE, TOK_CONDLISTID);
	}
	{VARIABLE_NAME}/{WS}*=	{
		/* we match to the = in the lexer so that we can switch scanner
		 * state.  By the time the parser see the = it may be too late
		 * as bison may have requested the next token from the scanner
		 */
		yylval.id = processid(yytext, yyleng);
		PUSH_AND_RETURN(EXTCOND_MODE, TOK_CONDID);
	}
	{VARIABLE_NAME}/{WS}+in{WS}*\(	{
		/* we match to 'in' in the lexer so that we can switch scanner
		 * state.  By the time the parser see the 'in' it may be to
		 * late as bison may have requested the next token from the
		 * scanner
		 */
		yylval.id = processid(yytext, yyleng);
		PUSH_AND_RETURN(EXTCOND_MODE, TOK_CONDID);
	}
}

<SUB_ID,SUB_ID_WS>{
	({IDS}|{QUOTED_ID}) {
		/* Go into separate state to match generic ID strings */
		yylval.id =  processid(yytext, yyleng);
		POP_AND_RETURN(TOK_ID);
	}
}

<SUB_VALUE>{
	({IDS}|{QUOTED_ID}) {
		/* Go into separate state to match generic VALUE strings */
		yylval.id =  processid(yytext, yyleng);
		POP_AND_RETURN(TOK_VALUE);
	}
}

<LIST_VAL_MODE>{
	{CLOSE_PAREN} { POP_AND_RETURN(TOK_CLOSEPAREN); }

	{COMMA}	{ EAT_TOKEN("listval: ,\n"); }

	({LIST_VALUE_ID}|{QUOTED_ID}) {
		yylval.id = processid(yytext, yyleng);
		RETURN_TOKEN(TOK_VALUE);
	}
}

<EXTCOND_MODE>{
	{EQUALS}{WS}*/[^(\n] { BEGIN_AND_RETURN(SUB_VALUE, TOK_EQUALS);}

	{EQUALS} { RETURN_TOKEN(TOK_EQUALS); }

	/* Don't push state here as this is a transition start condition and
	 * we want to return to the start condition that invoked <EXTCOND_MODE>
	 * when LIST_VAL_ID is done
	 */
	{OPEN_PAREN} { BEGIN_AND_RETURN(LIST_VAL_MODE, TOK_OPENPAREN); }

	in { RETURN_TOKEN(TOK_IN); }
}

<LIST_COND_VAL>{
	({LIST_VALUE_ID}|{QUOTED_LIST_VALUE_ID}) {
		yylval.id = processid(yytext, yyleng);
		POP_AND_RETURN(TOK_VALUE);
	}
}

<LIST_COND_PAREN_VAL>{
	{CLOSE_PAREN}	{ POP(); }

	({LIST_VALUE_ID}|{QUOTED_LIST_VALUE_ID}) {
		yylval.id = processid(yytext, yyleng);
		RETURN_TOKEN(TOK_VALUE);
	}
}

<LIST_COND_MODE>{
	{CLOSE_PAREN}	{ POP_AND_RETURN(TOK_CLOSEPAREN); }

	{COMMA}		{ EAT_TOKEN("listcond: , \n"); }

	{ID_CHARS_NOEQ}+	{
		yylval.id = processid(yytext, yyleng);
		RETURN_TOKEN(TOK_CONDID);
	}

	{EQUALS}{WS}*{OPEN_PAREN}	{
		PUSH_AND_RETURN(LIST_COND_PAREN_VAL, TOK_EQUALS);
	}

	{EQUALS}	{
		PUSH_AND_RETURN(LIST_COND_VAL, TOK_EQUALS);
	}
}

<EXTCONDLIST_MODE>{
	{EQUALS}	{ RETURN_TOKEN(TOK_EQUALS); }

	{OPEN_PAREN}	{
		/* Don't push state here as this is a transition
		 * start condition and we want to return to the start
		 * condition that invoked <EXTCONDLIST_MODE> when
		 * LIST_VAL_ID is done
		 */
		BEGIN_AND_RETURN(LIST_COND_MODE, TOK_OPENPAREN);
	}
}

<ASSIGN_MODE>{
	({IDS}|{QUOTED_ID}) {
		yylval.var_val = processid(yytext, yyleng);
		RETURN_TOKEN(TOK_VALUE);
	}

	{END_OF_RULE} {
		yylval.id = strdup(yytext);
		DUMP_PREPROCESS;
		yyerror(_("Variable declarations do not accept trailing commas"));
	}

	\\\n	{ DUMP_PREPROCESS; current_lineno++ ; }

	\r?\n	{
		/* don't use shared rule because we need POP() here */
		DUMP_PREPROCESS;
		current_lineno++;
		POP();
	}
}

<NETWORK_MODE>{
	{IDS} {
		yylval.id = strdup(yytext);
		RETURN_TOKEN(TOK_ID);
	}
}

<CHANGE_PROFILE_MODE>{
	safe		{ RETURN_TOKEN(TOK_SAFE); }
	unsafe		{ RETURN_TOKEN(TOK_UNSAFE); }

	{ARROW} {
		/**
		 * Push state so that we can return TOK_ID even when the
		 * change_profile target is 'safe' or 'unsafe'.
		 */
		PUSH_AND_RETURN(SUB_ID_WS, TOK_ARROW);
	}

	({IDS}|{QUOTED_ID}) {
		yylval.id = processid(yytext, yyleng);
		RETURN_TOKEN(TOK_ID);
	}
}

<RLIMIT_MODE>{
	-?{NUMBER} {
	        yylval.var_val = strdup(yytext);
		RETURN_TOKEN(TOK_VALUE);
	}

	{KEYWORD}	{
	        yylval.id = strdup(yytext);
		if (strcmp(yytext, "infinity") == 0)
			RETURN_TOKEN(TOK_VALUE);
		RETURN_TOKEN(TOK_ID);
	}

	{LT_EQUAL}	{ RETURN_TOKEN(TOK_LE); }
}

<UNIX_MODE>{
	listen	{ RETURN_TOKEN(TOK_LISTEN); }
	accept	{ RETURN_TOKEN(TOK_ACCEPT); }
	connect	{ RETURN_TOKEN(TOK_CONNECT); }
	getattr	{ RETURN_TOKEN(TOK_GETATTR); }
	setattr	{ RETURN_TOKEN(TOK_SETATTR); }
	getopt	{ RETURN_TOKEN(TOK_GETOPT); }
	setopt	{ RETURN_TOKEN(TOK_SETOPT); }
	shutdown	{ RETURN_TOKEN(TOK_SHUTDOWN); }
}

<UNIX_MODE,USERNS_MODE>{
	create	{ RETURN_TOKEN(TOK_CREATE); }
}

<DBUS_MODE,UNIX_MODE>{
	bind		{ RETURN_TOKEN(TOK_BIND); }
}

<DBUS_MODE>{
	eavesdrop	{ RETURN_TOKEN(TOK_EAVESDROP); }
}

<DBUS_MODE,SIGNAL_MODE,UNIX_MODE>{
	send		{ RETURN_TOKEN(TOK_SEND); }
	receive		{ RETURN_TOKEN(TOK_RECEIVE); }
}

<PTRACE_MODE>{
	trace		{ RETURN_TOKEN(TOK_TRACE); }
	readby		{ RETURN_TOKEN(TOK_READBY); }
	tracedby	{ RETURN_TOKEN(TOK_TRACEDBY); }
}

<DBUS_MODE,SIGNAL_MODE,PTRACE_MODE,UNIX_MODE>{
	read		{ RETURN_TOKEN(TOK_READ); }
	write		{ RETURN_TOKEN(TOK_WRITE); }
	{OPEN_PAREN}	{
			PUSH_AND_RETURN(LIST_VAL_MODE, TOK_OPENPAREN);
			}
	(r|w|rw|wr)/([[:space:],])	{
			yylval.mode = strdup(yytext);
			RETURN_TOKEN(TOK_MODE);
			}
}

<MOUNT_MODE>{
	{ARROW}		{ RETURN_TOKEN(TOK_ARROW); }
}

<MOUNT_MODE,DBUS_MODE,SIGNAL_MODE,PTRACE_MODE,UNIX_MODE>{
	({IDS_NOEQ}|{LABEL}|{QUOTED_ID}) {
		yylval.id = processid(yytext, yyleng);
		RETURN_TOKEN(TOK_ID);
	}
}

#include{WS}+if{WS}+exists/{WS}.*\r?\n	{
	/* Don't use PUSH() macro here as we don't want #include echoed out.
	 * It needs to be handled specially
	 */
	pwarn(WARN_INCLUDE, _("deprecated use of '#include'\n"));
	yy_push_state(INCLUDE_EXISTS);
}

include{WS}+if{WS}+exists/{WS}	{
	/* Don't use PUSH() macro here as we don't want #include echoed out.
	 * It needs to be handled specially
	 */
	yy_push_state(INCLUDE_EXISTS);
}

#include/.*\r?\n	{
	/* Don't use PUSH() macro here as we don't want #include echoed out.
	 * It needs to be handled specially
	 */
	pwarn(WARN_INCLUDE, _("deprecated use of '#include'\n"));
	yy_push_state(INCLUDE);
}

include/{WS}	{
	/* Don't use PUSH() macro here as we don't want #include echoed out.
	 * It needs to be handled specially
	 */
	yy_push_state(INCLUDE);
}

#.*\r?\n	{ /* normal comment */
	DUMP_AND_DEBUG("comment(%d): %s\n", current_lineno, yytext);
	current_lineno++;
}

{CARET}		{ PUSH_AND_RETURN(SUB_ID, TOK_CARET); }

{ARROW}		{ PUSH_AND_RETURN(SUB_ID_WS, TOK_ARROW); }

{EQUALS}	{ PUSH_AND_RETURN(ASSIGN_MODE, TOK_EQUALS); }

{ADD_ASSIGN}	{ PUSH_AND_RETURN(ASSIGN_MODE, TOK_ADD_ASSIGN); }

{SET_VARIABLE}	{
	yylval.set_var = strdup(yytext);
	RETURN_TOKEN(TOK_SET_VAR);
}

{BOOL_VARIABLE}	{
	yylval.bool_var = strdup(yytext);
	RETURN_TOKEN(TOK_BOOL_VAR);
}

{OPEN_BRACE}	{ RETURN_TOKEN(TOK_OPEN); }

{CLOSE_BRACE}	{ RETURN_TOKEN(TOK_CLOSE); }

({LABEL}|{QUOTED_LABEL}) {
	yylval.id = processid(yytext, yyleng);
	RETURN_TOKEN(TOK_ID);
}

({MODES})/([[:space:],]) {
	yylval.mode = strdup(yytext);
	RETURN_TOKEN(TOK_MODE);
}

{HAT}		{ PUSH_AND_RETURN(SUB_ID, TOK_HAT); }

{PROFILE}	{ PUSH_AND_RETURN(SUB_ID, TOK_PROFILE); }

{COLON}		{ RETURN_TOKEN(TOK_COLON); }

{OPEN_PAREN}	{ PUSH_AND_RETURN(LIST_VAL_MODE, TOK_OPENPAREN); }

{VARIABLE_NAME}	{
	int token = get_keyword_token(yytext);
	int state = INITIAL;

	/* special cases */
	switch (token) {
	case -1:
		/* no token found */
		yylval.id = processunquoted(yytext, yyleng);
		RETURN_TOKEN(TOK_ID);
		break;
	case TOK_RLIMIT:
		state = RLIMIT_MODE;
		break;
	case TOK_NETWORK:
		state = NETWORK_MODE;
		break;
	case TOK_CHANGE_PROFILE:
		state = CHANGE_PROFILE_MODE;
		break;
	case TOK_MOUNT:
	case TOK_REMOUNT:
	case TOK_UMOUNT:
		state = MOUNT_MODE;
		break;
	case TOK_DBUS:
		state = DBUS_MODE;
		break;
	case TOK_SIGNAL:
		state = SIGNAL_MODE;
		break;
	case TOK_PTRACE:
		state = PTRACE_MODE;
		break;
	case TOK_UNIX:
		state = UNIX_MODE;
		break;
	case TOK_ABI:
		state = ABI_MODE;
		break;
	case TOK_USERNS:
		state = USERNS_MODE;
		break;
	default: /* nothing */
		break;
	}
	PUSH_AND_RETURN(state, token);
}

<INITIAL,NETWORK_MODE,RLIMIT_MODE,CHANGE_PROFILE_MODE,MOUNT_MODE,DBUS_MODE,SIGNAL_MODE,PTRACE_MODE,UNIX_MODE,ABI_MODE,USERNS_MODE>{
	{END_OF_RULE}	{
		if (YY_START != INITIAL)
			POP_NODUMP();
		RETURN_TOKEN(TOK_END_OF_RULE);
	}
}

<INITIAL,SUB_ID_WS,INCLUDE,INCLUDE_EXISTS,LIST_VAL_MODE,EXTCOND_MODE,LIST_COND_VAL,LIST_COND_PAREN_VAL,LIST_COND_MODE,EXTCONDLIST_MODE,NETWORK_MODE,CHANGE_PROFILE_MODE,RLIMIT_MODE,MOUNT_MODE,DBUS_MODE,SIGNAL_MODE,PTRACE_MODE,UNIX_MODE,ABI_MODE,USERNS_MODE>{
	\r?\n	{
		DUMP_PREPROCESS;
		current_lineno++;
	}
}

<INITIAL,SUB_ID,SUB_ID_WS,SUB_VALUE,LIST_VAL_MODE,EXTCOND_MODE,LIST_COND_VAL,LIST_COND_PAREN_VAL,LIST_COND_MODE,EXTCONDLIST_MODE,ASSIGN_MODE,NETWORK_MODE,CHANGE_PROFILE_MODE,MOUNT_MODE,DBUS_MODE,SIGNAL_MODE,PTRACE_MODE,UNIX_MODE,RLIMIT_MODE,INCLUDE,INCLUDE_EXISTS,ABI_MODE,USERNS_MODE>{
	(.|\n)	{
		DUMP_PREPROCESS;
		/* Something we didn't expect */
		yyerror(_("Lexer found unexpected character: '%s' (0x%x) in state: %s"), yytext, yytext[0], state_names[YY_START].c_str());
	}
}
%%

/* Create a table mapping lexer state number to the name used in the
 * in the code.  This allows for better debug output
 */
unordered_map<int, string> state_names = {
	STATE_TABLE_ENT(INITIAL),
	STATE_TABLE_ENT(SUB_ID),
	STATE_TABLE_ENT(SUB_ID_WS),
	STATE_TABLE_ENT(SUB_VALUE),
	STATE_TABLE_ENT(EXTCOND_MODE),
	STATE_TABLE_ENT(EXTCONDLIST_MODE),
	STATE_TABLE_ENT(NETWORK_MODE),
	STATE_TABLE_ENT(LIST_VAL_MODE),
	STATE_TABLE_ENT(LIST_COND_MODE),
	STATE_TABLE_ENT(LIST_COND_VAL),
	STATE_TABLE_ENT(LIST_COND_PAREN_VAL),
	STATE_TABLE_ENT(ASSIGN_MODE),
	STATE_TABLE_ENT(RLIMIT_MODE),
	STATE_TABLE_ENT(MOUNT_MODE),
	STATE_TABLE_ENT(DBUS_MODE),
	STATE_TABLE_ENT(SIGNAL_MODE),
	STATE_TABLE_ENT(PTRACE_MODE),
	STATE_TABLE_ENT(UNIX_MODE),
	STATE_TABLE_ENT(CHANGE_PROFILE_MODE),
	STATE_TABLE_ENT(INCLUDE),
	STATE_TABLE_ENT(INCLUDE_EXISTS),
	STATE_TABLE_ENT(ABI_MODE),
	STATE_TABLE_ENT(USERNS_MODE),
};
