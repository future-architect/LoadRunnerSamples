#ifndef _GLOBALS_H
#define _GLOBALS_H
/**
 * Header file for all.
 *
 * @author kulikala
/*/

//--------------------------------------------------------------------
// Global Variables
#define CONFIG_ENV         "config/env.cfg"
#define FOUND              0
#define MAX_HTML_PARAM_LEN "10240"
#define MAX_THINK_TIME     30.0
#define SSL_VERSION        "2&3"

double g_dThinkTime;

//--------------------------------------------------------------------
// Structs and types
#define bool   _Bool
#define _Bool  int
#define true   1
#define false  0

//--------------------------------------------------------------------
// Include Files
#include "lrun.h"
#include "web_api.h"

#include "fa_read_config.c"
#include "fa_trim.c"

//--------------------------------------------------------------------
// Function definitions

#endif // _GLOBALS_H
