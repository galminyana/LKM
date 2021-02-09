## Kernel LKM
---
A la Debian

### Files
---
- `/boot/config-VERSION`: Stores the config for the actual kernel
- 

### Config
---
```bash
LKM# make olddefconfig            <== Dumps Kernel config to .config file
LKM# make nconfig                 <== ncurses setup
LKM# make config                  <== Old setup
```

### Build 
---
```bash
LKM# make                              <== Standard Build
LKM# make deb-pkg                      <== Build into a .deb
LKM# make deb-pkg LOCALVERSION=-TEXT   <== The same but appends TEXT
LKM# make clean                        <== Cleans Build
```
If using a existing `.config` file, edit the file and do:
```markup
CONFIG_SYSTEM_TRUSTED_KEYS = ""
```
or the build will fail with:
```markup
make[4]: *** No rule to make target 'debian/certs/test-signing-certs.pem', needed by 'certs/x509_certificate_list'.  Stop.
make[4]: *** Waiting for unfinished jobs....
```
### Results
---
In upper folder creates:



### Install
---


