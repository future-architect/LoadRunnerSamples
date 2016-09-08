# ReadConfigFileToParameter

This sample includes and uses `fa_read_config()` function that reads config files and sets parameters.

The **_parameter_** concept in VuGen is factually useful when you manually create the parameter in VuGen and specify as it is _Table_ type because VuGen automatically updates the data row on every iteration.
But LoadRunner does not have handy functions reading parameters specified in config files for script writers.  
The function `fa_read_config` reads config files and sets parameters at runtime.
This is helpful when you change the base url of the target application; such as staging.app.sample.com or development.app.sample.com .
You'll want to specify the base url in a config file and read the file from your script.


## To use this script 

 1. Install LoadRunner.
 2. Make a folder.
 3. Clone this repo within that folder.
 4. Open this script within VuGen LoadRunner.
 5. Execute the script.
 6. Make your own script ;-)


## Files in this sample

* **config/env.cfg**  
  is the config file initially read when `vuser_init()` is called.  
  See how config file should be formatted.

* **Action.c**  
  is where you'll place your main loop functions.

* **fa_read_config.c**  
  includes the main function `fa_read_config()`.
  ```c
  /**
   * Read config file and parse the content.
   * Parsed data will directly set into parameters.
   *
   * @param  szFileName The file name to be read.
   * @return            LR_PASS or LR_FAIL.
  /*/
  int fa_read_config(const char *szFileName);
  ```

* **fa_trim.c**  
  includes the utility function `fa_trim()`.
  ```c
  /**
   * Trim leading and trailing whitespace form a null terminated string.
   *
   * @author kulikala
   * @param  s     The null terminated string.
   * @return       LR_PASS or LR_FAIL.
  /*/
  int fa_trim(char *s);
  ```

* **globals.h**  
  contains global variable definitions and includes additional files.

* **vuser_init.c**  
  inclues sample initialization scripts.
