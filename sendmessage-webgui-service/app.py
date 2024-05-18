from flask import Flask, render_template, request, jsonify, redirect, url_for, flash, session
from functools import wraps
import datetime
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


def add_record_to_file(filename, record, encoding='windows-1251'):
    try:
        with open(filename, 'a') as file:  # Открываем файл в режиме добавления (append)
            file.write(record)  # Добавляем запись в файл с новой строки
    except Exception as e:
        print(f"Произошла ошибка при добавлении записи в файл: {e}")


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
    global time_start, log_text
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        time_start = datetime.datetime.now()
        if username == ADMIN_USERNAME and password == ADMIN_PASSWORD:
            session['logged_in'] = True
            log_text = f'\n \nСЕССИЯ пользователя {username} \nВремя начала: {time_start} \nСтатус: упешный вход \n'
            add_record_to_file('log-data/session_log.txt', log_text)
            return redirect(url_for('admin_panel'))
        else:
            flash('Неправильные учетные данные. Попробуйте снова.')
            log_text = f'\n \nСЕССИЯ пользователя: {username} \nВремя начала: {time_start} \nСтатус: неуспешный вход \nВремя конца {datetime.datetime.now()}'
            add_record_to_file('log-data/session_log.txt', log_text)
    return render_template('login.html')


@app.route('/admin-panel', methods=['GET', 'POST'])
@login_required
def admin_panel():
    global log_text
    if request.method == 'POST':
        # считывание из формы
        phone = request.form['phone']
        code = request.form['code']
        api = request.form['apikey']
        log_text = f'Действие: пользователь отправил данные -- телефон: {phone}, сообщение: {code}, api: {api} \n'
        add_record_to_file('log-data/session_log.txt', log_text)
        # take_data = [phone, code, api]

        # запрос на отправку данных
        r, time = HTTPClient.client.send_data(api, phone, code)
        code = int(r.status_code)
        if code in RESPONSES:
            log.append([RESPONSES[code], time])
            log_text = f'Ответ от сервера: {log[0][0]}\nКонец сессии: {time}'
            add_record_to_file('log-data/session_log.txt', log_text)
        else:
            print(f'Код не нашел {code}')

        return render_template('form.html', log=log)
    return render_template('form.html')


# получение токена от сервера
@app.route('/get-token', methods=['POST'])
def get_token():
    global log_text
    # data = request.get_json() не понятно пока куда это
    # api = data.get('api')
    token = HTTPClient.client.get_token()
    log_text = f'Действие: пользователь запросил токен \nДействие: пользователь получил токен {token}\n'
    add_record_to_file('log-data/session_log.txt', log_text)
    return jsonify({'token': token})


if __name__ == '__main__':
    app.run(debug=True)
