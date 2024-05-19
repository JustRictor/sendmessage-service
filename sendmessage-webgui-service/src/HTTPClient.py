import requests
import datetime

import Logger
class HTTPClient:
    # базовый URL
    URL = 'http://sendmessage-service-container:5000/'

    admin_token = '66c245eb9cf96e2cb886aa112745cb1ba0917fe8'
    def __init__(self):
        return

    # получаем токен
    def get_token(self):
        token = str(requests.get(f'{self.URL}token').text)
        return token

    # отправляем данные: токен, телефон и код
    def send_data(self,phone, msg):
        headers = {
            'Authorization': f'Bearer {self.admin_token}'
        }
        current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        jsondict = {'phone': phone, 'msg':msg}
        response = requests.post(f'{self.URL}sendMessage', json=jsondict, headers=headers)
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
        return response.text

    def delete_token(self,token):
        # Задайте заголовки запроса
        headers = {
            'Authorization': f'Bearer {self.admin_token}'
        }
        # Задайте заголовки запроса
        # Выполните GET запрос к серверу
        print(token)
        print(f'{self.URL}delToken/{token}')
        response = requests.delete(f'{self.URL}delToken/{token}', headers=headers)
        print(response)
        # Выведите статус-код и содержимое ответа
        #Logger.logger.write_log(f'удален токен: {self.admin_token}')
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

