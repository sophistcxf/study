select i in www groad net exit
do
  case $i in
    www) echo "1-www";;
    groad) echo "2-groad";;
    net) echo "3-net";;
    exit) exit 0;;
  esac
done
