#!/bin/sh
# $Id: sysinfo.sh,v 1.18 2001-03-15 17:27:44 wagner Exp $

if [ "$SYSINFO_DONE" != "yes" ] ; then

SYSINFO_DONE="yes"

UNAME=`uname`

PRJ_ROOT=${PRJ_ROOT:-${HOME}/work}

#-----------------------------------------------------------------------------
# set some defaults
CM3VERSION="5.1.2"
CM3_GCC_BACKEND=yes
CM3_GDB=no
CM3_INSTALL=/usr/local/cm3
EXE=""
SL="/"
SYSLIBDIR="/usr/local/cm3/lib"
SYSLIBS=""
DEV_LIB=""
DEV_BIN=""
TAR=tar

if [ -z "$TMPDIR" -o ! -d "$TMPDIR" ] ; then
  if [ -n "$TMP" -a -d "$TMP" ] ; then
    TMPDIR="$TMP"
  elif [ -n "$TEMP" -a -d "$TEMP" ] ; then
    TMPDIR="$TEMP"
  elif [ -d "/var/tmp" ] ; then
    TMPDIR=/var/tmp
  elif [ -d "/usr/tmp" ] ; then
    TMPDIR=/usr/tmp
  elif [ -d "/tmp" ] ; then
    TMPDIR=/tmp
  elif [ -d "c:/tmp" ] ; then
    TMPDIR="c:/tmp"
  elif [ -d "d:/tmp" ] ; then
    TMPDIR="d:/tmp"
  elif [ -d "e:/tmp" ] ; then
    TMPDIR="e:/tmp"
  elif [ -d "c:/temp" ] ; then
    TMPDIR="c:/temp"
  elif [ -d "d:/temp" ] ; then
    TMPDIR="d:/temp"
  elif [ -d "e:/temp" ] ; then
    TMPDIR="e:/temp"
  else
    echo "please define TMPDIR" 1>&2
    exit 1
  fi
fi

#-----------------------------------------------------------------------------
# some localization functions
find_dir() {
  for d in $@ ; do
    if [ -d "$d" ] ; then
      echo "$d"
      return 0
    fi
  done
  return 1
}

find_file() {
  f="$1"
  shift
  for d in $@ ; do
    if [ -d "$d" -a -f "$d/$f" ] ; then
      echo "$d/$f"
      return 0
    fi
  done
  echo "$f"
  return 1
}

#-----------------------------------------------------------------------------
# abstraction functions
cygpath() {
  echo "$2"
}

strip_exe() {
  strip $@
}

#-----------------------------------------------------------------------------
# evaluate uname information
case "${UNAME}" in

  Windows*|WinNT*|Cygwin*|CYGWIN*)
    CM3_OSTYPE=WIN32
    CM3_TARGET=NT386
    CM3_INSTALL="c:/cm3"
    CM3_GCC_BACKEND=no
    HAVE_SERIAL=yes
    EXE=".exe"
    SL='\\\\'
    SYSLIBS="ADVAPI32.LIB GDI32.LIB KERNEL32.LIB ODBC32.LIB"
    SYSLIBS="${SYSLIBS} OPENGL32.LIB WSOCK32.LIB COMDLG32.LIB"
    SYSLIBS="${SYSLIBS} GLU32.LIB NETAPI32.LIB ODBCCP32.LIB USER32.LIB"
    L="c:/cm3/lib d:/cm3/lib e:/cm3/lib c:/reactor5/lib d:/reactor5/lib"
    L="${L} e:/reactor5/lib c:/reactor/lib d:/reactor/lib"
    L="${L} e:/reactor/lib /usr/local/cm3/lib /usr/local/reactor/lib"
    L="${L} /usr/cm3/lib /usr/reactor/lib"
    CM3LIBSEARCHPATH="${L}"
    CM3BINSEARCHPATH="`echo ${L} | sed -e 's/lib/bin/g'`"
    if f="`find_file KERNEL32.LIB ${L}`" ; then
      SYSLIBDIR="`dirname $f`"
    else
      SYSLIBDIR="unknown"
    fi
    D="c:/msdev/bin d:/msdev/bin e:/msdev/bin f:/msdev/bin g:/msdev/bin"
    if f="`find_file cl.exe ${D}`" ; then
      DEV_BIN="`dirname ${f}`"
      DEV_LIB="`dirname ${DEV_BIN}`/lib"
    else
      DEV_LIB=""
      DEV_BIN=""
    fi
    if [ -f /usr/bin/tar.exe ] ; then
      TAR=/usr/bin/tar.exe
    fi

    cygpath() {
      /usr/bin/cygpath $@
    }
    strip_exe() {
      return 0;
    }
  ;;

  FreeBSD*)
    CM3_OSTYPE=POSIX
    if [ "`uname -m`" = i386 ] ; then
      case "`uname -r`" in
        1*) CM3_TARGET=FreeBSD;;
        2*) CM3_TARGET=FreeBSD2;;
        3*) CM3_TARGET=FreeBSD3;;
        4*) CM3_TARGET=FreeBSD4;;
        *) CM3_TARGET=FreeBSD4;;
      esac
    else
      CM3_TARGET=FBSD_ALPHA
    fi
  ;;

  SunOS*)
    CM3_OSTYPE=POSIX
    CM3_TARGET=SOLgnu
    #CM3_TARGET=SOLsun
  ;;

  Linux*)
    CM3_OSTYPE=POSIX
    CM3_TARGET=LINUXLIBC6
  ;;

  # more need to be added here, I haven't got all the platform info ready
