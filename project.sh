opt=0
while [ $opt -ne 1 ]
do
	echo "1. Iesire"
	echo "2. Afiseaza informatii masina"
	echo "3. Afiseaza data in format UTC"
	echo "4. Afiseaza primele 2 shells instalate in sistem"
	echo "5. Afiseaza nume login a utilizatorilor + directorul de home"
	echo "6. Afisare ultimele linii din /etc/protocols"
	echo "7. Afisare tip procesor si frecventa sa de rulare"
	echo "8. Afisare nume directoare din /proc care reprezinta imaginea unui proces pe disc"
	echo "9. Afisare si cautare in logs"
	read opt
	case $opt in
		1) echo "***** ATI PARASIT MENIUL *****";;
		2) #Functioneaza doar cu bash, nu cu sh 
		echo "---------------- INFORMATII ----------------";
		echo "Numele serverului: " $HOSTNAME;	
		echo "Tipul masinii: " $HOSTTYPE;
		echo "Tipul OS-ului:" $OSTYPE;
		echo "Calea curenta: $(pwd)";
		echo "--------------------------------------------";;
		3) 
		echo "---------------- DATA FORMAT UTC ----------------";
		echo "Data:" $(date --utc);
		echo "-------------------------------------------------";;
		4) 
		echo "---------------- PRIMELE 2 SHELLS ----------------";
		echo "$(awk 'NR == 2 || NR == 3 { print $0 }' /etc/shells)";
		echo "--------------------------------------------------";;
		5) 
		echo "---------------- NUME USERS & DIR HOME ----------------";
		echo "$(awk -F : '{ print $1, $6 }' /etc/passwd)";
		echo "-------------------------------------------------------";;
		6) 
		echo "---------------- ULTIMELE LINII DIN /ETC/PROTOCOLS ----------------";
		echo "$(tail /etc/protocols -n +9)";
		echo "-------------------------------------------------------------------";;
		7) 
		echo "---------------- MODEL PROC & FREQ ----------------";
		echo "$(awk 'NR == 5 || NR == 8 { print $0 }' /proc/cpuinfo )";
		echo "---------------------------------------------------";;
		8) 
		echo "---------------- DIRs /proc PENTRU IMAG PROCES PE DISC ----------------";
		ls -l /proc/ | grep ^d | awk '{ print $9 }' > temp.txt;
		cat temp.txt | grep ^[0-9];
		rm temp.txt;
		echo "-----------------------------------------------------------------------";;
		9) 
		echo "---------------- FISIERE LOG ARHIVATE ----------------";
		ls -l /var/log | grep \\.gz$ | awk '{ print $9 }';
		echo "------------------------------------------------------";
		echo "";
		echo "---------------- TOATE FISIERELE DIN logs & OTHER STUFFS ----------------";
		ls -l /var/log | grep ^- | awk '{ print $9 }' > temp.txt;
		cat temp.txt;
		echo "";
		echo "Cititi un nume de fisier: ";
		read fisier;
		ok=0;
		for i in $(cat temp.txt)
		do
			if [ $fisier = $i ]
			then 
				ok=1;
				echo "";
				echo "Cititi un text ce doriti sa-l cautati: ";
				read text;
				if [ ${fisier: -3} == ".gz" ] #{nume: -n} da numai ultimele n caractere din nume
				then
					gunzip -c /var/log/$fisier > temp.txt;
					grep -i $text temp.txt;
				else
					grep -i $text /var/log/$fisier;
				fi				
			fi
		done
		rm temp.txt;
		if [ $ok -eq 0 ]
		then 
			echo "";
			echo "NU S-A GASIT FISIERUL CAUTAT";
		fi
		echo "--------------------------------------------------------------------------";;
	esac
done
