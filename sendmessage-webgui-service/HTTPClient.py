import requests
import datetime

class HTTPClient:
    log = []

    URL = 'http://192.168.31.17:5000/'
    def __init__(self, phone, msg, token):
        self.phone = phone
        self.msg = msg
        self.token = token

    def get_token(self):
        current_time = datetime.datetime.now()

        self.token = str(requests.get(f'{self.URL}').text)
        self.log.append([self.token, current_time])
        print(self.token)


    def send_data(self):
        self.get_token()
        r = requests.post(f'{self.URL}get-api',data={'token': self.token, 'phone':self.phone,'msg':self.msg})
        print(r.text)

    def get_log(self):
        return self.log



client = HTTPClient("890394935", "1234","Rfudrb#jfj")
client.get_token()
#client.send_data()

