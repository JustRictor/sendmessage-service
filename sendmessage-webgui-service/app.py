from flask import Flask, render_template, request, jsonify, redirect, url_for, flash, session
from functools import wraps

import datetime
import HTTPClient
import Logger

app = Flask(__name__)
app.secret_key = 'your_secret_key'
TOKEN_ACCESS = '24d8389ec9fd8a2dbacd672122d13fbf112e01b0a164a993f5d46cf62bfcf0ad'
# записываем сюда все ответы
log = []

username = ''

RESPONSES = {
    400: 'Токен не верен',
    401: 'Вы не авторизованы',
    402: 'Недостаточно средств',
    404: 'Страница не найдена',
    200: 'ОК',
    202: 'Принято',
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
    Logger.logger.write_log(f'поключился неизвестный пользователь {request.remote_addr}')
    if request.method == 'POST':
        tokenname = request.form['tokenname']
        if tokenname == TOKEN_ACCESS:
            session['logged_in'] = True
            Logger.logger.write_log(f'пользователь авторизовался {request.remote_addr} login:{username}')
            return redirect(url_for('admin_panel'))
        else:
            Logger.logger.write_log(f'произведена попытка входа {request.remote_addr} login:{username}')
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
        Logger.logger.write_log(f'отправлен запрос login:{username} {api} {phone} {code}')
        if code in RESPONSES:
            log.append([RESPONSES[code], time])
            print(r.content)
            Logger.logger.write_log(f'получен ответ: {r.content}')
        else:
            Logger.logger.write_log(f'получен не обрабатываемый ответ: {code}')

        return render_template('form.html', log=log)
    return render_template('form.html')


# получение токена от сервера
@app.route('/get-token', methods=['POST'])
def get_token():
    Logger.logger.write_log(f'отправлен запрос на получение токена login:{username}')
    token = HTTPClient.client.get_token()
    Logger.logger.write_log(f'получен токен: {token}')
    return jsonify({'token': token})

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
