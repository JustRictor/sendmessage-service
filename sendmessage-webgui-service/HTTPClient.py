import requests
import datetime

class HTTPClient:
    # базовый URL
    URL = 'http://192.168.31.17:5000/'

    admin_token = '17a65071c496aff94e9ae6a296724beb'
    def __init__(self):
        return

    # получаем токен
    def get_token(self):
        token = str(requests.get(f'{self.URL}token').text)

        return token

    # отправляем данные: токен, телефон и код
    def send_data(self, token, phone, msg):
        current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        jsondict = {'phone': phone, 'token':token, 'msg':msg}
        response = requests.post(f'{self.URL}sendMessage', json=jsondict)
        return response, current_time


    def create_token(self):
        # Задайте заголовки запроса
        headers = {
            'Authorization': f'Bearer {self.admin_token}'
        }
        # Выполните GET запрос к серверу
        response = requests.get(f'{self.URL}genToken', headers=headers)

        # Выведите статус-код и содержимое ответа
        print(f'Status Code: {response.status_code}')
        print(f'Response Text:{response.text}')
        return response

    def delete_token(self):
        # Задайте заголовки запроса
        headers = {
            'Authorization': f'Bearer {self.admin_token}'
        }
        # Выполните GET запрос к серверу
        response = requests.delete(f'{self.URL}delToken', headers=headers)

        # Выведите статус-код и содержимое ответа
        print(f'Status Code: {response.status_code}')
        print(f'Response Text:{response.text}')
        return response

    def get_tokens(self):
        # Задайте заголовки запроса
        headers = {
            'Authorization': f'Bearer {self.admin_token}'
        }
        # Выполните GET запрос к серверу
        response = requests.get(f'{self.URL}getTokens', headers=headers)

        # Выведите статус-код и содержимое ответа
        print(f'Status Code: {response.status_code}')
        print(f'Response Text:{response.text}')
        return response
client = HTTPClient()