esac

#-----------------------------------------------------------------------------
# define the exported values
if [ -n "$root" ] ; then
  ROOT=${ROOT:-${root}}
else
  ROOT=${ROOT:-${PRJ_ROOT}/cm3}
fi
SCRIPTS=${SCRIPTS:-${ROOT}/scripts}
M3GDB=${M3GDB:-${CM3_GDB}}
M3OSTYPE=${M3OSTYPE:-${CM3_OSTYPE}}
TARGET=${TARGET:-${CM3_TARGET}}
GCC_BACKEND=${GCC_BACKEND:-${CM3_GCC_BACKEND}}
INSTALLROOT=${INSTALLROOT:-${CM3_INSTALL}}
PKGSDB=${PKGSDB:-$ROOT/scripts/PKGS}
QGREP=${QGREP:-"egrep >/dev/null 2>/dev/null"}
GREP=${GREP:-egrep}

if [ "${M3OSTYPE}" = "WIN32" ] ; then
  CM3ROOT="`cygpath -w ${ROOT} | sed -e 's;\\\;\\\\\\\\;g'`"
else
  CM3ROOT="${ROOT}"
fi


#-----------------------------------------------------------------------------
# output functions
debug() {
  if [ -n "$CM3_DEBUG" ] ; then
    echo "$*"
  fi
}

header() {
  echo ""
  echo '----------------------------------------------------------------------------'
  echo $@
  echo '----------------------------------------------------------------------------'
  echo ""
}

#-----------------------------------------------------------------------------
# elego customizations
#
# uncomment these if they interfere with your environment
if type domainname > /dev/null 2>/dev/null && \
   [ "${M3OSTYPE}" = "POSIX" -a \
     "`domainname 2>/dev/null`" = "iceflower" ] ; then
  STAGE=${STAGE:-/t/wagner/cm3}
  export STAGE
fi
if [ "${M3OSTYPE}" = "WIN32" -a "${HOSTNAME}" = "FIR" ] ; then
  STAGE=${STAGE:-e:/home/wagner/tmp/cm3stage}
  export STAGE
fi

#-----------------------------------------------------------------------------
# debug output
debug "ROOT        = $ROOT"
debug "SCRIPTS     = $SCRIPTS"
debug "M3GDB       = $M3GDB"
debug "M3OSTYPE    = $M3OSTYPE"
debug "TARGET      = $TARGET"
debug "GCC_BACKEND = $GCC_BACKEND"
debug "INSTALLROOT = $INSTALLROOT"
debug "PKGSDB      = $PKGSDB"
debug "QGREP       = $QGREP"
debug "GREP        = $GREP"
debug "TMPDIR      = $TMPDIR"
debug "EXE         = $EXE"
debug "SL          = $SL"
debug "SYSLIBDIR   = $SYSLIBDIR"
debug "SYSLIBS     = $SYSLIBS"
debug "DEV_BIN     = $DEV_BIN"
debug "DEV_LIB     = $DEV_LIB"
debug "TAR         = $TAR"
debug "CM3ROOT     = $CM3ROOT"

export ROOT SCRIPTS M3GDB M3OSTYPE TARGET GCC_BACKEND INSTALLROOT PKGSDB QGREP
export GREP TMPDIR EXE SL CM3VERSION SYSLIBDIR SYSLIB DEV_BIN DEV_LIB TAR
export CM3LIBSEARCHPATH CM3BINSEARCHPATH CM3ROOT
export SYSINFO_DONE

fi
