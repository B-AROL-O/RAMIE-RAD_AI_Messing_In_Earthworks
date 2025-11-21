import json
import requests
import gradio as gr

#   Computer: FW13
#   Application: LM Studio
#   Runtime: Vulkan llama.cpp
#   Model: Gemma 3 4B Q4 Vision Instruct
c_ollama_chat_endpoint = "http://localhost:1234/v1/chat/completions"

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

# ----------------------------------------------------------------------
#  Combined launch – UI + Tool API
# ----------------------------------------------------------------------

# ----------------------------------------------------------------------
#  Optional: minimal Gradio UI for local debugging
# ----------------------------------------------------------------------

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

def ui_get_temperature(i_city: str) -> tuple[str, float]:
    """
    Wrapper that calls the tool function and returns a user‑friendly string.
    """
    d_response = get_current_temperature(i_city)
    if "error" in d_response:
        return (d_response["error"], 0.0)
    return (d_response["temperature"], d_response["confidence"])

if __name__ == "__main__":
    
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



