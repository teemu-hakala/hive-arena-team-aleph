#!/bin/bash

make -C team-aleph-agent re
make -C team-aleph-agent_stack re

./match_million.sh team-aleph-agent team-aleph-agent_stack > trace
for i in {1..100}; do
    ./match_million.sh team-aleph-agent team-aleph-agent_stack >> trace
done