# Принятие решений
Основные архитектурные решения были приняты для возможности дальнейшего **расширения** и для обеспечения **понятности** и **читабельности** кода. Внутренняя реализация должна быть **производительна** и **безопасна**, поэтому прошла несколько итерации и конечный результат был принят в результате **профилирования** и **отладки**.
# Общее описание реализации
- **ParticleSystem** - основной класс системы частиц, служит контейнером для эмиттеров.
- **Particle Emitter** - выпускает частицы  с определённым параметрам(задаются в конструкторе или сеттерами) с помощью метода emit, внутри содержит Pool частиц, разделённый на вектора, каждый вектор обрабатывается в своём
потоке с помощью JobSystem.
- **JobSystem** - класс, реализующий систему работ, при вызове метода execute новая работа заносится в пул и когда освободится какой-либо поток из заранее подготовленных потоков - он начнёт её выполнять <br/>
Ожидание завершения всех активных работ реализовано с помощью метода wait.
- **ParticleRenderer** - отрисовывает частицы, создаётся для каждого ParticleSystem, соответственно в дальнейшем можно по-разному отрисовывать разные системы, доступ к патиклам - только чтение - поэтому не требует синхронизации с WorkerThread .
- **ExplosionEffect** - клиентский код, создание отдельной системы частиц для эффекта цепного взрыва со своими параметрами и коллбеком, привязанный к концу жизни частицы <br/> <br/>
**Input synchronization** - реализация синхронизации input'а и WorkerThread'а выполнена с помощью атомарной переменной.

# Проблемы и дальнейшее развитие
Данные патиклов лучше хранить в одном массиве (или в структуре массивов) в одном месте для обеспечения **cache locality**,
Таким образом легче будет применить к ним **SIMD** для обеспечения параллелизма на уровне данных.
Так как в задаче было указано, что память не должна выделяться, я отказался от использования алгоритма **for_each** с политикой выполнения **std::execution::par**, но он также может обеспечить прирост **производительности**.

# Использованные статьи

https://dev.to/pauljlucas/advanced-thread-safety-in-c-3ap5<br/>
https://vkguide.dev/docs/extra-chapter/multithreading/<br/>
https://yandexwebcache.net/yandbtm?fmode=inject&tm=1712735246&tld=ru&lang=en&la=1712021632&text=wicked+engine+job+system&url=https%3A//wickedengine.net/2018/11/24/simple-job-system-using-standard-c/comment-page-1/&l10n=ru&mime=html&sign=c339071bb14a9f7444fecf6fc0ada675&keyno=0
