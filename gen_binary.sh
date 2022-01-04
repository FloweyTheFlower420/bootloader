#!/bin/sh
set -xe

truncate -s 512 $3
truncate -s 1024 $4
cat "${1}/${3}" "${1}/${4}" > "${1}/${2}"
truncate -s 10321920 "$1"/"$2"