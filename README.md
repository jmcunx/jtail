## jtail - clone of tail(1)

jtail is a clone of tail(1) for systems that
does not have a full featured tail(1).
But it may have a couple of options not
found in tail(1).

If tail(1) exists on your system, you should use that instead.
It will be much faster.

This requires [j\_lib2](https://github.com/jmcunx/j_lib2) to build.

[GNU automake](https://en.wikipedia.org/wiki/Automake)
only confuses me, but this seems to be good enough for me.

**To compile:**
* If "DESTDIR" is not set, will install under /usr/local
* Execute ./build.sh to create a Makefile
* Works on Linux, BSD and AIX

_To uninstall_, execute
"make uninstall"
from the source directory
