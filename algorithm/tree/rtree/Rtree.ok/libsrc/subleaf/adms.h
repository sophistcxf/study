#ifndef	_INCLUDED_ADMS_H_TWICE
#define	_INCLUDED_ADMS_H_TWICE	"OK"
#else
#define	_INCLUDED_ADMS_H_TWICE	"TROUBLE"
	something has included adms.h twice. fix it now.
#endif

#ifndef	_INCL_ADMS_H


#ifndef	TRUE
#define	TRUE		1
#endif
#ifndef	FALSE
#define	FALSE		0
#endif

#define	BLOCKSIZE	1024

#define MAXUCMDLEN	1024		/* max length of Unix cmd */
#define MAXQCMDLEN	1024 		/* max length of user's query cmd */

#define MAXBLOCK 	5000		/* max block size */

#define USRNAMLEN	16		/* max length of user name */
#define MAXRELNAMELEN	40		/* max length of relation names */
#define MAXNUMDROPS	30		/* max #relations dropped together */
#define DFLDISSIZ	10		/* default display size (#lines) */
#define DFLDISWINSIZ	80		/* default display window size */
#define MAXTPLSIZE	512		/* max (byte) length of a tuple */

#define MAXATT		80		/* max #of attributes */
#define MAXSIZE		512
#define	TABLENAME_LEN	32

	/* Define parameters for ADMS parser */
#define MAXLINE	512
#define MAXARGS 128
#define MAXCMD	16
#define MAXATR	64

	/* String Constants & Delimiters */
#define	ALLMATCH_PAT	'*'
#define	PATHNAME_SEP	'/'
#define	TBLNAME_SEP	'.'

	/* definitions for constants used in the plus_minus system */
#define ADMS_HOME	".adms"		/* name of 'home' file in home dir */
#define MINUS_HOME	"minus_home"
#define PLUS_HOME	"plus_home"

#define MAXNUMSVC	32	/* max number of shared dbs in active list */
#define MAX_REL_NUM	16	/* max number of relations */
#define MAXOPTSLEN	5	/* max length (# of digits) of op_ts */
#define MAXBNAMELEN	4	/* max length (# of digits) of bnames */
#define MAXDBNAMELEN	7	/* max length of database names */
#define MAXHOSTLEN	32	/* max length of machine names */
#define MAXSBRLEN	50	/* max length of SBR names */
#define MAXATTLEN	12	/* max length of attribute names */
#define MAXRELNAME	64	/* max length of table names */
#define MAXCONDLEN	512	/* max length of condition strings */
#define MAXTPLLEN	1024	/* max length of tuple (in chars) */
#define MAXNTUPLES	128	/* max number of tuples in a base table */
#define MAXNAMELEN	32	/* max length of "generic" name */

#define RET_OK		1	/* Server: all right */
#define RET_NOT_EXIST	2	/* Server: relation does not exist */
#define RET_ERROR	3	/* Server: error occured */
#define RET_EXIST	4	/* Server: relation already exist */
#define RET_DRV_NOT_0	5	/* Server: cannot drop SBR (drv != 0) */
#define RPC_ERROR	6	/* Remote Procedure Call error */
#define LOCAL_ERROR	7	/* local (workstation) error */
#define UNIX_ERROR	8	/* local (workstation) Unix error */
#define BUG		9	/* (?) unexpected error */
#define RET_UPD		10	/* Server: update needed */
#define NO_REQ		11	/* Server: No request file found */

#define TYPENJOIN	1	/* Njoin binding */
#define TYPETJOIN	2	/* Tjoin binding */
#define TYPEHJOIN	3	/* Hjoin binding */

#define IS_VERS_NUM	1	/* version number */

#define IS_CRE_SBR_NUM	1	/* create an SBR */
#define IS_DRO_SBR_NUM	2	/* drop an SBR */
#define IS_INS_SBR_NUM	3	/* insert tuples in an SBR */
#define IS_DEL_SBR_NUM	4	/* delete tuples from an SBR */
#define STOP_SERVER_NUM	5	/* stop the server */

#define IS_CRE_SEL_NUM	6	/* create a selector binding */
#define IS_CRE_JOI_NUM	7	/* create a join binding */
#define IS_CRE_SJO_NUM	8	/* create a semi-join binding */

#define IS_UPD_SEL_NUM	9	/* update a selector binding */
#define IS_UPD_JOI_NUM	10	/* update a join binding */
#define IS_UPD_SJO_NUM	11	/* update a semi-join binding */

#define IS_DRO_SEL_NUM	12	/* drop  a selector binding */
#define IS_DRO_JOI_NUM	13	/* drop  a join binding */
#define IS_DRO_SJO_NUM	14	/* drop  a semi-join binding */
#define IS_UPD_CAT_NUM	15	/* update catalog of workstation */

#define IS_CRE_IND_NUM	16	/* create an index */
#define IS_RMV_IND_NUM	17	/* remove an index */

#define IS_ALT_SBR_NUM	18	/* alter tuples in an SBR */

#define IS_DO_UUPROJ_NUM 	19
#define IS_GET_BIND_NAME 	20
#define UPDSJO1			21
#define UPDSJO2			22
#define UPDSJO3			23

extern	char	*strcat();

/* MJR's allocation test & trace include
 * #include "mnemosyne.h"
 */

#define	_INCL_ADMS_H
#endif
