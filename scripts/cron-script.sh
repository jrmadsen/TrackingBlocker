#!/bin/bash -e
#
# Use/modify this script to run a nightly update:
# Sample crontab file:
#
#    SHELL=/bin/bash
#    PATH=/usr/local/bin:/usr/bin:/usr/sbin:/bin:/sbin:/usr/local/sbin
#    MAILTO=root
#
#    0 0 * * * /root/TrackingBlocker/scripts/cron-script.sh /root/TrackingBlocker
#

set -v

if [ "$(whoami)" != "root" ]; then
    echo "Run as root!"
    exit 1
fi

: ${PROFILE:="hosts.profile.0"}

if [ -z "${1}" ]; then echo "Provide argument: <source-dir>"; exit 1; fi
if [ -n "${2}" ]; then PROFILE=${2}; fi

SOURCE_DIR=$(realpath ${1})

cd ${SOURCE_DIR}
# add existing to profile
cat /etc/hosts >> ./share/${PROFILE}

echo "Hostname: $(hostname)"
sed -i s,$(hostname),localhost,g ./share/${PROFILE}

mkdir -p build-cron
cd build-cron

rm -rf *
cmake -DCMAKE_INSTALL_PREFIX=/usr/local .. -G Ninja
ninja
ninja install
cp ./hosts.profile.0 ../share/${PROFILE}

/usr/local/bin/tracking_blocker status on
cd ${SOURCE_DIR}
rm -rf build-cron

sed -i s,$(hostname),localhost,g ./share/${PROFILE}
