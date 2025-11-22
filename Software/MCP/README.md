# RAMIE's MCP Server

To run:

```bash
uv run main.py
```

Now you should be able to go to `https://localhost:7860` and use the MCP tools via the Gradio interface.

#### 2025-11-21

SO the gradio just copy over I need to call the actual LLM so I asked OSS20B to do a demo that does function calling

```
uv run demo_mcp_paris_temperature.py
Traceback (most recent call last):
  File "D:\Data\Project\Project OrangeBot\RAMIE-RAD_AI_Messing_In_Earthworks\Software\MCP\demo_mcp_paris_temperature.py", line 2, in <module>
    import requests
ModuleNotFoundError: No module named 'requests'

ModuleNotFoundError: No module named 'urllib3'
```

I don't have internet so I copy over dependencies from another VENV into  ```venv\Lib\site-packages```

```
2025-11-21 07:14:54 [ERROR]
 Unexpected endpoint or method. (POST /api/chat). Returning 200 anyway
```

This error is just because ollama has a different endpoint ```/v1/chat/completions```

OUTPUT

```
{'id': 'chatcmpl-qpm3ybfyak9p8v4kibask', 'object': 'chat.completion', 'created': 1763705846, 'model': 'gemma-3-4b-it', 'choices': [{'index': 0, 'message': {'role': 'assistant', 'content': "```tool_code\nprint(get_current_temperature(location='Paris'))\n```", 'tool_calls': []}, 'logprobs': None, 'finish_reason': 'stop'}], 'usage': {'prompt_tokens': 29, 'completion_tokens': 20, 'total_tokens': 49}, 'stats': {}, 'system_fingerprint': 'gemma-3-4b-it'}
```

LM Studio Log
```
 [gemma-3-4b-it] Running chat completion on conversation with 2 messages.
2025-11-21 07:17:26  [INFO]
 [gemma-3-4b-it] Prompt processing progress: 0.0%
2025-11-21 07:17:26  [INFO]
 [gemma-3-4b-it] Prompt processing progress: 100.0%
2025-11-21 07:17:29  [INFO]
 [gemma-3-4b-it] Model generated tool calls:  []
2025-11-21 07:17:29  [INFO]
 [gemma-3-4b-it] Generated prediction:  {
  "id": "chatcmpl-qpm3ybfyak9p8v4kibask",
  "object": "chat.completion",
  "created": 1763705846,
  "model": "gemma-3-4b-it",
  "choices": [
    {
      "index": 0,
      "message": {
        "role": "assistant",
        "content": "```tool_code\nprint(get_current_temperature(location='Paris'))\n```",
        "tool_calls": []
      },
      "logprobs": null,
      "finish_reason": "stop"
    }
  ],
  "usage": {
    "prompt_tokens": 29,
    "completion_tokens": 20,
    "total_tokens": 49
  },
  "stats": {},
  "system_fingerprint": "gemma-3-4b-it"
}
```

The above is just the test of the tool itself

Now I hook up the tool to a model request and this should automagically call?

```
  File "D:\Data\Project\Project OrangeBot\RAMIE-RAD_AI_Messing_In_Earthworks\Software\MCP\demo_mcp_paris_temperature.py", line 54, in <module>
    @tool_server.get("/get_current_temperature")
     ^^^^^^^^^^^^^^^
AttributeError: 'Blocks' object has no attribute 'get'
```

I hooked up the application but it fails

---

Vibecode a fix, no it keeps going

### Test LM Studio no server

```
  File "D:\Data\Project\Project OrangeBot\RAMIE-RAD_AI_Messing_In_Earthworks\Software\MCP\.venv\Lib\site-packages\requests\sessions.py", line 703, in send      
    r = adapter.send(request, **kwargs)
  File "D:\Data\Project\Project OrangeBot\RAMIE-RAD_AI_Messing_In_Earthworks\Software\MCP\.venv\Lib\site-packages\requests\adapters.py", line 700, in send      
    raise ConnectionError(e, request=request)
requests.exceptions.ConnectionError: HTTPConnectionPool(host='localhost', port=1234): Max retries exceeded with url: /v1/chat/completions (Caused by NewConnectionError('<urllib3.connection.HTTPConnection object at 0x000002AB6B3DFE00>: Failed to establish a new connection: [WinError 10061] No connection could be made because the target machine actively refused it'))
```

