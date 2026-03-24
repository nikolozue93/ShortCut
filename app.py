import subprocess
import platform
import os
from flask import Flask, render_template, send_from_directory
from flask_wtf import FlaskForm
from flask_wtf.file import FileField, FileAllowed, FileRequired
from wtforms import SelectField


def ensure_backend():
    """Detects OS and ensures the Huffman C++ executable exists."""
    is_windows = platform.system() == "Windows"
    exe_name = "Source.exe" if is_windows else "./source.out"
    
    # if the executable is missing, try to compile it automatically
    if not os.path.exists(exe_name.strip("./")): 
        print(f"--- Backend {exe_name} not found. Compiling source.cpp... ---")
        try:
            if is_windows:
                # compile for Windows (requires g++/MinGW in PATH)
                subprocess.run(["g++", "source.cpp", "-o", "Source.exe"], check=True)
            else:
                # compile for Linux/Mac
                subprocess.run(["g++", "source.cpp", "-o", "source.out"], check=True)
                os.chmod("source.out", 0o755) # ensure it has execution permissions
            print("--- Compilation Successful! ---")
        except Exception as e:
            print(f"--- ERROR: Could not compile C++ backend: {e} ---")
            print("--- Please ensure g++ is installed on your system. ---")
    
    return exe_name

# initialize the backend path globally
EXE_PATH = ensure_backend()

# --- FLASK APP CONFIGURATION ---

app = Flask(__name__)
app.config['SECRET_KEY'] = "NOT_A_SECRET"

class Form(FlaskForm):
    file = FileField('File', validators=[FileRequired(), FileAllowed(['txt', 'binary'], 'Text and binary files only!')])
    action = SelectField('Choose an action', choices=[('e', 'Compress'), ('d', 'Decompress')])

@app.route('/', methods=['GET', 'POST'])
def index():
    form = Form()

    if form.validate_on_submit():
        file = form.file.data
        action = form.action.data
        filename = file.filename
        file.save(filename)

        # remove extensions for the C++ logic
        filename_alt = filename.replace(".txt", "").replace(".binary", "")

        # ise the dynamically detected EXE_PATH
        try:
            subprocess.run([EXE_PATH, filename_alt, action], check=True)
        except Exception as e:
            return f"Runtime Error: Failed to execute backend. {e}"
        
        as_attachment = False
        if action == 'e':
            processed_filename = f'{filename_alt}.binary'
        else:
            processed_filename = f'{filename_alt}-decoded.txt'
            as_attachment = True

        return send_from_directory('.', processed_filename, as_attachment=as_attachment)
    
    return render_template('index.html', form=form)

if __name__ == '__main__':
    # allows running via 'python app.py' as well
    app.run(debug=True)