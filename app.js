const express = require('express');
const bodyParser = require('body-parser');
const { exec } = require('child_process');

const app = express();
const PORT = 3001;

// Middleware для разбора JSON запросов
app.use(bodyParser.json());

app.post('/play', (req, res) => {
    const { ip, port, username, password, file_path } = req.body;

    // Проверка всех необходимых параметров
    if (!ip || !port || !username || !password || !file_path) {
        return res.status(400).send('Missing required fields');
    }

    // Формирование команды для запуска sound_player
    const command = `./run_sound_player.sh ${ip} ${port} ${username} ${password} ${file_path}`;

    // Запуск команды
    exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error executing command: ${error.message}`);
            return res.status(500).send('Command execution failed');
        }

        console.log(`Command output: ${stdout}`);
        console.error(`Command error output: ${stderr}`);
        res.send({ status: 'playing' });
    });
});

// Запуск сервера
app.listen(PORT, () => {
    console.log(`Server is running at http://localhost:${PORT}`);
});