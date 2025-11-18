import gradio as gr
import json


def greet(name):
    """
    Greets the user with a customized greeting message.

    Args:
        name (str): The name of the user to greet.
    Returns:
        str: A greeting message addressed to the specified user.
    """
    return "Hello " + name + ", my name is RAMIE!"


def get_robot_manifest():
    """
    Reads and returns the json manifest file that describes the robot
    and its capabilities.

    Args:
        None
    Returns:
        str: a stringified json object that contains the manifest file content.
            - Failure: returns empty object string
    """

    try:
        with open("./robot_manifest.json", "r") as f:
            return json.load(f)
    except:
        return '{}'


def get_robot_capabilities():
    """
    Reads and returns the robot capabilities that are described inside
    the json manifest file.

    Args:
        None
    Returns:
        str: a stringified json object that contains the robot capabilities.
    """
    try:
        with open("./robot_manifest.json", "r") as f:
            res = json.load(f)
            return res["capabilities"]
    except:
        return '{}'


with gr.Blocks() as demo:
    gr.Markdown("RAMIE's MCP Server")
    with gr.Tab("Greet"):
        name = gr.Textbox(label="Enter your name", placeholder="Your name")
        greet_btn = gr.Button("Greet")
        output = gr.Textbox(label="Greeting Message")
        greet_btn.click(fn=greet, inputs=name,
                        outputs=output, api_name="greet")

    with gr.Tab("Robot Manifest"):
        greet_btn = gr.Button("Check")
        output = gr.Textbox(label="Result")
        greet_btn.click(fn=get_robot_manifest, inputs=None,
                        outputs=output, api_name="get_robot_manifest")

    with gr.Tab("Robot Capabilities"):
        greet_btn = gr.Button("Check")
        output = gr.Textbox(label="Result")
        greet_btn.click(fn=get_robot_capabilities, inputs=None,
                        outputs=output, api_name="get_robot_capabilities")

demo.launch(
    mcp_server=True,
    server_port=7860
)
