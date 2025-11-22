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
Tool Server – a tiny FastAPI app hosted inside Gradio Blocks
----------------------------------------------------------------------
Note: The above route is automatically exposed at
http://localhost:<demo.server_port>/get_current_temperature
with gr.Blocks() as tool_server:
# The UI is optional; you can keep it invisible.
# We only expose the endpoints, not any chat widgets.

@tool_server.get("/get_current_temperature")
def serve_get_current_temperature(
    location: str,
) -> dict:
    """
    HTTP GET endpoint that forwards a request to `get_current_temperature`.

    Parameters
    ----------
    location : str
        Query parameter supplied by the LLM (or any client).

    Returns
    -------
    dict
        The JSON payload returned by the underlying tool function.
    """
    return get_current_temperature(location)
----------------------------------------------------------------------
Combined launch – UI + Tool API
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

if name == "main":

print("Test LM Studio invoke MCP tool with GRADIO")
response = requests.post(c_ollama_chat_endpoint, json=l_payload)
print(response.json())
D:\Data\Project\Project OrangeBot\RAMIE-RAD_AI_Messing_In_Earthworks\Software\MCP>uv run demo_mcp_paris_temperature.py
D:\Data\Project\Project OrangeBot\RAMIE-RAD_AI_Messing_In_Earthworks\Software\MCP.venv\Lib\site-packages\requests_init_.py:86: RequestsDependencyWarning: Unable to find acceptable character detection dependency (chardet or charset_normalizer).
warnings.warn(
Traceback (most recent call last):
File "D:\Data\Project\Project OrangeBot\RAMIE-RAD_AI_Messing_In_Earthworks\Software\MCP\demo_mcp_paris_temperature.py", line 56, in <module>
@tool_server.get("/get_current_temperature")
^^^^^^^^^^^^^^^
AttributeError: 'Blocks' object has no attribute 'get'

gpt-oss-20b@q6_k_xl
Thought for 30.61 seconds








What went wrong

gr.Blocks is a Gradio UI container – it knows how to build the chat interface, but it does not expose HTTP routes.

The @tool_server.get("/…") decorator comes from FastAPI, not from Gradio.  Therefore the interpreter raises

AttributeError: 'Blocks' object has no attribute 'get'
To make a tool endpoint you need to create a real FastAPI (or Starlette) application, add the route there, and optionally mount the Gradio UI on that same app if you want both the API and an interactive front‑end.

Below is a clean, self‑contained rewrite that follows your style guidelines and shows how to expose the get_current_temperature tool as an HTTP endpoint while keeping the optional Gradio UI for debugging or manual testing.

1. The core logic – still the same
# ----------------------------------------------------------------------
#  MCP Tools (stand‑alone functions that can be invoked by an LLM)
# ----------------------------------------------------------------------


def get_current_temperature(i_location: str) -> dict:
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
    l_city_to_temp_celsius: dict[str, float] = {
        "Paris": 18.0,
        "New York": 22.5,
        "Tokyo": 27.0,
    }

    f_temp_celsius: float | None = l_city_to_temp_celsius.get(i_location)
    if f_temp_celsius is None:
        return {"error": f"Location '{i_location}' not found."}

    s_result: str = f"{f_temp_celsius:.1f} °C"
    return {"temperature": s_result, "confidence": 0.97}
2. FastAPI endpoint
# ----------------------------------------------------------------------
#  FastAPI app – the tool server that Ollama will call
# ----------------------------------------------------------------------


import fastapi

app = fastapi.FastAPI(
    title="MCP Temperature Tool",
    description="Provides current temperature for a given city.",
)

@app.get("/get_current_temperature")
async def serve_get_current_temperature(location: str) -> dict:
    """
    HTTP GET endpoint that forwards the request to ``get_current_temperature``.

    Parameters
    ----------
    location : str
        Query parameter supplied by the LLM (or any client).

    Returns
    -------
    dict
        The JSON payload returned by the underlying tool function.
    """
    return get_current_temperature(location)
Why use async?

FastAPI can work with synchronous functions too, but declaring it async gives you the full performance benefit if you later add I/O‑bound code.

3. Optional Gradio UI for manual testing
If you still want a quick web interface that lets you type a city name and see the result, mount a very small Gradio app on the same FastAPI process:

# ----------------------------------------------------------------------
#  Optional: minimal Gradio UI for local debugging
# ----------------------------------------------------------------------


import gradio as gr

def ui_get_temperature(i_city: str) -> tuple[str, float]:
    """
    Wrapper that calls the tool function and returns a user‑friendly string.
    """
    d_response = get_current_temperature(i_city)
    if "error" in d_response:
        return (d_response["error"], 0.0)
    return (d_response["temperature"], d_response["confidence"])

with gr.Blocks() as ui_block:
    with gr.Row():
        txt_input = gr.Textbox(label="City name")
        btn_query = gr.Button("Get temperature")
    txt_output = gr.Textbox(label="Result", interactive=False)

    def on_click(i_city: str) -> None:
        s_temp, f_conf = ui_get_temperature(i_city)
        txt_output.value = f"{s_temp} (confidence {f_conf:.2%})"

    btn_query.click(on_click, inputs=txt_input, outputs=None)

# Mount the Gradio blocks onto the FastAPI app
gr.mount_gradio_app(app, ui_block, path="/ui")
What this does

gr.mount_gradio_app registers a separate route (/ui) that serves the UI while still keeping the /get_current_temperature endpoint available for Ollama.

4. Running the combined server
uvicorn demo_mcp_paris_temperature:app --host 0.0.0.0 --port 1234
demo_mcp_paris_temperature.py – the file that contains all code above
The tool endpoint is now reachable at http://localhost:1234/get_current_temperature?location=Paris.
If you need to test it from a browser or via curl:
curl "http://localhost:1234/get_current_temperature?location=Paris"
Important – the Ollama endpoint (c_ollama_chat_endpoint) should point to this same URL, e.g.:

c_ollama_chat_endpoint = "http://localhost:1234/v1/chat/completions"  # unchanged
5. Example request payload (unchanged)
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

if __name__ == "__main__":
    import requests

    print("Test LM Studio invoke MCP tool with GRADIO")
    response = requests.post(c_ollama_chat_endpoint, json=l_payload)
    print(response.json())
6. Recap of key changes
What was wrong	How we fixed it
@tool_server.get on a gr.Blocks instance	Created a real FastAPI app (app) and used its .get decorator.
No UI when you wanted one	Optional Gradio UI mounted onto the same FastAPI process with gr.mount_gradio_app.
Potential confusion around async vs sync	Declared endpoint as async def, which is idiomatic for FastAPI.
With these adjustments your script will run, expose the tool to Ollama, and optionally provide a quick web interface for manual testing – all while keeping variable names clear and following the style guidelines you set out.

8.23 tok/sec

•

1766 tokens

•

11.41s to first token

•

Stop reason: EOS Token Found

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
Context: 1580 / 20000