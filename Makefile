all: agent

agent:
	make -C src
	make -C team-aleph-agent
	cp team-aleph-agent/agent .
