import os
import json
from flask import Flask, request, redirect, url_for, render_template
from werkzeug import secure_filename

UPLOAD_FOLDER = './images/'
ALLOWED_EXTENSIONS = set(['txt', 'png', 'jpg'])

app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

def allowed_file(filename):
    return '.' in filename and \
           filename.rsplit('.', 1)[1] in ALLOWED_EXTENSIONS


@app.route("/result", methods=['GET'])
def result():
    result = json.dumps({'Miller Lite': {'Given Price': '13.49', 'Recomended Price': '13.49'}, 'BUD LITE PLATINUM' : {'Given Price': '10.49', 'Recomended Price': '10.49'}, 'CORONA EXTRA': {'Given Price': '12.99', 'Recomended Price': '12.99'}})
    return result

@app.route("/", methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        file = request.files['file']
        if file and allowed_file(file.filename):
            filename = secure_filename(file.filename)
            file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
            return redirect(url_for('result'))
    return render_template("index.html")

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5001, debug=True)
