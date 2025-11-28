FROM ubuntu:latest

RUN apt-get update \
    && apt-get upgrade -y \
    && apt-get install -y build-essential python3 python3-venv curl git

RUN curl -fsSL https://ollama.com/install.sh | sh

WORKDIR /app
COPY . .

RUN python3 -m venv .venv
RUN .venv/bin/pip install .

CMD [".venv/bin/uvicorn", "main:app", "--host", "0.0.0.0", "--port", "3000"]
