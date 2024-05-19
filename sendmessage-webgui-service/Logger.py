class Logger:
    # путь к файлу записи
    PATH = 'log-data/session_log.txt'
    def __init__(self):
        return

    def write_log(self,data):
        try:
            with open(self.PATH, 'a') as file:  # Открываем файл в режиме добавления (append)
                file.write(data)  # Добавляем запись в файл с новой строки
        except Exception as e:
            print(f"Произошла ошибка при добавлении записи в файл: {e}")

logger = Logger()