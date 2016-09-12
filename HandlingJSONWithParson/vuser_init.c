/**
 * Called from generator when a virtual user is initialized.
 *
 * @author kulikala
 * @return       LR_PASS or LR_FAIL.
/*/
vuser_init()
{
	int rc;
	char *szJson;
	JSON_Value *lpRootValue;
	char *szJsonType;
	JSON_Object *lpRootObject;

	rc = LR_PASS;

	web_set_max_html_param_len(MAX_HTML_PARAM_LEN);

	szJson = "{"
			"\"property\":\"string\","
			"\"list\":["
				"{"
					"\"index\":0,"
					"\"data\":{"
						"\"prop\":\"value\","
						"\"flag\":true"
					"}"
				"},"
				"{"
					"\"index\":1,"
					"\"data\":{"
						"\"prop\":null,"
						"\"flag\":true"
					"}"
				"},"
				"{"
					"\"index\":2,"
					"\"data\":{"
						"\"prop\":123,"
						"\"flag\":false"
					"}"
				"}"
			"],"
			"\"deeper\":{"
				"\"object\":{"
					"\"prop\":\"value\""
				"}"
			"},"
			"\"empty\":{"
				"\"list\":[],"
				"\"object\":{}"
			"}"
		"}";

	// JSON string to parse
	lr_output_message("JSON to parse: %s", szJson);

	// Parse JSON string
	lpRootValue = json_parse_string(szJson);

	// Check the type of Root value
	switch (json_type(lpRootValue)) {
		case JSONError:
			szJsonType = "Error";
			break;

		case JSONNull:
			szJsonType = "Null";
			break;

		case JSONString:
			szJsonType = "String";
			break;

		case JSONNumber:
			szJsonType = "Number";
			break;

		case JSONObject:
			szJsonType = "Object";
			break;

		case JSONArray:
			szJsonType = "Array";
			break;

		case JSONBoolean:
			szJsonType = "Boolean";
			break;

		default:
			szJsonType = "undefined";
			break;
	}

	lr_output_message("Root type is: %s", szJsonType);

	if (JSONObject == json_type(lpRootValue)) {
		lr_output_message("Query: \"%s\", Result: \"%s\"", ".list[0].data.prop", json_value_query_string(lpRootValue, ".list[0].data.prop"));
		lr_output_message("Query: \"%s\", Result: \"%d\"", ".list[2].data.prop", (int)json_value_query_number(lpRootValue, ".list[2].data.prop"));
	}

	// Cleanup memory
	json_value_free(lpRootValue);

	return rc;
}
