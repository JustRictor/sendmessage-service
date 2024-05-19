import requests
import datetime

class HTTPClient:
    # базовый URL
    URL = 'http://192.168.31.17:5000/'
    def __init__(self):
        return

    # получаем токен
    def get_token(self):
        token = str(requests.get(f'{self.URL}token').text)
        return token

    # отправляем данные: токен, телефон и код
    def send_data(self, token, phone, msg):
        current_time = datetime.datetime.now()
        jsondict = {'phone': phone, 'token':token, 'msg':msg}
        response = requests.post(self.URL + 'sendMessage', json=jsondict)
        return response, current_time

client = HTTPClient()

