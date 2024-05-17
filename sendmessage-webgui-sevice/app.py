from flask import Flask, render_template, request, redirect, url_for

app = Flask(__name__)


@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        phone = request.form['phone']
        code = request.form['code']
        # Здесь можно обработать данные из формы
        takedata = [phone, code]
        print(takedata)
        return render_template('form.html')
    return render_template('form.html')


if __name__ == '__main__':
    app.run(debug=True)
