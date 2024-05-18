from flask import Flask, render_template, request, jsonify, redirect, url_for, flash, session
import requests
from functools import wraps

app = Flask(__name__)
app.secret_key = 'your_secret_key'
ADMIN_USERNAME = 'admin'
ADMIN_PASSWORD = 'password'


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
        phone = request.form['phone']
        code = request.form['code']
        api = request.form['apikey']
        take_data = [phone, code, api]
        print(take_data)
        return render_template('form.html')
    return render_template('form.html')


@app.route('/get-token', methods=['POST'])
def get_token():
    data = request.get_json()
    api = data.get('api')
    token = "sample_token"
    return jsonify({'token': token})


if __name__ == '__main__':
    app.run(debug=True)
