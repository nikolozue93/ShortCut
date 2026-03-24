import subprocess
from flask import Flask
from flask import render_template, redirect, url_for, send_from_directory

from flask_wtf import FlaskForm
from flask_wtf.file import FileField, FileAllowed, FileRequired
from wtforms import SelectField


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

        filename_alt = filename.replace(".txt", "").replace(".binary", "")

        subprocess.run(["Source.exe", filename_alt, action], check=True)

        as_attachment = False
        if action == 'e':
            processed_filename = f'{filename_alt}.binary'
        else:
            processed_filename = f'{filename_alt}-decoded.txt'
            as_attachment = True


        return send_from_directory('.', processed_filename, as_attachment=as_attachment)
    else:
        print(form.errors)

    return render_template('index.html', form=form)


if __name__ == '__main__':
    app.run()
