# Incoming dispatcher

Получает инфу от анмаршаллера и решает, как исполнять запрос.

Варианты исполнения:

* Приоритетная очередь с разбором одним (thread-unsafe случай) или несколькими (thread-safe, but resource-demanding
  случай) исполнителями
* Прямое создание потока для исполнения (thread-safe и resource-light случай)
* др.?

# Outgoing dispatcher

Получает запрос к внешнему сервису

1. Находит внешний сервис (service lookup, если необходимо)
2. Сопоставляет запросу подходящую пару маршаллер-транспорт, на основе приоритетного списка потенциально подходящих пар

Приоритеты пар строятся на основе предзаданных приоритетных списков:

1. Расположение вызываемого сервиса (локально, в локальной сети, во внешней сети)
2. Транспортный протокол, по которому готов общаться вызываемый сервис (по мотивам надёжности и скорости взаимодействия)
3.

# Service lookup

Сначала ищем среди локальных сервисов, потом на общем лукапе