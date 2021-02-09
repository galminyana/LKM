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
LKM# make deb-pkg                      <== Build into a .deb
LKM# make deb-pkg LOCALVERSION=-TEXT   <== The same but appends TEXT
LKM# make clean                        <== Cleans Build
```
