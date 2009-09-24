#/bin/sh

WS=${WORKSPACE}
uname -a
date
cd ${WS}/cm3/scripts || {
  echo "cannot cd to ${WS}/cm3/scripts" 1>&2 
  exit 1
}
. ./sysinfo.sh
echo "WS=${WS}"
echo "CVS_BRANCH=${CVS_BRANCH}"
echo "BUILD_TAG=${BUILD_TAG}"
echo ""
cd ${WS}/cm3/scripts/regression || {
  echo "cannot cd to ${WS}/cm3/scripts/regression" 1>&2 
  exit 1
}

CM3CG=${WS}/../cm3-m3cc-${TARGET}/cm3/m3-sys/m3cc/${TARGET}/cm3cg
if [ "$CLEAN" = "false" ]; then
  if [ -x "${CM3CG}" ]; then
    echo "checking for working pre-built cm3cg in ${CM3CG}"
    cp -p "${CM3CG}" ${WS}/cm3cg
    if ${WS}/cm3cg --version; then
      echo "using PREBUILT_CM3CG=${WS}/cm3cg"
      PREBUILT_CM3CG=${WS}/cm3cg
      OMIT_GCC=yes
      export PREBUILT_CM3CG OMIT_GCC
    else
      echo "NOT using ${WS}/cm3cg"
    fi
  else
    echo "no executable ${CM3CG}"
  fi
fi
. ./defs.sh

[ "${WS}/cm3/scripts/pkginfo.txt" -nt "${WS}/cm3/scripts/PKGS" ] && {
  echo "deleting outdated packages cache ${WS}/cm3/scripts/PKGS"
  rm -f "${WS}/cm3/scripts/PKGS"
}

# setup lastok version as a start
if [ ! -d "${INSTROOT_CUR}" ]; then
  cp -pR ${INSTROOT_LOK} ${INSTROOT_CUR}
fi

export TMPDIR=${WS}
test_build_system || {
  echo "test_build_system failed" 1>&2
  exit 1
}

