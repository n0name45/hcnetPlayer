# hcnetPlayer
Service for playing audio files via Hikvision SDK


RUS

Сервис для проигрывания аудио файлов на камерах и домофонах через Hikvision API.

Источник тут http://itunedvr.ru/forum/phpBB3/viewtopic.php?t=1118
Код адаптирован под запуск из linux и упакован в контейнер docker.

## Установка и запуск
git clone https://github.com/n0name45/hcnetPlayer.git
cd ./hcnetPlayer
docker-compose up -d

## Подготовка аудиофайлов
Файлы в формате ulaw складывать в папку wav
Команда для перекодировки

ffmpeg -y -i ./sound.mp3 -ar 8000 -f mulaw -ac 1 ./output.wav

## Играем музыку
curl -X POST http://IP:3001/play -H "Content-Type: application/json" -d '{"ip": "192.168.0.101", "port": "8000", "username": "admin",    "password": "password", "file_path": "wav/output.wav"}'

Вместо IP - адрес сервера своего, на котором сервис запущен
Вместо 192.168.0.101 - адрес камеры или домона
admin - пользователь на hikvision
password - пароль пользователя

