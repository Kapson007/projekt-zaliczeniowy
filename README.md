# Kontrola Mocy Karty Bezprzewodowej

## Opis projektu
Projekt Kontrola Mocy Karty Bezprzewodowej to program w języku C, który umożliwia zmianę mocy karty interfejsu sieciowego przy użyciu funkcji `ioctl`. Zapewnia prosty interfejs do regulacji poziomu mocy karty bezprzewodowej, co może być przydatne w sytuacjach, gdzie chcemy zoptymalizować wydajność sieci bezprzewodowej lub oszczędzać energię baterii.

## Funkcje
Użytkownik ma do wyboru dwa tryby ustawiania mocy:
- tryb ciągły - funkcja `setIWPower`, która ustawia na stałe odpowiedni poziom mocy w [dBm]
- tryb adaptacyjny - funkcja `adaptiveMode` prosi użytkownika o pożądany minimalny poziom mocy, a następnie co kilka sekund sprawdza aktualny poziom mocy punktu dostępu dostosowując moc karty do wymagań użytkownika, jeżeli poziom mocy spadnie poniżej oczekiwanego progu.
Dodatkowo program wyposażony jest w funkcje `checkAllowedPower` sprawdzającą czy wartośc wprowadzona przez użytkownika jest legalna na terenie Rzeczypospolitej oraz funkcje `getSSID` i `getIWFrequency`, które zwracają następujące paramertry na podstawie nazwy interfejsu sieciowego:
- nazwa(identyfikator) sieci - SSID,
- aktualna częstotliwośc, na której bezprzewodowy interfejs pracuje.

## Technologia
Projekt został zaimplementowany w języku C i wykorzystuje wywołanie systemowe `ioctl`, aby komunikować się z sterownikiem karty bezprzewodowej. Wykorzystuje możliwości dostarczane przez system operacyjny w celu modyfikacji ustawień mocy karty bezprzewodowej.

## Struktura plików
Projekt składa się z następującego pliku:

- `setIWower.c`: Ten plik zawiera implementację głównego programu w języku C do zmiany mocy karty IW przy użyciu `ioctl`.

## Kompilacja i uruchomienie
Aby skompilować i uruchomić program, postępuj zgodnie z poniższymi krokami:

1. Upewnij się, że masz zainstalowany kompilator C na swoim systemie (np. GCC).
2. Otwórz terminal lub wiersz polecenia i przejdź do katalogu, w którym znajduje się plik `iw_power_control.c`.
3. Użyj następującej komendy, aby skompilować program:
    `gcc setIWpower.c -o setIWpower`
4. Po pomyślnym skompilowaniu zostanie wygenerowany plik wykonywalny o nazwie `setIWpower`.
5. Uruchom program za pomocą poniższej komendy: `./iw_power_control <interface name>` gdzie `<interface name>` jest nazwą bezprzewodowej karty sieciowej.
To spowoduje wykonanie programu i wyświetlenie dostępnych opcji zmiany mocy karty bezprzewodowej.
6. Postępuj zgodnie z instrukcjami wyświetlanymi na ekranie, aby wybrać pożądany poziom mocy i zastosować zmiany.

Należy pamiętać, że ten program może wymagać uprawnień superużytkownika (roota) do uzyskania dostępu do odpowiednich poleceń IOCTL i modyfikacji ustawień mocy karty bezprzewodowej. Upewnij się, że uruchamiasz program z odpowiednimi uprawnieniami lub użyj polecenia `sudo`, aby uruchomić go jako superużytkownik.

Pamiętaj, aby ostrożnie modyfikować ustawienia mocy karty bezprzewodowej, ponieważ niewłaściwe dostosowania mogą
