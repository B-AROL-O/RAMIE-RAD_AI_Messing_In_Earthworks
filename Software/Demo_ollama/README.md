
# MCP tool to send trajectory

```
sona@lpmu8gb:~/Demo_ollama$ uv run demo_gradio6_robot_mcp.py 
✅ Ollama server started.
Failed to open serial port: name 'serial' is not defined
* Running on local URL:  http://0.0.0.0:7860
* To create a public link, set `share=True` in `launch()`.
➡️ Sent: ```
➡️ Sent: VT25R-5L+5/0
➡️ Sent: VT10R+5L+5/0
➡️ Sent: VT23R+5L-5/0
➡️ Sent: VT12R+2L+3/0
➡️ Sent: ```
➡️ Sent: ```
➡️ Sent: VT20R+5L+5/0
➡️ Sent: VT15R+3L+3/0
➡️ Sent: VT10R+1L+1/0
➡️ Sent: VT15R-1L-1/0
➡️ Sent: VT20R-3L-3/0
➡️ Sent: VT25R-5L-5/0
➡️ Sent: ```
```

I did it, now LLM generates the correct commands to send to the electronics

Had to do a reparsing because python is dumb and cannot not reencode the terminator

```
uv run demo_gradio6_robot_mcp_serial.py 
✅ Ollama server started.
* Running on local URL:  http://0.0.0.0:7860
* To create a public link, set `share=True` in `launch()`.
❌ Failed to parse: ```
➡️ Sent: b'VT25R5L5\x00'
➡️ Sent: b'VT10R5L5\x00'
➡️ Sent: b'VT23R5L-5\x00'
➡️ Sent: b'VT12R2L3\x00'
➡️ Sent: b'VT20R-3L2\x00'
➡️ Sent: b'VT15R4L-4\x00'
➡️ Sent: b'VT25R5L5\x00'
❌ Failed to parse: ```
^CKeyboard interruption in main thread... closing server.
🛑 Ollama server stopped.
```


# LLamaindex

added dependencies

```
[project]
name = "ollama-qwen"
version = "0.1.0"
description = "Python interface to Ollama using qwen3:4b-instruct"
readme = "README.md"
requires-python = ">=3.12"

dependencies = [
    "requests>=2.32",
    "gradio>=6.0",
    "pyserial",
    "llama-index-core",
    "llama-index-llms-ollama>=0.9.0",
    "llama-index-embeddings-openai>=0.5.1",
]
```