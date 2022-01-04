#!/bin/sh
set -xe

truncate -s 512 $3
cat "${1}/${3}" "${1}/${4}" > "${1}/${2}"
truncate -s 10321920 "$1"/"$2"