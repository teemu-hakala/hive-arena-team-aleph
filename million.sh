#!/bin/bash

make -C team-aleph-agent re
make -C team-aleph-agent-thursday re

./match_million.sh team-aleph-agent team-aleph-agent-thursday > trace
for i in {1..20}; do
    ./match_million.sh team-aleph-agent team-aleph-agent-thursday >> trace
done