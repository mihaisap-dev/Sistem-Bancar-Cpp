# Sistem de Gestiune Bancară (C++)

Acesta este un proiect personal dezvoltat în C++ care simulează operațiunile esențiale ale unui sistem bancar prin intermediul terminalului. Proiectul pune accent pe **securitatea datelor**, **validarea riguroasă a input-ului** și **persistența informațiilor**.

## Funcționalități Principale

* **Gestiune Conturi**: Creare conturi cu ID unic și nume titular.
* **Securitate Avansată**: 
    * Acces protejat prin parolă (suportă litere, cifre și caractere speciale).
    * Ștergere cont securizată prin confirmare dublă (Parolă + Text "DA").
* **Validare Robustă (Anti-Crash)**:
    * Protecție împotriva input-ului de tip text în câmpuri numerice.
    * Verificarea soldului pentru operațiunile de retragere.
* **Persistența Datelor**: Salvare automată și încărcare din fișierul `bank_data.txt`.

---

## Instalare și Rulare

Pentru a compila și rula proiectul pe Mac sau Linux, asigură-te că folosești standardul C++17:

```bash
g++ -std=c++17 Sistem_Bancar.cpp -o Sistem_Bancar && ./Sistem_Bancar