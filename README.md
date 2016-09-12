# LoadRunnerSamples

Following scripts are for developers who are struggling with HPE LoadRunner and its editor VuGen.
These scripts include examples and useful functions written in C.

Feel free to take advantage of them and write your own scripts.


## Scripts

* **ReadConfigFileToParameter**  
  This sample includes and uses `fa_read_config()` function that reads config files and sets parameters.  
  
  The **_parameter_** concept in VuGen is factually useful when you manually create the parameter in VuGen and specify as it is _Table_ type because VuGen automatically updates the data row on every iteration.
  But LoadRunner does not have handy functions reading parameters specified in config files for script writers.  
  The function `fa_read_config()` reads config files and sets parameters at runtime.
  This is helpful when you change the base url of the target application; such as staging.app.sample.com or development.app.sample.com .
  You'll want to specify the base url in a config file and read the file from your script.

* **HandlingJSONWithParson**  
  This sample uses [future-architect/parson](https://github.com/future-architect/parson) a lightweight [JSON](http://json.org) library written in C.  
  parson is forked from [kgabis/parson](https://github.com/kgabis/parson).


## License

MIT
