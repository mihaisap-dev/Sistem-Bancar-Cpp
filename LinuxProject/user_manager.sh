#!/bin/bash

user_logat=()

inregistrare_user() {
    read -p "Introduceti numele de utilizator: " username
    if grep -q "^$username," useri.csv 2>/dev/null; then    
        echo "Utilizatorul deja exista!"
        return
    fi

    read -p "Introduceti adresa de email: " email
    if ! echo "$email" | grep -Eq "^[^@]+@[^@]+\.[^@]+$"; then
        echo "Email invalid!"
        return
    fi

    read -s -p "Introduceti parola: " parola
    echo
    if [ -z "$parola" ]; then
        echo "Trebuie introdusa o parola!"
        return
    fi

    # Corectie macOS: shasum in loc de sha256sum
    hash=$(echo -n "$parola" | shasum -a 256 | sed 's/ .*//')
    id=$(uuidgen)
    home_dir="home/$username"
    mkdir -p "$home_dir"
    ultima_log=$(date "+%d-%m-%Y_%H:%M:%S")

    echo "$username,$email,$hash,$id,$ultima_log,$home_dir" >> useri.csv

    echo "Cont creat cu succes pentru $username!"
    echo "Email de confirmare simulat pentru $email"
}

login_user() {
    read -p "Nume utilizator: " username

    linie_user=$(grep "^$username," useri.csv)
    if [ -z "$linie_user" ]; then
        echo "Utilizator inexistent!"
        return
    fi

    ok=false
    for u in "${user_logat[@]}"; do
        if [ "$u" == "$username" ]; then
            ok=true
            break
        fi
    done

    if [ "$ok" = false ]; then
        read -s -p "Parola: " parola
        echo 
        hash_nou=$(echo -n "$parola" | shasum -a 256 | sed 's/ .*//')
        hash_vechi=$(echo "$linie_user" | cut -d "," -f3 | tr -d '[:space:]')

        if [ "$hash_nou" != "$hash_vechi" ]; then
            echo "Parola incorecta."
            return
        fi

        email=$(echo "$linie_user" | cut -d "," -f2)
        home_dir=$(echo "$linie_user" | cut -d "," -f6)
        ultima_log=$(date "+%d-%m-%Y_%H:%M:%S")
        
        # Corectie sed pentru macOS
        sed -i '' -E "s|^($username,[^,]*,[^,]*,[^,]*),[^,]*,(.*)|\1,$ultima_log,\2|" useri.csv

        # Genereaza codul TFA
        cod=$(( RANDOM % 900000 + 100000 ))
        echo "Codul de autentificare (simulat) este: $cod"

        read -p "Introdu codul pentru autentificare: " cod_user
        if [ "$cod_user" != "$cod" ]; then
            echo "Codul introdus este incorect. Autentificare esuata."
            return
        fi

        user_logat+=("$username")
        echo "Autentificare reusita."
    else
        echo "Utilizatorul $username este deja autentificat."
        home_dir=$(echo "$linie_user" | cut -d "," -f6)
    fi

    mkdir -p "$home_dir"
    echo "Intri acum in shell-ul personal din $home_dir."
    echo "Tasteaza 'exit' pentru a reveni la meniu."

    (cd "$home_dir"; bash)
}

logout_user() {
    read -p "Nume utilizator pentru logout: " username
    for i in "${!user_logat[@]}"; do
        if [ "${user_logat[$i]}" == "$username" ]; then
            unset 'user_logat[i]'
            echo "Logout reusit pentru $username."
            return
        fi
    done
    echo "Utilizatorul $username nu este autentificat."
}

generare_raport() {
    read -p "Nume utilizator: " username
    linie_user=$(grep "^$username," useri.csv)
    if [ -z "$linie_user" ]; then
        echo "Utilizator inexistent!"
        return
    fi

    home_dir=$(echo "$linie_user" | cut -d "," -f6)
    cale_raport="$home_dir/raport.txt"

    nr_f=$(find "$home_dir" -type f | wc -l)
    nr_d=$(find "$home_dir" -type d | wc -l)
    dim_disk=$(du -sh "$home_dir" | cut -f1)
    
    {
        echo "Raport pentru utilizatorul $username:" 
        echo "Numar fisiere: $nr_f" 
        echo "Numar directoare: $nr_d" 
        echo "Dimensiune totala: $dim_disk" 
    } > "$cale_raport"
    echo "Raportul a fost generat in $cale_raport."
}

while true; do
    echo ""
    echo "1. Inregistrare"
    echo "2. Login"
    echo "3. Logout"
    echo "4. Genereaza raport"
    echo "5. Lista utilizatorilor autentificati"
    echo "6. Iesire"
    read -p "Alege optiunea: " x

    case $x in
        1) inregistrare_user ;;
        2) login_user ;;
        3) logout_user ;;
        4) generare_raport ;;
        5) 
            if [ ${#user_logat[@]} -eq 0 ]; then
                echo "Niciun utilizator nu este autentificat."
            else
                echo "Utilizatori autentificati: ${user_logat[@]}"
            fi
            ;;
        6) echo "Ai iesit din sistem"; exit;;        
    esac
done
