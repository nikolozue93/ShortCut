#!/bin/bash

echo "--- Starting Huffman Tool Setup ---"

# kill any zombie processes on port 5000
fuser -k 5000/tcp 2>/dev/null

# setup virtual environment
if [ ! -d ".venv" ]; then
    echo "Creating virtual environment..."
    python3 -m venv .venv
fi

source .venv/bin/activate

# install dependencies
echo "Installing Python dependencies..."
pip install -r requirements.txt

# run the application
echo "Launching Huffman Tool on http://127.0.0.1:5000"
python3 app.py