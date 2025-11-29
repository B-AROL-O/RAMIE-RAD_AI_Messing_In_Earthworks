# INSTALL AND CONFIGURE SBC

## Luckfox PicoKVM

I always have trouble accessing SBC headless, I finally got a 49€ KVM to mitigate the issue and free myself from using discrete keyboard and mouse

[Luckfox PicoKVM Instructions](/Software/Luckfox%20PicoKVM)

![](/Software/Luckfox%20PicoKVM/Images/2025-11-22_11_11_IMG_20251122_111122.jpg)

## INSTALL UBUNTU

Install Ubuntu 24.04 Server LTS on the SBC and configure network and storage

[Ubuntu 24.04 Server Install Instructions](/Software/Ubuntu%2024_04%20server%20LTS)

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1311%20Test%20ping%20dev%20drive%20network.png)

### TTYS0 SSH PUTTY SERIAL

Configure the GRUB to use ttyS0 as UART serial interface to let the host connect via putty via USB to UART FTDI adapter

[TTYS0 SERIAL GRUB CONFIGURATION](/Software/ttys0%20SSH)

![](/Software/ttys0%20SSH/Images/2025-11-22T1349%20Putty%20Configuration.png)

# SBC DEVELOPMENT

## VS Code Remote

Use VS code on host machine with remote SSH plugin to evelop and run code directly on the SBC via WiFi radio

[VS Code Remote Instructions](/Software/VS%20Code%20Remote%20Development%20on%20SBC)

![](/Software/VS%20Code%20Remote%20Development%20on%20SBC/Images/2025-11-23-T0956%20Connected%20to%20SBC%20Home.png)

## PYTHON - COMMUNICATE WITH ELECTRONICS




# OLLAMA

## Install Ollama

```
curl -fsSL https://ollama.com/install.sh | sh

ollama pull qwen3:4b

ollama pull qwen3:4b-instruct

ollama run qwen3:4b "What's the height of the Tour Eiffel in meters?" --verbose

ollama run qwen3:4b-instruct "What's the height of the Tour Eiffel in meters?" --verbose
```

