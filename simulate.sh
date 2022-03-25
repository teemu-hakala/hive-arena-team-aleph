#!/usr/bin/env bash

if [ $# -ne 2 ]; then
    echo "Usage: ./simulate.sh agent0_directory agent1_directory"
    exit 1
fi

ITERATIONS=50

RED='\033[0;31m'
NC='\033[0m'
TEAM1=$(basename $1)
TEAM2=$(basename $2)

for (( c=1; c<=$ITERATIONS; c++))
do
	./match_sim.sh $1 $2
	if test -f $1/exceptions.log; then
		echo -e "${RED}ALERT: Exception for team $TEAM1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Exception for team ${TEAM1}${NC}"
		echo -e "Exception for team $TEAM1" >> simulation_exceptions.log
		cat $1/exceptions.log >> simulation_exceptions.log
	fi
	if test -f $2/exceptions.log; then
		echo -e "${RED}ALERT: Exception for team $TEAM2!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Exception for team ${TEAM2}${NC}"
		echo -e "Exception for team $TEAM2" >> simulation_exceptions.log
		cat $2/exceptions.log >> simulation_exceptions.log
	fi
done

for LOGFILE in *Z.log
do
	tail -1 $LOGFILE >> simulation_result.log
done

COUNT1=$(grep -c $TEAM1 simulation_result.log)
COUNT2=$(grep -c $TEAM2 simulation_result.log)

echo "Simulation results" >> simulation_stats_${TEAM1}_VS_${TEAM2}.log
echo "$TEAM1 won $COUNT1 / $ITERATIONS of the matches" >> simulation_stats_${TEAM1}_VS_${TEAM2}.log
echo "$TEAM2 won $COUNT2 / $ITERATIONS of the matches" >> simulation_stats_${TEAM1}_VS_${TEAM2}.log
if test -f simulation_exceptions.log; then
	echo "ALERT! EXCEPTIONS RAISED" >> simulation_stats_${TEAM1}_VS_${TEAM2}.log
	cat simulation_exceptions.log >> simulation_stats_${TEAM1}_VS_${TEAM2}.log
fi

rm *Z.log
rm simulation_result.log

if test -f simulation_exceptions.log; then
	rm simulation_exceptions.log
fi