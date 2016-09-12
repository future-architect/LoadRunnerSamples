# HandlingJSONWithParson

This sample uses [future-architect/parson](https://github.com/future-architect/parson) a lightweight [JSON](http://json.org) library written in C.  
parson is forked from [kgabis/parson](https://github.com/kgabis/parson).

HPE LoadRunner 12.50 above has JSON support `web_reg_save_param_json`, 
but you may need to run your script on LoadRunner 12.00 or below.  
(That happened to me - Load Generator servers are formerly built and still in use, 
and that servers are what I required to use.)


## To use this script 

 1. Install LoadRunner.
 2. Make a folder.
 3. Clone this repo within that folder.
 4. Open this script within VuGen LoadRunner.
 5. Execute the script.
 6. Make your own script ;-)


## Files in this sample

* **Action.c**  
  is where you'll place your main loop functions.

* **globals.h**  
  contains global variable definitions and includes additional files.

* **parson.c**  
  is the main parson library.

* **parson.h**  
  is the header file of parson.

* **vuser_init.c**  
  inclues sample initialization scripts.
