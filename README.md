# SCPI-сервер/клиент

Приложение реализует простой SCPI-сервер и клиент, обменивающиеся данными по TCP/IP.

## Функциональность

### Сервер
- Ожидает TCP-подключения на указанном порту.
- Обрабатывает SCPI-запросы от клиента.

### Клиент
- Подключается к серверу по указанному IP и порту.
- Отправляет SCPI-команды на выбор пользователя.
- Отображает ответы информацию об ответах сервера.


### Поддерживаемые команды
- `SYSTem:STATe?` / `SYST:STAT?` - запрос статуса устройства (bool).
- `MEASure:POINts?` / `MEAS:POIN?` - запрос сведений по измерениям прибора (uint32).
- `MEASure:DATA?` / `MEAS:DATA?` - запрос значений измерений прибора (массив значений uint16).

## Сборка

```bash
mkdir build
cd build
cmake ..
cmake --build . --target client-side # или server-side
```

## Запуск
Сервер:
```bash
./server-side
```

Клиент:
```bash
./client-side </path/to/config.json>
```


Конфиг формата {
"address": <server's IP address in string>,
"port": number
}


# Требования

C++20\
Qt6\
CMake 3.28+

env:
```bash
export CMAKE_PREFIX_PATH=/path/to/Qt/<version>/gcc_64/lib/cmake
```