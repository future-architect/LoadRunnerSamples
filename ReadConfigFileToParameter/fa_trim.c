/**
 * Trim leading and trailing whitespace form a null terminated string.
 *
 * @author kulikala
 * @param  s     The null terminated string.
 * @return       LR_PASS or LR_FAIL.
/*/
int fa_trim(char *s)
{
	int rc;
	int i;
	int iLen;
	char *szTmp;

	rc = LR_PASS;

	// If string is NULL,
	if (s == NULL) {
		return rc;
	}

	// Get length - s should be NULL terminated!
	iLen = strlen(s);

	// If string is empty,
	if (0 == iLen) {
		return rc;
	}

	// Start from length of the string
	i = iLen - 1;

	// Trim trailing space
	while (0 <= i && isspace(s[i])) {
		i--;
	}

	iLen = i + 1;

	if (iLen <= 0) {
		// Terminate with NULL
		s[0] = '\0';
		return rc;
	}

	// Write new NULL terminator
	s[iLen] = '\0';

	// Trim leading space
	i = 0;
	while (i < iLen && isspace(s[i])) {
		i++;
	}

	iLen -= i;

	// Allocate memory
	szTmp = (char *)malloc(iLen + 1);

	// Copy string to szTmp
	strncpy(szTmp, s + i, iLen);
	szTmp[iLen] = '\0';

	// Copy again
	strncpy(s, szTmp, iLen);
	s[iLen] = '\0';

	// Free memory
	free(szTmp);

	return rc;
}
