#!/usr/bin/env sh
set -e

caffe train --solver=imgo_black_policy_solver.prototxt $@
caffe train --solver=imgo_white_policy_solver.prototxt $@
