import json
from typing import Dict, Tuple

import requests  # pip install requests
import gradio as gr  # pip install gradio


# ----------------------------------------------------------------------
# Configuration constants --------------------------------------------------
# ----------------------------------------------------------------------
c_ollama_chat_endpoint: str = "http://localhost:1234/v1/chat/completions"

c_model_name: str = "qwen/qwen3-vl-4b"      # the real model you have locally
c_tool_name: str   = "get_current_temperature"


# ----------------------------------------------------------------------
# Tool implementation ----------------------------------------------------
# ----------------------------------------------------------------------
def get_current_temperature(
    i_location: str,
) -> Dict[str, str | float]:
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
# Helper that calls the tool and returns a user‑friendly string -------
# ----------------------------------------------------------------------
def ui_get_temperature(i_city: str) -> Tuple[str, float]:
    """
    Wrapper that calls the tool function and returns a user‑friendly
    temperature string together with a confidence value.

    Parameters
    ----------
    i_city : str
        City name supplied by the user.

    Returns
    -------
    tuple(str, float)
        Human‑readable temperature string and confidence score.
    """
    d_response = get_current_temperature(i_city)

    if "error" in d_response:
        return (d_response["error"], 0.0)

    return (
        d_response["temperature"],
        d_response["confidence"],
    )


# ----------------------------------------------------------------------
# Build the tool definition that will be sent to the LLM ----------------
# ----------------------------------------------------------------------
l_tool_definition: Dict = {
    "type": "function",
    "function": {
        "name": c_tool_name,
        "description": "Return the current temperature for a given city.",
        "parameters": {
            "type": "object",
            "properties": {"location": {"type": "string"}},
            "required": ["location"],
        },
    },
}


# ----------------------------------------------------------------------
# Build the payload that will be POSTed to the model -------------------
# ----------------------------------------------------------------------
l_payload: Dict = {
    "model": c_model_name,
    "messages": [
        # The assistant is told explicitly that it may use tools
        {"role": "system", "content": "You can call get_current_temperature when needed."},
        {"role": "user",   "content": f"What’s the temperature in Paris?"},
    ],
    "tools": [l_tool_definition],          # <-- note: **tools**, not functions
    "tool_choice": c_tool_name,           # force the model to pick this tool if it wants to
    "stream": False,
}


# ----------------------------------------------------------------------
# Gradio UI (optional) --------------------------------------------------
# ----------------------------------------------------------------------
def run_gradio_ui() -> None:
    """
    Starts a tiny Gradio UI that lets you type a city name and shows
    the temperature returned by our local tool implementation.
    """
    with gr.Blocks() as ui_block:
        txt_input = gr.Textbox(label="City name")
        btn_query = gr.Button("Get temperature")
        txt_output = gr.Textbox(label="Result", interactive=False)

        def on_click(i_city: str) -> None:
            s_temp, f_conf = ui_get_temperature(i_city)
            txt_output.value = f"{s_temp} (confidence {f_conf:.2%})"

        btn_query.click(on_click, inputs=txt_input, outputs=None)

    ui_block.launch(debug=True)