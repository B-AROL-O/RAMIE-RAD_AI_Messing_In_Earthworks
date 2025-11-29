#!/usr/bin/env python3
"""
Interface with Ollama using qwen3:4b-instruct + Gradio 6 UI

This script:
- Launches Ollama as a subprocess
- Queries the qwen3:4b-instruct model
- Hosts a Gradio webserver with a textbox for queries and a reply box
- Cleans up by terminating Ollama when the script exits

Requirements:
- Ollama installed locally
- qwen3:4b-instruct pulled via `ollama pull qwen3:4b-instruct`
- Python 3.12+ with requests and gradio installed (`pip install requests gradio`)
"""

import subprocess
import requests
import time
import atexit
import sys
import gradio as gr

OLLAMA_API_URL = "http://localhost:11434/api/generate"
MODEL_NAME = "qwen3:4b-instruct"

ollama_process = None

def start_ollama():
    """Start Ollama server as a subprocess."""
    global ollama_process
    try:
        ollama_process = subprocess.Popen(
            ["ollama", "serve"],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE
        )
        # Give Ollama a moment to spin up
        time.sleep(2)
        print("✅ Ollama server started.")
    except Exception as e:
        print(f"Failed to start Ollama: {e}")
        sys.exit(1)

def stop_ollama():
    """Terminate Ollama subprocess cleanly."""
    global ollama_process
    if ollama_process:
        ollama_process.terminate()
        try:
            ollama_process.wait(timeout=5)
        except subprocess.TimeoutExpired:
            ollama_process.kill()
        print("🛑 Ollama server stopped.")

def query_ollama(prompt: str) -> str:
    """Send a prompt to Ollama and return the model's response."""
    payload = {
        "model": MODEL_NAME,
        "prompt": prompt,
        "stream": False
    }
    try:
        response = requests.post(OLLAMA_API_URL, json=payload)
        response.raise_for_status()
        data = response.json()
        return data.get("response", "")
    except requests.exceptions.RequestException as e:
        return f"Error communicating with Ollama: {e}"

def main():
    start_ollama()
    atexit.register(stop_ollama)  # ensure cleanup on exit

    # Gradio interface
    iface = gr.Interface(
        fn=query_ollama,
        inputs=gr.Textbox(lines=3, placeholder="Enter your prompt here..."),
        outputs=gr.Textbox(lines=10, label="Model Response"),
        title="🧠 Ollama + Qwen3:4b-instruct",
        description="Query the local Ollama server running qwen3:4b-instruct"
    )

    iface.launch(server_name="0.0.0.0", server_port=7860)

if __name__ == "__main__":
    main()