### Test LM Studio server on

```
D:\Data\Project\Project OrangeBot\RAMIE-RAD_AI_Messing_In_Earthworks\Software\MCP>uv run demo_mcp_paris_temperature.py
D:\Data\Project\Project OrangeBot\RAMIE-RAD_AI_Messing_In_Earthworks\Software\MCP\.venv\Lib\site-packages\requests\__init__.py:86: RequestsDependencyWarning: Unable to find acceptable character detection dependency (chardet or charset_normalizer).
  warnings.warn(
Test LM Studio invoke MCP tool with GRADIO
{'id': 'chatcmpl-osjdmkg6x9odcjzvlb30i', 'object': 'chat.completion', 'created': 1763742761, 'model': 'gemma-3-4b-it', 'choices': [{'index': 0, 'message': {'role': 'assistant', 'content': "```tool_code\nprint(get_current_temperature(location='Paris'))\n```", 'tool_calls': []}, 'logprobs': None, 'finish_reason': 'stop'}], 'usage': {'prompt_tokens': 29, 'completion_tokens': 20, 'total_tokens': 49}, 'stats': {}, 'system_fingerprint': 'gemma-3-4b-it'}
```

```
2025-11-21 17:31:44  [INFO]
 [LM STUDIO SERVER] Success! HTTP server listening on port 1234
2025-11-21 17:31:44  [INFO]
2025-11-21 17:31:44  [INFO]
 [LM STUDIO SERVER] Supported endpoints:
2025-11-21 17:31:44  [INFO]
 [LM STUDIO SERVER] ->	GET  http://localhost:1234/v1/models
2025-11-21 17:31:44  [INFO]
 [LM STUDIO SERVER] ->	POST http://localhost:1234/v1/responses
2025-11-21 17:31:44  [INFO]
 [LM STUDIO SERVER] ->	POST http://localhost:1234/v1/chat/completions
2025-11-21 17:31:44  [INFO]
 [LM STUDIO SERVER] ->	POST http://localhost:1234/v1/completions
2025-11-21 17:31:44  [INFO]
 [LM STUDIO SERVER] ->	POST http://localhost:1234/v1/embeddings
2025-11-21 17:31:44  [INFO]
2025-11-21 17:31:44  [INFO]
 [LM STUDIO SERVER] Logs are saved into C:\Users\FatherOfMachines\.cache\lm-studio\server-logs
2025-11-21 17:31:44  [INFO]
 Server started.
2025-11-21 17:31:44  [INFO]
 Just-in-time model loading inactive.
2025-11-21 17:32:41  [INFO]
 [gemma-3-4b-it] Running chat completion on conversation with 2 messages.
2025-11-21 17:32:41  [INFO]
 [gemma-3-4b-it] Prompt processing progress: 0.0%
2025-11-21 17:32:41  [INFO]
 [gemma-3-4b-it] Prompt processing progress: 100.0%
2025-11-21 17:32:42  [INFO]
 [gemma-3-4b-it] Model generated tool calls:  []
2025-11-21 17:32:42  [INFO]
 [gemma-3-4b-it] Generated prediction:  {
  "id": "chatcmpl-osjdmkg6x9odcjzvlb30i",
  "object": "chat.completion",
  "created": 1763742761,
  "model": "gemma-3-4b-it",
  "choices": [
    {
      "index": 0,
      "message": {
        "role": "assistant",
        "content": "```tool_code\nprint(get_current_temperature(location='Paris'))\n```",
        "tool_calls": []
      },
      "logprobs": null,
      "finish_reason": "stop"
    }
  ],
  "usage": {
    "prompt_tokens": 29,
    "completion_tokens": 20,
    "total_tokens": 49
  },
  "stats": {},
  "system_fingerprint": "gemma-3-4b-it"
}
```


# MCP Server Robot Trajectory

MCP server that allows generation of a robot trajectory

TODO