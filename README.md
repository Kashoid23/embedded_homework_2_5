## Обладнання

- **STM32F411CEU6**
- модуль реле **JQC-3F-05VDC-C**
- DC мотор (альтернатива вентилятору)
- діод 1N4007 або 1N5819 паралельно мотору
- окреме джерело **5 В DC**

## Схема підключення

<img width="2720" height="1400" alt="stm32f411_fan_timer_schematic_checked" src="https://github.com/user-attachments/assets/a7252f0a-f5e8-4533-9681-b10ba1751781" />

## Налаштування CubeMX

<img width="1859" height="809" alt="Знімок екрана 2026-11-05 183435" src="https://github.com/user-attachments/assets/4bb4890c-935b-457e-9070-844b14e72b38" />
<img width="1861" height="807" alt="Знімок екрана 2026-11-05 182848" src="https://github.com/user-attachments/assets/f90bb072-4091-4916-96c7-c914071442bd" />
<img width="1860" height="814" alt="Знімок екрана 2026-11-05 183549" src="https://github.com/user-attachments/assets/18b27a52-3a7b-4f06-b616-b3f56f0162cb" />
<img width="1858" height="808" alt="Знімок екрана 2026-11-05 183329" src="https://github.com/user-attachments/assets/045ebdee-8105-404c-b258-20c303cdb706" />
<img width="1859" height="805" alt="Знімок екрана 2026-11-05 182814" src="https://github.com/user-attachments/assets/302c3afd-3a00-4433-bb11-1b80ae0b5914" />
<img width="1863" height="809" alt="Знімок екрана 2026-11-05 183254" src="https://github.com/user-attachments/assets/ab0fe262-95ca-40bb-a232-7e7bd91f5098" />
<img width="1864" height="808" alt="Знімок екрана 2026-11-05 183032" src="https://github.com/user-attachments/assets/0e44f3f9-1555-4abc-82d4-17feca0be307" />
<img width="1861" height="807" alt="Знімок екрана 2026-11-05 182907" src="https://github.com/user-attachments/assets/6a5bbd98-c53e-42a6-8438-6d36572719d8" />

## Як це працює

1. **Частота лічильника.** Таймер рахує з частотою 10 кГц: 100 МГц / (9999 + 1).
2. **Період.** Counter Period дорівнює 1 годині, тобто 36 000 000 тактів. Після цього лічильник починає з нуля.
3. **Імпульс.** Pulse дорівнює 15 хвилинам. Поки лічильник менший за Pulse, PWM-вихід PA1 тримає `LOW`, і реле увімкнене.
4. **Апаратне перемикання.** Реле перемикає сам таймер, програма для цього нічого не робить.
5. **Переривання таймера** лише оновлюють лог і LED:
   - Update (нова година): `onFanTurnedOn()`;
   - Compare CH2 (15 хв минули): `onFanTurnedOff()`.
6. **Головний цикл** тільки оновлює watchdog.

Модуль реле живиться від 5 В і вмикається низьким рівнем:
- `LOW`: PA1 замикає IN на GND, реле вмикається.
- `HIGH`: PA1 відпускає лінію, модуль підтягує її до 5 В, і реле вимикається.

## Розклад

`Core/Inc/Fan.h`:

```c
#define FAN_PERIOD_MS (60u * 60u * 1000u) // fan turns on every hour
#define FAN_ON_MS (15u * 60u * 1000u) // and runs for 15 minutes
```

## Структура коду

| Файл | Вміст |
|---|---|
| `Core/Inc/Fan.h` | макроси розкладу, оголошення функцій |
| `Core/Src/Fan.c` | запуск таймера, лог, LED |
| `Core/Src/main.c` | ініціалізація, запуск `fanStart(&htim2)`, watchdog, callback-и таймера (блоки `USER CODE`) |

## Запуск

1. Відкрити проєкт у STM32CubeIDE.
2. За потреби поставити короткі часи у `Fan.h`.
3. **Project → Build Project**.
4. **Debug**, потім **Resume**, і додати `fanIsOn`, `fanCycles`, `watchdogReset` у Live Expressions.
