# mod_mptcp
A module making the Apache2 Webserver aware of MPTCP.

This module was tested and developed with the MPTCP Linux Kernel Implementation by UCLouvain. If the TCP connection to the server is MPTCP enabled, the APACHE Environment Variable "MPTCP_ENABLED" is set to 1. If a client is connecting using plain-old TCP the variable will be set to 0.

Please note: This module will not make the Apache Web-Server MPTCP-capable, it merely adds awareness for MPTCP by setting an environment variable, allowing you to determine whether the TCP connections over which content is being delivered is MPTCP-enabled.

## Build
The build process is quick and easy. Make sure to install the Apache2 Development Environment (apache2-dev) first, then just change to the directory you have cloned this repo to and 'make':
```
$ make
$ sudo make install
$ sudo make restart
```
Please note that the latter command will cause your web server to restart in order to load the mod_mptcp module.

## Using the Module
The module sets an Apache Environment variable, which can easily be read by many scripting languages.

Below we show an example for a simple PHP script:
```
<?php
if ($_SERVER['MPTCP_ENABLED']) {
  echo "Cool, you are using MPTCP! :D";
} else {
  echo "No, you are just connecting with plain-old TCP :(";
}

```
### Demo
You can see our module in action here: http://mptcp.info/

## FAQ

Can this be extended to show additional information on the MPTCP connection?

Yes, we in theory could do this. However, due to the inner workings of the underlying Apache2 module we are passed the connection handle before any data is exchanged and multiple paths could be formed. The actions in the module are blocking, meaning that no data is exchanged until the method returns or exits. Therefore such information will not be available through the module.