[Qwen 3 4B Tool](https://ollama.com/library/qwen3:4b)

[Qwen 3 4B Vision Tool](https://ollama.com/library/qwen3-vl:4b)





<details>
  <summary>Ollama Install Log</summary>

```sh
sona@lpmu8gb:~$ curl -fsSL https://ollama.com/install.sh | sh
>>> Installing ollama to /usr/local
[sudo] password for sona:
>>> Downloading Linux amd64 bundle
######################################################################## 100.0%
>>> Creating ollama user...
>>> Adding ollama user to render group...
>>> Adding ollama user to video group...
>>> Adding current user to ollama group...
>>> Creating ollama systemd service...
>>> Enabling and starting ollama service...
Created symlink /etc/systemd/system/default.target.wants/ollama.service → /etc/systemd/system/ollama.service.
>>> The Ollama API is now available at 127.0.0.1:11434.
>>> Install complete. Run "ollama" from the command line.
WARNING: No NVIDIA/AMD GPU detected. Ollama will run in CPU-only mode.
  ```
  
</details>

<details>
  <summary>Ollama Run Inference</summary>

```sh
sona@lpmu8gb:~$ ollama run qwen3:4b-instructollama run qwen3:4b-instruct "What's the height of the Tour Eiffel in meters?" --verbose^C                                                                                              ollama run qwen3:4b-instruct "What's the height of the Tour Eiffel in meters?" --verbose the height of the Tour Eiffel in meters?" --verbose
The Eiffel Tower is 300 meters tall. This includes the tower structure and
the antenna on top. The original structure of the tower is about 300
meters tall, but the total height with the antenna is approximately 330
meters. However, the commonly cited height of the Eiffel Tower is **300
meters** when referring to the main structure, and **330 meters** when
including the antenna.

So, to clarify:

- **Height of the Eiffel Tower (main structure)**: **300 meters**
- **Total height (including antenna)**: **330 meters**

Therefore, the height of the Eiffel Tower is typically stated as **300
meters** for the tower itself. ✅

total duration:       18.293521648s
load duration:        97.885315ms
prompt eval count:    21 token(s)
prompt eval duration: 1.346030951s
prompt eval rate:     15.60 tokens/s
eval count:           160 token(s)
eval duration:        16.774703785s
eval rate:            9.54 tokens/s
  ```
  
</details>


<details>
  <summary>HTOP Durin Ollama</summary>

```sh
    0[###########################97.3%] Tasks: 47, 132 thr, 105 kthr; 4 runnin
    1[###########################99.3%] Load average: 1.95 0.57 0.39
    2[###########################99.3%] Uptime: 07:20:04
    3[###########################96.7%]
  Mem[|||||||||||||||||#@$$3.94G/7.47G]
  Swp[|#                    756K/4.00G]

  [Main] [I/O]
    PID USER       PRI  NI  VIRT   RES   SHR S  CPU%▽MEM%   TIME+  Command
   6322 ollama      20   0 5241M 3362M 25216 S  46.4 44.0  0:09.44 /usr/local/bi
   6325 ollama      20   0 5241M 3362M 25216 R  30.5 44.0  0:01.09 /usr/local/bi
   6324 ollama      20   0 5241M 3362M 25216 S  22.5 44.0  0:06.79 /usr/local/bi
   7353 ollama      20   0 5241M 3362M 25216 R   6.0 44.0  0:00.09 /usr/local/bi
   7352 ollama      20   0 5241M 3362M 25216 R   5.3 44.0  0:00.08 /usr/local/bi
   7354 ollama      20   0 5241M 3362M 25216 R   5.3 44.0  0:00.08 /usr/local/bi
   7068 sona        20   0  8348  4608  3712 R   1.3  0.1  0:00.09 htop
   5544 sona        20   0 1329M 77564 48896 S   0.7  1.0  0:02.18 /home/sona/.v
   5715 sona        20   0 31.8G  145M 56576 S   0.7  1.9  0:04.04 /home/sona/.v
   6329 ollama      20   0 5241M 3362M 25216 S   0.7 44.0  0:10.99 /usr/local/bi
      1 root        20   0 22748 13220  9124 S   0.0  0.2  0:04.41 /sbin/init
    325 root        19  -1 50504 16100 15076 S   0.0  0.2  0:00.63 /usr/lib/syst
    382 root        RT   0  282M 27008  8576 S   0.0  0.3  0:00.87 /sbin/multipa
F1Help  F2Setup F3SearchF4FilterF5Tree  F6SortByF7Nice -F8Nice +F9Kill  F10Quit
  ```
  
</details>


```python
#!/usr/bin/env python3
"""
Interface with Ollama using qwen3:4b-instruct

This script:
- Launches Ollama as a subprocess
- Queries the qwen3:4b-instruct model
- Cleans up by terminating Ollama when the script exits

Requirements:
- Ollama installed locally
- qwen3:4b-instruct pulled via `ollama pull qwen3:4b-instruct`
- Python 3.12+ with requests installed (`pip install requests`)
"""

import subprocess
import requests
import time
import atexit
import sys

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

    user_prompt = "Explain the difference between supervised and unsupervised learning."
    print(f"Querying {MODEL_NAME}...\n")
    answer = query_ollama(user_prompt)
    print("Response:\n", answer)

if __name__ == "__main__":
    main()


```


<details>
  <summary>PYTHON SCRIPT</summary>

```sh
uv run demo_ollama.py 
Using CPython 3.14.0
Creating virtual environment at: .venv
Installed 5 packages in 12ms
✅ Ollama server started.
Querying qwen3:4b-instruct...

Response:
 Certainly! The main difference between **supervised** and **unsupervised learning** lies in how the data is structured and how the model is trained.

---

### 🔹 Supervised Learning

**Definition:**  
In supervised learning, the model is trained on labeled data — that is, data where each input is paired with a known output or target.

**How it works:**  
The algorithm learns a mapping from inputs to outputs by observing examples of input-output pairs. Once trained, it can make predictions on new, unseen data.

**Example:**  
Predicting house prices based on features like size, location, number of bedrooms, etc.  
- Input: Size, location, number of bedrooms  
- Output: Price (known and labeled)

**Common Algorithms:**  
- Linear Regression  
- Decision Trees  
- Support Vector Machines (SVM)  
- Neural Networks  

**Use Cases:**  
- Classification (e.g., spam detection)  
- Regression (e.g., predicting sales)

---

### 🔹 Unsupervised Learning

**Definition:**  
In unsupervised learning, the model is trained on data without labeled outputs. The algorithm must find patterns, structures, or relationships in the data on its own.

**How it works:**  
There is no target variable to learn. Instead, the model discovers hidden patterns, groupings, or dimensions in the data.

**Example:**  
Clustering customers into groups based on purchasing behavior.  
- Input: Purchase history  
- Output: No known label — the model discovers natural groupings

**Common Algorithms:**  
- K-Means Clustering  
- Principal Component Analysis (PCA)  
- Hierarchical Clustering  
- Autoencoders  

**Use Cases:**  
- Customer segmentation  
- Anomaly detection  
- Dimensionality reduction  
- Data compression

---

### 🔍 Key Difference Summary:

| Feature                     | Supervised Learning                        | Unsupervised Learning                     |
|---------------------------|--------------------------------------------|------------------------------------------|
| Labeled data?             | Yes (input-output pairs)                  | No (only inputs)                        |
| Goal                      | Predict known outputs                    | Discover hidden patterns or structure  |
| Training objective        | Minimize prediction error              | Maximize data structure discovery      |
| Example use               | Spam detection, price prediction       | Customer segmentation, anomaly detection |

---

### 📝 In short:
- **Supervised learning** is like teaching a student with correct answers — you show them a question and the right answer, and they learn to respond correctly.
- **Unsupervised learning** is like letting a student explore a room without being told what's in it — they discover patterns and groupings on their own.

Both are essential in machine learning and are used depending on the nature of the problem and the availability of labeled data.
🛑 Ollama server stopped.
  ```
  
</details>


## (OLD) CONFIGURE NETWORK

For reasons I do not understand the network interfaces change name and stop working...

Powered

![](/Software/Image/2025-11-19-T1622_ramie_powered.jpg)

I had to plug screen and keyboard

![](/Software/Image/2025-11-19-T1620_configure_network.jpg)

The trick is to add a line to ```/etc/netplan/50-cloud-init.yaml```

```
sona@lpn10016gb:~$ ip a
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group defaul                                                    t qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host noprefixroute
       valid_lft forever preferred_lft forever
2: enp3s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP gr                                                    oup default qlen 1000
    link/ether d2:f7:f7:c0:0c:13 brd ff:ff:ff:ff:ff:ff
    inet 192.168.1.240/23 metric 100 brd 192.168.1.255 scope global dynamic enp3                                                    s0
       valid_lft 99850sec preferred_lft 99850sec
    inet6 fe80::d0f7:f7ff:fec0:c13/64 scope link
       valid_lft forever preferred_lft forever
3: wlp2s0: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN group default ql                                                    en 1000
    link/ether 10:b1:df:bd:b3:b9 brd ff:ff:ff:ff:ff:ff
sona@lpn10016gb:~$ sudo cat /etc/netplan/50-cloud-init.yaml
[sudo] password for sona:
network:
  version: 2
  ethernets:
    enp3s0:
      dhcp4: true
```

# CONFIGURE WIFI

```
sona@lpn10016gb:~$ ip a
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host noprefixroute
       valid_lft forever preferred_lft forever
2: enp3s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP group default qlen 1000
    link/ether d2:f7:f7:c0:0c:13 brd ff:ff:ff:ff:ff:ff
    inet 192.168.1.240/23 metric 100 brd 192.168.1.255 scope global dynamic enp3s0
       valid_lft 97509sec preferred_lft 97509sec
    inet6 fe80::d0f7:f7ff:fec0:c13/64 scope link
       valid_lft forever preferred_lft forever
3: wlp2s0: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN group default qlen 1000
    link/ether 10:b1:df:bd:b3:b9 brd ff:ff:ff:ff:ff:ff
sona@lpn10016gb:~$ ^C
sona@lpn10016gb:~$ ls
test_microphone
sona@lpn10016gb:~$ ^C
sona@lpn10016gb:~$ sudo nano /etc/netplan/50-cloud-init.yaml
[sudo] password for sona:
sona@lpn10016gb:~$ sudo cat /etc/netplan/50-cloud-init.yaml
network:
  version: 2
  ethernets:
    enp3s0:
      dhcp4: true
  wifis:
    wlp2s0:
      dhcp4: true
      access-points:
        "xxx":
          password: "yyy"
```

Doesn't work, so I search for debug

```
sudo netplan --debug generate

```

Apply without rebooting

```
sudo netplan apply
sudo systemctl restart NetworkManager
```

Of course it bricked the network -.-


