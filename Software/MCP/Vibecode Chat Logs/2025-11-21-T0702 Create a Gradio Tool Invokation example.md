## What an **MCP (Machine‑Computer‑Protocol) tool really is**

The term “MCP tool” comes from the *MCP* architecture used by many conversational AI systems today:

```
User  →  LLM   ↔  Tool API  ↔  External system
        ^          |
        |          v
      System     (your code)
```

* **LLM** – The language model that generates text.  
* **Tool API** – A small HTTP/JSON endpoint that the LLM can *call* during inference.  
  The LLM produces a JSON object describing which tool to invoke and with what arguments; the
  server receives that request, performs whatever computation is required, and returns a JSON
  response that the LLM then incorporates into its final answer.

In other words, an MCP tool is **not** a front‑end UI component.  
It is a *service* that the model talks to while it is “thinking.”  
When you run your own Ollama instance with function calling enabled, the model can
automatically generate calls like:

```json
{
  "name": "get_current_temperature",
  "arguments": {
    "location": "Paris"
  }
}
```

Your server receives that payload at a known URL (e.g. `http://localhost:7860/get_current_temperature`),
looks up the temperature, and returns something like:

```json
{
  "result": "18 °C",
  "confidence": 0.99
}
```

The model then stitches that response into its final reply.

---

## How to turn your Gradio MCP server into a *real* tool

Below is a minimal, production‑grade example that shows how to:

1. **Expose one or more tools as HTTP endpoints** (each with type‑hinted arguments).  
2. **Register those tools in an Ollama chat request** so the model can call them automatically.  
3. **Return the tool’s response to the LLM**.

> **Tip:** The code follows all of the developer guidelines you listed – snake_case, descriptive
> names, type hints, and clear docstrings.

### 1️⃣ Define the tool logic

```python
# ----------------------------------------------------------------------
#  MCP Tools (stand‑alone functions that can be invoked by an LLM)
# ----------------------------------------------------------------------


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
```

> **Why `dict` as return type?**  
> The LLM expects the tool to output JSON that can be parsed back into a Python dictionary
> (or any serialisable object). Returning a plain `str` would break the function‑calling contract.

### 2️⃣ Wrap the tools in a Gradio “tool server”

```python
# ----------------------------------------------------------------------
#  Tool Server – a tiny FastAPI app hosted inside Gradio Blocks
# ----------------------------------------------------------------------


import gradio as gr

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

# Note: The above route is automatically exposed at
#   http://localhost:<demo.server_port>/get_current_temperature
```

> **Why a Gradio `Blocks` app?**  
> Gradio already runs an ASGI server for you; we can piggy‑back our tool endpoints on it without
> pulling in FastAPI or Starlette explicitly.

### 3️⃣ Launch the MCP server *and* expose the tools

```python
# ----------------------------------------------------------------------
#  Combined launch – UI + Tool API
# ----------------------------------------------------------------------


def main() -> None:
    """
    Start the combined Gradio UI and tool API on port 7860.
    The LLM will be able to call `get_current_temperature` via HTTP GET.
    """
    with gr.Blocks() as demo_ui:
        # ... (your existing UI code goes here) ...
        pass

    # Launch the UI
    demo_ui.launch(
        mcp_server=True,   # this enables the tool endpoints automatically
        server_port=7860,
    )


if __name__ == "__main__":
    main()
```

> **How does `mcp_server=True` work?**  
> Gradio internally sets up a sub‑app that hosts all routes you defined with decorators
> (like our `/get_current_temperature`).  When the LLM sends an HTTP request to that URL, the
> request is routed directly to your Python function.

---

## 4️⃣ How the **LLM** calls the tool

Assume you’re running Ollama on `localhost:1234`.  
Ollama supports *function calling* – you send a list of “functions” in the chat payload.  
The model will then produce a JSON object with `name` and `arguments`.

### Example request to Ollama (Python)

```python
import json
import requests

c_ollama_chat_endpoint = "http://localhost:1234/api/chat"

# Tell Ollama about our available tool
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

response = requests.post(c_ollama_chat_endpoint, json=l_payload)
print(response.json())
```

### What the model will produce

If it decides to invoke your tool, the response might look like:

```json
{
  "role": "assistant",
  "content": null,
  "function_call": {
    "name": "get_current_temperature",
    "arguments": {"location":"Paris"}
  }
}
```

Your MCP server receives that *JSON body*, looks up the function name (`get_current_temperature`),
calls your Python implementation, and returns:

```json
{
  "role": "tool",
  "name": "get_current_temperature",
  "content": "{\"temperature\": \"18.0 °C\", \"confidence\": 0.97}"
}
```

The LLM then continues its inference using that content as part of the final answer.

---

## TL;DR

| Step | What you do | Why it matters |
|------|-------------|----------------|
| **1** | Implement a pure‑Python function (`get_current_temperature`). | This is the *logic* behind your tool. |
| **2** | Expose that function as an HTTP endpoint inside Gradio (or FastAPI). | The LLM will call this URL during inference. |
| **3** | Run `demo.launch(mcp_server=True)`. | Gradio automatically wires the endpoints to the MCP server. |
| **4** | Configure Ollama with a *function‑calling* request that includes your tool definition. | The model can now decide when to invoke your tool. |

With this setup, every time the LLM thinks it needs real data (or any external computation),
it will reach out to `http://localhost:7860/get_current_temperature`, receive JSON back,
and incorporate the answer into its response—exactly what an MCP *tool* is supposed to do!