# Test Bench Motor Orchestration

This is a codeblock projject to develop the class that takes care of orchestrating the motion instruction for the robot.

Capabilities:
- Store two wheel speed and a duration in time steps
- Pause/Start execution
- Add motion to the execution static circular queue
- Execute will decrease duration by one, if duration is zero delete the oldest motion, and take the speed of the oldest motion for execution and send it to the wheels

I designed it this way so that the MCP server can send a list of instructions all at once, saving me the trouble of doing the real time in python inside the MCP server. Real time is done properly inside the microcontroller.

I did something fun, I had a requirement file, and asked a local LLM OSS20B to vibe code it for me on my laptop on the go. It got quickly 80% of the way there.

I need to make a stronger system prompts to make it generate properly formatted and designed CPP code, it's a bit of a mess and required lots of cleaning up. But it was able to vibecode a circular queue that seems sensible with all the methods mostly ok.


