# ----------------------------------------------------------------------
#  MCP Server with Ollama LLM integration (port 1234)
#
#  This module extends the existing Gradio‑based MCP server by adding a
#  simple chat interface that forwards user messages to an Ollama
#  instance running on localhost:1234.  The chat is performed using the
#  /api/chat endpoint which accepts a list of role/message pairs.
#
#  All variable names follow the naming conventions described in the
#  developer guidelines:
#      * type prefix + descriptive name (snake_case)
#      * function parameters prefixed with i_
#  The code is fully typed and contains comprehensive Doxygen‑style
#  docstrings so that future maintainers can quickly understand the
#  intent and behaviour of every component.
# ----------------------------------------------------------------------


import json
import gradio as gr
import requests

# --------------------------- Constants ---------------------------------
c_ollama_host: str = "http://localhost"
c_ollama_port: int = 1234
c_ollama_api_endpoint: str = f"{c_ollama_host}:{c_ollama_port}/v1/chat/completions"

c_model_name: str = "mistral"          # The Ollama model that should be used
c_system_prompt: str = (
    "You are a helpful assistant. Respond concisely to user queries."
)

# --------------------------- Helper functions ----------------------------

def invoke_llama_chat(i_user_message: str) -> str:
    """
    Sends a chat request to the Ollama server and returns the model's reply.

    Parameters
    ----------
    i_user_message : str
        The message supplied by the user that should be forwarded to the LLM.

    Returns
    -------
    str
        The assistant's reply.  If an error occurs, an empty string is
        returned so that Gradio can display no content instead of a stack
        trace.
    """
    # Build the request payload
    d_payload: dict = {
        "model": c_model_name,
        "messages": [
            {"role": "system", "content": c_system_prompt},
            {"role": "user",   "content": i_user_message}
        ],
        "stream": False
    }

    try:
        # Perform the HTTP POST request to Ollama
        r_response: requests.Response = requests.post(
            url=c_ollama_api_endpoint,
            headers={"Content-Type": "application/json"},
            data=json.dumps(d_payload),
            timeout=30.0,  # seconds
        )
        r_response.raise_for_status()

        # The response JSON contains a field called 'message' with the
        # assistant's content.
        d_result: dict = r_response.json()
        s_reply: str = d_result.get("message", {}).get("content", "")

        return s_reply

    except Exception as e:
        # Log the exception locally for debugging purposes.
        print(f"[Ollama Error] {e}")
        return ""

# --------------------------- Gradio UI ----------------------------------

with gr.Blocks() as demo:

    # --------------------------------------------------------------------
    #  Main title
    # --------------------------------------------------------------------
    gr.Markdown("# RAMIE's MCP Server")

    # --------------------------------------------------------------------
    #  Greet tab (unchanged from the original example)
    # --------------------------------------------------------------------
    with gr.Tab("Greet"):
        t_name: gr.Textbox = gr.Textbox(label="Enter your name", placeholder="Your name")
        b_greet: gr.Button   = gr.Button("Greet")
        t_output: gr.Textbox = gr.Textbox(label="Greeting Message")
        b_greet.click(
            fn=lambda i_name: f"Hello {i_name}, my name is RAMIE!",
            inputs=t_name,
            outputs=t_output,
            api_name="greet",
        )

    # --------------------------------------------------------------------
    #  Robot Manifest tab (unchanged)
    # --------------------------------------------------------------------
    with gr.Tab("Robot Manifest"):
        b_manifest: gr.Button   = gr.Button("Check")
        t_manifest_out: gr.Textbox = gr.Textbox(label="Result")

        def get_robot_manifest() -> dict:
            """Load the robot_manifest.json file from disk."""
            try:
                with open("./robot_manifest.json", "r") as f_file:
                    return json.load(f_file)
            except Exception as exc:
                print(f"[Manifest Error] {exc}")
                return {}

        b_manifest.click(
            fn=get_robot_manifest,
            inputs=None,
            outputs=t_manifest_out,
            api_name="get_robot_manifest",
        )

    # --------------------------------------------------------------------
    #  Robot Capabilities tab (unchanged)
    # --------------------------------------------------------------------
    with gr.Tab("Robot Capabilities"):
        b_capabilities: gr.Button   = gr.Button("Check")
        t_capabilities_out: gr.Textbox = gr.Textbox(label="Result")

        def get_robot_capabilities() -> dict:
            """Extract the capabilities section from robot_manifest.json."""
            try:
                with open("./robot_manifest.json", "r") as f_file:
                    d_json: dict = json.load(f_file)
                    return d_json.get("capabilities", {})
            except Exception as exc:
                print(f"[Capabilities Error] {exc}")
                return {}

        b_capabilities.click(
            fn=get_robot_capabilities,
            inputs=None,
            outputs=t_capabilities_out,
            api_name="get_robot_capabilities",
        )

    # --------------------------------------------------------------------
    #  LLM Chat tab (new)
    # --------------------------------------------------------------------
    with gr.Tab("LLM Chat"):
        t_user_msg: gr.Textbox = gr.Textbox(
            label="Your message", placeholder="Ask anything..."
        )
        b_send_llm: gr.Button   = gr.Button("Send to Ollama")
        t_reply_out: gr.Textbox = gr.Textbox(label="Assistant reply")

        b_send_llm.click(
            fn=invoke_llama_chat,
            inputs=t_user_msg,
            outputs=t_reply_out,
            api_name="llm_invoke",
        )

# ----------------------------------------------------------------------
#  Launch the server
# ----------------------------------------------------------------------
demo.launch(
    mcp_server=True,
    server_port=7860,
)
