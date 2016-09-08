/**
 * Called from generator when a virtual user is initialized.
 * Reads config file.
 *
 * @author kulikala
 * @return       LR_PASS or LR_FAIL.
/*/
vuser_init()
{
	int rc;
	char *szFileName;
	char *think_time;
	char *url;
	char *proxy;
	char *proxy_pac;
	char *proxy_user;
	char *proxy_user_pw;
	char *ssl_version;

	rc = LR_PASS;

	lr_output_message("Reading config file: \"%s\"", CONFIG_ENV);

	// Read config file
	rc = fa_read_config(CONFIG_ENV);

	if (LR_PASS != rc) {
		lr_output_message("Failed to read file: \"%s\"", CONFIG_ENV);

		lr_exit(LR_EXIT_VUSER, rc);
		return rc;
	}

	// Get think_time parameter
	think_time = lr_eval_string("{think_time}");
	g_dThinkTime = atoi(think_time) / 1000.0;

	if (g_dThinkTime < 0) {
		lr_output_message("Think time should be more than 0 but: %f", g_dThinkTime);

		lr_exit(LR_EXIT_VUSER, rc);
		return LR_FAIL;
	}
	if (MAX_THINK_TIME < g_dThinkTime) {
		lr_output_message("Think time should be less than %f but: %f", MAX_THINK_TIME, g_dThinkTime);

		lr_exit(LR_EXIT_VUSER, rc);
		return LR_FAIL;
	}

	// Get url parameter
	url = lr_eval_string("{url}");

	lr_save_param_regexp(
		url,
		strlen(url),
		"RegExp=^(https?://[^/]+)",
		"Ordinal=1",
		"ResultParam=url_host",
		LAST);

	lr_output_message("Base url: \"%s\"", url);
	lr_output_message("URL host: \"%s\"", lr_eval_string("{url_host}"));

	// Get proxy parameter
	proxy = lr_eval_string("{proxy}");

	if (0 < strlen(proxy)) {
		// Set web proxy option
		web_set_proxy(proxy);
		web_set_secure_proxy(proxy);
	}

	// Get proxy_pac parameter
	proxy_pac = lr_eval_string("{proxy_pac}");

	if (0 < strlen(proxy_pac)) {
		// Set web proxy pac option
		web_set_pac(proxy_pac);
	}

	// Get proxy_user, proxy_user_pw parameter
	proxy_user = lr_eval_string("{proxy_user}");
	proxy_user_pw = lr_eval_string("{proxy_user_pw}");

	if (0 < strlen(proxy_user) && 0 < strlen(proxy)) {
		// Set web user option
		web_set_user(proxy_user, proxy_user_pw, proxy);
	}

	// Get ssl_version parameter
	ssl_version = lr_eval_string("{ssl_version}");

	if (0 < strlen(ssl_version)) {
		// Set SSL_VERSION option
		web_set_sockets_option("SSL_VERSION", ssl_version);
	} else {
		// Set default
		web_set_sockets_option("SSL_VERSION", SSL_VERSION);
	}

	web_set_max_html_param_len(MAX_HTML_PARAM_LEN);

	lr_output_message("Starting login request");

	lr_start_transaction("Login");

	// Login procedure will be here
	rc = LR_PASS;

	lr_end_transaction("Login", rc);

	if (LR_PASS != rc) {
		lr_output_message("Failed login request");

		lr_exit(LR_EXIT_VUSER, rc);
		return rc;
	}

	lr_output_message("Finished login request");

	lr_think_time(g_dThinkTime);

	return rc;
}
