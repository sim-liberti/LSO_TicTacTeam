# 🕹️ TicTacToe – Progetto LSO 2025

Applicazione client-server realizzata per il progetto di **Laboratorio di Sistemi Operativi (LSO)**  
📍 Università degli Studi di Napoli Federico II – Corso di laurea in Informatica  
📅 Anno Accademico 2024/2025

![Language](https://img.shields.io/badge/Server-C-blue) ![Language](https://img.shields.io/badge/Client-Python-yellow) ![Docker](https://img.shields.io/badge/Docker-Supported-blue)


## ✨ Funzionalità

- ✅ Comunicazione client-server tramite socket
- ✅ Scambio dati in formato JSON con [cJSON](https://github.com/DaveGamble/cJSON)
- ✅ GUI moderna per il client con [CustomTkinter](https://customtkinter.tomschimansky.com/)
- ✅ Server avviabile tramite Docker, Podman o Makefile
- ✅ Esecuzione multipiattaforma (Windows/Linux)

## 📁 Struttura del progetto

Il progetto è diviso in due macrocartelle:
- Server: scritto in C utilizzando le implementazioni Gnu/Linux delle socket, includendo la libreria esterna [cJSON](https://github.com/DaveGamble/cJSON) per convertire i messaggi da scambiare in formato JSON
- Client: implementato in Python utilizzando la libreria [customtkinter](https://customtkinter.tomschimansky.com/) e le implementazioni standard delle socket


## 🚀 Installazione ed Esecuzione
Per prima cosa, clona il repository:
```bash
git clone https://github.com/sim-liberti/LSO_TicTacTeam.git
cd LSO_TicTacTeam
```

### 🐳 Server – via Docker/Podman
#### Docker
```bash
docker build -t lso-tictacteam .
docker run -p 8080:8080 lso-tictacteam:latest
```
#### Podman
```bash
podman build -t lso-tictacteam .
podman run -p 8080:8080 lso-tictacteam:latest
```

### ⚙️ Server – via Makefile
In alternativa, puoi eseguire il server manualmente:
```bash
cd server
make
./build/server.out
```
📢 Se tutto è corretto, vedrai:
```bash
[INFO] Server is running on port 8080. Press Ctrl+C to stop.
```
---
### 🖥️ Client – Eseguibile pronto all’uso
Scarica l’eseguibile più recente dalla sezione [releases](https://github.com/sim-liberti/LSO_TicTacTeam/releases) del repository.
- `.exe` per Windows
- `.Appimage` per Linux

### 💻 Client – Esecuzione manuale
Assicurati di avere [Python ≥ 3.10](https://www.python.org/downloads/) e [pip](https://pypi.org/project/pip/) installati.

Crea un ambiente virtuale (opzionale ma consigliato)
```bash
pip install virtualenv
virtualenv .venv
```
Attiva l'ambiente
- Windows
```bash
.venv\scripts\activate
```

- Linux/macOS
```bash
source .venv/bin/activate
```

Installa le dipendenze
```bash
pip install -r Requirements.txt
```

Avvia il client
```bash
cd client
python main.py
```

## 🌐 Configurazione dell'host del server

Il client necessita dell'indirizzo IP del server per connettersi.

- 🖥️ **Server Docker sulla stessa macchina del client**  
  Inserisci l’indirizzo IP locale della macchina (es. `192.168.1.121`) nell’interfaccia del client (già presente di default).

- ⚙️ **Server compilato manualmente tramite Makefile sulla stessa macchina del client**  
  Puoi utilizzare `localhost:8080` o `127.0.0.1:8080`.

## 📌 Note
Se stai usando Windows, potresti dover disattivare l'antivirus o autorizzare manualmente il file `.exe` per avviarlo correttamente. \
Per saperne di più sul funzionamento del programma, puoi consultare la [documentazione](https://github.com/sim-liberti/LSO_TicTacTeam/blob/master/Docs.pdf).
