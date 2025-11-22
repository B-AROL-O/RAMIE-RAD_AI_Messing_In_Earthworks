LM Studio
- 0.3.32

Chats







































You
import json
import requests
import gradio as gr

Computer: FW13
Application: LM Studio
Runtime: Vulkan llama.cpp
Model: Gemma 3 4B Q4 Vision Instruct
c_ollama_chat_endpoint = "http://localhost:1234/v1/chat/completions"

----------------------------------------------------------------------
MCP Tools (stand‑alone functions that can be invoked by an LLM)
----------------------------------------------------------------------
def get_current_temperature(
i_location: str,
) -> dict:
"""
Dummy temperature lookup for a given city.

Parameters
----------
i_location : str
    The name of the city or location.

Returns
-------
dict
    JSON‑serialisable dictionary containing the result.
"""
# In a real system you would query a weather API here.
l_temperature_lookup: dict[str, float] = {
    "Paris": 18.0,
    "New York": 22.5,
    "Tokyo": 27.0,
}

f_temp_celsius: float | None = l_temperature_lookup.get(i_location)
if f_temp_celsius is None:
    return {"error": f"Location '{i_location}' not found."}

s_result: str = f"{f_temp_celsius:.1f} °C"
return {"temperature": s_result, "confidence": 0.97}    
----------------------------------------------------------------------
Combined launch – UI + Tool API
----------------------------------------------------------------------
----------------------------------------------------------------------
Optional: minimal Gradio UI for local debugging
----------------------------------------------------------------------
Tell Ollama about our available tool
l_functions = [
{
"name": "get_current_temperature",
"description": "Return the current temperature for a given city.",
"parameters": {
"type": "object",
"properties": {"location": {"type": "string"}},
"required": ["location"],
},
}
]

l_payload = {
"model": "mistral",          # or whatever you’re using
"messages": [
{"role": "system", "content": "You can call get_current_temperature when needed."},
{"role": "user",   "content": "What’s the weather in Paris?"},
],
"functions": l_functions,
"stream": False,
}

def ui_get_temperature(i_city: str) -> tuple[str, float]:
"""
Wrapper that calls the tool function and returns a user‑friendly string.
"""
d_response = get_current_temperature(i_city)
if "error" in d_response:
return (d_response["error"], 0.0)
return (d_response["temperature"], d_response["confidence"])

if name == "main":

print("Test LM Studio invoke MCP tool with GRADIO")
response = requests.post(c_ollama_chat_endpoint, json=l_payload)
print(response.json())

if (False):
    with gr.Blocks() as ui_block:
        with gr.Row():
            txt_input = gr.Textbox(label="City name")
            btn_query = gr.Button("Get temperature")
        txt_output = gr.Textbox(label="Result", interactive=False)

        def on_click(i_city: str) -> None:
            s_temp, f_conf = ui_get_temperature(i_city)
            txt_output.value = f"{s_temp} (confidence {f_conf:.2%})"

        btn_query.click(on_click, inputs=txt_input, outputs=None)
