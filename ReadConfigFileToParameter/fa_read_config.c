#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define COMMENT_LINE          "#"
#define MAXBUF                1024
#define PARAM_DELIM           "="
#define PARAM_LB              "["
#define PARAM_RB              "]"
#define REGEXP_PASSWORD_CRYPT "RegExp/IC=^@\\[([a-f0-9]+)\\]$"
#define REGEXP_PARAM_VALID    "RegExp/IC=^([a-z][a-z0-9_]*)\\(?[a-z0-9_]*:?[^():=]*\\)?$"
#define REGEXP_COND           "RegExp/IC=^([a-z][a-z0-9_]*)\\([a-z][a-z0-9_]*:[^():=]*\\)$"
#define REGEXP_COND_PARAM     "RegExp/IC=^[a-z][a-z0-9_]*\\(([a-z][a-z0-9_]*):[^():=]*\\)$"
#define REGEXP_COND_VAL       "RegExp/IC=^[a-z][a-z0-9_]*\\([a-z][a-z0-9_]*:([^():=]*)\\)$"
#define SECTION_OUT           0
#define SECTION_INIT          1
#define SECTION_CONT          2

#ifndef FOUND
#define FOUND                 0
#endif // FOUND

/**
 * Read config file and parse the content.
 * Parsed data will directly set into parameters.
 *
 * @author kulikala
 * @param  szFileName The file name to be read.
 * @return            LR_PASS or LR_FAIL.
/*/
int fa_read_config(const char *szFileName)
{
	int rc;
	FILE *hFile;
	char sLine[MAXBUF];
	char sParam[MAXBUF];
	char sVal[MAXBUF];
	char *szDlm;
	char *szShouldBe;
	char *szFound;
	int iLen;
	int iValLen;
	int i;
	int isSection;
	void *szPtr;
	char *szSection;
	int iSectionLen;
	char *szDecryptedVal;
	char *tmp_val;
	char *tmp_param;
	char *tmp_param_cond;
	char *tmp_param_cond_val;
	char sCondParam[MAXBUF];
	char *szCondVal;
	int iParamLen;

	rc = LR_PASS;

	hFile = fopen(szFileName, "r");

	// If not found,
	if (hFile == NULL) {
		// Fail critical
		return LR_FAIL;
	}

	i = 0;
	isSection = SECTION_OUT;

	// Iterate each lines
	while (fgets(sLine, sizeof(sLine), hFile) != NULL) {
		// Trim
		fa_trim(sLine);

		iLen = strlen(sLine);

		// If empty line,
		if (0 == iLen) {
			// Just ignore
			continue;
		}

		// Find parameter section left delimiter
		szDlm = strstr(sLine, PARAM_LB);

		// If found,
		if (szDlm == sLine) {
			// Suppose parameter section
			// Find right delimiter
			szShouldBe = sLine + iLen - strlen(PARAM_RB);
			szFound = strstr(sLine + strlen(PARAM_LB), PARAM_RB);

			// If found,
			if (szShouldBe == szFound) {
				// If in the section,
				if (SECTION_CONT == isSection) {
					// szSection has value
					// Save as parameter
					lr_param_sprintf(sParam, "%s", szSection);

					lr_output_message("Config section found: parameter = %s, value length = %d", sParam, strlen(szSection));

					// Free memory
					free(szSection);
				} else if (SECTION_INIT == isSection) {
					// Save empty string as parameter
					lr_save_string("", sParam);

					lr_output_message("Config section found: parameter = %s, with empty value", sParam);
				}

				// Calc parameter length
				iLen = iLen - strlen(PARAM_LB) - strlen(PARAM_RB);

				// Copy parameter
				strncpy(sParam, sLine + strlen(PARAM_LB), iLen);
				sParam[iLen] = '\0';

				// Trim parameter
				fa_trim(sParam);

				// Set flag
				isSection = SECTION_INIT;

				continue;
			}
		}

		if (isSection) {
			if (SECTION_INIT == isSection) {
				// Allocate memory
				szSection = (char *)malloc(iLen + 1);

				if (NULL == szSection) {
					// malloc failed
					lr_output_message("Failed to allocate memory");
					fclose(hFile);

					// Fail critical
					return LR_FAIL;
				}

				// Copy string
				strncpy(szSection, sLine, iLen);
				szSection[iLen] = '\0';

				// Set flag
				isSection = SECTION_CONT;

				continue;
			}

			// Reallocate memory
			iSectionLen = strlen(szSection);
			szPtr = realloc(szSection, iSectionLen + iLen + 1);

			if (NULL == szPtr) {
				// realloc failed
				lr_output_message("Failed to reallocate memory");
				free(szSection);
				fclose(hFile);

				// Fail critical
				return LR_FAIL;
			} else {
				szSection = (char *)szPtr;
			}

			// Concatenate string
			strncpy(szSection + iSectionLen, sLine, iLen);
			szSection[iSectionLen + iLen] = '\0';

			continue;
		}

		// Not in a section
		// Find comment char
		szDlm = strstr(sLine, COMMENT_LINE);

		// If found,
		if (szDlm == sLine) {
			// Suppose comment line
			// Skip this line
			continue;
		}

		// Find delimiter char
		szDlm = strstr(sLine, PARAM_DELIM);

		// If not found,
		if (szDlm == NULL) {
			// Irregular file
			fclose(hFile);

			// If in the section,
			if (SECTION_CONT == isSection) {
				// Free memory
				free(szSection);
			}

			lr_output_message("Failed to parse line: %s", sLine);

			// Fail critical
			return LR_FAIL;
		}

		// Calc length
		iLen = szDlm - sLine;

		// Copy parameter
		strncpy(sParam, sLine, iLen);
		sParam[iLen] = '\0';

		// Trim parameter
		fa_trim(sParam);

		// Calc length
		iValLen = strlen(sLine) - iLen - strlen(PARAM_DELIM) + 1;

		// Copy value
		strncpy(sVal, szDlm + strlen(PARAM_DELIM), iValLen);
		sVal[iValLen] = '\0';

		// Trim value
		fa_trim(sVal);

		// Get length again
		iLen = strlen(sParam);
		iValLen = strlen(sVal);

		// Init parameter
		lr_save_string("", "tmp_param");

		lr_save_param_regexp(
			sParam,
			iLen,
			REGEXP_PARAM_VALID,
			"Ordinal=1",
			"ResultParam=tmp_param",
			LAST);

		// Get tmp_param parameter
		tmp_param = lr_eval_string("{tmp_param}");

		if (0 == strlen(tmp_param)) {
			// Irregular file
			fclose(hFile);

			// If in the section,
			if (SECTION_CONT == isSection) {
				// Free memory
				free(szSection);
			}

			lr_output_message("Parameter must begin with a letter and contain only alphanumeric characters and '_' but: %s", sParam);

			// Free parameter
			lr_free_parameter("tmp_param");

			// Fail critical
			return LR_FAIL;
		}

		// Init parameter
		lr_save_string("", "tmp_val");

		// Check encrypted value
		lr_save_param_regexp(
			sVal,
			iValLen,
			REGEXP_PASSWORD_CRYPT,
			"Ordinal=1",
			"ResultParam=tmp_val",
			LAST);

		// Get tmp_val parameter
		tmp_val = lr_eval_string("{tmp_val}");

		if (0 < strlen(tmp_val)) {
			// Try decrypting
			szDecryptedVal = lr_decrypt(tmp_val);

			if (NULL == szDecryptedVal) {
				lr_output_message("Found encrypted value for parameter \"%s\" but failed to decrypt; skip this parameter", sParam);

				lr_free_parameter("tmp_val");
				continue;
			} else {
				lr_output_message("Found encrypted value for parameter \"%s\"; decrypting value", sParam);

				// Override value with decrypted value
				iValLen = strlen(szDecryptedVal);
				strncpy(sVal, szDecryptedVal, iValLen);
				sVal[iValLen] = '\0';
			}
		}

		// Init parameter
		lr_save_string("", "tmp_val");
		lr_save_string("", "tmp_param");
		lr_save_string("", "tmp_param_cond");
		lr_save_string("", "tmp_param_cond_val");

		// Check conditional parameter
		lr_save_param_regexp(
			sParam,
			iLen,
			REGEXP_COND,
			"Ordinal=1",
			"ResultParam=tmp_param",
			LAST);

		lr_save_param_regexp(
			sParam,
			iLen,
			REGEXP_COND_PARAM,
			"Ordinal=1",
			"ResultParam=tmp_param_cond",
			LAST);

		lr_save_param_regexp(
			sParam,
			iLen,
			REGEXP_COND_VAL,
			"Ordinal=1",
			"ResultParam=tmp_param_cond_val",
			LAST);

		// Get tmp_param, tmp_param_cond, tmp_param_cond_val parameter
		tmp_param = lr_eval_string("{tmp_param}");
		tmp_param_cond = lr_eval_string("{tmp_param_cond}");
		tmp_param_cond_val = lr_eval_string("{tmp_param_cond_val}");

		iParamLen = strlen(tmp_param_cond);

		if (0 < strlen(tmp_param) && 0 < iParamLen) {
			lr_output_message("Found conditional parameter: parameter = %s", sParam);

			// Create expressoin
			sCondParam[0] = '{';
			strncpy(sCondParam + 1, tmp_param_cond, iParamLen);
			sCondParam[iParamLen + 1] = '}';
			sCondParam[iParamLen + 2] = '\0';

			// Get conditional parameter
			szCondVal = lr_eval_string(sCondParam);

			if (FOUND == stricmp(szCondVal, tmp_param_cond_val)) {
				lr_output_message("Condition matched: %s = %s, config read: parameter = %s, value = %s", tmp_param_cond, tmp_param_cond_val, tmp_param, sVal);

				// Save as parameter
				lr_param_sprintf(tmp_param, "%s", sVal);
			} else {
				lr_output_message("Conditional value of parameter \"%s\" is \"%s\", but actually was \"%s\". Skip parameter", tmp_param_cond, tmp_param_cond_val, szCondVal);
			}
		} else {
			lr_output_message("Config read: parameter = %s, value = %s", sParam, sVal);

			// Save as parameter
			lr_param_sprintf(sParam, "%s", sVal);
		}

		// Free parameter
		lr_free_parameter("tmp_val");
		lr_free_parameter("tmp_param");
		lr_free_parameter("tmp_param_cond");
		lr_free_parameter("tmp_param_cond_val");
	} // End while

	fclose(hFile);

	// If in the section,
	if (SECTION_CONT == isSection) {
		// szSection has value
		// Save as parameter
		lr_param_sprintf(sParam, "%s", szSection);

		lr_output_message("Config section found: parameter = %s, value length = %d", sParam, strlen(szSection));

		// Free memory
		free(szSection);
	} else if (SECTION_INIT == isSection) {
		// Save empty string as parameter
		lr_save_string("", sParam);

		lr_output_message("Config section found: sParam = %s, with empty value", sParam);
	}

	return rc;
}
