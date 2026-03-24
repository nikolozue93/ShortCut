import subprocess
import os

if __name__ == "__main__":
    subprocess.run(['Source.exe'], check=True, timeout=60)
    # os.startfile('Source.exe')
