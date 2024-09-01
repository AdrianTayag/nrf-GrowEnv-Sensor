#!/usr/bin/env bash

set -e

docker run \
-v ${PWD}:/workdir/project nordicplayground/nrfconnect-sdk:v2.6-branch \
west build -p --build-dir /workdir/project/build /workdir/project/ --pristine --board nrf7002dk_nrf5340_cpuapp_ns
