# Email-server

## Konfiguracja serwerów
Adresy i porty ustawiane są w pliku `config.h`

## Serwer poczty wychodzącej
`server_outgoing.c` obsługuje maile wychodzące i bazę danych użytkowników

Tworzy 2 wątki:

  - mailowy (`server_outgoing_mail.c`) - odbiera maile od użytkowników, sprawdza ich poprawność i przesyła do serwera incoming

  - użytkowników (`server_outgoing_user.c`) - obsługuje tworzenie nowego użytkownika, logowanie i wylogowywanie (najwyższa warstwa)

    -- Warstwa niższa - `user.c`

    -- Użytkownicy są przechowywani jako struktury `Users` w tablicy `users` (wszyscy użytkownicy) i liście jednokierunkowej `active_users` (zalogowani użytkownicy). Zarządzają nimi funkcje z pliku `usertables.c`.

## Serwer poczty przychodzącej
`server_incoming.c` obsługuje maile przychodzące i przechowuje je.

Tworzy 3 wątki obsługujące sockety:

- mailowy (`server_incoming_mail.c`) odbierający maile od serwera poczty przychodzącej i umieszczający je w skrzynkach odbiorczych poszczególnych użytkowników.

  Skrzynka odbiorcza jest strukturą zadeklarowaną w pliku `mailbox.h`

- ogólnodostępny (`server_incoming_world.c`) na żądanie wysyłający użytkownikom wszystkie ich maile

- wewnętrzny (`server_incoming_inner.c`) do komunikacji między serwerami
