from flask import Flask, render_template, request, jsonify
import requests

app = Flask(__name__)


@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        phone = request.form['phone']  # Чтение из инпута телефона в переменную
        code = request.form['code']  # Чтение из инпута кода в переменную
        api = request.form['apikey']  # Чтение из инпута апи ключа в переменную
        # Здесь можно обработать данные из формы
        take_data = [phone, code, api]  # Запись все, что прочитали в список
        print(take_data)
        return render_template('form.html')  # Очистка формы
    return render_template('form.html')


@app.route('/get-token', methods=['POST'])
def get_token():
    data = request.get_json()
    api = data.get('api')
    # response = requests.get('http://docker-sendmessage-service:5000/get-api')
    # api_from_service = response.json().get('api')
    token = "sample_token"  # Заглушка для токена
    return jsonify({'token': token})


if __name__ == '__main__':
    app.run(debug=True)