D:\Data\Project\Project OrangeBot\RAMIE-RAD_AI_Messing_In_Earthworks\Software\MCP>uv run demo_mcp_paris_temperature.py
D:\Data\Project\Project OrangeBot\RAMIE-RAD_AI_Messing_In_Earthworks\Software\MCP.venv\Lib\site-packages\requests_init_.py:86: RequestsDependencyWarning: Unable to find acceptable character detection dependency (chardet or charset_normalizer).
warnings.warn(
Test LM Studio invoke MCP tool with GRADIO
{'id': 'chatcmpl-osjdmkg6x9odcjzvlb30i', 'object': 'chat.completion', 'created': 1763742761, 'model': 'gemma-3-4b-it', 'choices': [{'index': 0, 'message': {'role': 'assistant', 'content': "tool_code\nprint(get_current_temperature(location='Paris'))\n", 'tool_calls': []}, 'logprobs': None, 'finish_reason': 'stop'}], 'usage': {'prompt_tokens': 29, 'completion_tokens': 20, 'total_tokens': 49}, 'stats': {}, 'system_fingerprint': 'gemma-3-4b-it'}

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
I don't get it.

I expected the LLM to invoke the tool and get the temperature, but it didn't do that?

gpt-oss-20b@q6_k_xl
Thought for 55.79 seconds














Why the temperature wasn’t returned
What you see	Why it happens
The assistant’s reply contains tool_code … get_current_temperature(...) but no tool_calls field in the response payload.	The model (Gemma‑3‑4B‑It that runs inside LM Studio) does not support function‑calling out of the box – it only “suggests” code that you could run yourself.
Your script simply prints the raw JSON returned by the /v1/chat/completions endpoint.	Because there are no tool_calls, nothing is executed automatically, so your helper function (ui_get_temperature) never gets invoked.
Two things went wrong
The model has no built‑in tool‑calling capability

Gemma (and most llama.cpp based models) can’t interpret the functions/tool_calls protocol that OpenAI’s GPT‑4 or Anthropic’s Claude use.
They only generate code or textual hints, which is why you see the tool_code block.
Your client didn’t act on a tool call

Even if a model returned tool_calls, your script would still need to detect them and run the corresponding Python function.
Your current code only prints the raw JSON; it never checks for or executes any tool calls.
How to get the temperature automatically
There are two common paths:

Path	What you need	Pros	Cons
A – Use a model that supports function calling (e.g. OpenAI GPT‑4o, Anthropic Claude 3, Llama‑Index’s own tool‑calling)	Switch to an API or local server that implements the Function Calling protocol.	Automatic invocation; no extra parsing code.	Requires access to a capable model / service.
B – Keep Gemma but add a small wrapper	After each chat completion, inspect response['choices'][0]['message'].get('tool_calls'). If present, deserialize the arguments and call your Python function.	Works with any LLM; you control the execution logic.	Requires manual parsing; the model must actually emit tool_calls (which Gemma won’t do).
Because Gemma can’t produce real tool_calls, option B will never succeed unless you first rewrite the prompt so that the assistant writes code and then you run that code yourself.

A minimal working example with a function‑calling model
Below is a self‑contained script that demonstrates the complete workflow using OpenAI’s API (or any other compatible endpoint).

You can adapt it to LM Studio if you install the OpenAI Python client and point it at your local server.

# demo_mcp_paris_temperature_openai.py

import json
from typing import Dict, Tuple
import openai            # pip install openai

# ----------------------------------------------------------------------
#  Constants
# ----------------------------------------------------------------------
c_ollama_chat_endpoint = "http://localhost:1234/v1/chat/completions"   # LM Studio endpoint (if you use it)
c_openai_api_key      : str = "<YOUR_OPENAI_API_KEY>"
c_openai_endpoint     : str = "https://api.openai.com/v1"

# ----------------------------------------------------------------------
#  Tool implementation
# ----------------------------------------------------------------------
def get_current_temperature(
    i_location: str,
) -> Dict[str, str | float]:
    """
    Dummy temperature lookup for a given city.

    Parameters
    ----------
    i_location (str)
        The name of the city or location.

    Returns
    -------
    dict[str, str|float]
        JSON‑serialisable dictionary containing the result.
    """
    l_temperature_lookup: Dict[str, float] = {
        "Paris": 18.0,
        "New York": 22.5,
        "Tokyo": 27.0,
    }

    f_temp_celsius: float | None = l_temperature_lookup.get(i_location)
    if f_temp_celsius is None:
        return {"error": f"Location '{i_location}' not found."}

    s_result: str = f"{f_temp_celsius:.1f} °C"
    return {"temperature": s_result, "confidence": 0.97}


# ----------------------------------------------------------------------
#  Build the tool description for the LLM
# ----------------------------------------------------------------------
l_functions = [
    {
        "name": "get_current_temperature",
        "description": "Return the current temperature for a given city.",
        "parameters": {
            "type": "object",
            "properties": {
                "location": {"type": "string"},
            },
            "required": ["location"],
        },
    }
]


# ----------------------------------------------------------------------
#  Helper: invoke the LLM and automatically handle tool calls
# ----------------------------------------------------------------------
def ask_llm_and_handle_tools(
    i_prompt: str,
) -> Tuple[str, float]:
    """
    Send a prompt to the LLM, let it decide whether to call our tool,
    and return the final answer (temperature string + confidence).

    Returns
    -------
    tuple[str, float]
        The temperature string and the confidence value.
    """
    # Build the request payload
    d_payload = {
        "model": "gpt-4o-mini",          # any model that supports function calling
        "messages": [
            {"role": "system", "content": "You can call get_current_temperature when needed."},
            {"role": "user",   "content": i_prompt},
        ],
        "functions": l_functions,
        "function_call": "auto",          # let the model decide whether to call a function
    }

    # Call the API
    d_response = openai.ChatCompletion.create(
        api_key=c_openai_api_key,
        url=c_openai_endpoint,  # if you want to point at LM Studio: use c_ollama_chat_endpoint instead
        **d_payload
    )

    # ------------------------------------------------------------------
    #  Parse the response
    # ------------------------------------------------------------------
    d_message = d_response["choices"][0]["message"]

    # If the model decided to call a tool, we get it in the `tool_calls` field.
    l_tool_calls = d_message.get("tool_calls") or []

    if not l_tool_calls:
        # No function was called – just return
Send a message to the model...





System Prompt

/no_think
reasoning:low

You are a Python developer tasked with writing clean, readable, and maintainable code. Your absolute guiding principle is that someone will come one year after the fact to be able to understand what the code does and how.

Follow these guidelines strictly:

### VARIABLES
- Use descriptive names for variables starting with their type and following with their purpose.
- Combine words using underscores (`_`) to separate them (snake_case).
- Avoid single-letter variable names unless they are loop counters (e.g., `i`, `j`).

**Examples:**
```python
n_iteration_counter = 0  # number
tnn_position = (n_x, n_y)  # tuple of two numbers
ln_array = list()  # list of numbers
st_position = Position(3, 5)  # structure
cl_engine = Engine()  # class
s_name = "Meaningful"  # name
x_continue = True #boolean
```

### CLASSES
- Capitalize the first letter of each word in class names and use underscores to separate them. Only the first word is capitalize and none that follows.

**Example:**
```python
class My_custom_class:
    def __init__(self):
        self.s_some_variable = str()
```

### FUNCTIONS
- Use descriptive names for functions and take advantage of abstraction layers.
- Prefix input variables with `i_`.

**Example:**
```python
def my_function(i_name: str) -> Tuple[str, int]:
    pass
```

### COMMENTS
- Use Doxygen-like docstrings to explain the inputs and outputs of functions and describe the logic used.
- Write comments to explain the intent of the code. Avoid inline comments.

**Example:**
```python
def my_function(i_name: str) -> Tuple[str, int]:
    """
    This function takes a name as input and returns a tuple containing the name and its length.

    Parameters:
    i_name (str): The name to process.

    Returns:
    Tuple[str, int]: A tuple containing the name and the number of characters in the name.
    """
    return (i_name, len(i_name))
```

### TYPE HINTS
- Always use type hinting.

**Example:**
```python
def my_function(i_name: str) -> Tuple[str, int]:
    pass
```
### CONSTANTS

Use constants with meaningful names, prioritize constants as public class variables with "c" prefix.
**Example:**
```python
N_RESOLUTION = 1024
class Gravity():
    cn_gravity=9.81 #Earth gravity in m/s2
```

### SHORTHANDS
- Do not shorthand or use aliases. Use the full module names instead of imports with aliases. You don't use shorthands for tuples, lists and dictionary either.

**Example:**
```python
#no shorthands for imports
import numpy

ln_vector = numpy.array()

#no shorthand for types
ln_lottery = list()
tn_position= tuple()
d_names = dict()
```
Token count:
630


Input tokens:0
Context: 2444 / 20000