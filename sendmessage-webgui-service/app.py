from flask import Flask, render_template, request, jsonify, redirect, url_for, flash, session
from functools import wraps

import HTTPClient

app = Flask(__name__)
app.secret_key = 'your_secret_key'
ADMIN_USERNAME = 'admin'
ADMIN_PASSWORD = 'password'

# записываем сюда все ответы
log = []

RESPONSES = {
    401: 'Вы не авторизованы',
    402: 'Недостаточно средств',
    404: 'Страница не найдена',
    200: 'Все хорошо',
    500: 'Ошибка сервера',
    330: 'Номер телефона не распознан'
}


def login_required(f):
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if 'logged_in' not in session:
            flash('Вам нужно войти, чтобы получить доступ к этой странице.')
            return redirect(url_for('login'))
        return f(*args, **kwargs)

    return decorated_function


@app.route('/', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        if username == ADMIN_USERNAME and password == ADMIN_PASSWORD:
            session['logged_in'] = True
            return redirect(url_for('admin_panel'))
        else:
            flash('Неправильные учетные данные. Попробуйте снова.')
    return render_template('login.html')


@app.route('/admin-panel', methods=['GET', 'POST'])
@login_required
def admin_panel():
    if request.method == 'POST':
        # считывание из формы
        phone = '7' + request.form['clean_phone']
        code = request.form['code']
        api = request.form['apikey']
        # take_data = [phone, code, api]
        # запрос на отправку данных
        r, time = HTTPClient.client.send_data(api, phone, code)
        code = int(r.status_code)
        if code in RESPONSES:
            log.append([RESPONSES[code], time])
            print(log)
        else:
            print(f'Код не нашел {code}')

        return render_template('form.html', log=log)
    return render_template('form.html')


# получение токена от сервера
@app.route('/get-token', methods=['POST'])
def get_token():
    # data = request.get_json() не понятно пока куда это
    # api = data.get('api')
    token = HTTPClient.client.get_token()
    return jsonify({'token': token})


if __name__ == '__main__':
    app.run(debug=True)
