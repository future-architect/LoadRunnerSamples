// The url to access in this script
#define URL "https://api.github.com/search/repositories?q=stars%3A%3E10000&sort=updated&per_page=10"

/**
 * Called from generator by each iteration.
 *
 * @author kulikala
 * @return       LR_PASS or LR_FAIL.
/*/
Action()
{
	int rc;
	char *szBody;
	JSON_Value *lpRootValue, *lpItem;
	JSON_Array *lpArray;
	size_t i;

	rc = LR_PASS;

	// Save string as a parameter
	lr_save_string(URL, "url");

	// Capture all response body
	web_reg_save_param_ex(
		"ParamName=body",
		SEARCH_FILTERS,
		"ContentType=application/json*",
		"Scope=Body",
		LAST);

	// Do request API
	web_custom_request("Search GitHub",
		"URL={url}",
		"Method=GET",
		"RecContentType=application/json; charset=utf-8",
		"Resource=0",
		LAST);

	// Extract captured body string
	szBody = lr_eval_string("{body}");
	lr_output_message("Body: %d bytes", strlen(szBody));

	// Parse JSON body
	lpRootValue = json_parse_string(szBody);

	if (NULL != lpRootValue) {
		// Get items
		lpArray = json_value_query_array(lpRootValue, ".items[]");

		for (i = 0; NULL != lpArray && i < json_array_get_count(lpArray); i++) {
			lpItem = json_array_get_value(lpArray, i);

			if (NULL == lpItem) {
				continue;
			}

			lr_output_message(
				"Name: %-40s Owner: %-20s Stars: %7d Updated: %20s",
				json_value_query_string(lpItem, ".full_name"),
				json_value_query_string(lpItem, ".owner.login"),
				(int)json_value_query_number(lpItem, ".stargazers_count"),
				json_value_query_string(lpItem, ".updated_at")
				);
		}

		// Cleanup memory
		json_value_free(lpRootValue);
	}

	return rc;
}
