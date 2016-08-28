#!/usr/bin/env sh
set -e

caffe train --solver=imgo_policy_solver.prototxt $@
python2 predict.py
