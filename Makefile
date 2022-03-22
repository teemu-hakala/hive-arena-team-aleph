all: agent

agent:
	make -C src re
	make -C team-aleph-agent re
	cp team-aleph-agent/agent .
