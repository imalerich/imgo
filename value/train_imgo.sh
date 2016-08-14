#!/usr/bin/env sh
set -e

caffe train --solver=imgo_value_solver.prototxt $@
