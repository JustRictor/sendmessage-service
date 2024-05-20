from flask import Flask, render_template, request, jsonify, redirect, url_for, flash, session
from functools import wraps

import HTTPClient
import Logger
import os
app = Flask(__name__)


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
    Logger.logger.write_log(f'подключился неизвестный пользователь {request.remote_addr}')

    if request.method == 'POST':
        tokenname = request.form['tokenname']
        r = HTTPClient.client.pin_pong(tokenname)

        print(r.status_code)
        if r.status_code == 200:
            session['logged_in'] = True
            Logger.logger.write_log(f'пользователь авторизовался {request.remote_addr} login:{username}')
            HTTPClient.client.set_admin_token(tokenname)
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
        r, time = HTTPClient.client.send_data(phone, code)
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
@app.route('/create-token', methods=['GET'])
def create_token():
    Logger.logger.write_log(f'отправлен запрос на получение токена login:{username}')
    token = HTTPClient.client.create_token()
    Logger.logger.write_log(f'создан токен: {token}')
    return token, 200


@app.route('/delete-token', methods=['POST'])
def delete_token():
    data = request.get_json()
    token_to_delete = data.get("apikey")
    print(token_to_delete)
    Logger.logger.write_log(f'отправлен запрос на удаление токена login:{username}')
    result = HTTPClient.client.delete_token(token_to_delete)

    if result.status_code == 200:
        return token_to_delete, 200
    else:
        return RESPONSES[result.status_code], 200


@app.route('/get-tokens', methods=['GET'])
def get_tokens():
    Logger.logger.write_log(f'отправлен запрос на все токены login:{username}')
    tokens = HTTPClient.client.get_tokens()
    Logger.logger.write_log(f'токены получены')
    return jsonify(tokens.text), 200


if __name__ == '__main__':

    directory = 'log-data'
    file_path = os.path.join(directory, 'session_log.txt')

    # Чекаем есть ли папка
    if not os.path.exists(directory):
        os.makedirs(directory)

    # Чекаем есть ли файл
    if not os.path.isfile(file_path):
        with open(file_path, 'w') as file:
            file.write('')  # создаем пустой лог

    app.secret_key = 'super secret key'
    app.run(host='0.0.0.0', port=5000, debug=True)

