import subprocess
import os
import time
from flask import Flask
from flask import render_template, redirect, url_for

from flask_wtf import FlaskForm
from flask_wtf.file import FileField, FileAllowed, FileRequired
from wtforms import SelectField


app = Flask(__name__)
app.config['SECRET_KEY'] = "NOT_A_SECRET"


class Form(FlaskForm):
    file = FileField('File', validators=[FileRequired(), FileAllowed(['txt'], 'Text files only!')])
    action = SelectField('Choose an action', choices=[('e', 'Compress'), ('d', 'Decompress')])


@app.route('/', methods=['GET', 'POST'])
def index():
    form = Form()

    if form.validate_on_submit():
        file = form.file.data
        action = form.action.data
        filename = f"{action}_{file.filename}"

        with open(filename, "wb") as f:
            f.write(file.read())

        subprocess.run(["Source.exe"])
        
        return redirect(url_for('index'))

    return render_template('index.html', form=form)


if __name__ == '__main__':
    app.run()
