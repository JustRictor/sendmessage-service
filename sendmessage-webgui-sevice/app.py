from flask import Flask
from flask import render_template

app = Flask(__name__, template_folder='templates', static_folder='static')


@app.route('/')
def hello():
    return render_template('form.html')


app.run(
    debug=True)  # debug true задаем специально для разработки (в данном случае при обновление/изменение кода приложение автоматически само обновит данные на сайте)
