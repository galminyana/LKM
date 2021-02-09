## Kernel LKM
---
_A la Debian_ way

### Config
---
```bash
LKM# make olddefconfig            <== Dumps Kernel config to .config file
LKM# make menuconfig              <== My fav
LKM# make nconfig                 <== ncurses setup
LKM# make config                  <== Old setup
```
For the current kernel config, check files at `/boot/config-VERSION`. It stores installed kernels configs. 


### Build 
---
```bash
LKM# make                              <== Standard Build
LKM# make -j`nproc` bindeb-pkg         <== Build using all cpu
LKM# make deb-pkg                      <== Build into a .deb
LKM# make deb-pkg LOCALVERSION=-TEXT   <== The same but appends TEXT
LKM# make clean                        <== Cleans Build
```
If using an existing `.config` file, edit the file and do:
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
- `linux-image-4.19.174.deb`: Image to install
- `linux-image-4.19.174.dbg.deb`: Same as before but with debug symbols


### Install
---
```bash
LKM# dpkg -i ../linux-image-4.19.174.deb
```

### References
---
- Build a Debian Kernel Package: [https://wiki.debian.org/BuildADebianKernelPackage](https://wiki.debian.org/BuildADebianKernelPackage)
- Common Kernel Related Tasks: [https://kernel-team.pages.debian.net/kernel-handbook/ch-common-tasks.html](https://kernel-team.pages.debian.net/kernel-handbook/ch-common-tasks.html)

